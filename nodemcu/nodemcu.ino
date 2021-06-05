#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "JioFiber-qm6pQ";
const char* password = "90345327oo";
const char* host = "192.168.1.3"; //it will tell you the IP once it starts up
//just write it here afterwards and upload
int ledPin = D0;


WiFiServer server(301); //just pick any port number you like

void setup() {
  Serial.begin(9600);
  delay(10);
  Serial.println(WiFi.localIP());
  // prepare GPIO pins
  pinMode(ledPin, OUTPUT);
  pinMode(D1, OUTPUT); //extend these to D8 if you are using a 8 pin relay
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  digitalWrite(D0, HIGH);
  digitalWrite(D1, HIGH); // Make it low if you want everything to go off
  digitalWrite(D2, HIGH); // in case of a power cut
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");

  // Print the IP address
  String local_ip=WiFi.localIP().toString().c_str();

  // send ip to me
  HTTPClient http; //Object of class HTTPClient
  String url = "http://192.168.29.18:5000/newdevice/"; // enter your flask server's local ip
  url+="node0&";
  url+=local_ip;
  Serial.println(local_ip);
  http.begin(url);
  int httpCode = http.GET();
  String payload = http.getString();    //Get the response payload

  Serial.println(httpCode);   //Print HTTP return code
  Serial.println(payload);    //Print request response payload

  http.end();  //Close connection
}

void loop() {
  // Check if a client has connected

  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  client.flush();

  // Prepare the response
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";

  // Match the request
  if (req.indexOf("") != -10) {  //checks if you're on the main page
    //D0 routes
    if (req.indexOf("D0/off") != -1) { //checks if you clicked OFF
      digitalWrite(ledPin, HIGH);
      s += "D0 OFF";
    }
    if (req.indexOf("D0/on") != -1) { //checks if you clicked ON
      digitalWrite(ledPin, LOW);
      s += "D0 ON";
    }
    //D1 routes
    if (req.indexOf("D1/off") != -1) { //checks if you clicked OFF
      digitalWrite(D1, HIGH);
      s += "D1 OFF";
    }
    if (req.indexOf("D1/on") != -1) { //checks if you clicked ON
      digitalWrite(D1, LOW);
      s += "D1 ON";
    }
    //D2 routes
    if (req.indexOf("D2/off") != -1) { //checks if you clicked OFF
      digitalWrite(D2, HIGH);
      s += "D2 OFF";
    }
    if (req.indexOf("D2/on") != -1) { //checks if you clicked ON
      digitalWrite(D2, LOW);
      s += "D2 ON";
    }
    //D3 routes
    if (req.indexOf("D3/off") != -1) { //checks if you clicked OFF
      digitalWrite(D3, HIGH);
      s += "D3 OFF";
    }
    if (req.indexOf("D3/on") != -1) { //checks if you clicked ON
      digitalWrite(D3, LOW);
      s += "D3 ON";
    }
    //D4routes
    if (req.indexOf("D4/off") != -1) { //checks if you clicked OFF
      digitalWrite(D4, HIGH);
      s += "D4 OFF";
    }
    if (req.indexOf("D4/on") != -1) { //checks if you clicked ON
      digitalWrite(D4, LOW);
      s += "D4 ON";
    }
  }

  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }

  client.flush();

  // Send the response to the client
  client.print(s);
  delay(1);
}
