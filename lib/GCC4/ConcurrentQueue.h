#ifndef GCC4_CONCURRENT_QUEUE_H_INCLUDED
#define GCC4_CONCURRENT_QUEUE_H_INCLUDED

#include <queue>
#include <mutex>
#include <condition_variable>

#include "EventData.h"

template<typename Data>
class ConcurrentQueue
{
public:
    ConcurrentQueue() {}
    ~ConcurrentQueue() {}

    void push(Data const& data);
    bool empty();
    bool try_pop(Data& popped_value);
    void wait_and_pop(Data& popped_value);

private:
    std::mutex m_mutex;
    std::condition_variable m_cond;
    std::queue<Data> the_queue;
};

// force compiler generation
template class ConcurrentQueue<int>;
template class ConcurrentQueue<IEventDataPtr>;

typedef ConcurrentQueue<IEventDataPtr> ThreadSafeEventQueue;

#endif // GCC4_CONCURRENT_QUEUE_H_INCLUDED

