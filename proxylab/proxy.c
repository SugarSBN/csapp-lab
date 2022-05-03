/*
 * @Author: SuBonan
 * @Date: 2022-05-01 17:57:12
 * @LastEditTime: 2022-05-03 09:24:45
 * @FilePath: \proxylab\proxy.c
 * @Github: https://github.com/SugarSBN
 * これなに、これなに、これない、これなに、これなに、これなに、ねこ！ヾ(*´∀｀*)ﾉ
 */
#include <stdio.h>
#include "csapp.h"

/* Recommended max cache and object sizes */
#define MAX_CACHE_SIZE 1049000
#define MAX_OBJECT_SIZE 102400

/* You won't lose style points for including this long line in your code */
static const char *user_agent_hdr = "User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:10.0.3) Gecko/20120305 Firefox/10.0.3\r\n";
static const char *conn_hdr = "Connection: close\r\n";
static const char *prox_hdr = "Proxy-Connection: close\r\n";

void doit(int clientfd);
int parse_uri(char *uri, char *hostname, char *port, char *filepath);
void read_build_requesthdrs(rio_t *rp, char *headers); 
void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg); 

int main(int argc, char **argv) 
{
    int listenfd, connfd;
    char hostname[MAXLINE], port[MAXLINE];
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;

    /* Check command line args */
    if (argc != 2) {
	    fprintf(stderr, "usage: %s <port>\n", argv[0]);
	    exit(1);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1) {
	    clientlen = sizeof(clientaddr);
	    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen); //line:netp:tiny:accept
        Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE, 
                    port, MAXLINE, 0);
        printf("Proxy accepted connection from (%s, %s)\n", hostname, port);
	    doit(connfd);                                             //line:netp:tiny:doit
	    Close(connfd);                                            //line:netp:tiny:close
    }
}

void doit(int clientfd) 
{
    struct stat sbuf;
    char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char hostname[MAXLINE], port[MAXLINE], filepath[MAXLINE];
    rio_t rio;

    /* Read request line and headers */
    Rio_readinitb(&rio, clientfd);
    if (!Rio_readlineb(&rio, buf, MAXLINE))  //line:netp:doit:readrequest
        return;
    printf("%s", buf);
    sscanf(buf, "%s %s %s", method, uri, version);       //line:netp:doit:parserequest
    if (strcasecmp(method, "GET")) {                     //line:netp:doit:beginrequesterr
        clienterror(clientfd, method, "501", "Not Implemented",
                    "Tiny does not implement this method");
        return;
    }                                                    //line:netp:doit:endrequesterr

    char headers[MAXLINE];
    read_build_requesthdrs(&rio, headers);
    printf("Method: %s uri: %s version: %s Headers: %s", method, uri, version, headers);
    
    /* Parse URI from GET request */
    parse_uri(uri, hostname, port, filepath);       //line:netp:doit:staticcheck
    printf("%s\n%s\n", hostname, port);
    int serverfd = Open_clientfd(hostname, port);
    char to_server[MAXLINE], to_client[MAXLINE];
    
    sprintf(to_server, "GET %s HTTP/1.0\r\n%s", filepath, headers);
    Rio_writen(serverfd, to_server, strlen(to_server));
    Rio_readinitb(&rio, serverfd);    
    int n;
    while ((n = Rio_readlineb(&rio, to_client, MAXLINE)))
        Rio_writen(clientfd, to_client, n);
}

void read_build_requesthdrs(rio_t *rp, char *headers) 
{
    char buf[MAXLINE];
    Rio_readlineb(rp, buf, MAXLINE);
    strcat(headers, buf);
    while(strcmp(buf, "\r\n")) {          //line:netp:readhdrs:checkterm
	    Rio_readlineb(rp, buf, MAXLINE);
        strcat(headers, buf);
    }
    headers[strlen(headers) - 1] = '\0';
    strcat(headers, user_agent_hdr);
    strcat(headers, conn_hdr);
    strcat(headers, prox_hdr);
    strcat(headers, "\r\n");
    return;
}



/* $begin parse_uri */
int parse_uri(char *uri, char *hostname, char *port, char *filepath) 
{
    hostname[0] = '\0';
    port[0] = '\0';
    filepath[0] = '\0';
    strncat(port, "80", 2);
    if (!strncmp(uri, "http://", 7))    uri = uri + 7;
    int len = strlen(uri);
    for (int i = 0;i < len;i++)
        if (uri[i] == ':' || uri[i] == '/') {
            strncat(hostname, uri, i);
            strncat(filepath, uri + i, len - i);
            break;
        }
 
    for (int i = 0;i < len;i++) {
       if (uri[i] == ':') {
            for (int j = i + 1;j < len;j++)
                if (uri[j] < '0' || uri[j] > '9'){
                    port[0] = '\0';
                    strncat(port, uri + i + 1, j - i - 1);
                    filepath[0] = '\0';
                    strncat(filepath, uri + j, len - j);
                    break;
                }
        }
    }
    return 0;
}
/* $end parse_uri */

void proxyerror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg) 
{
    char buf[MAXLINE];

    /* Print the HTTP response headers */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Tiny Error</title>");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Tiny Web server</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}

void clienterror(int fd, char *cause, char *errnum, 
		 char *shortmsg, char *longmsg) 
{
    char buf[MAXLINE];

    /* Print the HTTP response headers */
    sprintf(buf, "HTTP/1.0 %s %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Content-type: text/html\r\n\r\n");
    Rio_writen(fd, buf, strlen(buf));

    /* Print the HTTP response body */
    sprintf(buf, "<html><title>Tiny Error</title>");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<body bgcolor=""ffffff"">\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "%s: %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<p>%s: %s\r\n", longmsg, cause);
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "<hr><em>The Tiny Web server</em>\r\n");
    Rio_writen(fd, buf, strlen(buf));
}