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
#include <DallasTemperature.h>

#include <SPI.h>
#include <Wire.h>
//#include <LiquidCrystal.h>
#include <max6675.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


/// ******PT100 Reading    ****** ///
//https://stackoverflow.com/questions/30012866/how-to-read-temperature-using-arduino-uno-board-with-pt100-rtd-sensor

float in[] = { 100.00, 100.39, 100.78, 101.17, 101.56, 101.95, 102.34, 102.73, 103.12, 103.51,
               103.90, 104.29, 104.68, 105.07, 105.46, 105.85, 106.24, 106.63, 107.02, 107.40,
               107.79, 108.18, 108.57, 108.96, 109.35, 109.73, 110.12, 110.51, 110.90, 111.29,
               111.67, 112.06, 112.45, 112.83, 113.22, 113.61, 114.00, 114.38, 114.77, 115.15,
               115.54, 115.93, 116.31, 116.70, 117.08, 117.47, 117.86, 118.24, 118.63, 119.01,
               119.40, 119.78, 120.17, 120.55, 120.94, 121.32, 121.71, 122.09, 122.47, 122.86,
               123.24, 123.63, 124.01, 124.39, 124.78, 125.16, 125.54, 125.93, 126.31, 126.69,
               127.08, 127.46, 127.84, 128.22, 128.61, 128.99, 129.37, 129.75, 130.13, 130.52 };

// known resistance in voltage divider
int R1 = 330/2;//217;
#define Pt100_pin A2

float MultiMap(float val, float* _in, uint8_t size)
{
  // calculate if value is out of range 
//  if (val < _in[0] ) return -99.99;
//  if (val > _in[size-1] ) return 99.99;

  //  search for 'value' in _in array to get the position No.
  uint8_t pos = 0;
  while(val > _in[pos]) pos++;  

  // handles the 'rare' equality case
  if (val == _in[pos]) return pos;

  float r1 = _in[pos-1];
  float r2 = _in[pos];
  int c1 = pos-1;
  int c2 = pos;

 return c1 + (val - r1) / (r2-r1) * (c2-c1);
}

float Pt100_reading() {
  // put your main code here, to run repeatedly:
   int pt100 = analogRead(Pt100_pin);


   float Vout = pt100 * (5.0 / 1023.0);
   float R2 = R1 * 1/(5.0/Vout - 1);

float c =  MultiMap(R2,in,80);

Serial.print("Resistance: ");
Serial.print(R2);
Serial.println(" Ohm");


Serial.print("Vout: ");
Serial.print(Vout);
Serial.println(" V");

Serial.print("pt100 temp: ");
Serial.print(c);
Serial.println(" C");


return c;
}
//
////More Information at: https://www.aeq-web.com/
////Version 1.0 | 03-DEC-2020
//
//const int PT1000_PIN = A5;
//const float vt_factor = 1.88;
//const float offset = 0;
//
//float temp_c;
//
//
//
//float Pt100_reading() {
//  int sensorvalue = analogRead(PT1000_PIN);
//  float voltage = sensorvalue * (5.0 / 1023.0);
//  temp_c = (((voltage * 100) / vt_factor) + offset);
//  Serial.print(voltage);
//  Serial.print(" V Temp: ");
//  Serial.println(temp_c, 1);
//  return temp_c;
//}

//// *** END of Pt100 Reading *****///

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
//char buffer[5];
//float tempC; // Celcius

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
// u8g2.begin();
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
//



Serial.println(""); //print inputValue
Serial.println(""); //print inputValue
Serial.println(""); //print inputValue
Serial.println(""); //print inputValue
Serial.println(""); //print inputValue
Serial.println(""); //print inputValue



//End Of Pot Part lowtemp

//The LCD Showing  And Termocouple Reading by max6675 
//  sprintf(buffer, "%s", temp.readCelsius());
//  tempC=temp.readCelsius();
//  String mystring = String(temp.readCelsius());
//  lcd.setCursor(0,0);
//  lcd.print("T1:");
//  lcd.print(temp.readCelsius());
//  lcd.print("C");
//
//  lcd.print("Pt:");
//  lcd.print(Pt100_reading());
//  lcd.print("C");
//  delay(1000);  
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
   delay(500);
//END of The LCD Showing  And Termocouple Reading by max6675 
 lcd.setCursor(0,0);
  lcd.print("T1:");
  lcd.print(temp.readCelsius());
  lcd.print("C");

  lcd.print("Pt:");
  lcd.print(Pt100_reading());
  lcd.print("C");

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
    
