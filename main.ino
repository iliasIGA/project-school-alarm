#include <WiFi.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>
#include "index.h"
#include <Preferences.h>
#include <RTClib.h>

const int SDA_PIN = 21;  // GPIO21 for SDA
const int SCL_PIN = 22;  // GPIO22 for SCL

// I2C LCD setup (0x27 is the default I2C address, adjust if needed)
// Parameters: I2C address, columns, rows
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;

// NTP Server settings
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 0;      // Change this based on your timezone (in seconds)
const int   daylightOffset_sec = 3600; // Change this for daylight saving time if needed
Preferences preferences;

// WiFi credentials
const char* ssid = "Alarm";
const char* password = "Alarm1234";

// Current day of week
String currentDay = "Monday";
int currentDayNum = 1; // 0=Sunday, 1=Monday, etc.

// Selected day for alarm setting
String selectedDay = "monday";

// Day-specific alarm status structure
struct DayAlarms {
  int setCounter1;
  int setCounter2;
  int setCounter3;
  int setCounter4;
  int setCounter5;
  int setCounter6;
  int setCounter7;
  int setCounter8;
  String set1;
  String set2;
  String set3;
  String set4;
  String set5;
  String set6;
  String set7;
  String set8;
  String name1;  // Added alarm names
  String name2;
  String name3;
  String name4;
  String name5;
  String name6;
  String name7;
  String name8;
  // Add actual time storage for each alarm
  int hour1, min1, sec1;
  int hour2, min2, sec2;
  int hour3, min3, sec3;
  int hour4, min4, sec4;
  int hour5, min5, sec5;
  int hour6, min6, sec6;
  int hour7, min7, sec7;
  int hour8, min8, sec8;
};

// Function pre-declarations

void updateAlarmsFromForm(DayAlarms &alarms);

// Create alarm status for each day
DayAlarms mondayAlarms = {0};
DayAlarms tuesdayAlarms = {0};
DayAlarms wednesdayAlarms = {0};
DayAlarms thursdayAlarms = {0};
DayAlarms fridayAlarms = {0};
DayAlarms saturdayAlarms = {0};
DayAlarms sundayAlarms = {0};

// Selected day for status view (this is different from alarm setting)
String statusSelectedDay = "monday";

// Legacy alarm status counters (keeping for backward compatibility)
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
int day_of_week;

// Alarm time variables (keeping for backward compatibility)
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
const int ledPin = 26;     // GPIO27 for LED

// Create web server on port 80
WebServer server(80);

// Function to get day of week as string
String getDayOfWeek(int dayNum) {
  switch(dayNum) {
    case 0: return "Sunday";
    case 1: return "Monday";
    case 2: return "Tuesday";
    case 3: return "Wednesday";
    case 4: return "Thursday";
    case 5: return "Friday";
    case 6: return "Saturday";
    default: return "Unknown";
  }
}

// Function to get day of week as number from string
int getDayOfWeekNum(String day) {
  day.toLowerCase();
  if (day == "sunday") return 0;
  if (day == "monday") return 1;
  if (day == "tuesday") return 2;
  if (day == "wednesday") return 3;
  if (day == "thursday") return 4;
  if (day == "friday") return 5;
  if (day == "saturday") return 6;
  return -1; // Invalid day
}

// Function to update time from internal clock
void updateTime() {
  DateTime now = rtc.now();
  Hor = now.hour();
  Min = now.minute();
  Sec = now.second();
  p_day = now.day();
  p_month = now.month();
  p_year = now.year();
  day_of_week = now.dayOfTheWeek(); // 0=Sunday
  
  currentDay = getDayOfWeek(day_of_week);
  currentDayNum = day_of_week;
}

void saveAlarmsToFlash() {
  preferences.begin("alarms", false);
  
  // Save alarms for each day
  saveDayAlarms("monday", mondayAlarms);
  saveDayAlarms("tuesday", tuesdayAlarms);
  saveDayAlarms("wednesday", wednesdayAlarms);
  saveDayAlarms("thursday", thursdayAlarms);
  saveDayAlarms("friday", fridayAlarms);
  saveDayAlarms("saturday", saturdayAlarms);
  saveDayAlarms("sunday", sundayAlarms);
  
  preferences.end();
}

