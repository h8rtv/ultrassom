#pragma once

#include "Service/SchedulerService.hpp"
#include "Service/ImageService.hpp"
#include "Service/UserService.hpp"
#include "Service/StaticFilesService.hpp"

class ServiceComponent {
public:

  /**
   *  Create Scheduler service compoenent
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<SchedulerService>, scheduler)([] {
    return std::make_shared<SchedulerService>();
  }());

  /**
   *  Create StaticFiles service component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<StaticFilesService>, staticFilesService)([] {
    return std::make_shared<StaticFilesService>(OUTPUT_DIR);
  }());

  /**
   *  Create User service component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<UserService>, userService)([] {
    return std::make_shared<UserService>();
  }());

  /**
   *  Create User service component
   */
  OATPP_CREATE_COMPONENT(std::shared_ptr<ImageService>, imageService)([] {
    return std::make_shared<ImageService>();
  }());
};
