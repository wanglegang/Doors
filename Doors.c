//定义IO口开始
int LuoGan_JiDianQi =D0;  //落杆继电器
int QiGan_JiDianQi =D1;   //起杆位继电器
int LuoDaoWei_JiDianQi =D2; //落到位继电器
int QiDaoWei_JiDianQi =D3;  //起到位继电器
int Run_Led = D4; //运行灯
int QiGan_AnNiu = D5;    //起杆按钮
int LuoGan_AnNiu = D6;   //落杆按钮
int LuoDaoWei_HuoEr =D9; //落到位霍尔传感器
int QiDaoWei_HuoEr =D7;  //起到位霍尔传感器
unsigned long run_high_time,run_low_time;
//定义IO口结束  

void changePort(int flag)//切换起杆落杆相关的引脚
{
  if(flag==0){//起落杆按钮一侧位落杆侧
    //定义IO口开始
    LuoGan_JiDianQi =D0;  //落杆继电器
    QiGan_JiDianQi =D1;   //起杆位继电器
    LuoDaoWei_JiDianQi =D2; //落到位继电器
    QiDaoWei_JiDianQi =D3;  //起到位继电器
    LuoDaoWei_HuoEr =D9; //落到位霍尔传感器
    QiDaoWei_HuoEr =D7;  //起到位霍尔传感器
  }
  if(flag==1){//电源一侧位落杆侧
     //定义IO口开始
    LuoGan_JiDianQi =D1;  //落杆继电器
    QiGan_JiDianQi =D0;   //起杆位继电器
    LuoDaoWei_JiDianQi =D3; //落到位继电器
    QiDaoWei_JiDianQi =D2;  //起到位继电器
//    LuoDaoWei_HuoEr =D7; //落到位霍尔传感器
//    QiDaoWei_HuoEr =D9;  //起到位霍尔传感器
  }
  
}

void setup() {
  Serial.begin(115200);
  // put your setup code here, to run once:
  //定义IO口输入输出开始
  pinMode(Run_Led,OUTPUT);//设置输出模式 运行灯
  pinMode(LuoGan_JiDianQi,OUTPUT);//设置输出模式 落杆继电器
  pinMode(QiGan_JiDianQi,OUTPUT);//设置输出模式 起杆继电器
  pinMode(LuoDaoWei_JiDianQi,OUTPUT);//设置输出模式，落到位继电器，低电平吸合，落杆到位继电器
  pinMode(QiDaoWei_JiDianQi,OUTPUT);//设置输出模式，起到位继电器，低电平吸合，起杆到位继电器
  pinMode(QiGan_AnNiu,INPUT);//设置输入模式，读取起杆按钮
  pinMode(LuoGan_AnNiu,INPUT);//设置输入模式，读取落杆按钮
  pinMode(LuoDaoWei_HuoEr,INPUT);//设置输入模式，读取 起到位 霍尔传感器状态，起杆到位，低电平
  pinMode(QiDaoWei_HuoEr,INPUT);//设置输入模式，读取 落到位 霍尔传感器状态，落杆到位，低电平
  pinMode(QiDaoWei_HuoEr,INPUT);//设置输入模式
  digitalWrite(LuoGan_JiDianQi,HIGH);
  digitalWrite(QiGan_JiDianQi,HIGH);
  digitalWrite(LuoDaoWei_JiDianQi,HIGH);
  digitalWrite(QiDaoWei_JiDianQi,HIGH);
  //定义IO口输入输出结束  
  Serial.print("A0--");
  Serial.println(analogRead(A0));
  delay(10);
  if(analogRead(A0)<600)//起落杆一侧位落杆
    {
      changePort(0);
    }else{
      changePort(1);
    }
}

