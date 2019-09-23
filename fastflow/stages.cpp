#include "./stages.hpp"

using namespace std;

/* Handles rest list */
bool check_rest(int i, int *t, Node **DB_rest, Node **DB_sky, int m, int *insert)
{
    Node *prev_node_rest = NULL;
    Node *cur_node_rest = *DB_rest;
    while (cur_node_rest != NULL)
    {
        // Remove rest node if not in window anymore or if dominated by new tuple
        if (cur_node_rest->expire <= i || dominate(t, cur_node_rest->tuple, m))
        {
            delete_node(&prev_node_rest, &cur_node_rest, DB_rest);
        }
        else
        {
            // If new tuple is dominated by rest node then it shouldn't be put in sky
            // until also that node expires
            if (dominate(cur_node_rest->tuple, t, m) && cur_node_rest->expire > *insert)
            {
                *insert = cur_node_rest->expire;
            }
            // If previous sky node keeping rest node from skyline
            // has expired, rest node should be moved from rest list to skyline
            if (cur_node_rest->insert <= i)
            {
                push_node(DB_sky, cur_node_rest->tuple, cur_node_rest->expire, 0);
                delete_node(&prev_node_rest, &cur_node_rest, DB_rest);
            }
            // Otherwise go to the next element in rest list
            else
            {
                prev_node_rest = cur_node_rest;
                cur_node_rest = cur_node_rest->next;
            }
        }
    }
}

/* Handles skyline list */
bool check_skyline(int i, int *t, Node **DB_sky, int m, int *insert)
{
    bool sky = true;

    Node *prev_node_sky = NULL;
    Node *cur_node_sky = *DB_sky;
    while (cur_node_sky != NULL)
    {
        // Remove sky node if not in window anymore or if dominated by new tuple
        if (cur_node_sky->expire <= i || dominate(t, cur_node_sky->tuple, m))
        {
            delete_node(&prev_node_sky, &cur_node_sky, DB_sky);
        }
        else
        {
            // If new tuple is dominated by sky node then it shouldn't be put in sky
            // until sky node expires
            if (dominate(cur_node_sky->tuple, t, m))
            {
                sky = false;
                if (cur_node_sky->expire > *insert)
                {
                    *insert = cur_node_sky->expire;
                }
            }
            // Go to next element in skyline
            prev_node_sky = cur_node_sky;
            cur_node_sky = cur_node_sky->next;
        }
    }
    return sky;
}

/* Check if the program is launched with the correct set of parameters */
bool check_parameters(int argc, char *argv[])
{
    bool error = false;
    // Check if there's the correct number of parameters
    if (argc < 7)
    {
        std::cout << "Parameters are:\n "
                  << "1) Stream length (n)\n"
                  << "2) Tuple size (m)\n"
                  << "3) Window size (w)\n"
                  << "4) Sliding factor (k)\n"
                  << "5) Seed (s)\n"
                  << "6) Verbose (0 = false)" << endl;
        error = true;
    }
    // Check if parameters are positive integers
    for (int i = 1; i < argc-1; i++)
    {
        if (atoi(argv[i]) <= 0)
        {
            std::cout << "Parameters must be all positive integers" << endl;
            error = true;
        }
    }

    // verbose must be 0 or 1
    if (atoi(argv[6]) < 0 || atoi(argv[6]) > 1) {
        std::cout << "Verbose is either 0 or 1" << endl;
        error = true;
    }

    // Stream length must be > than window, not sense ow
    if (atoi(argv[1]) < atoi(argv[3]))
    {
        std::cout << "Stream length must be greater than window" << endl;
        error = true;
    }
    // Sliding must be < than window, not interesting ow
    if (atoi(argv[4]) > atoi(argv[3]))
    {
        std::cout << "Sliding must be < than window" << endl;
        error = true;
    }
    return error;
}