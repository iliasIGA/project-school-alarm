#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>
#include "index.h" // Assuming this file contains your HTML content

const int SDA_PIN = 21;  // GPIO21 for SDA
const int SCL_PIN = 22;  // GPIO22 for SCL
Wire.begin(SDA_PIN, SCL_PIN);
// I2C LCD setup (0x27 is the default I2C address, adjust if needed)
// Parameters: I2C address, columns, rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

// NTP Server settings
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;      // Change this based on your timezone (in seconds)
const int   daylightOffset_sec = 3600; // Change this for daylight saving time if needed

// WiFi credentials
const char* ssid = "ilias";
const char* password = "ilias1234";

// Alarm status counters
int setCounter1 = 0;
int setCounter2 = 0;
int setCounter3 = 0;
int setCounter4 = 0;
int setCounter5 = 0;
int setCounter6 = 0;
int setCounter7 = 0;
int setCounter8 = 0;

// Status strings
String set1;
String set2;
String set3;
String set4;
String set5;
String set6;
String set7;
String set8;

// Time variables
int Hor;
int Min;
int Sec;

// Date variables
int p_day;
int p_month;
int p_year;

// Alarm time variables
int alarm_hour1, alarm_min1, alarm_sec1;
int alarm_hour2, alarm_min2, alarm_sec2;
int alarm_hour3, alarm_min3, alarm_sec3;
int alarm_hour4, alarm_min4, alarm_sec4;
int alarm_hour5, alarm_min5, alarm_sec5;
int alarm_hour6, alarm_min6, alarm_sec6;
int alarm_hour7, alarm_min7, alarm_sec7;
int alarm_hour8, alarm_min8, alarm_sec8;

// Define output pins for ESP32
const int buzzerPin = 26;  // GPIO26 for buzzer
const int ledPin = 27;     // GPIO27 for LED

// Create web server on port 80
WebServer server(80);

// Function to update time from internal clock
void updateTime() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  
  Hor = timeinfo.tm_hour;
  Min = timeinfo.tm_min;
  Sec = timeinfo.tm_sec;
  
  p_day = timeinfo.tm_mday;
  p_month = timeinfo.tm_mon + 1;  // tm_mon is 0-11
  p_year = timeinfo.tm_year + 1900; // tm_year is years since 1900
}

// Function to set the internal clock
void setTime(int year, int month, int day, int hour, int minute, int second) {
  struct tm timeinfo;
  
  timeinfo.tm_year = year - 1900;
  timeinfo.tm_mon = month - 1;
  timeinfo.tm_mday = day;
  timeinfo.tm_hour = hour;
  timeinfo.tm_min = minute;
  timeinfo.tm_sec = second;
  
  time_t t = mktime(&timeinfo);
  struct timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);
}

//Check if header is present and correct
bool is_authentified() {
  Serial.println("Enter is_authentified");
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("ESPSESSIONID=1") != -1) {
      Serial.println("Authentification Successful");
      return true;
    }
  }
  Serial.println("Authentification Failed");
  return false;
}

//login page, also called for disconnect
void handleLogin() {
  String msg;
  if (server.hasHeader("Cookie")) {
    Serial.print("Found cookie: ");
    String cookie = server.header("Cookie");
    Serial.println(cookie);
  }
  if (server.hasArg("DISCONNECT")) {
    Serial.println("Disconnection");
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.sendHeader("Set-Cookie", "ESPSESSIONID=0");
    server.send(301);
    return;
  }
  if (server.hasArg("USERNAME") && server.hasArg("PASSWORD")) {
    if (server.arg("USERNAME") == "admin" &&  server.arg("PASSWORD") == "admin") {
      server.sendHeader("Location", "/");
      server.sendHeader("Cache-Control", "no-cache");
      server.sendHeader("Set-Cookie", "ESPSESSIONID=1");
      server.send(301);
      Serial.println("Log in Successful");
      return;
    }
    msg = "Wrong username/password! try again.";
    Serial.println("Log in Failed");
  }
  server.send(200, "text/html", LOGIN_PAGE);
}

