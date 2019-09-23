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
    int verbose = atoi(argv[6]);

    int nw_sky = 4;
    int nw_rest = 4;

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

        if (verbose)
        {
            window.push_front(t);
        }

        if ((i >= w - 1) && verbose)
        {
            if ((i + 1 - w) % k == 0)
            {
                std::cout << "WINDOW: ";
                print_queue(window, m);
            }
            window.pop_back();
        }
        int expire = i + w;
        int insert = 0;

        /******** SCAN SKYLINE LIST (F2) ********/

        start = std::chrono::high_resolution_clock::now();

        // Check each element in the skyline list

        vector<bool> sky_arr(nw_sky);
        vector<int> insert_sky(nw_sky);

        pf_sky.parallel_for(0, nw_sky, [t, m, i, &insert_sky, insert, &skylines, &sky_arr](const long j) {
            bool sky = true;

            Node *prev_node_sky = NULL;
            Node *cur_node_sky = skylines[j];

            sky_arr[j] = true;

            while (cur_node_sky != NULL)
            {
                // Remove sky node if not in window anymore or if dominated by new tuple
                if (cur_node_sky->expire <= i || dominate(t, cur_node_sky->tuple, m))
                {
                    delete_node(&prev_node_sky, &cur_node_sky, &(skylines)[j]);
                }
                else
                {
                    // If new tuple is dominated by sky node then it shouldn't be put in sky
                    // until sky node expires
                    if (dominate(cur_node_sky->tuple, t, m))
                    {
                        sky_arr[j] = false;
                        if (cur_node_sky->expire > insert)
                        {
                            insert_sky[j] = cur_node_sky->expire;
                        }
                    }
                    // Go to next element in skyline
                    prev_node_sky = cur_node_sky;
                    cur_node_sky = cur_node_sky->next;
                }
            }
        });

        for (int j = 0; j < nw_sky; j++)
        {
            if (insert_sky[j] > insert)
            {
                insert = insert_sky[j];
            }
        }

        stop = std::chrono::high_resolution_clock::now();
        time_f2 += duration_cast<microseconds>(stop - start).count();

        /******** SCAN REST LIST (F3) ********/

        start = std::chrono::high_resolution_clock::now();

        vector<int> insert_rest(nw_rest);

        // Check each element in the Rest list

        pf_rest.parallel_for(0, nw_rest, [t, m, i, &insert_rest, insert, &rests, &skylines, nw_sky](const long j) {
            Node *prev_node_rest = NULL;
            Node *cur_node_rest = rests[j];
            while (cur_node_rest != NULL)
            {
                // Remove rest node if not in window anymore or if dominated by new tuple
                if (cur_node_rest->expire <= i || dominate(t, cur_node_rest->tuple, m))
                {
                    delete_node(&prev_node_rest, &cur_node_rest, &(rests)[j]);
                }
                else
                {
                    // If new tuple is dominated by rest node then it shouldn't be put in sky
                    // until also that node expires
                    if (dominate(cur_node_rest->tuple, t, m) && cur_node_rest->expire > insert)
                    {
                        insert_rest[j] = cur_node_rest->expire;
                    }
                    // If previous sky node keeping rest node from skyline
                    // has expired, rest node should be moved from rest list to skyline
                    if (cur_node_rest->insert <= i)
                    {
                        int rand = std::rand() % nw_sky;
                        push_node(&(skylines)[rand], cur_node_rest->tuple, cur_node_rest->expire, 0);
                        delete_node(&prev_node_rest, &cur_node_rest, &(rests)[j]);
                    }
                    // Otherwise go to the next element in rest list
                    else
                    {
                        prev_node_rest = cur_node_rest;
                        cur_node_rest = cur_node_rest->next;
                    }
                }
            }
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
                    print_node_list(skylines[j], m);
                }
                std::cout << "\nREST: ";
                for (int j = 0; j < nw_rest; j++)
                {
                    if (j > 0)
                    {
                        std::cout << " - ";
                    }
                    print_node_list(rests[j], m);
                }
                std::cout << "\n"
                          << endl;
            }
        }
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