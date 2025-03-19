#include <WiFi.h>

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

WiFiServer server(80);

// Define motor pins
const int motorA_IN1 = 27;
const int motorA_IN2 = 26;
const int motorA_ENA = 14;

const int motorB_IN3 = 25;
const int motorB_IN4 = 33;
const int motorB_ENB = 32;

void setup() {
  Serial.begin(115200);

  // Set motor pins as outputs
  pinMode(motorA_IN1, OUTPUT);
  pinMode(motorA_IN2, OUTPUT);
  pinMode(motorA_ENA, OUTPUT);
  
  pinMode(motorB_IN3, OUTPUT);
  pinMode(motorB_IN4, OUTPUT);
  pinMode(motorB_ENB, OUTPUT);

  // Initialize motors off
  digitalWrite(motorA_ENA, LOW);
  digitalWrite(motorB_ENB, LOW);

  // Connect to Wi-Fi network
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected.");
  
  server.begin();
  
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  
  WiFiClient client = server.available();
  
  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    // Simple HTTP request parsing
    if (request.indexOf("/forward") != -1) {
      moveForward();
    } else if (request.indexOf("/backward") != -1) {
      moveBackward();
    } else if (request.indexOf("/left") != -1) {
      turnLeft();
    } else if (request.indexOf("/right") != -1) {
      turnRight();
    } else if (request.indexOf("/stop") != -1) {
      stopMotors();
    }

    // Send HTTP response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println("<html><body>");
    client.println("<h1>ESP32 Motor Control</h1>");
    client.println("<p><a href=\"/forward\">Forward</a></p>");
    client.println("<p><a href=\"/backward\">Backward</a></p>");
    client.println("<p><a href=\"/left\">Left</a></p>");
    client.println("<p><a href=\"/right\">Right</a></p>");
    client.println("<p><a href=\"/stop\">Stop</a></p>");
    client.println("</body></html>");

    delay(10);
    client.stop();
  }
}

// Motor control functions
void moveForward() {
  digitalWrite(motorA_IN1, HIGH);
  digitalWrite(motorA_IN2, LOW);
  
  digitalWrite(motorB_IN3, HIGH);
  digitalWrite(motorB_IN4, LOW);

  analogWrite(motorA_ENA, 200); // Speed between (0-255)
  analogWrite(motorB_ENB, 200);
}

void moveBackward() {
  digitalWrite(motorA_IN1, LOW);
  digitalWrite(motorA_IN2, HIGH);
  
  digitalWrite(motorB_IN3, LOW);
  digitalWrite(motorB_IN4, HIGH);

  analogWrite(motorA_ENA, 200);
  analogWrite(motorB_ENB, 200);
}

void turnLeft() {
  digitalWrite(motorA_IN1, LOW);
  digitalWrite(motorA_IN2, HIGH);
  
  digitalWrite(motorB_IN3, HIGH);
  digitalWrite(motorB_IN4, LOW);

  analogWrite(motorA_ENA,200);
  analogWrite(motorB_ENB,200);
}

void turnRight() {
 digitalWrite(motorA_IN1,HIGH);
 digitalWrite(motorA_IN2,LOW);

 digitalWrite(motorB_IN3,LOW);
 digitalWrite(motorB_IN4,HIGH);

 analogWrite(motorA_ENA,200); 
 analogWrite(motorB_ENB,200); 
}

void stopMotors() {
 analogWrite(motorA_ENA,0); 
 analogWrite(motorB_ENB,0); 
}
