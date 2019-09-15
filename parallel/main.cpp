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

    int nw_sky = 4;
    int nw_rest = 4;

    // Decide seed for random numbers
    std::srand(s);

    // time spent in each phase
    auto time_f1 = 0;
    auto time_f2 = 0;
    auto time_f3 = 0;
    auto time_f4 = 0;
    system_clock::time_point start = std::chrono::high_resolution_clock::now();

    /******** GENERATE TUPLE (F1) ********/

    stdQueue<int> *coll_to_gen = new stdQueue<int>();
    vector<stdQueue<int *> *> disp_to_sky(nw_sky);
    vector<stdQueue<Message *> *> sky_to_conn(nw_sky);

    for (int i = 0; i < nw_sky; i++)
    {
        disp_to_sky[i] = new stdQueue<int *>();
        sky_to_conn[i] = new stdQueue<Message *>();
    }

    thread gen_thread(generator, n, m, w, k, nw_sky, verbose, coll_to_gen, disp_to_sky);  

    /******** SCAN SKYLINE LIST (F2) ********/

    vector<Node *> skylines(nw_sky);
    vector<thread> sky_workers;

    for (int i = 0; i < nw_sky; i++)
    {
        skylines[i] = NULL;
        sky_workers.push_back(thread(check_skyline, &(skylines[i]), m, disp_to_sky[i], sky_to_conn[i]));
    }

    /******** CONNECT SKYLINE AND REST (F3a) ********/

    vector<stdQueue<Message *> *> conn_to_rest(nw_rest);
    vector<stdQueue<Message *> *> rest_to_coll(nw_rest);

    for (int i = 0; i < nw_rest; i++)
    {
        conn_to_rest[i] = new stdQueue<Message *>();
        rest_to_coll[i] = new stdQueue<Message *>();
    }

    thread connector(connect_sky_rest, nw_sky, nw_rest, sky_to_conn, conn_to_rest);

    /******** SCAN REST LIST (F3) ********/

    vector<Node *> rests(nw_rest);
    vector<thread> rest_workers;

    for (int i = 0; i < nw_rest; i++)
    {
        rests[i] = NULL;
        rest_workers.push_back(thread(check_rest, &(rests[i]), &skylines, nw_sky, m, conn_to_rest[i], rest_to_coll[i]));
    }

    /******** INSERT NEW TUPLE (F4) ********/

    thread coll(collector, &rests, &skylines, m, w, k, nw_sky, nw_rest, verbose, rest_to_coll, coll_to_gen);

    /******** JOIN THREADS ********/

    gen_thread.join();
    for (int i = 0; i < nw_sky; i++)
    {
        sky_workers.back().join();
        sky_workers.pop_back();
    }
    connector.join();
    for (int i = 0; i < nw_rest; i++)
    {
        rest_workers.back().join();
        rest_workers.pop_back();
    }
    coll.join();

    system_clock::time_point stop = std::chrono::high_resolution_clock::now();
    auto timex = duration_cast<microseconds>(stop - start).count();
    cout << "Total " << (timex) << " micro sec" << endl;

    return 0;
}
