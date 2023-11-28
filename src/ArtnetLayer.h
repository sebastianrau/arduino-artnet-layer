#ifndef ARTNET_WIFI_H
#define ARTNET_WIFI_H

#include <Arduino.h>
#include "ArtnetProtocolDef.h"



#if defined(__AVR_ATmega328P__)
#include <avr/pgmspace.h>
#endif

//FORMAT: MINOR.MAYOR
#define ART_FIRMWARE_VERSION    0x0100

typedef struct {
  uint8_t  mac[6];
  uint8_t  ip[4];
  uint8_t  subnet[4];
  uint8_t  gateway[4];
  uint8_t  dnsServer[4];
  uint8_t  dhcpEnable;
  char  shortName[18];
  char  longName[64];
  ART_NET_UNIVERSE_T select_universe;
  uint32_t crc32;
} NODE_CONFIGURATION_T;


const  NODE_CONFIGURATION_T defaultConfig
#if defined(__AVR_ATmega328P__) 
PROGMEM
#endif
= {
  {0x00, 0x00, 0x00, 0x00, 0x00, 0x00},     // mac
  {192, 168, 0, 7},                       	// IP
  {255, 255, 255, 0},                       // SUBNET MASK
  {0, 0, 0, 0},                             // DNS
  {0, 0, 0, 0},                             // Gateway
  1,                                        // DHCP Enabled
  "WiFi LED BAR\0",                         // Short name
  "WiFi LED BAR unconfigured\0",            // Long Name
  0x0001,                                   // DMX Universe
  0x00000000
};





class ArtnetLayer
{
  public:
    NODE_CONFIGURATION_T nodeConfig;

    void begin();
    int32_t read(uint16_t packetSize);

    uint8_t isConfigCrcValid();

    inline ArtnetLayer(
      void (*udpSend)(uint8_t ip[4], uint16_t port, uint8_t* packetData, size_t len, uint8_t broadcast),
      void (*networkRestart)(void)) {

      updSendCallback = udpSend;
      networkRestartCallback = networkRestart;
    }
    inline void setConfigChangedCallback(void (*fptr)(NODE_CONFIGURATION_T* config)) {
      configChangedCallback = fptr;
    }
    inline void setArtDmxCallback(void (*fptr)(uint8_t* data, uint16_t length, uint8_t sequence)) {
      artDmxCallback = fptr;
    }

    inline uint8_t* getPacketBuffer() {
      return artnetPacket;
    }

  private:

    const unsigned long crc_table[16] = {
      0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
      0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
      0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
      0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
    };
    uint8_t broadcastAdr[4] = {0xFF, 0xFF, 0xFF, 0xFF};

    uint8_t       artnetPacket[ART_NET_BUFFER_SIZE]; //buffer to store incoming data
    unsigned int  pollCounter = 0;                  //must be compatible with sprintf("...%4d...");
    uint16_t      packetSize = 0;

    //Callback FunktionPointer
    void (*updSendCallback)(uint8_t ip[4], uint16_t port, uint8_t* packetData, size_t len, uint8_t broadcast);
    void (*networkRestartCallback)(void);
    void (*configChangedCallback)(NODE_CONFIGURATION_T* config);

    void (*artDmxCallback)(uint8_t* data, uint16_t length, uint8_t sequence);


    //handle OP Codes
    void handleArtDmx(uint8_t artnetPacket[ART_NET_BUFFER_SIZE]);
    void handleArtPoll(uint8_t artnetPacket[ART_NET_BUFFER_SIZE]);
    void handleArtIpProg(uint8_t artnetPacket[ART_NET_BUFFER_SIZE]);
    void handleArtAddress(uint8_t artnetPacket[ART_NET_BUFFER_SIZE]);

    //sending data back
    void sendArtPollReply();
    void sendArtProgIpReply();

    void sendUdp(uint8_t ip[4], uint16_t port, uint8_t* packetData, size_t len, uint8_t broadcast);
    void restartNetwork();
    void configChanged();

    //internal
    uint32_t  calculateCRC(uint8_t* data, size_t len);
    void   updateConfigCrc();
  


};

#endif
