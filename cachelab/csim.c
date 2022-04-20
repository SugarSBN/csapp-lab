/*
 * @Author: SuBonan
 * @Date: 2022-04-20 08:34:41
 * @LastEditTime: 2022-04-20 11:43:14
 * @FilePath: \cachelab\csim.c
 * @Github: https://github.com/SugarSBN
 * これなに、これなに、これない、これなに、これなに、これなに、ねこ！ヾ(*´∀｀*)ﾉ
 */
#include "cachelab.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_help(){
    printf("Usage: ./csim [-hv] -s <num> -E <num> -b <num> -t <file>\n");
    printf("Options:\n");
    printf("  -h         Print this help message.\n");
    printf("  -v         Optional verbose flag.\n");
    printf("  -s <num>   Number of set index bits.\n");
    printf("  -E <num>   Number of lines per set.\n");
    printf("  -b <num>   Number of block offset bits.\n");
    printf("  -t <file>  Trace file.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  linux>  ./csim -s 4 -E 1 -b 4 -t traces/yi.trace\n");
    printf("  linux>  ./csim -v -s 8 -E 2 -b 4 -t traces/yi.trace\n");
}

int s, E, b, display;
char *filename;
int arg_parsor(int argc, char *argv[]){
    display = 0;
    s = -1; E = -1; b = -1;
    for (int i = 1;i < argc;){
        if (!strcmp(argv[i], "-h"))   {
            print_help();
            return 0;
        } else
        if (!strcmp(argv[i], "-v"))  {
            display = 1;
            i++;
        } else
        if (!strcmp(argv[i], "-s")){
            s = atoi(argv[i + 1]);
            if (!s && *argv[i + 1] != '0') goto error;
            i += 2;
        } else
        if (!strcmp(argv[i], "-E")){
            E = atoi(argv[i + 1]);
            if (!E && *argv[i + 1] != '0') goto error;
            i += 2;
        } else
        if (!strcmp(argv[i], "-b")){
            b = atoi(argv[i + 1]);
            if (!b && *argv[i + 1] != '0') goto error;
            i += 2;
        } else
        if (!strcmp(argv[i], "-t")){
            filename = argv[i + 1];
            i += 2;
        } else goto error;
    }
    if (s == -1 || E == -1 || b == -1 || !filename)  goto error;
    return 1;
error:
    printf("Wrong arguments!\n");
    print_help();
    return 0;
}

#define get_t(x) ((x) >> (s + b))
#define get_s(x) (((x) >> b) & ((1 << s) - 1))
#define get_b(x) ((x) & ((1 << b) - 1))

typedef struct {
    int valid;
    unsigned long int tag;
    char *val; // 1 byte
    int timestamp;
} Line;

typedef Line* Set;
typedef Set* Cache;
int timestamp;

Cache init_cache(int s, int E, int b){
    int S = (1 << s);
    Cache cache = malloc(S * sizeof(Set));
    for (int i = 0;i < S;i++)   {
        cache[i] = malloc(E * sizeof(Line));
        for (int j = 0;j < E;j++)   {
            cache[i][j].val = malloc((1 << b) * sizeof(char));
            cache[i][j].valid = 0;
        }
    }
    return cache;
}
int query_cache(Cache cache, unsigned long int addr){  // 1=hit, 0=miss, 2=miss and evict
    timestamp++;
    unsigned long int i = get_s(addr);
    for (int j = 0;j < E;j++)   if (cache[i][j].valid == 1 && cache[i][j].tag == get_t(addr))   {
        cache[i][j].timestamp = timestamp;
        return 1;
    }
    for (int j = 0;j < E;j++)   if (cache[i][j].valid == 0) {
        cache[i][j].valid = 1;
        cache[i][j].timestamp = timestamp;
        cache[i][j].tag = get_t(addr);
        return 0;
    }
    int min_timestamp, minj;
    min_timestamp = 0x3fffffff;
    for (int j = 0;j < E;j++)   if (cache[i][j].timestamp <= min_timestamp){
        min_timestamp = cache[i][j].timestamp;
        minj = j;
    }
    cache[i][minj].timestamp = timestamp;
    cache[i][minj].tag = get_t(addr);
    return 2;
}

unsigned long int parseNum(char *c){
    unsigned long int res = 0;
    for (int i = 0;;i++){
        if (('0' <= c[i] && c[i] <= '9') || ('a' <= c[i] && c[i] <= 'f'))
            res = (res << 4) + (('0' <= c[i] && c[i] <= '9') ? c[i] - '0' : 10 + c[i] - 'a');
        if (c[i] == ',')    return res;
    }
    return res;
}

int nhits, nmisses, nevicts;
char *info[3] = {"miss", "hit", "miss eviction"};

int main(int argc, char *argv[]){
    if (!arg_parsor(argc, argv))    return 0;
    FILE *fp = fopen(filename, "r");
    char tmp[1024];
    Cache cache = init_cache(s, E, b);
    nhits = nmisses = nevicts = 0;    
    while(fgets(tmp, 1024, fp)) {
        tmp[strlen(tmp) - 1] = '\0';
        if (tmp[1] != 'L' && tmp[1] != 'S' && tmp[1] != 'M')    continue;
        if (tmp[1] == 'L' || tmp[1] == 'S'){
            int que = query_cache(cache, parseNum(tmp + 1));
            if (display)    printf("%s %s\n", tmp, info[que]);
            if (que == 0)   nmisses++;
            if (que == 1)   nhits++;
            if (que == 2)   {nmisses++; nevicts++;}
        } else {
            int que = query_cache(cache, parseNum(tmp + 1));
            if (display)    printf("%s %s hit\n", tmp, info[que]);
            if (que == 0)   nmisses++;
            if (que == 1)   nhits++;
            if (que == 2)   {nmisses++; nevicts++;}
            nhits++;
        }
    }
    printSummary(nhits, nmisses, nevicts);
    return 0;
}
