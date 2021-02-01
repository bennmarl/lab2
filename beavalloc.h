// R. Jesse Chaney
// chaneyr@oregonstate.edu

#ifndef __BEAVALLOC_H
# define __BEAVALLOC_H

# include <stdint.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>
# include <stddef.h>
# include <values.h>
# include <stdlib.h>
# include <stdio.h>

// enable the define below to turn off assert.
//# define NDEBUG
# include <assert.h>

#ifndef MAX
# define MAX(_a,_b) ((_a) > (_b) ? (_a) : (_b))
#endif // MAX

#ifndef MIN
# define MIN(_a,_b) ((_a) < (_b) ? (_a) : (_b))
#endif // MIN

#ifndef TRUE
# define TRUE 1
#endif // TRUE
#ifndef FALSE
# define FALSE 0
#endif // FALSE

// Allows you you to specify the algorithm to be use when
//   finding blocks to use.
// See OSTEP Chap 17
// For your assignement, you will only use FIRST_FIT. The others are
//   for projects in another space time continuum.
typedef enum {
    FIRST_FIT
    , BEST_FIT
    , WORST_FIT
    , NEXT_FIT
} beavalloc_fit_algorithm_t;

#ifndef MIN_SBRK_SIZE
# define MIN_SBRK_SIZE 1024
//# define MIN_SBRK_SIZE 4096
#endif // MIN_SBRK_SIZE


// The basic memory allocator.
// If you pass NULL or 0, then NULL is returned.
// If, for some reason, the system cannot allocate the requested
//   memory, set errno and return NULL.
// Do not allocate a chunk of memory smaller than 20 bytes (plus
//   whatever you need for your structure).
// You must use sbrk() or brk() in requesting more memory for your
//   beavalloc() routine to manage.
void *beavalloc(size_t);

// A pointer returned from a prevous call to beavalloc() must
//   be passed.
// If a pointer is passed to a block than is already free, 
//   simply return. If you are in verbose mode, print a snarky message
//   before returning.
// Blocks must be coalesced, where possible, as they are free'ed.
void beavfree(void *);

// Completely reset your heap back to zero bytes allocated.
// You are going to like being able to do this.
// Implementation can be done in as few as 1 line, though
//   you will probably use more to reset the stats you keep
//   about heap.
// After you've called this function, everything you had in
//   the heap is just __GONE__!!!
// You should be able to call beavalloc() after calling beavalloc_reset()
//   to restart building the heap again.
void beavalloc_reset(void);

// Set the verbosity of your beavalloc() code (and related functions).
// This should modify a variable that is static to your C module.
void beavalloc_set_verbose(uint8_t);

// Set the stream into which diagnostic information will be sent.
void beavalloc_set_log(FILE *);

// This is like the regular calloc() call. See the man page for details.
// Unlike the memory returned from malloc(), the memory returned from a
//   call to calloc() will initialize the returned memory to all zeroes.
// Your beavcallo() should do the same.
// Do youself a BIG favor, don't try to reimplement the capabality of 
//   beavalloc() in here, call it instead.
void *beavcalloc(size_t, size_t);

// This is like the regular realloc() call. See the man page for details.
// As realloc() is magic, so is beavrealloc().
// Do youself a BIG favor, don't try to reimplement the capabality of 
//   beavalloc() in here, call it instead.
void *beavrealloc(void *, size_t);

// This is exactly like the strdup() call. See the man page for details.
// Do youself a BIG favor, don't try to reimplement the capabality of 
//   beavalloc() in here, call it instead.
void *beavstrdup(const char *s);

// Just read the source code in beavalloc.c. It is pretty straight forward.
void beavalloc_dump(void);

#endif // __BEAVALLOC_H
