#pragma once

#include <oatpp-websocket/WebSocket.hpp>

#include "Dto/Event.hpp"

/**
 * WebSocket listener listens on incoming WebSocket events.
 */
class Client : public oatpp::websocket::WebSocket::Listener {
private:
  OATPP_COMPONENT(std::shared_ptr<oatpp::data::mapping::ObjectMapper>, objectMapper);
  bool enabled;
  v_int32 id;
  const oatpp::websocket::WebSocket& socket;
public:
  Client(v_int32 id, const oatpp::websocket::WebSocket& socket)
  : id(id),
    socket(socket),
    enabled(true)
  {}

  void disable() {
    enabled = false;
  }

  v_int32 getId() const {
    return id;
  }

  void sendEvent(const oatpp::Object<Event>& event) {
    if (enabled) {
      auto message = objectMapper->writeToString(event);
      socket.sendOneFrameText(message);
    }
  }

  /**
   * Called on "ping" frame.
   */
  void onPing(const WebSocket& socket, const oatpp::String& message) {
    OATPP_LOGD("WS", "onPing");
    socket.sendPong(message);
  }

  void onPong(const WebSocket& socket, const oatpp::String& message) {
    OATPP_LOGD("WS", "onPong");
  }

  void onClose(const WebSocket& socket, v_uint16 code, const oatpp::String& message) {
    OATPP_LOGD("WS", "onClose code=%d", code);
  }

  void readMessage(const WebSocket& socket, v_uint8 opcode, p_char8 data, oatpp::v_io_size size) {
  }
};
