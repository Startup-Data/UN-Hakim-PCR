//#include <U8g2lib.h>
//
//#ifdef U8X8_HAVE_HW_SPI
//#include <SPI.h>
//#endif
//#ifdef U8X8_HAVE_HW_I2C
//#include <Wire.h>
//#endif

// Please UNCOMMENT one of the contructor lines below
// U8g2 Contructor List (Frame Buffer)
// The complete list is available here: https://github.com/olikraus/u8g2/wiki/u8g2setupcpp
// Please update the pin numbers according to your setup. Use U8X8_PIN_NONE if the reset pin is not connected
//U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0);

#include <OneWire.h>
//#include <DallasTemperature.h>

#include <SPI.h>
//#include <Wire.h>
//#include <LiquidCrystal.h>
#include <max6675.h>
#include <LiquidCrystal_I2C.h>

#include <DallasTemperature.h>
//#define DS18B20 52

  

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


//=================================================================
//                  PID variables http://electronoobs.com/eng_arduino_tut39_code1.php
//=================================================================
float PID_error = 0;
float previous_error = 0;
float elapsedTime, Time, timePrev;
float PID_value = 0;
float Temp_PID=0;
//PID constants
int kp = 203;   int ki= 7.2;   int kd = 1.04;
int PID_p = 0;    int PID_i = 0;    int PID_d = 0;
unsigned long previousMillis = 0; 
unsigned long currentMillis = 0;
int temp_read_Delay = 500;
float real_temperature = 0;
//float setpoint = 100;
bool pressed_1 = false;
bool pressed_2 = false;

//=================================================================
//               END of PID variables
//=================================================================

//=================================================================
//               Remove temp Sensor Noise
//=================================================================
//Define the number of samples to keep track of. The higher the number, the
// more the readings will be smoothed, but the slower the output will respond to
// the input. Using a constant rather than a normal variable lets us use this
// value to determine the size of the readings array.
const int numReadings = 20;

float readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
float total = 0;                  // the running total
float average = 0;                // the average
//=================================================================
//               END of Remove temp Sensor Noise
//=================================================================

//=================================================================
//               Timer for not change the relay fast !
//=================================================================
//https://forum.arduino.cc/t/want-relay-with-button-and-time-limit-not-sure-if-loop-will-work-correctly/210374/9

//long previousMillis = 0;        // will store last time relay was updated
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 1000;           // 1 second interval at which to stay on (milliseconds)
bool t=0; //index for time reached to its interval

//=================================================================
//             END OF Timer for not change the relay fast !
//=================================================================

//=================================================================
//             Termocouple Settings and ... by max6675 IC
//=================================================================
int SO = 7;
int CS = 6;
int CLK = 5;

#define ONE_WIRE_BUS 2
#define relay1 3
#define relay2 4
float lowtemp=6;
float hightemp=8;
MAX6675 temp(CLK,CS,SO);
//=================================================================
//            END of Termocouple Settings and ... by max6675 IC
//=================================================================

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

 
//LiquidCrystal lcd (1,9,10,11,12,13);

//=================================================================
//                Ports part
//=================================================================

//Ports part
//#define analogPin_hightemp  A0 //the analog input pin attach to
//#define analogPin_lowtemp A1 //the analog input pin attach to
float inputValue1 = 0;//variable to store the value coming from sensor
float inputValue2 = 0;//variable to store the value coming from sensor
int analogPin_hightemp =  A0;//variable to store the value coming from sensor
int analogPin_lowtemp = A1;//variable to store the value requestTemperaturescoming from sensor

int outputValue = 0;//variable to store the output value
//=================================================================
//              END of  Ports part
//=================================================================

//char buffer[5];
//float tempC; // Celcius

//=================================================================
//               Smoothing Function for The temperature Reading Value
//=================================================================

float Smoothing_Value(int inputPin) {
  readIndex = 0;              // the index of the current reading
  total = 0;                  // the running total
  average = 0;
  for (byte i = 0; i == numReadings; i = i + 1) {
  readings[i]=0;
  }
  while (readIndex < numReadings+1) {
    // ...wrap around to the beginning:
  
    
    // read from the sensor:
    lowtemp=analogRead(inputPin);
    readings[readIndex] = map(lowtemp, 0, 1023, 0,100); //Convert from 0-1023 proportional to the number of a number of from 0 to 255

//    Serial.print("Smoothing_Value (readings[readIndex])iS: ");
//    Serial.println(readings[readIndex] );
//    Serial.print("total iS: ");
//    Serial.println(total);
    
    // add the reading to the total:
    total = total + readings[readIndex];
    // advance to the next position in the array:
    readIndex = readIndex + 1;
    delay(2);
  }

    // calculate the average:
    average = total / float(numReadings);
    // send it to the computer as ASCII digits
//    Serial.print("average iS: ");
//    Serial.println(average);
   
  return average;
}
//=================================================================
//             END of Smoothing Function for The temperature Reading Value
//=================================================================

