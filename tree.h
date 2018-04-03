#ifndef TREE_H
#define TREE_H

typedef struct tree Tree;
struct tree{
    unsigned int  user_id;
    Sorted_List *movies;
    struct tree *father,*left_son,*left_brother,*right_brother,*right_son;

};

void add_node(Tree *parent_node,unsigned int new_node_value,Tree *new_node);

void delete_user_who_has_children(Tree *user_node);


void delete_user_without_children(Tree *user_node);
	

void delete_user_from_tree(Tree *user_node);

void delete_tree(Tree *node);
#endif
