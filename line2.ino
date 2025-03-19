/*
This code uses adpative learning algo that dynamically adjusts PID gains based on real-time performance.
*/
#include <WiFi.h>               // Library to manage WiFi connection
#include <WebServer.h>           // Library to handle HTTP server functionality

// Pin assignments for the IR sensors and motor control
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
#define LED 2  // LED to indicate WiFi connection status

// WiFi credentials
const char* ssid = "Your_SSID";     // Replace with your WiFi SSID
const char* password = "Your_PASSWORD";  // Replace with your WiFi password

WebServer server(80);               // Create an HTTP server object on port 80

// PID control parameters
float Kp = 2.0, Ki = 0.0, Kd = 1.0;  // Proportional, Integral, Derivative constants for PID control
float previousError = 0, integral = 0;  // Variables to store error history for PID
float adaptiveFactor = 0.01;  // Learning rate for adapting PID constants based on error
int baseSpeed = 120;                // Base speed for the motors

// Function to read the IR sensors and calculate the error based on the sensor values
int getError() {
    int sensorValues[] = {digitalRead(IR1), digitalRead(IR2), digitalRead(IR3), digitalRead(IR4), digitalRead(IR5)};
    int position[] = {-2, -1, 0, 1, 2};   // Sensor positions corresponding to each IR sensor
    int error = 0, count = 0;

    // Loop through the sensor values and calculate the error based on which sensors are activated
    for (int i = 0; i < 5; i++) {
        if (sensorValues[i] == 0) {   // If sensor detects the line (LOW = detected)
            error += position[i];      // Add the position of the sensor to the error
            count++;                   // Increment the count of sensors detecting the line
        }
    }

    // If no sensors detect the line, return the previous error value
    return (count > 0) ? (error / count) : previousError;
}

// Function to apply the adaptive PID control and adjust motor speeds accordingly
void adaptivePID() {
    int error = getError();   // Get the current error value from the sensors
    integral += error;        // Update the integral (sum of past errors)
    float derivative = error - previousError;  // Calculate the derivative (change in error)
    float correction = Kp * error + Ki * integral + Kd * derivative;  // Calculate the correction based on PID formula

    // Calculate the speeds for the left and right motors based on the correction value
    int leftSpeed = baseSpeed - correction; 
    int rightSpeed = baseSpeed + correction;

    // Constrain the speeds to be within 0-255 (PWM range)
    leftSpeed = constrain(leftSpeed, 0, 255);
    rightSpeed = constrain(rightSpeed, 0, 255);

    // Apply the calculated speeds to the motors
    analogWrite(ENA, leftSpeed);
    analogWrite(ENB, rightSpeed);

    // Set motor direction based on the error
    if (error < 0) {    // Turn left
        digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
    } else {             // Turn right
        digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
    }

    // 🔹 Adaptive Learning Algorithm 🔹
    // Adjust PID constants based on the magnitude of the error
    if (abs(error) > 1) {  
        Kp += adaptiveFactor * abs(error);   // Increase Kp if error is large
        Ki += adaptiveFactor * (error / 10); // Small adjustments for Ki
        Kd += adaptiveFactor * abs(derivative); // Increase Kd to prevent oscillations
    }

    previousError = error;  // Update the previous error for the next loop
}

// Web Handlers
// Handle the status request (e.g., "/status") and return the current PID constants and WiFi status
void handleStatus() {
    String json = "{";
    json += "\"connected\": " + String(WiFi.status() == WL_CONNECTED ? "true" : "false") + ",";  // WiFi connection status
    json += "\"Kp\": " + String(Kp) + ",";  // Current value of Kp
    json += "\"Ki\": " + String(Ki) + ",";  // Current value of Ki
    json += "\"Kd\": " + String(Kd);        // Current value of Kd
    json += "}";

    server.send(200, "application/json", json);  // Send JSON response with status and PID parameters
}

// Handle the update request (e.g., "/update") and update the PID constants if provided in the request
void handleUpdate() {
    if (server.hasArg("Kp")) Kp = server.arg("Kp").toFloat();  // Update Kp if provided
    if (server.hasArg("Ki")) Ki = server.arg("Ki").toFloat();  // Update Ki if provided
    if (server.hasArg("Kd")) Kd = server.arg("Kd").toFloat();  // Update Kd if provided
    server.send(200, "text/plain", "PID Updated");  // Send response that the PID constants have been updated
}

void setup() {
    pinMode(LED, OUTPUT);   // Set LED pin as output
    pinMode(ENA, OUTPUT); pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT); pinMode(ENB, OUTPUT);

    WiFi.begin(ssid, password);  // Start connecting to WiFi
    while (WiFi.status() != WL_CONNECTED) {   // Wait until WiFi is connected
        delay(500);   // Wait for 500ms
        digitalWrite(LED, !digitalRead(LED));  // Blink LED to show connection progress
    }
    digitalWrite(LED, HIGH);  // Turn on LED when connected

    // Define routes for the web server
    server.on("/status", handleStatus);  // Handle status requests
    server.on("/update", HTTP_POST, handleUpdate);  // Handle PID update requests

    server.begin();  // Start the server
}

void loop() {
    server.handleClient();  // Handle incoming client requests
    adaptivePID();             // Apply the adaptive PID control to adjust motor speeds
}