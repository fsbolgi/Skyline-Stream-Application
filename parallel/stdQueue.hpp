#ifndef STDQUEUE_HPP
#define STDQUEUE_HPP

#include "../util.hpp"

// it's a template class since depends on the type given for the queue
template <typename T>
class stdQueue
{
private:
    std::mutex d_mutex;
    std::condition_variable d_condition; // condition variable works only if there's a mutex
    std::deque<T> d_queue;               // uses a double queue
    int queueSize;

public:
    // create a queue of given size
    stdQueue(int size)
    {
        queueSize = size;
        {}
    }

    // create a queue of max size
    stdQueue()
    {
        queueSize = std::numeric_limits<unsigned int>::max();
        {}
    }

    void push(T const &value)
    {
        {
            // declare a lock that uses the mutex
            std::unique_lock<std::mutex> lock(d_mutex);
            // this lines are executed taking the lock
            if (d_queue.size() == queueSize)
            {
                d_condition.wait(lock);
            }
            d_queue.push_front(value);
        } // the effect of the lock ends here
        // notifies the threads waiting
        d_condition.notify_all();
    }

    T pop()
    {
        // declare the unique lock
        std::unique_lock<std::mutex> lock(d_mutex);
        // check the condition variable, it lock until the second parameter
        // doesn't return me a false value (wait until not empty)
        if (d_queue.empty()){
            d_condition.wait(lock);
        }
        // if we're here we know that there's an element in the queue
        // so we can return the element (actually we read the element and remove it)
        T item(std::move(d_queue.back()));
        d_queue.pop_back();
        return item;
    }
};

// the problem using this queue is the termination, how to understand when EOS?
// one way is to add some method to send close the queue and a method to check if
// it should close it. Or define a special value to insert into the queue that
// means EOS

#endif