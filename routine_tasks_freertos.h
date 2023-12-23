extern struct PARAMETERS parameters; 
extern struct GL_VARIABLES var; 

/* WiFi and MQTT connectivity related routine task; it will will on top priority when comm. mode is wifi or gsm */
void WIFi_MQTT_Connectivity_Routine(void *parameter){
  Serial.print("WiFi and MQTT connection related task running on ");
  Serial.println(xPortGetCoreID());
  for(;;){
    if(WiFi.status()!=WL_CONNECTED){ /* If WiFi connectivity is lost then this condition will be true */    
      var.state[1]=WiFi_Reconnect();   
    }else if(!client.connected()){ /* In this state the device will try to connect with the MQTT server */
      var.state[2]=Reconnect_Client();       
    }else{ /* WiFi and MQTT both are connected then the state will be 0*/
      client.loop();  
    }
    vTaskDelay(1000);    
  } 
}

/* System and parameter configuration related routine */
void System_Config_Routine(void *parameter){
  Serial.print("System configuration task running on ");
  Serial.println(xPortGetCoreID());
  for(;;){ 
    Configure_Mode_Converter();
    Data_Update_To_Web();
    Restart_System(); /* Device restart on command */
    vTaskDelay(200);
  } 
}

/* Sensor reading or sensor data get routine */
void IAQI_Sensor_Read_Routine(void *parameter){
  Serial.print("AQI sensor bench reading routine task on ");
  Serial.println(xPortGetCoreID());
  for(;;){
    vTaskDelay(400);
  } 
}
