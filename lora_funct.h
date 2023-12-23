extern struct PARAMETERS parameters; 
extern struct GL_VARIABLES var; 

/* LoRa receiver mode function */
void LoRa_Rx_Mode(){
  LoRa.enableInvertIQ(); /* Active invert I and Q signals*/
  LoRa.receive(); /* Set receiver mode */
}

/* Lora transmitter mode function */
void LoRa_Tx_Mode(){
  LoRa.idle(); /* Set standby mode */
  LoRa.disableInvertIQ(); /* Set normal mode */
}

//lora message send function
void LoRa_Send_Message(String message){
  LoRa_Tx_Mode(); /* Set transmitter mode */
  LoRa.beginPacket(); /* Start packet to be send */
  LoRa.print(message); /* Add payload string */
  LoRa.endPacket(); /* Finish packet and send it */
  LoRa_Rx_Mode(); /* Set receiver mode */
}

/* LoRa payload handling function */
void onReceive(int packetSize){
  String rec_message = "";
  while(LoRa.available()&&(var.state[3]==0)&&(Run_Every(30000)!=true)) { /* Payload gathering loop */
    rec_message += (char)LoRa.read();
  }      
  if(strstr(rec_message.c_str(), ((String)parameters.device_id).c_str())){ /* Device id matching condition if it is true then further process will be performed */
    var.state[3]=1; /* Now the device can send data over lora link */
    Serial.print("Node Received: " + rec_message);
    var.state[3]=0;
  } 
  if(strstr(rec_message.c_str(), var.pair_acknowledge)){ /* LoRa pair done acknowledgement done check condition */
    var.state[4] = 1; /* The device is connected with lora gateway */
    rec_message = ""; /* Nullify the receive message variable */
  } 
  return;
}

/* Send data over LoRa link */
void Send_Data(String data_msg){
  String data_str;
  uint32_t index=0, data_len = ((String)data_msg).length();
  while(true){
    if(index<data_len){
      for(uint8_t n_index=0; n_index<250; n_index++){
        data_str += (String)data_msg[index + n_index];
      }        
      LoRa_Send_Message(data_str); 
      delay(45);    
      data_str = ""; /* Clear after sending packet of 250 Byte */   
      index=index+250; /* Increment toward next packet to be sent */  
      //Serial.println("Sample " + (String)i + " Data: " + data_str); 
    }
    else break;  
  }
}

/* Pair request send function */
void Pair_Request_Send(){
  String pair_req="CONSIDER_THIS_DEV_" + (String)parameters.gateway_id + "<" + (String)parameters.device_id + ">";
  Serial.println(pair_req);
  LoRa_Send_Message(pair_req); 
  LoRa_Rx_Mode();
  delay(100);
}

/* Some global variables which will be accessed accross whole code */
void LoRa_Initializer(){
  LoRa.setPins(LORA_CS_PIN, LORA_RS_PIN, LORA_IT_PIN); /* LoRa initiate with defined gpio pins */ 
  if(!LoRa.begin(433E6)){ /* LoRa begin with defined frequency */
    Serial.println("LoRa init. failed!");                      
  }
  else{
    LoRa.onReceive(onReceive);  
    LoRa_Rx_Mode();  
    Serial.println("LoRa init. success!");  
  }
}
