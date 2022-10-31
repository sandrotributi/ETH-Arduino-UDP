#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

unsigned int localPort = 5000;
// dichiarazione di un oggetto UDP
EthernetUDP udp;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // attendere la connessione della porta seriale. Necessario solo per la porta USB nativa.
  }
  Ethernet.init(10);
  // inizializazione Ethernet
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Impossibile configurare Ethernet tramite DHCP");
    // verifica la presenza di hardware Ethernet
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("La shield Ethernet non è stata trovata. Spiacenti, impossibile eseguire senza hardware. :(");
    } else if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Cavo Ethernet non connesso.");
    }
    // non ha senso andare avanti, quindi non fare nulla per sempre:
    while (true) {
      delay(1);
    }
  }

  // visualizza indirizzo MAC
  byte macBuffer[6];
  Ethernet.MACAddress(macBuffer); 
  Serial.print("MAC: ");
  for (byte octet = 0; octet < 6; octet++) {
    Serial.print(macBuffer[octet], HEX);
    if (octet < 5) {
      Serial.print(':');
    }
  }
  Serial.println();
  // visualizza indirizzo IP
  Serial.print("IP:  ");
  Serial.println(Ethernet.localIP());
  // visualizza subnet mask 
  Serial.print("SM:  ");
  Serial.println(Ethernet.subnetMask());
  // visualizza IP gateway 
  Serial.print("GW:  ");
  Serial.println(Ethernet.gatewayIP());
  // visualizza IP DNS server
  Serial.print("DNS: ");
  Serial.println(Ethernet.dnsServerIP());
  // inizializzazione UDP
  udp.begin(localPort);
  delay(1500);
}

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
// String datReq;
int packetSize;

const byte pinRed = 2;
const byte pinGreen = 3;
const byte pinBlue = 4;

byte statusRed = LOW;
byte statusGreen = LOW;
byte statusBlue = LOW;

void loop() {
  // lettura dimensione pacchetto UDP
  packetSize = udp.parsePacket();
  if (packetSize > 0) { // controllo richiesta UDP
    udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); // lettura dati UDP
    String datReq(packetBuffer);

    if (datReq == "red") {
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.print("E' stato richiesto: red");
      udp.endPacket();
    }
    if (datReq == "green") {
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.print("E' stato richiesto: green");
      udp.endPacket();
    }
    if (datReq == "blue") {
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.print("E' stato richiesto: blue");
      udp.endPacket();
    }
  }
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
}
