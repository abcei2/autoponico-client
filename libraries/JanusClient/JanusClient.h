#ifndef JANUS_CLIENT_H
#define JANUS_CLIENT_H

#include <WebSocketsClient.h>
#include <UUID.h>
#include <Arduino_JSON.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

class JanusClient
{
  UUID uuid;
  WebSocketsClient webSocket;

  const char* janusHost;  
  int janusHttpPort;
  int janusWsPort;

  const char *sessionId;
  void (*webSocketEvent)(WStype_t type, uint8_t *payload, size_t length);

public:
  JanusClient(const char *janusHost = "localhost", 
    int janusHttpPort = 8088, int janusWsPort = 8188
  );
  void connectJanusWs();
  void setWebSocketEventCallback(
    void (*webSocketEvent)(WStype_t type, uint8_t *payload, size_t length)
  );
  void janusRequests(const char *type);
  const char *newUUID();

  void wsLoop(){this->webSocket.loop();};
};

#endif
