#include "./stages.hpp"

using namespace std;
using namespace std::chrono;

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

    // time spent in each phase
    auto time_f1 = 0;
    auto time_f2 = 0;
    auto time_f3 = 0;
    auto time_f4 = 0;
    system_clock::time_point start;
    system_clock::time_point stop;

    system_clock::time_point startx = std::chrono::high_resolution_clock::now();

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

        /******** GENERATE TUPLE (F1) ********/

        start = std::chrono::high_resolution_clock::now();

        int *t = generate_tuple(m, verbose);

        stop = std::chrono::high_resolution_clock::now();
        time_f1 += duration_cast<microseconds>(stop - start).count();

        if (verbose)
        {
            window.push_front(t);
        }
        int expire = i + w;
        int insert = 0;

        /******** SCAN SKYLINE LIST (F2) ********/

        start = std::chrono::high_resolution_clock::now();

        // Check each element in the skyline list
        bool sky = check_skyline(i, t, &DB_sky, m, &insert);

        stop = std::chrono::high_resolution_clock::now();
        time_f2 += duration_cast<microseconds>(stop - start).count();

        /******** SCAN REST LIST (F3) ********/

        start = std::chrono::high_resolution_clock::now();

        // Check each element in the Rest list
        check_rest(i, t, &DB_rest, &DB_sky, m, &insert);

        stop = std::chrono::high_resolution_clock::now();
        time_f3 += duration_cast<microseconds>(stop - start).count();

        // New tuple is inserted either in the skyline or in the rest list
        if (sky)
        {
            push_node(&DB_sky, t, expire, 0);
        }
        else
        {
            push_node(&DB_rest, t, expire, insert);
        }

        /******** PRINT LISTS (F4) ********/

        start = std::chrono::high_resolution_clock::now();

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
        stop = std::chrono::high_resolution_clock::now();
        time_f4 += duration_cast<microseconds>(stop - start).count();
    }

    system_clock::time_point stopx = std::chrono::high_resolution_clock::now();
    auto timex = duration_cast<microseconds>(stopx - startx).count();
    cout << "Total: " << (timex) << " micro sec" << endl;

    cout << "Time f1: " << (time_f1) << " micro sec" << endl;
    cout << "Time f2: " << (time_f2) << " micro sec" << endl;
    cout << "Time f3: " << (time_f3) << " micro sec" << endl;
    cout << "Time f4: " << (time_f4) << " micro sec" << endl;

    return 0;
}