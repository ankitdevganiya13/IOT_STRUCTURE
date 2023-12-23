console.log("Enter Script");

const userName="admin";
const password="admin";

var lock;
var comstate;

let allData=[
];

if (!!window.EventSource) 
{
 var source = new EventSource('/events');
 
 source.addEventListener('open', function(e) {
  console.log("Events Connected");
 }, false);
 
 source.addEventListener('error', function(e) {
  if (e.target.readyState != EventSource.OPEN) {
    console.log("Events Disconnected");
  }
 }, false);
 
 source.addEventListener('message', function(e) {
  console.log("message", e.data);
 }, false);
 
 let once=false;
 source.addEventListener('comstate', function(e) {
  if(!once)
  {
	once=true;
    console.log("comstate", e.data);
    commModeDropDown(e.data);
  }	  
 }, false);
 
 let once1 = false;
 source.addEventListener('comtype', function(e) { 
  if(!once1)
  {
	console.log("comtype", e.data);
    commTypeDropDown(e.data);
  }  
 }, false);
 
  source.addEventListener('macid', function(e) {
  console.log("macid", e.data);
  document.getElementById("macid").innerHTML = e.data;
 }, false);
 
 source.addEventListener('ipaddr', function(e) {
  console.log("ipaddr", e.data);
  document.getElementById("ipaddr").innerHTML = e.data;
 }, false);
 
 source.addEventListener('responsetime', function(e) {
  console.log("responsetime", e.data);
  document.getElementById("responsetime").innerHTML = e.data;
 }, false);
 
 source.addEventListener('connectednodes', function(e) {
  console.log("connectednodes", e.data);
  document.getElementById("connectednodes").innerHTML = e.data;
 }, false);
 
 source.addEventListener('temperature', function(e) {
  console.log("temperature", e.data);
  document.getElementById("temperature").innerHTML = e.data;
 }, false);
 
 source.addEventListener('allnodedata', function(e) {
	 
  console.log(typeof(e.data));
  console.log("allnodedata", e.data);

  allData=JSON.parse(e.data);
  

  //allData=`[${JSON.parse(e.data)}]`;
  //console.log("dfgrdyftry",allData,typeof(allData));

  createTable(allData); 
 
}, false);

    

 
 function commModeDropDown(val){
  document.getElementById("commod").selectedIndex = val;	
 }
 
 function commTypeDropDown(val){
  document.getElementById("comtype").selectedIndex = val;	
}
}

/*
allData=[
	{
        id:'BT0001',
		delete:false
    },
    {
        id:'OS0001',
		delete:false
    },
    {
        id:'TH0001',
		delete:false
    },
    {
        id:'AQM001',
		delete:false
    },
]
*/

let tableHead=`<thead>
<tr>
<td class="bold-txt"> # </td>
<td class="bold-txt">Node ID</td>
<td class="bold-txt">Action</td>
</tr></thead>`

function createTable(data){
   let newData= data.filter((item,index)=>{
        if(item.delete!=true){
            return item;
        }
    })
    //let  newData=[data];

    let allDataTemplate="";
    newData.map((item,index)=>{
		
		console.log("item",item,typeof(item),item.id)
    allDataTemplate=allDataTemplate+`
    <tr>
    <td>${index+1}</td>
    <td>${item.id}</td>
	<td><button type="button" id="removeDevice" data-id="${index}" class="" style="width:100%; background:transparent;"><img src="delete.png" alt="Italian Trulli" style="height:30px; width:30px;"></button></td>  
  
    </tr>
    `    
})

$("#iotDevices").html(tableHead+allDataTemplate);
}


allData.map((item,index)=>{
})

createTable(allData); 

$('#iotDevices').on('click', '#removeDevice', function(e) {
	
	console.log($(this).data('id'));
	$("#removeid").val($(this).data('id'));
	$("#FORM5").submit();
	//$("#FORM5").submit((e)=>{ e.preventDefault();});
	
	//window.location.reload();
	
    let id=$(this).data('id')

      allData.map((item,index)=>{
        if(id==item.id){
            item.delete=true;
        }
    })
	
   createTable(allData);
})

$("#rmv").hide();
$("#removeid").hide();
  

// to handle tabs
$(".deviceConfig").show();
$(".networkConfig").hide();
$(".deviceInfo").hide();
$(".advanceSetup").hide();

// handle bottom button
$("#btnDeviceConf").css("border-bottom","1px solid white");
$("#btnAdvanceSetup").css("border-bottom","unset");
$("#btnNetworkConf").css("border-bottom","unset");
$("#btnDeviceinfo").css("border-bottom","unset");


