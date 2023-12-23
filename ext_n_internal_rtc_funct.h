extern struct PARAMETERS parameters; 
extern struct GL_VARIABLES var; 

/* External RTC initializer function */
void Ext_Or_Internal_RTC_Begin(){
  if(external_rtc.begin()){
    var.state[5]=true;        
  }
  else{
    
  }
}

/* Fetch date and time from external or internal rtc of the device */
String Date_Time(){
  if(var.state[5]==true){ 
    DateTime now = external_rtc.now();
    var.date_time = "";
    var.date_time += String(now.year())+"-";
    var.date_time += String(now.month())+"-";
    var.date_time += String(now.day())+" ";
    var.date_time += String(now.hour())+":";
    var.date_time += String(now.minute())+":";
    var.date_time += String(now.second());    
  }
  else{
    uint8_t cur_month = internal_rtc.getMonth() + 1;
    uint8_t cur_hours = internal_rtc.getHour(true) -1;
    var.date_time = "";
    var.date_time += String(internal_rtc.getYear())+"-";
    var.date_time += String(cur_month)+"-";
    var.date_time += String(internal_rtc.getDay())+" ";
    var.date_time += String(cur_hours)+":";
    var.date_time += String(internal_rtc.getMinute())+":";
    var.date_time += String(internal_rtc.getSecond());   
  }
  return var.date_time;
}

/* Update internal or external RTC as per user input */
void Date_Time_Update(){
  if(strcmp(var.web_command,"rtcupdate")==false){
    if((strcmp(var.date_value,"")!=false)&&(strcmp(var.time_value,"")!=false)&&(var.state[5]==true)){
      external_rtc.adjust(DateTime(var.date_value,var.time_value));
      Serial.println("date & time successfully upadated!");
    }
  }
}
