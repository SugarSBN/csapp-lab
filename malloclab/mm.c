/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "Neko",
    /* First member's full name */
    "SuBonan",
    /* First member's email address */
    "bokkenasu@163.com",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

/* Basic constants and macros */
#define WSIZE 4
#define DSIZE 8
#define CHUNKSIZE (1 << 12) /* Extend heap by this amount (bytes) */

#define MAX(x, y) ((x) > (y) ? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)      (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block pointer bp, compute address of its header and footer */
#define HDRP(bp)    ((char *)(bp) - WSIZE)                        /* header| Block(pointer here) | Block | footer */
                                                                  /* GET_SIZE(HDRP(bp)) = 5 * WSIZE */
#define FTRP(bp)    ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)   /* header| Block(pointer here) | Block | footer */
                                                                  /* GET_SIZE(HDRP(bp)) = 5 * WSIZE */

/* Given block pointer bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp)   ((char *)(bp) + GET_SIZE((char *)(bp) - WSIZE))    /* header | Block(pointer here) | Block | footer | header | Block */
                                                                           /* GET_SIZE = Size of the current block */
#define PREV_BLKP(bp)   ((char *)(bp) - GET_SIZE((char *)(bp) - DSIZE))    /* Block | footer | header | Block(pointer here) | Block | footer */
                                                                           /* GET_SIZE = Size of the previous block */
/*
 * The smallest blank block is 6 WSIZE:
 * +------------+------------+------------+-----------+------------+-----------+
 * |    size    | left child |   Value    |    father | right child|   size    |
 * |  4 bytes   |  4 bytes   |  4 bytes   |   4 bytes |  4 bytes   |  4 bytes  |
 * +------------+------------+------------+-----------+------------+-----------+  value = Value
 * Or:
 * +------------+------------+------------+-----------+------------+------------+------------+-----------+
 * |    size    | left child |   Value    |   Block   |   Block    |    father  |right child |   size    |
 * |  4 bytes   |  4 bytes   |  4 bytes   |   4 bytes |  4 bytes   |  4 bytes   |  4 bytes   |   4 bytes |
 * +------------+------------+------------+-----------+------------+------------+------------+-----------+ value = Value
 */
#define LEFT(bp)            (bp)
#define RIGHT(bp)           (FTRP(bp) - WSIZE)
#define FATHER(bp)          (FTRP(bp) - 2 * WSIZE)
#define CHILD(bp, a)        ((a) == 0 ? LEFT(bp) : RIGHT(bp))
#define VALUE(bp)           (bp + WSIZE)
#define GET_LEFT(bp)        GET(LEFT(bp))                     /* The address of left child */
#define GET_RIGHT(bp)       GET(RIGHT(bp))                    /* The address of right child */
#define GET_FATHER(bp)      GET(FATHER(bp))                   /* The address of father */
#define GET_CHILD(bp, a)    GET(CHILD(bp, a))
#define GET_VALUE(bp)       GET(VALUE(bp)) /* The value of the treap node */

void *heap_listp;
void *treap;

static int check_tree(void *bp, void *bp1){
    if (bp == bp1)  return 1;
    if (bp == NULL) return 0;
    if (check_tree(GET_LEFT(bp), bp1))  return 1;
    if (check_tree(GET_RIGHT(bp), bp1)) return 1;
    return 0;
}

static void print_block(void *bp){
    if (GET_ALLOC(HDRP(bp)))    return;
    printf("Addr=%p size=%p Alloc=%p\n", bp, GET_SIZE(HDRP(bp)), GET_ALLOC(HDRP(bp)));
    if (!GET_ALLOC(HDRP(bp))){
        printf("Left=%p Right=%p Father=%p Value=%p\n", GET_LEFT(bp), GET_RIGHT(bp), GET_FATHER(bp), GET_VALUE(bp));
    }
    printf("\n");
}

void print_heap(){
    
    printf("----------------------------------------------\n");
    for (void *bp = heap_listp;GET_SIZE(HDRP(bp));bp = NEXT_BLKP(bp)){
        print_block(bp);
    }
    printf("treap=%p\n", treap);
    printf("----------------------------------------------\n");
    
}


