#include <WiFi.h>
#include <ESP32Servo.h>

const char* ssid = "Test";  //   Wifi name
const char* password = "Test";  //  password

WiFiServer server(80);  // Set up the server on port 80 
Servo myServo;  // Create a servo object
const int servoPin = 13;  // Pin 13 for the servo

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the server
  server.begin();

  // Attach the servo to pin 13
  myServo.attach(servoPin, 500, 2500);  // the min/max pulse 
  Serial.println("Servo attached to pin 13");
}

void loop() {
  WiFiClient client = server.available();  // Check  new client

  if (client) {
    Serial.println("New Client Connected");

    // Wait for data from the client
    String request = "";
    while (client.available()) {
      request += (char)client.read();
    }

    Serial.println("Request: " + request);

    // Send HTTP response
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println();

    // Simple HTML response with links to control the servo
    client.println("<!DOCTYPE HTML><html>");
    client.println("<head><title>ESP32 Servo Control</title></head>");
    client.println("<body>");
    client.println("<h1>ESP32 Servo Control</h1>");
    client.println("<p><a href=\"/on\"><button>Turn ON</button></a></p>");
    client.println("<p><a href=\"/off\"><button>Turn OFF</button></a></p>");
    client.println("</body></html>");
    
    // Check if the request contains '/on' or '/off' 
    if (request.indexOf("/on") != -1) {
      myServo.write(90);  // Move servo to 90 degrees (turn ON)
      client.println("<p>Servo Moved to 90 Degrees (ON)</p>");
      Serial.println("Servo moved to 90 degrees");
    } 
    else if (request.indexOf("/off") != -1) {
      myServo.write(0);  // Move servo to 0 degrees (turn OFF)
      client.println("<p>Servo Moved to 0 Degrees (OFF)</p>");
      Serial.println("Servo moved to 0 degrees");
    } 
    else {
      client.println("<p><a href='/on'>Move Servo to 90 Degrees</a></p>");
      client.println("<p><a href='/off'>Move Servo to 0 Degrees</a></p>");
    }

    client.println("</html>");
    client.stop();  // Close the connection properly
    Serial.println("Client Disconnected");
  }
}