void saveDayAlarms(const char* dayName, DayAlarms &alarms) {
  String prefix = dayName;
  preferences.putInt((prefix + "_cnt1").c_str(), alarms.setCounter1);
  preferences.putInt((prefix + "_cnt2").c_str(), alarms.setCounter2);
  preferences.putInt((prefix + "_cnt3").c_str(), alarms.setCounter3);
  preferences.putInt((prefix + "_cnt4").c_str(), alarms.setCounter4);
  preferences.putInt((prefix + "_cnt5").c_str(), alarms.setCounter5);
  preferences.putInt((prefix + "_cnt6").c_str(), alarms.setCounter6);
  preferences.putInt((prefix + "_cnt7").c_str(), alarms.setCounter7);
  preferences.putInt((prefix + "_cnt8").c_str(), alarms.setCounter8);

  preferences.putString((prefix + "_time1").c_str(), alarms.set1);
  preferences.putString((prefix + "_time2").c_str(), alarms.set2);
  preferences.putString((prefix + "_time3").c_str(), alarms.set3);
  preferences.putString((prefix + "_time4").c_str(), alarms.set4);
  preferences.putString((prefix + "_time5").c_str(), alarms.set5);
  preferences.putString((prefix + "_time6").c_str(), alarms.set6);
  preferences.putString((prefix + "_time7").c_str(), alarms.set7);
  preferences.putString((prefix + "_time8").c_str(), alarms.set8);
  
  preferences.putString((prefix + "_name1").c_str(), alarms.name1);
  preferences.putString((prefix + "_name2").c_str(), alarms.name2);
  preferences.putString((prefix + "_name3").c_str(), alarms.name3);
  preferences.putString((prefix + "_name4").c_str(), alarms.name4);
  preferences.putString((prefix + "_name5").c_str(), alarms.name5);
  preferences.putString((prefix + "_name6").c_str(), alarms.name6);
  preferences.putString((prefix + "_name7").c_str(), alarms.name7);
  preferences.putString((prefix + "_name8").c_str(), alarms.name8);
  
}

void loadAlarmsFromFlash() {
  preferences.begin("alarms", true);
  
  loadDayAlarms("monday", mondayAlarms);
  loadDayAlarms("tuesday", tuesdayAlarms);
  loadDayAlarms("wednesday", wednesdayAlarms);
  loadDayAlarms("thursday", thursdayAlarms);
  loadDayAlarms("friday", fridayAlarms);
  loadDayAlarms("saturday", saturdayAlarms);
  loadDayAlarms("sunday", sundayAlarms);
  
  preferences.end();
}

void loadDayAlarms(const char* dayName, DayAlarms &alarms) {
  String prefix = dayName;
  alarms.setCounter1 = preferences.getInt((prefix + "_cnt1").c_str(), 0);
  alarms.setCounter2 = preferences.getInt((prefix + "_cnt2").c_str(), 0);
  alarms.setCounter3 = preferences.getInt((prefix + "_cnt3").c_str(), 0);
  alarms.setCounter4 = preferences.getInt((prefix + "_cnt4").c_str(), 0);
  alarms.setCounter5 = preferences.getInt((prefix + "_cnt5").c_str(), 0);
  alarms.setCounter6 = preferences.getInt((prefix + "_cnt6").c_str(), 0);
  alarms.setCounter7 = preferences.getInt((prefix + "_cnt7").c_str(), 0);
  alarms.setCounter8 = preferences.getInt((prefix + "_cnt8").c_str(), 0);
  
  alarms.set1 = preferences.getString((prefix + "_time1").c_str(), "");
  alarms.set2 = preferences.getString((prefix + "_time2").c_str(), "");
  alarms.set3 = preferences.getString((prefix + "_time3").c_str(), "");
  alarms.set4 = preferences.getString((prefix + "_time4").c_str(), "");
  alarms.set5 = preferences.getString((prefix + "_time5").c_str(), "");
  alarms.set6 = preferences.getString((prefix + "_time6").c_str(), "");
  alarms.set7 = preferences.getString((prefix + "_time7").c_str(), "");
  alarms.set8 = preferences.getString((prefix + "_time8").c_str(), "");
  
  alarms.name1 = preferences.getString((prefix + "_name1").c_str(), "");
  alarms.name2 = preferences.getString((prefix + "_name2").c_str(), "");
  alarms.name3 = preferences.getString((prefix + "_name3").c_str(), "");
  alarms.name4 = preferences.getString((prefix + "_name4").c_str(), "");
  alarms.name5 = preferences.getString((prefix + "_name5").c_str(), "");
  alarms.name6 = preferences.getString((prefix + "_name6").c_str(), "");
  alarms.name7 = preferences.getString((prefix + "_name7").c_str(), "");
  alarms.name8 = preferences.getString((prefix + "_name8").c_str(), "");

}

