#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <EEPROM.h>
#include "ArtnetLayer.h"

#define PIN_LED                 2
#define DMX_DATA_SIZE           180
#define EEPROM_START_ADDRESS    0x00

void startEthernet();
void loadConfig();
void loadDefaultConfig();


//Callbacks from Artnet
void onArtUpdSend(uint8_t ip[4], uint16_t port, uint8_t* packetData, size_t len, uint8_t broadcast);
void onNetworkRestart();
void onConfigChange(NODE_CONFIGURATION_T* config);
void onArtDmxFrame(uint8_t* data, uint16_t length, uint8_t sequence);

EthernetUDP Udp;
ArtnetLayer artnet(onArtUpdSend, onNetworkRestart);

uint8_t dmxArray[DMX_DATA_SIZE] = {0};




void setup() {
  Serial.begin(115200);
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  loadConfig();
  artnet.setConfigChangedCallback(onConfigChange);
  artnet.setArtDmxCallback(onArtDmxFrame);
  artnet.begin();
}

void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize > 0 && packetSize <= ART_NET_BUFFER_SIZE) {
    Udp.read(artnet.getPacketBuffer(), ART_NET_BUFFER_SIZE);
    artnet.read(packetSize);
  }
}

void onArtUpdSend(uint8_t ip[4], uint16_t port, uint8_t* packetData, size_t len, uint8_t broadcast) {
  IPAddress ipAddress(ip[0], ip[1], ip[2], ip[3]);
  if (broadcast) {
    ipAddress = ~Ethernet.subnetMask() | Ethernet.localIP();
  }
  Udp.beginPacket(ipAddress, port);
  Udp.write(packetData, len);
  Udp.endPacket();
}

void onNetworkRestart() {
  Udp.stop();
  if (artnet.nodeConfig.dhcpEnable) {
    Ethernet.begin(artnet.nodeConfig.mac);
    artnet.nodeConfig.ip[0] = Ethernet.localIP()[0];          //get IP
    artnet.nodeConfig.ip[1] = Ethernet.localIP()[1];          //get IP
    artnet.nodeConfig.ip[2] = Ethernet.localIP()[2];          //get IP
    artnet.nodeConfig.ip[3] = Ethernet.localIP()[3];          //get IP

    artnet.nodeConfig.subnet[0] = Ethernet.subnetMask()[0];   //get Subnet
    artnet.nodeConfig.subnet[1] = Ethernet.subnetMask()[1];   //get Subnet
    artnet.nodeConfig.subnet[2] = Ethernet.subnetMask()[2];   //get Subnet
    artnet.nodeConfig.subnet[3] = Ethernet.subnetMask()[3];   //get Subnet
  } else {
    Ethernet.begin(artnet.nodeConfig.mac, artnet.nodeConfig.ip, artnet.nodeConfig.dnsServer, artnet.nodeConfig.gateway, artnet.nodeConfig.subnet);
  }

  Serial.println(Ethernet.localIP());
  Serial.println(Ethernet.subnetMask());
  Udp.begin(ART_NET_PORT);
}


void onArtDmxFrame(uint8_t* data, uint16_t length, uint8_t sequence) {
  Serial.print(F("DMX Frame "));
  Serial.println(sequence);
  if (length < DMX_DATA_SIZE) {
    memcpy(&dmxArray, data, length );
  } else {
    memcpy(&dmxArray, data, DMX_DATA_SIZE );
  }
}


void onConfigChange(NODE_CONFIGURATION_T* config) {
  writeConfigEEPROM(config, sizeof(NODE_CONFIGURATION_T));
}

void loadConfig() {
  readConfigEEPROM(&artnet.nodeConfig, sizeof(NODE_CONFIGURATION_T) );

  if (!artnet.isConfigCrcValid()) {
    loadDefaultConfig();
    Serial.println(F("CONFIG LOAD FAILED. RESTORE DEFAULT."));
  } else {
    Serial.println(F("CONFIG LOAD SUCCESSFUL."));
  }
}

void loadDefaultConfig() {
#if defined(__AVR_ATmega328P__)
  memcpy_P(&artnet.nodeConfig, &defaultConfig, sizeof(artnet.nodeConfig) );
#else
  memcpy(&artnet.nodeConfig, &defaultConfig, sizeof(artnet.nodeConfig) );
#endif
}


int writeConfigEEPROM(NODE_CONFIGURATION_T* config, size_t len ) {
  uint8_t* data = (uint8_t*)config;
  uint16_t i = 0;
  uint16_t eeprom_Address = EEPROM_START_ADDRESS;

  for (i = 0; i < len; i++) {
    EEPROM.write(eeprom_Address++, *data++);
  }
  return i;
}

int readConfigEEPROM(NODE_CONFIGURATION_T* config, size_t len) {
  uint8_t* data = (uint8_t*)config;
  uint16_t i = 0;
  uint16_t eeprom_Address = EEPROM_START_ADDRESS;

  for (i = 0; i < len; i++) {
    *data++ = EEPROM.read(eeprom_Address++);
  }
  return i;
}
