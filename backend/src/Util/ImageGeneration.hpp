#pragma once

#include <iostream>
#include <Magick++.h>
#include <Eigen/Dense>

#include <random>
#include <sstream>

namespace uuid {
  static std::random_device              rd;
  static std::mt19937                    gen(rd());
  static std::uniform_int_distribution<> dis(0, 15);
  static std::uniform_int_distribution<> dis2(8, 11);

  std::string generate_uuid_v4() {
    std::stringstream ss;
    int i;
    ss << std::hex;
    for (i = 0; i < 8; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    ss << "-4";
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    ss << dis2(gen);
    for (i = 0; i < 3; i++) {
        ss << dis(gen);
    }
    ss << "-";
    for (i = 0; i < 12; i++) {
        ss << dis(gen);
    };
    return ss.str();
  }
}

[[nodiscard]]
std::string gen_image(const Eigen::VectorXd& image) {
  Magick::Blob blob;
  Magick::Image img{
    60,
    60,
    "R",
    Magick::StorageType::DoublePixel,
    image.data()
  };
  img.normalize();
  img.magick("PNG");
  img.rotate(90);
  img.flop();
  img.write(&blob);
  img.write("out/" + uuid::generate_uuid_v4() + ".png");

  return blob.base64();
}
