#pragma once

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class RegisterImage : public oatpp::DTO {
  DTO_INIT(RegisterImage, DTO)
  
  DTO_FIELD(String, name);
  DTO_FIELD(String, algo) = "CGNR";
  DTO_FIELD(String, data);
};

#include OATPP_CODEGEN_END(DTO)
