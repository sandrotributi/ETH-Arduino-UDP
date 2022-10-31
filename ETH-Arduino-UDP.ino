#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <DHT.h>
// modello sensore di temperatura
#define DHTTYPE DHT11 
// assegna un indirizzo MAC
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
// buffer per l'elaborazione del pacchetto UDP
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
String datReq;  // stringa contenente i dati
int packetSize; // dimensione pacchetto UDP

// assegnazione dei pin
const byte dhtPin = 2;
const byte pinRed = 7;
const byte pinGreen = 8;
const byte pinBlue = 9;
// stato dei led (on/off)
byte statusRed = LOW;
byte statusGreen = LOW;
byte statusBlue = LOW;
// porta UDP locale
unsigned int localPort = 5000;
// dichiarazione di un oggetto UDP
EthernetUDP udp;
// dichiarazione oggetto dht (sensore temperatura)
DHT dht(dhtPin, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(pinRed, OUTPUT);
  pinMode(pinGreen, OUTPUT);
  pinMode(pinBlue, OUTPUT);

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
  // inizializzazione sensore temperatura
  dht.begin();  
  // inizializzazione UDP
  udp.begin(localPort);
  delay(1500);
}

void loop() {
  // lettura dimensione pacchetto UDP
  packetSize = udp.parsePacket();
  if (packetSize > 0) { // controllo richiesta UDP
    udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE); // lettura dati UDP
    String datReq(packetBuffer);  // converte array di byte in stringa
    Serial.println(datReq);

    // elaborazione dei comandi
    if (datReq == "red") {
      sendUdpData(datReq);
      statusRed = ledChangeStatus(pinRed, statusRed);
    }
    if (datReq == "green") {
      sendUdpData(datReq);
      statusGreen = ledChangeStatus(pinGreen, statusGreen);
    }
    if (datReq == "blue") {
      sendUdpData(datReq);
      statusBlue = ledChangeStatus(pinBlue, statusBlue);
    }
    if (datReq == "off") {
      sendUdpData(datReq);
      allLedOnOff(LOW);
      statusRed = statusGreen = statusBlue = LOW;
    }
    if (datReq == "on") {
      sendUdpData(datReq);
      allLedOnOff(HIGH);
      statusRed = statusGreen = statusBlue = HIGH;      
    }
    if (datReq == "temp") {
      float t = dht.readTemperature();
      String strtmp = String(t, 2);
      sendUdpData(strtmp);
    }
    if (datReq == "hum") {
      float h = dht.readHumidity();
      String strtmp = String(h, 2);
      sendUdpData(strtmp);
    }
  }
  memset(packetBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
}

void sendUdpData(String str) {
  udp.beginPacket(udp.remoteIP(), udp.remotePort());
  udp.print(str);
  udp.endPacket();  
}

void allLedOnOff(byte status) {
  digitalWrite(pinRed, status);
  digitalWrite(pinGreen, status);
  digitalWrite(pinBlue, status);
}

byte ledChangeStatus(byte pin, byte status) {
  status = !status;
  digitalWrite(pin, status);
  return status;
}