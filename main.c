#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "sorted_list.h"
#include "tree.h"
#include "user_list.h"


#define zero_character_code 48
#define max_user_id 65535
#define max_movie_id 2147483647
#define COMMAND_MAX_LENGTH 32
#define MAX_WORD_LENGTH_IN_COMMAND 8
#define UNSIGNED_INT_MAX_LENGTH 6
#define LONG_INT_MAX_LENGTH 11

bool memory_error = false;
enum command {delUser, addMovie,delMovie,addUser,Marathon,NONE};

// users are stored in hash_table[1000] each element of hash_table is a User List keeping id's and pointers to nodes
// user with id = user_id is stored in list hash_table[user_id%1000] hence each User List in hash_table has length bounded by 66

void error()
{
    fprintf( stderr, "ERROR");
    fprintf(stderr,"\n");
}


void inform_valid_operation()
{
    printf("OK");
    putchar(10);
}


long int max(long int x, long int y)
{
    if(x >y)
    return x;
    else return y;
}


long int min(long int x, long int y)
{
    if(x <y)
        return x;
    else
        return y;
}


void go_to_the_end_of_line(char read_sign)
{
    while (read_sign != '\n')
    {
        read_sign = getchar();
    }
}


void array_copy(char array1[],int array2[], int array1_length)
//we assume values of array2 fit into the range of char
{
    for (int i = 0; i< array1_length;i++)
    {
        array1[i] = array2[i];
    }
}


void change_value_list(Sorted_List *list, long int value,long int *smallest_movie_rating_so_far)
/* this function is only used during marathon
If long int value is smaller then the smallest element of the list, function change_value_list swaps them and sorts the list
*/
{
    long int temp_value,temp_value_2;
    while (list->value > value)
    {
        list = list->next;
    }
    if ((list->value < value) && (list->value != -1))
    {
        temp_value = list->value;
        list->value = value;
        list=list->next;
        while (list->value != -1)
        {
                temp_value_2 = list->value;
                list->value = temp_value;
                list = list->next;
                *smallest_movie_rating_so_far = temp_value;
                temp_value = temp_value_2;
        }
        
    }
}


bool user_is_in_database(unsigned int user,List_User *hash_table[1000],List_User **user_node)
{
    return is_in_user_list(user,hash_table[user%1000],user_node);
}


bool add_user(unsigned int parent_user, unsigned int new_user,List_User *hash_table[1000])
// returns false in case of an error
{
    List_User *parent_user_node = NULL;
    List_User *new_user_node = NULL;
    if((user_is_in_database(parent_user,hash_table,&parent_user_node)) &&(!(user_is_in_database(new_user,hash_table,&new_user_node))))
    {
        Tree *new_user_tree_node = (Tree*) malloc(sizeof(Tree));
        if(new_user_node == NULL)
        {
            memory_error =true;
            return false;
        }
        else
        {
                add_node(parent_user_node->user_node,new_user,new_user_tree_node);
                insert_into_user_list(&(hash_table[new_user%1000]),new_user,new_user_tree_node);
                inform_valid_operation();
                return true;
        }
    }
    else
    {
        error();
        return false;
    }
}


void add_movie_rating(Tree *user_node, long int movie_rating,bool *was_in_list)
/*with assumption that user is in database    */
{
    insert_into_list(&(user_node->movies),movie_rating,was_in_list);
}


void delete_movie_rating(Tree *user_node, long int movie_rating)
/*with assumption that user is in database    */
{
    bool was_in_list = false;
    delete_from_list(&user_node->movies,movie_rating,&was_in_list);
    if (!was_in_list)
        error();
    else
        inform_valid_operation();
}



