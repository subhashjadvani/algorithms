/* Let's take the final logical step and implement an architecture-independent
 * memset that can cope with unaligned pointers and sizes. Note, the same
 * caveat as for wordwise_memset applies; you wouldn't write code like this in
 * real life.
 */
void* wordwise_unaligned_memset(void* s, int c, size_t sz) {
    uintptr_t* p;
    uintptr_t x = c & 0xff;
    byte* pp = (byte*)s;
    byte xx = c & 0xff;
    size_t tail;
    int i;
    int bytes_per_word;
    int shift;

    /*
     for (i = 3; (1<<i) < __WORDSIZE; ++i)
         x |= x << (1<<i);
     bytes_per_word = 1<<(i-3);
     */
    if (__WORDSIZE == 8) {
        bytes_per_word = 1;
        shift = 0;
    } else if (__WORDSIZE == 16) {
        x |= x << 8;
        bytes_per_word = 2;
        shift = 1;
    } else if (__WORDSIZE == 32) {
        /* Construct a word's worth of the value we're supposed to be setting. */
        x |= x << 8;
        x |= x << 16;
        bytes_per_word = 4;
        shift = 2;
    } else if (__WORDSIZE == 64) {
        /* Construct a word's worth of the value we're supposed to be setting. */
        x |= x << 8;
        x |= x << 16;
        x |= x << 32;
        bytes_per_word = 8;
        shift = 3;
    } else {
        assert(false);
    }

    /* Prologue. */
    while (((unsigned int)pp & (bytes_per_word-1)) && sz--)
        *pp++ = xx;
    tail = sz & (bytes_per_word-1);
    p = (uintptr_t*)pp;

    /* Main loop. */
    sz >>= log(bytes_per_word);
    while (sz--)
        *p++ = x;

    /* Epilogue. */
    pp = (byte*)p;
    while (tail--)
        *pp++ = xx;

    return s;
}



/* Let's take the final logical step and implement an architecture-independent
 * memset that can cope with unaligned pointers and sizes. Note, the same
 * caveat as for wordwise_memset applies; you wouldn't write code like this in
 * real life.
 */
void* wordwise_unaligned_memset(void* s, int c, size_t sz) {
    uintptr_t* p;
    uintptr_t x = c & 0xff;
    byte* pp = (byte*)s;
    byte xx = c & 0xff;
    size_t tail;
    int i;
    int bytes_per_word;
    int shift;

    /*
     for (i = 3; (1<<i) < __WORDSIZE; ++i)
         x |= x << (1<<i);
     bytes_per_word = 1<<(i-3);
     */
    
    bytes_per_word = __WORDSIDE >> 3;
    shift = log(bytes_per_word);
    for (int i = 1; i < shift; i++) {
        x |= x << (8 * i);
    }

    /* Prologue. */
    while (((unsigned int)pp & (bytes_per_word-1)) && sz--)
        *pp++ = xx;
    tail = sz & (bytes_per_word-1);
    p = (uintptr_t*)pp;

    /* Main loop. */
    sz >>= log(bytes_per_word);
    while (sz--)
        *p++ = x;

    /* Epilogue. */
    pp = (byte*)p;
    while (tail--)
        *pp++ = xx;

    return s;
}