//root page can be accessed only if authentification is ok
void handleRoot() {
  Serial.println("Enter handleRoot");
  String header;
  if (!is_authentified()) {
    server.sendHeader("Location", "/login");
    server.sendHeader("Cache-Control", "no-cache");
    server.send(301);
    return;
  }

  if (server.hasArg("HH_PRE") && server.hasArg("MM_PRE") && server.hasArg("SS_PRE") && 
      server.hasArg("DD_PRE") && server.hasArg("MO_PRE") && server.hasArg("YYYY_PRE")) {
    String pre_h1 = server.arg("HH_PRE");
    String pre_m1 = server.arg("MM_PRE");
    String pre_s1 = server.arg("SS_PRE");
    int pre_hour1 = atoi(pre_h1.c_str());
    int pre_min1 = atoi(pre_m1.c_str());
    int pre_sec1 = atoi(pre_s1.c_str());

    String pre_d1 = server.arg("DD_PRE");
    String pre_mo1 = server.arg("MO_PRE");
    String pre_y1 = server.arg("YYYY_PRE");
    int pre_day1 = atoi(pre_d1.c_str());
    int pre_month1 = atoi(pre_mo1.c_str());
    int pre_year1 = atoi(pre_y1.c_str());
    
    // Set the internal time
    setTime(pre_year1, pre_month1, pre_day1, pre_hour1, pre_min1, pre_sec1);
    
    Serial.print("Time set to: ");
    Serial.print(pre_hour1);
    Serial.print(":");
    Serial.print(pre_min1);
    Serial.print(":");
    Serial.println(pre_sec1);

    Serial.print("Date set to: ");
    Serial.print(pre_day1);
    Serial.print("/");
    Serial.print(pre_month1);
    Serial.print("/");
    Serial.println(pre_year1);

    server.send(200, "text/html", PRETIME_SET);
  }
  
  if (server.hasArg("HH_ALARM1") && server.hasArg("MM_ALARM1") && server.hasArg("SS_ALARM1")) {
    String alarm_h1 = server.arg("HH_ALARM1");
    String alarm_m1 = server.arg("MM_ALARM1");
    String alarm_s1 = server.arg("SS_ALARM1");
    alarm_hour1 = atoi(alarm_h1.c_str());
    alarm_min1 = atoi(alarm_m1.c_str());
    alarm_sec1 = atoi(alarm_s1.c_str());
    setCounter1 = 1;
    
    Serial.print("Alarm 1 set to: ");
    Serial.print(alarm_hour1);
    Serial.print(":");
    Serial.print(alarm_min1);
    Serial.print(":");
    Serial.println(alarm_sec1);

    server.send(200, "text/html", ALARM_SET);
  }

  if (server.hasArg("HH_ALARM2") && server.hasArg("MM_ALARM2") && server.hasArg("SS_ALARM2")) {
    String alarm_h2 = server.arg("HH_ALARM2");
    String alarm_m2 = server.arg("MM_ALARM2");
    String alarm_s2 = server.arg("SS_ALARM2");
    alarm_hour2 = atoi(alarm_h2.c_str());
    alarm_min2 = atoi(alarm_m2.c_str());
    alarm_sec2 = atoi(alarm_s2.c_str());

    setCounter2 = 1;
    
    Serial.print("Alarm 2 set to: ");
    Serial.print(alarm_hour2);
    Serial.print(":");
    Serial.print(alarm_min2);
    Serial.print(":");
    Serial.println(alarm_sec2);

    server.send(200, "text/html", ALARM_SET);
  }

  if (server.hasArg("HH_ALARM3") && server.hasArg("MM_ALARM3") && server.hasArg("SS_ALARM3")) {
    String alarm_h3 = server.arg("HH_ALARM3");
    String alarm_m3 = server.arg("MM_ALARM3");
    String alarm_s3 = server.arg("SS_ALARM3");
    alarm_hour3 = atoi(alarm_h3.c_str());
    alarm_min3 = atoi(alarm_m3.c_str());
    alarm_sec3 = atoi(alarm_s3.c_str());

    setCounter3 = 1;
    Serial.print("Alarm 3 set to: ");
    Serial.print(alarm_hour3);
    Serial.print(":");
    Serial.print(alarm_min3);
    Serial.print(":");
    Serial.println(alarm_sec3);

    server.send(200, "text/html", ALARM_SET);
  }
  
  if (server.hasArg("HH_ALARM4") && server.hasArg("MM_ALARM4") && server.hasArg("SS_ALARM4")) {
    String alarm_h4 = server.arg("HH_ALARM4");
    String alarm_m4 = server.arg("MM_ALARM4");
    String alarm_s4 = server.arg("SS_ALARM4");
    alarm_hour4 = atoi(alarm_h4.c_str());
    alarm_min4 = atoi(alarm_m4.c_str());
    alarm_sec4 = atoi(alarm_s4.c_str());

    setCounter4 = 1;
    Serial.print("Alarm 4 set to: ");
    Serial.print(alarm_hour4);
    Serial.print(":");
    Serial.print(alarm_min4);
    Serial.print(":");
    Serial.println(alarm_sec4);

    server.send(200, "text/html", ALARM_SET);
  }

  if (server.hasArg("HH_ALARM5") && server.hasArg("MM_ALARM5") && server.hasArg("SS_ALARM5")) {
    String alarm_h5 = server.arg("HH_ALARM5");
    String alarm_m5 = server.arg("MM_ALARM5");
    String alarm_s5 = server.arg("SS_ALARM5");
    alarm_hour5 = atoi(alarm_h5.c_str());
    alarm_min5 = atoi(alarm_m5.c_str());
    alarm_sec5 = atoi(alarm_s5.c_str());
    
    setCounter5 = 1;
    Serial.print("Alarm 5 set to: ");
    Serial.print(alarm_hour5);
    Serial.print(":");
    Serial.print(alarm_min5);
    Serial.print(":");
    Serial.println(alarm_sec5);

    server.send(200, "text/html", ALARM_SET);
  }

  if (server.hasArg("HH_ALARM6") && server.hasArg("MM_ALARM6") && server.hasArg("SS_ALARM6")) {
    String alarm_h6 = server.arg("HH_ALARM6");
    String alarm_m6 = server.arg("MM_ALARM6");
    String alarm_s6 = server.arg("SS_ALARM6");
    alarm_hour6 = atoi(alarm_h6.c_str());
    alarm_min6 = atoi(alarm_m6.c_str());
    alarm_sec6 = atoi(alarm_s6.c_str());

    setCounter6 = 1;
    
    Serial.print("Alarm 6 set to: ");
    Serial.print(alarm_hour6);
    Serial.print(":");
    Serial.print(alarm_min6);
    Serial.print(":");
    Serial.println(alarm_sec6);

    server.send(200, "text/html", ALARM_SET);
  }

  if (server.hasArg("HH_ALARM7") && server.hasArg("MM_ALARM7") && server.hasArg("SS_ALARM7")) {
    String alarm_h7 = server.arg("HH_ALARM7");
    String alarm_m7 = server.arg("MM_ALARM7");
    String alarm_s7 = server.arg("SS_ALARM7");
    alarm_hour7 = atoi(alarm_h7.c_str());
    alarm_min7 = atoi(alarm_m7.c_str());
    alarm_sec7 = atoi(alarm_s7.c_str());

    setCounter7 = 1;
    
    Serial.print("Alarm 7 set to: ");
    Serial.print(alarm_hour7);
    Serial.print(":");
    Serial.print(alarm_min7);
    Serial.print(":");
    Serial.println(alarm_sec7);

    server.send(200, "text/html", ALARM_SET);
  }

  if (server.hasArg("HH_ALARM8") && server.hasArg("MM_ALARM8") && server.hasArg("SS_ALARM8")) {
    String alarm_h8 = server.arg("HH_ALARM8");
    String alarm_m8 = server.arg("MM_ALARM8");
    String alarm_s8 = server.arg("SS_ALARM8");
    alarm_hour8 = atoi(alarm_h8.c_str());
    alarm_min8 = atoi(alarm_m8.c_str());
    alarm_sec8 = atoi(alarm_s8.c_str());

    setCounter8 = 1;
    
    Serial.print("Alarm 8 set to: ");
    Serial.print(alarm_hour8);
    Serial.print(":");
    Serial.print(alarm_min8);
    Serial.print(":");
    Serial.println(alarm_sec8);

    server.send(200, "text/html", ALARM_SET);
  }

  if (server.hasArg("ALARM")) {
    server.send(200, "text/html", ALARM_FORM);
  }
  
  if (server.hasArg("RESET_DATE")) {
    // Update time before generating the status page
    updateTime();
    
    String statusPage = SHOW_DATE;
    statusPage.replace("@@H@@", String(Hor));
    String s = statusPage;
    s.replace("@@M@@", String(Min));
    String f = s;
    f.replace("@@S@@", String(Sec));
    s = f;
    s.replace("@@D@@", String(p_day));
    f = s;
    f.replace("@@MO@@", String(p_month));
    s = f;
    s.replace("@@Y@@", String(p_year));
    f = s;
    server.send(200, "text/html", f);
  }
  
  if (server.hasArg("HOME_AUTOMATION")) {
    server.send(200, "text/html", APPLIANCES);
  }
  
  if (server.hasArg("LED_ON")) {
    digitalWrite(ledPin, LOW);
    server.send(200, "text/html", APPLIANCES_ON);
  }

  if (server.hasArg("LED_OFF")) {
    digitalWrite(ledPin, HIGH);
    server.send(200, "text/html", APPLIANCES_OFF);
  }
  
  if (server.hasArg("ALARM_STATUS")) {
    String alarmStatus = STATUS_PAGE;
    
    if(setCounter1 == 1) {
      set1 = "SET";
    } else {
      set1 = "NOT SET";
    }
    
    if(setCounter2 == 1) {
      set2 = "SET";
    } else {
      set2 = "NOT SET";
    }
    
    if(setCounter3 == 1) {
      set3 = "SET";
    } else {
      set3 = "NOT SET";
    }
    
    if(setCounter4 == 1) {
      set4 = "SET";
    } else {
      set4 = "NOT SET";
    }

    if(setCounter5 == 1) {
      set5 = "SET";
    } else {
      set5 = "NOT SET";
    }

    if(setCounter6 == 1) {
      set6 = "SET";
    } else {
      set6 = "NOT SET";
    }

    if(setCounter7 == 1) {
      set7 = "SET";
    } else {
      set7 = "NOT SET";
    }
    
    if(setCounter8 == 1) {
      set8 = "SET";
    } else {
      set8 = "NOT SET";
    }

    alarmStatus.replace("@@SET1@@", set1);
    String s = alarmStatus;
    s.replace("@@SET2@@", set2);
    String f = s;
    f.replace("@@SET3@@", set3);
    s = f;
    s.replace("@@SET4@@", set4);
    f = s;
    f.replace("@@SET5@@", set5);
    s = f;
    s.replace("@@SET6@@", set6);
    f = s;             
    f.replace("@@SET7@@", set7);
    s = f;             
    s.replace("@@SET8@@", set8);
    f = s;             

server.send(200, "text/html", f);
  }
  
  server.send(200, "text/html", HOME_PAGE);
}

