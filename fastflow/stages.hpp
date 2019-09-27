#ifndef STAGES_HPP
#define STAGES_HPP

#include "../linked_list.hpp"

void print_sky_rest(std::vector<Node *> *skylines, std::vector<Node *> *rests, int m, int i, int w, int k, int nw_sky, int nw_rest, bool verbose);

void print_window(int *t, std::deque<int *> *window, int m, int i, int w, int k, bool verbose);

void check_rest(int *t, int m, int i, std::vector<int> *insert_rest, int insert, std::vector<Node *> *rests, std::vector<Node *> *skylines, int nw_sky, long j);

void check_skyline(int *t, int m, int i, std::vector<int> *insert_sky, int insert, std::vector<Node *> *skylines, std::vector<bool> *sky_arr, long j);

bool check_parameters(int argc, char *argv[]);

#endif