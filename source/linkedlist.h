//
//  linkedlist.inl
//
//  Created by Shogun3D on 11/30/13.
//  Copyright (c) 2013 Shogun3D. All rights reserved.
//

#include <stdio.h>
#include "linkedlist.hpp"


/*
 * A C++ port of the old C based linked list.
 * You can write your own customized node classes while inheriting from node_t
 * as the base class.  Just remember to take care of any class specific uninitialization
 * in the deconstructor, of course.
 */

/*
 * Usage notes:
 * When iterating through a linked list with the possibility of deleting nodes,
 * it's important get the next node BEFORE you delete anything! I found this out
 * the hard way, and it was a hassle.
 */



/* Add a node at the beginning of the list */
template <class T>
inline void list_add_beginning( node_t<T>** head, T data )
{
	node_t<T>* temp;
    
	temp = new node_t<T>;
	temp->data = data;
    
	if( (*head) == NULL )
	{
		(*head) = temp;
		(*head)->next = NULL;
	}
	else
	{
		temp->next = (*head);
		(*head) = temp;
	}
}

/* Add list node to the end */
template <class T>
inline void list_add_end( node_t<T>** head, T data )
{
	node_t<T>* temp1;
	node_t<T>* temp2;
    
	temp1 = new node_t<T>;
	temp1->data = data;
    
	temp2 = (*head);
    
	if( (*head) == NULL )
	{
		(*head) = temp1;
		(*head)->next = NULL;
	}
	else
	{
		while( temp2->next != NULL )
			temp2 = temp2->next;
        
		temp1->next = NULL;
		temp2->next = temp1;
	}
}

/* Add a new node at a specific position */
template <class T>
inline void list_add_at( node_t<T>** head, T data, int loc )
{
	int i;
	node_t<T> *temp, *prev_ptr, *cur_ptr;
    
	cur_ptr = (*head);
    
	if( loc > (list_length( head )+1) || loc <= 0 )
	{
	}
	else
	{
		if( loc == 1 )
		{
			list_add_beginning(head, data);
		}
		else
		{
			for( i = 1; i < loc; i++ )
			{
				prev_ptr = cur_ptr;
				cur_ptr = cur_ptr->next;
			}
            
			temp = new node_t<T>;
			temp->data = data;
            
			prev_ptr->next = temp;
			temp->next = cur_ptr;
		}
	}
}


/* Returns the number of elements in the list */
template <class T>
inline int list_length( node_t<T>** head )
{
	node_t<T>* cur_ptr;
	int count= 0;
    
	cur_ptr = (*head);
    
	while( cur_ptr != NULL )
	{
		cur_ptr = cur_ptr->next;
		count++;
	}
    
	return count;
}


/* Delete a node from the list */
template <class T>
inline int list_delete( node_t<T>** head, T data )
{
	node_t<T> *prev_ptr, *cur_ptr;
    
	cur_ptr = (*head);
    
	while( cur_ptr != NULL )
	{
		if( cur_ptr->data == data )
		{
			if( cur_ptr == (*head) )
			{
				(*head) = cur_ptr->next;
				delete cur_ptr;
				return 1;
			}
			else
			{
				prev_ptr->next = cur_ptr->next;
				delete cur_ptr;
				return 1;
			}
		}
		else
		{
			prev_ptr = cur_ptr;
			cur_ptr = cur_ptr->next;
		}
	}
    
	return 0;
}

/* Deletes a node from the given position */
template <class T>
inline int list_delete_loc( node_t<T>** head, int loc )
{
	node_t<T> *prev_ptr, *cur_ptr;
	int i;
    
	cur_ptr = (*head);
    
	if( loc > list_length( head ) || loc <= 0 )
	{
	}
	else
	{
		if( loc == 1 )
		{
			(*head) = cur_ptr->next;
            delete cur_ptr;
			return 1;
		}
		else
		{
			for( i = 1; i < loc; i++ )
			{
				prev_ptr = cur_ptr;
				cur_ptr = cur_ptr->next;
			}
            
			prev_ptr->next = cur_ptr->next;
            delete cur_ptr;
		}
	}
    
	return 0;
}

/* Deletes every node in the list */
template <class T>
inline void list_clear( node_t<T>** head )
{
	node_t<T>* cur_ptr, *temp;
    
	cur_ptr = (*head);
    
	while( cur_ptr )
	{
		temp = cur_ptr->next;
        delete cur_ptr;
		cur_ptr = temp;
	}
    
    (*head) = NULL;
}

/* Retrieve data from the selected node */
template <class T>
inline T list_get_node_data( node_t<T>** head, int loc )
{
	node_t<T>* cur_ptr;
	int i = 1;
    
	cur_ptr = (*head);
    
	if( loc < 1 )
		return NULL;
    
	while( i < loc )
	{
		cur_ptr = cur_ptr->next;
		i++;
	}
    
    if( !cur_ptr )
        return NULL;
	
	return cur_ptr->data;
}

