#pragma once

#include <oatpp/core/macro/codegen.hpp>
#include <oatpp/core/Types.hpp>

#include "Image.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

ENUM(EventType, v_int32,
     VALUE(ENQUEUED, 0, "ENQUEUED"),
     VALUE(START_PROCESSING, 1, "START_PROCESSING"),
     VALUE(FINISH_PROCESSING, 2, "FINISH_PROCESSING"),
     VALUE(FAILED_PROCESSING, 3, "FAILED_PROCESSING")
)

class Event : public oatpp::DTO {
  DTO_INIT(Event, DTO)

  DTO_FIELD(Enum<EventType>, type);
  DTO_FIELD(Object<Image>, image);
};

#include OATPP_CODEGEN_END(DTO)
