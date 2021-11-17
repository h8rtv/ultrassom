#pragma once

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

#include <oatpp/web/mime/multipart/FileStreamProvider.hpp>
#include <oatpp/web/mime/multipart/Reader.hpp>
#include <oatpp/web/mime/multipart/PartList.hpp>

#include "../Dto/RegisterImage.hpp"
#include "../Dto/Images.hpp"

namespace multipart = oatpp::web::mime::multipart;

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Sample Api Controller.
 */
class Ultrasound : public oatpp::web::server::api::ApiController {
public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  Ultrasound(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
public:
  
  ENDPOINT("GET", "/users/{username}/images", getImagesByUser,
           PATH(String, username, "username"))
  {
    OATPP_LOGD("Test", "username=%s", username->getData());
    auto images = Images::createShared();
    return createDtoResponse(Status::CODE_200, images);
  }

  ENDPOINT("POST", "/images", postUsers,
           BODY_DTO(Object<RegisterImage>, registerImageDto))
  {
    return createResponse(Status::CODE_200, "OK");
  }
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen
