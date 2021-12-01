#pragma once

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/data/stream/FileStream.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>

#include "Dto/Image.hpp"

#include "Service/ImageService.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Sample Api Controller.
 */
class ImageController : public oatpp::web::server::api::ApiController {
public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  ImageController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
private:
  ImageService imageService;
public:
  
  ENDPOINT("GET", "/users/{user_id}/images", getImagesByUser,
           PATH(Int32, user_id, "user_id"))
  {
    return createDtoResponse(Status::CODE_200, imageService.getImagesByUser(user_id));
  }

  ENDPOINT("POST", "/images", createImage,
           BODY_DTO(Object<Image>, imageDto))
  {
    return createDtoResponse(Status::CODE_201, imageService.createImage(imageDto));
  }

  ENDPOINT("POST", "/images/{image_id}/signal", processSignal,
           BODY_STRING(String, body),
           PATH(Int32, image_id, "image_id"))
  {
    return createDtoResponse(Status::CODE_202, imageService.processSignal(image_id, body->std_str()));
  }
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen
