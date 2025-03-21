#include <WiFi.h>

const char *ssid = "SUMO-ROBO";
const char *password = "61991600";

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

int normalSpeed = 100;  // Normal speed for A & B motors
int boostedSpeed = 255; // Boosted speed (max)

int currentSpeed = 100; // Variable to hold current speed

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
  analogWrite(motorA_ENA, normalSpeed); // Normal initial speed
  analogWrite(motorB_ENB, normalSpeed);
  analogWrite(motorC_ENC, 255); // Motor C speed

  // Set ESP32 as Access Point
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("ESP32 AP IP address: ");
  Serial.println(IP);

  server.begin();
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

    // HTTP response (optional)
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

// Motor control functions
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
