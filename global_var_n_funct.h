/*
 * General Information about Global Variables!
 * var.state[1] = wifi status related flag
 * var.state[2] = mqtt status related flag
 * var.state[3] = lora send state related flag
 * var.state[4] = lora device pair acknowledgement related flag
 * var.state[5] = external rtc begin flag
 * var.state[6] = configure mode converter flag ; if it's true for 5 sec then device will be converted into apmode
 * var.state[7] = lora pair request send flag
 * var.state[8] = system restart flag
 */

/* Input output pins & constant int variables */
enum APPLICATION_RELATED_USED_GPIO_PINS
{
  DEFAULT_CONFG_PIN,
  LORA_IT_PIN = 2,
  LORA_CS_PIN = 5,
  LORA_RS_PIN = 14,
  ADDL_CUSTON_CONFG_PIN = 32
};

/* All setting parameters' structure */
struct PARAMETERS
{
  char com_mode[10];
  char com_type[10]; 
  char response_time[10]; /* response_time */
  char mqtt_port[10];
  char device_id[10]; 
  char gateway_id[10]; 
  char wifi_ssid[20]; 
  char wifi_password[20];
  char mqtt_server[20];
  char mqtt_auth_username[20]; 
  char mqtt_auth_password[20]; 
  char mqtt_sub_topic[40]; 
  char mqtt_pub_topic[40]; 
  char http_server[40]; 
  char http_auth_username[20];
  char http_auth_password[20];
  char occup_range[10];
  char wait_time[10];
} parameters; 

/* Global state variable for whole code */
struct GL_VARIABLES
{
  bool state[10];
  char web_command[20];
  char date_value[15];
  char time_value[15];
  uint8_t cnt[5]; 
  String mac_addr;
  String date_time;
  const char *pair_acknowledge; 
  unsigned long person_count;
  unsigned long last_time;
  unsigned long timer_delay;
  IPAddress ip_address;
} var; 

unsigned long count;

WiFiClient _local_client; /* ESP wifi client */
PubSubClient client(_local_client); /* PubSub client */
RTC_DS3231 external_rtc; /* DS3231 rtc variable */
ESP32Time internal_rtc(3600); /* ESP's internal rtc variable */
AsyncWebServer server(80); /* Websocket server on port 80 */
AsyncEventSource events("/events"); /* events on websocket */

/* Task handlers declaration along with functions */
TaskHandle_t _aqi_sensor_reading_task;
TaskHandle_t _wifi_mqtt_task;
TaskHandle_t _config_mode_task;

/* Delay creation function which will be accessed accross whole code */
bool Run_Every(unsigned long int_time){
  unsigned long prev_mill=0,curr_mill;
  curr_mill=millis();
  if(curr_mill-prev_mill>=int_time)
  {
    prev_mill=curr_mill;
    return true;
  }
  return false;
}

/* IP address to string converter function */
String IP_Addr_Str(const IPAddress& ip_addr) {
  return String(ip_addr[0]) + String(".") +\
  String(ip_addr[1]) + String(".") +\
  String(ip_addr[2]) + String(".") +\
  String(ip_addr[3])  ;
}