//=================================================================
//              PID Function for better ontrol On Temprature
//=================================================================

float PID_Temp(float setpoint,float previous_error ){
    currentMillis = millis();           //Save the value of time before the loop
   /*  We create this if so we will read the temperature and change values each "temp_read_Delay"
    *  value. Change that value above iv you want. The MAX6675 read is slow. Tha will affect the
    *  PID control. I've tried reading the temp each 100ms but it didn't work. With 500ms worked ok.*/
  if(currentMillis - previousMillis >= temp_read_Delay){
    previousMillis += temp_read_Delay;              //Increase the previous time for next loop
    real_temperature = sensors.getTempCByIndex(0);//temp.readCelsius();  //get the real temperature in Celsius degrees

    PID_error = setpoint - real_temperature;        //Calculate the pid ERROR
    
    if(PID_error > 10)                              //integral constant will only affect errors below 30ºC             
    {PID_i = 0;}
    
    PID_p = kp * PID_error;                         //Calculate the P value
    PID_i = PID_i + (ki * PID_error);               //Calculate the I value
    timePrev = Time;                    // the previous time is stored before the actual time read
    Time = millis();                    // actual time read
    elapsedTime = (Time - timePrev) / 1000;   
    PID_d = kd*((PID_error - previous_error)/elapsedTime);  //Calculate the D value
    PID_value = PID_p + PID_i + PID_d;                      //Calculate total PID value

    //We define firing delay range between 0 and 7400. Read above why 7400!!!!!!!
    if(PID_value < 0)
    {      PID_value = 0;       }
    if(PID_value > 7400)
    {      PID_value = 7400;    }
    //Printe the values on the LCD
//    lcd.clear();
//    lcd.setCursor(0,0);
//    lcd.print("Set: ");
//    lcd.setCursor(5,0);
//    lcd.print(setpoint);
//    lcd.setCursor(0,1);
//    lcd.print("PID_error:");
//    lcd.setCursor(11,1);
//    lcd.print(PID_error);
//    delay(800);
    previous_error = PID_error; //Remember to store the previous error.
    return PID_value,previous_error;
  }
}


//=================================================================
//           END of PID Function for better ontrol On Temprature
//=================================================================

//=================================================================
//           Setup
//=================================================================

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
//  lcd.begin(16,2);
  delay(500);

  //Remove Noise from chaninge in temp Value https://www.arduino.cc/en/Tutorial/BuiltInExamples/Smoothing

    for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }

  //LiquidCrystal_I2C https://github.com/johnrickman/LiquidCrystal_I2C/
    lcd.init();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(3,0);
  lcd.print("Hello, world!");
  lcd.setCursor(2,1);
  lcd.print("Ywrobot Arduino!");
  lcd.setCursor(0,2);
  lcd.print("Arduino LCM IIC 2004");
  lcd.setCursor(2,3);
  lcd.print("Power By Ec-yuan!");
  lcd.clear();
// u8g2.begin();
}
//=================================================================
//           END of Setup
//=================================================================
 
//=================================================================
//             Loop
//=================================================================

void loop(void)
{
//  lcd.clear();
//Pot Part hightemp
  inputValue1 = Smoothing_Value(analogPin_hightemp); //analogRead(analogPin_hightemp);//read the  value from the potentiometer

  //Serial.print("Input: "); //print "Input"

  //Serial.println(inputValue1); //print inputValue

  hightemp = inputValue1;//map(inputValue1, 0, 1023, 0,100); //Convert from 0-1023 proportional to the number of a number of from 0 to 255

  Serial.print("Hightemp_Limit_POT: "); //print "Output"

  Serial.println(hightemp); //print outputValue
  lcd.setCursor(0,1);
  lcd.print("PH:");
  lcd.print(hightemp);

//End Of Pot Part lowtemp

//Pot Part lowtemp
  inputValue2 = Smoothing_Value(analogPin_lowtemp);//analogRead(analogPin_lowtemp);//read the  value from the potentiometer

  //Serial.print("Input_LowTemp_Limit_POT: "); //print "Input"

  //Serial.println(inputValue2); //print inputValue

  lowtemp = inputValue2;//map(inputValue2, 0, 1023, 0,100); //Convert from 0-1023 proportional to the number of a number of from 0 to 255

  Serial.print("LowTemp_Limit_POT: "); //print "Output"

  Serial.print(lowtemp); //print outputValue
  lcd.setCursor(8,1);
  lcd.print(" PL:");
  lcd.print(lowtemp);
//



Serial.println(""); //print inputValue
Serial.println(""); //print inputValue
Serial.println(""); //print inputValue
Serial.println(""); //print inputValue
Serial.println(""); //print inputValue
Serial.println(""); //print inputValue

PID_value,previous_error=PID_Temp(hightemp,previous_error);
 unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    // save the last time you switched the relay 
    previousMillis += currentMillis; 
    t=1; 
  }

