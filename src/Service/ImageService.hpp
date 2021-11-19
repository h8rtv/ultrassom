
#pragma once

#include <oatpp/web/protocol/http/Http.hpp>
#include <oatpp/core/macro/component.hpp>

#include "Persistence/Repository/ImageDb.hpp"
#include "Dto/RegisterImage.hpp"
#include "Dto/Image.hpp"

#include "AlgebraState.hpp"
#include "Algorithm/Factory.hpp"
#include "Parser/CSVParser.hpp"
#include "Util/ImageUtils.hpp"

class ImageService {
private:
  typedef oatpp::web::protocol::http::Status Status;
private:
  OATPP_COMPONENT(std::shared_ptr<ImageDb>, imageDb); // Inject database component
public:

  oatpp::Object<Image> createImage(const oatpp::Object<RegisterImage>& dto) {
    auto H = AlgebraState::instance().get_model_matrix();
    auto solver = AlgorithmFactory::create(dto->algo->std_str());
    Eigen::VectorXd g = CSVParser(std::string(dto->signal->std_str())).parse();
    Eigen::VectorXd f = solver->solve(g, H);
    std::string img = gen_image(f);

    return Image::createShared();
  }

  oatpp::List<oatpp::Object<Image>> getImagesByUser(std::string_view username) {
    oatpp::List<oatpp::Object<Image>> images;
    return images;
  }
};
