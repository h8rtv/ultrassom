#pragma once

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

#include "Dto/User.hpp"

#include "Service/UserService.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Sample Api Controller.
 */
class UserController : public oatpp::web::server::api::ApiController {
public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  UserController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
private:
  OATPP_COMPONENT(std::shared_ptr<UserService>, userService);
public:
  
  ENDPOINT("GET", "/users/{id}", getUserById,
           PATH(Int32, id, "id"))
  {
    return createDtoResponse(Status::CODE_200, userService->getUserById(id));
  }

  ENDPOINT("GET", "/users", getUserByName,
           QUERY(String, name)) 
  {
    return createDtoResponse(Status::CODE_200, userService->getUserByName(name));
  }

  ENDPOINT("POST", "/users", createUser,
           BODY_DTO(Object<User>, userDto))
  {
    return createDtoResponse(Status::CODE_201, userService->createUser(userDto));
  }
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen
