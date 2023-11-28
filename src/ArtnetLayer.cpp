#include "ArtnetLayer.h"

void ArtnetLayer::begin() {
  restartNetwork();
  sendArtPollReply();
}

int32_t ArtnetLayer::read(uint16_t packetSize) {
  uint16_t opcode = 0;
  //uint16_t protoVersion = 0;

  // Has a valid size
  if (packetSize > 0)
  {
    // Check "Art-Net" String
    for (byte i = 0 ; i < 9 ; i++)
    {
      if (artnetPacket[i] != ART_NET_ID[i]) {
        return 0;
      }
    }
    opcode = artnetPacket[9] << 8 | artnetPacket[8];
    //protoVersion = artnetPacket[10] | artnetPacket[11] << 8;
    switch (opcode) {
      case ART_OpDmx:
        handleArtDmx(artnetPacket);
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
  }
  return opcode;
}

void ArtnetLayer::handleArtDmx(uint8_t data[ART_NET_BUFFER_SIZE]) {

  //TODO handle sequence numbers

  ART_DMX_T* packetData = (ART_DMX_T*)data;

  if ( packetData->universe.universe16 == nodeConfig.select_universe.universe16) {
    if (artDmxCallback) {
		uint16_t len = (((uint16_t)packetData->lengthH	<< 8) | packetData->lengthL);
      (*artDmxCallback)(packetData->data, len, packetData->sequence);
    }
  }

}

void ArtnetLayer::handleArtPoll(uint8_t artnetPacket[ART_NET_BUFFER_SIZE]) {
  //NOT Used for now
  //ART_POLL_T* packetData = (ART_POLL_T*)artnetPacket;
  sendArtPollReply();
}

void ArtnetLayer::handleArtIpProg(uint8_t data[ART_NET_BUFFER_SIZE]) {

  Serial.println("Network Config received");
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
    if (artnetPacket->command.prog_dhcp_enabled) {
      nodeConfig.dhcpEnable = 1;
      restartEthernet = 1;
    } else  {
      nodeConfig.dhcpEnable = 0;
      restartEthernet = 1;
    }
    if (artnetPacket->command.prog_default) {
      memcpy_P(&nodeConfig.ip, &defaultConfig.ip, 4);
      memcpy_P(&nodeConfig.subnet, &defaultConfig.subnet, 4);
      nodeConfig.dhcpEnable = defaultConfig.dhcpEnable;
      restartEthernet = 1;
    }


    if (restartEthernet == 1) {
      restartNetwork();
    }
  }
  sendArtProgIpReply();
  configChanged();
}

void ArtnetLayer::handleArtAddress(uint8_t data[ART_NET_BUFFER_SIZE]) {

  ART_ADDRESS_T* artnetPacket =  (ART_ADDRESS_T*)data;
  //Remove the added 0x80 from this bytes
  artnetPacket->netSwitch &=  0x7F;
  for (int i = 0; i < 4; i++) {
    artnetPacket->swIn[i]   &= 0x7F;
    artnetPacket->swOut[i]  &= 0x7F;
  }

  if (artnetPacket->netSwitch != 0x7F) {
    nodeConfig.select_universe.net       =  artnetPacket->netSwitch;
  }
  if (artnetPacket->subSwitch != 0x7F) {
    nodeConfig.select_universe.subnet    =  artnetPacket->subSwitch;
  }
  if (artnetPacket->swIn[0] != 0x7F) {
    nodeConfig.select_universe.universe    =  artnetPacket->swIn[0];
  }

  if (strlen((char*)(artnetPacket->shortName)) > 0) {
    strcpy(nodeConfig.shortName, (char*)artnetPacket->shortName );
  }

  if (strlen((char*)(artnetPacket->longName)) > 0) {
    strcpy(nodeConfig.longName, (char*)artnetPacket->longName );
  }
  sendArtPollReply();
  configChanged();
}

