/*
Aligned malloc variation:
Expose an API which allocates the memory,
users give the input interms of allocated memory should be x bytes aligned.
There is another input from user, the returned address should be y bytes aligned.
*/
void* alloc_memory(size_t size,
                   size_t bytes_aligned,
                   size_t return_address_alignment)
{
// TODO
// Example:
// size = 80 bytes,
// x = allocation should be 64 bytes aligned => 64 + 64 => 128 bytes (80 bytes)
// y = return address should be 32 byte aligned.
    
    // size alignement with bytes_aligned
    size_t orig_size = size;
    size_t new_size = size + bytes_aligned; // 80 + 64 => 140
    new_size -= new_size % byte_aligned; // 144 - (144 % 64) => 144 - 16 => 128
    // new size = 128 bytes
    // original = 80 bytes
    // spare space: 48 bytes // 64 byte
    
    // return_address_alignment alignement
    
    // new_size = 128 bytes
    size_t extra_space_required = return_address_alignment + sizeof(void*); // 32 + 8 = 40
    size_t spare_space = new_size - size; // 48
    if (spare_space < extra_space_required) {
        // new_size += (return_address_alignment - spare_space); // 16 bytes
        new_size += bytes_aligned; // TODO: what if extra_space_required > bytes_aligned
    } else {
        // no increase required
    }
    
    //size_t alignment = return_address_alignment;
    //size_t extra = sizeof(void*);
    void* base = malloc(new_size);
    //size_t extra = return_address_alignment + sizeof(void*);
    
    size_t aux = ((size_t)base) + extra_space_required;
    aux = aux - (aux % return_address_alignment);
    void* ans = (void*)aux;
    *((uintptr_t*)ans - 1) = (uintptr_t)base;

    return ans;
}

void free_aligned(void* ptr) {
    free((void*)*((uintptr_t*)ptr - 1));
}
