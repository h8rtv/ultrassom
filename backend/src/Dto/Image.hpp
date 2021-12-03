#pragma once

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

ENUM(Algorithm, v_int32,
     VALUE(CGNR, 0, "CGNR"),
     VALUE(CGNE, 1, "CGNE")
)

ENUM(Quality, v_int32,
     VALUE(LOW, 0),
     VALUE(MEDIUM, 1),
     VALUE(HIGH, 2)
)

class Image : public oatpp::DTO {
  DTO_INIT(Image, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(Enum<Algorithm>, algo) = Algorithm::CGNR;
  DTO_FIELD(Enum<Quality>::AsNumber, quality) = Quality::MEDIUM;
  DTO_FIELD(String, start_date);
  DTO_FIELD(String, end_date);
  DTO_FIELD(String, data);
  DTO_FIELD(Int32, height);
  DTO_FIELD(Int32, width);
  DTO_FIELD(Int32, iterations);
  DTO_FIELD(Float32, time);
  DTO_FIELD(Int32, user);
};

#include OATPP_CODEGEN_END(DTO)
