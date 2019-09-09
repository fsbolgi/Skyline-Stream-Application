#include "./stages.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    if (check_parameters(argc, argv))
    {
        return -1;
    }

    // Store parameters
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int w = atoi(argv[3]);
    int k = atoi(argv[4]);
    int s = atoi(argv[5]);
    int verbose = atoi(argv[6]);

    // Decide seed for random numbers
    srand(s);

    // The window is stored in the queue
    deque<int *> window;
    // The current skyline is saved as a linked list
    Node *DB_sky = NULL;
    // Another linked list is used to store tuples that may end up
    // in the skyline later
    Node *DB_rest = NULL;

    // For each element in the stream
    for (int i = 0; i < n; i++)
    {
        int *t = generate_tuple(m, verbose);
        if (verbose)
        {
            window.push_front(t);
        }
        int expire = i + w;
        int insert = 0;

        // Check each element in the skyline list
        bool sky = check_skyline(i, t, &DB_sky, m, &insert);

        // Check each element in the Rest list
        check_rest(i, t, &DB_rest, &DB_sky, m, &insert, expire);

        // New tuple is inserted either in the skyline or in the rest list
        if (sky)
        {
            push_node(&DB_sky, t, expire, insert);
        }
        else
        {
            push_node(&DB_rest, t, expire, insert);
        }

        // Wait until correct window and print only when necessary
        if ((i >= w - 1) && verbose)
        {
            if ((i + 1 - w) % k == 0)
            {
                cout << "WINDOW: ";
                print_queue(window, m);
                cout << "SKYLINE: ";
                print_node_list(DB_sky, m);
                cout << "REST: ";
                print_node_list(DB_rest, m);
                cout << endl;
            }
            window.pop_back();
        }
    }
    return 0;
}