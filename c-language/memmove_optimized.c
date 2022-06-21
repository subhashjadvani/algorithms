void *my_memmove(void *dest, const void *src, unsigned int n)
{
    unsigned char isCopyRequire = 0;  //flag bit
    char *pcSource =(char *)src;
    char *pcDstn =(char *)dest;
    // return if pcDstn and pcSource is NULL
    if ((pcSource == NULL) || (pcDstn == NULL))
    {
        return NULL;
    }
    // overlap buffer
    if((pcSource < pcDstn) && (pcDstn < pcSource + n))
    {
        for (pcDstn += n, pcSource += n; n--;)
        {
            *--pcDstn = *--pcSource;
        }
    }
    else
    {
        while(n--)
        {
            *pcDstn++ = *pcSource++;
        }
    }
    return dest;
}


void *my_memmove_overlap(void *dest, const void *src, unsigned int n) {
    char *pps =(char *)src + n;
    char *ppd =(char *)dest + n;
    uintptr_t *pd, *ps;
    
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
    while (((unsigned int)pps & (bytes_per_word-1)) &&
           ((unsigned int)ppd & (bytes_per_word-1)) &&
           sz--)
        *--ppd = *--pps;

    int tail = sz & (bytes_per_word - 1);
    pd = (uintptr_t*)ppd;
    ps = (uintptr_t*)pps;

    /* Main loop. */
    sz >>= log(bytes_per_word);
    while (sz--)
        *--pd = *--ps;

    /* Epilogue. */
    ppd = (byte*)pd;
    pps = (byte*)ps;
    while (tail--)
        *--ppd = *--pps;
}

void *my_memmove(void *dest, const void *src, unsigned int n)
{
    unsigned char isCopyRequire = 0;  //flag bit
    char *pcSource =(char *)src;
    char *pcDstn =(char *)dest;
    
    // return if pcDstn and pcSource is NULL
    if ((pcSource == NULL) || (pcDstn == NULL))
    {
        return NULL;
    }
    
    // overlap buffer
    if ((pcSource < pcDstn) && (pcDstn < pcSource + n))
    {
        my_memmove_overlap(dest, src, n);
    }
    else
    {
        memcpy(pcDstn, pcSource, n);
    }
    
    // dst: 0 1 2 3 4 5 6 7
    // src:     2 3 4 5 6 7 9 8
    
    return dest;
}

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
