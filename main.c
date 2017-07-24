#include <project.h>
#include <stdio.h> //需要使用sprintf
#include<stdlib.h>
/***************************************
函数功能：由数值转换为字符串的函数
****************************************/
void IntToString(char *str, int number)// 由数值变换为字符串的函数
{
    sprintf(str, "%d", number);
}

static int16 Pos[3] = {1,2,3};
void Position(int wr,int wl)
{
static double ins[3] = {0,0,0};
//ins[0] = ins[0] + (1/60)*(36.5/2)*(wr+wl)*(60*3.14/180)*cos(ins[2] +(1/60)*(36.5/176)*(wr-wl))*(60*3.14/180);
ins[0] = ins[0] + 0.3*(wr+wl)*(60*3.14/180)*cos(ins[2] +(1/60)*(36.5/176)*(wr-wl))*(60*3.14/180);
ins[1] = ins[1] + 0.3*(wr+wl)*(60*3.14/180)*sin(ins[2] +(1/60)*(36.5/176)*(wr-wl))*(60*3.14/180);
ins[2] = ins[2] + (wr+wl);
/* 基于机器人位姿的计算公式，参数 60*3.14/180 用于将角速度单位 rpm 转换为 rad/s， 180/3.14
用于将角度单位 rad 转换为° */
Pos[0] = ins[0]; //小车位姿的 X 坐标，单位 mm
Pos[1] = ins[1]; //小车位姿的 Y 坐标，单位 mm
Pos[2] = ins[2]-360*(int)(ins[2]/360); //小车逆时针旋转角度，单位°

char string[50];

IntToString(string, Pos[2]);
        LCD_Char_Position(0,0);
        LCD_Char_PrintString("The IR Distance is ");  
        LCD_Char_Position(1,0);
        //LCD_Char_PrintString("             ");       //输入前清屏        
        LCD_Char_Position(1,0);
        LCD_Char_PrintString(string);        // 显示AD采样值

}

void ByWall(int AjustK)
{

        PWM_WriteCompare(5000-AjustK*10);         
        PWM_2_WriteCompare(4150-AjustK*10);


}

void Forward(int DelayTime)
{

        QuadDec_WriteCounter(0);//解码器计数清零
        QuadDec_T_WriteCounter(0);//解码器计数清零
        for(;;)
        {
            static int Angle,Angle_2,AngleAdjust;
            QuadDec_WriteCounter(0);//解码器计数清零
            QuadDec_T_WriteCounter(0);//解码器计数清零
            
            PWM_WriteCompare(5000+10*AngleAdjust);         
            PWM_2_WriteCompare(4150+10*AngleAdjust);
            
            CyDelay(30);
            
            Angle += QuadDec_ReadCounter()-32768 ;
            Angle_2 +=  QuadDec_T_ReadCounter()-32768;
            
            AngleAdjust += QuadDec_ReadCounter() + QuadDec_T_ReadCounter();

            if(Angle-Angle_2>DelayTime)
            {
                Angle=0;
                Angle_2=0;
                AngleAdjust = 0;
                break;
            }
        }
        
        PWM_WriteCompare(4500);
        PWM_2_WriteCompare(4500);
    
    
}

void Backward()
{
    QuadDec_WriteCounter(0);//解码器计数清零
    QuadDec_T_WriteCounter(0);//解码器计数清零
    for(;;)
    {
        static int Angle,Angle_2,AngleAdjust;
        QuadDec_WriteCounter(0);//解码器计数清零
        QuadDec_T_WriteCounter(0);//解码器计数清零
        
        PWM_WriteCompare(4150-AngleAdjust);         
        PWM_2_WriteCompare(5000+AngleAdjust);
        
        CyDelay(30);
        
        Angle += QuadDec_ReadCounter()-32768 ;
        Angle_2 +=  QuadDec_T_ReadCounter()-32768;
        
        AngleAdjust += QuadDec_ReadCounter() + QuadDec_T_ReadCounter();

            
        if(Angle_2-Angle>38)
        {
            Angle=0;
            Angle_2=0;
            AngleAdjust = 0;
            break;
        }
    }
    
}

void Stop(int StopTime)
{
    PWM_WriteCompare(4500);
    PWM_2_WriteCompare(4500);
    CyDelay(StopTime);
}

