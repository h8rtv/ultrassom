#pragma once

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

#include "Dto/User.hpp"

#include "Service/ImageService.hpp"

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
  ImageService imageService;
public:
  
  ENDPOINT("GET", "/users/{id}", getUserById,
           PATH(Int32, id, "id"))
  {
    OATPP_LOGD("Test", "username=%s", id);
    return createResponse(Status::CODE_200, "OK");
  }

  ENDPOINT("POST", "/users", createUser,
           BODY_DTO(Object<User>, userDto))
  {
    
    return createResponse(Status::CODE_200, "OK");
  }
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen
