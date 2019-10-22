#include "./stages.hpp"

using namespace std;
using namespace std::chrono;

#define EOS NULL

/* Prints the current skyline and the rest list */
void print_sky_rest(vector<Node *> *DB_rest, vector<Node *> *DB_sky, int m, int i, int w, int k, int nw_sky, int nw_rest, bool verbose)
{
    if ((i >= w - 1) && verbose)
    {
        if ((i + 1 - w) % k == 0)
        {
            cout << "SKYLINE: ";
            for (int j = 0; j < nw_sky; j++)
            {
                if (j > 0)
                {
                    cout << " - ";
                }
                print_node_list((*DB_sky)[j], m);
            }
            cout << "\nREST: ";
            for (int j = 0; j < nw_rest; j++)
            {
                if (j > 0)
                {
                    cout << " - ";
                }
                print_node_list((*DB_rest)[j], m);
            }
            cout << "\n"
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

/* Collects results from rest list and prints it */
void collector(vector<Node *> *DB_rest, vector<Node *> *DB_sky, int m, int w, int k, int nw_sky, int nw_rest, bool verbose, vector<stdQueue<Message *> *> inputQueues, stdQueue<int> *outputQueue)
{
    Message *extracted = inputQueues[0]->pop();
    system_clock::time_point start, stop;
    long elapsed;
    int i = 0;
    while (extracted != EOS)
    {
        start = high_resolution_clock::now();

        bool sky = (*extracted).sky;
        int max_ins = (*extracted).insert;
        for (int j = 1; j < nw_rest; j++)
        {
            if (!(*extracted).sky)
            {
                sky = false;
            }
            if ((*extracted).insert > max_ins)
            {
                max_ins = (*extracted).insert;
            }
            extracted = inputQueues[j]->pop();
        }
        if (sky)
        {
            int rand = std::rand() % nw_sky;
            push_node(&(*DB_sky)[rand], (*extracted).tuple, i + w, max_ins);
        }
        else
        {
            int rand = std::rand() % nw_rest;
            push_node(&(*DB_rest)[rand], (*extracted).tuple, i + w, max_ins);
        }
        print_sky_rest(DB_rest, DB_sky, m, i, w, k, nw_sky, nw_rest, verbose);
        (*extracted).sky = sky;
        (*extracted).insert = max_ins;
        outputQueue->push(1);
        stop = high_resolution_clock::now();
        elapsed += duration_cast<microseconds>(stop - start).count();
        extracted = inputQueues[0]->pop();
        i++;
    }
    cout << "Time f4: " << (elapsed) << " micro sec" << endl;
}

/* Handles rest list */
void check_rest(Node **DB_rest, vector<Node *> *DB_sky, int nw_sky, int m, stdQueue<Message *> *inputQueue, stdQueue<Message *> *outputQueue, bool verbose)
{
    int i = 0;
    Message *extracted = inputQueue->pop();
    system_clock::time_point start, stop;
    long elapsed;
    while (extracted != EOS)
    {
        start = high_resolution_clock::now();
        int insert = (*extracted).insert;

        Node *prev_node_rest = NULL;
        Node *cur_node_rest = *DB_rest;
        while (cur_node_rest != NULL)
        {
            // Remove rest node if not in window anymore or if dominated by new tuple
            if (cur_node_rest->expire <= i || dominate((*extracted).tuple, cur_node_rest->tuple, m))
            {
                delete_node(&prev_node_rest, &cur_node_rest, DB_rest);
            }
            else
            {
                // If new tuple is dominated by rest node then it shouldn't be put in sky
                // until also that node expires
                if (dominate(cur_node_rest->tuple, (*extracted).tuple, m) && cur_node_rest->expire > insert)
                {
                    (*extracted).insert = cur_node_rest->expire;
                }
                // If previous sky node keeping rest node from skyline
                // has expired, rest node should be moved from rest list to skyline
                if (cur_node_rest->insert <= i)
                {
                    int rand = std::rand() % nw_sky;
                    push_node(&(*DB_sky)[rand], cur_node_rest->tuple, cur_node_rest->expire, 0);
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
        outputQueue->push(extracted);
        stop = high_resolution_clock::now();
        elapsed += duration_cast<microseconds>(stop - start).count();
        extracted = inputQueue->pop();
        i++;
    }
    if (verbose) {
        cout << "Time f3: " << (elapsed) << " micro sec" << endl;
    }
    outputQueue->push(EOS);
}

/* Node that takes the results from the skyline and forwards it to the rest */
void connect_sky_rest(int nw_sky, int nw_rest, vector<stdQueue<Message *> *> inputQueues, vector<stdQueue<Message *> *> outputQueues)
{
    Message *extracted = inputQueues[0]->pop();
    system_clock::time_point start, stop;
    long elapsed;
    while (extracted != EOS)
    {
        start = high_resolution_clock::now();
        bool sky = (*extracted).sky;
        int max_ins = (*extracted).insert;
        for (int i = 1; i < nw_sky; i++)
        {
            if (!(*extracted).sky)
            {
                sky = false;
            }
            if ((*extracted).insert > max_ins)
            {
                max_ins = (*extracted).insert;
            }
            extracted = inputQueues[i]->pop();
        }
        (*extracted).sky = sky;
        (*extracted).insert = max_ins;
        for (int i = 0; i < nw_rest; i++)
        {
            outputQueues[i]->push(extracted);
        }
        stop = high_resolution_clock::now();
        elapsed += duration_cast<microseconds>(stop - start).count();
        extracted = inputQueues[0]->pop();
    }
    cout << "Time f2a: " << (elapsed) << " micro sec" << endl;
    for (int i = 0; i < nw_rest; i++)
    {
        outputQueues[i]->push(EOS);
    }
}

/* Handles skyline list */
void check_skyline(Node **DB_sky, int m, stdQueue<int *> *inputQueue, stdQueue<Message *> *outputQueue, bool verbose)
{
    int i = 0;
    int *extracted = inputQueue->pop();
    system_clock::time_point start, stop;
    long elapsed;
    while (extracted != EOS)
    {
        start = high_resolution_clock::now();
        int insert = 0;
        bool sky = true;

        Node *prev_node_sky = NULL;
        Node *cur_node_sky = *DB_sky;
        while (cur_node_sky != NULL)
        {
            // Remove sky node if not in window anymore or if dominated by new tuple
            if (cur_node_sky->expire <= i || dominate(extracted, cur_node_sky->tuple, m))
            {
                delete_node(&prev_node_sky, &cur_node_sky, DB_sky);
            }
            else
            {
                // If new tuple is dominated by sky node then it shouldn't be put in sky
                // until sky node expires
                if (dominate(cur_node_sky->tuple, extracted, m))
                {
                    sky = false;
                    if (cur_node_sky->expire > insert)
                    {
                        insert = cur_node_sky->expire;
                    }
                }
                // Go to next element in skyline
                prev_node_sky = cur_node_sky;
                cur_node_sky = cur_node_sky->next;
            }
        }
        Message mes;
        mes.tuple = extracted;
        mes.sky = sky;
        mes.insert = insert;
        outputQueue->push(&mes);
        stop = high_resolution_clock::now();
        elapsed += duration_cast<microseconds>(stop - start).count();
        extracted = inputQueue->pop();
        i++;
    }
    if (verbose) {
        cout << "Time f2: " << (elapsed) << " micro sec" << endl;
    }
    outputQueue->push(EOS);
}

/* Given a length l, generate an array of random integers of length l. 
If verbose the random numbers are taken in the range [0-99] to better compare*/
void generator(int n, int m, int w, int k, int nw_sky, bool verbose, stdQueue<int> *inputQueue, vector<stdQueue<int *> *> outputQueues)
{
    deque<int *> window;
    system_clock::time_point start, stop;
    long elapsed;
    for (int i = 0; i < n; i++)
    {
        start = high_resolution_clock::now();
        int *t = generate_tuple(m, verbose);
        print_window(t, &window, m, i, w, k, verbose);
        for (int i = 0; i < nw_sky; i++)
        {
            outputQueues[i]->push(t);
        }
        stop = high_resolution_clock::now();
        elapsed += duration_cast<microseconds>(stop - start).count();
        inputQueue->pop();
    }
    cout << "Time f1: " << (elapsed) << " micro sec" << endl;
    for (int i = 0; i < nw_sky; i++)
    {
        outputQueues[i]->push(EOS);
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
