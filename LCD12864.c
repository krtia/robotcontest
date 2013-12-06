#include"msp430f5438.h"
#include"TimerA1.h"
#include"delay.h"

//�û����ú�����LCD12864_drawDot();LCD12864_drawLine();DisplayCgrom();

/*12864Ӧ��ָ��*/
#define CLEAR_SCREEN 0x01      //����ָ�������ACֵΪ00H
#define AC_INIT  0x02      //��AC����Ϊ00H�����α��Ƶ�ԭ��λ��
#define CURSE_ADD 0x06      //�趨�α��Ƶ�����ͼ�������ƶ�����Ĭ���α����ƣ�ͼ�����岻����
#define FUN_MODE 0x30      //����ģʽ��8λ����ָ�
#define DISPLAY_ON 0x0c      //��ʾ��,��ʾ�α꣬���α�λ�÷���
#define DISPLAY_OFF 0x08      //��ʾ��
#define CURSE_DIR 0x14      //�α������ƶ�:AC=AC+1
#define SET_CG_AC 0x40      //����AC����ΧΪ��00H~3FH
#define SET_DD_AC 0x80

/*12864���Ŷ���*/
#define RS_CLR  P8OUT &= ~BIT5        //RS�õ�
#define RS_SET  P8OUT |= BIT5         //RS�ø�
#define RW_CLR  P8OUT &= ~BIT6        //RW�õ�
#define RW_SET  P8OUT |= BIT6         //RW�ø�
#define EN_CLR  P8OUT &= ~BIT7        //E�õ�
#define EN_SET  P8OUT |= BIT7         //E�ø�
#define PSB_CLR P8OUT &= ~BIT0        //PSB�õͣ����ڷ�ʽ
#define PSB_SET P8OUT |= BIT0         //PSB�øߣ����ڷ�ʽ
#define RST_CLR P8OUT &= ~BIT1        //RST�õ�
#define RST_SET P8OUT |= BIT1         //RST�ø�
#define DataOut P9DIR |= 0xFF         //P9��Ϊ���ݿ�
#define DataIn  P9DIR &= ~0xFF
#define InData  P9IN
#define OutData P9OUT

//����Ƿ�æ������ȴ�
void LCD12864_chkbsy()
{
    unsigned char read;
    DataIn;
    
    do{
     RS_CLR;
     RW_SET;
     EN_SET;
     _NOP();
     read=InData;
     EN_CLR;
    }while((read&0x80)!=0);
}

//д�����
void LCD12864_wrCmd(char cmd)
{
    LCD12864_chkbsy();
    RS_CLR;
    RW_CLR;
    EN_SET;
    DataOut;  
    OutData = cmd;
    EN_CLR;
}


//д���ݺ���
void LCD12864_wrDat(char dat)
{
    LCD12864_chkbsy();
    RS_SET;
    RW_CLR;
    EN_SET;
    DataOut;
    OutData = dat;   
    EN_CLR;
}

char LCD12864_rdDat(void)
{
    char dat;
    LCD12864_chkbsy();
    RS_SET;
    RW_SET;
    EN_SET;
    DataIn; 
    dat = InData;
    EN_CLR;
    return dat;
}

void LCD12864_clnGDR(void)
{
    char i,j;
    LCD12864_wrCmd(0x34); //������ָ�
    for (i=0;i<32;i++)
    {
      LCD12864_wrCmd(0x80+i);//��дY��ַ(����)
      LCD12864_wrCmd(0x80); //��дX��ַ(����)
      for (j=0;j<32;j++)
        LCD12864_wrDat(0x00);
    }
    LCD12864_wrCmd(0x30);//�ر�����ָ�
}

//���㺯��,���Ͻ�Ϊ�ο��㣨0��0��
//���½�Ϊ��63,127������������ʽΪ(�����꣬������)
//����type�������û��ڵ㡢�׵��ȡ�����ڱ�ף��ױ�ڣ�
//type = 0Ϊ��ɫ��1 Ϊ��ɫ��2Ϊȡ��
void LCD12864_drawDot(char x, char y, char type){
  
 unsigned char X,Y,k;  //X�洢�е�ַ��Y�洢�е�ַ
                       //k�洢�������е�λ�ô�������Ϊ0~15
 unsigned char DH,DL;  //��Ŷ������ݵĸ��ֽں͵��ֽ�
 
  if(y <= 63 && x <= 127)
  { 
    if(y < 32)     //ȷ��������ĵ�ַ�����е�ַ
    {
     X = 0x80 + (x >> 4);
     Y = 0x80 + y; 
    }
    else
    {
     X = 0x88 + (x >> 4);
     Y = 0x80 + (y - 32);
    }
         
    LCD12864_wrCmd(0x34);  //������չָ��رջ�ͼ��ʾ
    LCD12864_wrCmd(Y);  //д����ȷ���ĵ����λ��ַ  
    LCD12864_wrCmd(X);   //д����ȷ���ĵ�����ֵ�ַ
   
    DH = LCD12864_rdDat(); //�ٶ�
    DH = LCD12864_rdDat();    //�����ֽ�
    DL = LCD12864_rdDat(); //�����ֽ�
   
    k = x % 16;          //����Ϊ�������е�λ��
   
    //����
    switch(type){     //�������ͣ�1�ڻ�0�׻�2ȡ��
     
     case 0:
      if(k < 8)   //���ڸ��ֽ�
      {
        DH &= ~(0x01 << (7 - k));  //�޸ĸõ�ͬʱ��������λ����
      }
      else        //���ڵ��ֽ�
      {         
        DL &= ~(0x01 << (7 - (k % 8)));  //�޸ĸõ�ͬʱ��������λ����
      }
       break;
     case 1:
      if(k < 8)
      {
        DH |= (0x01 << (7 - k));  //�޸ĸõ�ͬʱ��������λ����
      }
      else
      {
        DL |= (0x01 << (7 - (k % 8))); //�޸ĸõ�ͬʱ��������λ����
      }
       break;
     case 2:
      if(k < 8)
      {
        DH ^= (0x01 << (7 - k));  //�޸ĸõ�ͬʱ��������λ����
      }
      else
      {        
        DL ^= (0x01 << (7 - (k % 8)));   //�޸ĸõ�ͬʱ��������λ����
      }
       break;
     default:
       break;  
    }
    
    LCD12864_wrCmd(Y);     //д��λ��ַ
    LCD12864_wrCmd(X);     //д���ֵ�ַ
   
    LCD12864_wrDat(DH);  //�����ֽ�����д��
    LCD12864_wrDat(DL);  //�����ֽ�����д��
   
    LCD12864_wrCmd(0x30);  //ת����ָͨ��
  }
}


