#pragma once

#include <tbb/concurrent_hash_map.h>

#include <oatpp-websocket/ConnectionHandler.hpp>
#include <oatpp-websocket/WebSocket.hpp>

#include <oatpp/core/utils/ConversionUtils.hpp>

#include "Client.hpp"

/**
 * Listener on new WebSocket connections.
 */
class ClientConnectionListener : public oatpp::websocket::ConnectionHandler::SocketInstanceListener {
private:
  tbb::concurrent_hash_map<v_int32, std::shared_ptr<Client>> clients;

public:
  std::shared_ptr<Client> getClient(v_int32 id) {
    tbb::concurrent_hash_map<v_int32, std::shared_ptr<Client>>::accessor acc;
    if (clients.find(acc, id)) {
      return acc->second;
    }
    return nullptr;
  }

  /**
   *  Called when socket is created
   */
  void onAfterCreate(const oatpp::websocket::WebSocket& socket, const std::shared_ptr<const ParameterMap>& params) override {
    auto user_str = params->find("user")->second;
    bool success = false;
    auto user = oatpp::utils::conversion::strToInt32(user_str, success);
    OATPP_LOGD("WSInstance", "User = %d ", user);
    OATPP_LOGD("WSInstance", "New Incoming Connection");
    auto client = std::make_shared<Client>(user, socket);
    socket.setListener(client);
    clients.insert({user, client});
  }
  /**
   *  Called before socket instance is destroyed.
   */
  void onBeforeDestroy(const oatpp::websocket::WebSocket& socket) override {
    OATPP_LOGD("WSInstance", "Connection closed");
    auto client = std::static_pointer_cast<Client>(socket.getListener());
    client->disable();
    clients.erase(client->getId());
  }
};
