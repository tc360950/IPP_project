#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "sorted_list.h"

extern bool memory_error;


void insertAfter(Sorted_List *list,long int number)
//list is never empty because it is initialized with a pivot
{
    Sorted_List *new_element = (Sorted_List*) malloc(sizeof(Sorted_List));
    if (new_element == NULL)
        memory_error = true;
    else
    {
        new_element->next = list->next;
        new_element->value = number;
        list->next = new_element;
    }
}


Sorted_List *initialize_list(void)
{
    Sorted_List *list = (Sorted_List*) malloc(sizeof(Sorted_List));
    if (list == NULL)
        memory_error = true;
    else{
    list->value =-1;
    //values stored in the list will be positive
    list->next = NULL;
    }
    return list;
}


void insert_into_list(Sorted_List **list,long int number,bool *was_in_list)
//without repetition hence does nothing if number already is in list
{
    *was_in_list = true;
    Sorted_List *previous = (Sorted_List*) malloc(sizeof(Sorted_List));
    if (previous == NULL)
        memory_error = true;
    else
    {
        previous->next = (*list);
        Sorted_List *temp = previous;
        Sorted_List *present = (*list);
        (*list)= previous;
        while ( present->value > number)
        {
            previous =present;
            present = present->next;
        }
        if (present->value != number)
        {
            insertAfter(previous,number);
            *was_in_list = false;
        }
        (*list)=(*list)->next;
        free(temp);
    }
}


void erase_list(Sorted_List *list)
{
    Sorted_List *previous = list;
    while (list->next != NULL)
    {
        list=list->next;
        free(previous);
        previous = list;
    }
    free(list);
}


void delete_from_list(Sorted_List **list, long int number,bool *was_in_list)
{
    *was_in_list = false;
    Sorted_List *previous = (*list);
    Sorted_List *first = (*list);
    if ((*list)->value == number)
    {
        (*list)=(*list)->next;
        free(previous);
        *was_in_list = true;
    }
    else
        {
            while (( (*list)!=NULL) && ((*list)->value !=number))
            {
                previous = (*list);
                (*list) = (*list)->next;
            }
            if ((*list)!= NULL)
            {
                previous->next = (*list)->next;
                free(*list);
                *was_in_list = true;
            }
            (*list) = first;
    }
}