/********************************************************
* ���� LCD12864_drawLine()
      ���ò���ɭ��ķ(Bresenham)�㷨����
* ���ܣ�����������ֱ�ߡ�����Ӳ���ص㣬ʵ�ּ��١�
* ��ڲ�����x0       ֱ����������е�λ��
*         y0       ֱ����������е�λ��
*         x1     ֱ���յ������е�λ��
'       y1     ֱ���յ������е�λ��
* ���ڲ�����   ��
* ˵��������ʧ��ԭ����ָ����ַ������������Χ��
*********************************************************/
void LCD12864_drawLine(char x0, char y0, char x1, char y1, char type)
{
  signed int temp;
  signed int dx,dy;               //������㵽�յ�ĺᡢ����������ֵ
  signed char s1,s2,status,i;
  signed int Dx,Dy,sub;

  dx=x1-x0;
  if(dx>=0)                 //X�ķ��������ӵ�
    s1=1;
  else                     //X�ķ����ǽ��͵�
    s1=-1;
  dy=y1-y0;                 //�ж�Y�ķ��������ӻ��ǽ�����
  if(dy>=0)
    s2=1;
  else
    s2=-1;
    
  Dx=(x1-x0)*s1;             //����ᡢ�ݱ�־����ֵ�ľ���ֵ
  Dy=(y1-y0)*s2;
  if(Dy>Dx)                                
  {                         //��45�Ƚ�Ϊ�ֽ��ߣ�����Y����status=1,����X����status=0 
    temp=Dx;
    Dx=Dy;
    Dy=temp;
    status=1;
  } 
  else
    status=0;
  
  /********�жϴ�ֱ�ߺ�ˮƽ��********/
  if(dx==0)                   //������û����������һ����ֱ��
  {
    for(i=y0;i!=y1;i+=s2)
      LCD12864_drawDot(x0, i, type);
    return;
  }
  if(dy==0)                   //������û����������һ��ˮƽ��
  {
    for(i=x0;i!=x1;i+=s1)
      LCD12864_drawDot(i, y0, type);
    return;
  }
  
  
  /*********Bresenham�㷨������������ֱ��********/ 
    sub=(Dy<<1)-Dx;                 //��1���ж��¸����λ��
    for(i=0;i<Dx;i++)
    { 
      LCD12864_drawDot(x0, y0, type);           //���� 
      if(sub>=0)                               
      { 
        if(status==1)               //�ڿ���Y������xֵ��1
          x0+=s1; 
        else                     //�ڿ���X������yֵ��1               
          y0+=s2; 
        sub-=Dx<<1;                 //�ж����¸����λ�� 
      } 
      if(status==1)
        y0+=s2; 
      else       
        x0+=s1; 
      sub+=Dy<<1; 
    } 
}

//��ڲ�����addr����Ҫд����ʼ��ַ
//          hz�������ַ�����ָ��
void DisplayCgrom(char addr,char *hz)
{
  LCD12864_wrCmd(addr);
  delayms(5);
  while(CountDown);  
  while(*hz != '\0')
  {
    LCD12864_wrDat(*hz);
    hz++;
    delayms(5);
    while(CountDown);   
  }
} 

void Display(void)
{
//  DisplayCgrom(0x80,"Hello World!");
//  DisplayCgrom(0x88,"by: ��Ӣ��");
  char i;
  while(1)
  {
  LCD12864_clnGDR();

  LCD12864_drawLine(0, 63, i, 0, 1);
  LCD12864_drawLine(127,63,i++,0,1);
  if (i>127) i = 0;
  LCD12864_wrCmd(0x36);

  delayms(30);  //��ʱ
  while(CountDown);
  
  LCD12864_wrCmd(0x34);
  }

//  LCD12864_wrCmd(0x30);  //��������ָ�  
}

//��Ҫ���ڿ����ж�֮��
void LCD12864_init()
{
    P8DIR |= 0xFF;
    PSB_SET;
    while(delayms(5));
    DataOut;
    OutData = 0;
    while(CountDown);
    RST_SET;
    RST_CLR;
    RST_SET;
    
    LCD12864_wrCmd(FUN_MODE);   //��ʾģʽ����
    delayms(5);
    while(CountDown);    
    LCD12864_wrCmd(FUN_MODE);   //��ʾģʽ����
    delayms(5);
    while(CountDown);    
    LCD12864_wrCmd(DISPLAY_ON);   //��ʾ��
    delayms(5);
    while(CountDown);    
    LCD12864_wrCmd(CLEAR_SCREEN);   //����
    delayms(5);
    while(CountDown);    
}

