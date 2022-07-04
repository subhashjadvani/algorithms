/*
 * CREDITs:
 *   - This based on paper "Fast Efficient Fixed-Size Memory Pool" (https://www.thinkmind.org/download.php?articleid=computation_tools_2012_1_10_80006)
 *   - Prabhu Chinnah took the original implementation from above paper and simplified little (stores the pointers instead of index for the free block pointers).
 */

#include <stdint.h>
#include <cstddef>
#include <iostream>

class FixedMemPoolAlloc {
    private:
        uint8_t* _memPool;
        uintptr_t* _next_blk;
        uintptr_t _blockSize;
        uint32_t _maxBlocks;
        uint32_t _numOfFreeBlocks;
        uint32_t _numOfInitializedBlocks;

    public:
        FixedMemPoolAlloc(size_t blockSize, uint32_t maxBlocks) {

            _maxBlocks = maxBlocks;
            _numOfFreeBlocks = _maxBlocks;
            _blockSize = blockSize;
	    // minimum space to store next address or index        
            if (_blockSize < sizeof(uintptr_t))
                _blockSize = sizeof(uintptr_t);
            _memPool = new uint8_t[_blockSize * _maxBlocks];
            _numOfInitializedBlocks = 0;
            _next_blk = (uintptr_t*)_memPool;
        }

        ~FixedMemPoolAlloc() {
            delete[] _memPool;
            _memPool = nullptr;
        }

        // allocates one block
        void* Allocate() {
            
            if (_numOfInitializedBlocks < _maxBlocks) {
                uintptr_t* ptr = (uintptr_t*)((uintptr_t)_memPool + (_numOfInitializedBlocks * _blockSize));
                *ptr = ((uintptr_t)ptr + _blockSize);
                _numOfInitializedBlocks++;
            }

            void* result = nullptr;
            if (_numOfFreeBlocks > 0) {
                _numOfFreeBlocks--;
                result = (void*) _next_blk;

                if (_numOfFreeBlocks > 0) {
                    _next_blk = (uintptr_t*)(*_next_blk);         //move the next ptr to next block
                }
                else {
                    _next_blk = nullptr;
                }
            }
            return result;
        }

        void DeAllocate(void * ptr) {

            if (_numOfFreeBlocks > 0) {
                (*(uintptr_t*)ptr) = (uintptr_t)_next_blk;
            }
            //else - incoming ptr is the only block available

            _next_blk = (uintptr_t*)ptr;
            _numOfFreeBlocks++;
        }
};

int main()
{
}
