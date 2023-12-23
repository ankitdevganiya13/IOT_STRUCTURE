extern struct PARAMETERS parameters; 
extern struct GL_VARIABLES var; 

//not found funtion
void Not_Found(AsyncWebServerRequest *request) 
{
  request->send(404, "text/plain", "Not found");
}

/* Get response from webpage */
char* Get_Response(String id, AsyncWebServerRequest* request, char *para)
{
  String input_param, get_value;
  if(request->hasParam(id)){
    get_value = request->getParam(id)->value();
    input_param = id;
  }
  size_t buff_len = get_value.length()+1;
  char temp_arr[buff_len];
  memset(temp_arr,NULL,sizeof(temp_arr));
  strcpy(temp_arr,get_value.c_str());
  //Serial.printf("get value: %s\n",get_value);
  if(strcmp(temp_arr, "")!=false){
    memset(para,NULL,sizeof(para));
    strcpy(para,get_value.c_str());
    Serial.printf("value in parameter: %s\n",para);
    return temp_arr;
  }
}

/* System or device reboot function */
uint8_t Restart_System()
{
  if(var.state[8]==true){  
    Serial.print("system is rebooting ");
    delay(2000);
    Serial.println("system is rebooting ...");
    ESP.restart();
  }
}

//initializer function
void Web_Sever_Function()
{ 
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) /* Webpage launcher */
  { 
    request->send(SPIFFS, "/index.html", "text/html");
  });    
  server.serveStatic("/", SPIFFS, "/"); /* Logo image and other thing reader */
  
  events.onConnect([](AsyncEventSourceClient *client) /* Event handler routine */
  { 
    if(client->lastId()){
      Serial.printf("client reconnected! last message ID that it got is: %u\n", client->lastId());
    }
    client->send("hello!", NULL, millis(), 60000);
  });

  server.on("/parameters", HTTP_GET, [] (AsyncWebServerRequest *request)
  { 
    memset(parameters.gateway_id,NULL,sizeof(parameters.gateway_id)); 
    Get_Response("commod", request, parameters.com_mode);
    Get_Response("comtype", request, parameters.com_type);
    Get_Response("deviceID", request, parameters.device_id);
    Get_Response("gatewayid", request, parameters.gateway_id);
    Get_Response("responseTime", request, parameters.response_time);
    Get_Response("ssid", request, parameters.wifi_ssid);
    Get_Response("password1", request, parameters.wifi_password);
    Get_Response("host", request, parameters.mqtt_server);
    Get_Response("port", request, parameters.mqtt_port);
    Get_Response("authUsernameOne", request, parameters.mqtt_auth_username);
    Get_Response("authPasswordOne", request, parameters.mqtt_auth_password);
    Get_Response("host", request, parameters.mqtt_server);
    Get_Response("port", request, parameters.mqtt_port);
    Get_Response("authUsernameOne", request, parameters.mqtt_auth_username);
    Get_Response("authPasswordOne", request, parameters.mqtt_auth_password);
    Get_Response("outtopic", request, parameters.mqtt_pub_topic);
    Get_Response("intopic", request, parameters.mqtt_sub_topic);
    Get_Response("server", request, parameters.http_server);
    Get_Response("authUsernameTwo", request, parameters.http_auth_username);
    Get_Response("authPasswordTwo", request, parameters.http_auth_password);
    Get_Response("occuprange", request, parameters.occup_range);
    Get_Response("wait", request, parameters.wait_time);
    Get_Response("erase", request, var.web_command);
    Get_Response("reboot", request, var.web_command);
    Get_Response("countreset", request, var.web_command);
    Get_Response("reset", request, var.web_command);
    Get_Response("rtcupdate", request, var.web_command);
    Get_Response("dateValue", request, var.date_value);
    Get_Response("timeValue", request, var.time_value);
        
    if(strcmp(parameters.gateway_id,"")!=0){ /* Check if lora pairing request is received from fe then go through it*/
      Update_Settings(0, parameters); /* Update received gateway id to eeprom */ 
      if(var.state[7]==false){/* If pair request is not send since last 10 seconds */
        Pair_Request_Send(); /* Send pair request to lora gateway */
        delay(3000); /* Wait some time for acknowledgemenet */
      }
    }
    else{
      Update_Settings(0, parameters); /* Parameters and all other setting update to eeprom */
      Fetch_Settings(0, 1); /* Parameters and all other setting fetching from eeprom */
      Erase_EEPROM(); /* Erase eeprom when command is receive */
      Reset_Count(); /* Occupancy sensor person count number reset */
      if(strcmp(var.web_command, "Reboot")==false){ /* System restart related command check */
        var.state[8]=true; /* System restart flag */
      }    
      Date_Time_Update(); /* Update or set date and time of external rtc manually */
    }
    request->send(SPIFFS, "/index.html", "text/html"); /* Relauch page after get method */
    server.serveStatic("/", SPIFFS, "/");
  });
  server.onNotFound(Not_Found);
  server.addHandler(&events);
  server.begin(); 
}

/* Webpage value updater */
void Data_Update_To_Web()
{
  String occuprange=(String)parameters.occup_range;
  String temperature=(String)temperatureRead();
  
  if((millis()-var.last_time)>var.timer_delay){ 
    events.send("ping",NULL,millis()); 
    events.send(String(parameters.com_mode).c_str(),"commod",millis()); 
    events.send(String(parameters.com_type).c_str(),"comtype",millis());
    events.send(String(parameters.wifi_ssid).c_str(),"wifissid",millis());    
    events.send(String(parameters.wifi_password).c_str(),"wifipass",millis()); 
    if(var.state[1]==1){
      events.send("Connected","wifistatus",millis());
      events.send(IP_Addr_Str(var.ip_address).c_str(),"ipaddr",millis());
    }else{
      events.send("Not Connected","wifistatus",millis());
      events.send("NULL","ipaddr",millis());
    }
    events.send(String(parameters.gateway_id).c_str(),"gatewayidrev",millis());    
    events.send(String(var.mac_addr).c_str(),"macid",millis()); 
    events.send(String(parameters.response_time).c_str(),"responsetime",millis()); 
    events.send(occuprange.c_str(),"temperature",millis());  
    events.send(temperature.c_str(),"occupvalue",millis());  
    var.last_time = millis();  
  }
  vTaskDelay(1770); /* odd time delay for not confliction */
}
