#include "./util.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    // read parameters
    if (argc < 7)
    {
        cout << "Parameters are: 1) Stream length (n)\n"
             << "2) Tuple size (m)\n"
             << "3) Window size (w)\n"
             << "4) Sliding factor (k)\n"
             << "5) Seed (s)"
             << "6) Verbose (0 = false)" << endl;
        return -1;
    }

    /* TODO: check if parameters are positive integers */
    /* TODO: stream length must be > than window */
    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int w = atoi(argv[3]);
    int k = atoi(argv[4]);
    int s = atoi(argv[5]);
    int verbose = atoi(argv[6]);

    srand(s);

    /* TODO: use type T */
    deque<int *> window;
    deque<pair<int *, int>> DB_rest;
    deque<pair<int *, int>> DB_sky;

    for (int i = 0; i < n; i++)
    {
        int *t = generate_tuple(m);
        if (verbose)
        {
            window.push_front(t);
        }
        bool sky = true;
        int expire = i + w;
        int sky_pos = 0;
        for (pair<int *, int> t_sky : DB_sky)
        {
            if (dominate(t, t_sky.first, m))
            {
                //DB_sky.push_front(make_pair(t, i + w));
                DB_sky.erase(DB_sky.begin() + sky_pos);
            }
            if (dominate(t_sky.first, t, m))
            {
                sky = false;
                expire = t_sky.second;
            }
            sky_pos++;
        }
        if (sky)
        {
            DB_sky.push_front(make_pair(t, expire));
        }
        else
        {
            DB_rest.push_front(make_pair(t, expire));
        }
        // wait til up to speed
        if (i >= w - 1 && verbose)
        {
            cout << "WINDOW: ";
            print_window(window, m);
            cout << "SKYLINE: ";
            print_skyline(DB_sky, m);

            window.pop_back();
            //
        }
    }

    return 0;
}