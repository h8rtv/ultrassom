#pragma once

#include <Magick++.h>
#include <array>
#include <Eigen/Dense>

namespace Util::ImageGeneration {
  void plot(Eigen::VectorXd& f, const std::string& filename) {
    std::ofstream image_file("out/test.pgm");
    image_file << "P2" << '\n' << 60 << ' ' << 60 << '\n' << "255" << '\n';

    for (auto j = 0u; j < 60; j++) {
        for (auto i = 0u; i < 60; i++) {
          unsigned int pixel = (unsigned int) f(60 * i + j);
          image_file << pixel << " ";
        }
        image_file << "\n";
    }

    image_file.close();
    Magick::Image image_magick;
    image_magick.read("out/test.pgm");
    image_magick.write(OUTPUT_DIR + filename);
  }

  void save_image(const Eigen::VectorXd& image, const std::string& filename) {
    std::array<double, 3600> buffer;

    for (auto j = 0u; j < 60; j++) {
        for (auto i = 0u; i < 60; i++) {
          double pixel = image(60 * j + i);
          buffer[60 * i + j] = pixel;
        }
    }
    Magick::Blob blob;
    Magick::Image img{
      60,
      60,
      "I",
      Magick::StorageType::DoublePixel,
      buffer.data()
    };
    img.magick("PNG");
    img.write(&blob);
    img.write(OUTPUT_DIR + filename);
  }
}