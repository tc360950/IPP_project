#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "sorted_list.h"
#include "tree.h"


void add_node(Tree *parent_node,unsigned int new_node_value,Tree *new_node)
{
		new_node->user_id = new_node_value;
                new_node->left_son = NULL;
                new_node->right_son = NULL;
                new_node->left_brother = NULL;
                new_node->movies = initialize_list();
                new_node->right_brother = parent_node->left_son;
                parent_node->left_son = new_node;

                if(parent_node->right_son == NULL)
                    parent_node->right_son = new_node;
                else
                    new_node->right_brother->left_brother = new_node;

                new_node->father = parent_node;
}

void delete_user_who_has_children(Tree *user_node)
{
	user_node->left_son->left_brother = user_node->left_brother;
        user_node->right_son->right_brother = user_node->right_brother;
        if(user_node -> left_brother == NULL)
        {
            user_node->father->left_son = user_node->left_son;
            user_node->left_son->father = user_node->father;
        }
        else
        {
            user_node->left_brother->right_brother = user_node->left_son;
        }
        if(user_node->right_brother == NULL)
        {
            user_node->father->right_son = user_node->right_son;
            user_node->right_son->father = user_node->father;
        }
        else
        {
            user_node->right_brother->left_brother= user_node->right_son;
        }
  
}

void delete_user_without_children(Tree *user_node)
{	
   if(user_node->right_brother != NULL)
        {
            user_node->right_brother->left_brother = user_node->left_brother;
            if(user_node->left_brother != NULL)
            {
                user_node->left_brother->right_brother=user_node->right_brother;
            }
            else
            {
                user_node->right_brother->father = user_node->father;
                user_node->father->left_son = user_node->right_brother;
            }
        }
        else
        // user has no right brother
        {
            if(user_node->left_brother == NULL)
            {
                user_node->father->left_son = NULL;
                user_node->father->right_son = NULL;
            }
            else
            {
                user_node->left_brother->right_brother = NULL;
                user_node->father->right_son = user_node->left_brother;
                user_node->left_brother->father = user_node->father;
            }
        }
  
}
void delete_user_from_tree(Tree *user_node)
{
    erase_list(user_node->movies);
    if(user_node->left_son !=NULL)      
	delete_user_who_has_children(user_node);
    else
       delete_user_without_children(user_node);
       
    free(user_node);
}



void delete_tree(Tree *node)
{
    if(node!= NULL)
    {
        Tree *child = node->left_son;
        Tree *temp;
        while(child != NULL)
        {
            temp = child->right_brother;
            delete_tree(child);
            child = temp;
        }
     erase_list(node->movies);
    free(node);
    }
}

