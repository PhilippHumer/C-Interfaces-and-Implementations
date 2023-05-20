#pragma once

#include <stdlib.h>

void*   my_malloc(size_t size, const char* file,  const char* function, const int line);
void*   my_calloc(size_t count, size_t size, const char* file, const char* function, const int line);
void    my_free(void* block);

#ifndef IMPLEMENTATION
    #ifdef MEMORY_TRACKING
        #define malloc(size)        my_malloc(size, __FILE__, __FUNCTION__, __LINE__)
        #define calloc(count, size) my_calloc(count, size, __FILE__, __FUNCTION__, __LINE__)
    #elif
        #define malloc(size)        my_malloc(size, NULL, NULL, NULL)
        #define calloc(count, size) my_calloc(count, size, NULL, NULL, NULL)
    #endif
    #define free(block)         my_free(block)
#endif

