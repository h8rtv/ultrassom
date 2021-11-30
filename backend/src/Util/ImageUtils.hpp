#pragma once

#include <iostream>
#include <Magick++.h>
#include <Eigen/Dense>

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

  return blob.base64();
}
