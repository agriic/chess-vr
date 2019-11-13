#pragma once

#include <queue>

namespace aic
{
    static const size_t MAX_QUEUED_FRAMES = 5;

    template<typename Data>
    class ConcurrentQueue
    {
    private:
        std::queue<Data> theQueue;
        mutable std::mutex theMutex;
        std::condition_variable theConditionVariable;
    public:
        //!  Push data onto queue.
        //!  \param data data to be pushed.
        void push(const Data& data)
        {
            std::unique_lock<std::mutex> lock(theMutex);
            theQueue.push(data);
            lock.unlock();
            theConditionVariable.notify_one();
        }

        // pushes data only if size < maxQSize
        bool push(const Data& data, size_t maxQSize)
        {
            std::unique_lock<std::mutex> lock(theMutex);
            if (theQueue.size() < maxQSize)
            {
                theQueue.push(data);
                lock.unlock();
                theConditionVariable.notify_one();
                return true;
            }

            return false;
        }

        // pushes data and pops one out if size > maxQSize
        // returns if pop happened
        bool forcedPush(const Data& data, size_t maxQSize)
        {
            bool popped = false;

            std::unique_lock<std::mutex> lock(theMutex);
            theQueue.push(data);
            if (theQueue.size() > maxQSize)
            {
                theQueue.pop();
                popped = true;
            }

            lock.unlock();
            theConditionVariable.notify_one();

            return popped;
        }

        //!  Test whether the queue is empty.
        //!  \returns whether the queue is empty (i.e. whether its size is zero.)
        bool empty() const
        {
            std::unique_lock<std::mutex> lock(theMutex);
            return theQueue.empty();
        }

        //!  \param popped_value reference to popped value, if successful.
        //!  \returns true if a value was popped, false otherwise (i.e. nothing in the queue.)
        bool tryPop(Data& poppedValue)
        {
            std::unique_lock<std::mutex> lock(theMutex);
            if (theQueue.empty())
            {
                return false;
            }

            poppedValue = theQueue.front();
            theQueue.pop();
            return true;
        }

        //!  Pops value off the queue (waits until queue has value first.)
        //!  \param popped_value reference to popped value.
        void waitAndPop(Data& poppedValue)
        {
            std::unique_lock<std::mutex> lock(theMutex);
            while (theQueue.empty())
            {
                theConditionVariable.wait(lock);
            }

            poppedValue = theQueue.front();
            theQueue.pop();
        }


        bool waitAndPopWithTimeout(Data& poppedValue, int64_t ms)
        {
            auto const timeout = std::chrono::milliseconds(ms);

            std::unique_lock<std::mutex> lock(theMutex);

            if (!theConditionVariable.wait_for(lock, timeout, [&]{return !theQueue.empty();}))
            {
                return false;
            }

            poppedValue = theQueue.front();
            theQueue.pop();
            return true;
        }


        //!  Return size.
        //!  \returns number of elements in the queue.
        typename std::queue<Data>::size_type size()
        {
            std::unique_lock<std::mutex> lock(theMutex);
            return theQueue.size();
        }
    };
}
