#include <JanusClient.h>

JanusClient::JanusClient(const char *janusHost, int janusHttpPort, int janusWsPort)
{
  this->uuid.seed(2);
  this->janusHost = janusHost;
  this->janusHttpPort = janusHttpPort;
  this->janusWsPort = janusWsPort;
}


void JanusClient::setWebSocketEventCallback(
  void (*webSocketEvent)(WStype_t type, uint8_t *payload, size_t length)
){
  this->webSocketEvent = webSocketEvent;
};

void JanusClient::connectJanusWs()
{
  // server address, port and URL
  this->webSocket.begin(this->janusHost, this->janusWsPort, "/", "janus-protocol");
  // event handler
  this->webSocket.onEvent(webSocketEvent);
  this->webSocket.setReconnectInterval(5000);
  // start heartbeat (optional)
  // ping server every 15000 ms
  // expect pong from server within 3000 ms
  // consider connection disconnected if pong is not received 2 times
  this->webSocket.enableHeartbeat(15000, 3000, 2);
}

const char *JanusClient::newUUID()
{
  uuid.generate();
  return uuid.toCharArray();
}


void JanusClient::janusRequests( const char *type)
{
  HTTPClient http;
  WiFiClient client;

  JSONVar data, response;
  char janusHttpPortStr[10];
  itoa(this->janusHttpPort, janusHttpPortStr, 10);

  char hostUrlBuf[100 /*Path*/ + 10 /*protocol ej; http://*/];
  strcpy(hostUrlBuf, "http://");
  strcat(hostUrlBuf, this->janusHost);
  strcat(hostUrlBuf, ":");
  strcat(hostUrlBuf, janusHttpPortStr);
  strcat(hostUrlBuf, "/janus");

  data["janus"] = type;
  if (type == "create")
    data["token"] = "123456789";
  else if (type == "attach")
  {
    strcat(hostUrlBuf, "/");
    strcat(hostUrlBuf, this->sessionId);
    data["plugin"] = "janus.plugin.textroom";
  }
  data["transaction"] = this->newUUID();
  // configure traged server and url
  http.begin(client, hostUrlBuf); // HTTP
  http.addHeader("Content-Type", "application/json");

  // start connection and send HTTP header and body
  int httpCode = http.POST(JSON.stringify(data));

  // httpCode will be negative on error
  if (httpCode > 0)
  {
    // file found at server
    if (httpCode == HTTP_CODE_OK)
    {
      response = JSON.parse(http.getString());
      if (type == "create")
        this->sessionId = JSON.stringify(response["data"]["id"]).c_str();
    }
  }
  else
  {
    //error
  }

  http.end();
}



