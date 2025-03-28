#include <WiFi.h>

// Set the credentials for your ESP32 access point
const char *ssid = "RoboVortex-Bot-"; // Network name for ESP32 AP
const char *password = "61991600"; // Password for the AP (optional, but recommended)

WiFiServer server(80);

// Motor pins
const int motorA_IN1 = 19;
const int motorA_IN2 = 21;
const int motorA_ENA = 33;

const int motorB_IN3 = 5;
const int motorB_IN4 = 18;
const int motorB_ENB = 32;

const int motorC_IN5 = 25;
const int motorC_IN6 = 26;
const int motorC_ENC = 27;

int normalSpeed = 150;  // Normal speed for A & B motors
int boostedSpeed = 255; // Boosted speed (max)

int normalSpeed_c = 100;
int currentSpeed = 150; // Variable to hold current speed
int currentSpeed_c = 100;

void setup()
{
  Serial.begin(115200);

  // Configure motor pins
  pinMode(motorA_IN1, OUTPUT);
  pinMode(motorA_IN2, OUTPUT);
  pinMode(motorA_ENA, OUTPUT);

  pinMode(motorB_IN3, OUTPUT);
  pinMode(motorB_IN4, OUTPUT);
  pinMode(motorB_ENB, OUTPUT);

  pinMode(motorC_IN5, OUTPUT);
  pinMode(motorC_IN6, OUTPUT);
  pinMode(motorC_ENC, OUTPUT);

  stopMotors();
  analogWrite(motorA_ENA, normalSpeed);
  analogWrite(motorB_ENB, normalSpeed);
  analogWrite(motorC_ENC, normalSpeed_c); // Motor C speed

  // Set up the ESP32 as an Access Point
  WiFi.softAP(ssid, password);  // Start the access point with the given SSID and password
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP()); // Display the IP address of the ESP32 in AP mode

  server.begin(); // Start the server to listen for incoming connections
}

void loop()
{
  WiFiClient client = server.available();
  if (client)
  {
    String request = client.readStringUntil('\r');
    client.flush();

    if (request.indexOf("/forward") != -1)
      moveForward();
    else if (request.indexOf("/backward") != -1)
      moveBackward();
    else if (request.indexOf("/left") != -1)
      turnLeft();
    else if (request.indexOf("/right") != -1)
      turnRight();
    else if (request.indexOf("/stop") != -1)
      stopMotors();
    else if (request.indexOf("/motorcw") != -1)
      motorClockwise();
    else if (request.indexOf("/motorccw") != -1)
      motorAntiClockwise();
    else if (request.indexOf("/motorstop") != -1)
      stopMotorC();
    else if (request.indexOf("/speedboost") != -1)
      setMotorSpeed(boostedSpeed);
    else if (request.indexOf("/speednormal") != -1)
      setMotorSpeed(normalSpeed);
    else if (request.indexOf("/motor_c_boost") != -1)
      setMotorSpeed_c(boostedSpeed);
    else if (request.indexOf("/motor_c_normal") != -1)
      setMotorSpeed_c(normalSpeed_c);

    // Simple HTTP response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();
    client.println("<html><body><h2>ESP32 Motor Control</h2>");
    client.println("<a href=\"/forward\">Forward</a><br>");
    client.println("<a href=\"/backward\">Backward</a><br>");
    client.println("<a href=\"/left\">Left</a><br>");
    client.println("<a href=\"/right\">Right</a><br>");
    client.println("<a href=\"/stop\">Stop</a><br>");
    client.println("<a href=\"/motorcw\">Motor C CW</a><br>");
    client.println("<a href=\"/motorccw\">Motor C CCW</a><br>");
    client.println("<a href=\"/motorstop\">Motor C Stop</a><br>");
    client.println("</body></html>");

    delay(10);
    client.stop();
  }
}

// Motor control functions (same as before)
void moveForward()
{
  digitalWrite(motorA_IN1, HIGH);
  digitalWrite(motorA_IN2, LOW);
  digitalWrite(motorB_IN3, HIGH);
  digitalWrite(motorB_IN4, LOW);
}

void moveBackward()
{
  digitalWrite(motorA_IN1, LOW);
  digitalWrite(motorA_IN2, HIGH);
  digitalWrite(motorB_IN3, LOW);
  digitalWrite(motorB_IN4, HIGH);
}

void turnLeft()
{
  digitalWrite(motorA_IN1, LOW);
  digitalWrite(motorA_IN2, HIGH);
  digitalWrite(motorB_IN3, HIGH);
  digitalWrite(motorB_IN4, LOW);
}

void turnRight()
{
  digitalWrite(motorA_IN1, HIGH);
  digitalWrite(motorA_IN2, LOW);
  digitalWrite(motorB_IN3, LOW);
  digitalWrite(motorB_IN4, HIGH);
}

void stopMotors()
{
  digitalWrite(motorA_IN1, LOW);
  digitalWrite(motorA_IN2, LOW);
  digitalWrite(motorB_IN3, LOW);
  digitalWrite(motorB_IN4, LOW);
}

void motorClockwise()
{
  digitalWrite(motorC_IN5, HIGH);
  digitalWrite(motorC_IN6, LOW);
}

void motorAntiClockwise()
{
  digitalWrite(motorC_IN5, LOW);
  digitalWrite(motorC_IN6, HIGH);
}

void stopMotorC()
{
  digitalWrite(motorC_IN5, LOW);
  digitalWrite(motorC_IN6, LOW);
}

void setMotorSpeed(int speed)
{
  analogWrite(motorA_ENA, speed);
  analogWrite(motorB_ENB, speed);
  currentSpeed = speed;
  Serial.print("Motor A & B speed set to: ");
  Serial.println(speed);
}

void setMotorSpeed_c(int speed)
{
  analogWrite(motorC_ENC, speed);
  currentSpeed_c = speed;
  Serial.print("Motor C speed set to: ");
  Serial.println(speed);
}
