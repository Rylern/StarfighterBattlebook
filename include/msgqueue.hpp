#ifndef MSGQUEUE_H
#define MSGQUEUE_H


#include <deque>
#include <mutex>


template<class T>
class MsgQueue {

public:
    void send(T && msg) {
        std::lock_guard<std::mutex> lck(this->_mutex);
        this->_queue.push_front(std::move(msg));
    }
    T receive() {
        std::lock_guard<std::mutex> lck(this->_mutex);
        if (_queue.empty()) {
            throw "Queue empty";
        }
        T msg = std::move(this->_queue.back());
        this->_queue.pop_back();
        return msg;
    }
    bool messageAvailable() {
        std::lock_guard<std::mutex> lck(this->_mutex);
        return !_queue.empty();
    }

private:
	std::deque<T> _queue;
    std::mutex _mutex;
};

#endif