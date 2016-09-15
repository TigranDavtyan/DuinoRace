#define data PORTD
#define rs 8
#define rw 9
#define e 10
void send_character(unsigned char character)
{
  check_if_busy();
  data = character;
  digitalWrite(rs,1);
  digitalWrite(e,1);
  delay(1);
  digitalWrite(e,0); 
}
void send_command(unsigned char command)
{
  check_if_busy();
  data = command;
  digitalWrite(rs,0);
  digitalWrite(e,1);
  delay(1);
  digitalWrite(e,0); 
}

void clearLCD()
{
  send_command(0x01);
}

void check_if_busy()
{
  pinMode(rw,INPUT);
  while(digitalRead(9)>=0x80){}
  pinMode(rw,OUTPUT);
}

void set_cursor(unsigned char y,unsigned char x)
{
 send_command(0x80 + y*64 + x);
}
void send_string(char * str)
{
  char i=0;
  while(str[i]!='\0')
    send_character(str[i++]);
}
void beginLCD()
{
  DDRD = B11111111;
  pinMode(rs,OUTPUT);
  pinMode(e,OUTPUT);
  delay(15);
  
  send_command(0x01);
  send_command(0x38);
  send_command(B00001111);
  send_command(B00111100);
}

void check_if_busy()
{
  pinMode(rw,INPUT);
  while(digitalRead(9)>=0x80){}
  pinMode(rw,OUTPUT);
}






unsigned char qanak=3,g=3;
long speedd=1000,s=1000,speed_t=0,avto_t=0,tt;
unsigned char road[2][16],l=1;
void setup()
{
  beginLCD();
  tt=millis();
  for(int i=0;i<2;i++)
    for(int j=0;j<16;j++)
      {
        road[i][j]=' ';
      }
}

bool side = false,start=true;

void loop()
{
  randomSeed(analogRead(A0));
  if(analogRead(A5)<300)
    side=true;
  else 
    if(analogRead(A5)>500)
      side=false;
  if(speedd<2000)
  {
  if(side)
  {
     road[0][0]=62;
     road[1][0]=' ';
  }
  else 
  {
    road[0][0]=' ';
    road[1][0]=62;
  }

  avto_t+=millis()-tt;
  speed_t+=millis()-tt;

   if(start)
      {
        if(g==3)
        {
          road[0][11]=road[1][11]=road[1][10]=road[0][9]=road[1][9]=road[1][8]=road[0][7]=road[1][7]=255;
          }
          if(avto_t >=1000)
          {
            avto_t=0;
                   g--;
                  if(g==2)
                  {
                    road[0][8]=255;
                    road[1][8]=' ';
           
                  }
                  if(g==1)
                  {
                    road[0][11]=road[0][9]=road[0][8]=road[0][7]=' ';
                    road[1][8]=road[0][10]=255;
                  }
                if(g==0)
                {
                   road[1][11]= road[1][10]= road[1][9]= road[1][8]= road[1][7]=road[0][10]=' ';
                  start=false;
                }
     
          }
      }
  else
  {
    if(speedd<=200)
      qanak=5;
  if(avto_t >= speedd * qanak)
    {
      avto_t=0;
        long r=random(10);
        if(r>=5)
          road[1][15]=62;
        else road[0][15]=62;
      
    }
  }
 tt=millis();
  if(tt / 2000 >= l)
  {
    if(speedd > 100)
    {
    speedd-=50;
    s-=50;
    }
    else if(speedd>80)
    {
      speedd-=20;
      s-=20;
      }
    l++;
  }
  if(analogRead(A4)<300)
  {
    speedd=100;
  }
  else speedd=s;
  
 if(speed_t>=speedd && !start)
 {
  speed_t=0;
   for(int i=0;i<2;i++)
    for(int j=0;j<15;j++)
      {
        if(road[i][j]==62 && road[i][j+1]==62)
        {
          speedd=100000;
          s=100000;
        }
        road[i][j]=road[i][j+1];
      }
      road[0][15]=road[1][15]=' ';
  }
 en:
  for(int i=0;i<2;i++)
    for(int j=0;j<16;j++)
    {
      set_cursor(i,j);
      send_character(road[i][j]);
    }
  }
  else
  {
    if(side)
  {
     road[0][0]='*';
     if(avto_t>700)
        {
          road[0][1]=road[1][0]='*';
          }
          if(avto_t>1400)
          {
             road[0][1]=road[1][0]=' ';
             avto_t=0;
          }
  }
  else 
  {
    if(avto_t>700)
        {
          road[1][1]=road[0][0]='*';
          }
          if(avto_t>1400)
          {
             road[1][1]=road[0][0]=' ';
             avto_t=0;
          }
    road[1][0]='*';
  }
     avto_t+=millis()-tt;
      

     tt=millis();
    for(int i=0;i<2;i++)
      for(int j=0;j<16;j++)
    {
      set_cursor(i,j);
      send_character(road[i][j]);
    }
  }
    
}
