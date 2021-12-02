#pragma once

#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/core/macro/component.hpp>

#include "Task/ProcessImage.hpp"
#include "SchedulerService.hpp"
#include "Persistence/ImageDb.hpp"
#include "Dto/Image.hpp"
#include "ModelMatrix.hpp"

class ImageService {
private:
  typedef oatpp::web::protocol::http::Status Status;
private:
  OATPP_COMPONENT(std::shared_ptr<ImageDb>, imageDb); // Inject database component
  OATPP_COMPONENT(std::shared_ptr<ModelMatrix>, modelMatrix); // Inject model matrix component
  OATPP_COMPONENT(std::shared_ptr<SchedulerService>, scheduler); // Inject task scheduler service

public:
  oatpp::Object<Image> createImage(const oatpp::Object<Image>& dto) {
    dto->height = 60;
    dto->width = 60;
    auto dbResult = imageDb->createImage(dto);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    v_int32 id = oatpp::sqlite::Utils::getLastInsertRowId(dbResult->getConnection());
    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<Image>>>();
    return getImageById(id);
  }

  oatpp::Object<Image> getImageById(v_int32 id) {
    auto dbResult = imageDb->getImageById(id, nullptr);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "Image not found");

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<Image>>>();
    OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");

    return result[0];
  }

  oatpp::Object<Image> processSignal(v_int32 id, std::string data) {
    auto image = getImageById(id);

    auto task = ProcessImage{image, data, *modelMatrix, imageDb};
    scheduler->schedule(task);
    return image;
  }

  oatpp::Vector<oatpp::Object<Image>> getImagesByUser(v_int32 userId) {
    auto dbResult = imageDb->getImagesByUser(userId);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    auto results = dbResult->fetch<oatpp::Vector<oatpp::Object<Image>>>();

    return results;
  }
};
