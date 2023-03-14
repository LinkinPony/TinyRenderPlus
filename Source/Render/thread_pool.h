//
// Created by linkinpony on 23-3-11.
//

#ifndef SIMPLETHREADPOOL_THREAD_POOL_H
#define SIMPLETHREADPOOL_THREAD_POOL_H

#include <future>
#include <mutex>
#include <thread>
#include <chrono>
#include <queue>
#include <vector>
#include <functional>
#include <memory>
#include <iostream>


namespace ThreadPool {
static const size_t kPoolSize = 20;
template <typename T,size_t SIZE>
class LockFreeQueue{
 public:
  bool enqueue(const T & in_var);
  bool dequeue(T & ret_var);
  LockFreeQueue();
 private:
  //head of queue
  std::atomic_size_t head_;
  //reserved tail for data waiting to be stored
  std::atomic_size_t tail_res_;
  //the tail of **stored** data
  std::atomic_size_t tail_;
  T data_[SIZE];
};
template<typename T, size_t SIZE>
LockFreeQueue<T, SIZE>::LockFreeQueue() {
  head_ = 0;
  tail_ = 0;
  tail_res_ = 0;
}
template<typename T, size_t SIZE>
bool LockFreeQueue<T, SIZE>::dequeue(T &ret_var) {
  size_t cur_head = head_;
  size_t cur_tail;
  do{
    cur_tail = tail_;
    if(cur_tail == cur_head){
      return false;
    }
    //this statement must in loop!
    ret_var = data_[cur_head];
  }while(head_.compare_exchange_strong(cur_head,(cur_head + 1) % SIZE));
  return true;
}
template<typename T, size_t SIZE>
bool LockFreeQueue<T, SIZE>::enqueue(const T &in_var) {

  size_t cur_head = head_;
  size_t cur_tail = tail_res_;
  do{
    cur_head = head_;
    //queue is full
    if((cur_tail + 1) % SIZE == cur_head){
      return false;
    }
  }while(!tail_res_.compare_exchange_strong(cur_tail,(cur_tail+1) % SIZE));
  data_[cur_tail] = in_var;
  while(!tail_.compare_exchange_strong(cur_tail,(cur_tail+1)%SIZE));
  return true;
}
class ThreadPool {
 public:
  //submit a task, return the future of this task.
  template<typename F, typename... Args>
  auto submit(F &&f, Args &&... args) -> std::future<decltype(f(args...))>;
 public:
  explicit ThreadPool(size_t thread_num = std::thread::hardware_concurrency());
  ~ThreadPool();
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool(ThreadPool &&) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;
  ThreadPool &operator=(ThreadPool &&) = delete;
 public:
  [[nodiscard]] bool isRun() const { return run_; }
  //Terminate and discard all queued works
  void stop();
 public:
  std::mutex worker_mutex_;
  std::condition_variable queue_cond_;
  LockFreeQueue<std::function<void()>,kPoolSize> task_queue_;
 private:
  void init();
 private:
  bool run_;
  int thread_num_;
  std::vector<std::thread> threads_;
};
//extract tasks in queue and handle them. each worker is a endless thread.
class ThreadWorker {
 public:
  ThreadWorker(ThreadPool *pool, int id);
  void work();
  void operator()();
 private:
  void run();
 private:
  int id_;
  //we don't care how to manage pool's resource in worker, so just use bare pointer.
  ThreadPool *pool_;
};
template<typename F, typename... Args>
auto ThreadPool::submit(F &&f, Args &&... args) -> std::future<decltype(f(args...))> {
  //bind origin function into func. now we can finish task by simply call func().
  auto func = std::bind(
      std::forward<F>(f),
      std::forward<Args>(args)...
  );
  using kTaskType = std::packaged_task<decltype(f(args...))()>;
  auto task_ptr = std::make_shared<kTaskType>(func);
  //use closure to capture task pointer and wrap it into void function.
  //so we can process these task in a queue easily.
  auto closure = std::function<void()>(
      [task_ptr] {
        (*task_ptr)();
      }
  );
  auto res = task_ptr->get_future();
  task_queue_.enqueue(closure);
  queue_cond_.notify_one();
  return res;
}
};
#endif //SIMPLETHREADPOOL_THREAD_POOL_H
