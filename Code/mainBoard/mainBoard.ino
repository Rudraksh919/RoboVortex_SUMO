#include <WiFi.h>

const char* ssid = "SUMO-ROBO";
const char* password = "61991600";

WiFiServer server(80);

// Motor pins
const int motorA_IN1 = 5;
const int motorA_IN2 = 18;
const int motorA_ENA = 32;

const int motorB_IN3 = 19;
const int motorB_IN4 = 21;
const int motorB_ENB = 33;

const int motorC_IN5 = 25;
const int motorC_IN6 = 26;
const int motorC_ENC = 27;

void setup() {
  Serial.begin(115200);

  // Configure motor pins
  pinMode(motorA_IN1, OUTPUT);
  pinMode(motorA_IN2, OUTPUT);
  pinMode(motorA_ENA, OUTPUT);

  pinMode(motorB_IN3, OUTPUT);
  pinMode(motorB_IN4, OUTPUT);
  pinMode(motorB_ENB, OUTPUT);

  pinMode(motorC_IN5, OUTPUT);
  pinMode(motorC_IN6 ,OUTPUT);
  pinMode(motorC_ENC, OUTPUT);

  stopMotors();
  analogWrite(motorA_ENA, 255); // Motor A at full speed
  analogWrite(motorB_ENB, 255); // Motor B at full speed

  // Set ESP32 as Access Point
  WiFi.softAP(ssid, password);
  
  IPAddress IP = WiFi.softAPIP();
  Serial.print("ESP32 AP IP address: ");
  Serial.println(IP);

  server.begin();
}

void loop() {
  
 WiFiClient client = server.available();
 if (client) {
   String request = client.readStringUntil('\r');
   client.flush();

   if (request.indexOf("/forward") != -1) moveForward();
   else if (request.indexOf("/backward") != -1) moveBackward();
   else if (request.indexOf("/left") != -1) turnLeft();
   else if (request.indexOf("/right") != -1) turnRight();
   else if (request.indexOf("/stop") != -1) stopMotors();
   else if (request.indexOf("/motorcw") != -1) motorClockwise();
   else if (request.indexOf("/motorccw") != -1) motorAntiClockwise();

   // HTTP response with control buttons
   client.println("HTTP/1.1 200 OK");
   client.println("Content-type:text/html");
   client.println();
   client.println("<html><body><h2>ESP32 Motor Control</h2>");
   client.println("<a href=\"/forward\">Forward</a><br>");
   client.println("<a href=\"/backward\">Backward</a><br>");
   client.println("<a href=\"/left\">Left</a><br>");
   client.println("<a href=\"/right\">Right</a><br>");
   client.println("<a href=\"/stop\">Stop</a><br>");
   client.println("<a href=\"/motorcw_c\">Motor C CW</a><br>");
   client.println("<a href=\"/motorccw_c\">Motor C CCW</a><br>");
   client.println("</body></html>");

   delay(10);
   client.stop();
 }
}

// Motor control functions
void moveForward() {
 digitalWrite(motorA_IN1,HIGH); digitalWrite(motorA_IN2,LOW);
 digitalWrite(motorB_IN3,HIGH); digitalWrite(motorB_IN4,LOW);
}

void moveBackward() {
 digitalWrite(motorA_IN1,LOW); digitalWrite(motorA_IN2,HIGH);
 digitalWrite(motorB_IN3,LOW); digitalWrite(motorB_IN4,HIGH);
}

void turnLeft() {
 digitalWrite(motorA_IN1,LOW); digitalWrite(motorA_IN2,HIGH);
 digitalWrite(motorB_IN3,HIGH); digitalWrite(motorB_IN4,LOW);
}

void turnRight() {
 digitalWrite(motorA_IN1,HIGH); digitalWrite(motorA_IN2,LOW);
 digitalWrite(motorB_IN3,LOW); digitalWrite(motorB_IN4,HIGH);
}

void stopMotors() {
  digitalWrite(motorA_IN1,LOW); digitalWrite(motorA_IN2,LOW);
 digitalWrite(motorB_IN3,LOW); digitalWrite(motorB_IN4,LOW);
}

void motorClockwise() {
  digitalWrite(motorC_IN5, HIGH); digitalWrite(motorC_IN6, LOW);
  analogWrite(motorC_ENC, 200);
}

void motorAntiClockwise() {
  digitalWrite(motorC_IN5, LOW); digitalWrite(motorC_IN6, HIGH);
  analogWrite(motorC_ENC, 200);
}
