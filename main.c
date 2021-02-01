// R. Jesse Chaney

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

//#define NDEBUG
#include <assert.h>

#include "beavalloc.h"

#ifndef NUM_PTRS
# define NUM_PTRS 100
#endif // NUM_PTRS

#define OPTIONS "hvt:o:"

static unsigned test_number = 0;
static FILE *log_stream = NULL;
static char *base = NULL;

void first_fit_tests(void);

void basic1(int);
void basic2(int);
void bytes10(int);
void bytes2000(int);
void bytes10000(int);

void allocs5(int);
void alloc5free3(int);
void alloc5free2(int);

void coalesce1(int);
void memset1(int);
void memset2(int);
void memset3(int);
void memset4(int);
void splitcoalesce1(int);

void freefree(int);

void calloc1(int);
void calloc2(int);
void calloc3(int);
void realloc1(int);

void realloc2(int);
void realloc3(int);
void realloc4(int);
void realloc5(int);

void stress1(int);
void stress2(int);
void stress3(int);
void stress4(int);
void stress5(int);

void strdup1(int);

// This is some gcc magic.
static void init_streams(void) __attribute__((constructor));

static void init_streams(void)
{
    log_stream = stderr;
}

int
main(int argc, char **argv)
{
    uint8_t isVerbose = FALSE;
    {
        int opt = -1;

        while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
            switch (opt) {
            case 'h':
                fprintf(log_stream, "%s %s\n", argv[0], OPTIONS);
                fprintf(log_stream, "  -h        : print help and exit\n");
                fprintf(log_stream, "  -v        : verbose output\n");
                fprintf(log_stream, "  -t #      : test number to run, 0 for all\n");
                
                exit(0);
                break;
            case 'o':
                log_stream = fopen(optarg, "w");
                if (NULL == log_stream) {
                    perror("cannot open log file");
                    exit(EXIT_FAILURE);
                }
                beavalloc_set_log(log_stream);
                break;
            case 'v':
                isVerbose = isVerbose;
                beavalloc_set_verbose(TRUE);
                fprintf(log_stream, "Verbose enabled\n");
                break;
            case 't':
                test_number = atoi(optarg);
                break;
            default: /* '?' */
                fprintf(log_stream, "%s\n", argv[0]);
                exit(EXIT_FAILURE);
            }
        }
    }
    first_fit_tests();

    return EXIT_SUCCESS;
}

