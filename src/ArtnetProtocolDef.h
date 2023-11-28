#ifndef __ARTNET_PROTOCOL_DEF_H__
#define __ARTNET_PROTOCOL_DEF_H__


// UDP specific
//#define ART_NET_PORT 0x1936
#define ART_NET_PORT              6454
#define ART_NET_BUFFER_SIZE       532   //512DMX + 18ARTNET_HEADER + 10SPARE

// Packet
#define ART_NET_ID                "Art-Net\0"
#define ART_DMX_DATA_OFFSET       18
#define ART_MAX_PORTS             4
#define ART_REPORT_LENGTH         64
#define ART_SHORT_NAME_LENGTH     18
#define ART_LONG_NAME_LENGTH      64


// Opcodes
#define ART_OpPoll              0x2000
#define ART_OpPollReply         0x2100
#define ART_OpDiagData          0x2300
#define ART_OpCommand           0x2400
#define ART_OpOutput            0x5000
#define ART_OpDmx               0x5000
#define ART_OpNzs               0x5100
#define ART_OpSync              0x5200
#define ART_OpAddress           0x6000
#define ART_OpInput             0x7000
#define ART_OpTodRequest        0x8000
#define ART_OpTodData           0x8100
#define ART_OpTodControl        0x8200
#define ART_OpRdm               0x8300
#define ART_OpRdmSub            0x8400
#define ART_OpVideoSetup        0xa010
#define ART_OpVideoPalette      0xa020
#define ART_OpVideoData         0xa040
#define ART_OpMacMaster         0xf000
#define ART_OpMacSlave          0xf100
#define ART_OpFirmwareMaster    0xf200
#define ART_OpFirmwareReply     0xf300
#define ART_OpFileTnMaster      0xf400
#define ART_OpFileFnMaster      0xf500
#define ART_OpFileFnReply       0xf600
#define ART_OpIpProg            0xf800
#define ART_OpIpProgReply       0xf900
#define ART_OpMedia             0x9000
#define ART_OpMediaPatch        0x9100
#define ART_OpMediaControl      0x9200
#define ART_OpMediaContrlReply  0x9300
#define ART_OpTimeCode          0x9700
#define ART_OpTimeSync          0x9800
#define ART_OpTrigger           0x9900
#define ART_OpDirectory         0x9a00
#define ART_OpDirectoryReply    0x9b00


#define ART_POLL_REPLY_PORTTYPE_DMX512     0b00000000
#define ART_POLL_REPLY_PORTTYPE_MIDI       0b00000001
#define ART_POLL_REPLY_PORTTYPE_AVAB       0b00000010
#define ART_POLL_REPLY_PORTTYPE_COLOTRAN   0b00000011
#define ART_POLL_REPLY_PORTTYPE_ADB62_5    0b00000100
#define ART_POLL_REPLY_PORTTYPE_ART_NET    0b00000101
#define ART_POLL_REPLY_PORTTYPE_INPUT      0b01000000
#define ART_POLL_REPLY_PORTTYPE_OUTPUT     0b10000000

#define ART_POLL_REPLY_GOODINPUT_RECEIVE_ERROR            0x04
#define ART_POLL_REPLY_GOODINPUT_INPUT_DISABLED           0x08
#define ART_POLL_REPLY_GOODINPUT_DMX512_TEXT              0x10
#define ART_POLL_REPLY_GOODINPUT_DMX512_SIP               0x20
#define ART_POLL_REPLY_GOODINPUT_DMX512_TEST              0x40
#define ART_POLL_REPLY_GOODINPUT_DATA_RECEIVED            0x80

#define ART_POLL_REPLY_STATUS1_UBEA_PRESENT               0x01
#define ART_POLL_REPLY_STATUS1_RDM_CAPABLE                0x02
#define ART_POLL_REPLY_STATUS1_BOOTED_FROM_ROM            0x04
#define ART_POLL_REPLY_STATUS1_BOOTED_NORMAL              0x00
#define ART_POLL_REPLY_STATUS1_PORT_ADR_UNKNOWN           0x00
#define ART_POLL_REPLY_STATUS1_PORT_ADR_FRONT_PANEL       0x10
#define ART_POLL_REPLY_STATUS1_PORT_ADR_WEB_NET           0x20
#define ART_POLL_REPLY_STATUS1_INDICATOR_STATE_UNKNOWN    0x00
#define ART_POLL_REPLY_STATUS1_INDICATOR_STATE_LOCATE     0x40
#define ART_POLL_REPLY_STATUS1_INDICATOR_STATE_MUTE       0x80
#define ART_POLL_REPLY_STATUS1_INDICATOR_STATE_NORMAL     0xC0