void loop() {
  Serial.print("A0--");
  Serial.println(analogRead(A0));
  delay(10);
//=======================================================以下运行灯闪烁，勿动===============================================
    if((digitalRead(Run_Led) == LOW)&&(millis()-run_high_time>800)) //当运行灯低电平时且当前时间比黄灯高电平时的时间差大于0.5秒时，频率是1hz
    {
      digitalWrite(Run_Led, HIGH);  //灯亮，20口输出高电平
      run_low_time=millis(); //记住低电平时的时间
    }
    if((digitalRead(Run_Led) == HIGH)&&(millis()-run_low_time>800))//当运行灯为高电平时且当前时间比黄灯低电平时的时间差大于0.5秒时，频率是1hz
   {
     digitalWrite(Run_Led, LOW);  //灯灭，20口输出低电平
     run_high_time=millis();//记住高电平时的时间
   }
//=======================================================以上运行灯闪烁，勿动===============================================      
//=======================================================以下控制程序===============================================  

  if(digitalRead(QiGan_AnNiu)==1 && digitalRead(LuoGan_AnNiu)==0 && digitalRead(LuoDaoWei_HuoEr)!=0)//按下起杆按钮
  {
    if(digitalRead(LuoGan_JiDianQi)==LOW)
     {
      digitalWrite(LuoGan_JiDianQi,HIGH);//落杆停止，落杆继电器高电平触发关闭
      delay(500); 
     }
     digitalWrite(LuoGan_JiDianQi,HIGH);//落杆停止，落杆继电器高电平触发关闭
     digitalWrite(QiGan_JiDianQi,LOW);//开始起杆，起杆继电器低电平触发
     pinMode(QiDaoWei_JiDianQi,OUTPUT); //起到位io置为输出模式
     digitalWrite(QiDaoWei_JiDianQi,HIGH);//起杆到位继电器，高电平断开继电器
     pinMode(LuoDaoWei_JiDianQi,INPUT); //落到位io置为输入模式
     digitalWrite(LuoDaoWei_JiDianQi,HIGH);//落杆到位继电器，高电平断开继电器
  }
  if(digitalRead(LuoDaoWei_HuoEr)==0)//落杆到位
  { 
    digitalWrite(LuoGan_JiDianQi,HIGH);//释放落杆继电器
    digitalWrite(QiGan_JiDianQi,HIGH);//释放起杆继电器
    pinMode(QiDaoWei_JiDianQi,OUTPUT); //起到位io置为输出模式
    digitalWrite(QiDaoWei_JiDianQi,LOW);//起杆到位继电器，低电平吸合继电器
    digitalWrite(LuoDaoWei_JiDianQi,HIGH);//落杆到位继电器关闭
  }
  if(digitalRead(LuoGan_AnNiu)==1 && digitalRead(QiGan_AnNiu)==0 && digitalRead(QiDaoWei_HuoEr)!=0)//按下落杆按钮
  {      
    if(digitalRead(QiGan_JiDianQi)==LOW)
     {
      digitalWrite(QiGan_JiDianQi,HIGH);//起杆停止，起杆继电器高电平触发关闭
      delay(500); 
     }
    digitalWrite(QiGan_JiDianQi,HIGH);//起杆停止
    digitalWrite(LuoGan_JiDianQi,LOW);//开始落杆，继电器低电平触发
    pinMode(LuoDaoWei_JiDianQi,OUTPUT); //落到位io置为输出模式
    digitalWrite(LuoDaoWei_JiDianQi,HIGH);//落杆到位继电器，高电平断开继电器，落到位LED关闭
    pinMode(QiDaoWei_JiDianQi,INPUT); //起到位io置为输入模式
    digitalWrite(QiDaoWei_JiDianQi,HIGH);//起杆到位继电器，高电平断开继电器，起到位LED关闭
  }
  if(digitalRead(QiDaoWei_HuoEr)==0)//起杆到位
  {    
    digitalWrite(QiGan_JiDianQi,HIGH);//释放起杆继电器
    digitalWrite(LuoGan_JiDianQi,HIGH);//释放落杆继电器
    digitalWrite(QiDaoWei_JiDianQi,HIGH);//起杆到位继电器关闭
    pinMode(LuoDaoWei_JiDianQi,OUTPUT); //落到位io置为输出模式
    digitalWrite(LuoDaoWei_JiDianQi,LOW);//落杆到位继电器，低电平吸合继电器
  }
 if(digitalRead(QiGan_AnNiu)==1 && digitalRead(LuoGan_AnNiu)==1)//按下停止按钮
 {
   pinMode(QiGan_JiDianQi,OUTPUT); //落到位io置为输出模式
   digitalWrite(QiGan_JiDianQi,HIGH);//释放起杆继电器
   pinMode(LuoGan_JiDianQi,OUTPUT); //落到位io置为输出模式
   digitalWrite(LuoGan_JiDianQi,HIGH);//释放落杆继电器
   pinMode(QiDaoWei_JiDianQi,OUTPUT); //落到位io置为输出模式
   digitalWrite(QiDaoWei_JiDianQi,HIGH);//起杆到位继电器关闭
   pinMode(LuoDaoWei_JiDianQi,OUTPUT); //落到位io置为输出模式
   digitalWrite(LuoDaoWei_JiDianQi,HIGH);//落到位继电器关闭
 }
 if(digitalRead(LuoDaoWei_HuoEr)==1)//监控霍尔，进行复位
  { 
    digitalWrite(QiDaoWei_JiDianQi,HIGH);//落杆到位继电器关闭
  }
  if(digitalRead(QiDaoWei_HuoEr)==1)//监控霍尔，进行复位
  { 
    digitalWrite(LuoDaoWei_JiDianQi,HIGH);//落杆到位继电器关闭
  }

}
