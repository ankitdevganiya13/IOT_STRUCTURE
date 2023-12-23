extern struct PARAMETERS parameters; 
extern struct GL_VARIABLES var; 

typedef struct parameters str; 

/* SPIFFS contents lisout on serial related function */
void List_Files(){
  File root = SPIFFS.open("/"); 
  File file = root.openNextFile(); 
  while(file){
    Serial.print("File: " + (String)file.name());
    file = root.openNextFile();
  }
}

/* SPIFFS initialize function */
void SPIFFS_Begin(){
  if(!SPIFFS.begin(true)){
    Serial.println("An error has occurred while mounting SPIFFS");
    return;
  } 
}

/* EEPROM erase once in life of the device function */
void Erase_EEPROM_Once(){
  bool eeprom_check;
  EEPROM.get(511, eeprom_check);
  delay(100);

  if(eeprom_check>0){
    Serial.println("Erase EEPROM once flag " + (String)eeprom_check);
    external_rtc.adjust(DateTime(F(__DATE__),F(__TIME__))); /* Update system date and time to HW RTC once */
    for(uint16_t addr=0; addr<512; addr++){
      EEPROM.writeByte(addr,0);
    }
    EEPROM.commit();
    Serial.println("EEPROM wiping done! now device will be restarted");
    delay(1000);  
    ESP.restart();
  }
}

/* EEPROM erase on command */
void Erase_EEPROM(){
  if(var.web_command == "erase"){  //eeprom wiping loop 
    for(uint16_t addr=0; addr<512; addr++){
      EEPROM.writeByte(addr,0);
    }
    EEPROM.commit();
    Serial.println("eeprom wiping done!");
  }
}

/* EEPROM update string value function */
void EEPROM_Update(uint16_t addr, size_t siz, String val){
  char temp_buff[siz];
  memset(temp_buff,NULL,sizeof(temp_buff));
  strcpy(temp_buff,val.c_str()); 
  EEPROM.writeString(addr, temp_buff);
  EEPROM.commit();
  //Serial.println("value " + (String)tempBuff + " saved!");
}

/* EEPROM read string value function */
String EEPROM_Read(uint16_t addr, size_t siz){
  char temp_buff[siz];
  memset(temp_buff,NULL,sizeof(temp_buff));
  strcpy(temp_buff,(EEPROM.readString(addr)).c_str());
  return (String)temp_buff;
}

/* Update settings to eeprom at specific memory location */
void Update_Settings(uint16_t addr, struct PARAMETERS str){
  EEPROM.put(addr, str);
  EEPROM.commit();
  vTaskDelay(100);
}

/* Fetch settings from eeprom in parameter structure which will be accessed accross whole code! */
void Fetch_Settings(uint16_t addr, uint8_t set_para){
  EEPROM.get(addr, parameters);
  Serial.println();
  Serial.printf("Communication Mode: %s\n", parameters.com_mode);
  Serial.printf("Communication Type: %s\n", parameters.com_type);
  Serial.printf("Response Time: %s\n", parameters.response_time);
  Serial.printf("Routers' SSID: %s\n", parameters.wifi_ssid);
  Serial.printf("Routers' Password: %s\n", parameters.wifi_password);
  Serial.printf("Device ID: %s\n", parameters.device_id);
  Serial.printf("Gateway ID: %s\n", parameters.gateway_id);
  Serial.printf("MQTT Server: %s\n", parameters.mqtt_server);
  Serial.printf("MQTT Port: %s\n", parameters.mqtt_port);
  Serial.printf("MQTT Auth Username: %s\n", parameters.mqtt_auth_username);
  Serial.printf("MQTT Auth Password: %s\n", parameters.mqtt_auth_password);
  Serial.printf("MQTT Subscribe Topic: %s\n", parameters.mqtt_sub_topic);
  Serial.printf("MQTT Publish Topic: %s\n", parameters.mqtt_pub_topic);
  Serial.printf("HTTP Server: %s\n", parameters.http_server);
  Serial.printf("HTTP Auth Username: %s\n", parameters.http_auth_username);
  Serial.printf("HTTP Auth Password: %s\n", parameters.http_auth_password);
}

/* Set detection range in inches */
void Reset_Count(){
  if(var.web_command == "countreset"){  
    var.person_count=0; /* Set to zero on command */
    EEPROM_Update(450,((String)var.person_count).length(), (String)var.person_count);
    Serial.println("count value reset done!");
  }
}
