
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "sorted_list.h"

#include "tree.h"
#include "user_list.h"


extern bool memory_error;




void insertAfter_user_list(List_User *list,unsigned int id,Tree *user_node)
{
    List_User *new_element = (List_User*) malloc(sizeof(List_User));
    if (new_element == NULL)
        memory_error = true;
    else
    {
        new_element->next = list->next;
        new_element->user_id = id;
        new_element->user_node = user_node;
        list->next = new_element;
    }
}



List_User *initialize_user_list(Tree *zero_user_node)
{
    List_User *list = (List_User*) malloc(sizeof(List_User));
    if (list == NULL)
        memory_error = true;
    else
    {
    list->user_id =0;
    //each list has a pivot beacause we know its values will be positive
    list->next = NULL;
    list->user_node= zero_user_node;
    }
    return list;
}



void insert_into_user_list(List_User **list,unsigned int user,Tree *node)
//list is never empty
{
    List_User *previous = (List_User*) malloc(sizeof(List_User));
    if (previous == NULL)
        memory_error = true;
    else
    {
        previous->next = (*list);
        List_User *temp = previous;
        List_User *present = (*list);
        (*list)= previous;
        while ( present->user_id > user)
        {
            previous =present;
            present = present->next;
        }
        if (present->user_id != user)
        {
            insertAfter_user_list(previous,user,node);
        }
        (*list)=(*list)->next;
        free(temp);
    }
}




void erase_user_list(List_User *list)
{
    List_User *previous = list;
    while (list->next != NULL)
    {
        list=list->next;
        free(previous);
        previous = list;
    }
    free(list);
}


void delete_from_user_list(List_User **list, unsigned int user)
{
    List_User *previous = (*list);
    List_User *first = (*list);
    if ((*list)->user_id == user)
    {
        (*list)=(*list)->next;
        free(previous);
    }
    else{
        while (( (*list)!=NULL) && ((*list)->user_id !=user))
        {
            previous = (*list);
            (*list) = (*list)->next;
        }
        if ((*list)!= NULL)
        {
            previous->next = (*list)->next;
            free(*list);
        }
    (*list) = first;
    }
}




bool is_in_user_list(unsigned int user, List_User *list, List_User **user_node)
{
    while((list->user_id != user)&&(list->user_id != 0))
        list = list->next;
    (*user_node)= list;
    return
            (list->user_id == user);
}
