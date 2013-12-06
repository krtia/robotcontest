#include"msp430f5438.h"
#include"TimerA1.h"
#include"delay.h"

//用户可用函数：LCD12864_drawDot();LCD12864_drawLine();DisplayCgrom();

/*12864应用指令*/
#define CLEAR_SCREEN 0x01      //清屏指令：清屏且AC值为00H
#define AC_INIT  0x02      //将AC设置为00H。且游标移到原点位置
#define CURSE_ADD 0x06      //设定游标移到方向及图像整体移动方向（默认游标右移，图像整体不动）
#define FUN_MODE 0x30      //工作模式：8位基本指令集
#define DISPLAY_ON 0x0c      //显示开,显示游标，且游标位置反白
#define DISPLAY_OFF 0x08      //显示关
#define CURSE_DIR 0x14      //游标向右移动:AC=AC+1
#define SET_CG_AC 0x40      //设置AC，范围为：00H~3FH
#define SET_DD_AC 0x80

/*12864引脚定义*/
#define RS_CLR  P8OUT &= ~BIT5        //RS置低
#define RS_SET  P8OUT |= BIT5         //RS置高
#define RW_CLR  P8OUT &= ~BIT6        //RW置低
#define RW_SET  P8OUT |= BIT6         //RW置高
#define EN_CLR  P8OUT &= ~BIT7        //E置低
#define EN_SET  P8OUT |= BIT7         //E置高
#define PSB_CLR P8OUT &= ~BIT0        //PSB置低，串口方式
#define PSB_SET P8OUT |= BIT0         //PSB置高，并口方式
#define RST_CLR P8OUT &= ~BIT1        //RST置低
#define RST_SET P8OUT |= BIT1         //RST置高
#define DataOut P9DIR |= 0xFF         //P9口为数据口
#define DataIn  P9DIR &= ~0xFF
#define InData  P9IN
#define OutData P9OUT

//检测是否忙，是则等待
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

//写命令函数
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


//写数据函数
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
    LCD12864_wrCmd(0x34); //打开扩充指令集
    for (i=0;i<32;i++)
    {
      LCD12864_wrCmd(0x80+i);//先写Y地址(行数)
      LCD12864_wrCmd(0x80); //再写X地址(列数)
      for (j=0;j<32;j++)
        LCD12864_wrDat(0x00);
    }
    LCD12864_wrCmd(0x30);//关闭扩充指令集
}

//画点函数,左上角为参考点（0，0）
//右下角为（63,127），点坐标形式为(行坐标，列坐标)
//参数type用于设置画黑点、白点或取反（黑变白，白变黑）
//type = 0为白色，1 为黑色，2为取反
void LCD12864_drawDot(char x, char y, char type){
  
 unsigned char X,Y,k;  //X存储行地址，Y存储列地址
                       //k存储点在字中的位置从左至右为0~15
 unsigned char DH,DL;  //存放读出数据的高字节和低字节
 
  if(y <= 63 && x <= 127)
  { 
    if(y < 32)     //确定所画点的地址行与列地址
    {
     X = 0x80 + (x >> 4);
     Y = 0x80 + y; 
    }
    else
    {
     X = 0x88 + (x >> 4);
     Y = 0x80 + (y - 32);
    }
         
    LCD12864_wrCmd(0x34);  //开启扩展指令，关闭绘图显示
    LCD12864_wrCmd(Y);  //写入所确定的点的行位地址  
    LCD12864_wrCmd(X);   //写入所确定的点的列字地址
   
    DH = LCD12864_rdDat(); //假读
    DH = LCD12864_rdDat();    //读高字节
    DL = LCD12864_rdDat(); //读低字节
   
    k = x % 16;          //余数为点在字中的位置
   
    //画点
    switch(type){     //画点类型，1黑或0白或2取反
     
     case 0:
      if(k < 8)   //点在高字节
      {
        DH &= ~(0x01 << (7 - k));  //修改该点同时保持其他位不变
      }
      else        //点在低字节
      {         
        DL &= ~(0x01 << (7 - (k % 8)));  //修改该点同时保持其他位不变
      }
       break;
     case 1:
      if(k < 8)
      {
        DH |= (0x01 << (7 - k));  //修改该点同时保持其他位不变
      }
      else
      {
        DL |= (0x01 << (7 - (k % 8))); //修改该点同时保持其他位不变
      }
       break;
     case 2:
      if(k < 8)
      {
        DH ^= (0x01 << (7 - k));  //修改该点同时保持其他位不变
      }
      else
      {        
        DL ^= (0x01 << (7 - (k % 8)));   //修改该点同时保持其他位不变
      }
       break;
     default:
       break;  
    }
    
    LCD12864_wrCmd(Y);     //写行位地址
    LCD12864_wrCmd(X);     //写列字地址
   
    LCD12864_wrDat(DH);  //将高字节数据写回
    LCD12864_wrDat(DL);  //将低字节数据写回
   
    LCD12864_wrCmd(0x30);  //转回普通指令
  }
}


