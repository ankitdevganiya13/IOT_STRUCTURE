extern struct PARAMETERS parameters; 
extern struct GL_VARIABLES var; 

/* WiFi reconnect try function */
uint8_t WiFi_Reconnect() 
{
  vTaskSuspend(_aqi_sensor_reading_task);
  //vTaskSuspend(_config_mode_task);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(((String)parameters.wifi_ssid).c_str(), ((String)parameters.wifi_password).c_str());
  Serial.print("Reconnecting to WiFi access point .");
  while((WiFi.status()!=WL_CONNECTED) && (var.state[1]==0)){    
    Serial.print('.');
    vTaskDelay(3000);  
  }
  var.ip_address = WiFi.localIP(); /* Connected network ip address get */
  Serial.println(var.ip_address); /* Display the ip address */

  var.state[1]=WiFi.status();
  return var.state[1]; /* Once wifi connection status return during setup */ 
}

/* WiFi connection setup function */
uint8_t WiFi_Connection_Setup() 
{
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(((String)parameters.wifi_ssid).c_str(),((String)parameters.wifi_password).c_str());
  Serial.print("Connecting to WiFi access point .");
  uint8_t try_to_wifi_connect;
  while((try_to_wifi_connect++<10) && (WiFi.status()!=WL_CONNECTED)){    
    vTaskDelay(1000);
    Serial.print('.');
  }
  var.ip_address = WiFi.localIP(); /* Connected network ip address get */
  Serial.println(var.ip_address); /* Display the ip address */

  if(WiFi.status()==WL_CONNECTED) var.state[1]=1;
  else var.state[1]=0;
  return var.state[1]; /* Once wifi connection status return during setup */
}

/* Payload handler function */
void WiFi_Access_Point_Setup()
{
  char _access_point_ssid[40];
  uint32_t _chip_id = 0;
  for(uint8_t i=0; i<17; i=i+8){
    _chip_id |= ((ESP.getEfuseMac()>>(40-i))&0xff)<<i;
  }
  memset(_access_point_ssid,NULL,sizeof(_access_point_ssid));
  sprintf(_access_point_ssid,"TEST_SSID_%d",_chip_id);  
  WiFi.disconnect();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(_access_point_ssid, "123456789"); /* WiFi access point begin with these credentials */
  Serial.println(WiFi.softAPIP());
  vTaskDelay(1000); 
  EEPROM.put(500, false); /* Write configuration state for after restart action */
  EEPROM.commit();
  vTaskDelay(1000);
}

/* Configure mode converter function */
void Configure_Mode_Converter()
{
  if(var.state[6]==true)
  {
    while((digitalRead(DEFAULT_CONFG_PIN)==LOW)&&(var.cnt[1]<6)){  
      delay(200); /* 1000 as one second dely between counting */
      Serial.printf("long press counter %d\n",var.cnt[1]++);
    }
    if((var.cnt[1]>4)&&(var.state[6]==true)){ /* If button is pressed for 5 second then device will be converted into wifi ap */
      Serial.println("configure mode started!");
      LoRa_Tx_Mode(); /* Disable lora during accesspoint conversion */
      vTaskSuspend(_aqi_sensor_reading_task); /* Sensor value reading routine will be suspended during accesspoint conversion */
      vTaskSuspend(_wifi_mqtt_task); /* Device will no check for wifi or mqtt reconnection */
      WiFi_Access_Point_Setup();
      Web_Sever_Function();
      var.state[6]=var.cnt[1]=false; /* Clear both config flag and long press counter on mode conversion */
    }
    else{
      var.state[6]=var.cnt[1]=false; /* Clear both config flag and long press counter on mode conversion */
      Serial.printf("config mode flag %d, cnt %d\n",var.state[6], var.cnt[1]);
      interrupts();
    }
  }
}