void operation(enum command which_operation, unsigned int user, long int movie_rating,List_User *hash_table[1000])
/* performs one of the following: delUser,addMovie,delMovie  */
{
    List_User *user_list_node = NULL;
    bool was_in_list;
    if (user_is_in_database(user,hash_table,&user_list_node))
    {
        switch (which_operation)
        {
            case delUser:
                {
                    delete_user_from_tree(user_list_node->user_node);
                    delete_from_user_list(&hash_table[user%1000], user);
                    inform_valid_operation();
                }
                break;
            case addMovie:
                {
                    add_movie_rating(user_list_node->user_node,movie_rating,&was_in_list);
                    if ((!memory_error) &&(!was_in_list))
                        inform_valid_operation();
                    else
                        error();
                }
                break;
            case delMovie:
                    delete_movie_rating(user_list_node->user_node,movie_rating);
                    // here inform_valid_operation() is executed in delete_movie_rating() function
                break;
            default:
                break;
        }
    }
    else
        error();
}


void marathon_add_movie_to_list(long int k, long int *number_chosen_movies,Sorted_List **chosen_movies,long int movie_rating,long int *smallest_movie_rating_so_far)
{
    bool was_in_list;

    if (k> *number_chosen_movies)
    {
        insert_into_list(chosen_movies,movie_rating,&was_in_list);
        if (!was_in_list)
        {
            *number_chosen_movies = *number_chosen_movies +1;
        }
        *smallest_movie_rating_so_far = min(*smallest_movie_rating_so_far,movie_rating);
    }
    else
    //we already have k candidates
    {
        change_value_list(*chosen_movies, movie_rating,smallest_movie_rating_so_far);
    }
}


void marathon(Tree *user_node, long int k, Sorted_List **chosen_movies, long int *number_chosen_movies, long int father_max_rating, long int *smallest_chosen_movie_so_far)
{
    Sorted_List *user_ratings = user_node->movies;
    long int user_max_rating = user_ratings->value;
    long int counter =0;
    while(( user_ratings->value> father_max_rating) && (counter <= k))
    //we are only interested in k greatest movie rating
    {
        marathon_add_movie_to_list(k, number_chosen_movies,chosen_movies, user_ratings->value,smallest_chosen_movie_so_far);
        counter++;
        user_ratings = user_ratings->next;
    }
    Tree* child = user_node->left_son;
    while( child!= NULL)
    {
        marathon(child,k,chosen_movies, number_chosen_movies,max(father_max_rating,user_max_rating),smallest_chosen_movie_so_far);
        child= child->right_brother;
    }
}


void marathon_depict_results(Sorted_List *chosen_movies,long int k)
{
    if(chosen_movies -> value == -1 )
    {
        printf("NONE");
        putchar(10);
    }
    else
    {
        bool first_one = true;
        long int counter = 0;
        while((chosen_movies->value != -1)&&(counter < k))
        {
            if(!first_one)
                putchar(' ');
            else 
              first_one = false;
            printf("%ld",chosen_movies->value);
            counter ++;
            chosen_movies = chosen_movies -> next;
        }
        putchar(10);
    }
}


void begin_marathon(Tree *user_node, long int k)
{
    Sorted_List* chosen_movies= initialize_list();
    long int number_chosen_movies = 0;
    long int smallest_chosen_movie_so_far = -1;
    marathon(user_node,k,&chosen_movies,&number_chosen_movies,-1,&smallest_chosen_movie_so_far);   
    marathon_depict_results(chosen_movies,k);
    erase_list(chosen_movies);
}



void operation_marathon(unsigned int user_id, long int k, List_User *hash_table[1000])
{

    List_User *user_list_node = NULL;
    if(user_is_in_database(user_id,hash_table,&user_list_node))
        begin_marathon(user_list_node->user_node, k);
    else
        error();
}


bool is_long_int(int array[], int start)
{
// checks whether number written in array[start],.. is between 0 and max_movie_id
    long long int number = 0;
    int length_of_number = 0;
    bool not_a_number = false;
    int counter = start;
    if((array[counter] == zero_character_code&& !((array[counter+1] > zero_character_code +9) ||(array[counter+1] < zero_character_code))) || array[counter] == ' ' || array[counter] == '\n')
    //numbers begining with zero are treated as invalid (except for 0)
        not_a_number = true;
    while ((length_of_number<=LONG_INT_MAX_LENGTH) && (array[counter]!=' ' )&&( !not_a_number)&&(array[counter] != '\n'))
    {
        if( (array[counter] > zero_character_code +9) ||(array[counter] < zero_character_code) )
            not_a_number = true;
        else
            {
                number = 10*number + array[counter]-zero_character_code;
                counter ++;
            }
    }
    if(!not_a_number)
        not_a_number= (number>max_movie_id);
    return !not_a_number;
}


