#include "ESPWIFI.h"

WiFiClient client;
//MySQL_Connection conn((Client *)&client);
WiFiUDP Udp;


ESPWIFI::ESPWIFI (unsigned int udpPort) {
	this->udpPort = udpPort;
}

void ESPWIFI::initConsole(int serialNumber) {
	Serial.begin(serialNumber);
}

String ESPWIFI::getWifiList() {
	Serial.println("** Scan Networks **");
	int n = WiFi.scanNetworks();
	Serial.println("Erreich");
	Serial.print("SSID List:");
	Serial.println(n);
	String list = "";
	for (int i  = 0; i < n; i++) {
		list += WiFi.SSID(i) + " - " + WiFi.RSSI(i) + "\n";
	}
	
	return list;
}

bool ESPWIFI::tryConnect(char ssid[], char key[]) {
	Serial.println("Connection To: ");
	Serial.print(" SSID: ");
	Serial.print(ssid);
	//Serial.print(" Key: ");
	//Serial.println(key);
	WiFi.begin(ssid, key);
	while(WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	if (WiFi.status() != WL_CONNECTED)
		return false;
	
	//Serial.print("ESP Connected to: ");
	//Serial.println(ssid);
	return true;
}

String ESPWIFI::getWifiData() {
	bool status = (WiFi.status() == WL_CONNECTED);
	String wifiData = "Wifi: ";
	wifiData += status ? "Connected " : "Disconnected ";
	
	if (status) {
		wifiData += WiFi.localIP().toString().c_str();
	}
		
	return wifiData;
}

void ESPWIFI::UDPStarten()
{
	Udp.begin(udpPort);
	//Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), udpPort);
}

String ESPWIFI::UDPReceiveData()
{
	String message = "";
	char data[255];  // buffer for incoming packets
	int packetSize = Udp.parsePacket();
    if (packetSize)
    {
	   // receive incoming UDP packets
	   //Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
	   int len = Udp.read(data, 255);
	   if (len > 0)
	   {
	     data[len] = 0;
	   }
	   //Serial.printf("UDP packet contents: %s\n", data);
	   message += data;
	   
	   return message;
   }
}

void ESPWIFI::UDPSendData(String data)
{
	char replyPacekt[data.length() + 1];
	sprintf(replyPacekt, data.c_str());
	Udp.beginPacket(Udp.remoteIP(), 57942);
    Udp.write(replyPacekt);
    Udp.endPacket();
}

/*
bool ESPWIFI::connectToDB(IPAddress server_addr, int server_port, char user[], char password[]) {
	int counter = 0;
	while ((conn.connect(server_addr, server_port, user, password) != true) && counter < 100) {
		Serial.print(".");
		counter ++;
	}
	if (counter < 100) {
		Serial.println("");
		Serial.println("Connected to SQL Server!");  
		return true;
	}
	
	return false;
}*/

/*
void ESPWIFI::sendDataToDB(String table_name, String dataType, String data) {
	//SQL Query 
	String dataCopy = data.substring(0, data.length() - 1);
	String query_text = "INSERT INTO testdb." + table_name + " ";
	query_text += dataType;
	String values = " VALUES ";
	values += dataCopy;
	query_text += values;
	query_text += ";";
	char query[query_text.length() + 1];
	sprintf(query, query_text.c_str());
	Serial.print("Number of Column: ");
	Serial.println(sizeof(dataType));
	Serial.print("Query: ");
	Serial.println(query);
	MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
	cur_mem->execute(query);

	delete cur_mem;
}
int ESPWIFI::sendDataWithPhp(String accxyz, String gyroxyz){
	if (!client.connect(host, httpPort)) {
		Serial.println("connection failed");
		return "Error 1";
	} else {
		client.print("Post /insert?accxyz=" + accxyz  + "");
		client.print(data);
		client.println(" HTTP/1.1");
		client.println("Host: 192.168.2.83");
		client.println();
	}
	
}*/
/*
String ESPWIFI::receiveData(char host[], int httpPort) {
	String data = "";
	if (!client.connect(host, httpPort)) {
		Serial.println("connection failed");
		return "Error 1";
	}
	while(client.available()){
		String line = client.readStringUntil('\r');
		data += line;
	}
	client.stop();
	return data;
}

void ESPWIFI::closeConnection(){
	conn.close();
}*/



