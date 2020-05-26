#include <LiquidCrystal.h>
#include <DallasTemperature.h>
#include <OneWire.h>

#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensor(&oneWire);

//LCD pin to Arduino
const int pin_RS = 8; 
const int pin_EN = 9; 
const int pin_d4 = 4; 
const int pin_d5 = 5; 
const int pin_d6 = 6; 
const int pin_d7 = 7; 
const int pin_BL = 10; 
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);
void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("[ESM tools]");
  lcd.setCursor(8,1);
  lcd.print(":FJ-RATO");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("T1:  C|T2:  C");
  lcd.setCursor(0,1);
  lcd.print("T3:  C|T4:  C");
  Serial.begin(9600);
}
void loop() {
  int temp[] = {read_temp(0),read_temp(1),read_temp(2),read_temp(3)};
  send2lcd(temp);
}

int read_temp(int address){
  sensor.requestTemperaturesByIndex(address);
  int leitura = sensor.getTempCByIndex(address);
  return leitura;
  }
  
void send2lcd(int temp[]){
  lcd.setCursor(3,0);
  if(validate(temp[0]))
    lcd.print(temp[0]);
  else
    lcd.print("DC?");

  lcd.setCursor(10,0);
  if(validate(temp[1]))
    lcd.print(temp[1]);
  else
    lcd.print("DC?");

  lcd.setCursor(3,1);
  if(validate(temp[2]))
    lcd.print(temp[2]);
  else
    lcd.print("DC?");
    
  lcd.setCursor(10,1);
  if(validate(temp[3]))
    lcd.print(temp[3]);
  else
    lcd.print("DC?");
   
  }

bool validate(int value){
  if((value != -127)&&(value != 85))
    return true;
  else
    return false;
  }
