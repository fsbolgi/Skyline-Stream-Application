#include "./util.hpp"

using namespace std;

int *generate_tuple(int m)
{
    int *tuple;
    tuple = new int[m];
    int rand_int;
    for (int i = 0; i < m; i++)
    {
        rand_int = rand() % 20; /* TODO: remove limit to integer */
        tuple[i] = rand_int;
    }
    return tuple;
}

bool dominate(int *t1, int *t2, int m)
{
    for (int i = 0; i < m; i++)
    {
        if (t1[i] < t2[i])
        {
            return false;
        }
    }
    return true;
}

void print_array(int *a, int l)
{
    cout << "[";
    for (int i = 0; i < l - 1; i++)
    {
        cout << a[i] << ", ";
    }
    cout << a[l - 1] << "]";
}

void print_window(deque<int *> q, int m)
{
    bool first = true;
    for (int *e : q)
    {
        if (first)
        {
            print_array(e, m);
            first = false;
        }
        else
        {
            cout << " - ";
            print_array(e, m);
        }
    }
    cout << "\n";
}

void print_skyline(std::deque<std::pair<int *, int>> q, int m)
{
    bool first = true;
    for (pair<int *, int> e : q)
    {
        if (first)
        {
            print_array(e.first, m);
            first = false;
        }
        else
        {
            cout << " - ";
            print_array(e.first, m);
        }
    }
    cout << "\n";
}