#include "ff/parallel_for.hpp"
#include "ff/map.hpp"

#include "./stages.hpp"

using namespace std;
using namespace std::chrono;
using namespace ff;

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
    int nw_sky = atoi(argv[6]);
    int nw_rest = atoi(argv[7]);
    int verbose = atoi(argv[8]);

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

    vector<Node *> skylines(nw_sky);
    vector<Node *> rests(nw_rest);

    for (int j = 0; j < nw_sky; j++)
    {
        skylines[j] = NULL;
    }

    for (int j = 0; j < nw_rest; j++)
    {
        rests[j] = NULL;
    }
    ParallelFor pf_sky(nw_sky);
    ParallelFor pf_rest(nw_rest);

    // For each element in the stream
    for (int i = 0; i < n; i++)
    {

        /******** GENERATE TUPLE (F1) ********/

        start = std::chrono::high_resolution_clock::now();

        int *t = generate_tuple(m, verbose);

        stop = std::chrono::high_resolution_clock::now();
        time_f1 += duration_cast<microseconds>(stop - start).count();

        print_window(t, &window, m, i, w, k, verbose);

        int expire = i + w;
        int insert = 0;

        /******** SCAN SKYLINE LIST (F2) ********/

        start = std::chrono::high_resolution_clock::now();

        vector<bool> sky_arr(nw_sky);
        vector<int> insert_sky(nw_sky);

        // Check each element in the skyline list
        pf_sky.parallel_for(0, nw_sky, [t, m, i, &insert_sky, insert, &skylines, &sky_arr](const long j) {
            check_skyline(t, m, i, &insert_sky, insert, &skylines, &sky_arr, j);
        });

        stop = std::chrono::high_resolution_clock::now();
        time_f2 += duration_cast<microseconds>(stop - start).count();

        for (int j = 0; j < nw_sky; j++)
        {
            if (insert_sky[j] > insert)
            {
                insert = insert_sky[j];
            }
        }

        /******** SCAN REST LIST (F3) ********/

        start = std::chrono::high_resolution_clock::now();

        vector<int> insert_rest(nw_rest);

        // Check each element in the Rest list
        pf_rest.parallel_for(0, nw_rest, [t, m, i, &insert_rest, insert, &rests, &skylines, nw_sky](const long j) {
            check_rest(t, m, i, &insert_rest, insert, &rests, &skylines, nw_sky, j);
        });

        stop = std::chrono::high_resolution_clock::now();
        time_f3 += duration_cast<microseconds>(stop - start).count();

        bool sky = true;
        for (int j = 0; j < nw_sky; j++)
        {
            if (sky_arr[j] == false)
            {
                sky = false;
            }
        }

        for (int j = 0; j < nw_sky; j++)
        {
            if (insert_rest[j] > insert)
            {
                insert = insert_rest[j];
            }
        }

        // New tuple is inserted either in the skyline or in the rest list
        if (sky)
        {
            int rand = std::rand() % nw_sky;
            push_node(&(skylines)[rand], t, expire, 0);
        }
        else
        {
            int rand = std::rand() % nw_rest;

            push_node(&(rests)[rand], t, expire, insert);
        }

        /******** PRINT LISTS (F4) ********/

        start = std::chrono::high_resolution_clock::now();

        print_sky_rest(&skylines, &rests, m, i, w, k, nw_sky, nw_rest, verbose);

        stop = std::chrono::high_resolution_clock::now();
        time_f4 += duration_cast<microseconds>(stop - start).count();
    }

    system_clock::time_point stopx = std::chrono::high_resolution_clock::now();
    auto timex = duration_cast<microseconds>(stopx - startx).count();

    // Print times
    std::cout << "Total time: " << (timex) << " micro sec" << endl;

    std::cout << "Time f1: " << (time_f1) << " micro sec" << endl;
    std::cout << "Time f2: " << (time_f2) << " micro sec" << endl;
    std::cout << "Time f3: " << (time_f3) << " micro sec" << endl;
    std::cout << "Time f4: " << (time_f4) << " micro sec" << endl;

    return 0;
}