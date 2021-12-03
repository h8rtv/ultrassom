#pragma once

#include "Scheduler/SchedulerQueue.hpp"

class SchedulerService {
private:
  SchedulerQueue queue;

public:
  void schedule(std::function<void()> task) {
    queue.push(task);
  }
};