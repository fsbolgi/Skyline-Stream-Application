#include "./stages.hpp"

using namespace std;

/* Prints the current skyline and the rest list */
void print_sky_rest(vector<Node *> *skylines, vector<Node *> *rests, int m, int i, int w, int k, int nw_sky, int nw_rest, bool verbose)
{
    // Wait until correct window and print only when necessary
    if ((i >= w - 1) && verbose)
    {
        if ((i + 1 - w) % k == 0)
        {
            std::cout << "SKYLINE: ";
            for (int j = 0; j < nw_sky; j++)
            {
                if (j > 0)
                {
                    std::cout << " - ";
                }
                print_node_list((*skylines)[j], m);
            }
            std::cout << "\nREST: ";
            for (int j = 0; j < nw_rest; j++)
            {
                if (j > 0)
                {
                    std::cout << " - ";
                }
                print_node_list((*rests)[j], m);
            }
            std::cout << "\n"
                      << endl;
        }
    }
}

/* Prints current window */
void print_window(int *t, deque<int *> *window, int m, int i, int w, int k, bool verbose)
{
    if (verbose)
    {
        window->push_front(t);
    }
    if ((i >= w - 1) && verbose)
    {
        if ((i + 1 - w) % k == 0)
        {
            cout << "WINDOW: ";
            print_queue(*window, m);
        }
        window->pop_back();
    }
}

/* Handles rest list */
void check_rest(int *t, int m, int i, vector<int> *insert_rest, int insert, vector<Node *> *rests, vector<Node *> *skylines, int nw_sky, long j)

{
    Node *prev_node_rest = NULL;
    Node *cur_node_rest = (*rests)[j];
    while (cur_node_rest != NULL)
    {
        // Remove rest node if not in window anymore or if dominated by new tuple
        if (cur_node_rest->expire <= i || dominate(t, cur_node_rest->tuple, m))
        {
            delete_node(&prev_node_rest, &cur_node_rest, &(*rests)[j]);
        }
        else
        {
            // If new tuple is dominated by rest node then it shouldn't be put in sky
            // until also that node expires
            if (dominate(cur_node_rest->tuple, t, m) && cur_node_rest->expire > insert)
            {
                (*insert_rest)[j] = cur_node_rest->expire;
            }
            // If previous sky node keeping rest node from skyline
            // has expired, rest node should be moved from rest list to skyline
            if (cur_node_rest->insert <= i)
            {
                int rand = std::rand() % nw_sky;
                push_node(&(*skylines)[rand], cur_node_rest->tuple, cur_node_rest->expire, 0);
                delete_node(&prev_node_rest, &cur_node_rest, &(*rests)[j]);
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
void check_skyline(int *t, int m, int i, vector<int> *insert_sky, int insert, vector<Node *> *skylines, vector<bool> *sky_arr, long j)
{
    bool sky = true;

    Node *prev_node_sky = NULL;
    Node *cur_node_sky = (*skylines)[j];

    (*sky_arr)[j] = true;

    while (cur_node_sky != NULL)
    {
        // Remove sky node if not in window anymore or if dominated by new tuple
        if (cur_node_sky->expire <= i || dominate(t, cur_node_sky->tuple, m))
        {
            delete_node(&prev_node_sky, &cur_node_sky, &(*skylines)[j]);
        }
        else
        {
            // If new tuple is dominated by sky node then it shouldn't be put in sky
            // until sky node expires
            if (dominate(cur_node_sky->tuple, t, m))
            {
                (*sky_arr)[j] = false;
                if (cur_node_sky->expire > insert)
                {
                    (*insert_sky)[j] = cur_node_sky->expire;
                }
            }
            // Go to next element in skyline
            prev_node_sky = cur_node_sky;
            cur_node_sky = cur_node_sky->next;
        }
    }
}

/* Check if the program is launched with the correct set of parameters */
bool check_parameters(int argc, char *argv[])
{
    bool error = false;
    // Check if there's the correct number of parameters
    if (argc < 9)
    {
        std::cout << "Parameters are:\n"
                  << "1) Stream length (n)\n"
                  << "2) Tuple size (m)\n"
                  << "3) Window size (w)\n"
                  << "4) Sliding factor (k)\n"
                  << "5) Seed (s)\n"
                  << "6) Number of sky workers\n"
                  << "7) Number of rest workers\n"
                  << "8) Verbose (0 = false)" << endl;
        error = true;
    }
    // Check if parameters are positive integers
    for (int i = 1; i < argc - 1; i++)
    {
        if (atoi(argv[i]) < 0)
        {
            std::cout << "Parameters must be all positive integers" << endl;
            error = true;
        }
    }

    // verbose must be 0 or 1
    if (atoi(argv[8]) < 0 || atoi(argv[8]) > 1)
    {
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
