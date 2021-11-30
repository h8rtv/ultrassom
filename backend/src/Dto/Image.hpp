#pragma once

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class Image : public oatpp::DTO {
  DTO_INIT(Image, DTO)

  DTO_FIELD(String, username);
  DTO_FIELD(String, algo) = "CGNR";
  DTO_FIELD(String, start_date);
  DTO_FIELD(String, end_date);
  DTO_FIELD(String, data);
  DTO_FIELD(Int32, height);
  DTO_FIELD(Int32, width);
  DTO_FIELD(Int32, iterations);
  DTO_FIELD(Int32, reconstruction_time);
};

#include OATPP_CODEGEN_END(DTO)