// Function to set the internal clock
void setTime(int year, int month, int day, int hour, int minute, int second) {
  rtc.adjust(DateTime(year, month, day, hour, minute, second));
  // The ESP32 internal time will be updated through updateTime() 
}

// Helper function to get DayAlarms pointer for a given day
DayAlarms* getDayAlarms(String day) {
  day.toLowerCase();
  if (day == "monday") {
    return &mondayAlarms;
  } else if (day == "tuesday") {
    return &tuesdayAlarms;
  } else if (day == "wednesday") {
    return &wednesdayAlarms;
  } else if (day == "thursday") {
    return &thursdayAlarms;
  } else if (day == "friday") {
    return &fridayAlarms;
  } else if (day == "saturday") {
    return &saturdayAlarms;
  } else if (day == "sunday") {
    return &sundayAlarms;
  } else {
    return &mondayAlarms; // Default to Monday if invalid
  }
}

// Helper function to get DayAlarms pointer based on day number
DayAlarms* getDayAlarmsByNum(int dayNum) {
  switch(dayNum) {
    case 0: return &sundayAlarms;
    case 1: return &mondayAlarms;
    case 2: return &tuesdayAlarms;
    case 3: return &wednesdayAlarms;
    case 4: return &thursdayAlarms;
    case 5: return &fridayAlarms;
    case 6: return &saturdayAlarms;
    default: return &mondayAlarms; // Default to Monday if invalid
  }
}

// Function to copy alarms from one day to another
void copyAlarms(DayAlarms &source, DayAlarms &destination) {
  destination = source;
}

// Function to update alarms based on form input
void updateAlarmsFromForm(DayAlarms &alarms) {
  if (server.hasArg("NAME_ALARM1")) alarms.name1 = server.arg("NAME_ALARM1");
  if (server.hasArg("NAME_ALARM2")) alarms.name2 = server.arg("NAME_ALARM2");
  if (server.hasArg("NAME_ALARM3")) alarms.name3 = server.arg("NAME_ALARM3");
  if (server.hasArg("NAME_ALARM4")) alarms.name4 = server.arg("NAME_ALARM4");
  if (server.hasArg("NAME_ALARM5")) alarms.name5 = server.arg("NAME_ALARM5");
  if (server.hasArg("NAME_ALARM6")) alarms.name6 = server.arg("NAME_ALARM6");
  if (server.hasArg("NAME_ALARM7")) alarms.name7 = server.arg("NAME_ALARM7");
  if (server.hasArg("NAME_ALARM8")) alarms.name8 = server.arg("NAME_ALARM8");

  if (server.hasArg("HH_ALARM1") && server.hasArg("MM_ALARM1") && server.hasArg("SS_ALARM1")) {
    alarms.set1 = server.arg("HH_ALARM1") + ":" + server.arg("MM_ALARM1") + ":" + server.arg("SS_ALARM1");
    alarms.hour1 = atoi(server.arg("HH_ALARM1").c_str());
    alarms.min1 = atoi(server.arg("MM_ALARM1").c_str());
    alarms.sec1 = atoi(server.arg("SS_ALARM1").c_str());
    alarms.setCounter1 = 1;
  }
  
  if (server.hasArg("HH_ALARM2") && server.hasArg("MM_ALARM2") && server.hasArg("SS_ALARM2")) {
    alarms.set2 = server.arg("HH_ALARM2") + ":" + server.arg("MM_ALARM2") + ":" + server.arg("SS_ALARM2");
    alarms.hour2 = atoi(server.arg("HH_ALARM2").c_str());
    alarms.min2 = atoi(server.arg("MM_ALARM2").c_str());
    alarms.sec2 = atoi(server.arg("SS_ALARM2").c_str());
    alarms.setCounter2 = 1;
  }
  
  if (server.hasArg("HH_ALARM3") && server.hasArg("MM_ALARM3") && server.hasArg("SS_ALARM3")) {
    alarms.set3 = server.arg("HH_ALARM3") + ":" + server.arg("MM_ALARM3") + ":" + server.arg("SS_ALARM3");
    alarms.hour3 = atoi(server.arg("HH_ALARM3").c_str());
    alarms.min3 = atoi(server.arg("MM_ALARM3").c_str());
    alarms.sec3 = atoi(server.arg("SS_ALARM3").c_str());
    alarms.setCounter3 = 1;  
  }
  
  if (server.hasArg("HH_ALARM4") && server.hasArg("MM_ALARM4") && server.hasArg("SS_ALARM4")) {
    alarms.set4 = server.arg("HH_ALARM4") + ":" + server.arg("MM_ALARM4") + ":" + server.arg("SS_ALARM4");
    alarms.hour4 = atoi(server.arg("HH_ALARM4").c_str());
    alarms.min4 = atoi(server.arg("MM_ALARM4").c_str());
    alarms.sec4 = atoi(server.arg("SS_ALARM4").c_str());
    alarms.setCounter4 = 1;
  }
  
  if (server.hasArg("HH_ALARM5") && server.hasArg("MM_ALARM5") && server.hasArg("SS_ALARM5")) {
    alarms.set5 = server.arg("HH_ALARM5") + ":" + server.arg("MM_ALARM5") + ":" + server.arg("SS_ALARM5");
    alarms.hour5 = atoi(server.arg("HH_ALARM5").c_str());
    alarms.min5 = atoi(server.arg("MM_ALARM5").c_str());
    alarms.sec5 = atoi(server.arg("SS_ALARM5").c_str());
    alarms.setCounter5 = 1;
  }
  
  if (server.hasArg("HH_ALARM6") && server.hasArg("MM_ALARM6") && server.hasArg("SS_ALARM6")) {
    alarms.set6 = server.arg("HH_ALARM6") + ":" + server.arg("MM_ALARM6") + ":" + server.arg("SS_ALARM6");
    alarms.hour6 = atoi(server.arg("HH_ALARM6").c_str());
    alarms.min6 = atoi(server.arg("MM_ALARM6").c_str());
    alarms.sec6 = atoi(server.arg("SS_ALARM6").c_str());
    alarms.setCounter6 = 1;
  }
  
  if (server.hasArg("HH_ALARM7") && server.hasArg("MM_ALARM7") && server.hasArg("SS_ALARM7")) {
    alarms.set7 = server.arg("HH_ALARM7") + ":" + server.arg("MM_ALARM7") + ":" + server.arg("SS_ALARM7");
    alarms.hour7 = atoi(server.arg("HH_ALARM7").c_str());
    alarms.min7 = atoi(server.arg("MM_ALARM7").c_str());
    alarms.sec7 = atoi(server.arg("SS_ALARM7").c_str());
    alarms.setCounter7 = 1;
  }
  
  if (server.hasArg("HH_ALARM8") && server.hasArg("MM_ALARM8") && server.hasArg("SS_ALARM8")) {
    alarms.set8 = server.arg("HH_ALARM8") + ":" + server.arg("MM_ALARM8") + ":" + server.arg("SS_ALARM8");
    alarms.hour8 = atoi(server.arg("HH_ALARM8").c_str());
    alarms.min8 = atoi(server.arg("MM_ALARM8").c_str());
    alarms.sec8 = atoi(server.arg("SS_ALARM8").c_str());
    alarms.setCounter8 = 1;
  }
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
  if (server.hasArg("LOGOUT")) {
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

  if (server.hasArg("COPY_ALARMS")) {
    String sourceDay = server.arg("SOURCE_DAY");
    DayAlarms* sourceDayAlarms = getDayAlarms(sourceDay);
    
    // Copy to all days except the source day
    if (sourceDay != "monday") copyAlarms(*sourceDayAlarms, mondayAlarms);
    if (sourceDay != "tuesday") copyAlarms(*sourceDayAlarms, tuesdayAlarms);
    if (sourceDay != "wednesday") copyAlarms(*sourceDayAlarms, wednesdayAlarms);
    if (sourceDay != "thursday") copyAlarms(*sourceDayAlarms, thursdayAlarms);
    if (sourceDay != "friday") copyAlarms(*sourceDayAlarms, fridayAlarms);
    if (sourceDay != "saturday") copyAlarms(*sourceDayAlarms, saturdayAlarms);
    if (sourceDay != "sunday") copyAlarms(*sourceDayAlarms, sundayAlarms);

    Serial.print("Copied alarms from ");
    Serial.print(sourceDay);
    Serial.println(" to all other days");
    
    server.send(200, "text/html", ALARM_COPY);
    return;
  }

  // Set alarms for specific day
  if (server.hasArg("DAY_SELECTED")) {
    selectedDay = server.arg("DAY_SELECTED");
    selectedDay.toLowerCase(); // Ensure lowercase for comparison

    DayAlarms* dayAlarms = getDayAlarms(selectedDay);
    updateAlarmsFromForm(*dayAlarms);

    // For backward compatibility - store the alarm times for the first alarm
    if (selectedDay == "monday" && server.hasArg("HH_ALARM1")) {
      alarm_hour1 = dayAlarms->hour1;
      alarm_min1 = dayAlarms->min1;
      alarm_sec1 = dayAlarms->sec1;
    }

    String response = ALARM_SET;
    response.replace("@@DAY_SET@@", selectedDay);
    server.send(200, "text/html", response);
    return;
  }

  // Handle individual alarm settings (keeping for backward compatibility)
  if (server.hasArg("HH_ALARM1") && server.hasArg("MM_ALARM1") && server.hasArg("SS_ALARM1")) {
    String alarm_h1 = server.arg("HH_ALARM1");
    String alarm_m1 = server.arg("MM_ALARM1");
    String alarm_s1 = server.arg("SS_ALARM1");
    alarm_hour1 = atoi(alarm_h1.c_str());
    alarm_min1 = atoi(alarm_m1.c_str());
    alarm_sec1 = atoi(alarm_s1.c_str());
    
    // Get alarm name if provided
    String alarmName = "Alarm 1";  // Default name
    if (server.hasArg("NAME_ALARM1")) {
      alarmName = server.arg("NAME_ALARM1");
    }
    
    // Update the setCounter and name for the appropriate day
    DayAlarms* dayAlarms = getDayAlarms(selectedDay);
    dayAlarms->setCounter1 = 1;
    dayAlarms->name1 = alarmName;
    dayAlarms->hour1 = alarm_hour1;
    dayAlarms->min1 = alarm_min1;
    dayAlarms->sec1 = alarm_sec1;
    
    // Keep this for backward compatibility
    setCounter1 = 1;
    
    Serial.print("Alarm 1 set to: ");
    Serial.print(alarm_hour1);
    Serial.print(":");
    Serial.print(alarm_min1);
    Serial.print(":");
    Serial.println(alarm_sec1);
    Serial.print("Name: ");
    Serial.println(alarmName);
    Serial.print("For day: ");
    Serial.println(selectedDay);
  
    String response = ALARM_SET;
    response.replace("@@DAY_SET@@", selectedDay);
    server.send(200, "text/html", response);
    return;
  }

  if (server.hasArg("RESET_DATE")) {
    String timePage = SHOW_DATE;
    timePage.replace("@@H@@", String(Hor));
    timePage.replace("@@M@@", String(Min));
    timePage.replace("@@S@@", String(Sec));
    timePage.replace("@@D@@", String(p_day));
    timePage.replace("@@MO@@", String(p_month));
    timePage.replace("@@Y@@", String(p_year));
    timePage.replace("@@CURRENT_DAY@@", currentDay);
    server.send(200, "text/html", timePage);
    return;
  }

  // Handle Appliance Control Page
  if (server.hasArg("HOME_AUTOMATION")) {
    server.send(200, "text/html", APPLIANCES);
    return;
  }

  // Handle Appliance Actions
  if (server.hasArg("LED_ON")) {
    digitalWrite(ledPin, LOW); // Turn ON appliance
    server.send(200, "text/html", APPLIANCES_ON);
    return;
  }
  
  if (server.hasArg("LED_OFF")) {
    digitalWrite(ledPin, HIGH); // Turn OFF appliance
    server.send(200, "text/html", APPLIANCES_OFF);
    return;
  }

  
  // Similar implementations for alarms 2-8 would go here but are omitted for brevity
  // Make sure to update both the day-specific structure and legacy variables

  // Handle time setting
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
    return;
  }

  // Handle ALARM_STATUS request
  if (server.hasArg("ALARM_STATUS")) {
    // Update time to get current day
    updateTime();
    
    // Check if a day is selected in the dropdown
    if (server.hasArg("DAY_SELECT")) {
      statusSelectedDay = server.arg("DAY_SELECT");
      statusSelectedDay.toLowerCase(); // Ensure lowercase for consistency
      Serial.print("Status selected day: ");
      Serial.println(statusSelectedDay);
    }
    
    String alarmStatus = STATUS_PAGE;
    
    // Get the appropriate day's alarms
    DayAlarms* selectedAlarms = getDayAlarms(statusSelectedDay);
    String displayDay = getDayOfWeek(getDayOfWeekNum(statusSelectedDay));
    
    // Set the "selected" attribute for the appropriate day in dropdown
    if (statusSelectedDay == "monday") {
      alarmStatus.replace("@@MON_SELECTED@@", "selected");
    } else if (statusSelectedDay == "tuesday") {
      alarmStatus.replace("@@TUE_SELECTED@@", "selected");
    } else if (statusSelectedDay == "wednesday") {
      alarmStatus.replace("@@WED_SELECTED@@", "selected");
    } else if (statusSelectedDay == "thursday") {
      alarmStatus.replace("@@THU_SELECTED@@", "selected");
    } else if (statusSelectedDay == "friday") {
      alarmStatus.replace("@@FRI_SELECTED@@", "selected");
    } else if (statusSelectedDay == "saturday") {
      alarmStatus.replace("@@SAT_SELECTED@@", "selected");
    } else if (statusSelectedDay == "sunday") {
      alarmStatus.replace("@@SUN_SELECTED@@", "selected");
    }
    
    // Clear all selected markers not set above
    alarmStatus.replace("@@MON_SELECTED@@", "");
    alarmStatus.replace("@@TUE_SELECTED@@", "");
    alarmStatus.replace("@@WED_SELECTED@@", "");
    alarmStatus.replace("@@THU_SELECTED@@", "");
    alarmStatus.replace("@@FRI_SELECTED@@", "");
    alarmStatus.replace("@@SAT_SELECTED@@", "");
    alarmStatus.replace("@@SUN_SELECTED@@", "");
    
    // Update alarm status strings for the selected day
    
  alarmStatus.replace("@@CURRENT_DAY@@", currentDay);
  alarmStatus.replace("@@SELECTED_DAY@@", displayDay);
  alarmStatus.replace("@@SET1@@", (selectedAlarms->setCounter1 == 1) ? selectedAlarms->set1 : "NOT SET");
  alarmStatus.replace("@@SET2@@", (selectedAlarms->setCounter2 == 1) ? selectedAlarms->set2 : "NOT SET");
  alarmStatus.replace("@@SET3@@", (selectedAlarms->setCounter3 == 1) ? selectedAlarms->set3 : "NOT SET");
  alarmStatus.replace("@@SET4@@", (selectedAlarms->setCounter4 == 1) ? selectedAlarms->set4 : "NOT SET");
  alarmStatus.replace("@@SET5@@", (selectedAlarms->setCounter5 == 1) ? selectedAlarms->set5 : "NOT SET");
  alarmStatus.replace("@@SET6@@", (selectedAlarms->setCounter6 == 1) ? selectedAlarms->set6 : "NOT SET");
  alarmStatus.replace("@@SET7@@", (selectedAlarms->setCounter7 == 1) ? selectedAlarms->set7 : "NOT SET");
  alarmStatus.replace("@@SET8@@", (selectedAlarms->setCounter8 == 1) ? selectedAlarms->set8 : "NOT SET");

    // Replace placeholders with actual values
    alarmStatus.replace("@@CURRENT_DAY@@", currentDay);
    alarmStatus.replace("@@SELECTED_DAY@@", displayDay);
    alarmStatus.replace("@@SET1@@", selectedAlarms->set1);
    alarmStatus.replace("@@SET2@@", selectedAlarms->set2);
    alarmStatus.replace("@@SET3@@", selectedAlarms->set3);
    alarmStatus.replace("@@SET4@@", selectedAlarms->set4);
    alarmStatus.replace("@@SET5@@", selectedAlarms->set5);
    alarmStatus.replace("@@SET6@@", selectedAlarms->set6);
    alarmStatus.replace("@@SET7@@", selectedAlarms->set7);
    alarmStatus.replace("@@SET8@@", selectedAlarms->set8);

    // Add name replacements
    alarmStatus.replace("@@NAME1@@", selectedAlarms->name1.length() > 0 ? selectedAlarms->name1 : "Alarm 1");
    alarmStatus.replace("@@NAME2@@", selectedAlarms->name2.length() > 0 ? selectedAlarms->name2 : "Alarm 2");
    alarmStatus.replace("@@NAME3@@", selectedAlarms->name3.length() > 0 ? selectedAlarms->name3 : "Alarm 3");
    alarmStatus.replace("@@NAME4@@", selectedAlarms->name4.length() > 0 ? selectedAlarms->name4 : "Alarm 4");
    alarmStatus.replace("@@NAME5@@", selectedAlarms->name5.length() > 0 ? selectedAlarms->name5 : "Alarm 5");
    alarmStatus.replace("@@NAME6@@", selectedAlarms->name6.length() > 0 ? selectedAlarms->name6 : "Alarm 6");
    alarmStatus.replace("@@NAME7@@", selectedAlarms->name7.length() > 0 ? selectedAlarms->name7 : "Alarm 7");
    alarmStatus.replace("@@NAME8@@", selectedAlarms->name8.length() > 0 ? selectedAlarms->name8 : "Alarm 8");
    
    // Handle the extra alarm display based on day (Friday has fewer alarms)
    if (statusSelectedDay == "friday") {
      alarmStatus.replace("@@EXTRA_DISPLAY@@", "none");
    } else {
      alarmStatus.replace("@@EXTRA_DISPLAY@@", "block");
    }
    
    server.send(200, "text/html", alarmStatus);
    return;
  }
  
  // Handle other pages
  if (server.hasArg("ALARM")) {
    server.send(200, "text/html", ALARM_FORM);
    return;
  }

  if (server.hasArg("SHOW_COPY")) {
    // Create a copy page with the dropdown pre-populated with the current selected day
    String copyPage = COPY_FORM;
    
    // Set the source day dropdown to show the currently selected day
    if (selectedDay == "monday") {
      copyPage.replace("@@MON_SELECTED@@", "selected");
    } else if (selectedDay == "tuesday") {
      copyPage.replace("@@TUE_SELECTED@@", "selected");
    } else if (selectedDay == "wednesday") {
      copyPage.replace("@@WED_SELECTED@@", "selected");
    } else if (selectedDay == "thursday") {
      copyPage.replace("@@THU_SELECTED@@", "selected");
    } else if (selectedDay == "friday") {
      copyPage.replace("@@FRI_SELECTED@@", "selected");
    } else if (selectedDay == "saturday") {
      copyPage.replace("@@SAT_SELECTED@@", "selected");
    } else if (selectedDay == "sunday") {
      copyPage.replace("@@SUN_SELECTED@@", "selected");
    }
    
    // Clear any unused placeholders
    copyPage.replace("@@MON_SELECTED@@", "");
    copyPage.replace("@@TUE_SELECTED@@", "");
    copyPage.replace("@@WED_SELECTED@@", "");
    copyPage.replace("@@THU_SELECTED@@", "");
    copyPage.replace("@@FRI_SELECTED@@", "");
    copyPage.replace("@@SAT_SELECTED@@", "");
    copyPage.replace("@@SUN_SELECTED@@", "");

    server.send(200, "text/html", copyPage);
    return;
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
  
  Wire.begin(SDA_PIN, SCL_PIN);
  // Initialize RTC with retries
   // Initialize RTC with proper error handling
   if (!rtc.begin()) {
    Serial.println("Couldn't find RTC!");
    Serial.println("Checking I2C connections...");
    while (1) {
      // Blink LED to indicate error
      digitalWrite(ledPin, !digitalRead(ledPin));
      delay(500);
    }
  }

  // Check if RTC lost power
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting to compile time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Add I2C scanner for debugging
  Serial.println("Scanning I2C devices...");
  byte error, address;
  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("Found device at 0x");
      Serial.println(address, HEX);
    }
  }

  // Initialize pins
  pinMode(ledPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  digitalWrite(buzzerPin, HIGH);

 // Initialize RTC with retries
 bool rtcStarted = false;
 for(int i=0; i<5; i++) {
   if (rtc.begin()) {
     rtcStarted = true;
     break;
   }
   Serial.println("RTC not found, retrying...");
   delay(1000);
 }
 
 if (!rtcStarted) {
   Serial.println("Couldn't find RTC. Check wiring!");
   while(1); // Halt if RTC fails
 }

 if (rtc.lostPower()) {
   // Set with compile time or NTP time
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
 }

  preferences.begin("alarms", false);
  loadAlarmsFromFlash(); // Load saved alarms on boot
  preferences.end();

  Serial.begin(115200);  // ESP32 typically uses higher baud rate
  
  // Initialize I2C
  Wire.begin(SDA_PIN, SCL_PIN);
  
  // Initialize LCD
  lcd.begin();
  lcd.backlight();
  
  // Set up WiFi Access Point
  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);
  
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  
  
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
void alarm_on(String alarmName) {
  // Display the alarm name on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ALARM ACTIVE:");
  lcd.setCursor(0, 1);
  lcd.print(alarmName);
  
  // Debug output
  Serial.print("Alarm triggered: ");
  Serial.println(alarmName);
  
  // Sound the buzzer
  digitalWrite(buzzerPin, LOW);  // Turn on buzzer
  
  // Flash the LED to make it more noticeable
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin, LOW);  // Turn on LED
    delay(500);
    digitalWrite(ledPin, HIGH); // Turn off LED
    delay(500);
  }
  
  digitalWrite(buzzerPin, HIGH); // Turn off buzzer
}

