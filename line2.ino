/*
This code uses adpative learning algo that dynamically adjusts PID gains based on real-time performance.
*/
#include <WiFi.h>
#include <WebServer.h>

#define IR1 32
#define IR2 33
#define IR3 34
#define IR4 35
#define IR5 36
#define ENA 14
#define IN1 27
#define IN2 26
#define IN3 25
#define IN4 33

#define ENB 12
#define LED 2

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

WebServer server(80);

float Kp = 2.0, Ki = 0.0, Kd = 1.0;
float previousError = 0, integral = 0;
float adaptiveFactor = 0.01;  // Learning rate for adjustments
int baseSpeed = 120;

int getError() {
    int sensorValues[] = {digitalRead(IR1), digitalRead(IR2), digitalRead(IR3), digitalRead(IR4), digitalRead(IR5)};
    int position[] = {-2, -1, 0, 1, 2};
    int error = 0, count = 0;

    for (int i = 0; i < 5; i++) {
        if (sensorValues[i] == 0) {
            error += position[i];
            count++;
        }
    }
    return (count > 0) ? (error / count) : previousError;
}

void adaptivePID() {
    int error = getError();
    integral += error;
    float derivative = error - previousError;
    float correction = Kp * error + Ki * integral + Kd * derivative;

    int leftSpeed = baseSpeed - correction;
    int rightSpeed = baseSpeed + correction;

    leftSpeed = constrain(leftSpeed, 0, 255);
    rightSpeed = constrain(rightSpeed, 0, 255);

    analogWrite(ENA, leftSpeed);
    analogWrite(ENB, rightSpeed);

    if (error < 0) {
        digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    } else {
        digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    }

    // 🔹 Adaptive Learning Algorithm 🔹
    if (abs(error) > 1) {  
        Kp += adaptiveFactor * abs(error);   // Increase Kp if error is large
        Ki += adaptiveFactor * (error / 10); // Small adjustments for Ki
        Kd += adaptiveFactor * abs(derivative); // Increase Kd to prevent oscillations
    }

    previousError = error;
}

// Web Handlers
void handleStatus() {
    String json = "{";
    json += "\"connected\": " + String(WiFi.status() == WL_CONNECTED ? "true" : "false") + ",";
    json += "\"Kp\": " + String(Kp) + ",";
    json += "\"Ki\": " + String(Ki) + ",";
    json += "\"Kd\": " + String(Kd);
    json += "}";

    server.send(200, "application/json", json);
}

void handleUpdate() {
    if (server.hasArg("Kp")) Kp = server.arg("Kp").toFloat();
    if (server.hasArg("Ki")) Ki = server.arg("Ki").toFloat();
    if (server.hasArg("Kd")) Kd = server.arg("Kd").toFloat();
    server.send(200, "text/plain", "PID Updated");
}

void setup() {
    pinMode(LED, OUTPUT);
    pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        digitalWrite(LED, !digitalRead(LED));
    }
    digitalWrite(LED, HIGH);

    server.on("/status", handleStatus);
    server.on("/update", HTTP_POST, handleUpdate);
    server.begin();
}

void loop() {
    server.handleClient();
    adaptivePID();
}
