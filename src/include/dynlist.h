#ifndef JLANG_DYNLIST_H
#define JLANG_DYNLIST_H

#include <stdlib.h>

// dynamic list
typedef struct
{
    void **items;
    size_t item_size;
    size_t size;
} dynlist;

// create the dynlist obj
dynlist *Dynlist_Init(size_t item_size);
// push to list
void Dynlist_Append(dynlist *list, void *item);

#endif

