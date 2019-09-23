#ifndef STAGES_HPP
#define STAGES_HPP

#include "../linked_list.hpp"

void print_lists(int i, int w, int m, int k, bool verbose, std::deque<int *> *window, Node *DB_sky, Node *DB_rest);

bool check_rest(int i, int *t, Node **DB_rest, Node **DB_sky, int m, int *insert);

bool check_skyline(int i, int *t, Node **DB_sky, int m, int *insert);

bool check_parameters(int argc, char *argv[]);

#endif