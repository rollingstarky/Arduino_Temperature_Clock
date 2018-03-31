/******************************************************************************
----------------1.开发环境:Arduino IDE-----------------------------------------
----------------2.测试使用开发板型号：Arduino UNO R3-------
----------------3.单片机使用晶振：16M------------------------------------------
----------------4.作者：starky---------------------------------------------
******************************************************************************/
#include <Arduino.h>
#include "DS1302.h"
#include "DHT.h"
#include <LiquidCrystal.h>

#define DHTPIN 10
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
#define Buzz_Pin 5                 // 有源蜂鸣器引脚
//String comdata="";                 // 定义字符串表示串口输入
unsigned char Alarm_Hour=6;          // 闹钟小时
unsigned char Alarm_Minute=30;        // 闹钟分钟
bool Always=false;                 //是否不间断地输出时间到串口

LiquidCrystal lcd(12, 11, 6, 7, 8, 9);

void setup()
{
  lcd.begin(16, 2);
  dht.begin();
  pinMode(Buzz_Pin,OUTPUT);
  DS1302_Init();
  Serial.begin(9600); 
  Serial.println("Welcome to use!");  
  DS1302_GetTime(&DS1302Buffer);
  Display_RTCC();
  Serial.println("Type \"help\" To Get The Help Information.");
}

//显示时间
void Display_RTCC()
{
  
  if(DS1302Buffer.Year < 10)
  {
    Serial.print("200");
  }
  else
  {
    Serial.print("20");
  }
  Serial.print(DS1302Buffer.Year);
  Serial.print('-');
  Serial.print(DS1302Buffer.Month);
  lcd.setCursor(0,0);
  lcd.print(DS1302Buffer.Month);
  Serial.print('-');
  lcd.print('-');
  Serial.print(DS1302Buffer.Day);
  lcd.print(DS1302Buffer.Day);
  Serial.print("   ");
  lcd.print(" ");

  switch(DS1302Buffer.Week)
  {
    case 1:
        Serial.println("Mon");         //显示星期一
        break;
    case 2:
        Serial.println("Tue");         //显示星期二
        break;
    case 3:
        Serial.println("Wed");         //显示星期三
        break;
    case 4:
        Serial.println("Thu");         //显示星期四
        break;
    case 5:
        Serial.println("Fri");         //显示星期五
        break;
    case 6:
        Serial.println("Sat");         //显示星期六
        break;
    case 7:
        Serial.println("Sun");         //显示星期日
        break;
    default : break;            
  }
  Serial.print(DS1302Buffer.Hour);
  lcd.print(DS1302Buffer.Hour);
  Serial.print(':');
  lcd.print(':');
  Serial.print(DS1302Buffer.Minute);
  lcd.print(DS1302Buffer.Minute);
  Serial.print(':');
  lcd.print(':');
  Serial.println(DS1302Buffer.Second);
  lcd.print(DS1302Buffer.Second);
  Display_DHT();
}

// 获取串口输入
void Scan_Input()
{
  String comdata=""; 
  while (Serial.available() > 0)  
   {
     comdata += char(Serial.read());
     delay(2);
   }
  if(comdata.length()==20 && comdata.startsWith("set"))
    {
      Set_Time(comdata);
    }
  if(comdata=="show")
  {
    Serial.println("The Time Now Is:");
    Always=false;
    Display_RTCC();
  }

//是否不间断地将时间输出到串口
  if(comdata=="always")
  {
    Always=!Always;
  }

//显示闹钟
  if(comdata=="alarm")
  {
    Serial.print("Alarm Set to ");
    Serial.print(Alarm_Hour); 
    Serial.print(":");
    Serial.println(Alarm_Minute);   
  }
  if(comdata=="help")
  {
    help();
  }
}

//时间设置函数
void Set_Time(String &command)
{
  DS1302_ON_OFF(0);     //关闭振荡
  DS1302Buffer.Year=command.substring(4,6).toInt();
  DS1302Buffer.Month=command.substring(7,9).toInt();
  DS1302Buffer.Day=command.substring(10,12).toInt();
  DS1302Buffer.Week=command.substring(13,14).toInt();
  DS1302Buffer.Hour=command.substring(15,17).toInt();
  DS1302Buffer.Minute=command.substring(18).toInt();
  DS1302_SetTime( DS1302_YEAR , DS1302Buffer.Year );
  DS1302_SetTime( DS1302_MONTH , DS1302Buffer.Month );
  DS1302_SetTime( DS1302_DAY , DS1302Buffer.Day );
  DS1302_SetTime( DS1302_WEEK , DS1302Buffer.Week );
  DS1302_SetTime( DS1302_HOUR , DS1302Buffer.Hour );
  DS1302_SetTime( DS1302_MINUTE , DS1302Buffer.Minute );
  DS1302_ON_OFF(1);
  Always=false;
  Serial.println("Set Done");
  Display_RTCC();   
  
}

//蜂鸣器报警函数
void buzz(int &times)
{
  for(int i=0;i<times;i++){
    digitalWrite(Buzz_Pin,HIGH);
    delay(650);
    digitalWrite(Buzz_Pin,LOW);
    delay(250);
    }
}

//帮助内容
void help()
{
  Serial.println("************************************* HELP *****************************************");
  Serial.println("Edited By Starky");
  Serial.println("Type \"set YY MM DD WEEK HH MM\" To Set The Time!");
  Serial.println("Such As Type \"set 08 08 08 1 12 00\" To Set The Time As 2008-8-8 Monday 12:00");
  Serial.println("Type \"show\" To Display The Time Now!");
  Serial.println("Type \"always\" To Print The Time Every Second!");
  Serial.println("Type \"alarm\" To Check The Current Alarm");
  Serial.println("Welcome To Use!"); 
  Serial.println("************************************* HELP *****************************************");
}

//温湿度函数
void Display_DHT()
{
//  delay(2000);
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
//  float hic = dht.computeHeatIndex(t, h, false);
  lcd.setCursor(0,1);
  lcd.print("H:");
  lcd.print(h);
  lcd.print("  ");
  lcd.print("T:");
  lcd.print(t);
}

void loop()
{
  Scan_Input();
  DS1302_GetTime(&DS1302Buffer);        //获取当前RTCC值
  if(Always)
  {
    if(Flag_Time_Refresh == 1)
      {
        lcd.clear();
        Flag_Time_Refresh = 0;
        Display_RTCC();
      }
  }
  
  // 闹钟激活
  if(DS1302Buffer.Hour==Alarm_Hour && DS1302Buffer.Minute==Alarm_Minute && DS1302Buffer.Second==0)
  {
    int times=15;        // 报警次数
    buzz(times);
  }
}


