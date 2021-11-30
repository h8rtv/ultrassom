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
    OATPP_LOGD("Test", "user_id=%d", user_id);
    return createResponse(Status::CODE_200, "OK");
  }

  ENDPOINT("POST", "/images", createImage,
           BODY_DTO(Object<Image>, imageDto))
  {
    return createDtoResponse(Status::CODE_202, imageService.createImage(imageDto));
  }

  ENDPOINT("POST", "/images/{image_id}/upload", uploadImage,
           BODY_STRING(String, body),
           PATH(Int32, image_id, "image_id"))
  {
    std::string filename = "signals/" + std::to_string(image_id) + ".csv";
    body->saveToFile(filename.c_str());
    return createResponse(Status::CODE_202, "ACCEPTED");
  }
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen
