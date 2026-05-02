#include "ArtnetLayer.h"

void ArtnetLayer::begin() {
  restartNetwork();
  sendArtPollReply();
}

int32_t ArtnetLayer::read(uint16_t packetSize) {
  if (packetSize < 10) return 0;

  for (byte i = 0 ; i < 8 ; i++) {
    if (artnetPacket[i] != ART_NET_ID[i]) return 0;
  }

  uint16_t opcode = artnetPacket[9] << 8 | artnetPacket[8];

  switch (opcode) {
    case ART_OpDmx:
      handleArtDmx(artnetPacket, packetSize);
      break;
    case ART_OpPoll:
      handleArtPoll(artnetPacket);
      break;
    case ART_OpIpProg:
      handleArtIpProg(artnetPacket);
      break;
    case ART_OpAddress:
      handleArtAddress(artnetPacket);
      break;
    default:
      return -1;
  }

  return opcode;
}

void ArtnetLayer::handleArtDmx(uint8_t data[ART_NET_BUFFER_SIZE], uint16_t packetSize) {
  ART_DMX_T* packetData = (ART_DMX_T*)data;

  uint16_t len = (((uint16_t)packetData->lengthH << 8) | packetData->lengthL);

  if (len > 512 || packetSize < ART_DMX_DATA_OFFSET + len) return;

  if (packetData->universe.universe16 == nodeConfig.select_universe.universe16) {
    if (artDmxCallback) {
      (*artDmxCallback)(packetData->data, len, packetData->sequence);
    }
  }
}

void ArtnetLayer::handleArtPoll(uint8_t artnetPacket[ART_NET_BUFFER_SIZE]) {
  sendArtPollReply();
}

void ArtnetLayer::handleArtIpProg(uint8_t data[ART_NET_BUFFER_SIZE]) {
  ART_PROG_IP_T* artnetPacket = (ART_PROG_IP_T*)data;

  uint8_t restartEthernet = 0;
  if (artnetPacket->command.prog_eabled) {
    if (artnetPacket->command.prog_subnet) {
      memcpy(nodeConfig.subnet, artnetPacket->subnetmask, 4);
      restartEthernet = 1;
    }
    if (artnetPacket->command.prog_ip) {
      memcpy(nodeConfig.ip, artnetPacket->ip, 4);
      restartEthernet = 1;
    }

    if (artnetPacket->command.prog_default) {
      ARTNET_MEMCPY_FROM_PROGMEM(&nodeConfig.ip, &defaultConfig.ip, 4);
      ARTNET_MEMCPY_FROM_PROGMEM(&nodeConfig.subnet, &defaultConfig.subnet, 4);
      nodeConfig.dhcpEnable = defaultConfig.dhcpEnable;
      restartEthernet = 1;
    }

    if (restartEthernet) restartNetwork();
  }

  sendArtProgIpReply();
  configChanged();
}

void ArtnetLayer::handleArtAddress(uint8_t data[ART_NET_BUFFER_SIZE]) {
  ART_ADDRESS_T* artnetPacket =  (ART_ADDRESS_T*)data;

  artnetPacket->netSwitch &=  0x7F;
  for (int i = 0; i < 4; i++) {
    artnetPacket->swIn[i] &= 0x7F;
    artnetPacket->swOut[i] &= 0x7F;
  }

  if (artnetPacket->netSwitch != 0x7F)
    nodeConfig.select_universe.net = artnetPacket->netSwitch;

  if (artnetPacket->subSwitch != 0x7F)
    nodeConfig.select_universe.subnet = artnetPacket->subSwitch;

  if (artnetPacket->swIn[0] != 0x7F)
    nodeConfig.select_universe.universe = artnetPacket->swIn[0];

  size_t len = strnlen((char*)artnetPacket->shortName, ART_SHORT_NAME_LENGTH);
  if (len > 0) {
    memcpy(nodeConfig.shortName, artnetPacket->shortName, len);
    nodeConfig.shortName[len] = '\0';
  }

  len = strnlen((char*)artnetPacket->longName, ART_LONG_NAME_LENGTH);
  if (len > 0) {
    memcpy(nodeConfig.longName, artnetPacket->longName, len);
    nodeConfig.longName[len] = '\0';
  }

  sendArtPollReply();
  configChanged();
}

uint32_t ArtnetLayer::calculateCRC(uint8_t* data, size_t len) {
  uint32_t crc = ~0L;

  for (size_t index = 0 ; index < len ; ++index) {
    crc = crc_table[(crc ^ data[index]) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (data[index] >> 4)) & 0x0f] ^ (crc >> 4);
  }

  return ~crc;
}
