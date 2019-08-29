#ifndef UTIL_HPP
#define UTIL_HPP

#include <iostream>
#include <cstdlib>
#include <deque>

int *generate_tuple(int m);

bool dominate(int *t1, int *t2, int m);

void print_array(int *a);

void print_window(std::deque<int *> q, int m);

void print_skyline(std::deque<std::pair<int *, int>> q, int m);

#endif