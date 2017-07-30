/*
 *  This sketch demonstrates how to set up a simple HTTP-like server.
 *  The server will set a GPIO pin depending on the request
 *    http://server_ip/gpio/0 will set the GPIO2 low,
 *    http://server_ip/gpio/1 will set the GPIO2 high
 *  server_ip is the IP address of the ESP8266 module, will be 
 *  printed to Serial when the module is connected.
 */

#include <ESP8266WiFi.h>
#include <Servo.h>

const char* ssid = "cheesenip";
const char* password = "12345678";

int pre_cook_time = 0;

//straw values
const int STRAW_PIN = D1;
const int STRAW_INIT_POS = 5; //?
const int STRAW_TURN_POS = 70; //?
const int STRAW_SUSPEND_TIME = 1.5 * 60 * 1000; //?

//spice dispenser values
const int SPICE_PIN = D2;
const int SPICE_INIT_POS = 0;
const int SPICE_TURN_POS = 180;
int spice_intensity = 7; //? through function call

// heater values
const int HEAT_PIN = D3;
const int HEAT_OFF_POS = 180;
const int HEAT_MED_POS = 0;
const int HEAT_LOW_POS = 150;
const int HEAT_TIME = 3 * 60 * 1000; //? until hot enough to put in noodles

// noodle dispenser values
const int NOODLE_PIN = D4;
const int NOODLE_INIT_POS = 50; //?
const int NOODLE_TURN_POS = 115; //?
const int NOODLE_HANG_TIME = 2000;
const int NOODLE_COOK_TIME = 3 * 60 * 1000; //? how long to cook noodles

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(80);
Servo straw_servo;
Servo heat_servo;
Servo spice_servo;
Servo noodle_servo;


void setup() {
  // straw setup
  straw_servo.attach(STRAW_PIN);
  straw_servo.write(STRAW_INIT_POS);
  
  // spice setup
  spice_servo.attach(SPICE_PIN);
  spice_servo.write(SPICE_INIT_POS);

  // heat setup
  heat_servo.attach(HEAT_PIN);
  heat_servo.write(HEAT_OFF_POS);
  
  // noodle setup
  noodle_servo.attach(NOODLE_PIN);
  noodle_servo.write(NOODLE_INIT_POS);
  
  Serial.begin(115200);
  delay(10);
  
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
  Serial.println(WiFi.localIP());
}

void loop() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();

  String s = "";
  
  // Match the request
  if (req.indexOf("/makeramen") != -1)
  {
    //int info_index = req.indexOf('/', req.indexOf("/makeramen") + 1) + 1;
    //int num_input = req.substring(info_index, req.indexOf('/', info_index)).toInt();
    //spice_intensity = num_input / 10000;
    //int hhmm_format = num_input % 10000;
    //pre_cook_time = (hhmm_format / 100) * 3600000 + (hmm_format % 100) * 60000;
    makeramen();
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nMaking ramen ";
    s += "</html>\n";
  }
  else if (req.indexOf("/test/strawdown") != -1)
  {
    straw_down();
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nStraw is down ";
    s += "</html>\n";
  }
  else if (req.indexOf("/test/strawup") != -1)
  {
    straw_up();
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nStraw is up ";
    s += "</html>\n";
  }
  else if (req.indexOf("/test/dispensespice") != -1)
  {
    dispense_spice();
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nDispensing spice ";
    s += "</html>\n";
  }
  else if (req.indexOf("/test/turnonheat") != -1)
  {
    turn_on_heat();
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nTurning heat on ";
    s += "</html>\n";
  }
  else if (req.indexOf("/test/pushnoodles") != -1)
  {
    push_noodles();
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nAdding noodles ";
    s += "</html>\n";
  }
  else if (req.indexOf("/test/retractnoodles") != -1)
  {
    retract_noodles();
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nAdding noodles ";
    s += "</html>\n";
  }
  else if (req.indexOf("/test/turnoffheat") != -1)
  {
    turn_off_heat();
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nTurning heat off ";
    s += "</html>\n";
  }
  else if (req.indexOf("/stove/med") != -1)
  {
    heat_servo.write(HEAT_MED_POS);
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nHeat is set to medium ";
    s += "</html>\n";
  }
  else if (req.indexOf("/stove/low") != -1)
  {
    heat_servo.write(HEAT_LOW_POS);
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nHeat is set to low ";
    s += "</html>\n";
  }
  else if (req.indexOf("/stove/off") != -1)
  {
    heat_servo.write(HEAT_OFF_POS);
    s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nHeat is off ";
    s += "</html>\n";
  }
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  
  client.flush();

  // Prepare the response
 

  // Send the response to the client
  client.print(s);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}

void makeramen()
{
  //pre-cook wait time
  unsigned long startMillis = millis();
  while (millis() - startMillis < pre_cook_time);
  
  //pour water
  straw_down();
  delay(STRAW_SUSPEND_TIME);
  straw_up();

  //dispense spice
  dispense_spice();

  //turn on heat
  turn_on_heat();
  delay(HEAT_TIME);

  //add noodles
  push_noodles();
  retract_noodles();
  delay(NOODLE_COOK_TIME);
  

  //turn off heat
  turn_off_heat();

  //close water
  return;
}

void straw_down()
{
  straw_servo.write(STRAW_TURN_POS);
}

void straw_up()
{
  straw_servo.write(STRAW_INIT_POS);
  delay(1000);
}

void dispense_spice()
{
  for (int i =0; i < spice_intensity * 3; i++)
  {
    spice_servo.write(SPICE_TURN_POS);
    delay(1000);
    spice_servo.write(SPICE_INIT_POS);
    delay(1000);
  }
  return;
}

void turn_on_heat()
{
  heat_servo.write(HEAT_MED_POS);
  return;
}

void push_noodles()
{
  noodle_servo.write(NOODLE_TURN_POS);
  delay(NOODLE_HANG_TIME);
}

void retract_noodles()
{
  noodle_servo.write(NOODLE_INIT_POS);
  for (int i = 0; i < 3; i++)
  {
    noodle_servo.write(NOODLE_INIT_POS - 5);
    delay(500);
    noodle_servo.write(NOODLE_INIT_POS + 5);
    delay(500);
  }
}

void turn_off_heat()
{
  heat_servo.write(HEAT_OFF_POS);
  delay(1000);
  return;
}

