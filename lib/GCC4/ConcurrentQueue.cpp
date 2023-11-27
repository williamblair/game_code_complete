#include "ConcurrentQueue.h"

// force compiler generation
template class ConcurrentQueue<int>;
template class ConcurrentQueue<IEventDataPtr>;

template<typename Data>
void ConcurrentQueue<Data>::push(Data const& data)
{
    std::unique_lock<std::mutex> lk(m_mutex);
    the_queue.push(data);
    lk.unlock();
    m_cond.notify_all();
}

template<typename Data>
bool ConcurrentQueue<Data>::empty()
{
    std::unique_lock<std::mutex> lk(m_mutex);
    return the_queue.empty();
}

template<typename Data>
bool ConcurrentQueue<Data>::try_pop(Data& popped_value)
{
    std::unique_lock<std::mutex> lk(m_mutex);
    if (the_queue.empty()) {
        return false;
    }
    popped_value = the_queue.front();
    the_queue.pop();
    return true;
}

template<typename Data>
void ConcurrentQueue<Data>::wait_and_pop(Data& popped_value)
{
    std::unique_lock<std::mutex> lk(m_mutex);
    while (the_queue.empty()) {
        m_cond.wait(lk);
    }
    popped_value = the_queue.front();
    the_queue.pop();
}

