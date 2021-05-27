#include <OneWire.h>
#include <DallasTemperature.h>

#include <SPI.h>
#include <Wire.h>
//#include <LiquidCrystal.h>
#include <max6675.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// %%%%%%%%%% Remove temp Sensor Noise %%%%%%%%%%
//Define the number of samples to keep track of. The higher the number, the
// more the readings will be smoothed, but the slower the output will respond to
// the input. Using a constant rather than a normal variable lets us use this
// value to determine the size of the readings array.
const int numReadings = 10;

float readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
float total = 0;                  // the running total
float average = 0;                // the average

// %%%%%%%%%% END of Remove temp Sensor Noise %%%%%%%%

// %%%% Timer for not change the relay fast ! %%%%%%%%%
//https://forum.arduino.cc/t/want-relay-with-button-and-time-limit-not-sure-if-loop-will-work-correctly/210374/9

long previousMillis = 0;        // will store last time relay was updated
// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 3000;           // 3 second interval at which to stay on (milliseconds)
bool t=0; //index for time reached to its interval
// %%%% END OF  Timer for not change the relay fast ! %%%%%%%%%

int SO = 7;
int CS = 6;
int CLK = 5;

#define ONE_WIRE_BUS 2
#define relay1 3
#define relay2 4
float lowtemp=6;
float hightemp=8;
 
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

 
//LiquidCrystal lcd (1,9,10,11,12,13);
MAX6675 temp(CLK,CS,SO);

//Pot part
//#define analogPin_hightemp  A0 //the analog input pin attach to
//#define analogPin_lowtemp A1 //the analog input pin attach to
float inputValue1 = 0;//variable to store the value coming from sensor
float inputValue2 = 0;//variable to store the value coming from sensor
int analogPin_hightemp =  A0;//variable to store the value coming from sensor
int analogPin_lowtemp = A1;//variable to store the value coming from sensor

int outputValue = 0;//variable to store the output value
//END of the Pot part
char buffer[5];
float tempC; // Celcius

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

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
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
//  
}
 
 
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
Serial.println(""); //print inputValue
Serial.println(""); //print inputValue
Serial.println(""); //print inputValue
Serial.println(""); //print inputValue
Serial.println(""); //print inputValue
Serial.println(""); //print inputValue



//End Of Pot Part lowtemp

//The LCD Showing  And Termocouple Reading by max6675 
//  sprintf(buffer, "%s", temp.readCelsius());
  tempC=temp.readCelsius();
  String mystring = String(temp.readCelsius());
  lcd.setCursor(0,0);
  lcd.print("Temp:");
  lcd.print(mystring);
  lcd.print("C");
  
//  lcd.setCursor(0,1);
//  lcd.print("Far.:");
//  lcd.print((temp.readCelsius()*9/5)+32);
//  lcd.print("F");
  
  Serial.print("Cel.:");
  Serial.print(temp.readCelsius());
  Serial.print("   Far.:");
  Serial.print((temp.readCelsius()*9/5)+32);
  Serial.println("");

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > interval) {
    // save the last time you switched the relay 
    previousMillis = currentMillis; 
    t=1; 
  }
  lcd.setCursor(10,0);
  lcd.print(" RL:");

  if  (lowtemp > hightemp){
  Serial.println(" LowTemp_Limit_POT is > HighTemp_Limit_POT !!!");
  lcd.print("Miss");}  
  else if ((temp.readCelsius() < hightemp) and (temp.readCelsius() > lowtemp) and (t==1)){
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    Serial.println("Relays On");
    lcd.print("On");
    
    t=0;}
  else if (t==1) {
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    Serial.println("Relays Off");
    lcd.print("Off");
    t=0;}
   
   delay(200);
//END of The LCD Showing  And Termocouple Reading by max6675 


  sensors.requestTemperatures(); 
  Serial.print("Temp Cel 1.:");
  Serial.println(sensors.getTempCByIndex(0));
//  if (sensors.getTempCByIndex(0) < lowtemp){
//    digitalWrite(relay1, HIGH);
//    digitalWrite(relay2, HIGH);
//    Serial.println("Relays On");}
//  if (sensors.getTempCByIndex(0) > hightemp){
//    digitalWrite(relay1, LOW);
//    digitalWrite(relay2, LOW);
//    Serial.println("Relays Off");}


  Serial.println(""); //print inputValue
  Serial.println(""); //print inputValue

  
  }
    
