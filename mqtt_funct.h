extern struct PARAMETERS parameters; 
extern struct GL_VARIABLES var; 

/* Payload handler function */
void callback(char* topic, byte* _payload, unsigned int _len_payload){
  for(uint32_t i=0; i<_len_payload; i++){
    Serial.print((char)_payload[i]);
  }
  Serial.println();
}

/* Reconnect client function */
uint8_t Reconnect_Client() { 
  vTaskSuspend(_aqi_sensor_reading_task);
  //vTaskSuspend(_config_mode_task);
  uint8_t err_state;
  while((WiFi.status()==WL_CONNECTED) && (client.connected()!=true)){
    Serial.print("Relooking to connect with MQTT - ");
    if(client.connect(var.mac_addr.c_str())){
      Serial.println("Connected");
      client.subscribe(((String)parameters.mqtt_sub_topic).c_str());
    } 
    else{
      Serial.printf("failed to connect RC %d\n",client.state());
      vTaskDelay(3000);
      err_state=2;
    }
  }  

  var.state[2]=err_state;
  
  if(client.connected()==true){
    Serial.println("All tasks are resumed!");
    vTaskResume(_aqi_sensor_reading_task);
    //vTaskResume(_config_mode_task); 
    var.state[2]=0;
    return var.state[2];
  }
}

/* MQTT client setup function */
uint8_t MQTT_Client_Setup(){
  Serial.print("Attempting to connect with MQTT - ");
  client.setServer(((String)parameters.mqtt_server).c_str(), atoi(parameters.mqtt_port)); 
  client.setCallback(callback);
  uint8_t err_state, tryToConnect=0;
  while((tryToConnect++<10) && (WiFi.status()==WL_CONNECTED) && (!client.connected())){ 
    if(client.connect(var.mac_addr.c_str(), ((String)parameters.mqtt_auth_username).c_str(), ((String)parameters.mqtt_auth_password).c_str())){
      Serial.println("Connected");
      client.subscribe(((String)parameters.mqtt_sub_topic).c_str());
    } 
    else{
      Serial.printf("failed to connect RC %d\n",client.state());
      vTaskDelay(3000);
      err_state=2;
    }
  }
  if(client.connected()) return 0;
  return err_state;
}
