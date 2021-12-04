#pragma once

#include <oatpp/core/data/stream/FileStream.hpp>
#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/macro/component.hpp>
#include <oatpp/web/server/api/ApiController.hpp>
#include <oatpp/web/protocol/http/outgoing/StreamingBody.hpp>

#include <oatpp-websocket/Handshaker.hpp>
#include <oatpp/network/ConnectionHandler.hpp>

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
  OATPP_COMPONENT(std::shared_ptr<ImageService>, imageService);
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, websocketConnectionHandler, "websocket");

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
    return createDtoResponse(Status::CODE_200, imageService->getImagesByUser(user_id));
  }

  ENDPOINT("POST", "/images", createImage,
           BODY_DTO(Object<Image>, imageDto))
  {
    return createDtoResponse(Status::CODE_201, imageService->createImage(imageDto));
  }

  ENDPOINT("POST", "/images/{image_id}/signal", processSignal,
           BODY_STRING(String, body),
           REQUEST(std::shared_ptr<IncomingRequest>, request),
           PATH(Int32, image_id, "image_id"))
  {
    auto contentType = request->getHeaders().get(Header::CONTENT_TYPE);
    OATPP_ASSERT_HTTP(
      contentType == "text/csv",
      Status::CODE_415,
      "Unsupported Content Type"
    );
    return createDtoResponse(Status::CODE_202, imageService->processSignal(image_id, body->std_str()));
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

  ENDPOINT("GET", "users/{user_id}/ws", imageEvents,
           REQUEST(std::shared_ptr<IncomingRequest>, request),
           PATH(String, user_id, "user_id")) {
    auto response = oatpp::websocket::Handshaker::serversideHandshake(request->getHeaders(), websocketConnectionHandler);
    auto parameters = std::make_shared<oatpp::network::ConnectionHandler::ParameterMap>();
    
    (*parameters)["user"] = user_id;

    response->setConnectionUpgradeParameters(parameters);

    return response;
  };

};

#include OATPP_CODEGEN_END(ApiController) //<-- End Codegen