void
first_fit_tests(void)
{
    fprintf(log_stream, "beavalloc first fit tests starting\n");
    if (test_number == 0) {
        fprintf(log_stream, "  running all tests\n");
    }
    else {
        fprintf(log_stream, "  running only test %d\n", test_number);
    }

    // Get the beginning address of the start of the stack.
    base = sbrk(0);
    fprintf(log_stream, "Initial base address of heap: %p\n", base);

    // This is some really boring code.
    if (test_number == 0 || test_number == 1) {
        basic1(test_number);
    }
    if (test_number == 0 || test_number == 2) {
        basic2(test_number);
    }

    if (test_number == 0 || test_number == 3) {
        bytes10(test_number);
    }
    if (test_number == 0 || test_number == 4) {
        bytes2000(test_number);
    }
    if (test_number == 0 || test_number == 5) {
        bytes10000(test_number);
    }

    if (test_number == 0 || test_number == 6) {
        allocs5(test_number);
    }
    if (test_number == 0 || test_number == 7) {
        alloc5free2(test_number);
    }
    if (test_number == 0 || test_number == 8) {
        alloc5free3(test_number);
    }

    if (test_number == 0 || test_number == 9) {
        coalesce1(test_number);
    }
    if (test_number == 0 || test_number == 10) {
        splitcoalesce1(test_number);
    }

    if (test_number == 0 || test_number == 11) {
        freefree(test_number);
    }

    if (test_number == 0 || test_number == 12) {
        memset1(test_number);
    }
    if (test_number == 0 || test_number == 13) {
        memset2(test_number);
    }
    if (test_number == 0 || test_number == 14) {
        memset3(test_number);
    }
    if (test_number == 0 || test_number == 15) {
        memset4(test_number);
    }

    if (test_number == 0 || test_number == 16) {
        calloc1(test_number);
    }
    if (test_number == 0 || test_number == 17) {
        calloc2(test_number);
    }
    if (test_number == 0 || test_number == 18) {
        calloc3(test_number);
    }

    if (test_number == 0 || test_number == 19) {
        realloc1(test_number);
    }
    if (test_number == 0 || test_number == 20) {
        realloc2(test_number);
    }
    if (test_number == 0 || test_number == 21) {
        realloc3(test_number);
    }
    if (test_number == 0 || test_number == 22) {
        realloc4(test_number);
    }
    if (test_number == 0 || test_number == 23) {
        realloc5(test_number);
    }

    if (test_number == 0 || test_number == 24) {
        stress1(test_number);
    }
    if (test_number == 0 || test_number == 25) {
        stress2(test_number);
    }
    if (test_number == 0 || test_number == 26) {
        stress3(test_number);
    }
    if (test_number == 0 || test_number == 27) {
        stress4(test_number);
    }
    if (test_number == 0 || test_number == 28) {
        stress5(test_number);
    }
    if (test_number == 0 || test_number == 29) {
        strdup1(test_number);
    }

    if (test_number == 0) {
        fprintf(log_stream, "\n\nWoooooooHooooooo!!! "
                "All tests done and you survived. This only means it did not seg-fault.\n\n"
                "\tNow, make sure they are correct\n\n\n");
    }
    else {
        fprintf(log_stream, "\n\nWoooooooHooooooo!!! "
                "You survived test %d. This only means it did not seg-fault.\n\n"
                "\tNow, make sure it is correct.\n\n\n", test_number);
    }
}

void 
basic1(int testno)
{
    char *ptr1 = NULL;

    // Does the heap reset function worK?
    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      Basic reset\n");

    beavalloc_reset();
    ptr1 = sbrk(0);
    fprintf(log_stream, "  ptr : %p\n", base);
    assert(ptr1 == base);

    beavalloc_reset();
    ptr1 = sbrk(0);
    fprintf(log_stream, "  ptr : %p\n", base);
    assert(ptr1 == base);

    fprintf(log_stream, "*** End %d\n", testno);
}

void 
basic2(int testno)
{
    char *ptr1 = NULL;

    // Allocate zero bytes and see if that works.
    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      zero bytes\n");

    ptr1 = beavalloc(0);
    assert(ptr1 == NULL);

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
bytes10(int testno)
{
    char *ptr1 = NULL;

    // Just allocate 10 bytes.
    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      10 bytes\n");

    ptr1 = beavalloc(10);
    assert(ptr1 != NULL);
    assert(base < ptr1);

    fprintf(log_stream, "  ptr1 : %p\n"
            , ptr1);

    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
bytes2000(int testno)
{
    char *ptr1 = NULL;

    // Just allocate 2000 bytes.
    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      2000 bytes\n");

    ptr1 = beavalloc(2000);
    assert(ptr1 != NULL);
    assert(base < ptr1);

    fprintf(log_stream, "  ptr1 : %p\n"
            , ptr1);

    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
bytes10000(int testno)
{
    char *ptr1 = NULL;

    // Just allocate 10000 bytes.
    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      10000 bytes\n");

    ptr1 = beavalloc(10000);
    assert(ptr1 != NULL);
    assert(base < ptr1);

    fprintf(log_stream, "  ptr1 : %p\n"
            , ptr1);

    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
allocs5(int testno)
{
    char *ptr1 = NULL;
    char *ptr2 = NULL;
    char *ptr3 = NULL;
    char *ptr4 = NULL;
    char *ptr5 = NULL;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      5 allocs\n");

    ptr1 = beavalloc(500);
    ptr2 = beavalloc(530);
    ptr3 = beavalloc(550);
    ptr4 = beavalloc(1024);
    ptr5 = beavalloc(2048);

    assert(base < ptr1);
    assert(ptr1 < ptr2);
    assert(ptr2 < ptr3);
    assert(ptr3 < ptr4);
    assert(ptr4 < ptr5);

    fprintf(log_stream, "  ptr1 : %p\n  ptr2 : %p\n  ptr3 : %p\n  ptr4 : %p\n  ptr5 : %p\n"
            , ptr1, ptr2, ptr3, ptr4, ptr5);

    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
alloc5free3(int testno)
{
    char *ptr1 = NULL;
    char *ptr2 = NULL;
    char *ptr3 = NULL;
    char *ptr4 = NULL;
    char *ptr5 = NULL;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      5 allocs 3 frees\n");
    ptr1 = beavalloc(510);
    ptr2 = beavalloc(530);
    ptr3 = beavalloc(550);
    ptr4 = beavalloc(570);
    ptr5 = beavalloc(590);

    fprintf(log_stream, "  ptr1 : %p\n  ptr2 : %p\n  ptr3 : %p\n  ptr4 : %p\n  ptr5 : %p\n"
            , ptr1, ptr2, ptr3, ptr4, ptr5);
    assert(base < ptr1);
    assert(ptr1 < ptr2);
    assert(ptr2 < ptr3);
    assert(ptr3 < ptr4);
    assert(ptr4 < ptr5);

    beavalloc_dump();

    beavfree(ptr1);
    beavfree(ptr3);
    beavfree(ptr5);

    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
alloc5free2(int testno)
{
    char *ptr1 = NULL;
    char *ptr2 = NULL;
    char *ptr3 = NULL;
    char *ptr4 = NULL;
    char *ptr5 = NULL;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      5 allocs 2 frees\n");

    ptr1 = beavalloc(510);
    ptr2 = beavalloc(530);
    ptr3 = beavalloc(550);
    ptr4 = beavalloc(570);
    ptr5 = beavalloc(590);

    fprintf(log_stream, "  ptr1 : %p\n  ptr2 : %p\n  ptr3 : %p\n  ptr4 : %p\n  ptr5 : %p\n"
            , ptr1, ptr2, ptr3, ptr4, ptr5);
    assert(base < ptr1);
    assert(ptr1 < ptr2);
    assert(ptr2 < ptr3);
    assert(ptr3 < ptr4);
    assert(ptr4 < ptr5);

    beavalloc_dump();

    beavfree(ptr2);
    beavfree(ptr4);

    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
coalesce1(int testno)
{
    char *ptr1 = NULL;
    char *ptr2 = NULL;
    char *ptr3 = NULL;
    char *ptr4 = NULL;
    char *ptr5 = NULL;

    fprintf(log_stream,"*** Begin %d\n", testno);
    fprintf(log_stream,"      begin coalesce\n");

    ptr1 = beavalloc(510);
    ptr2 = beavalloc(530);
    ptr3 = beavalloc(550);
    ptr4 = beavalloc(570);
    ptr5 = beavalloc(590);

    assert(base < ptr1);
    assert(ptr1 < ptr2);
    assert(ptr2 < ptr3);
    assert(ptr3 < ptr4);
    assert(ptr4 < ptr5);
    fprintf(log_stream, "  ptr1 : %p\n  ptr2 : %p\n  ptr3 : %p\n  ptr4 : %p\n  ptr5 : %p\n"
            , ptr1, ptr2, ptr3, ptr4, ptr5);

    beavalloc_dump();

    beavfree(ptr2);
    beavfree(ptr4);
    beavalloc_dump();

    fprintf(log_stream,"-- coalesce right\n");
    beavfree(ptr1);
    beavalloc_dump();

    fprintf(log_stream,"-- coalesce left\n");
    beavfree(ptr5);
    beavalloc_dump();

    fprintf(log_stream,"-- coalesce right and left\n");
    beavfree(ptr3);
    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream,"*** End %d\n", testno);
}

void 
memset1(int testno)
{
    char *ptr1 = NULL;
    char *ptr2 = NULL;
    char *ptr3 = NULL;
    char *ptr4 = NULL;
    char *ptr5 = NULL;
    int allocs[] = {900, 3000, 5000, 7000, 10000};

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      memset 1\n");

    ptr1 = beavalloc(allocs[0]);
    ptr2 = beavalloc(allocs[1]);
    ptr3 = beavalloc(allocs[2]);
    ptr4 = beavalloc(allocs[3]);
    ptr5 = beavalloc(allocs[4]);

    assert(base < ptr1);
    assert(ptr1 < ptr2);
    assert(ptr2 < ptr3);
    assert(ptr3 < ptr4);
    assert(ptr4 < ptr5);

    beavalloc_dump();

    memset(ptr1, 0x1, allocs[0]);
    memset(ptr2, 0x2, allocs[1]);
    memset(ptr3, 0x3, allocs[2]);
    memset(ptr4, 0x4, allocs[3]);
    memset(ptr5, 0x5, allocs[4]);

    beavalloc_dump();

    {
        char ch[10000] = {0};

        memset(ch, 0x1, allocs[0]);
        assert(memcmp(ptr1, ch, allocs[0]) == 0);

        memset(ch, 0x2, allocs[1]);
        assert(memcmp(ptr2, ch, allocs[1]) == 0);

        memset(ch, 0x3, allocs[2]);
        assert(memcmp(ptr3, ch, allocs[2]) == 0);

        memset(ch, 0x4, allocs[3]);
        assert(memcmp(ptr4, ch, allocs[3]) == 0);

        memset(ch, 0x5, allocs[4]);
        assert(memcmp(ptr5, ch, allocs[4]) == 0);
    }

    beavfree(ptr1);
    beavfree(ptr2);
    beavfree(ptr3);
    beavfree(ptr4);
    beavfree(ptr5);

    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
memset2(int testno)
{
    char *ptr1 = NULL;
    char *ptr2 = NULL;
    char *ptr3 = NULL;
    char *ptr4 = NULL;
    char *ptr5 = NULL;
    int allocs[] = {2000, 4000, 6000, 8000, 12000};

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      memset 2\n");

    ptr1 = beavalloc(allocs[0]);
    ptr2 = beavalloc(allocs[1]);
    ptr3 = beavalloc(allocs[2]);
    ptr4 = beavalloc(allocs[3]);
    ptr5 = beavalloc(allocs[4]);

    assert(base < ptr1);
    assert(ptr1 < ptr2);
    assert(ptr2 < ptr3);
    assert(ptr3 < ptr4);
    assert(ptr4 < ptr5);

    beavalloc_dump();

    memset(ptr1, 0x1, allocs[0]);
    memset(ptr2, 0x2, allocs[1]);
    memset(ptr3, 0x3, allocs[2]);
    memset(ptr4, 0x4, allocs[3]);
    memset(ptr5, 0x5, allocs[4]);

    beavalloc_dump();

    beavfree(ptr1);
    beavfree(ptr3);
    beavfree(ptr5);

    {
        char ch[12000] = {0};

        memset(ch, 0x2, allocs[1]);
        assert(memcmp(ptr2, ch, allocs[1]) == 0);

        memset(ch, 0x4, allocs[3]);
        assert(memcmp(ptr4, ch, allocs[3]) == 0);
    }
    beavalloc_dump();

    beavfree(ptr2);
    beavfree(ptr4);

    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
memset3(int testno)
{
    char *ptr1 = NULL;
    char *ptr2 = NULL;
    char *ptr3 = NULL;
    char *ptr4 = NULL;
    char *ptr5 = NULL;
    int allocs[] = {500, 700, 900, 1100, 1300};

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      memset 3\n");

    ptr1 = beavalloc(allocs[0]);
    ptr2 = beavalloc(allocs[1]);
    ptr3 = beavalloc(allocs[2]);
    ptr4 = beavalloc(allocs[3]);
    ptr5 = beavalloc(allocs[4]);

    assert(base < ptr1);
    assert(ptr1 < ptr2);
    assert(ptr2 < ptr3);
    assert(ptr3 < ptr4);
    assert(ptr4 < ptr5);

    beavalloc_dump();

    memset(ptr1, 0x1, allocs[0]);
    memset(ptr2, 0x2, allocs[1]);
    memset(ptr3, 0x3, allocs[2]);
    memset(ptr4, 0x4, allocs[3]);
    memset(ptr5, 0x5, allocs[4]);

    beavalloc_dump();

    beavfree(ptr1);
    beavfree(ptr2);
    beavfree(ptr5);
    beavfree(ptr4);

    {
        char ch[2000] = {0};

        memset(ch, 0x3, allocs[2]);
        assert(memcmp(ptr3, ch, allocs[2]) == 0);
    }
    beavalloc_dump();

    beavfree(ptr3);

    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
memset4(int testno)
{
    char *ptr1 = NULL;
    char *ptr2 = NULL;
    char *ptr3 = NULL;
    char *ptr4 = NULL;
    char *ptr5 = NULL;
    int allocs[] = {1 * 1024, 2 * 1024, 3 * 1024, 4 * 1024, 5 * 1024};

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      memset 4\n");

    ptr1 = beavalloc(allocs[0]);
    ptr2 = beavalloc(allocs[1]);
    ptr3 = beavalloc(allocs[2]);
    ptr4 = beavalloc(allocs[3]);
    ptr5 = beavalloc(allocs[4]);

    assert(base < ptr1);
    assert(ptr1 < ptr2);
    assert(ptr2 < ptr3);
    assert(ptr3 < ptr4);
    assert(ptr4 < ptr5);

    beavalloc_dump();

    memset(ptr1, 0x1, allocs[0]);
    memset(ptr2, 0x2, allocs[1]);
    memset(ptr3, 0x3, allocs[2]);
    memset(ptr4, 0x4, allocs[3]);
    memset(ptr5, 0x5, allocs[4]);

    beavalloc_dump();

    beavfree(ptr2);
    beavfree(ptr4);

    {
        char ch[5 * 1024] = {0};

        memset(ch, 0x1, allocs[0]);
        assert(memcmp(ptr1, ch, allocs[0]) == 0);

        memset(ch, 0x3, allocs[2]);
        assert(memcmp(ptr3, ch, allocs[2]) == 0);

        memset(ch, 0x5, allocs[4]);
        assert(memcmp(ptr5, ch, allocs[4]) == 0);
    }
    beavalloc_dump();

    beavfree(ptr1);
    beavfree(ptr3);
    beavfree(ptr5);

    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
splitcoalesce1(int testno)
{
    char *ptr1 = NULL;
    char *ptr2 = NULL;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      split and coalesce\n");

    ptr1 = beavalloc(10000);
    beavalloc_dump();
    beavfree(ptr1);
    beavalloc_dump();

    ptr1 = beavalloc(100);
    beavalloc_dump();

    // should force a split
    ptr2 = beavalloc(200);
    beavalloc_dump();

    fprintf(log_stream, "  ptr1 : %p\n  ptr2 : %p\n"
            , ptr1, ptr2);


    beavfree(ptr2);
    beavalloc_dump();

    // coalesce right
    beavfree(ptr1);
    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);

    fprintf(log_stream, "*** End %d\n", testno);
}

void 
stress1(int testno)
{
    const ushort num_ptrs = NUM_PTRS;
    void *ptrs[num_ptrs];
    int i = 0;
    char *ptr1 = NULL;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      stress 1  %u\n", num_ptrs);

    for (i = 0; i < num_ptrs; i++) {
        ptrs[i] = beavalloc(i + 100);
    }
    beavalloc_dump();

    for (i = 3; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
    }

    for (i = 0; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
    }

    for (i = 1; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
    }
    for (i = 2; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
    }

    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);

    fprintf(log_stream, "*** End %d\n", testno);
}

void 
stress2(int testno)
{
    const ushort num_ptrs = NUM_PTRS;
    void *ptrs[num_ptrs];
    int i = 0;
    char *ptr1 = NULL;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      stress 2  %u\n", num_ptrs);

    for (i = 0; i < num_ptrs; i++) {
        ptrs[i] = beavalloc((i * 15) + 900);
        memset(ptrs[i], 0x0, (i * 15) + 900);
    }
    beavalloc_dump();

    for (i = 3; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    for (i = 2; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    for (i = 1; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    for (i = 0; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    /////////////////////
    for (i = 0; i < num_ptrs; i++) {
        ptrs[i] = beavalloc((i * 15) + 500);
        memset(ptrs[i], 0x0, (i * 15) + 500);
    }
    beavalloc_dump();

    for (i = 0; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    for (i = 1; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    for (i = 2; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    for (i = 3; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    ///////////////////
    for (i = 0; i < num_ptrs; i++) {
        ptrs[i] = beavalloc((i * 15) + 2000);
        memset(ptrs[i], 0x0, (i * 15) + 2000);
    }
    beavalloc_dump();

    for (i = 3; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    for (i = 0; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    for (i = 2; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    for (i = 3; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);

    fprintf(log_stream, "*** End %d\n", testno);
}

void 
stress3(int testno)
{
    const ushort num_ptrs = NUM_PTRS;
    void *ptrs[num_ptrs];
    int i = 0;
    char *ptr1 = NULL;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      stress 3  %u\n", num_ptrs);

    for (i = 0; i < num_ptrs; i++) {
        ptrs[i] = beavalloc((i * 15) + 900);
    }
    beavalloc_dump();

    for (i = 3; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
        ptrs[i] = beavalloc((i * 5) + 100);
    }

    for (i = 2; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
        ptrs[i] = beavalloc((i * 5) + 1000);
    }

    for (i = 1; i < num_ptrs; i += 4) {
        beavfree(ptrs[i]);
        ptrs[i] = beavalloc(i + 2000);
    }
    beavalloc_dump();

    for (i = 0; i < num_ptrs; i++) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);

    fprintf(log_stream, "*** End %d\n", testno);
}

void 
stress4(int testno)
{
    const ushort num_ptrs = NUM_PTRS;
    void *ptrs[num_ptrs];
    int i = 0;
    char *ptr1 = NULL;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      stress 4  %u\n", num_ptrs);

    for (i = 0; i < num_ptrs; i++) {
        ptrs[i] = beavalloc(i + 10);
    }
    beavalloc_dump();
    for (i = 0; i < num_ptrs; i ++) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    //beavalloc_set_verbose(TRUE);
    for (i = 0; i < num_ptrs; i ++) {
        ptrs[i] = beavalloc(i + 1000);
        //fprintf(log_stream, "**** i = %d   %p\n", i, ptrs[i]);
        //beavalloc_dump();
    }
    beavalloc_dump();
    for (i = 0; i < num_ptrs; i ++) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    for (i = 0; i < num_ptrs; i ++) {
        ptrs[i] = beavalloc(i + 107);
    }
    beavalloc_dump();
    for (i = 0; i < num_ptrs; i ++) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    for (i = 0; i < num_ptrs; i ++) {
        ptrs[i] = beavalloc(i + 2000);
    }
    beavalloc_dump();
    for (i = 0; i < num_ptrs; i ++) {
        beavfree(ptrs[i]);
    }

    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);

    fprintf(log_stream, "*** End %d\n", testno);
}

void 
stress5(int testno)
{
    const ushort num_ptrs = NUM_PTRS;
    void *ptrs[num_ptrs];
    int i = 0;
    char *ptr1 = NULL;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      stress 5  %u\n", num_ptrs);

    ptrs[0] = beavalloc(100000);
    beavfree(ptrs[0]);

    for (i = 0; i < num_ptrs; i++) {
        ptrs[i] = beavalloc(i + 1);
    }
    beavalloc_dump();

    for (i = 0; i < num_ptrs; i++) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    //
    for (i = 0; i < num_ptrs; i++) {
        ptrs[i] = beavalloc(i + 1);
    }
    beavalloc_dump();

    for (i = NUM_PTRS - 1; i >= 0; i--) {
        beavfree(ptrs[i]);
    }
    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);

    fprintf(log_stream, "*** End %d\n", testno);
}

void
strdup1(int testno)
{
    char ch1[] = "";
    char ch2[] = "a";
    char ch3[] = "hello world";
    char ch4[50] = "goodbye world";
    char *ptr1 = NULL;
    char *ptr2 = NULL;

    fprintf(log_stream,"*** Begin %d\n", testno);
    fprintf(log_stream,"      begin beavstrdup\n");

    ptr1 = beavstrdup(ch1);
    assert(strcmp(ptr1, ch1) == 0);
    beavfree(ptr1);

    ptr1 = beavstrdup(ch2);
    assert(strcmp(ptr1, ch2) == 0);
    beavfree(ptr1);

    ptr1 = beavstrdup(ch3);
    assert(strcmp(ptr1, ch3) == 0);
    beavfree(ptr1);

    ptr1 = beavstrdup(ch4);
    assert(strcmp(ptr1, ch4) == 0);

    ptr2 = beavstrdup(ptr1);
    assert(strcmp(ptr1, ptr2) == 0);

    beavalloc_dump();
    beavfree(ptr1);
    beavfree(ptr2);
    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);

    fprintf(log_stream,"*** End %d\n", testno);
}

void 
freefree(int testno)
{
    char *ptr1 = NULL;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      Free the free\n");

    ptr1 = beavalloc(10);
    beavfree(ptr1);
    beavalloc_set_verbose(TRUE);
    beavfree(ptr1);
    beavalloc_set_verbose(FALSE);
    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
calloc1(int testno)
{
    char *ptr1 = NULL;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      beavcalloc 1\n");

    ptr1 = beavcalloc(1000, 17);
    assert(ptr1 != NULL);
    {
        char ch[1000 * 17] = {0};

        //memset(ch, 0x1, 1000);
        assert(memcmp(ptr1, ch, 2000) == 0);
    }
    beavalloc_dump();

    beavfree(ptr1);

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
calloc2(int testno)
{
    char *ptr1 = NULL;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      beavcalloc 2\n");

    ptr1 = beavcalloc(0, 17);
    assert(ptr1 == NULL);
    beavalloc_dump();

    beavfree(ptr1);

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
calloc3(int testno)
{
    char *ptr1 = NULL;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      beavcalloc 3\n");

    ptr1 = beavcalloc(1000, 0);
    assert(ptr1 == NULL);
    beavalloc_dump();

    beavfree(ptr1);

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
realloc1(int testno)
{
    char *ptr1 = NULL;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      beavrealloc 1\n");

    ptr1 = beavrealloc(ptr1, 100);
    assert(ptr1 != NULL);
    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
realloc2(int testno)
{
    char *ptr1 = NULL;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      beavrealloc 2\n");

    ptr1 = beavrealloc(ptr1, 1000);
    assert(ptr1 != NULL);
    memset(ptr1, 0x1, 1000);

    ptr1 = beavrealloc(ptr1, 5000);
    assert(ptr1 != NULL);
    memset(ptr1, 0x1, 5000);
    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
realloc3(int testno)
{
    char *ptr1 = NULL;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      beavrealloc 3\n");

    ptr1 = beavrealloc(ptr1, 5000);
    assert(ptr1 != NULL);

    ptr1 = beavrealloc(ptr1, 1000);
    assert(ptr1 != NULL);
    memset(ptr1, 0x1, 1000);
    beavalloc_dump();

    beavalloc_reset();
    ptr1 = sbrk(0);
    assert(ptr1 == base);
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
realloc4(int testno)
{
    char **ptr1 = NULL;
    char *ptrs[10] = {0};
    int i = 0;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      beavrealloc 4\n");

    for (i = 0; i < 10; i++) {
        ptrs[i] = beavalloc((i + 5) * 10);
    }

    ptr1 = beavrealloc(ptr1, 5 * (sizeof(char *)));
    assert(ptr1 != NULL);

    for (i = 0; i < 5; i++) {
        ptr1[i] = ptrs[i];
    }

    assert(memcmp(ptrs, ptr1, 5 * sizeof(char *)) == 0);
    beavalloc_dump();

    ptr1 = beavrealloc(ptr1, 2000 * (sizeof(char *)));
    assert(memcmp(ptrs, ptr1, 5 * sizeof(char *)) == 0);

    for (i = 5; i < 10; i++) {
        ptr1[i] = ptrs[i];
    }
    assert(memcmp(ptrs, ptr1, 10 * sizeof(char *)) == 0);
    beavalloc_dump();

    ptr1 = beavrealloc(ptr1, 2 * (sizeof(char *)));
    assert(memcmp(ptrs, ptr1, 2 * sizeof(char *)) == 0);

    beavalloc_dump();

    for (i = 0; i < 10; i++) {
        beavfree(ptrs[i]);
    }
    beavfree(ptr1);
    beavalloc_dump();

    beavalloc_reset();
    {
        char *ptr0 = sbrk(0);
        assert(ptr0 == base);
    }
    fprintf(log_stream, "*** End %d\n", testno);
}

void 
realloc5(int testno)
{
    char **ptr1 = NULL;
    char *ptrs[10] = {0};
    int i = 0;

    fprintf(log_stream, "*** Begin %d\n", testno);
    fprintf(log_stream, "      beavrealloc 5\n");

    ptr1 = beavrealloc(ptr1, 5 * (sizeof(char *)));
    assert(ptr1 != NULL);

    for (i = 0; i < 10; i++) {
        ptrs[i] = beavalloc((i + 2) * 1000);
    }

    ptr1 = beavrealloc(ptr1, 5 * (sizeof(char *)));
    assert(ptr1 != NULL);

    for (i = 0; i < 5; i++) {
        ptr1[i] = ptrs[i];
    }

    assert(memcmp(ptrs, ptr1, 5 * sizeof(char *)) == 0);
    beavalloc_dump();

    ptr1 = beavrealloc(ptr1, 2000 * (sizeof(char *)));
    assert(memcmp(ptrs, ptr1, 5 * sizeof(char *)) == 0);

    for (i = 5; i < 10; i++) {
        ptr1[i] = ptrs[i];
    }
    assert(memcmp(ptrs, ptr1, 10 * sizeof(char *)) == 0);
    beavalloc_dump();

    ptr1 = beavrealloc(ptr1, 2 * (sizeof(char *)));
    assert(memcmp(ptrs, ptr1, 2 * sizeof(char *)) == 0);

    beavalloc_dump();

    for (i = 0; i < 10; i++) {
        beavfree(ptrs[i]);
    }
    beavfree(ptr1);
    beavalloc_dump();

    beavalloc_reset();
    {
        char *ptr0 = sbrk(0);
        assert(ptr0 == base);
    }
    fprintf(log_stream, "*** End %d\n", testno);
}