bool is_unsigned_int(int array[], int start)
// checks whether number written in array[start],... is between 0 and max_user_id
{

    long int number = 0;
    int length_of_number = 0;
    bool not_a_number = false;
    int counter = start;

    if((array[counter] == zero_character_code &&!( (array[counter+1] > zero_character_code +9) ||(array[counter+1] < zero_character_code) )) || array[counter] == ' ' || array[counter] == '\n')
        not_a_number = true;
    while( (length_of_number<=UNSIGNED_INT_MAX_LENGTH) && (array[counter]!=' ' )&&( !not_a_number) && (array[counter] != '\n'))
    {
        if( (array[counter] > zero_character_code +9) ||(array[counter] < zero_character_code) ){
            not_a_number = true;}
        else
            {
                number = 10*number + array[counter]-zero_character_code;
                counter ++;
            }
    }
    if(!not_a_number)
        not_a_number= (number>max_user_id);
    return !not_a_number;
}


unsigned int read_id(int array[],int start,int *end)
{

    unsigned int id =0;
    int counter = start;
    while((array[counter] != ' ') && (array[counter] != '\n'))
    {
        id = 10*id + (array[counter]-zero_character_code);
        counter++;
    }
    *end = counter;

    return id;
}


long int read_movie_rating(int array[],int start,int *end)
{ 
    long int rating = 0;
    int counter = start;
    while((array[counter] != ' ') && (array[counter] != '\n'))
    {
        rating = 10*rating + (array[counter]-zero_character_code);
        counter++;
    }
    *end = counter;
    return rating;
}


void read_addUser(int line[COMMAND_MAX_LENGTH],List_User *hash_table[1000] )
{
    unsigned int parent_user,new_user;
    int end_of_user_id;
    bool valid_operation = false;
    if( is_unsigned_int(line,MAX_WORD_LENGTH_IN_COMMAND))
        {
            parent_user = read_id(line,MAX_WORD_LENGTH_IN_COMMAND ,&end_of_user_id);
            if(is_unsigned_int(line,end_of_user_id+1))
            {
                new_user = read_id(line,end_of_user_id+1,&end_of_user_id);
                if(line[end_of_user_id] == '\n')
                {
                    valid_operation = true;
                    add_user( parent_user, new_user,hash_table);
                }
            }
        }
    if(!valid_operation)
        error();
}


void read_delUser(int line[COMMAND_MAX_LENGTH],List_User *hash_table[1000])
{
    unsigned int    user;
    int end_of_user_id;
    if( is_unsigned_int(line,MAX_WORD_LENGTH_IN_COMMAND))
    {
        user = read_id(line,MAX_WORD_LENGTH_IN_COMMAND,&end_of_user_id);
        if((user != 0) && (line[end_of_user_id] == '\n'))
           operation(delUser,  user, 0,hash_table); 
        else
            error();
    }
    else
        error();
}


void read_addMovie(int line[COMMAND_MAX_LENGTH],List_User *hash_table[1000])
{
    unsigned int user;
    int end_of_user_id;
    long int movie_rating;
    bool valid_operation = false;
    if( is_unsigned_int(line,MAX_WORD_LENGTH_IN_COMMAND +1))
    {
        user = read_id(line,MAX_WORD_LENGTH_IN_COMMAND +1,&end_of_user_id);
        if(is_long_int(line,end_of_user_id+1))
        {
            movie_rating = read_movie_rating(line,end_of_user_id+1,&end_of_user_id);
            if(line[end_of_user_id] == '\n')
            {
                valid_operation = true;
                operation(addMovie,user,movie_rating,hash_table);
            }
        }
    }
    if(!valid_operation)
        error();
}


