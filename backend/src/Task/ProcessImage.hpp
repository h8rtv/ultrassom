#pragma once

#include <string_view>
#include <oatpp/core/Types.hpp>

#include "ProcessImageEmitter.hpp"
#include "ModelMatrix.hpp"

#include "Parser/CSVParser.hpp"
#include "Algorithm/Settings.hpp"
#include "Algorithm/Factory.hpp"

#include "Util/ImageGeneration.hpp"
#include "Util/Uuid.hpp"
#include "Util/Time.hpp"

class ProcessImage {
private:
  std::shared_ptr<ProcessImageEmitter> eventEmitter;
  const ModelMatrix& modelMatrix;
  oatpp::Object<Image> image;
  std::string data;

  std::string persist_file(const Eigen::VectorXd& f) {
    std::string filename = Util::Uuid::generate_uuid_v4() + ".png";
    OATPP_LOGI("ProcessImage", "Generating image: %s", filename.c_str());
    Util::ImageGeneration::save_image(f, filename);
    return filename;
  }

  void normalize_data(Eigen::VectorXd& f, std::pair<double, double> range = {0.f, 1.f}) {
    auto [a, b] = range;
    double min = f.minCoeff();
    double max = f.maxCoeff();
    f = a + (f.array() - min) * ((b - a) / (max - min));
  }

public:
  ProcessImage(
    oatpp::Object<Image> image,
    std::string data,
    const ModelMatrix& modelMatrix,
    std::shared_ptr<ProcessImageEmitter> eventEmitter) 
  : image(image),
    data(data),
    modelMatrix(modelMatrix),
    eventEmitter(eventEmitter)
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
    auto [start, finish] = Util::Time::time_it();

    image->start_date = start.c_str();
    eventEmitter->emit(EventType::START_PROCESSING, image);

    auto [f, iterations] = solver->solve(g);
    normalize_data(f);

    auto [time, end] = finish();
    OATPP_LOGI("ProcessImage", "Time: %f", time);

    std::string filename = persist_file(f);
    image->end_date = end.c_str();
    image->data = filename.c_str();
    image->time = time;
    image->iterations = iterations;

    eventEmitter->emit(EventType::FINISH_PROCESSING, image);
  };
};
