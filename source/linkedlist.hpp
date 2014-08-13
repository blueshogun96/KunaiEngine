//
//  linkedlist.hpp 
//
//  Created by admin on 11/30/13.
//  Copyright (c) 2013 Shogun3D. All rights reserved.
//

#ifndef linkedlist_hpp
#define linkedlist_hpp

#ifndef PURE
#define PURE =0
#endif

/* Linked list node base class */
template <class T>
class node_t
{
public:
    node_t() {};
    virtual ~node_t() { delete data; }
    
public:
	T data;
	node_t<T>* next;
};

/* Example class inhereting from node_t:
 
 template <class T>
 class xnode_t : public node_t<T>
 {
 public:
     xnode_t();
     virtual ~xnode_t();
    
 public:
     T data;
     node_t<T>* next;
 };
 */

/* Add a node at the beginning of the list */
template <class T>
void list_add_beginning( node_t<T>** head, T data );

/* Add list node to the end */
template <class T>
void list_add_end( node_t<T>** head, T data );

/* Add a new node at a specific position */
template <class T>
void list_add_at( node_t<T>** head, T data, int loc );

/* Returns the number of elements in the list */
template <class T>
int list_length( node_t<T>** head );

/* Delete a node from the list */
template <class T>
int list_delete( node_t<T>** head, T data );

/* Deletes a node from the given position */
template <class T>
int list_delete_loc( node_t<T>** head, int loc );

/* Deletes every node in the list */
template <class T>
void list_clear( node_t<T>** head );

/* Retrieve data from the selected node */
template <class T>
T list_get_node_data( node_t<T>** head, int loc );

#endif
