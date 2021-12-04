#pragma once

#include <tbb/concurrent_queue.h>
#include <tbb/task_group.h>

#include <functional>
#include <atomic>
#include <thread>

class SchedulerQueue {
private:
  std::atomic_bool running;
  tbb::concurrent_bounded_queue<std::function<void()>> pending;
  tbb::concurrent_bounded_queue<bool> executing;

  void process_queue() {
    tbb::task_group tg;
    std::function<void()> task;
    while (true) {
      pending.pop(task);
      executing.push(true);
      tg.run([&, task] {
        bool finished_executing;
        task();
        executing.pop(finished_executing);
      });
    }
  }

  void run() {
    if (!running) {
      running = true;
      std::thread coordinator([&] {
        process_queue();
        running = false;
      });
      coordinator.detach();
    }
  }

public:
  SchedulerQueue()
  : running(false)
  {
#ifdef PARALLEL_TASKS
    executing.set_capacity(PARALLEL_TASKS);
#endif
    run();
  }

  void push(std::function<void()> task) {
    pending.push(task);
  }
};