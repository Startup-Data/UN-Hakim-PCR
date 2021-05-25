#include <OneWire.h>
#include <DallasTemperature.h>

#include <SPI.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <max6675.h>


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

 
LiquidCrystal lcd (1,9,10,11,12,13);
MAX6675 temp(CLK,CS,SO);

//Pot part
#define analogPin_hightemp  A0 //the analog input pin attach to
#define analogPin_lowtemp A1 //the analog input pin attach to
int inputValue1 = 0;//variable to store the value coming from sensor
int inputValue2 = 0;//variable to store the value coming from sensor

int outputValue = 0;//variable to store the output value
//END of the Pot part

void setup(void)
{
  Serial.begin(9600);
  sensors.begin();
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  lcd.begin(16,2);
  delay(500);
}
 
 
void loop(void)
{
  
//Pot Part hightemp
  inputValue1 = analogRead(analogPin_hightemp);//read the  value from the potentiometer

  //Serial.print("Input: "); //print "Input"

  //Serial.println(inputValue1); //print inputValue

  hightemp = map(inputValue1, 0, 1023, 0,100); //Convert from 0-1023 proportional to the number of a number of from 0 to 255

  Serial.print("Hightemp_Limit_POT: "); //print "Output"

  Serial.println(hightemp); //print outputValue



//End Of Pot Part lowtemp

//Pot Part lowtemp
  inputValue2 = analogRead(analogPin_lowtemp);//read the  value from the potentiometer

  //Serial.print("Input_LowTemp_Limit_POT: "); //print "Input"

  //Serial.println(inputValue2); //print inputValue

  lowtemp = map(inputValue2, 0, 1023, 0,100); //Convert from 0-1023 proportional to the number of a number of from 0 to 255

  Serial.print("LowTemp_Limit_POT: "); //print "Output"

  Serial.println(lowtemp); //print outputValue

Serial.println(""); //print inputValue
Serial.println(""); //print inputValue
Serial.println(""); //print inputValue
Serial.println(""); //print inputValue
Serial.println(""); //print inputValue
Serial.println(""); //print inputValue



//End Of Pot Part lowtemp

//The LCD Showing  And Termocouple Reading by max6675 
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.println("Cel.:");
  lcd.print(temp.readCelsius());
  lcd.print("C");
  Serial.print("Cel.:");
  Serial.print(temp.readCelsius());
  lcd.setCursor(0,1);
  lcd.print("Far.:");
  lcd.print((temp.readCelsius()*9/5)+32);
  lcd.print("F");
  Serial.print("   Far.:");
  Serial.print((temp.readCelsius()*9/5)+32);
  Serial.println("");
   
  if (temp.readCelsius() < lowtemp){
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    Serial.println("Relays On");}
  if (temp.readCelsius() > hightemp){
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    Serial.println("Relays Off");}
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
    
