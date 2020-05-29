#include <LiquidCrystal.h>
#include <DallasTemperature.h>
#include <OneWire.h>
#include <SD.h>

#define ONE_WIRE_BUS 2 


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensor(&oneWire);

//delay

int wait = 0;
int timestamp = 0;
//SD card
const int chipSelect = 10;

//Analog
const int pin_A3 = A3;

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
  lcd.print("T1:    T2:   ");
  lcd.setCursor(0,1);
  lcd.print("T3:    T4:   ");
  Serial.begin(9600);

  if (!SD.begin(chipSelect)) //checks for the card
    Serial.println("Card failed, or not present");
  else
    Serial.println("SD ok");

  

}
void loop() {

  int button = analogRead(0);
  if((button > 600)&&(button < 800))
    showpw(button);
  boot();
  int temp[] = {read_temp(0),read_temp(1),read_temp(2),read_temp(3)};

  send2lcd(temp);
  int  pw = power();
  writeSD(temp,pw);
  
}

void boot(){
  lcd.setCursor(0,0);
  lcd.print("T1:");
  lcd.setCursor(7,0);
  lcd.print("T2:");
  lcd.setCursor(0,1);
  lcd.print("T3:");
  lcd.setCursor(7,1);
  lcd.print("T4:");
  }

void showpw(int button){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("V:");
  while((button > 600)&&(button < 800)){
    button = analogRead(0);
    lcd.setCursor(3,0);
    lcd.print(analogRead(pin_A3));
    delay(500);
  }
  }
  
int power(){
  int pw = 0;
  while( pw == 0 ){
    pw = analogRead(pin_A3);}// to clean false 0's
  
  if(pw < 10)//the engine is off
    wait = 5000;
    delay(wait);

  if((pw > 10) && (pw < 100)) //low
    wait = 1000;
    delay(wait);

  if(pw > 100)//high
    wait = 500;
    delay(wait);

    return pw;
  }

int read_temp(int address){ //reads the temp by IC2
  sensor.requestTemperaturesByIndex(address);
  int leitura = sensor.getTempCByIndex(address);
  return leitura;
  }
  
void send2lcd(int temp[]){ //writes on the display the temps
  lcd.setCursor(3,0);
  if(validate(temp[0]))
    lcd.print(temp[0]);
  else
    lcd.print("DC");

  lcd.setCursor(10,0);
  if(validate(temp[1]))
    lcd.print(temp[1]);
  else
    lcd.print("DC");

  lcd.setCursor(3,1);
  if(validate(temp[2]))
    lcd.print(temp[2]);
  else
    lcd.print("DC");
    
  lcd.setCursor(10,1);
  if(validate(temp[3]))
    lcd.print(temp[3]);
  else
    lcd.print("DC");
   
  }

bool validate(int value){ //checks if the temp os valid
  if((value != -127)&&(value != 85))
    return true;
  else
    return false;
  }
  
void writeSD(int temp[], int pw){ //writes on the SD card the temps
  int count = 0;
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    while(count < 4){
    dataFile.print("Probe id: ");
    dataFile.print(count);
    dataFile.print(" => ");
    dataFile.println(temp[count]);
    count++;
    }
    pw = (pw * 220)/100; //pw is now wattage
    dataFile.print("Power: ");
    dataFile.print(" => ");
    dataFile.println(pw);
    dataFile.print("TimeStamp: ");
    dataFile.print(" => ");
    dataFile.println(timestamp);
    wait = wait / 100;
    timestamp = timestamp + wait;
    dataFile.println("===========");
    dataFile.close();
    Serial.println("Card save");
  }
}
