#pragma once

#include "Service/WS/Client.hpp"
#include "Persistence/ImageDb.hpp"

#include "Dto/Event.hpp"

class ProcessImageEmitter {
private:
  std::shared_ptr<Client> client;
  std::shared_ptr<ImageDb> imageDb;

  void update_image(const oatpp::Object<Image>& image) {
    try {
      auto dbResult = imageDb->updateImage(image);
    } catch (const std::runtime_error& e) {
      OATPP_LOGE("ProcessImage", "DB Error: %s", e.what());
    }
  }

public:
  ProcessImageEmitter(std::shared_ptr<Client> client, std::shared_ptr<ImageDb> imageDb)
  : client(client), imageDb(imageDb)
  {}

  void emit(oatpp::Enum<EventType> type, const oatpp::Object<Image>& image) {
    update_image(image);
    auto event = Event::createShared();
    event->type = type;
    event->image = image;

    if (client != nullptr) {
      client->sendEvent(event);
    }
  }
};
