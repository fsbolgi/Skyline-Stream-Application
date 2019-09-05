#include "./util.hpp"

using namespace std;

class Node
{
public:
    int *tuple; // array of integer
    int expire; // time when tuple out of window
    int insert; // time when it can be inserted in skyline
    Node *next; // pointer to next tuple
};

/* Given the head of the list and the size of each tuple 
prints the whole list */
void print_node_list(Node *n, int m)
{
    bool first = true;
    int i = 0;
    while (n != NULL)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            cout << " - ";
        }
        print_array(n->tuple, m);
        n = n->next;
        i++;
    }
    cout << endl;
}

/* Given the head of the list and the parameters to put in 
the new node, appends it to the back of the list */
void push(struct Node **head_ref, int *new_tuple, int new_exp, int new_ins = 0)
{
    // Allocate new node
    struct Node *new_node = (struct Node *)malloc(sizeof(struct Node));

    // Fill the node
    new_node->tuple = new_tuple;
    new_node->expire = new_exp;
    new_node->insert = new_ins;

    // Make next of new node as head
    new_node->next = *head_ref;

    // Move the head to the new node
    *head_ref = new_node;
}

/* In order to delete the current node also the previous node 
and the head of the list is required */
void delete_node(struct Node **prev_node, struct Node **cur_node, struct Node **head)
{
    // Store temporarily the new current point
    struct Node *temp = (*cur_node)->next;
    // If the node to delete is the head of the list, it must be updated
    if (*prev_node == NULL)
    {
        *head = temp;
    }
    // Otherwise the pointer of the previous node must be updated
    else
    {
        (*prev_node)->next = (*cur_node)->next;
    }
    // Delete the node
    free(*cur_node);
    // Update the reference to the current node
    *cur_node = temp;
}