#define ART_POLL_REPLY_STATUS2_WEB_CONFIG                 0x01
#define ART_POLL_REPLY_STATUS2_DHCP_ENABLED               0x02
#define ART_POLL_REPLY_STATUS2_DHCP_CAPABLE               0x04
#define ART_POLL_REPLY_STATUS2_15BIT_ADDRESS_SUPPORT      0x08
#define ART_POLL_REPLY_STATUS2_SWITCH_TO_sCAN             0x10
#define ART_POLL_REPLY_STATUS2_SQUAWKING                  0x20

#define ART_POLL_REPLY_STYLE_CODE_ST_NODE                 0x00
#define ART_POLL_REPLY_STYLE_CODE_ST_CONTROLLER           0x01
#define ART_POLL_REPLY_STYLE_CODE_ST_MEDIA                0x02
#define ART_POLL_REPLY_STYLE_CODE_ST_ROUTE                0x03
#define ART_POLL_REPLY_STYLE_CODE_ST_BACKUP               0x04
#define ART_POLL_REPLY_STYLE_CODE_ST_CONFIG               0x05
#define ART_POLL_REPLY_STYLE_CODE_ST_VISUAL               0x06

#define ART_PROG_IP_REPLY_STATUS_DHCP_ENABLE              0x40


#define ART_ADDRESS_COMMAND_ACNONE                        0x00
#define ART_ADDRESS_COMMAND_ACCANCEL MERGE                0x01
#define ART_ADDRESS_COMMAND_ACLEDNORMAL                   0x02
#define ART_ADDRESS_COMMAND_ACLEDMUTE                     0x03
#define ART_ADDRESS_COMMAND_ACLEDLOCATE                   0x04
#define ART_ADDRESS_COMMAND_ACRESETRX FLAGS               0x05
#define ART_ADDRESS_COMMAND_ACMERGELTP0                   0x10
#define ART_ADDRESS_COMMAND_ACMERGELTP1                   0x11
#define ART_ADDRESS_COMMAND_ACMERGELTP2                   0x12
#define ART_ADDRESS_COMMAND_ACMERGELTP3                   0x13
#define ART_ADDRESS_COMMAND_ACMERGEHTP0                   0x50
#define ART_ADDRESS_COMMAND_ACMERGEHTP1                   0x51
#define ART_ADDRESS_COMMAND_ACMERGEHTP2                   0x52
#define ART_ADDRESS_COMMAND_ACMERGEHTP3                   0x53
#define ART_ADDRESS_COMMAND_ACARTNETSEL0                  0x60
#define ART_ADDRESS_COMMAND_ACARTNETSEL1                  0x61
#define ART_ADDRESS_COMMAND_ACARTNETSEL2                  0x62
#define ART_ADDRESS_COMMAND_ACARTNETSEL3                  0x63
#define ART_ADDRESS_COMMAND_ACACNSEL0                     0x70
#define ART_ADDRESS_COMMAND_ACACNSEL1                     0x71
#define ART_ADDRESS_COMMAND_ACACNSEL2                     0x72
#define ART_ADDRESS_COMMAND_ACACNSEL3                     0x73
#define ART_ADDRESS_COMMAND_ACCLEAROP0                    0x90
#define ART_ADDRESS_COMMAND_ACCLEAROP1                    0x91
#define ART_ADDRESS_COMMAND_ACCLEAROP2                    0x92
#define ART_ADDRESS_COMMAND_ACCLEAROP3                    0x93

#ifndef ARDUINO_AVR_ADK
  #pragma pack(push,1)
  #pragma pack(1) 
#endif

typedef union {
  uint16_t universe16;
  struct {
    uint8_t universe: 4;
    uint8_t subnet: 4;
    uint8_t net;
  } ;
} ART_NET_UNIVERSE_T;

