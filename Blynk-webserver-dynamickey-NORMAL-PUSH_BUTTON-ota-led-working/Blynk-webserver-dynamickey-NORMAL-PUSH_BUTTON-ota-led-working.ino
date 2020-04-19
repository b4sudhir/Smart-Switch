/*************************************************************
  This is a DEMO. You can use it only for development and testing.
  You should open Setting.h and modify General options.

  If you would like to add these features to your product,
  please contact Blynk for Businesses:

                   http://www.blynk.io/

 *************************************************************/

#define USE_NODE_MCU_BOARD        // Comment out the boards you are not using

#define APP_DEBUG        // Comment this out to disable debug prints

#define BLYNK_PRINT Serial

#include "BlynkProvisioning.h"


//===================================OUTSIDE LIBRARY===========================================

//////Additional lib added for serial print // arduino communcation // timer

#define BLYNK_PRINT Serial
#include <SoftwareSerial.h>
#include <SimpleTimer.h>

int check_Btn = 0; // 0 for NORMAL SWITCH // 1 for PRESS SWITCH

//===================================VARIABLES===========================================




bool ipprint=0;
uint16_t x;
String d1, d2, d3, d4, d5;
int io1val=0,io2val=0,io3val=0,io4val=0,io5val=0; //to store xml output value send via webserver
String webSite,javaScript,XML,confi;

//Variable for dimmer send data to arduino for setting dimmer
//int pinValue = 1;
String v2arduino; // values sending to arduino


String myString; // complete message from arduino, which consist of fan speed
char rdata; // received characters from arduino via serial
int valueforslider = 1;  //store rdata value for future use
int chk_valforslider = 200; // to check the value for slider is changed before sending to arduino


//TIMER INTILATISING AND DECLARATION
BlynkTimer timer;

void checkPressButton(); //Funtion for Press switch
void check_normal_Btn();   // Funtion for Normal Switch

bool switch_flag=0; // Variable for running only one time for intilaising timmer funtion; 
// Attach virtual serial terminal to Virtual Pin V1
WidgetTerminal terminal(V1);


const int btnPin1 = 12;      // pin for physical push button switch.
const int btnPin2 = 14;      // pin for physical push button switch.
const int btnPin3 = 13;      // pin for physical push button switch.
const int btnPin4 = 2;       // pin for physical push button switch.

const int OutPin1 =  5;      // pin for relay output.
const int OutPin2 =  16;      // pin for relay output.
const int OutPin3 =  4;      // pin for relay output.
const int OutPin4 =  0;      // pin for relay output.


int btnPin1State = LOW;           // ON
int OutPin1State = HIGH;        // OFF
int btnPin2State = LOW;           // ON
int OutPin2State = HIGH;        // OFF
int btnPin3State = LOW;           // ON
int OutPin3State = HIGH;        // OFF
int btnPin4State = LOW;           // ON
int OutPin4State = HIGH;        // OFF

int btnPin1old = 2;
int btnPin2old = 2;
int btnPin3old = 2;
int btnPin4old = 2;

//================================   FUNCTIONS   ===========================================


//////FUNCTION FOR WEBSERVER HTML PAGES CONFIG PAGE
const char* config_page = R"html(
<HTML>
<style>
body {
    width: 100%;
    margin: 0;
    padding: 0;
    background: #121212;
    padding: 30px;
}

fieldset {
    width: 380px;
    border: 2px solid #222;
    border-radius: 8px;
    margin: auto;
    min-height: 10px;
    line-height: 10px;
    position: relative;
    float: center;
    padding: 20px;
    top: 60px;
}

h2 {
    font-family: 'Lobster', cursive;
    font-weight: normal;
    color: #bb86fc;
    text-shadow: 0 1px 0 rgba(255, 255, 255, 0.12);
    display: inline-block;
    margin: 0;
    line-height: 1.3;
    position: absolute;
    top: -20px;
    background: #1c1920;
    padding: 0 5px 0 5px;
}

.blur {
    filter: blur(20px);
}

input[type='checkbox'] {
    padding-top: 18px;
    line-height: 15px;
    padding-bottom: 18px;
    text-align: center;
    color: #000;
    text-transform: uppercase;
    font-weight: 600;
    margin-left: 60px;
    margin-bottom: 0px;
    margin-top: 10px;
    cursor: pointer;
    display: inline-block;
    -webkit-appearance: none;
    outline: none;
    box-shadow: 0 0 0 1px #353535, 0 0 0 3px #3e3e3e, inset 0 0 10px rgba(0, 0, 0, 1);
}

