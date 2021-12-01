#pragma once

#include "Schedule/Scheduler.hpp"

class ScheduleComponent {
public:

  /**
   *  Create Scheduler compoenent
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<Scheduler>, scheduler)([] {
    return std::make_shared<Scheduler>();
  }());
};