typedef struct  {
  uint8_t   id[8];
  uint16_t  opcode;
  uint16_t  ver;
  uint8_t   sequence;
  uint8_t   physical;
  ART_NET_UNIVERSE_T universe;
  uint8_t  lengthH;
  uint8_t  lengthL;
  uint8_t   data[512];
} ART_DMX_T;

typedef struct  {
  uint8_t   id[8];      //0-7
  uint16_t  opcode;     //8-9
  uint16_t  ver;        //10-11
  uint8_t   talkToMe;   //12
  uint8_t   priority;   //13
} ART_POLL_T;



typedef struct  {
  uint8_t  id[8];
  uint16_t opcode;
  byte  ip[4];
  uint16_t port;
  uint16_t ver;
  uint8_t  net;
  uint8_t  sub;
  uint8_t  oemH;
  uint8_t  oemL;
  uint8_t  ubea;
  uint8_t  status;
  uint8_t  etsaManCodeL;
  uint8_t  etsaManCodeH;
  uint8_t  shortname[ART_SHORT_NAME_LENGTH];
  uint8_t  longname[ART_LONG_NAME_LENGTH];
  uint8_t  nodereport[ART_REPORT_LENGTH];
  uint8_t  numbportsH;
  uint8_t  numbportsL;
  uint8_t  porttypes[ART_MAX_PORTS];
  uint8_t  goodinput[ART_MAX_PORTS];
  uint8_t  goodoutput[ART_MAX_PORTS];
  uint8_t  swin[ART_MAX_PORTS];
  uint8_t  swout[ART_MAX_PORTS];
  uint8_t  swvideo;
  uint8_t  swmacro;
  uint8_t  swremote;
  uint8_t  sp1;
  uint8_t  sp2;
  uint8_t  sp3;
  uint8_t  style;
  uint8_t  mac[6];
  uint8_t  bindIp[4];
  uint8_t  bindIndex;
  uint8_t  status2;
  uint8_t  filler[32];
} ART_POLL_REPLY_T;



typedef union {
  struct {
    uint8_t prog_port: 1;
    uint8_t prog_subnet: 1;
    uint8_t prog_ip: 1;
    uint8_t prog_default: 1;
    uint8_t prog_unused: 2;
    uint8_t prog_dhcp_enabled: 1;
    uint8_t prog_eabled: 1;
  };
  uint8_t command;
} PROG_IP_COMMAND_T;


typedef struct  {
  uint8_t  id[8];               // 0-7
  uint16_t opcode;              // 8-9
  uint16_t versioin;            // 10-11
  uint8_t filler1;              // 12
  uint8_t filler2;              // 13
  PROG_IP_COMMAND_T  command;   // 14
  uint8_t  filler4;             // 15
  uint8_t  ip[4];                 // 16-19
  uint8_t  subnetmask[4];         // 20-23
  uint8_t  progPortHi;          // 24
  uint8_t  progPort;            // 25
  uint8_t  spare[8];            // 26-33
} ART_PROG_IP_T;


typedef struct  {
  uint8_t   id[8];
  uint16_t  opcode;
  uint8_t  versionH;
  uint8_t  versionL;
  uint8_t   filler1;
  uint8_t   filler2;
  uint8_t   filler3;
  uint8_t   filler4;
  uint8_t   ip[4];
  uint8_t   subnetmask[4];
  uint8_t   progPortHi;
  uint8_t   progPort;
  uint8_t   status;
  uint8_t  spare[7];
} ART_PROG_IP_REPLY_T;


typedef struct {
  uint8_t   id[8];
  uint16_t  opcode;
  uint8_t   versionH;
  uint8_t   versionL;
  uint8_t   netSwitch;
  uint8_t   bindIndex;
  uint8_t   shortName[18];
  uint8_t   longName[64];
  uint8_t   swIn[4];
  uint8_t   swOut[4];
  uint8_t   subSwitch;
  uint8_t   swVideo;
  uint8_t   command;
} ART_ADDRESS_T;

#ifndef ARDUINO_AVR_ADK
#pragma pack(pop)
#endif


#endif
