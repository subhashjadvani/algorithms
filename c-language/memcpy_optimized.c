void* wordwise_unaligned_memcpy(void* dst, void *src, size_t sz) {
    uintptr_t* psrc_word;
    uintptr_t* pdst_word;
    uint8_t* pps = (uint8_t*)src;
    uint8_t* ppd = (uint8_t*)dst;
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
    pdst_word = (uintptr_t*)ppd;
    psrc_word = (uintptr_t*)pps;

    /* Main loop. */
    sz >>= shift;
    while (sz--)
        *pdst_word++ = *psrc_word++;

    /* Epilogue. */
    ppd = (uint8_t*)pdst_word;
    pps = (uint8_t*)psrc_word;
    while (tail--)
        *ppd++ = *pps++;

    return s;
}