void rotate(void *bp){
    void *fa = GET_FATHER(bp);
    if (fa == NULL) return;
    int ch = (GET_RIGHT(fa) == bp);
    void *grand = GET_FATHER(fa);
    PUT(FATHER(bp), grand);
    if (grand != NULL)
        PUT(CHILD(grand, (GET_RIGHT(grand) == fa)), bp);
    PUT(CHILD(fa, ch), GET_CHILD(bp, (ch ^ 1)));
    if (GET_CHILD(bp, (ch ^ 1)) != NULL) 
        PUT(FATHER(GET_CHILD(bp, (ch ^ 1))), fa);
    PUT(CHILD(bp, (ch ^ 1)), fa);
    PUT(FATHER(fa), bp);
    if (fa == treap)    treap = bp;
}

void maintain(void *bp){
    void *fa = GET_FATHER(bp);
    while ((fa = GET_FATHER(bp)) != NULL && GET_VALUE(fa) < GET_VALUE(bp)){
        //printf("maintain:\n");
        //print_block(fa);
        //print_block(bp);
        rotate(bp);
    }
}

void insert_node(void *bp){
    int value;
    value = rand() + 1;
    PUT(VALUE(bp), value);

    PUT(LEFT(bp), NULL);
    PUT(RIGHT(bp), NULL);
    PUT(FATHER(bp), NULL);
    void *p = treap;
    if (p == NULL)  {treap = bp;    return bp;}
    while(1){
        int ch = GET_SIZE(HDRP(bp)) > GET_SIZE(HDRP(p));
        if (GET_CHILD(p, ch) == NULL) {
            PUT(CHILD(p, ch), bp);
            PUT(FATHER(bp), p);
            maintain(bp);
            return;
        } else p = GET_CHILD(p, ch);
    }
}

void remove_node(void *bp){
    int exists = check_tree(treap, bp);
    while(1){    
        if (GET_LEFT(bp) == NULL && GET_RIGHT(bp) == NULL){
            void *fa = GET_FATHER(bp);
            if (fa != NULL){
                int ch = (GET_RIGHT(fa) == bp);
                PUT(CHILD(fa, ch), NULL);
            }
            PUT(FATHER(bp), 0);
            if (bp == treap)   treap = NULL;
            return;
        }
        int leftvalue = GET_LEFT(bp) == NULL ? 0x80000000 : (GET_VALUE(GET_LEFT(bp)));
        int rightvalue = GET_RIGHT(bp) == NULL ? 0x80000000 : (GET_VALUE(GET_RIGHT(bp)));
        rotate(GET_CHILD(bp, rightvalue > leftvalue));
    }
}

void *query_node(void *bp, size_t size){
    if (bp == NULL) return NULL;
    if (GET_SIZE(HDRP(bp)) >= size) {
        void *left = query_node(GET_LEFT(bp), size);
        if (left == NULL)   return bp;
        else return left;
    } else {
        void *right = query_node(GET_RIGHT(bp), size);
        return right;
    }
}


static void *coalesce(void *bp){
    if (GET_SIZE(HDRP(bp)) == 0)    return bp;
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));
    void *prev = PREV_BLKP(bp), *next = NEXT_BLKP(bp);
    //printf("before coalesce bp=%p prev_alloc=%p next_alloc=%p prev=%p next=%p\n", bp, prev_alloc, next_alloc, prev, next);
    //print_heap();
    if (GET_SIZE(HDRP(bp)) > 2 * DSIZE)   remove_node(bp);
    //printf("rua!\n");
    if (prev_alloc && next_alloc){  
        insert_node(bp);
    } else
    if (prev_alloc && !next_alloc){
        if (GET_SIZE(HDRP(NEXT_BLKP(bp))) > 2 * DSIZE) remove_node(next);
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0)); /* 注意顺序，要先动header，这样FTRP就会找到下一个Block的footer了，就合并了 */
        insert_node(bp);
    } else
    if (!prev_alloc && next_alloc){
        if (GET_SIZE(HDRP(PREV_BLKP(bp))) > 2 * DSIZE) remove_node(prev);
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
        insert_node(bp);
    } else
    if (!prev_alloc && !next_alloc){
        if (GET_SIZE(HDRP(PREV_BLKP(bp))) > 2 * DSIZE) remove_node(prev);
        if (GET_SIZE(HDRP(NEXT_BLKP(bp))) > 2 * DSIZE) remove_node(next);
        size += GET_SIZE(HDRP(NEXT_BLKP(bp))) + GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
        insert_node(bp);
    }
    //printf("After coalsce bp=%p prev_alloc=%p next_alloc=%p prev=%p next=%p\n", bp, prev_alloc, next_alloc, prev, next);
    //print_heap();
    return bp;
}

