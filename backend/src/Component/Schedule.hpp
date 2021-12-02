#pragma once

#include "Service/SchedulerService.hpp"

class ScheduleComponent {
public:

  /**
   *  Create Scheduler compoenent
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<SchedulerService>, scheduler)([] {
    return std::make_shared<SchedulerService>();
  }());
};
