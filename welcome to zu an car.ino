
#define SPEAKER 11
#define NOP do { __asm__ __volatile__ ("nop"); } while (0)
#include "Pitches.h"
#include <IRremote.h>
int RECV_PIN = A4;
IRrecv irrecv(RECV_PIN);
decode_results results;
int on = 0;
unsigned long last = millis();

int tune[]={A,B,CC,DD,EE,AA,GG,EE,A,EE,DD,CC,B,A,B,CC,DD,EE,DD,CC,B,A,B,CC,B,A,Ab,B,
             A,B,CC,DD,EE,AA,GG,EE,A,EE,DD,CC,B,A,B,CC,DD,EE,DD,CC,B,CC,DD,EE,
             GG,AA,EE,DD,EE,DD,EE,GG,AA,EE,DD,EE,DD,EE,DD,CC,B,G,A,G,A,B,CC,DD,EE,A,
             EE,GG,GG,AA,EE,DD,EE,DD,EE,GG,AA,EE,DD,EE,
             AA,BB,CCC,BB,AA,GG,EE,DD,EE,DD,CC,B,G,A
             };  //旋律
float durt[]={1,1,1,1,2,1,1,2,2,1,1,1,1,1,1,1,1,2,1,1,1,1,1,1,1,1,1,1,
                  1,1,1,1,2,1,1,2,2,1,1,1,1,1,1,1,1,2,1,1,2,2,2,2,
                  1,1,1,1,2,1,1,1,1,1,1,2,1,1,1,1,1,1,2,1,1,1,1,1,1,2,
                  1,1,1,1,1,1,2,1,1,1,1,1,1,2,
                  1,1,1,1,1,1,2,1,1,1,1,1,1,2
                  }; //音符时值
  
long run_car = 0x00FF02FD;//按键+
long back_car = 0x00FF9867;//按键-
long left_car = 0x00FFE01F ;//按键左
long right_car = 0x00FF906F;//按键右
long stop_car = 0x00FFA857 ;//按键暂停
long left_turn = 0x00FF6897 ;//按键0
long right_turn = 0x00FFB04F;//按键C
long sing = 0x00FF38C7;//按键5
long dance = 0x00FF4AB5;//按键8






//==============================
int Left_motor_back=9;     //左电机后退
int Left_motor_go=5;     //左电机前进
int Right_motor_go=6;    // 右电机前进
int Right_motor_back=10;    // 右电机后退

void setup()
{
  //初始化电机驱动IO为输出方式
  pinMode(Left_motor_go,OUTPUT); // PIN 5 
  pinMode(Left_motor_back,OUTPUT); // PIN 9 
  pinMode(Right_motor_go,OUTPUT);// PIN 6  
  pinMode(Right_motor_back,OUTPUT);// PIN 10 
  pinMode(13, OUTPUT);////端口模式，输出
  Serial.begin(9600);	//波特率9600
  irrecv.enableIRIn(); // Start the receiver
}
void run(int time)     // 前进
{
  digitalWrite(Right_motor_go,HIGH);  
  digitalWrite(Right_motor_back,LOW);     
 
  digitalWrite(Left_motor_go,HIGH);  
  digitalWrite(Left_motor_back,LOW);
  
  delay(time * 100);   //执行时间，可以调整  
}

void brake(int time)         //刹车，停车
{
  digitalWrite(Right_motor_go,LOW);
  digitalWrite(Right_motor_back,LOW);
  digitalWrite(Left_motor_go,LOW);
  digitalWrite(Left_motor_back,LOW);
  delay(time * 100);//执行时间，可以调整  
}

void left(int time)         //左转(左轮不动，右轮前进)
{
  digitalWrite(Right_motor_go,HIGH);	// 右电机前进
  digitalWrite(Right_motor_back,LOW);
 
  digitalWrite(Left_motor_go,LOW);   //左轮不动
  digitalWrite(Left_motor_back,LOW);

  delay(time * 100);	//执行时间，可以调整  
}

void spin_left(int time)         //左转(左轮后退，右轮前进)
{
  digitalWrite(Right_motor_go,HIGH);	// 右电机前进
  digitalWrite(Right_motor_back,LOW);
  
  digitalWrite(Left_motor_go,LOW);   //左轮后退
  digitalWrite(Left_motor_back,HIGH);
  
  delay(time * 100);	//执行时间，可以调整  
}

void right(int time)        //右转(右轮不动，左轮前进)
{
  digitalWrite(Right_motor_go,LOW);   //右电机不动
  digitalWrite(Right_motor_back,LOW);
  
  digitalWrite(Left_motor_go,HIGH);//左电机前进
  digitalWrite(Left_motor_back,LOW);
  
  delay(time * 100);	//执行时间，可以调整  
}

