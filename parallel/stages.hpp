#ifndef STAGES_HPP
#define STAGES_HPP

#include "../linked_list.hpp"
#include "../stdQueue.hpp"

struct Message
{
    int *tuple;
    bool sky;
    int insert;
};

void collector(std::vector<Node *> *DB_rest, std::vector<Node *> *DB_sky, int m, int w, int k, int nw_sky, int nw_rest, bool verbose, std::vector<stdQueue<Message *> *> inputQueues, stdQueue<int> *outputQueue);

void check_rest(Node **DB_rest, std::vector<Node *> *DB_sky, int nw_sky, int m, stdQueue<Message *> *inputQueue, stdQueue<Message *> *outputQueue);

void connect_sky_rest(int nw_sky, int nw_rest, std::vector<stdQueue<Message *> *> inputQueues, std::vector<stdQueue<Message *> *> outputQueues);

void check_skyline(Node **DB_sky, int m, stdQueue<int *> *inputQueue, stdQueue<Message *> *outputQueue);

void generator(int n, int m, int w, int k, int nw_sky, bool verbose, stdQueue<int> *inputQueue, std::vector<stdQueue<int *> *> outputQueues);

bool check_parameters(int argc, char *argv[]);

#endif