unsigned long alarmStartTime = 0;
bool alarmActive = false;
int alarmState = 0;
String currentAlarmName = "";

void startAlarm(String alarmName) {
  alarmActive = true;
  alarmStartTime = millis();
  alarmState = 0;
  currentAlarmName = alarmName;
  
  // Display on LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("ALARM ACTIVE:");
  lcd.setCursor(0, 1);
  lcd.print(alarmName);
  
  // Debug output
  Serial.print("Alarm triggered: ");
  Serial.println(alarmName);
  
  // Turn on buzzer
  digitalWrite(buzzerPin, LOW);  // Active LOW - change to HIGH if your buzzer is active HIGH
}

void handleAlarm() {
  if (!alarmActive) return;
  
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - alarmStartTime;
  
  // Total alarm duration: 5 seconds
  if (elapsedTime > 5000) {
    // Turn off buzzer and end alarm
    digitalWrite(buzzerPin, HIGH);  // Active LOW - change to HIGH if your buzzer is active HIGH
    digitalWrite(ledPin, HIGH);     // Ensure LED is off
    alarmActive = false;
    return;
  }
}

void loop(void) {

  // Handle server requests
  server.handleClient();
  
  // If an alarm is active, handle it
  if (alarmActive) {
    handleAlarm();
  }

  // Update time from internal clock
  updateTime();

  // Update LCD only every second (not on every cycle)
  static unsigned long lastDisplayUpdate = 0;
  if (millis() - lastDisplayUpdate >= 1000) {
    lastDisplayUpdate = millis();
  
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
}
  
   // Check for alarms - only if no alarm is currently active and seconds are 0
   if (!alarmActive && Sec == 0) {
    // Get the current day's alarms structure
    DayAlarms* currentDayAlarms = getDayAlarmsByNum(currentDayNum);
    
    // Check alarms using the current day's alarm times, not the global variables
    if (currentDayAlarms->setCounter1 == 1 && 
        Hor == currentDayAlarms->hour1 && 
        Min == currentDayAlarms->min1) {
      Serial.println("1st Alarm");
      String name = currentDayAlarms->name1.length() > 0 ? currentDayAlarms->name1 : "Alarm 1";
      startAlarm(name);
    }
    
    else if (currentDayAlarms->setCounter2 == 1 && 
             Hor == currentDayAlarms->hour2 && 
             Min == currentDayAlarms->min2) {
      Serial.println("2nd Alarm");
      String name = currentDayAlarms->name2.length() > 0 ? currentDayAlarms->name2 : "Alarm 2";
      startAlarm(name);
    }
    
    else if (currentDayAlarms->setCounter3 == 1 && 
             Hor == currentDayAlarms->hour3 && 
             Min == currentDayAlarms->min3) {
      Serial.println("3rd Alarm");
      String name = currentDayAlarms->name3.length() > 0 ? currentDayAlarms->name3 : "Alarm 3";
      startAlarm(name);
    }
    
    else if (currentDayAlarms->setCounter4 == 1 && 
             Hor == currentDayAlarms->hour4 && 
             Min == currentDayAlarms->min4) {
      Serial.println("4th Alarm");
      String name = currentDayAlarms->name4.length() > 0 ? currentDayAlarms->name4 : "Alarm 4";
      startAlarm(name);
    }
    
    // Only check additional alarms if it's not Friday
    if (!currentDay.equalsIgnoreCase("Friday")) {
      if (currentDayAlarms->setCounter5 == 1 && 
          Hor == currentDayAlarms->hour5 && 
          Min == currentDayAlarms->min5) {
        Serial.println("5th Alarm");
        String name = currentDayAlarms->name5.length() > 0 ? currentDayAlarms->name5 : "Alarm 5";
        startAlarm(name);
      }
      
      else if (currentDayAlarms->setCounter6 == 1 && 
               Hor == currentDayAlarms->hour6 && 
               Min == currentDayAlarms->min6) {
        Serial.println("6th Alarm");
        String name = currentDayAlarms->name6.length() > 0 ? currentDayAlarms->name6 : "Alarm 6";
        startAlarm(name);
      }
      
      else if (currentDayAlarms->setCounter7 == 1 && 
               Hor == currentDayAlarms->hour7 && 
               Min == currentDayAlarms->min7) {
        Serial.println("7th Alarm");
        String name = currentDayAlarms->name7.length() > 0 ? currentDayAlarms->name7 : "Alarm 7";
        startAlarm(name);
      }
      
      else if (currentDayAlarms->setCounter8 == 1 && 
               Hor == currentDayAlarms->hour8 && 
               Min == currentDayAlarms->min8) {
        Serial.println("8th Alarm");
        String name = currentDayAlarms->name8.length() > 0 ? currentDayAlarms->name8 : "Alarm 8";
        startAlarm(name);
      }
    }
  }

  server.handleClient();
  delay(50);
}

