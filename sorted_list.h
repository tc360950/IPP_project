#ifndef SORTED_LIST_H
#define SORTED_LIST_H
struct list{
//Sorted list containing movie ratings
    long int value;
    struct list *next;
};

typedef struct list Sorted_List;

void insertAfter(Sorted_List *list,long int number);

Sorted_List *initialize_list(void);

void insert_into_list(Sorted_List **list,long int number,bool *was_in_list);

void erase_list(Sorted_List *list);

void delete_from_list(Sorted_List **list, long int number,bool *was_in_list);



#endif 
