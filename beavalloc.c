// R. Jesse Chaney
// chaneyr@oregonstate.edu

#include "beavalloc.h"

typedef struct mem_block_s {
    uint8_t free;

    size_t capacity;
    size_t size;

    struct mem_block_s *prev;
    struct mem_block_s *next;
} mem_block_t;

#define BLOCK_SIZE (sizeof(mem_block_t))
#define BLOCK_DATA(__curr) (((void *) __curr) + (BLOCK_SIZE))

static mem_block_t *block_list_head = NULL;

static void *lower_mem_bound = NULL;
static void *upper_mem_bound = NULL;

static uint8_t isVerbose = FALSE;
static FILE *beavalloc_log_stream = NULL;

// This is some gcc magic.
static void init_streams(void) __attribute__((constructor));

// Leave this alone
static void 
init_streams(void)
{
    beavalloc_log_stream = stderr;
}

// Leave this alone
void 
beavalloc_set_verbose(uint8_t verbosity)
{
    isVerbose = verbosity;
    if (isVerbose) {
        fprintf(beavalloc_log_stream, "Verbose enabled\n");
    }
}

// Leave this alone
void 
beavalloc_set_log(FILE *stream)
{
    beavalloc_log_stream = stream;
}

void *
beavalloc(size_t size)
{
    struct mem_block_s *newMem = NULL;
    struct mem_block_s *s = NULL;
    void *ptr = NULL;
    size_t numBytes_1024;
    size_t reqBytes = size + BLOCK_SIZE;

    if(!size){
        return NULL;
    }

    /* check if split memory */
    s = block_list_head;

    // look for split
    while(s){
        if(!s->free && ((s->capacity - s->size) >= reqBytes)){

            newMem = ((void*)s + BLOCK_SIZE + s->size);

            newMem->capacity = s->capacity - s->size - BLOCK_SIZE;
            s->capacity = s->size;
            newMem->next = s->next;

            s->next = newMem;
            newMem->prev = s;

            newMem->size = size;
            
            
            ptr = newMem;
            break;

        }
        else if(s->free && (s->capacity >= size)){
                        // overwrite free memory
            s->free = FALSE;
            s->size = size;
            newMem = s;
            ptr = s;

            break;

        }
        else
        {
            s = s->next;
        }
    }


    /*  allocate new memory */
    if(!newMem){

                // calculate number of bytes to allocate
        // required: num bytes plus space for data structure (40B)
        // must be a multiple of 1024
        numBytes_1024 = ((size + BLOCK_SIZE+1023) / 1024) * 1024;
        ptr = sbrk(numBytes_1024);
        printf("allocating new mem: %zu\n", numBytes_1024);
        
        // init lower_mem_bound
        if(lower_mem_bound == NULL)
            lower_mem_bound = (void*)ptr;

        if(upper_mem_bound == NULL)
            upper_mem_bound = (void*)ptr + numBytes_1024;
        else
            upper_mem_bound = upper_mem_bound + numBytes_1024;

        // save struct values    
        newMem = ptr;
        newMem->capacity = numBytes_1024 - BLOCK_SIZE;
        newMem->size = size;
        
        // update double linked list structure
        if(block_list_head){
            s = block_list_head;
            while(s->next){
                s = s->next;
            }
            newMem->prev = s;
            newMem->next = NULL;
            s->next = newMem;
        }
        else{
            newMem->prev = NULL;
            newMem->next = NULL;
            block_list_head = newMem;
        }


        
    }

    // return pointer to available address for data
    return BLOCK_DATA(ptr);
}

void 
beavfree(void *ptr)
{
    struct mem_block_s *s = block_list_head;
    ptr = ptr - BLOCK_SIZE;

    while(s){
        if(s == ptr){
            s->free = TRUE;
            s->size = 0;

            if(s->next){
                if(s->next->free == TRUE){ //coalesce
                    s->capacity += (s->next->capacity + BLOCK_SIZE);
                    s->next = s->next->next;
                    if(s->next)
                        s->next->prev = s;
                }
            }
                            // check if previous block was free
            if(s->prev){
                if(s->prev->free == TRUE){
                    beavfree(BLOCK_DATA((void*)s->prev));
                }
            }
            
            break;

        }
        else
        {
            s = s->next;
        }
    }

    

    return;
}