void spin_right(int time)        //右转(右轮后退，左轮前进)
{
  digitalWrite(Right_motor_go,LOW);   //右电机后退
  digitalWrite(Right_motor_back,HIGH);
  
  digitalWrite(Left_motor_go,HIGH);//左电机前进
  digitalWrite(Left_motor_back,LOW);
  
  delay(time * 100);	//执行时间，可以调整  
}

void back(int time)          //后退
{
  digitalWrite(Right_motor_go,LOW);  //右轮后退
  digitalWrite(Right_motor_back,HIGH);
  
  digitalWrite(Left_motor_go,LOW);  //左轮后退
  digitalWrite(Left_motor_back,HIGH);
  
  delay(time * 100);     //执行时间，可以调整  
}
//发声函数


void delayNANO( unsigned long NANO )      //无返回值,引 NANO 作为延时值
{
  for (unsigned long I=0; I< (unsigned int) (NANO*1.3) ; I++)
    { NOP; }
}
void sz_tone( int SPEAKER_PIN, int FREQUENCY, unsigned long TEMPO )
{
  pinMode( SPEAKER_PIN, OUTPUT);      //定义喇叭接口作为输出

  unsigned long CYCLE = 1000000/FREQUENCY;      //cycle*1000提高换算精确度，提高周期时间
  unsigned long LOOP = ( TEMPO*1000 / CYCLE);   //回圈数=分子*1000倍调回正常比例
  if ( FREQUENCY == 0)                //若频率为0 (休止符)则延迟 TEMPO 时间，不发声
    delay( TEMPO );
  else                                           //若不为0(非休止符)则发声
  {
    for ( unsigned long I=0; I < LOOP; I++ )    //发声长短由loop设定
    {
      digitalWrite( SPEAKER_PIN, HIGH );   //发声音
      delayNANO( CYCLE/2 );                    //延时0.5周期
      digitalWrite( SPEAKER_PIN, LOW );    //不发声
      delayNANO( CYCLE/2 );                    //延时0.5周期
    }  
  }
  delay( int ( TEMPO*0.3 ));            //暂停（音符区的间隔）
}




//接下来就是把它应用了

void playTest()
{
 int length = sizeof(tune)/sizeof(tune[0]);   //计算长度
 for(int x=0; x < length;x++)
  {
    
    int noteDuration = 1000 / durt[x];
    sz_tone( SPEAKER, tune[x], noteDuration);    
  }
  delay(2000);
  

}
void danc()
{    
  int i;
  delay(2000); 
  run(10);
  back(10);
  brake(5);

  for(i=0;i<5;i++)
  {
    run(10);
    brake(1);
  }

  for(i=0;i<5;i++)
  {
    back(10);
    brake(1);
  }

  for(i=0;i<5;i++)
  {
    left(10);
    spin_left(5);
  }

  for(i=0;i<5;i++)
  {
    right(10);
    spin_right(5);
  }

  for(i=0;i<10;i++)
  {
    right(1);
    brake(1);
  }

  for(i=0;i<10;i++)
  {
    left(1);
    brake(1);
  }

  for(i=0;i<10;i++)
  {
    left(3);
    right(3);
  }

  for(i=0;i<10;i++)
  {
    spin_left(3);
    brake(3);
  }

  for(i=0;i<10;i++)
  {
    spin_right(3);
    brake(3);
  }

}



void dump(decode_results *results)
{
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) 
  {
    //Serial.println("Could not decode message");
    brake(2000000);
  } 
  

}

void loop()
{
  if (irrecv.decode(&results)) //调用库函数：解码
  {
    // If it's been at least 1/4 second since the last
    // IR received, toggle the relay
    if (millis() - last > 250) //确定接收到信号
    {
      on = !on;//标志位置反
      digitalWrite(13, on ? HIGH : LOW);//板子上接收到信号闪烁一下led
      dump(&results);//解码红外信号
    }
    if (results.value == run_car )//按键2
      run(200);//前进
    if (results.value == back_car )//按键8
      back(200);//后退
    if (results.value == left_car )//按键4
      left(200);//左转
    if (results.value == right_car )//按键6
      right(200);//右转
    if (results.value == stop_car )//按键5
      brake(200);//停车
    if (results.value == left_turn )//按键1
      spin_left(200);//左旋转
    if (results.value == right_turn )//按键3
      spin_right(200);//右旋转
     if (results.value == sing )//按键5
      playTest();
      if(results.value == dance)//按键8
      danc();
    last = millis();      
    irrecv.resume(); // Receive the next value
  }
}
