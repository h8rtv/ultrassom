#pragma once

#include <mutex>

#include "Service/WS/ClientConnectionListener.hpp"
#include "Persistence/ImageDb.hpp"

#include "Dto/Event.hpp"

class ProcessImageEmitter {
private:
  v_int32 clientId;
  std::shared_ptr<ClientConnectionListener> clientsListener;
  std::shared_ptr<ImageDb> imageDb;
  std::shared_ptr<std::mutex> dbLock;

  void update_image(const oatpp::Object<Image>& image) {
    try {
      std::lock_guard<std::mutex> lock(*dbLock);
      auto dbResult = imageDb->updateImage(image);
    } catch (const std::runtime_error& e) {
      OATPP_LOGE("ProcessImage", "DB Error: %s", e.what());
    }
  }

public:
  ProcessImageEmitter(
    v_int32 clientId,
    std::shared_ptr<ClientConnectionListener> clientsListener,
    std::shared_ptr<ImageDb> imageDb,
    std::shared_ptr<std::mutex> dbLock)
  : clientId(clientId),
    clientsListener(clientsListener),
    imageDb(imageDb),
    dbLock(dbLock)
  {}

  void emit(oatpp::Enum<EventType> type, const oatpp::Object<Image>& image) {
    update_image(image);
    auto event = Event::createShared();
    event->type = type;
    event->image = image;

    auto client = clientsListener->getClient(clientId);
    if (client != nullptr) {
      client->sendEvent(event);
    }
  }
};