void circle()
{
    for(;;)
        {
            static int Angle,Angle_2,AngleAdjust;
            QuadDec_WriteCounter(0);//解码器计数清零
            QuadDec_T_WriteCounter(0);//解码器计数清零
            
            PWM_WriteCompare(4850);
            PWM_2_WriteCompare(4300-600);

            // PWM_WriteCompare(4900-AngleAdjust*5);
            // PWM_2_WriteCompare(4100+AngleAdjust*5);
            
            CyDelay(30);
            
            Angle += QuadDec_ReadCounter()-32768 ;
            Angle_2 +=  QuadDec_T_ReadCounter()-32768;
            
            AngleAdjust += QuadDec_ReadCounter() - QuadDec_T_ReadCounter();

                    char string[30];


        ADC_SAR_Seq_IsEndConversion(ADC_SAR_Seq_WAIT_FOR_RESULT); 
        int dis;
        dis=(int)(0.5/(0.00002*ADC_SAR_Seq_GetResult16(0) -0.00045)-2);


        IntToString(string,dis );
        LCD_Char_Position(0,0);
        LCD_Char_PrintString("The IR Distance is ");  
        LCD_Char_Position(1,0);
        LCD_Char_PrintString("             ");       //输入前清屏        
        LCD_Char_Position(1,0);
        LCD_Char_PrintString(string);        // 显示AD采样值
        
        CyDelay(50);
            if(dis<32)
            {
                Angle=0;
                Angle_2=0;
                AngleAdjust=0;

                

                LCD_Char_Position(0,0);
        LCD_Char_PrintString("jieshule!!");        // 显示AD采样值
        Stop(200);
        


                break;
            }
        }
}


void Turn(int TFlag)
{
    char string[50];
    
    //90 degrees
    if( TFlag )//turnLeft
    {
        for(;;)
        {
            static int Angle,Angle_2,AngleAdjust;
            QuadDec_WriteCounter(0);//解码器计数清零
            QuadDec_T_WriteCounter(0);//解码器计数清零
            
            PWM_WriteCompare(5000-AngleAdjust*5);
            PWM_2_WriteCompare(5000+AngleAdjust*5);
            
            CyDelay(30);
            
            Angle += QuadDec_ReadCounter()-32768 ;
            Angle_2 +=  QuadDec_T_ReadCounter()-32768;
            
            AngleAdjust += QuadDec_ReadCounter() - QuadDec_T_ReadCounter();

                    char string[30];

        IntToString(string,Angle );
        LCD_Char_Position(0,0);
        LCD_Char_PrintString("The IR Distance is ");  
        LCD_Char_Position(1,0);
        LCD_Char_PrintString("             ");       //输入前清屏        
        LCD_Char_Position(1,0);
        LCD_Char_PrintString(string);        // 显示AD采样值
        
        IntToString(string,Angle_2 );
        LCD_Char_Position(0,0);
        LCD_Char_PrintString("The IR Distance is ");  
        LCD_Char_Position(1,0);
        
        LCD_Char_Position(1,5);
        LCD_Char_PrintString(string);        // 显示AD采样值

            if(-Angle-Angle_2>87)
            {
                Angle=0;
                Angle_2=0;
                AngleAdjust=0;
                
                break;
            }
        }
         
        PWM_WriteCompare(4500);
        PWM_2_WriteCompare(4500);
        
        CyDelay(150);
    }
    else//turnRight
    {
        QuadDec_WriteCounter(0);//解码器计数清零
        QuadDec_T_WriteCounter(0);//解码器计数清零
        for(;;)
        {
            static int Angle,Angle_2,AngleAdjust;
            QuadDec_WriteCounter(0);//解码器计数清零
            QuadDec_T_WriteCounter(0);//解码器计数清零
            
            PWM_WriteCompare(4100+10*AngleAdjust);
            
            PWM_2_WriteCompare(4100+10*AngleAdjust);
            
            CyDelay(10);
            
            Angle += QuadDec_ReadCounter()-32768 ;
            Angle_2 +=  QuadDec_T_ReadCounter()-32768;
            
            AngleAdjust += QuadDec_ReadCounter()- QuadDec_T_ReadCounter();

                    
                            char string[30];

        IntToString(string,Angle );
        LCD_Char_Position(0,0);
        LCD_Char_PrintString("The IR Distance is ");  
        LCD_Char_Position(1,0);
        LCD_Char_PrintString("             ");       //输入前清屏        
        LCD_Char_Position(1,0);
        LCD_Char_PrintString(string);        // 显示AD采样值
        
        IntToString(string,Angle_2);
        LCD_Char_Position(0,0);
        LCD_Char_PrintString("The IR Distance is ");  
        LCD_Char_Position(1,0);
        
        LCD_Char_Position(1,5);
        LCD_Char_PrintString(string);        // 显示AD采样值

            if(Angle+Angle_2>18)
            {
                Angle=0;
                Angle_2=0;
                Stop(10);
                break;
            }
        }

    }
    Stop(300);
}