/********************************************************
* 名称 LCD12864_drawLine()
      采用布兰森汉姆(Bresenham)算法画线
* 功能：任意两点间的直线。根据硬件特点，实现加速。
* 入口参数：x0       直线起点所在行的位置
*         y0       直线起点所在列的位置
*         x1     直线终点所在行的位置
'       y1     直线终点所在列的位置
* 出口参数：   无
* 说明：操作失败原因是指定地址超出缓冲区范围。
*********************************************************/
void LCD12864_drawLine(char x0, char y0, char x1, char y1, char type)
{
  signed int temp;
  signed int dx,dy;               //定义起点到终点的横、纵坐标增加值
  signed char s1,s2,status,i;
  signed int Dx,Dy,sub;

  dx=x1-x0;
  if(dx>=0)                 //X的方向是增加的
    s1=1;
  else                     //X的方向是降低的
    s1=-1;
  dy=y1-y0;                 //判断Y的方向是增加还是降到的
  if(dy>=0)
    s2=1;
  else
    s2=-1;
    
  Dx=(x1-x0)*s1;             //计算横、纵标志增加值的绝对值
  Dy=(y1-y0)*s2;
  if(Dy>Dx)                                
  {                         //以45度角为分界线，靠进Y轴是status=1,靠近X轴是status=0 
    temp=Dx;
    Dx=Dy;
    Dy=temp;
    status=1;
  } 
  else
    status=0;
  
  /********判断垂直线和水平线********/
  if(dx==0)                   //横向上没有增量，画一条垂直线
  {
    for(i=y0;i!=y1;i+=s2)
      LCD12864_drawDot(x0, i, type);
    return;
  }
  if(dy==0)                   //纵向上没有增量，画一条水平线
  {
    for(i=x0;i!=x1;i+=s1)
      LCD12864_drawDot(i, y0, type);
    return;
  }
  
  
  /*********Bresenham算法画任意两点间的直线********/ 
    sub=(Dy<<1)-Dx;                 //第1次判断下个点的位置
    for(i=0;i<Dx;i++)
    { 
      LCD12864_drawDot(x0, y0, type);           //画点 
      if(sub>=0)                               
      { 
        if(status==1)               //在靠近Y轴区，x值加1
          x0+=s1; 
        else                     //在靠近X轴区，y值加1               
          y0+=s2; 
        sub-=Dx<<1;                 //判断下下个点的位置 
      } 
      if(status==1)
        y0+=s2; 
      else       
        x0+=s1; 
      sub+=Dy<<1; 
    } 
}

//入口参数：addr，需要写的起始地址
//          hz，汉字字符串的指针
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
//  DisplayCgrom(0x88,"by: 吴英南");
  char i;
  while(1)
  {
  LCD12864_clnGDR();

  LCD12864_drawLine(0, 63, i, 0, 1);
  LCD12864_drawLine(127,63,i++,0,1);
  if (i>127) i = 0;
  LCD12864_wrCmd(0x36);

  delayms(30);  //延时
  while(CountDown);
  
  LCD12864_wrCmd(0x34);
  }

//  LCD12864_wrCmd(0x30);  //开启基本指令集  
}

//需要放在开总中断之后
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
    
    LCD12864_wrCmd(FUN_MODE);   //显示模式设置
    delayms(5);
    while(CountDown);    
    LCD12864_wrCmd(FUN_MODE);   //显示模式设置
    delayms(5);
    while(CountDown);    
    LCD12864_wrCmd(DISPLAY_ON);   //显示开
    delayms(5);
    while(CountDown);    
    LCD12864_wrCmd(CLEAR_SCREEN);   //清屏
    delayms(5);
    while(CountDown);    
}

