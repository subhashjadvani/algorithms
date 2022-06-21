void* wordwise_unaligned_memcpy(void* dst, void *src, size_t sz) {
    uintptr_t* ps;
    uintptr_t* pd;
    uintptr_t x = c & 0xff;
    byte* pps = (byte*)src;
    byte* ppd = (byte*)dst;
    byte xx = c & 0xff;
    size_t tail;
    int i;
    int bytes_per_word;
    int shift;

    if (__WORDSIZE == 8) {
        bytes_per_word = 1;
        shift = 0;
    } else if (__WORDSIZE == 16) {
        bytes_per_word = 2;
        shift = 1;
    } else if (__WORDSIZE == 32) {
        bytes_per_word = 4;
        shift = 2;
    } else if (__WORDSIZE == 64) {
        bytes_per_word = 8;
        shift = 3;
    } else {
        assert(false);
    }

    /* Prologue. */
    while (((unsigned int)pps & (bytes_per_word-1)) && ((unsigned int)ppd & (bytes_per_word-1)) && sz--)
        *ppd++ = *pps++;
    
    tail = sz & (bytes_per_word - 1);
    pd = (uintptr_t*)ppd;
    ps = (uintptr_t*)pps;

    /* Main loop. */
    sz >>= log(bytes_per_word);
    while (sz--)
        *pd++ = *ps++;

    /* Epilogue. */
    ppd = (byte*)pd;
    pps = (byte*)ps;
    while (tail--)
        *ppd++ = *pps++;

    return s;
}