void ArtnetLayer::sendArtPollReply() {
  pollCounter++;
  ART_POLL_REPLY_T reply;
  memset(&reply, 0, sizeof(reply));

  memcpy(reply.id, ART_NET_ID, 8);
  reply.opcode    = ART_OpPollReply;
  reply.port      = ART_NET_PORT;
  reply.ver       = ART_FIRMWARE_VERSION;
  reply.ubea      = 0;       //NO UBEA Support
  reply.status    = ART_POLL_REPLY_STATUS1_INDICATOR_STATE_NORMAL
                    | ART_POLL_REPLY_STATUS1_PORT_ADR_WEB_NET
                    | ART_POLL_REPLY_STATUS1_BOOTED_NORMAL;   //Set to All Port-Address set by front panel controls. Normal firmware boot (from flash).Not capable of Remote Device Management (RDM). UBEA present
  reply.style     = ART_POLL_REPLY_STYLE_CODE_ST_VISUAL;

  reply.goodinput[0] = ART_POLL_REPLY_GOODINPUT_DATA_RECEIVED;    //TODO Change status dependend
  reply.etsaManCodeH = 0x7E;   // TODO
  reply.etsaManCodeL = 0xE7;   // ESTA Prototyping CODE

  sprintf ((char*)reply.nodereport, "%d[%d] OK", 0x0001, pollCounter);
  reply.oemH       = 0x7F;   //TODO
  reply.oemL       = 0xFF;   //TODO


  memcpy(reply.ip,  nodeConfig.ip, 4);
  memcpy(reply.mac, nodeConfig.mac, 6);
  strcpy((char*)reply.shortname, nodeConfig.shortName );
  strcpy((char*)reply.longname, nodeConfig.longName );

  reply.numbportsH   = 0;          //One Channel
  reply.numbportsL   = 1;          //One Channel
  reply.porttypes[0] = ART_POLL_REPLY_PORTTYPE_INPUT | ART_POLL_REPLY_PORTTYPE_DMX512;    //Channel 0 ArtNet Input
  reply.net          = nodeConfig.select_universe.net;
  reply.sub          = nodeConfig.select_universe.subnet;
  reply.swin[0]      = nodeConfig.select_universe.universe;

  if (nodeConfig.dhcpEnable) {
    reply.status2   = ART_POLL_REPLY_STATUS2_DHCP_CAPABLE
                      | ART_POLL_REPLY_STATUS2_DHCP_ENABLED
                      | ART_POLL_REPLY_STATUS2_15BIT_ADDRESS_SUPPORT;
  } else {
    reply.status2   = ART_POLL_REPLY_STATUS2_DHCP_CAPABLE
                      | ART_POLL_REPLY_STATUS2_15BIT_ADDRESS_SUPPORT;
  }

  //reply.swremote = 0;
  //reply.bindIp = {0,0,0,0};
  //reply.bindIndex = 0;

  //Send Packet
  sendUdp(broadcastAdr, ART_NET_PORT, (uint8_t *)&reply, sizeof(reply),1);
}

void ArtnetLayer::sendArtProgIpReply() {
  ART_PROG_IP_REPLY_T reply;
  memset(&reply, 0, sizeof(reply));

  memcpy(reply.id, ART_NET_ID, 8);
  reply.opcode    = ART_OpIpProgReply;
  reply.versionL  = 14;

  reply.progPortHi = 0x19;
  reply.progPort = 0x36;

  memcpy(reply.ip, nodeConfig.ip, 4);
  memcpy(reply.subnetmask, nodeConfig.subnet, 4);

  if (nodeConfig.dhcpEnable) {
    reply.status = ART_PROG_IP_REPLY_STATUS_DHCP_ENABLE;
  }
 
  sendUdp(broadcastAdr, ART_NET_PORT, (uint8_t *)&reply, sizeof(reply),1);
}

void ArtnetLayer::sendUdp(uint8_t ip[4], uint16_t port,uint8_t* packetData, size_t len, uint8_t broadcast) {
  if (updSendCallback) {
    (*updSendCallback)(ip, port, packetData, len, broadcast);
  }
}

void ArtnetLayer::restartNetwork() {
  if (networkRestartCallback) {
    (*networkRestartCallback)();
  }
}

void ArtnetLayer::configChanged() {
  updateConfigCrc();
  if (configChangedCallback) {
    (*configChangedCallback)(&nodeConfig);
  }
}

uint32_t ArtnetLayer::calculateCRC(uint8_t* data, size_t len) {

  uint32_t crc = ~0L;

  for (uint16_t index = 0 ; index < len  ; ++index) {
    crc = crc_table[(crc ^ data[index]) & 0x0f] ^ (crc >> 4);
    crc = crc_table[(crc ^ (data[index] >> 4)) & 0x0f] ^ (crc >> 4);
    crc = ~crc;
  }
  return crc;
}

void ArtnetLayer::updateConfigCrc(){
  nodeConfig.crc32 = 0;
  nodeConfig.crc32 = calculateCRC((uint8_t*)&nodeConfig, sizeof(NODE_CONFIGURATION_T) - 4 ); //excluding the CRC field
}

uint8_t ArtnetLayer::isConfigCrcValid(){
  uint32_t crc_value = calculateCRC((uint8_t*)&nodeConfig, sizeof(NODE_CONFIGURATION_T) - 4 ); //excluding the CRC field
  return (crc_value == nodeConfig.crc32);
}

