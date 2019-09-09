#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <cstdlib>
#include <deque>
#include <tuple>

int *generate_tuple(int m, bool verbose);

bool dominate(int *t1, int *t2, int m);

void print_array(int *a, int l);

void print_queue(std::deque<int *> q, int m);

#endif