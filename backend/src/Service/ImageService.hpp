#pragma once

#include <mutex>

#include <oatpp/orm/QueryResult.hpp>
#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/core/macro/component.hpp>

#include "WS/ClientConnectionListener.hpp"
#include "Persistence/ImageDb.hpp"
#include "SchedulerService.hpp"
#include "ModelMatrix.hpp"

#include "Task/ProcessImage.hpp"
#include "Task/ProcessImageEmitter.hpp"

#include "Dto/Image.hpp"

class ImageService {
private:
  typedef oatpp::web::protocol::http::Status Status;
private:
  OATPP_COMPONENT(std::shared_ptr<std::mutex>, dbLock); // Inject database lock component
  OATPP_COMPONENT(std::shared_ptr<ImageDb>, imageDb); // Inject database component
  OATPP_COMPONENT(std::shared_ptr<ModelMatrix>, modelMatrix); // Inject model matrix component
  OATPP_COMPONENT(std::shared_ptr<SchedulerService>, scheduler); // Inject task scheduler service
  OATPP_COMPONENT(std::shared_ptr<ClientConnectionListener>, clientsListener); // Inject client connection listener

public:
  oatpp::Object<Image> createImage(const oatpp::Object<Image>& dto) {
    dto->height = 60;
    dto->width = 60;
    std::shared_ptr<oatpp::orm::QueryResult> dbResult;
    {
      std::lock_guard<std::mutex> lock(*dbLock);
      imageDb->enableForeignKey();
      dbResult = imageDb->createImage(dto);
    }
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    v_int32 id = oatpp::sqlite::Utils::getLastInsertRowId(dbResult->getConnection());
    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<Image>>>();
    return getImageById(id);
  }

  oatpp::Object<Image> getImageById(v_int32 id) {
    std::shared_ptr<oatpp::orm::QueryResult> dbResult;
    {
      std::lock_guard<std::mutex> lock(*dbLock);
       dbResult = imageDb->getImageById(id);
    }
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
    OATPP_ASSERT_HTTP(dbResult->hasMoreToFetch(), Status::CODE_404, "Image not found");

    auto result = dbResult->fetch<oatpp::Vector<oatpp::Object<Image>>>();
    OATPP_ASSERT_HTTP(result->size() == 1, Status::CODE_500, "Unknown error");

    return result[0];
  }

  Eigen::VectorXd parseSignal(std::string_view data) {
    try {
      Eigen::VectorXd g = CSVParser(data).parse();
      OATPP_ASSERT_HTTP(g.cols() == 1 && g.rows() == 50816, Status::CODE_422, "Invalid vector size");
      return g;
    } catch (const CSVParserException& e) {
      throw oatpp::web::protocol::http::HttpError(Status::CODE_500, e.what());
    }
  }

  oatpp::Object<Image> processSignal(v_int32 id, std::string data) {
    try {
      auto image = getImageById(id);
      Eigen::VectorXd g = parseSignal(data);
      auto eventEmitter = std::make_shared<ProcessImageEmitter>(image->user, clientsListener, imageDb, dbLock);
      auto task = ProcessImage{image, g, *modelMatrix, eventEmitter};
      scheduler->schedule(task);
      image->status = ImageStatus::RECEIVED;
      eventEmitter->emit(EventType::ENQUEUED, image);
      return image;
    } catch (const oatpp::web::protocol::http::HttpError& e) {
      deleteImageById(id);
      throw oatpp::web::protocol::http::HttpError(e);
    }
  }

  void deleteImageById(v_int32 userId) {
    std::shared_ptr<oatpp::orm::QueryResult> dbResult;
    {
      std::lock_guard<std::mutex> lock(*dbLock);
      dbResult = imageDb->deleteImageById(userId);
    }
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());
  }

  oatpp::Vector<oatpp::Object<Image>> getImagesByUser(v_int32 userId) {
    std::shared_ptr<oatpp::orm::QueryResult> dbResult;
    {
      std::lock_guard<std::mutex> lock(*dbLock);
      dbResult = imageDb->getImagesByUser(userId);
    }
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    auto results = dbResult->fetch<oatpp::Vector<oatpp::Object<Image>>>();

    return results;
  }
};
