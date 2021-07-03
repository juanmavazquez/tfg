//___________LIBRERIAS___________//

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

//___________DEFINES___________//

#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4

#define BOTON D5

#define TIEMPO_DeepSleep 840000000 //tiempo en uS (actual=14min,840 000 000)
 
//___________VALORES RED WIFI___________//

const char* ssid = "Nombre Red Wi-Fi";
const char* password = "Contraseña Wi-Fi";
const char* mqtt_server = "IP Raspberry";

//___________TOPICS___________// 

const char* topicDesplegar = "casa/motor/desplegar";
const char* topicRecoger = "casa/motor/recoger";

const char* topicConfiguracion = "casa/motor/configuracion";

const char* topicEstadoGuardarPasos = "casa/motor/estado/pasos/guardar";
const char* topicEstadoPasos = "casa/motor/estado/pasos";

//___________VARIABLES___________// 

int Paso [ 8 ][ 4 ] =
{ {1, 0, 0, 0},
  {1, 1, 0, 0},
  {0, 1, 0, 0},
  {0, 1, 1, 0},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 0, 0, 1},
  {1, 0, 0, 1}
};
int steps_left;
boolean Recogido = true; // Recogido == false -> giro horario (despliega el toldo)
int Steps = 0;
int ESTADOBOTON;
char pasos[50];
int pasosDespliegue;

//___________SUBPROGRMAS___________//

void setup_wifi(); //Inicia conexion WiFi
void callback(String topic, byte* message, unsigned int length); //Procesamiento de los mensajes recibidos a los topics
void reconnect(); //Reconexión del dispositivo a la red 
void dormir();
void motor();
void stepper();
void SetDirection();
void configuracion(); //Entra en modo "configuracion" para ajustar las vueltas a dar por el motor
bool PresionarBoton();

//___________CREACION DE LOS OBJETOS___________//

WiFiClient espClient;
PubSubClient client(espClient);
 
//___________PROGRAMA___________//

void setup(void){

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(BOTON,INPUT);  
 
  setup_wifi();
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
}
 
void loop(){

  if (!client.connected()) reconnect();
  
  if(!client.loop()) client.connect("ESP8266ClientMotor");
  
  if(PresionarBoton()) configuracion();

}

//___________SUBPROGRAMAS___________//

void setup_wifi() {
    
  //Conexión wifi:
  
  WiFi.begin(ssid, password); 
  
  //Mientras no se realice la conexión, este bucle es ejecutado
  
  while(WiFi.status() != WL_CONNECTED) delay(500);

}

void callback(String topic, byte* message, unsigned int length) {

  String messageTemp;

  //Llega un mensaje al topic 
  
  for (int i = 0; i < length; i++) messageTemp += (char)message[i];

  //Ejecucion de las operaciones a realizar en función del mensaje recibido:

  if(topic == topicRecoger){

      if(Recogido == false){ //Evita que el motor gire en el sentido "Recoger" cuando ya se encuentra recogido      
        Recogido = true;
        motor();
      }

      dormir();
  }
  
  else if(topic == topicDesplegar){
      
      if(Recogido == true){//Evita que el motor gire en el sentido "Desplegar" cuando ya se encuentra desplegado
        Recogido = false;
        motor();
      }

      dormir();

  }
  
  else if(topic == topicEstadoPasos) pasosDespliegue = messageTemp.toInt();
  
}

void reconnect() {
  
  while (!client.connected()) {

    //Si el dispositivo es desconectado de la red, procede a suscribirse al topic
    
    if (client.connect("ESP8266ClientMotor")){ 
      
      client.subscribe(topicDesplegar);
      client.subscribe(topicRecoger);
      client.subscribe(topicConfiguracion);
      client.subscribe(topicEstadoPasos);
      
    }
    
    else delay(50);
  
  }
  
}

void dormir(){

  ESP.deepSleep(TIEMPO_DeepSleep, WAKE_NO_RFCAL);  // cuando se reinicia el microprocesador no se calibra la señal de radio. Esto reduce el consumo la señal de radio después del reencendido
  
}

void motor(){
  
  steps_left = pasosDespliegue;

  while (steps_left > 0){
    stepper() ;     // Avanza un paso
    steps_left-- ;  // Decrementa el contador de pasos necesarios
    delay(1);
  }
  
}

void stepper(){            //Avanza un paso
  
  digitalWrite( IN1, Paso[Steps][ 0] );
  digitalWrite( IN2, Paso[Steps][ 1] );
  digitalWrite( IN3, Paso[Steps][ 2] );
  digitalWrite( IN4, Paso[Steps][ 3] );

  SetDirection();
}
void SetDirection(){
  
  if (Recogido) Steps++;
  else Steps--;

  Steps = ( Steps + 8 ) % 8 ;
  
}

void configuracion(){

  int pasos_int = 0;

  if(Recogido == false){  //Si se procede a la configuracion con el toldo desplegado, primero, es plegado
    Recogido = true;
    motor();
  }

  Recogido=false;
  
  while(!PresionarBoton()){
    stepper();
    pasos_int++;
    delay(1);
  }
         
  String pasos_str = String(pasos_int);
  pasos_str.toCharArray(pasos, pasos_str.length() + 1);
      
  client.publish(topicEstadoGuardarPasos,pasos);

}

bool PresionarBoton(){
  
  if(digitalRead(BOTON) == HIGH) ESTADOBOTON = 1;                                          
         
  if(ESTADOBOTON == HIGH && digitalRead(BOTON) == LOW){ 
    ESTADOBOTON = 0;                                          
    return true;
  }else return false;
  
}