.button {
    width: 100px;
    padding-top: 18px;
    line-height: 15px;
    padding-bottom: 18px;
    text-align: center;
    color: #000;
    text-transform: uppercase;
    font-weight: 600;
    margin-left: 60px;
    margin-bottom: 0px;
    margin-top: 10px;
    cursor: pointer;
    display: inline-block;
}

.button-1 {
    background-color: transparent;
    border: 2px solid #2af1fc;
    border-radius: 30px;
    -webkit-transition: all 0.15s ease-in-out;
    transition: all 0.15s ease-in-out;
    color: #00d7c3;
}

.button-1:hover {
    box-shadow: 0 0 10px 0 #00d7c3 inset, 0 0 20px 2px #00d7c3;
    border: 3px solid #00d7c3;
}
</style>
<script>
function reset() { var r = confirm('Do You want to Reset the device??!!All Data will be deleted.'); if (r == true) { document.getElementById('rst').submit(); } else { alert('Cancel!!!'); } }

function reboot() { var r = confirm('Do You want to Reboot the device?'); if (r == true) { document.getElementById('rbt').submit(); } else { alert('Cancel!!!'); } }

function alertbox() { alert('Working on it!!'); }
</script>

<BODY>
    <fieldset> <a href='/' class='button button-1'> Back </a> <a href='/xml' target='_blank' class='button button-1'> XML </a></fieldset></br></br></br>
    <fieldset>
        <h2 class='blur'>MODE!</h2>
        <h2>MODE!!!</h2>
        <div onclick='alertbox()' class='button button-1'>Touch Switch</div>
        <div onclick='alertbox()' class='button button-1'>Press Switch</div>
        <div onclick='alertbox()' class='button button-1'>NORMAL Switch</div>
    </fieldset></br></br>
    <fieldset>
        <h2 class='blur'>SYSTEM!</h2>
        <h2>SYSTEM!!!</h2>
        <div onclick='reset()' class='button button-1'>Reset</div>
        <div onclick='reboot()' class='button button-1'> Reboot </div>
    </fieldset>
    <form id='rst' method='get' action='reset'></form>
    <form id='rbt' method='get' action='reboot'></form>
</BODY>
</HTML>
)html";

const char* index_page = R"html(
<!DOCTYPE HTML>
<HTML>
<title>SMART HOME</title>
<style>
body {
    width: 100%;
    margin: 0;
    padding: 0;
    background: #121212;
    padding: 30px;
}

a,
input[type='radio']+label {
    text-align: center;
    display: inline-block;
    text-decoration: none;
    cursor: pointer;
    font-family: 'Trebuchet MS', Helvetica, sans-serif;
}

fieldset {
    width: 380px;
    border: 2px solid #222;
    border-radius: 8px;
    margin: auto;
    min-height: 10px;
    line-height: 10px;
    position: relative;
    float: center;
    padding: 20px;
    top: 60px;
}

h2 {
    font-family: 'Lobster', cursive;
    font-weight: normal;
    color: #bb86fc;
    text-shadow: 0 1px 0 rgba(255, 255, 255, 0.12);
    display: inline-block;
    margin: 0;
    line-height: 1.3;
    position: absolute;
    top: -20px;
    background: #1c1920;
    padding: 0 5px 0 5px;
}

.blur {
    filter: blur(20px);
}

input[type='radio'] {
    display: none;
}

