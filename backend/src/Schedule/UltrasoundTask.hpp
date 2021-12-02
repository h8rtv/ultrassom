#pragma once

#include <string_view>

#include "Persistence/Repository/ImageDb.hpp"
#include "ModelMatrix.hpp"
#include "Algorithm/Factory.hpp"
#include "Parser/CSVParser.hpp"
#include "Util/ImageGeneration.hpp"
#include "Util/Uuid.hpp"
#include "Util/Time.hpp"

class UltrasoundTask {
private:
  OATPP_COMPONENT(std::shared_ptr<ImageDb>, imageDb); // Inject database component
  OATPP_COMPONENT(std::shared_ptr<ModelMatrix>, modelMatrix); // Inject model matrix component

  oatpp::Object<Image> image;
  std::string algorithm;
  std::string data;

public:
  UltrasoundTask(
    oatpp::Object<Image> image,
    std::string algorithm,
    std::string data)
  : image(image),
    algorithm(algorithm),
    data(data)
  {};

  void operator()() const {
    auto config = get_config(Quality::LOW);
    auto solver = AlgorithmFactory::create(algorithm, *modelMatrix, config);

    Eigen::VectorXd g = CSVParser(data).parse();
    auto finish = Time::time_it();
    Eigen::VectorXd f = solver->solve(g);
    float time = finish();

    std::string filename = "out/" + Uuid::generate_uuid_v4() + ".png";
    OATPP_LOGI("UltrasoundTask", "Generating image: %s", filename.c_str());
    OATPP_LOGI("UltrasoundTask", "Time: %f", time);
    save_image(f, filename);

    try {
      image->data = filename.c_str();
      image->time = time;
      auto dbResult = imageDb->updateImage(image);
    } catch (const std::runtime_error e) {
      OATPP_LOGE("UltrasoundTask", "DB Error: %s", e.what());
    }
  };
};
