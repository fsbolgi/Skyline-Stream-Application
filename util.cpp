#include "./util.hpp"

using namespace std;

/* Given a length l, generate an array of random integers
of length l. If verbose the random numbers are taken
in the range [0-99] to better compare*/
int *generate_tuple(int l, bool verbose)
{
    int *tuple;
    tuple = new int[l];
    for (int i = 0; i < l; i++)
    {
        int rand_int = rand();
        if (verbose)
        {
            rand_int = rand_int % 20;
        }
        tuple[i] = rand_int;
    }
    return tuple;
}

/* Checks if the tuple t1 dominates tuple t2 
by doing a pairwise comparison on the elements */
bool dominate(int *t1, int *t2, int l)
{
    for (int i = 0; i < l; i++)
    {
        if (t1[i] < t2[i])
        {
            return false;
        }
    }
    return true;
}

/* Prints an array with brackets and commas */
void print_array(int *a, int l)
{
    cout << "[";
    for (int i = 0; i < l - 1; i++)
    {
        cout << a[i] << ", ";
    }
    cout << a[l - 1] << "]";
}

/* Prints a queue of integer arrays */
void print_queue(deque<int *> q, int l)
{
    bool first = true;
    for (int *e : q)
    {
        if (first)
        {
            first = false;
        }
        else
        {
            cout << " - ";
        }
        print_array(e, l);
    }
    cout << endl;
}