#pragma once

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include "Image.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class Images : public oatpp::DTO {
  DTO_INIT(Images, DTO)
  
  DTO_FIELD(List<Object<Image>>, images);
};

#include OATPP_CODEGEN_END(DTO)
