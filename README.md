# Arduino library to implement Artnet UPD Protocol to Arduino and ESP

This library allows you to remote control LED's via ArtNet Protocol.

## Supported micro-controllers

* ATmega 328 & 1284p (slow)
* Arduino MKR Zero
* boards supported by ESP8266 and ESP32 Arduino boards package  

## Supported networking libraries
* mostly soll Ethernet and WiFi Libs
* Tested: Ethernet library - Ethernet shields and modules with Wiznet 5100, 5200 and 5500 chips
* Tested: WiFi library of ESP8266 and ESP32 Arduino boards package


## Usage / Implementation
This Lib implements the whole ArtNet protocol handling but not the Ethernet /WiFi handling.
So a few Software connections must be implemented to run the ArtNet protocol on your microcontroller.
A working ethernet / wifi upd library is required
Make a new instance with
```
ArtnetLayer artnet(onArtUpdSend, onNetworkRestart);
```

In setup() set the call backfunctions:
```
  artnet.setConfigChangedCallback(onConfigChange);
  artnet.setArtDmxCallback(onArtDmxFrame);
  artnet.begin();
```

In loop() read the incoming UDP Packages and forward to ArtNet layer
```
void loop() {
  int packetSize = Udp.parsePacket();
  if (packetSize > 0 && packetSize <= ART_NET_BUFFER_SIZE) {
    Udp.read(artnet.getPacketBuffer(), ART_NET_BUFFER_SIZE);
    artnet.read(packetSize);
  }

}
```

### implement callback fuctions
```
void onArtUpdSend(uint8_t ip[4], uint16_t port, uint8_t* packetData, size_t len, uint8_t broadcast);
```
Will be called from the ArtNet Stack if a UDP Packet must be send. Make sure the paket will be send asap.
Handle UPD Broadcast properly!

```
void onNetworkRestart();
```
Restart the Network Stack (mostly with Upd.Stop() ). And restart the UDP Stack
Make sure the IP Settings / DHCP Settings from ArtNet Confit are used correctly. 
* artnet.nodeConfig.dhcpEnable 
* artnet.nodeConfig.ip 
* artnet.nodeConfig.subnet


```
void onConfigChange(NODE_CONFIGURATION_T* config);
```
make sure the new configuration is stored properly if needed. e.g. in an EEPROM


```
void onArtDmxFrame(uint8_t* data, uint16_t length, uint8_t sequence);
```
handle the DMX frame proberly and update your leds if needed




## Contribution
Please report tested boards and feel free to add Pull Requests
