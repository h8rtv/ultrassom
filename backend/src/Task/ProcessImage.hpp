#pragma once

#include <string_view>
#include <oatpp/core/Types.hpp>

#include "Algorithm/Settings.hpp"
#include "Persistence/ImageDb.hpp"
#include "ModelMatrix.hpp"
#include "Algorithm/Factory.hpp"
#include "Parser/CSVParser.hpp"
#include "Util/ImageGeneration.hpp"
#include "Util/Uuid.hpp"
#include "Util/Time.hpp"

class ProcessImage {
private:
  std::shared_ptr<ImageDb> imageDb;
  const ModelMatrix& modelMatrix;
  oatpp::Object<Image> image;
  std::string data;

public:
  ProcessImage(
    oatpp::Object<Image> image,
    std::string data,
    const ModelMatrix& modelMatrix,
    std::shared_ptr<ImageDb> imageDb) 
  : image(image),
    data(data),
    modelMatrix(modelMatrix),
    imageDb(imageDb)
  {};

  std::string algorithm() {
    return oatpp::Enum<Algorithm>::getEntryByValue(image->algo).name.std_str();
  }

  Settings::Config config() {
    auto quality = (Settings::Quality) oatpp::Enum<Quality>::getEntryByValue(image->quality).index;
    return Settings::setting(quality);
  }

  std::unique_ptr<ISolver> create_solver() {
    return AlgorithmFactory::create(algorithm(), modelMatrix, config());
  }

  void operator()() {
    auto solver = create_solver();
    Eigen::VectorXd g = CSVParser(data).parse();
    auto finish = Util::Time::time_it();
    auto [f, iterations] = solver->solve(g);
    auto [time, start, end] = finish();

    std::string filename = Util::Uuid::generate_uuid_v4() + ".png";
    OATPP_LOGI("ProcessImage", "Generating image: %s", filename.c_str());
    OATPP_LOGI("ProcessImage", "Time: %f", time);
    Util::ImageGeneration::save_image(f, filename);

    image->start_date = start.c_str();
    image->end_date = end.c_str();
    image->data = filename.c_str();
    image->time = time;
    image->iterations = time;
    try {
      auto dbResult = imageDb->updateImage(image);
    } catch (const std::runtime_error e) {
      OATPP_LOGE("ProcessImage", "DB Error: %s", e.what());
    }
  };
};
