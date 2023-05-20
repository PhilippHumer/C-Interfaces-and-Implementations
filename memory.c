#define IMPLEMENTATION
#include "memory.h"
#undef IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef struct mem_node
{
    void* data;
    char* file;
    char* function;
    int line;
    struct mem_node* next;
} mem_node;

static struct{
    mem_node* first;
    mem_node* last;
} memory;

static void init_memory();
static mem_node* create_mem_node(void* data, const char* file, const char* function, const int line);

void* my_malloc(size_t size, const char* file,  const char* function, const int line)
{
    void* data = malloc(size);

    #ifdef MEMORY_TRACKING
        mem_node* newNode = create_mem_node(data, file, function, line);

        if (memory.first == NULL)
        {
            init_memory();
            memory.first = newNode;
            memory.first->next = newNode;
            memory.last = newNode;
        }
        else
        {
            memory.last->next = newNode;
            memory.last = newNode;
        }
    #endif

    return data;
}

void* my_calloc(size_t count, size_t size, const char* file,  const char* function, const int line)
{
    return my_malloc(count * size, file, function, line);
}

void my_free(void* block)
{
    free(block);

    #ifdef MEMORY_TRACKING
        mem_node* ptr = memory.first;

        if (ptr == NULL)
            return;

        if (ptr == memory.last)
        {
            free(ptr->file);
            free(ptr->function);
        }
        else
        {
            while (ptr->next->data == block)
            {
                ptr = ptr->next;
            }

            mem_node* tmp = ptr->next->next;
            free(ptr->next->file);
            free(ptr->next->function);
            free (ptr->next);
            ptr->next = tmp;
        }
    #endif
}

static void init_memory()
{
    memory.first = NULL;
    memory.last = NULL;
}

static mem_node* create_mem_node(void* data, const char* file, const char* const function, const int line)
{
    mem_node* new_node = (mem_node*) malloc(sizeof(mem_node));
    assert(new_node);

    new_node->function = (char*) calloc(strlen(function) + 1, sizeof(char));
    new_node->file = (char*) calloc(strlen(file) + 1, sizeof(char));
    assert(new_node->function);
    assert(new_node->file);

    new_node->data = data;
    new_node->line = line;
    strcpy_s(new_node->function, strlen(function) + 1, function);
    strcpy_s(new_node->file, strlen(file) + 1, file);
    new_node->next = NULL;

    return new_node;
}