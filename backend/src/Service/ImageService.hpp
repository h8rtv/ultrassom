#pragma once

#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/core/macro/component.hpp>

#include "Schedule/Scheduler.hpp"
#include "Persistence/Repository/ImageDb.hpp"
#include "Dto/Image.hpp"


class ImageService {
private:
  typedef oatpp::web::protocol::http::Status Status;
private:
  OATPP_COMPONENT(std::shared_ptr<ImageDb>, imageDb); // Inject database component
  OATPP_COMPONENT(std::shared_ptr<Eigen::MatrixXd>, modelMatrix); // Inject model matrix component
  OATPP_COMPONENT(std::shared_ptr<Scheduler>, scheduler); // Inject task scheduler component
public:

  oatpp::Object<Image> createImage(const oatpp::Object<Image>& dto) {
    dto->height = 60;
    dto->width = 60;
    auto dbResult = imageDb->createImage(dto);
    OATPP_ASSERT_HTTP(dbResult->isSuccess(), Status::CODE_500, dbResult->getErrorMessage());

    u_int32_t user = dto->user;
    std::string data = dto->data->std_str();
    std::string algo = oatpp::Enum<Algorithm>::getEntryByValue(dto->algo).name.std_str();
    
    scheduler->schedule(UltrasoundTask{user, algo, data, modelMatrix});
    return dto;
  }

  oatpp::List<oatpp::Object<Image>> getImagesByUser(std::string_view username) {
    oatpp::List<oatpp::Object<Image>> images;
    return images;
  }
};
