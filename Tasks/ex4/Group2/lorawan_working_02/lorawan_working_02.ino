#include <rn2xx3.h>
#include <SoftwareSerial.h>


SoftwareSerial mySerial(10, 11); // RX, TX

//create an instance of the rn2xx3 library,
//giving the software serial as port to use
rn2xx3 myLora(mySerial);
String test = "";
// the setup routine runs once when you press reset:
void setup()
{
  //output LED pin
  pinMode(13, OUTPUT);
  led_on();

  // Open serial communications and wait for port to open:
  Serial.begin(57600); //serial port to computer
  mySerial.begin(9600); //serial port to radio
  Serial.println("Startup");

  initialize_radio();

  //transmit a startup message
  myLora.tx("Startup message");

  led_off();
  delay(2000);

  
}

void initialize_radio()
{
  //reset rn2483
  pinMode(12, OUTPUT);
  digitalWrite(12, LOW);
  delay(500);
  digitalWrite(12, HIGH);

  delay(100); //wait for the RN2xx3's startup message
  mySerial.flush();

  //Autobaud the rn2483 module to 9600. The default would otherwise be 57600.
  myLora.autobaud();

  //check communication with radio
  String hweui = myLora.hweui();
  while(hweui.length() != 16)
  {
    Serial.println("Communication with RN2xx3 unsuccessful. Power cycle the board.");
    Serial.println(hweui);
    delay(10000);
    hweui = myLora.hweui();
  }

  //print out the HWEUI so that we can register it via ttnctl
  Serial.println("When using OTAA, register this DevEUI: ");
  Serial.println(myLora.hweui());
  Serial.println("RN2xx3 firmware version:");
  Serial.println(myLora.sysver());

  //configure your keys and join the network
  Serial.println("Trying to join Koliot");
  bool join_result = false;

  /*
   * OTAA: initOTAA(String AppEUI, String AppKey);
   * If you are using OTAA, paste the example code from the koliot.cesnet.cz console here:
   */
 // const char *appEui = "70B3D57EF0003B59";
 // const char *appKey = "20190309100413640586029885528663";
const char *appEui = "70B3D57ED0019112";
const char *appKey = "F7B40B586BE32A50DA361994380B3689";

  join_result = myLora.initOTAA(appEui, appKey);


  while(!join_result)
  {
    Serial.println("Unable to join. Are your keys correct, and do you have Koliot coverage?");
    delay(60000); //delay a minute before retry
    join_result = myLora.init();
  }
  Serial.println("Successfully joined Koliot");

}

// the loop routine runs over and over again forever:
void loop()
{
  
    led_on();

    digitalWrite(2, HIGH);
  delay(100);
  int reading = analogRead(0);
  float voltage = reading * 5.0;
  voltage /= 1024.0;
  Serial.print("Aktualni napeti je :");
  Serial.print(voltage);
  Serial.print(" V\n");
  float temp = (voltage - 0.5) * 100;
  Serial.print("Teplota je ");
  Serial.print(temp);
  Serial.print("\n");
  digitalWrite(2, LOW);
  delay(5000);


String message;
message.concat("Napeti: ");
message.concat(voltage);
message.concat("V, ");
message.concat("Teplota: ");
message.concat(temp);
message.concat("C, ");
message.concat("kru0142 ");


    
    Serial.println(message);
    myLora.tx(message);
    led_off();
    delay(10000);
}

void led_on()
{
  digitalWrite(2, 1);
}

void led_off()
{
  digitalWrite(2, 0);
}
