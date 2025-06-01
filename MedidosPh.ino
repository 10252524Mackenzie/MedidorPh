//Incluir bibliotecas
#include <WiFi.h> //https://www.arduinolibraries.info/libraries/wi-fi
#include <PubSubClient.h> //https://www.arduinolibraries.info/libraries/pub-sub-client

// Define Variaveis
#define SENSORPH 35
#define LEDR 32
#define LEDY 33
#define LEDG 25

char SSIDName[] = "Wokwi-GUEST"; //nome da rede WiFi
char SSIDPass[] = ""; //senha da rede WiFI

char BrokerURL[] = "test.mosquitto.org"; //URL do broker MQTT
char BrokerUserName[] = ""; //nome do usuario para autenticar no broker MQTT
char BrokerPassword[] = ""; //senha para autenticar no broker MQTT
char MQTTClientName[] = "4567"; //nome do cliente MQTT
int BrokerPort = 1883; //porta do broker MQTT

char Topico_01[] = "mack10252524/ph4567"; //nome do topico 01

//Variaveis globais e objetos
WiFiClient espClient; //instancia o objeto espClient que pode se conectar a um endereço IP e porta de Internet especificados, conforme definido em clienteMQTT.connect()
PubSubClient clienteMQTT(espClient); //instancia o objeto clienteMQTT parcialmente inicializado cujo argumento eh o objeto para conectar na Internet
//Funcoes definidas pelo usuário

//funcao para reconectar ao broker
void mqttReconnect() {
  while (!clienteMQTT.connected()) {
    Serial.println("Conectando-se ao broker MQTT...");
    Serial.println(MQTTClientName);
    if (clienteMQTT.connect(MQTTClientName, BrokerUserName, BrokerPassword)) {
      Serial.print(MQTTClientName);
      Serial.println(" conectado!");
      clienteMQTT.subscribe(Topico_01);
    } else {
      Serial.print("failed, rc=");
      Serial.print(clienteMQTT.state());
      Serial.println(" tente novamente em 5 segundos.");
      delay(5000);
    }
  }
}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setup()
{
  // Define sensorPH para input
  pinMode(SENSORPH, INPUT);
  // Define Leds para OUTPUT
  pinMode(LEDR, OUTPUT);
  pinMode(LEDY, OUTPUT);
  pinMode(LEDG, OUTPUT);
  //configura os detalhes do broker MQTT passando URL e porta
  clienteMQTT.setServer(BrokerURL, BrokerPort);
  Serial.begin(9600);
  Serial.print("Conectando-se ao Wi-Fi");
  WiFi.begin(SSIDName, SSIDPass); //inicializa Wi-Fi
  while (WiFi.status() != WL_CONNECTED) { //repete enquanto nao estabelece conexao
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP()); //imprime o endereco IP
}

void loop() {
   // Mapeia leitura analogica do PH no range 0-14
  float valorPh = mapFloat(analogRead(SENSORPH), 0.0, 4095.0, 0.0, 14.0);
  Serial.print("pH Value: ");
  Serial.println(valorPh);
  // Define qual led acender
  if (valorPh>8.6)
  {
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDY, LOW);
    digitalWrite(LEDG, LOW);
  }
  else if (valorPh>8.3)
  {
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDY, HIGH);
    digitalWrite(LEDG, LOW);
  }
  else if(valorPh>7.8)
 {
   digitalWrite(LEDR, LOW);
   digitalWrite(LEDY, LOW);
   digitalWrite(LEDG, HIGH);
 }
 else if(valorPh>7.5)
 {
   digitalWrite(LEDR, LOW);
   digitalWrite(LEDY, HIGH);
   digitalWrite(LEDG, LOW);
 }
 else
 {
   digitalWrite(LEDR, HIGH);
   digitalWrite(LEDY, LOW);
   digitalWrite(LEDG, LOW); 
 }
 if (!clienteMQTT.connected()) { //verifica se o cliente esta conectado ao broker
    mqttReconnect(); //se o cliente nao estiver conectado, tenta reconectar
  }
  clienteMQTT.loop(); //funcao necessaria para manter a conexao com o broker MQTT ativa e coletar as mensagens recebidas  
  char valor[8]="";
  dtostrf(valorPh, 1, 2, valor);  
  clienteMQTT.publish(Topico_01, valor); //publica um topico no broker
  Serial.print("PH Publicado: ");
  Serial.println(valorPh);

  delay(5000); // this speeds up the simulation

}