$("#btnAdvanceSetup").click(()=>{
	$(".navbar-toggler").click();
    // handle bottom button
	$("#btnDeviceConf").css("border-bottom","unset");
	$("#btnAdvanceSetup").css("border-bottom","1px solid white");
	$("#btnNetworkConf").css("border-bottom","unset");
	$("#btnDeviceinfo").css("border-bottom","unset");    
	$(".deviceConfig") .hide();
	$(".networkConfig") .hide();
	$(".deviceInfo") .hide();
	$(".advanceSetup") .show();
})

$("#btnNetworkConf").click(()=>{
  	$(".navbar-toggler").click();  
    // handle bottom button
	$("#btnDeviceConf").css("border-bottom","unset");
	$("#btnAdvanceSetup").css("border-bottom","unset");
	$("#btnNetworkConf").css("border-bottom","1px solid white");
	$("#btnDeviceinfo").css("border-bottom","unset");   
    $(".deviceConfig") .hide();
    $(".networkConfig") .show();
    $(".deviceInfo") .hide();
    $(".advanceSetup") .hide();
 })

 $("#btnDeviceinfo").click(()=>{
	$(".navbar-toggler").click();
     // handle bottom button
	$("#btnDeviceConf").css("border-bottom","unset");
	$("#btnAdvanceSetup").css("border-bottom","unset");
	$("#btnNetworkConf").css("border-bottom","unset");
	$("#btnDeviceinfo").css("border-bottom","1px solid white");
    $(".deviceConfig") .hide();
    $(".networkConfig") .hide();
    $(".deviceInfo") .show();
    $(".advanceSetup") .hide();
 })

$("#btnDeviceConf").click(()=>{
$(".navbar-toggler").click();     
     // handle bottom button
	$("#btnDeviceConf").css("border-bottom","1px solid white");
	$("#btnAdvanceSetup").css("border-bottom","unset");
	$("#btnNetworkConf").css("border-bottom","unset");
	$("#btnDeviceinfo").css("border-bottom","unset");  
    $(".deviceConfig") .show();
    $(".networkConfig") .hide();
    $(".deviceInfo") .hide();
    $(".advanceSetup") .hide();
 })
 
 $("#savesettings").click(()=>{
	setTimeout(function(){ $("#exampleModal1").modal('toggle'); form.submit(); }, 2000);
	let loginForm = document.getElementById("FORM1"); //FORM1
	setTimeout(function formSubmit(){document.getElementById("FORM1").submit();}, 1000); //FORM1
})

 $("#rebootbtn").click(()=>{
	setTimeout(function(){ $("#exampleModal3").modal('toggle'); form.submit(); 
    $("#rbt").click(); 
	}, 2000);	
	let loginForm = document.getElementById("FORM3"); //FORM1	
	setTimeout(function formSubmit(){document.getElementById("FORM3").submit();}, 1000); //FORM1
})

 $("#savesettings1").click(()=>{
	setTimeout(function(){ $("#exampleModal2").modal('toggle'); form.submit(); }, 2000);
	let loginForm = document.getElementById("FORM2"); //FORM1
	setTimeout(function formSubmit(){document.getElementById("FORM2").submit();}, 1000); //FORM1
})

//handle change type
$("#contRestAPI").hide();
$("#comtype").change(()=>{
	console.log("connectednodes");
	if($("#comtype").val()=="0"){
		$("#contMQTT").show();
		$("#contRestAPI").hide();
		console.log("connectednodes");
	}else{
		$("#contMQTT").hide();
		$("#contRestAPI").show();
		console.log("connectednodes");
	}
})

$("#configurations").hide();
$("#btnDeviceConf").hide();
$("#btnAdvanceSetup").hide();
$("#btnNetworkConf").hide();
$("#btnDeviceinfo").hide();

$("#logout").click(()=>{
	sessionStorage.clear();
	location.reload();
	
});

if(sessionStorage.getItem("user")){
	 $("#loginform").hide();
	 $("#configurations").show();
	 $("#btnDeviceConf").show();
	 $("#btnAdvanceSetup").show();
	 $("#btnNetworkConf").show();
	 $("#btnDeviceinfo").show();
	 $("#logout").show(); 
}else{
	 $("#loginform").show();
	 $("#configurations").hide();
	 $("#logout").hide();
}

$("#login").click(()=>{
	let name=$("#username").val();	
	let pass=$("#password").val();	
	if(name==userName&&pass==password){
	console.log(name,pass);
    sessionStorage.setItem("user",true);
	$("#loginform").hide();
    $("#configurations").show();	
	$("#btnDeviceConf").show();
	$("#btnAdvanceSetup").show();
	$("#btnNetworkConf").show();
	$("#btnDeviceinfo").show();
	$("#logout").show();	
	}
	else if(name==userName && pass!=password)
	{
		alert("Password is wrong, please enter right one!");
	}	
	else if(name!=userName && pass==password)
	{
		alert("Username is wrong, please enter right one!");
	}
	else if(name!=userName && pass!=password)
	{
		alert("Username & Password is wrong, try again with appropriate!");
	}
});
