#pragma once

#include <Magick++.h>
#include <Eigen/Dense>

void save_image(const Eigen::VectorXd& image, const std::string& filename) {
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
  img.write(filename);
}