void 
beavalloc_reset(void)
{
    brk(lower_mem_bound);
    block_list_head = NULL;
}

void *
beavcalloc(size_t nmemb, size_t size)
{
    void *ptr = NULL;
    if(nmemb > 0 && size > 0)
        ptr = beavalloc(nmemb * size);

    if(ptr)
        memset(ptr, 0, nmemb);

    return ptr;
}

void *
beavrealloc(void *ptr, size_t size)
{
    void *nptr = NULL;
    struct mem_block_s *s = block_list_head;

    // malloc if null pointer
    if(!ptr)
        return(beavalloc(size));

    ptr = ptr - BLOCK_SIZE;

    // free mem if valid pointer and size = 0
    if (size == 0)
        beavfree(ptr);

    // otherwise
    else{
        while(s){
            if(s == ptr){
                if(s->capacity >= size ){ // enough space to relloc in same spot
                    s->size = size;
                    nptr = BLOCK_DATA(ptr);
                }
                else{ //new mem block
                    nptr = beavalloc(size);
                    memcpy(nptr, BLOCK_DATA(ptr), s->size);
                    beavfree(BLOCK_DATA(ptr));
                }
                break;
            }
            else
                s = s->next;
        }
    }

    return nptr;
}

void *
beavstrdup(const char *s)
{
    void *nptr = NULL;

    if(s){
        nptr = malloc(strlen(s));
        memcpy(nptr+BLOCK_SIZE, s, strlen(s));
    }

    return nptr;
}

// Leave this alone
void 
beavalloc_dump(void)
{
    mem_block_t *curr = NULL;
    unsigned i = 0;
    unsigned user_bytes = 0;
    unsigned capacity_bytes = 0;
    unsigned block_bytes = 0;
    unsigned used_blocks = 0;
    unsigned free_blocks = 0;

    fprintf(beavalloc_log_stream, "Heap map\n");
    fprintf(beavalloc_log_stream
            , "  %s\t%s\t%s\t%s\t%s" 
              "\t%s\t%s\t%s\t%s\t%s"
            "\n"
            , "blk no  "
            , "block add "
            , "next add  "
            , "prev add  "
            , "data add  "
            
            , "blk size "
            , "capacity "
            , "size     "
            , "excess   "
            , "status   "
        );
    for (curr = block_list_head, i = 0; curr != NULL; curr = curr->next, i++) {
        fprintf(beavalloc_log_stream
                , "  %u\t\t%9p\t%9p\t%9p\t%9p\t%u\t\t%u\t\t"
                  "%u\t\t%u\t\t%s\t%c"
                , i
                , curr
                , curr->next
                , curr->prev
                , BLOCK_DATA(curr)

                , (unsigned) (curr->capacity + BLOCK_SIZE)
                , (unsigned) curr->capacity
                , (unsigned) curr->size
                , (unsigned) (curr->capacity - curr->size)
                , curr->free ? "free  " : "in use"
                , curr->free ? '*' : ' '
            );
        fprintf(beavalloc_log_stream, "\n");
        user_bytes += curr->size;
        capacity_bytes += curr->capacity;
        block_bytes += curr->capacity + BLOCK_SIZE;
        if (curr->free == TRUE) {
            free_blocks++;
        }
        else {
            used_blocks++;
        }
    }
    fprintf(beavalloc_log_stream
            , "  %s\t\t\t\t\t\t\t\t"
              "%u\t\t%u\t\t%u\t\t%u\n"
            , "Total bytes used"
            , block_bytes
            , capacity_bytes
            , user_bytes
            , capacity_bytes - user_bytes
        );
    fprintf(beavalloc_log_stream
            , "  Used blocks: %4u  Free blocks: %4u  "
              "Min heap: %9p    Max heap: %9p   Block size: %lu bytes\n"
            , used_blocks
            , free_blocks
            , lower_mem_bound
            , upper_mem_bound
            , BLOCK_SIZE
        );
}
