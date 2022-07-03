/* thread safe memset() */

/* Warning: following is just a pseudo code */

mutex_t lock = init;

typedef struct {
    void *start_addr;
    size_t size;
    Node *next;
} Node;

Node *head;
void check_overlap(void *dst, size_t size)
{
    mutex_lock();
    while (is_dst_preset_in_ll(dst, size)) {
        cond_wait(&cond, &mutex);
    }
    // add it to the linked list
    mutex_unlock();
}

void memset(void *dst, int val, size_t size)
{

    check_overlap(dst, size);
 
    while (size--)
        *dst++ = val & 0xff;

    // remove my block from the linked list
    cond_signal_broadcast();
}

thread1()
{
    memset(0x1000, 0x0, 0x100);
}

thread2()
{
    memset(0x1050, 0x1, 0x100);
}

thread3()
{
    memset(0x2000, random(), 0x100);
}
