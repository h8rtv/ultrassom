#pragma once

#include <tbb/task_group.h>
#include <functional>

class SchedulerService {
private:
  tbb::task_group tg;

public:
  void schedule(std::function<void()> task) {
    tg.run(task);
  }
};