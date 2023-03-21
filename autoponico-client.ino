#include <Arduino.h>
#include <Arduino_JSON.h>
#include <JanusClient.h>

JanusClient janus("192.168.80.13"); 

#define USE_SERIAL Serial

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{

	switch (type)
	{
		case WStype_DISCONNECTED:
			USE_SERIAL.printf("[WSc] Disconnected!\n");
			break;
		case WStype_CONNECTED:
		{
			USE_SERIAL.printf("[WSc] Connected to url: %s\n", payload);
			// send message to server when Connected
		}
		break;
		case WStype_TEXT:
			USE_SERIAL.printf("[WSc] get text: %s\n", payload);
			// send message to server
			// webSocket.sendTXT("message here");
			break;
		case WStype_BIN:
			USE_SERIAL.printf("[WSc] get binary length: %u\n", length);
			hexdump(payload, length);
			// send data to server
			// webSocket.sendBIN(payload, length);
			break;
		case WStype_PING:
			// pong will be send automatically
			USE_SERIAL.printf("[WSc] get ping\n");
			break;
		case WStype_PONG:
			// answer to a ping we send
			USE_SERIAL.printf("[WSc] get pong\n");
			break;
	}
}

void setup()
{
	// USE_SERIAL.begin(921600);
	USE_SERIAL.begin(115200);

	for (uint8_t t = 4; t > 0; t--)
	{
		USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
		USE_SERIAL.flush();
		delay(1000);
	}

	WiFi.begin("JUANMA", "42782896");

	// WiFi.disconnect();
	while (WiFi.status() != WL_CONNECTED)
	{
		delay(100);
	}

	janus.janusRequests("create");
	janus.janusRequests("attach");
	janus.setWebSocketEventCallback(webSocketEvent);
	janus.connectJanusWs();
}

void loop()
{
	janus.wsLoop();
}
