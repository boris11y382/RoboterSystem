#ifndef __ESPWIFI__
#define __ESPWIFI__

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>


class ESPWIFI {
  private: 
		   unsigned int udpPort;
  public:
          ESPWIFI (unsigned int udpPort);
		  void initConsole(int serialNumber);
          String getWifiList();
          bool tryConnect(char ssid[], char key[]);
		  String getWifiData();
		  void UDPStarten();
		  String UDPReceiveData();
		  void UDPSendData(String data);
		  
		  /*bool connectToDB(IPAddress server_addr, int server_port, char user[], char password[]);
		  void sendDataToDB(String table_name, String dataType, String data);
		  int sendDataWithPhp(String accxyz, String gyroxyz);
		  String receiveData(char host[], int httpPort);
		  void closeConnection();*/
};

#endif
