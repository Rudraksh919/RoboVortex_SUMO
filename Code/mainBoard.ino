#include <WiFi.h>
#include <WebSocketsServer.h>

// const char* ssid = "SUMO_BOT";      // WiFi AP Name
// const char* password = "12345678";  // WiFi Password

// WebSocketsServer webSocket(81);  // WebSocket Server on Port 81

// Define Motor Pins
#define MOTOR1_1 5  // Left forward
#define MOTOR1_2 18 // Left backward
#define MOTOR2_1 19 // Right forward
#define MOTOR2_2 21 // Right backward
#define MOTOR1_PWM 14 // PWM control for left motor
#define MOTOR2_PWM 15 // PWM control for right motor

void setup() {
  Serial.begin(115200);

  // // Set up WiFi Access Point
  // WiFi.softAP(ssid, password);
  // Serial.println("WiFi AP Started");
  // Serial.print("IP Address: ");
  // Serial.println(WiFi.softAPIP());

  // // Start WebSocket Server
  // webSocket.begin();
  // webSocket.onEvent(webSocketEvent);

  // Set motor pins as output
  pinMode(MOTOR1_1, OUTPUT);
  pinMode(MOTOR1_2, OUTPUT);
  pinMode(MOTOR2_1, OUTPUT);
  pinMode(MOTOR2_2, OUTPUT);
  pinMode(MOTOR1_PWM, OUTPUT);
  pinMode(MOTOR2_PWM, OUTPUT);

  // Initialize motors to stop state
  // stopMotors();
}

void loop() {
  // webSocket.loop();  // Handle WebSocket Events
  analogWrite(MOTOR1_PWM,4096);
  analogWrite(MOTOR2_PWM,4096);
}

// // Function to control motors
// void moveBot(String direction) {
//   if (direction == "forward") {
//     digitalWrite(MOTOR1_1, HIGH);
//     digitalWrite(MOTOR2_1, HIGH);
//     digitalWrite(MOTOR1_2, LOW);
//     digitalWrite(MOTOR2_2, LOW);
//   } else if (direction == "backward") {
//     digitalWrite(MOTOR1_1, LOW);
//     digitalWrite(MOTOR2_1, LOW);
//     digitalWrite(MOTOR1_2, HIGH);
//     digitalWrite(MOTOR2_2, HIGH);
//   } else if (direction == "left") {
//     digitalWrite(MOTOR1_1, LOW);
//     digitalWrite(MOTOR2_1, HIGH);
//     digitalWrite(MOTOR1_2, HIGH);
//     digitalWrite(MOTOR2_2, LOW);
//   } else if (direction == "right") {
//     digitalWrite(MOTOR1_1, HIGH);
//     digitalWrite(MOTOR2_1, LOW);
//     digitalWrite(MOTOR1_2, LOW);
//     digitalWrite(MOTOR2_2, HIGH);
//   } else if (direction == "stop") {
//     stopMotors();
//   }
// }

// // Stop all motors
// void stopMotors() {
//   digitalWrite(MOTOR1_1, LOW);
//   digitalWrite(MOTOR2_1, LOW);
//   digitalWrite(MOTOR1_2, LOW);
//   digitalWrite(MOTOR2_2, LOW);
// }

// // Handle WebSocket Messages
// void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
//   if (type == WStype_TEXT) {
//     String command = String((char*)payload).substring(0, length);  // Ensuring correct parsing
//     Serial.println("Received: " + command);
//     moveBot(command);
//   }
// }