input[type='radio']+label {
    z-index: 10;
    margin: 20px 10px 10px 7px;
    width: 115.5px;
    height: 37.29px;
    line-height: 36.3px;
    position: relative;
    font-size: 13.2px;
    letter-spacing: 0.1em;
    color: #0e0e0e;
    text-shadow: 0 1px 0 rgba(255, 255, 255, 0.1);
    font-weight: bold;
    background-image: linear-gradient(#111, #000);
    border-radius: 3px;
    box-shadow: 0 -1px 0 rgba(0, 0, 0, 0.2), 0 0 0 1px rgba(0, 0, 0, 0.3), 0 1px 0 rgba(255, 255, 255, 0.05), 0 0 3px rgba(255, 255, 255, 0.2);
}

input[type='radio']+label:after {
    z-index: -1;
    content: '';
    cursor: pointer;
    top: 1.98px;
    margin-left: 50%;
    left: -55px;
    width: 110px;
    height: 33px;
    display: block;
    position: absolute;
    background-image: linear-gradient(#444, #373738);
    border-radius: 2px;
    box-shadow: inset 0 2px 1px -1px rgba(255, 255, 255, 0.2), inset 0 -2px 1px -1px rgba(0, 0, 0, 0.2), 0 12px 12px rgba(0, 0, 0, 0.5), 0 4px 6px rgba(0, 0, 0, 0.3);
}

a.hover,
input[type='radio']+label:hover {
    color: #aeaeae;
    text-shadow: 0 -1px 0 rgba(0, 0, 0, 0.4);
}

a.hover:after,
input[type='radio']+label:hover:after {
    background-image: linear-gradient(#02aab0, #00cdac);
}

/* ------------------- Active States -------------------- */
a.active,
a.active:hover,
input[type='radio']:checked+label {
    color: #00ccff;
    text-shadow: 0 -1px 0 rgba(0, 0, 0, 0.3), 0 0px 30px rgba(0, 255, 255, 0.7);
    box-shadow: 0 -1px 0 rgba(0, 193, 255, 0.2), 0 0 0 1px rgba(0, 0, 0, 0.4), 0 1px 0 rgba(255, 255, 255, 0.05), 0 0 20px rgba(0, 193, 255, 0.1), 0 0 30px rgba(0, 193, 255, 0.1);
}

a.active,
a.active:hover {
    text-shadow: 0 -1px 0 rgba(0, 0, 0, 1), 0 0px 30px rgba(0, 255, 255, 1), 0 0px 50px rgba(0, 255, 255, 1);
}

a.active:after,
a.active:hover:after,
input[type='radio']:checked+label:after {
    background-image: linear-gradient(#262627, #2d2d2e);
    box-shadow: inset 0 5px 6px rgba(0, 0, 0, 0.3), inset 0 0 4px rgba(0, 0, 0, 0.9), 0 0 0 black;
}

input[type='radio']+label {
    font-family: 'ModernPictogramsNormal', Arial, sans-serif;
    font-size: 36.6666666667px;
    line-height: 55px;
    letter-spacing: 0;
    width: 55px;
    height: 55px;
}

input[type='radio']+label:after {
    top: 2.64px;
    left: -25px;
    width: 50px;
    height: 50px;
}

.rainbow-text {
    background-image: linear-gradient(90deg, #e646ff, #8737ff);
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
}

.center {
    position: relative;
    left: 10px;
    top: 10px;
}

input[type='checkbox'] {
    margin: 10px;
    position: relative;
    cursor: pointer;
    width: 150px;
    height: 55px;
    align: right;
    -webkit-appearance: none;
    background: linear-gradient(0deg, #333, #000);
    outline: none;
    border-radius: 40px;
    box-shadow: 0 0 0 1px #353535, 0 0 0 3px #3e3e3e, inset 0 0 10px rgba(0, 0, 0, 1);
}

input:checked[type='checkbox']:nth-of-type(1) {
    background: linear-gradient(60deg, #f79533, #f37055, #ef4e7b, #a166ab, #5073b8, #1098ad, #07b39b, #6fba82);
    box-shadow: 0 0 0 1px #353535, 0 0 0 3px #3e3e3e, inset 0 0 10px rgba(0, 0, 0, 1);
}

input:checked[type='checkbox']:nth-of-type(2) {
    background: linear-gradient(60deg, #ef32d9, #00f260, #0575e6);
    box-shadow: 0 0 0 4px #353535, 0 0 0 5px #3e3e3e, inset 0 0 10px rgba(0, 0, 0, 1);
}

input:checked[type='checkbox']:nth-of-type(3) {
    background: linear-gradient(0deg, #70a1ff, #1e90ff);
    box-shadow: 0 0 0 1px #353535, 0 0 0 3px #3e3e3e, inset 0 0 10px rgba(0, 0, 0, 1);
}

input:checked[type='checkbox']:nth-of-type(4) {
    background: linear-gradient(0deg, #e67e22, #f39c12);
    box-shadow: 0 0 0 4px #353535, 0 0 0 5px #3e3e3e, inset 0 0 10px rgba(0, 0, 0, 1);
}

input[type='checkbox']:before {
    content: '';
    position: absolute;
    top: 0;
    left: 0px;
    width: 70px;
    height: 55px;
    background: linear-gradient(0deg, #000, #6b6b6b);
    border-radius: 40px;
    box-shadow: 0 0 0 1px #232323;
    transform: scale(0.98, 0.96);
    transition: 0.5s;
}

input:checked[type='checkbox']:before {
    left: 80px;
}

/*---DOT SEEN IN BEETWEEN-->*/
input[type='checkbox']:after {
    content: '';
    position: absolute;
    top: calc(50% - 2px);
    left: 15px;
    width: 4px;
    height: 4px;
    background: linear-gradient(0deg, #6b6b6b, #000);
    border-radius: 50%;
    transition: 0.5s;
}

input:checked[type='checkbox']:after {
    left: 130px;
}

h1 {
    margin: 20px, 0, 0, 40px;
    padding: 0;
    right: 1%;
    font-family: sans-serif;
    color: #fff;
    font-size: 20px;
    padding: 15px 0;
    text-transform: uppercase;
    letter-spacing: 4px;
    display: inline-block;
    position: absolute;
    align: right;
}

h5 {
    position: absolute;
    left: 50%;
    top: 30px;
    transform: translate(-50%, -50%);
    margin: 0;
    padding: 0;
    font-family: Lucidatypewriter, monospace;
    text-align: center;
    color: #fff;
    font-size: 35px;
    /*padding: 35px 0;*/
    text-transform: uppercase;
    letter-spacing: 4px;
    display: inline-block;
}

/* -------------------  SuperBtn -------------------- */
a.superBtn {
    color: #0e0e0e;
    text-shadow: 0 1px 0 rgba(255, 255, 255, .1);
    font-weight: bold;
    letter-spacing: 0.08em;
    width: 242px;
    height: 39.6px;
    line-height: 39.6px;
    position: relative;
    border-radius: 30px;
    background-image: linear-gradient(#2e3537, #424748);
    box-shadow: inset 0 1px 0 rgba(255, 255, 255, .6), inset 0 -1px 0 rgba(0, 0, 0, .3), inset 0 0 0px 1px rgba(0, 0, 0, .1), 0 0 1px rgba(0, 0, 0, .5), 0 2px 2px rgba(0, 0, 0, .4);
}

a.superBtn:after {
    width: 253px;
    height: 49.5px;
    top: -4.95px;
    margin-left: 50%;
    left: -126.5px;
    content: '';
    display: block;
    border-radius: 30px;
    background-image: linear-gradient(#2af1fc, #17b2e6);
    box-shadow: 0 -1px 1px rgba(0, 0, 0, .7), 0 1px 1px rgba(255, 255, 255, .4), 0 0 1px 1px rgba(0, 0, 0, .3), inset 0 0px 3px rgba(0, 0, 0, .4), inset 0 1px 2px rgba(0, 0, 0, .6);
    position: absolute;
    z-index: -1;
}

a.superBtn:hover {
    color: #aeaeae;
    text-shadow: 0 -1px 0 rgba(0, 0, 0, .8);
    background-image: linear-gradient(#545454, #373738);
}

a.superBtn:hover:after {
    box-shadow: 0 -1px 1px rgba(0, 0, 0, .7), 0 1px 1px rgba(255, 255, 255, .4), 0 0 1px 1px rgba(0, 0, 0, .3), inset 0 0px 3px rgba(0, 0, 0, .4), inset 0 1px 2px rgba(0, 0, 0, .6), 0 0 15px 5px rgba(0, 255, 255, .2);
}

a.superBtn:active {
    color: #0cf;
    text-shadow: 0 -1px 0 rgba(0, 0, 0, .3), 0 0px 30px rgba(0, 255, 255, .7);
    background: #333;
    box-shadow: 0 1px 0px rgba(255, 255, 255, .4), inset 0 3px 3px rgba(0, 0, 0, .7), inset 0 0 4px rgba(0, 0, 0, .9), 0 0 0 black;
}

.button {
    width: 90px;
    padding-top: 18px;
    padding-bottom: 18px;
    text-align: center;
    color: #000;
    text-transform: uppercase;
    font-weight: 600;
    margin-left: 60px;
    margin-bottom: 0px;
    cursor: pointer;
    display: inline-block;
}

.button-1 {
    background-color: transparent;
    border: 2px solid #2af1fc;
    border-radius: 30px;
    -webkit-transition: all .15s ease-in-out;
    transition: all .15s ease-in-out;
    color: #00d7c3;
}

.button-1:hover {
    box-shadow: 0 0 10px 0 #00d7c3 inset, 0 0 20px 2px #00d7c3;
    border: 3px solid #00d7c3;
}
</style>

<script>
    var xmlHttp=createXmlHttpObject();
  function createXmlHttpObject(){
  if(window.XMLHttpRequest){
  xmlHttp=new XMLHttpRequest();
  }else{
  xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');
  }
  return xmlHttp;
  }
  
  var s1val=1;
  var s2val=1;
  var s3val=1;
  var s4val=1;
  var s5val=1;
  var s6val=0;
  
  function handleServerResponse()
  {
        xmlResponse=xmlHttp.responseXML;
        var xmldoc = xmlResponse.getElementsByTagName('response');
        var message = xmldoc[0].firstChild.nodeValue;
        
        if(message == 1){s1val = 1; document.getElementById('checkbox1').checked = true; s6val=1;}else{s1val=0; document.getElementById('checkbox1').checked = false;}
        message1 = xmldoc[1].firstChild.nodeValue;
        if(message1 == 1){s2val = 1; document.getElementById('checkbox2').checked = true; s6val=1;}else{s2val=0; document.getElementById('checkbox2').checked = false;}
        message2 = xmldoc[2].firstChild.nodeValue;
        if(message2 == 1){s3val = 1; document.getElementById('checkbox3').checked = true; s6val=1;}else{s3val=0; document.getElementById('checkbox3').checked = false;}
        message3 = xmldoc[3].firstChild.nodeValue;
        if(message3 == 1){s4val = 1; document.getElementById('checkbox4').checked = true; s6val=1;}else{s4val=0; document.getElementById('checkbox4').checked = false;}
        message4 = xmldoc[4].firstChild.nodeValue;
        s5val = message4; 
        
        if(103>s5val && s5val>80) 
          { 
            document.getElementById('radio1-1').checked = true; 
          } 
          else if(79>s5val && s5val>60)
            {
             document.getElementById('radio1-2').checked = true; 
           }
           else if(59>s5val && s5val>40)
            {
              document.getElementById('radio1-3').checked = true;
           }
           else if(39>s5val && s5val>20) 
           { 
            document.getElementById('radio1-4').checked = true;
           }
           else if(19>s5val && s5val>-1) 
           { 
            document.getElementById('radio1-5').checked = true; 
           }
  }
  
  function process(){
  xmlHttp.open('PUT','xml',true);
  xmlHttp.onreadystatechange=handleServerResponse; 
  xmlHttp.send(null);
  setTimeout('process()',1000);
  }
  function s1(){
  s1val = (s1val==1)?0:1;
  xmlHttp.open('SET','set1ESPval?io1='+s1val+'&io2='+s2val+'&io3='+s3val+'&io4='+s4val+'&io5='+s5val,true);
  xmlHttp.send(null);
  if(s1val==1){document.getElementById('checkbox5').checked = true;s6val=1;}
  }
  function s2(){
  s2val = (s2val==1)?0:1;
  xmlHttp.open('SET','set1ESPval?io1='+s1val+'&io2='+s2val+'&io3='+s3val+'&io4='+s4val+'&io5='+s5val,true);
  xmlHttp.send(null);
  if(s2val==1){document.getElementById('checkbox5').checked = true; s6val=1;}
  }
  function s3(){
  s3val = (s3val==1)?0:1;
  xmlHttp.open('SET','set1ESPval?io1='+s1val+'&io2='+s2val+'&io3='+s3val+'&io4='+s4val+'&io5='+s5val,true);
  xmlHttp.send(null);
  if(s3val==1){document.getElementById('checkbox5').checked = true;s6val=1;}
  }
  function s4(){
  s4val = (s4val==1)?0:1;
  xmlHttp.open('SET','set1ESPval?io1='+s1val+'&io2='+s2val+'&io3='+s3val+'&io4='+s4val+'&io5='+s5val,true);
  xmlHttp.send(null);
  if(s4val==1){document.getElementById('checkbox5').checked = true;s6val=1;}
  }
  function s5(fanval){
  s5val = fanval;
  xmlHttp.open('SET','set1ESPval?io1='+s1val+'&io2='+s2val+'&io3='+s3val+'&io4='+s4val+'&io5='+s5val,true);
  xmlHttp.send(null);
  }
  function s6(){
  if(s6val==0){s1val = 1;s2val = 1;s3val = 1;s4val = 1;}else{s1val = 0;s2val = 0;s3val = 0;s4val = 0;}
  var x='set1ESPval?io1='+s1val+'&io2='+s2val+'&io3='+s3val+'&io4='+s4val+'&io5='+s5val;
  xmlHttp.open('SET',x,true);
  xmlHttp.send(null);
  s6val=!s6val;
  }
  </script>

<body onload='process()'>
    <h5 class='rainbow-text'>SMART HOME</h5>
    <fieldset>
        <h2 class='blur'>Switches!</h2>
        <h2>Switches!</h2>
        <div class='center'>
            <h1> Light </h1>
            <input onclick='s1()' type='checkbox' name='checkbox1' id='checkbox1'> </br>
            <h1> FAN </h1> <input onclick='s2()' type='checkbox' name='checkbox2' id='checkbox2'> </br>
            <h1> NIGHT BULB </h1> <input onclick='s3()' type='checkbox' name='checkbox3' id='checkbox3'> </br>
            <h1> PLUG POINT </h1>
            <input onclick='s4()' type='checkbox' name='checkbox4' id='checkbox4'>
        </div>
    </fieldset><br />
    <fieldset>
        <h2 class='blur'>Fan Speed!</h2>
        <h2>Fan Speed!</h2> <input type='radio' id='radio1-1' onclick='s5(100)' checked='checked' name='radio'> <label for='radio1-1'>1</label> <input type='radio' id='radio1-2' onclick='s5(75)' name='radio'> <label for='radio1-2'>2</label>
        <input type='radio' id='radio1-3' onclick='s5(50)' name='radio'> <label for='radio1-3'>3</label> <input type='radio' id='radio1-4' onclick='s5(25)' name='radio'> <label for='radio1-4'>4</label> <input type='radio' id='radio1-5' onclick='s5(1)' name='radio'> <label for='radio1-5'>5</label>
    </fieldset>
    </br>
    <fieldset>
        <h2 class='blur'>The Super Switch!</h2>
        <h2>The Super Switch!</h2>
        <center> <a onclick='s6()' class='superBtn'>ON/OFF All Switch</a> </center>
    </fieldset><br /><br />
    <fieldset>
        <h2 class='blur'>Config</h2>
        <h2>Config</h2>
        <center> <a href='/conf' class='superBtn'>Configure</a> </center>
    </fieldset>
    </div>
</body>

</HTML>

)html";

void buildXML(){
  XML="<?xml version='1.0' encoding='UTF-8'?>";
  XML+="<main>";
  XML+="<response>";
  XML+=d1;
  XML+="</response>";
  XML+="<response>";
  XML+=d2;
  XML+="</response>";
  XML+="<response>";
  XML+=d3;
  XML+="</response>";
  XML+="<response>";
  XML+=d4;
  XML+="</response>";
  XML+="<response>";
  XML+=d5;
  XML+="</response>";
  XML+="</main>";  
}




void handleXML(){
  buildXML();
  server.send(200,"text/xml",XML);
}

void handle1ESPval(){//IN THIS FUNCTION VALUE IS COMING FROM WEBSERVER HTML PAGE
  OutPin1State = server.arg("io1").toFloat();
  OutPin2State = server.arg("io2").toFloat();
  OutPin3State = server.arg("io3").toFloat();
  OutPin4State = server.arg("io4").toFloat();
  valueforslider=server.arg("io5").toFloat();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Action function is giving output to nodemce(Controlling Relay) 

void action()
{
    digitalWrite(OutPin1, OutPin1State);
  digitalWrite(OutPin2, OutPin2State);
  digitalWrite(OutPin3, OutPin3State);
  digitalWrite(OutPin4, OutPin4State);
  if(chk_valforslider!=valueforslider || chk_valforslider==200)
  {
    Serial.println(valueforslider);
   
  }
  // Updating BLYNK APP 
  Blynk.virtualWrite(V3, OutPin1State);  //Update button widget 
  Blynk.virtualWrite(V4, OutPin2State);  //Update button widget 
  Blynk.virtualWrite(V5, OutPin3State);  //Update button widget 
  Blynk.virtualWrite(V6, OutPin4State);  //Update button widget 
  Blynk.virtualWrite(V10, valueforslider);  //Update button widget
}

         
    
    
          /////// FUNCTION FOR PHYSICAL BUTTON CHECK
        void check_normal_Btn()
    {
      //Switch 1
      
      if (digitalRead(btnPin1) == LOW)
      {
        if(btnPin1old==0 || btnPin1old==2) 
        { OutPin1State=HIGH;
          btnPin1old=1; }
      }else
      {
         if(btnPin1old==1 || btnPin1old==2) 
         {OutPin1State=LOW;
        btnPin1old=0;}
      }
      
      //Switch 2
      
      if (digitalRead(btnPin2) == LOW)
      {
        if(btnPin2old==0 || btnPin2old==2) 
        { OutPin2State=HIGH;
          btnPin2old=1; }
      }else
      {
         if(btnPin2old==1 || btnPin2old==2) 
         { OutPin2State=LOW;
         btnPin2old=0; }
      }
      
      //Switch 3
      
      if (digitalRead(btnPin3) == LOW)
      {
        if(btnPin3old==0 || btnPin3old==2) 
        { OutPin3State=HIGH;
          btnPin3old=1; }
      }else
      {
         if(btnPin3old==1 || btnPin3old==2) 
         { OutPin3State=LOW;
         btnPin3old=0; }
      }
      
      //Switch 4
      
      if (digitalRead(btnPin4) == LOW)
      {
        if(btnPin4old==0 || btnPin4old==2) 
        { OutPin4State=HIGH;
          btnPin4old=1; }
      }else
      {
         if(btnPin4old==1 || btnPin4old==2) 
         { OutPin4State=LOW;
         btnPin4old=0; }
      }
    }

          
          void checkPressButton()
          {
                  if (digitalRead(btnPin1) == LOW)
                  {
                    // btnPinState is used to avoid sequential toggles
                    if (btnPin1State != LOW)
                    {
                      // Toggle relay state
                      OutPin1State = !OutPin1State;
                      digitalWrite(OutPin1, OutPin1State);
                      Blynk.virtualWrite(V3, OutPin1State);  //Update button widget
                    }
                    btnPin1State = LOW; 
                  }
                  else
                  {
                    btnPin1State = HIGH;
                  }
      
      
                  ////for button 2
                  if (digitalRead(btnPin2) == LOW)
                  {
                    // btnPinState is used to avoid sequential toggles
                    if (btnPin2State != LOW)
                    {
                      // Toggle relay state
                      OutPin2State = !OutPin2State;
                      digitalWrite(OutPin2, OutPin2State);
                      Blynk.virtualWrite(V4, OutPin2State);  //Update button widget
                    }
                    btnPin2State = LOW; 
                  }
                  else
                  {
                    btnPin2State = HIGH;
                  }
                  
                  ////for button 3
                  if (digitalRead(btnPin3) == LOW)
                  {
                    // btnPinState is used to avoid sequential toggles
                    if (btnPin3State != LOW)
                    {
                      // Toggle relay state
                      OutPin3State = !OutPin3State;
                      digitalWrite(OutPin3, OutPin3State);
                      Blynk.virtualWrite(V5, OutPin3State);  //Update button widget
                    }
                    btnPin3State = LOW; 
                  }
                  else
                  {
                    btnPin3State = HIGH;
                  }
                  
                  ////for button 4
                  if (digitalRead(btnPin4) == LOW)
                  {
                    // btnPinState is used to avoid sequential toggles
                    if (btnPin4State != LOW)
                    {
                      // Toggle relay state
                      OutPin4State = !OutPin4State;
                      digitalWrite(OutPin4, OutPin4State);
                      Blynk.virtualWrite(V6, OutPin4State);  //Update button widget
                    }
                    btnPin4State = LOW; 
                  }
                  else
                  {
                    btnPin4State = HIGH;
                  }
            }

            void checkPhysicalButton()
            {
               if (String(configStore.switchtype)=="normal")
               {
                 // Setup a function to be called every 100 ms it will check the hard switch action.              
                timer.setInterval(100L, check_normal_Btn);
                Serial.println("Normal Switch is Running Now.... ");
                
               }else if(String(configStore.switchtype)=="press")
               {
                  // Setup a function to be called every 100 ms it will check the hard switch action.              
                  timer.setInterval(100L, checkPressButton);
               Serial.println("Press Switch is Running Now....");
               }
               else 
               {
                  // Setup a function to be called every 100 ms it will check the hard switch action.              
                  timer.setInterval(100L, checkPressButton);
               Serial.println("Switch is not decided so running Defaul Press switch....");
               }


            }
///////////////////////////////////////////////////////////////////////////////////////////////////

//==========================================SETUP======================================
void setup() {
  delay(500);
  Serial.begin(115200);
  
 
  BlynkProvisioning.begin();    
  
  // == I/O PIN INTIALISATION
      
              pinMode(OutPin1, OUTPUT);            //  initialize your pin as output.
              pinMode(btnPin1, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup"
              pinMode(OutPin2, OUTPUT);            //  initialize your pin as output.
              pinMode(btnPin2, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup"
              pinMode(OutPin3, OUTPUT);            //  initialize your pin as output.
              pinMode(btnPin3, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup"
              pinMode(OutPin4, OUTPUT);            //  initialize your pin as output.
              pinMode(btnPin4, INPUT_PULLUP);        //  initialize your pin as input with enable internal pull-up resistor "input_pullup"
    //== I/O pin END              
}


//==================================     LOOP   ==================================================
void loop() {
  // This handles the network and cloud connection
  BlynkProvisioning.run();
  
  if(first_time==1)
  {
    if(WiFi.status() != WL_CONNECTED)
    {//if wifi is not connected then retry connecting
      if(ipprint==1)
      {
          Serial.println("Wifi Disconnected. Retrying to connect to wifi if possible !!!");
          ipprint=0;
          server.stop();
      }
        }else
    {//if wifi is connected
    // Coding for WEBserver start and handling
    if(ipprint==0) // ipprint save the value to print on serial only one time.
      {
        server.begin();
        
        server.on("/", []() {
           server.send(200, "text/html", index_page);
           });
        
        server.on("/conf", []() {
           server.send(200, "text/html", config_page);
           });
        
        server.on("/xml",handleXML);
        server.on("/set1ESPval",handle1ESPval);
        server.on("/reset", []() {
              Serial.println("Resetting Device...");
              enterResetConfig();
            });
        server.on("/reboot", []() {
              Serial.println("Rebooting Device...");
              restartMCU();
            });
        Serial.println(WiFi.localIP());
        ipprint=1;
      }
      
       server.handleClient();
    
    }
    // Run function after second time
    if (switch_flag==0)
    {
            checkPhysicalButton();
            switch_flag=1;       
    }
      timer.run(); 
      action();
  }  
  ////////////////Coding in loop for accepting values from Arduino Fan speed
     
                    
           if (Serial.available() > 0 ) 
            {
              rdata = Serial.read(); 
              myString = myString+ rdata; 
             // Serial.print(rdata);
              if( rdata == '\n')
              {
             //Serial.println(myString);
                valueforslider = myString.toInt();
                myString = "";
                Blynk.virtualWrite(V10, valueforslider);
              // end new code
              }
            }


  /////////////////////////////////////////////////////////////////////////     

  //Serial.println("sudhir1 :" +sudhir1);
  //Serial.println("start :" +start);
  
    d1 =(String)OutPin1State;
  d2 =(String)OutPin2State;    
  d3 =(String)OutPin3State;
  d4 =(String)OutPin4State;
  d5 =(String)valueforslider;
  chk_valforslider=valueforslider;

}

///////////////////////////////////////=========END OF LOOP=======================///////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////
//BLYNK FUNCTION TO CONTROL GPIO  PIN
        
        // You can send commands from Terminal to your hardware. Just use
        // the same Virtual Pin as your Terminal Widget
    
        BLYNK_WRITE(V1)//v1 set to control Terminal Print.
        {
      terminal.clear();
      // if you type "IP" into Terminal Widget - it will respond: "IP ADDRESS:"
      if (String("ip") == param.asStr()||String("IP") == param.asStr()) 
      {
              terminal.println("IP Address : ") ;
        terminal.println(WiFi.localIP());
      } else 
      {
        terminal.print("IP : ");
        terminal.println(WiFi.localIP());
        terminal.println("if you type 'IP' into Terminal Widget - it will respond: 'IP ADDRESS:'");
            }
          // Ensure everything is sent
          terminal.flush();
        }
          // When App button is pushed - switch the state
          // Map this Virtual Pin to your Mobile Blynk apps widget.
          
          BLYNK_WRITE(V3)
          {                           
             OutPin1State = param.asInt();
             digitalWrite(OutPin1, OutPin1State);
          }
      BLYNK_WRITE(V4)
          {                           
             OutPin2State = param.asInt();
             digitalWrite(OutPin2, OutPin2State);
          }
          BLYNK_WRITE(V5)
          {                           
             OutPin3State = param.asInt();
             digitalWrite(OutPin3, OutPin3State);
          }
          BLYNK_WRITE(V6)
          {                           
             OutPin4State = param.asInt();
             digitalWrite(OutPin4, OutPin4State);
          }
      
    
      BLYNK_WRITE(V10) // send Fan values to arduino to set fan speed
       {
         valueforslider = param.asInt(); // assigning incoming value from pin V10 to a variable
          if (chk_valforslider!=valueforslider || chk_valforslider==200)
        {
          Serial.println(valueforslider);
        }
        delay(100); 
       }
 
  // Every time we connect to the cloud...
          BLYNK_CONNECTED()
          {
            // Request the latest state from the server
            
               Blynk.syncVirtual(V1);
               Blynk.syncVirtual(V2);
               Blynk.syncVirtual(V3);
               Blynk.syncVirtual(V4);
               Blynk.syncVirtual(V5);
               Blynk.syncVirtual(V6);
               Blynk.syncVirtual(V10);
          }
          
