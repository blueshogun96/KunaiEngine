#pragma once
#include <typeinfo>

namespace washu {
    class allocator {
    public:
        static void * allocate (size_t bytes, std::type_info const & info, char const * from, size_t line);
        static void release (void * ptr, char const * from, size_t line);
        static void print_leaks ( );

        template<class T>
        static void release (T * ptr, char const * from, size_t line) {
            ptr->~T ( );
            release (reinterpret_cast<void *>( ptr ), from, line);
        }

        template<class T>
        static T * allocate_array (size_t count, char const * from, size_t line) {
            T * arrayPtr = reinterpret_cast<T*>( allocate (sizeof(T) * count, typeid( T ), from, line) );
            for (size_t i = 0; i < count; ++i) {
                new ( arrayPtr + i ) T ();
            }

            return arrayPtr;
        }

        template<class T>
        static void release_array (T * ptr, char const * from, size_t line) {
            char * originPtr = reinterpret_cast<char *>(ptr) -16;
            size_t count = *reinterpret_cast<size_t *>( originPtr + sizeof( void * ) ) / sizeof( T );
            for (size_t i = 0; i < count; ++i) {
                ( ptr + i )->~T ( );
            }

            release (ptr, from, line);
        }
    };
}

#ifdef ENABLE_ALLOCATION_LOGGING
#define NEW(Type) new (washu::allocator::allocate(sizeof(Type), typeid(Type), __FILE__, __LINE__)) Type
#define NEW_ARRAY(Type, Count) washu::allocator::allocate_array<Type>(Count, __FILE__, __LINE__)
#define DEL(Ptr) washu::allocator::release(Ptr, __FILE__, __LINE__)
#define DEL_ARRAY(Ptr) washu::allocator::release_array(Ptr, __FILE__, __LINE__)
#else
#define NEW(Type) new Type
#define DEL(Ptr) delete Ptr;
#define NEW_ARRAY(Type, Count) new Type[Count]
#define DEL_ARRAY(Ptr) delete [] Ptr
#endif
