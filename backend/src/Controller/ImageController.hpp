#pragma once

#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/core/data/stream/FileStream.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/protocol/http/outgoing/StreamingBody.hpp>

#include "Dto/Image.hpp"

#include "Service/StaticFilesService.hpp"
#include "Service/ImageService.hpp"

#include OATPP_CODEGEN_BEGIN(ApiController) //<-- Begin Codegen

/**
 * Sample Api Controller.
 */
class ImageController : public oatpp::web::server::api::ApiController {
private:
  OATPP_COMPONENT(std::shared_ptr<StaticFilesService>, staticFilesService);
  ImageService imageService;

public:
  /**
   * Constructor with object mapper.
   * @param objectMapper - default object mapper used to serialize/deserialize DTOs.
   */
  ImageController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
  {}
  
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

  ENDPOINT("GET", "/images/*", getStaticImage,
           REQUEST(std::shared_ptr<IncomingRequest>, request))
  {
    auto filename = request->getPathTail();
    OATPP_ASSERT_HTTP(filename, Status::CODE_400, "Filename is empty");

    auto range = request->getHeader(Header::RANGE);
    auto file = staticFilesService->getFile(filename);

    OATPP_ASSERT_HTTP(file.get() != nullptr, Status::CODE_404, "File not found");

    std::shared_ptr<OutgoingResponse> response = createResponse(Status::CODE_200, file);

    response->putHeader("Accept-Ranges", "bytes");
    response->putHeader(Header::CONNECTION, Header::Value::CONNECTION_KEEP_ALIVE);
    auto mimeType = staticFilesService->guessMimeType(filename);
    if (mimeType) {
      response->putHeader(Header::CONTENT_TYPE, mimeType);
    } else {
      OATPP_LOGD("Server", "Unknown Mime-Type. Header not set");
    }

    return response;
  }

  ENDPOINT("POST", "/images/{image_id}/signal", processSignal,
           BODY_STRING(String, body),
           PATH(Int32, image_id, "image_id"))
  {
    return createDtoResponse(Status::CODE_202, imageService.processSignal(image_id, body->std_str()));
  }
  
};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen
