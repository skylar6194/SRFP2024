#include <Arduino.h>
//#include <WiFi.h>               //we are using the ESP32
//#include <ESP8266WiFi.h>  
#include "DFRobot_MICS.h"
#include <Wire.h>    // uncomment this line if you are using esp8266 and comment the line above
#include <Firebase_ESP_Client.h>

#define Mics_I2C_ADDRESS 0x75
DFRobot_MICS_I2C mics(&Wire, Mics_I2C_ADDRESS);


//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "WIFI-SSID"
#define WIFI_PASSWORD "PASS"

// Insert Firebase project API Key
#define API_KEY "XX__KEY_XX"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://mask-icmr-default-rtdb.asia-southeast1.firebasedatabase.app/" 

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;                     //since we are doing an anonymous sign in 

void setup(){


  Wire.begin();
  Serial.begin(9600);
  while(!Serial);
  while(!mics.begin()){
    Serial.println("NO Devices !");
    delay(5000);
  } Serial.println("Device connected successfully !");




  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
  delay(1000);}

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  uint8_t mode = mics.getPowerState();
  if(mode == SLEEP_MODE){
    mics.wakeUpMode();
    Serial.println("wake up sensor success!");}
  else{
    Serial.println("The sensor is wake up mode");}

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("Database Connected");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

}

void loop(){
  
 float CO_data=0;
 float CH4_data=0;
 float C2H5OH_data=0;
 float NO2_data=0;
 float NH3_data=0;
 float H2_data=0;

  
  CO_data = mics.getGasData(CO);
  

  CH4_data=mics.getGasData(CH4);
  

  C2H5OH_data=mics.getGasData(C2H5OH);
  

  NO2_data=mics.getGasData(NO2);
  

  NH3_data=mics.getGasData(NH3);
  

  H2_data=mics.getGasData(H2);
  

  
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 3000 || sendDataPrevMillis == 0)){
    
    sendDataPrevMillis = millis();
    
    if (Firebase.RTDB.setInt(&fbdo, "MICS/CO", CO_data)){
    // This command will be executed even if you dont serial print but we will make sure its working
    Serial.print("Carbon Monoxide= ");
    Serial.print(CO_data,2);
    Serial.println(" PPM");}
  
    else {
    Serial.println("Failed to Read CO DATA");
    Serial.println("REASON: " + fbdo.errorReason());}

    
    
    if (Firebase.RTDB.setInt(&fbdo, "MICS/CH4", CH4_data)){
    Serial.print("Methane= ");
    Serial.print(CH4_data,2);
    Serial.println(" PPM");}
    
    else {
    Serial.println("Failed to Read CH4 Data");
    Serial.println("REASON: " + fbdo.errorReason());}

    if (Firebase.RTDB.setInt(&fbdo, "MICS/C2H5OH", C2H5OH_data)){
    Serial.print("Ethanol= ");
    Serial.print(C2H5OH_data,2);
    Serial.println(" PPM");}

    
    else {
    Serial.println("Failed to Read C2H5OH Data");
    Serial.println("REASON: " + fbdo.errorReason());}


    if (Firebase.RTDB.setInt(&fbdo, "MICS/NO2", NO2_data)){
    Serial.print("Nitric Oxide= ");
    Serial.print(NO2_data,2);
    Serial.println(" PPM");}

    
    else {
    Serial.println("Failed to Read NO2 Data");
    Serial.println("REASON: " + fbdo.errorReason());}



    if (Firebase.RTDB.setInt(&fbdo, "MICS/NH3", NH3_data)){
    Serial.print("Ammonia= ");
    Serial.print(NH3_data,2);
    Serial.println(" PPM");}

    
    else {
    Serial.println("Failed to Read NH3 Data");
    Serial.println("REASON: " + fbdo.errorReason());}



    if (Firebase.RTDB.setInt(&fbdo, "MICS/H2", H2_data)){
    Serial.print("Hydrogen= ");
    Serial.print(H2_data,2);
    Serial.println(" PPM");}

    
    else {
    Serial.println("Failed to Read H2 Data");
    Serial.println("REASON: " + fbdo.errorReason());}

  Serial.println("_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _");
  }

}

