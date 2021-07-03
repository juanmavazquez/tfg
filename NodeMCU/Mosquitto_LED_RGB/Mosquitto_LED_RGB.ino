//___________LIBRERIAS___________//

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//___________VALORES RED WIFI___________//

const char* ssid = "Nombre Red Wi-Fi";
const char* password = "Contraseña Wi-Fi";
const char* mqtt_server = "IP Raspberry";

//___________VARIABLES___________//

const int g = D4, r = D6, b = D5;
int value;

//___________SUBPROGRMAS___________//

void setup_wifi(); //Inicia conexion WiFi
void callback(String topic, byte* message, unsigned int length); //Procesamiento de los mensajes recibidos a los topics
void reconnect(); //Reconexión del dispositivo a la red 

//___________CREACION DEL CLIENTE___________//

WiFiClient espClient;
PubSubClient client(espClient);

//___________PROGRAMA___________//

void setup() {

  pinMode(r, OUTPUT);  
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);
  
  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) reconnect();
  
  if(!client.loop()) client.connect("ESP8266ClientRGB");

}

//___________SUBPROGRAMAS___________//

void setup_wifi() {
    
  //Conexión wifi:
  
  WiFi.begin(ssid, password); 
  
  //Mientras no se realice la conexión, este bucle es ejecutado
  
  while (WiFi.status() != WL_CONNECTED) delay(500);

  for(int k=0; k<10; k++){ //Parpadea el LED verde para verificar que se ha conectado a la red Wi-Fi
    digitalWrite(g,HIGH);
    delay(100);
    digitalWrite(g,LOW);
    delay(100);
  }

}

void callback(String topic, byte* message, unsigned int length) {

  String messageTemp;

  //Llega un mensaje al topic 
  
  for (int i = 0; i < length; i++) messageTemp += (char)message[i];

  //Ejecucion de las operaciones a realizar en función del mensaje recibido:
  if(messageTemp != "ON"){
  
    if(topic == "casa/red"){
      value = messageTemp.toInt();
      value = value*1023/255;
      analogWrite(r,value);
    }

    if(topic == "casa/blue"){
      value = messageTemp.toInt();
      value = value*1023/255;
      analogWrite(b,value);
    }

    if(topic == "casa/green"){
      value = messageTemp.toInt();
      value = value*1023/255;
      analogWrite(g,value);
    }
  
  }
}

void reconnect() {
  
  while (!client.connected()) {

    //Si el dispositivo es desconectado de la red, procede a suscribirse a los topics
    
    if (client.connect("ESP8266ClientRGB")){
      client.subscribe("casa/red");
      client.subscribe("casa/blue");
      client.subscribe("casa/green");
    }
    else delay(50);
  }
  
}
