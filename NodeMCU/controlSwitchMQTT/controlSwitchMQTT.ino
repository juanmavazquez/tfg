  //___________LIBRERIAS___________//

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//___________VALORES RED WIFI___________//

const char* ssid = "Nombre Red Wi-Fi";
const char* password = "Contraseña Wi-Fi";
const char* mqtt_server = "IP Raspberry";

//___________VARIABLES___________//

unsigned int switch1 = D1;
const char* topicswitch1 = "casa/switch1";

//___________SUBPROGRMAS___________//

void setup_wifi(); //Inicia conexion WiFi
void callback(String topic, byte* message, unsigned int length); //Procesamiento de los mensajes recibidos a los topics
void reconnect(); //Reconexión del dispositivo a la red 

//___________CREAR CLIENTE___________//

WiFiClient espClient;
PubSubClient client(espClient);

//___________PROGRAMA___________//

void setup() {
 
  pinMode(switch1, OUTPUT);  

  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) reconnect();
  
  if(!client.loop()) client.connect("ESP8266ClientSwitch");

}

//___________SUBPROGRAMAS___________//

void setup_wifi() {
    
  //Conexión wifi:
  
  WiFi.begin(ssid, password); 
  
  //Mientras no se realice la conexión, este bucle es ejecutado
  
  while (WiFi.status() != WL_CONNECTED) delay(500);

}

void callback(String topic, byte* message, unsigned int length) {
  
  String messageTemp;
  
  //Llega un mensaje al topic 
  
  for (int i = 0; i < length; i++) messageTemp += (char)message[i];
  
  //Ejecucion de las operaciones a realizar en función del mensaje recibido:

  if(topic == topicswitch1){
    
      if(messageTemp == "on") digitalWrite(switch1, HIGH);
      else if(messageTemp == "off") digitalWrite(switch1, LOW);
  
  }
   
}

void reconnect() {
  
  while (!client.connected()) {

    //Si el dispositivo es desconectado de la red, procede a suscribirse al topic
    
    if (client.connect("ESP8266ClientSwitch")) client.subscribe(topicswitch1);
    else delay(50);
  }
  
}
