#include "allocator.h"
#include <cassert>
#include <memory>
#include <iostream>

namespace washu {
    namespace {
        struct allocation_node {
            allocation_node * Next;
            allocation_node * Prev;
            std::type_info const * Info;
            char const * From;
            void * Ptr;
            size_t Length;
            size_t Line;
        };

        struct header {
            allocation_node * AllocationInfo;
            size_t Length;
        };

        allocation_node * root = 0;
    }

    void * allocator::allocate (size_t bytes, std::type_info const & info, char const * from, size_t line) {
        size_t padding = 16 - sizeof( header );

#ifdef __APPLE__    /* MacOSX guarantees that all memory allocated is 16-byte aligned */
        void * ptr = malloc (bytes + sizeof(header) +padding);
#else
        void * ptr = _aligned_malloc (bytes + sizeof(header) +padding, 16);
#endif

        header * header = reinterpret_cast<washu::header *>( ptr );
        header->Length = bytes;

        header->AllocationInfo = reinterpret_cast<allocation_node *>( malloc (sizeof( allocation_node )) );
        header->AllocationInfo->Info = &info;
        header->AllocationInfo->From = from;
        header->AllocationInfo->Line = line;
        header->AllocationInfo->Next = nullptr;
        header->AllocationInfo->Ptr = reinterpret_cast<char *>(ptr) +16;
        header->AllocationInfo->Length = bytes;

        if (root) {
            root->Next = header->AllocationInfo;
        }

        header->AllocationInfo->Prev = root;
        root = header->AllocationInfo;

        return reinterpret_cast<char *>(ptr) +16;
    }

    void allocator::release (void * ptr, char const * from, size_t line) {
        char * realPtr = reinterpret_cast<char *>(ptr) -16;

        header * header = reinterpret_cast<washu::header *>( realPtr );
        if (header->AllocationInfo->Next) {
            header->AllocationInfo->Next->Prev = header->AllocationInfo->Prev;
        }

        if (header->AllocationInfo->Prev) {
            header->AllocationInfo->Prev->Next = header->AllocationInfo->Next;
        }

        if (header->AllocationInfo == root) {
            root = header->AllocationInfo->Prev;
        }

        free (header->AllocationInfo);
#ifdef __APPLE__
        free (realPtr);
#else
        _aligned_free (realPtr);
#endif
    }

    void allocator::print_leaks ( ) {
        allocation_node * p = root;
        while (p) {
            std::cout << p->Ptr << " [" << p->Info->name ( ) << ", " << p->Length << "] from: " << p->From << ":" << p->Line << std::endl;
            p = p->Prev;
        }
    }
}