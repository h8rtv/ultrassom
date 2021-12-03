#pragma once

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include OATPP_CODEGEN_BEGIN(DTO)

class User : public oatpp::DTO {
  DTO_INIT(User, DTO)

  DTO_FIELD(Int32, id);
  DTO_FIELD(String, name);
};

#include OATPP_CODEGEN_END(DTO)
