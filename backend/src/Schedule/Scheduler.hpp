#pragma once

#include <tbb/task_group.h>
#include "UltrasoundTask.hpp"

class Scheduler {
private:
  tbb::task_group tg;

public:
  void schedule(UltrasoundTask task) {
    tg.run(task);
  }
};