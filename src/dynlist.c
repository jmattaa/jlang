#include "include/dynlist.h"

dynlist *Dynlist_Init(size_t item_size)
{
    dynlist *list = calloc(1, sizeof(dynlist));

    list->items = 0;
    list->item_size = item_size;
    list->size = 0;

    // varlist specific
    list->current_offset = 0;

    return list;
}

void Dynlist_Append(dynlist *list, void *item)
{
    list->size += 1;

    if (!list->items)
        list->items = calloc(1, list->item_size);
    else
        list->items = realloc(list->items, list->item_size * list->size);

    list->items[list->size - 1] = item;
}

void Dynlist_Freelist(dynlist *list)
{
    if (list->items)
        free(list->items);
    if (list)
        free(list);
}