void PIDSpeed(int speed,int speed_2,int PWMAdjust)
{
    char string[50];
    static int times=0,SpeedError = 0;
    SpeedError = 0;
    for(;;)
    {
        
        PWM_WriteCompare(5000+PWMAdjust*50+5*SpeedError);
        
        PWM_2_WriteCompare(4150-PWMAdjust*50+5*SpeedError);
                            
        CyDelay(20);
        speed = QuadDec_ReadCounter()-32768;//每1.67ms读取解码器的计数
        QuadDec_WriteCounter(0);//解码器计数清零
        speed_2 = QuadDec_T_ReadCounter()-32768;//每1.67ms读取解码器的计数
        QuadDec_T_WriteCounter(0);//解码器计数清零
        IntToString(string, speed);
        LCD_Char_Position(0,0);
        LCD_Char_PrintString("The IR Distance is ");  
        LCD_Char_Position(1,0);
        LCD_Char_PrintString("             ");       //输入前清屏        
        LCD_Char_Position(1,0);
        LCD_Char_PrintString(string);        // 显示AD采样值
        
        IntToString(string, -speed_2);
        LCD_Char_Position(1,4);
        LCD_Char_PrintString(string);        // 显示AD采样值
        
        IntToString(string, SpeedError);
        LCD_Char_Position(1,8);
        LCD_Char_PrintString(string);        // 显示AD采样值
        
        SpeedError += speed_2- speed;
        
        if(times>3)
        {times =0;break;}
        else 
        {times++;}
    }
    
}



/****************************************
函数功能：初始化元器件
****************************************/

int main()
{
    uint32 ADC_Result = 0;//ADC_Result;表示数值的变量
    char string[50];// 由数值变换为字符串时存储的阵列
	float dist;
    int dis;
	static int dis_p = 0,dis_d = 0,dis_i = 0;
    CyGlobalIntEnable;          //ADC组件的初始化
    ADC_SAR_Seq_Start();
    ADC_SAR_Seq_StartConvert();    //开始AD转换，因为在组件属性里设定为Continuous，因此以下自动连续采样
    LCD_Char_Start();
    LCD_Char_LoadCustomFonts(LCD_Char_customFonts);
    
    

    static int speed,speed_i,speed_d,speed_2,speed_d_2,speed_i_2;
    QuadDec_Start();
    QuadDec_TriggerCommand(QuadDec_MASK, QuadDec_CMD_RELOAD);
    QuadDec_T_Start();
    QuadDec_T_TriggerCommand(QuadDec_MASK, QuadDec_CMD_RELOAD);
    
    for(;;)
    {
        PWM_Start();
        PWM_2_Start();
        PWM_WriteCompare(4500);
        PWM_2_WriteCompare(4500);
        

        ADC_Result = ADC_SAR_Seq_GetResult16(0); 
        dist = 0.5/(0.00002*ADC_Result -0.00045)-2;
        dis = (int)dist;
        dis_d = (20 -(int)dist) - dis_p;
        dis_p=20 -(int)dist; 
        dis_i += dis_p;    
        
        //PIDSpeed(speed,speed_2,(int)((dis_p+dis_d+dis_i)*0.1));
        //PIDSpeed(speed,speed_2,4);
        IntToString(string, dis);
        LCD_Char_Position(1,10);
        LCD_Char_PrintString(string);        // 显示AD采样值
        
        ADC_SAR_Seq_IsEndConversion(ADC_SAR_Seq_WAIT_FOR_RESULT); 
        ADC_Result = ADC_SAR_Seq_GetResult16(0);                              //结果存储到变量ADC_Result
        if(ADC_Result >0x7FFF)
        {
            ADC_Result =0;
        }
		
        if(ADC_Result>0)
        {
            dist=0.5/(0.00002*ADC_Result -0.00045)-2;
            dis=(int)dist;
            
            if(Button_Read()>0||Button_2_Read()>0)
            {
                Backward();
                Stop(200);
                //  Turn(1);//TurnLeft 90 degrees
                PWM_WriteCompare(5000);
                PWM_2_WriteCompare(5000);
            
                CyDelay(200);
            }
            else
            {
                if(dis<36)
                {
                ByWall(dis-22);//沿墙调用
                }
                else
                {
                    PWM_WriteCompare(5000);         
                    PWM_2_WriteCompare(4150);
                    
                    CyDelay(200);
                    circle();

                }
                
            }
            


            IntToString(string, dis);
            LCD_Char_Position(0,0);
            LCD_Char_PrintString("The IR Distance is ");  
            LCD_Char_Position(1,0);
            LCD_Char_PrintString("      ");       //输入前清屏        
            LCD_Char_Position(1,0);
            LCD_Char_PrintString(string);        // 显示AD采样值
            

            CyDelay(500);
        }
    }
    return 0;
}