//End Of Pot Part lowtemp
//
////The LCD Showing  And Termocouple Reading by max6675 
////  sprintf(buffer, "%s", temp.readCelsius());
////  tempC=temp.readCelsius();
////  String mystring = String(temp.readCelsius());
//  lcd.setCursor(0,0);
//  lcd.print("TmpCo:");
//  lcd.print(temp.readCelsius());
//  lcd.print("C");
//
////  delay(1000);  
////  lcd.setCursor(0,1);
////  lcd.print("Far.:");
////  lcd.print((temp.readCelsius()*9/5)+32);
////  lcd.print("F");
//////  
////  Serial.print("Cel.:");
////  Serial.print(temp.readCelsius());
////  Serial.print("   Far.:");
////  Serial.print((temp.readCelsius()*9/5)+32);
////  Serial.println("");
//
// 
//
//  lcd.setCursor(11,0);
//  lcd.print(" RL:Off");
//      
//
//  if  (lowtemp > hightemp){
//  Serial.println(" LowTemp_Limit_POT is > HighTemp_Limit_POT !!!");
//  lcd.print("Miss");}  
//  
////  else if ((PID_value < hightemp) and (PID_value > lowtemp) and (t==1)){
//
//  else if ((temp.readCelsius() < hightemp) and (temp.readCelsius() > lowtemp) and (t==1)){
////    delayMicroseconds(maximum_firing_delay - PID_value); //This delay controls the po
//    delayMicroseconds( PID_value); //This delay controls the po
//    digitalWrite(relay1, LOW);
//    digitalWrite(relay2, LOW);
//    Serial.println("Relays On");
//    lcd.print("On");
//   
//    t=0;}
//  else if (t==1) {
//    digitalWrite(relay1, HIGH);
//    digitalWrite(relay2, HIGH);
//    Serial.println("Relays Off");
//    lcd.print("Off");
//   
//    t=0;}
//   delay(1000);
////END of The LCD Showing  And Termocouple Reading by max6675 
//
  lcd.setCursor(0,0);
  sensors.requestTemperatures(); 
  Serial.print("Temp:");
  Serial.println();
  Serial.print((char)176);//shows degrees character
  Serial.print("C  ");
  lcd.print("Tmp:");
  lcd.print(sensors.getTempCByIndex(0));
  lcd.print("C");
  
//  if (sensors.getTempCByIndex(0) < lowtemp){
//    digitalWrite(relay1, HIGH);
//    digitalWrite(relay2, HIGH);
//    Serial.println("Relays On");}
//  if (sensors.getTempCByIndex(0) > hightemp){
//    digitalWrite(relay1, LOW);
//    digitalWrite(relay2, LOW);
//    Serial.println("Relays Off");}

//
//  Serial.println(""); //print inputValue
//  Serial.println(""); //print inputValue
//


  
  lcd.setCursor(11,0);
//  lcd.print(" RL:Off");
      

  if  (lowtemp > hightemp){
  Serial.println(" LowTemp_Limit_POT is > HighTemp_Limit_POT !!!");
  lcd.print("Miss");}  
  
//  else if ((PID_value < hightemp) and (PID_value > lowtemp) and (t==1)){

  else if ((sensors.getTempCByIndex(0) < hightemp) and (sensors.getTempCByIndex(0) > lowtemp) and (t==1)){
//    delayMicroseconds(maximum_firing_delay - PID_value); //This delay controls the po
    delayMicroseconds( PID_value); //This delay controls the po
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);    
    Serial.println("Relays On");
//    lcd.setCursor(9,0);
    lcd.print("RL:On  ");
   
    t=0;}
  else if (t==1) {
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    Serial.println("Relays Off");
    lcd.print("RL:Off  ");
//    lcd.setCursor(0,0);
//    lcd.print("hi-se: ");
//    lcd.print(hightemp-sensors.getTempCByIndex(0));
//   
    t=0;}
//    else{
//
//    lcd.setCursor(0,0);
//    lcd.print("hi-se: ");
//    lcd.print(hightemp-sensors.getTempCByIndex(0));
//    lcd.setCursor(0,1);
//    lcd.print("T : ");
//    lcd.print(t);
//    lcd.print("  is");
//    t=0; 
//    }
//   delay(500);
//END of The LCD Showing  And Termocouple Reading by max6675 


  }
    