void read_delMovie(int line[COMMAND_MAX_LENGTH],List_User *hash_table[1000])
{
    unsigned int user;
    int end_of_user_id;
    long int movie_rating;
    bool valid_operation = false;
    if( is_unsigned_int(line,MAX_WORD_LENGTH_IN_COMMAND +1))
    {
        user = read_id(line,MAX_WORD_LENGTH_IN_COMMAND +1,&end_of_user_id);
        if(is_long_int(line,end_of_user_id+1))
        {
            movie_rating = read_movie_rating(line,end_of_user_id+1,&end_of_user_id);
            if(line[end_of_user_id] == '\n')
            {
                valid_operation = true;
                operation(delMovie,user,movie_rating,hash_table);
            }
        }       
    }
    if(!valid_operation)
        error();
}


void read_marathon(int line[COMMAND_MAX_LENGTH],List_User *hash_table[1000])
{
    unsigned int user;
    int end_of_user_id;
    long int number_of_movies;
    bool valid_operation = false;
    if( is_unsigned_int(line,MAX_WORD_LENGTH_IN_COMMAND +1))
    {
        user = read_id(line,MAX_WORD_LENGTH_IN_COMMAND +1,&end_of_user_id);
        if(is_long_int(line,end_of_user_id+1))
        {
            number_of_movies = read_movie_rating(line,end_of_user_id+1,&end_of_user_id);
            if(line[end_of_user_id]== '\n')
            {
                valid_operation = true;
                operation_marathon(user,number_of_movies,hash_table);
            }          
        }     
    }
    if(!valid_operation)
        error();
}


void interpret_command(int line[COMMAND_MAX_LENGTH],List_User *hash_table[1000])
{
        char command[MAX_WORD_LENGTH_IN_COMMAND +1];
        array_copy(command,line,MAX_WORD_LENGTH_IN_COMMAND );
        command[MAX_WORD_LENGTH_IN_COMMAND ] = '\0';
        if( strcmp(command, "addUser ")==0)          
             read_addUser(line,hash_table);
        else if (strcmp(command, "delUser ") == 0)
             read_delUser(line,hash_table);
        else if(strcmp(command,"addMovie")==0)
             read_addMovie(line,hash_table);
        else if (strcmp(command,"delMovie")==0)
             read_delMovie(line,hash_table);
        else if (strcmp(command,"marathon")==0)
             read_marathon(line,hash_table);
        else
        {
            error();
        }

}


void read_one_line(bool *end_of_instructions,List_User *hash_table[1000])
{
    
    int read_sign;
    int line[COMMAND_MAX_LENGTH];
    int char_count = 0;
    read_sign = getchar();   
    if(read_sign == EOF)
        *end_of_instructions = true;
    
    else
    {
      while((read_sign!= '\n')&&(char_count<COMMAND_MAX_LENGTH) &&(read_sign != EOF))
      {
            line[char_count] = read_sign;
            read_sign = getchar();
            char_count++;
      }
      line[char_count] = read_sign;
      
      if( !((line[0]=='#')||(line[0] == '\n')))
      {
            if((read_sign!= '\n' )||(char_count <= 8))
            {
                go_to_the_end_of_line(read_sign);
                error();
            }
            else
                interpret_command(line,hash_table);
      }
      else
            go_to_the_end_of_line(read_sign);
    }
}


int program()
{

    Tree *zero_user_node = (Tree*)malloc(sizeof(Tree));
    zero_user_node->user_id = 0;
    zero_user_node->left_brother=NULL;
    zero_user_node->left_son= NULL;
    zero_user_node->right_son = NULL;
    zero_user_node->right_brother = NULL;
    zero_user_node->movies= initialize_list();
    List_User* hash_table[1000];
    for(int i =0;i<= 999; i++)
    {
        hash_table[i] = initialize_user_list(zero_user_node);
    }
    bool end_of_instructions = false;
    
    while(!end_of_instructions && !memory_error)    
        read_one_line(&end_of_instructions,hash_table); 
 
    delete_tree(zero_user_node);
    for(int i=0;i<1000;i++)
    {
        erase_user_list(hash_table[i]);
    }
    if(memory_error)
        return 1;
    else
        return 0;
}


int main(void)
{
    int returned_value = program();
    return  returned_value;
}

