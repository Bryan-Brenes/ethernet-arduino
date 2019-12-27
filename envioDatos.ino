
#include <EtherCard.h>

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[700];
static uint32_t timer;
Stash stash;
int dato = 0;
const char* datoinfo = "";
String datoStr = "";

const char website[] PROGMEM = "php-store-data-ethernet.herokuapp.com";

// called when the client request is complete
static void my_callback (byte status, word off, word len) {
  Serial.println(">>>");
  Ethernet::buffer[off+300] = 0;
  Serial.print((const char*) Ethernet::buffer + off);
  Serial.println("...");
}

void setup () {
  Serial.begin(57600);
  Serial.println("\n[prueba API]");
  
  if (ether.begin(sizeof Ethernet::buffer, mymac, 8) == 0){
    Serial.println(F("Failed to access Ethernet controller"));
  } else {
    Serial.println(F("Access to ethernet controller OK..."));  
  }
  if (!ether.dhcpSetup()){
    Serial.println("DHCP failed");
  } else {
    Serial.println("DHCP OK");
  }

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip); 
  ether.printIp("DNS: ", ether.dnsip); 

  if (!ether.dnsLookup(website)){
    Serial.println("DNS failed");
  } else {
    Serial.println("DNS OK");
  }
    
  ether.printIp("SRV: ", ether.hisip);
}

void loop () {

  ether.packetLoop(ether.packetReceive());

  
  
  if (millis() > timer) {

    timer = millis() + 5000;

    dato = random(100);
    Serial.println(dato);
    datoStr = String(dato);
    Serial.println(datoStr);
    datoinfo = datoStr.c_str();
    //strDato = "/php/save.php?dato=" + dato;
    
    Serial.println();
    Serial.print("<<< REQ ");
    //ether.browseUrl(strDato, "", website, my_callback);
    ether.browseUrl(PSTR("/php/save.php?dato="), datoinfo, website, my_callback);

    //ether.browseUrl(PSTR("/dato/"), "bar", website, my_callback);
  }
}