//no need authentification
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  // Initialize pins
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  digitalWrite(buzzerPin, HIGH);
  
  Serial.begin(115200);  // ESP32 typically uses higher baud rate
  
  // Initialize I2C
  Wire.begin();
  
  // Initialize LCD
  lcd. begin ();
  lcd.backlight();
  
  // Set up WiFi Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  
  // Initialize internal time (starting with default values)
  // You can set your own default time here
  setTime(2025, 4, 20, 12, 0, 0); // Year, Month, Day, Hour, Minute, Second
  
  // Display initial message on LCD
  lcd.setCursor(0, 0);
  lcd.print("SMART ALARM");
  delay(2000);
  lcd.clear();
  
  // Set up server routes
  server.on("/", handleRoot);
  server.on("/login", handleLogin);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works without need of authentification");
  });

  server.onNotFound(handleNotFound);
  
  // Here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent", "Cookie"};
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char*);
  // Ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize);
  
  server.begin();
  Serial.println("HTTP server started");
}

void alarm_on(void) {
  digitalWrite(buzzerPin, LOW);  // Turn on buzzer
  delay(5000);                  // Keep it on for 5 seconds
  digitalWrite(buzzerPin, HIGH); // Turn off buzzer
}

void loop(void) {
  // Update time from internal clock
  updateTime();
  
  // Print time to serial
  Serial.print("Time: ");
  Serial.print(Hor);
  Serial.print(":");
  Serial.print(Min);
  Serial.print(":");
  Serial.println(Sec);
  
  // Print date to serial
  Serial.print("Date: ");
  Serial.print(p_day);
  Serial.print("/");
  Serial.print(p_month);
  Serial.print("/");
  Serial.println(p_year);
  
  // Update LCD display
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(Hor);
  lcd.print(":");
  lcd.print(Min);
  lcd.print(":");
  lcd.print(Sec);
  
  lcd.setCursor(0, 1);
  lcd.print("Date: ");
  lcd.print(p_day);
  lcd.print("/");
  lcd.print(p_month);
  lcd.print("/");
  lcd.print(p_year);
  
  // Check for alarms
  if (Hor == alarm_hour1 && Min == alarm_min1) {
    Serial.println("1st Alarm");
    if (setCounter1 == 1) {
      set1 = "NOT SET";
      alarm_on();
      setCounter1 = 0;
    }
  }

  if (Hor == alarm_hour2 && Min == alarm_min2) {
    Serial.println("2nd Alarm");
    if (setCounter2 == 1) {
      set2 = "NOT SET";
      alarm_on();
      setCounter2 = 0;
    }
  }

  if (Hor == alarm_hour3 && Min == alarm_min3) {
    Serial.println("3rd Alarm");
    if (setCounter3 == 1) {
      set3 = "NOT SET";
      alarm_on();
      setCounter3 = 0;
    }
  }

  if (Hor == alarm_hour4 && Min == alarm_min4) {
    Serial.println("4th Alarm");
    if (setCounter4 == 1) {
      set4 = "NOT SET";
      alarm_on();
      setCounter4 = 0;
    }
  }

  if (Hor == alarm_hour5 && Min == alarm_min5) {
    Serial.println("5th Alarm");
    if (setCounter5 == 1) {
      set5 = "NOT SET";
      alarm_on();
      setCounter5 = 0;
    }
  }
  if (Hor == alarm_hour6 && Min == alarm_min6) {
    Serial.println("6th Alarm");
    if (setCounter6 == 1) {
      set6 = "NOT SET";
      alarm_on();
      setCounter6 = 0;
    }
  }
  if (Hor == alarm_hour7 && Min == alarm_min7) {
    Serial.println("7th Alarm");
    if (setCounter7 == 1) {
      set7 = "NOT SET";
      alarm_on();
      setCounter7 = 0;
    }
  }
  if (Hor == alarm_hour8 && Min == alarm_min8) {
    Serial.println("8th Alarm");
    if (setCounter8 == 1) {
      set8 = "NOT SET";
      alarm_on();
      setCounter8 = 0;
    }
  }

  server.handleClient();
  delay(1000);
}