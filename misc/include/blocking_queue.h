/*
 * Version: @(#)blocking_queue.h 1.0.0 25/07/2021
 *
 * Authors: ZHONG Ming
 *
 */

#ifndef __BLOCKING_QUEUE_H__
#define __BLOCKING_QUEUE_H__

#include <deque>
#include <semaphore.h>

template<class T>
class BlockingQueue {
public:
  // constructor
  BlockingQueue()
  {
    sem_init(&mutex_, 0, 1);
    sem_init(&items_, 0, 0);
  }

  // deconstructor
  virtual ~BlockingQueue()
  {
    queue_.clear();
  }
  
  template<class TT>
  void put(TT &&t)
  {
    sem_wait(&mutex_);
    queue_.emplace_back(std::forward<TT>(t));
    sem_post(&mutex_);
    sem_post(&items_);
  }

  T take()
  {
    sem_wait(&items_);
    sem_wait(&mutex_);
    T ret(std::move(queue_.front()));
    queue_.pop_front();
    sem_post(&mutex_);
    return ret;
  }
  
  bool take(T &t)
  {
    sem_wait(&mutex_);
    bool ret = queue_.size() > 0;
    sem_post(&mutex_);
    if (ret) t = take();
    return ret;
  }
  
  size_t size() const
  {
    sem_wait(&mutex_);
    size_t ret = queue_.size();
    sem_post(&mutex_);
    return ret;
  }

private:
  sem_t mutex_;                 // binary semaphore for locking
  sem_t items_;
  std::deque<T> queue_;
};

#endif  // __BLOCKING_QUEUE_H__
