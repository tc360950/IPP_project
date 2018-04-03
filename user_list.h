



#ifndef USER_LIST_H
#define USER_LIST_H




struct list_user{
    unsigned int user_id;
    Tree *user_node;
    struct list_user *next;
};
typedef struct list_user List_User;



void insertAfter_user_list(List_User *list,unsigned int id,Tree *user_node);



List_User *initialize_user_list(Tree *zero_user_node);



void insert_into_user_list(List_User **list,unsigned int user,Tree *node);




void erase_user_list(List_User *list);


void delete_from_user_list(List_User **list, unsigned int user);




bool is_in_user_list(unsigned int user, List_User *list,List_User **user_node);

#endif
