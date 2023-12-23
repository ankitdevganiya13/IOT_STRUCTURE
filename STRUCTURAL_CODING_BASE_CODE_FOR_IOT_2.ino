/* CODE OPTIMIZATION ; RESTRUCTURIZATION OF ALL FLOWS ON FREERTOS! */

#include <ArduinoJson.h>
#include <RTClib.h>
#include <Wire.h>  
#include <SPI.h> 
#include <LoRa.h>
#include <WiFi.h>
#include <EEPROM.h>
#include <RTClib.h>
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"
#include <PubSubClient.h>
#include <ESP32Time.h>
#include "time.h"
#include "global_var_n_funct.h"
#include "lora_funct.h"
#include "spiffs_n_eeprom_funct.h"
#include "ext_n_internal_rtc_funct.h" 
#include "webserver_setup.h"
#include "wifi_funct.h"
#include "mqtt_funct.h"
#include "routine_tasks_freertos.h"

void WIFi_MQTT_Connectivity_Routine(void *parameter);
void System_Config_Routine(void *parameter);
void IAQI_Sensor_Read_Routine(void *parameter);

/* Configure mode converter */
void Configure_Mode_Selector(){
  noInterrupts();
  var.state[6]=true;
  //Serial.printf("config mode flag %d\n",var.state[6]);
}

/* Main setup function */
void setup() 
{
  Serial.begin(115200);   
  Wire.begin(); 
  EEPROM.begin(512);
  pinMode(DEFAULT_CONFG_PIN,INPUT); /* WiFi access point converter button pin as input */
  attachInterrupt(DEFAULT_CONFG_PIN, Configure_Mode_Selector, FALLING); /* Configuration mode converter button on interrupt */
  SPIFFS_Begin(); /* SPIFFS initialize */ 
  Ext_Or_Internal_RTC_Begin(); /* RTC intialize */ 
  Erase_EEPROM_Once(); /* It will check if eeprom have any value at location 511 then eeprom will be wiped up! */
  
  //parameters = (PARAMETERS){0,0,2000,8051,"DEV001","LG0001","Tenda_2001","Resonent@2001","192.168.1.52","Resonent","India@123","test_sub_topic","test_pub_topic","","",""}; /* Default parameters value */ 
  
  Serial.println("Date & Time: " + Date_Time()); /* Display rtc date and time on startup */
  Fetch_Settings(0, 1); /* Parameters and all other setting fetching from eeprom */

  xTaskCreatePinnedToCore(System_Config_Routine,"_config_mode_task",5000,NULL,3,&_config_mode_task, 1); /* Configure device task initialized first */  
  
  WiFi_Connection_Setup(); /* WiFi connection with the target access point */ 
  MQTT_Client_Setup(); /* If wifi is connected then try for MQTT connection */
  LoRa_Initializer(); /* LoRa module initialize will be after wifi and mqtt setup */ 

  var.mac_addr = WiFi.macAddress().c_str(); /* Getting mac address of the device into the global variable */
  var.ip_address = WiFi.localIP(); /* Fetching wifi assined ip address */
  var.pair_acknowledge = "PAIRING_DONE_"; /* Pair done acknowledgement check sample string */ 
  var.timer_delay=atoi(parameters.response_time);
  
  delay(500); /* Some delay before initiating all tasks */
  
  xTaskCreatePinnedToCore(WIFi_MQTT_Connectivity_Routine,"_wifi_mqtt_task",5000,NULL,1,&_wifi_mqtt_task, 1); /* WiFi and MQTT reconnection related task */  
  xTaskCreatePinnedToCore(IAQI_Sensor_Read_Routine,"_aqi_sensor_reading_task",5000,NULL,4,&_aqi_sensor_reading_task, 1); /* Sensor value get routine task */  
}

/* Main loop */
void loop() 
{
  delay(1000);
}
