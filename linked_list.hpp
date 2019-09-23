#ifndef LLIST_HPP
#define LLIST_HPP

#include "./util.hpp"

class Node
{
public:
    int *tuple; // array of integer
    int expire; // time when tuple out of window
    int insert; // time when it can be inserted in skyline
    Node *next; // pointer to next tuple
};

void print_node_list(Node *n, int m);

void push_node(struct Node **head_ref, int *new_tuple, int new_exp, int new_ins);

void delete_node(struct Node **prev_node, struct Node **cur_node, struct Node **head);

#endif