static void *extend_heap(size_t words){
    //printf("word=%p\n", words);
    char *bp;
    size_t size;
    size = ((words & 1) ? words + 1 : words) * WSIZE;
    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;
    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));
    /* | 0 | header | Block | Block | Block | Block | footer | (0, 1) | */
    insert_node(bp);
    bp = coalesce(bp);
    return bp;
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *)-1)  
        return -1;
    PUT(heap_listp, 0);                             /* Alignment padding */
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));  /* Prologue header */
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));  /* Prologue footer */
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1));      /* Epilogue header */
    heap_listp += (2 * WSIZE);
    /* Create the initial empty heap */
    /* | 0 | header | (heap_listp here) footer | (0,1) | */
    treap = NULL;
    if (extend_heap(CHUNKSIZE / WSIZE) == NULL)
        return -1;
    return 0;
}

static void *find_fit(size_t asize){
    return query_node(treap, asize);
}

static void* place(void *bp, size_t asize){
    size_t csize = GET_SIZE(HDRP(bp));
    void *res = bp;
    remove_node(bp);
    //printf("before place:\n");
    //print_heap();
    if ((csize - asize) >= 2 * DSIZE) {
        if (asize >= 100){
            PUT(HDRP(bp), PACK(csize - asize, 0));
            PUT(FTRP(bp), PACK(csize - asize, 0));
            if (csize - asize > 2 * DSIZE) insert_node(bp);
            bp = NEXT_BLKP(bp);
            PUT(HDRP(bp), PACK(asize, 1));
            PUT(FTRP(bp), PACK(asize, 1));
            return bp;
        }else{
            PUT(HDRP(bp), PACK(asize, 1));
            PUT(FTRP(bp), PACK(asize, 1));
            bp = NEXT_BLKP(bp);
            PUT(HDRP(bp), PACK(csize - asize, 0));
            PUT(FTRP(bp), PACK(csize - asize, 0));
            if (csize - asize > 2 * DSIZE) insert_node(bp);
            return res;
        }
    } else {
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
        return bp;
    }
}
/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize;       /* Adjust block size */
    size_t extendsize;
    char *bp;
    if (size == 0)  return NULL;
    asize = ALIGN(size + 2 * WSIZE);
    //printf("malloc: %p\n", asize);
    //print_heap();
    //print_tree(treap);
    if ((bp = find_fit(asize)) != NULL){
        //printf("malloc find = %p\n", bp);
        bp = place(bp, asize);
        //printf("After malloc");
        //print_heap();
        return bp;
    }
    
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize / WSIZE)) == NULL)
        return NULL;
    bp = coalesce(bp);
    //printf("malloc find = %p\n", bp);
    bp = place(bp, asize);
    //printf("After malloc");
    //print_heap();
    //print_tree(treap);
    return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    //printf("\nfree =%p\n", ptr);
    //print_heap();
    //print_tree(treap);
    size_t size = GET_SIZE(HDRP(ptr));
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    if (size <= 2 * DSIZE)  {
        coalesce(ptr);
        return;
    }
    insert_node(ptr);
    //printf("after free:\n");
    //print_heap();
    //print_tree(treap);
    coalesce(ptr);

    //printf("after free add = %p", ptr);
    //print_heap();
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
    size_t newsize = ALIGN(size + 2 * WSIZE);
    if (GET_SIZE(HDRP(ptr)) >= newsize)    return ptr;
    if (!GET_ALLOC(HDRP(NEXT_BLKP(ptr))) && GET_SIZE(HDRP(ptr)) + GET_SIZE(HDRP(NEXT_BLKP(ptr))) >= newsize){
        size_t csize = GET_SIZE(HDRP(ptr)) + GET_SIZE(HDRP(NEXT_BLKP(ptr))) - newsize;
        //printf("\n%p %d\n", NEXT_BLKP(ptr), check_tree(treap, NEXT_BLKP(ptr)));
        remove_node(NEXT_BLKP(ptr));
        PUT(HDRP(ptr), PACK(GET_SIZE(HDRP(ptr)) + GET_SIZE(HDRP(NEXT_BLKP(ptr))), 1));
        PUT(FTRP(ptr), PACK(GET_SIZE(HDRP(ptr)), 1));
        return;
    }
    void *newptr = mm_malloc(size);
    memcpy(newptr, ptr, GET_SIZE(HDRP(ptr)) - 2 * WSIZE);
    mm_free(ptr);
    return newptr;
}














