#include <Ethernet.h>
#include <SPI.h>


byte mac[] = { 0xA6, 0xA3, 0xA2, 0x25, 0xFE, 0xED };
byte server[] = { 104, 236, 247, 150 }; 
//104.236.247.150

  char id1[] = "p55a11e1r1";
  char id2[]= "p55a11e1r2";
  char alt[3];
  int i =0;
  int altura = 0;
  int initial = 0;
  int count = 0;
  int conn = 0;


//----------sensor 1--------
#define echoPin 3 // Echo Pin
#define trigPin 2 // Trigger Pin
//---------------------------

//----------sensor 2---------
#define echoPin2 6 // Echo Pin
#define trigPin2 5 // Trigger Pin
//-----------------------------
#define LEDPin 13 // Onboard LED

#define rele 12 // rele



int a;
int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance, duration2, distance2; // Duration used to calculate distance

EthernetClient client;

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT); //sensor1
  pinMode(echoPin, INPUT); //sensor1

  pinMode(trigPin2, OUTPUT); //sensor 2
  pinMode(echoPin2, INPUT); //sensor 2

  pinMode(9, OUTPUT); //sensor 2

  pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)

  pinMode(rele, OUTPUT); // rele

  


  Ethernet.begin(mac);
  
  Serial.println("connecting...");


if (client.connect(server, 80)) {
    Serial.println("connected");
    client.println("GET /return.altura.php?eqpt=p55a11e2&reserva=1");
    client.println();
  } else {
    Serial.println("connection failed");
  }

  
}

void loop() {
  /* The following trigPin/echoPin cycle is used to determine the
   distance of the nearest object by bouncing soundwaves off of it. */

   count++;
    conn++;
  
 if (client.available()) {
    char c = client.read();
    Serial.println(c);
     if(altura == 0){   
          alt[i] = c;
          conn = 0;
     }
    i++;
    
 }

  Serial.println("Já tem altura");
  Serial.print("altura : ");
  Serial.println(altura);
       

  //sensor 1 --------------------------------------
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);




  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/58.2;

  if (distance >= maximumRange || distance <= minimumRange){
    /* Send a negative number to computer and Turn LED ON
     to indicate "out of range" */
    Serial.println("Sensor 1 MaxD / MinD");

  }
  else {
    /* Send the distance to the computer using Serial protocol, and
     turn LED OFF to indicate successful reading. */
    Serial.println("Sensor 1 : "),   Serial.println(distance);

  }
  //sensor 2 ----------------------------------------------
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);




  //Calculate the distance (in cm) based on the speed of sound.
  distance2 = duration2/58.2;

  if (distance2 >= maximumRange || distance2 <= minimumRange){
    /* Send a negative number to computer and Turn LED ON
     to indicate "out of range" */
    Serial.println("Sensor 2 MaxD / MinD");

  }
  else {
    /* Send the distance to the computer using Serial protocol, and
     turn LED OFF to indicate successful reading. */
    Serial.println("Sensor 2 : "),  Serial.println(distance2);


  }

 if (!client.connected() && conn == 1) {
    altura = atoi(alt);
    client.stop();
    if(client.connect(server, 80))
        Serial.println("conectado 1");
    client.print("GET /altura.php?reserva_id=p55a11e2r1&distancia=");
    client.println(distance);
  }

 if(count > 1500 ){
   if (!client.connected() && conn == 2) {
      client.stop();
      if(client.connect(server, 80))
          Serial.println("conectado 3");
      client.print("GET /save.volume.php?reserva_id=p55a11e2r1&distancia=");
      client.println(distance);
      conn = -30;
    }
    if (!client.connected() && conn < 0 ) {
      client.stop();
      if(client.connect(server, 80))
          Serial.println("conectado 4");
      client.print("GET /save.volume.php?reserva_id=p55a11e2r2&distancia=");
      client.println(distance);
      conn = 0;
      count = 0;
    }

 }  
  

 if (!client.connected() && conn == 2) {
    altura = atoi(alt);
    client.stop();
    if(client.connect(server, 80))
        Serial.println("conectado 2");
    client.print("GET /altura.php?reserva_id=p55a11e2r2&distancia=");
    client.println(distance2);
    conn = 0;
  }

 

  
   

  

  if (distance > 10){
    
    Serial.println("precisa de agua, Ligar Bomba");
    a=1;
  } 
  else {
    Serial.println("Está cheio");
    digitalWrite(9, LOW);
    digitalWrite(A1, LOW);


    a=0;
  
  }

  digitalWrite(8, HIGH);

  if (distance2 < 16 && a==1){
    Serial.println("bomba está ligada");
    a=0;


    digitalWrite(9, HIGH);


  } 
  else {
    if(a == 1){
      Serial.println("Não temos agua no reservatorio");
      a = 1;
    }
  }
  if(a==1){

    Serial.println("Valvula aberta");

    digitalWrite(9, LOW);

    digitalWrite(A1, HIGH);
  }



 



  //Delay 50ms before next reading.
  delay(1000);
}

