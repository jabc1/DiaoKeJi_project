/*
由于我自己的雕刻机最大的雕刻尺寸为15mm*15mm，s16 为-32767~32767，将坐标位置放大100倍对能够满足雕刻机0.01mm的精度，且所有运算为整型计算
坐标位置X 1.01 Y 2.02 对应的值为整型 X 101 Y 202 这样直线插补和圆弧插补没有使用复杂的浮点运算全部为整型计算。
这样对移植可能有点问题，但对于我自己的机器精度完全足够
*/



#include "g_code.h"
#include "device_conf.h"
#include <math.h>
#include "dk_conf.h"
#include "malloc.h"	


GCodeStruct G_Code;
u8 OneLine[64];						//一条Gcode的ASCII码的储存数组

u8 read_float(u8 *line, uint8_t *char_counter, float *float_ptr);
//---------------------------------------------------------------------------------



#define MAX_INT_DIGITS 	8 // G代码中允许的最大小数点位数 Maximum number of digits in int32 (and float)
#define USE_DIGITS			2	//实际使用的小数点位数为2位


//
u8 read_s32(u8 *line, uint8_t *char_counter, s32 *Val)                  
{
	s32 intval = 0;
  s8 exp = 0;
  u8 ndigit = 0;
  u8 isdecimal = false;  //小数
	u8 isnegative = false;  //负数
//	float fval;
  u8 *ptr = line + *char_counter;
  unsigned char c;
  c = *ptr++;
  if (c == '-') 
	{
    isnegative = true;
    c = *ptr++;
  } 
	else if (c == '+') 
	{
    c = *ptr++;
  }
  while(1) 
	{
    c -= '0';  
    if (c <= 9) //判断是否0-9
		{
      ndigit++;
      if (ndigit <= MAX_INT_DIGITS) 
			{
        if (isdecimal) { exp++; }
        intval = (((intval << 2) + intval) << 1) + c; // intval*10 + c
      } 
			else 
			{
        if (!(isdecimal)) { exp--; }  // Drop overflow digits  超过最大位数后统计
      }
    } 
		else if (c == (('.'-'0') & 0xff)  &&  !(isdecimal))  //判断是否是第一次小数点
		{
      isdecimal = true;
    } 
		else //数字结束
		{
      break;
    }
    c = *ptr++;
  }
  if (!ndigit) { return(false); };  //没有一个数字
	if(exp > USE_DIGITS)	//小数点的数量大于要求值
	{
	intval = intval / pow(10,exp - USE_DIGITS);
	
	}
	else if(exp < USE_DIGITS)
	{
		intval = intval * pow(10,USE_DIGITS - exp);
	}
	else
	{
	}

  if (isnegative) {
    *Val = -intval;
  } else {
    *Val = intval;
  }
  *char_counter = ptr - line - 1; // Set char_counter to next statement
  return(true);
}









u8 read_Str_s32(u8 *line, u8 *char_counter, s32 *Val_f,s32* Val_i)                  
{
	s32 intval = 0;
  s8 exp = 0;
  u8 ndigit = 0;
  u8 isdecimal = false;  //小数
	u8 isnegative = false;  //负数
//	float fval;
  u8 *ptr = line + *char_counter;
  unsigned char c;
  c = *ptr++;
  if (c == '-') 
	{
    isnegative = true;
    c = *ptr++;
  } 
	else if (c == '+') 
	{
    c = *ptr++;
  }
  while(1) 
	{
    c -= '0';  
    if (c <= 9) //判断是否0-9
		{
      ndigit++;
      if (ndigit <= MAX_INT_DIGITS) 
			{
        if (isdecimal) { exp++; }
        intval = (((intval << 2) + intval) << 1) + c; // intval*10 + c
      } 
			else 
			{
        if (!(isdecimal)) { exp--; }  // Drop overflow digits  超过最大位数后统计
      }
    } 
		else if (c == (('.'-'0') & 0xff)  &&  !(isdecimal))  //判断是否是第一次小数点
		{
      isdecimal = true;
			
    } 
		else //数字结束
		{
			break;
    }
    c = *ptr++;
  }
  if (!ndigit) { return(false); };  //没有一个数字
	if(exp > USE_DIGITS)	//小数点的数量大于要求值
	{
	intval = intval / pow(10,exp - USE_DIGITS);
	
	}
	else if(exp < USE_DIGITS)
	{
		intval = intval / pow(10,exp - USE_DIGITS);
	}
	else
	{
	}

  if (isnegative) {
    *Val_f = -intval;
		*Val_i = -intval /pow(10,USE_DIGITS);
  } else {
    *Val_f = intval;
		*Val_i = intval /pow(10,USE_DIGITS);
  }
  *char_counter = ptr - line - 1; // Set char_counter to next statement
  return(true);
}

//

static s32 next_statement(u8 *letter, s32 *Value_f,s32 *Value_i, u8 *line, u8 *char_counter)
{
	while (line[*char_counter] == ' ') (*char_counter)++; //字符串为空格

	if (line[*char_counter] == 0 || line[*char_counter] == ';' ||
		line[*char_counter] == '\n' || line[*char_counter] == '\r') return false;
	*letter = line[*char_counter];
	if ((*letter < 'A') || (*letter > 'Z'))
	{
		DK.GC_Status = GCSTATUS_EXPECTED_COMMAND_LETTER;
		return false;
	}
	(*char_counter)++;
	return read_Str_s32(line, char_counter, Value_f, Value_i);
}


static double to_millimeters(double value)
{
	return (DK.State_B_P->Inches_Mode ? (value * MM_PER_INCH) : value);
}
u8 TransformGCode(u8 *line,GCodeStruct * Queue)//parser_state_t *G_Code)
{
	u8 char_counter = 1;
	u8 letter;
	s32 value_f;
	s32 value_i;
	u8 word_bit = 0; //跟踪变量的位值	
	DK.GC_Status = 0;
	mymemset(Queue,0,sizeof(GCodeStruct));	
	if (line[1] == ';'
		|| line[1] == '('
		|| line[1] == '%'
		)
		return GCSTATUS_OK;	// comments
		DK.N++;
	// Pass 1: Commands
	while (next_statement(&letter, &value_f,&value_i, line, &char_counter))
	{
		switch (letter)
		{
		case 'N':
			break;
		case 'G':
			SetBit(Queue->ValueWords,WORD_GM);
			switch (value_i)
			{
			case 0: 	Queue->Action = GC_SEEK_G0; 			break;		//快速移动
			case 1: 	Queue->Action = GC_LINEAR_G1; 		break;		//直线移动
			case 2: 	Queue->Action = GC_CW_ARC; 				break;
			case 3: 	Queue->Action = GC_CCW_ARC; 			break;
			case 4: 	Queue->Action = GC_DWELL_G4; 			break;		//暂停
			case 20: 	DK.State_B_P->Inches_Mode = true; 	break;
			case 21: 	DK.State_B_P->Inches_Mode = false; 	break;
			case 28: 	Queue->Action = GC_GO_HOME_G28; 	break;//移动到原点 
			case 90: 	DK.State_B_P->Absolute_Mode = true; break;
			case 91: 	DK.State_B_P->Absolute_Mode = false;break;
			case 92: 	Queue->Action = GC_RESET_XYZ_G92; break;
			//G92：定义当前位置 例如：G92X10允许编程的绝对零点，通过重置当前位置为指定的值。这将设置机器的X坐标为10;若没有指定坐标的G92将重置所有轴为零。
			case 30: 	
			case 64:
			case 40:
			case 17:	// G17 蔓犷?疣犷麇?镫铖觐耱?X-Y
			case 94:	// Feedrate per minute
			case 98:	// Feedrate per minute (group type A)
			case 97:	// Constant spindle speed M T Takes an S address integer, which is interpreted as rev/min (rpm). The default speed mode per system parameter if no mode is programmed. 
			case 49:	// Tool length offset compensation cancel
			case 80:	// Cancel canned cycle
				break;
			default: FAIL(GCSTATUS_UNSUPPORTED_STATEMENT);
			}
			break;
		case 'M':
			SetBit(Queue->ValueWords,WORD_GM);
			switch (value_i)
			{
			case 112: // Emergency Stop 
			case 0:
			case 1:
			case 2:
			case 30:
			case 60:
				Queue->Action = GC_STOP;
				break;
			case 3: Queue->Action = GC_M03; break;
			//	case 4: G_Code->spindle_direction = -1; break;
			case 5: Queue->Action = GC_M05; break;
			case 23: // Thread gradual pullout ON
			case 24: // Thread gradual pullout OFF
			case 52: // Unload Last tool from spindle
			case 49: // Feedrate override NOT allowed
			case 48: // Feedrate override allowed
			case 8:  // Coolant on
			case 9:  // Coolant off
			case 105: // M105: Get Extruder Temperature Example: M105 Request the temperature of the current extruder and the build base in degrees Celsius. The temperatures are returned to the host computer. For example, the line sent to the host in response to this command looks like 
			case 106: // M106: Fan On Example: M106 S127 Turn on the cooling fan at half speed. Optional parameter 'S' declares the PWM value (0-255) 
			case 107: // Fan Off 
			case 108: // M108: Set Extruder Speed  Sets speed of extruder motor. (Deprecated in current firmware, see M113) 
			case 110: // Set Current Line Number 
			case 113: // Set Extruder PWM 
			case 140: // Bed Temperature (Fast) Example: M140 S55 Set the temperature of the build bed to 55oC 
			case 141: //Chamber Temperature (Fast) Example: M141 S30 Set the temperature of the chamber to 30oC
			case 142: // Holding Pressure Example: M142 S1 Set the holding pressure of the bed to 1 bar. 
			case 6:
				return DK.GC_Status;
			default: FAIL(GCSTATUS_UNSUPPORTED_STATEMENT);
			}
			break;
		}
		if (DK.GC_Status)
		return (DK.GC_Status);
	}
	if (DK.GC_Status)
		return(DK.GC_Status);

	char_counter = 1;
	while (next_statement(&letter, &value_f,  &value_i,line, &char_counter))
	{
		double unit_millimeters_value = to_millimeters(value_f);
		switch (letter)
		{
		case 'X':
			word_bit = WORD_X;
			if (DK.State_B_P->Absolute_Mode)
				Queue->X  = unit_millimeters_value;
			else
				Queue->X  += unit_millimeters_value;
			break;			
		case 'Y':
			word_bit = WORD_Y;
			if (DK.State_B_P->Absolute_Mode)
				Queue->Y  = unit_millimeters_value;
			else
				Queue->Y  += unit_millimeters_value;
			break;
		case 'Z':
			word_bit = WORD_Z;
			if (DK.State_B_P->Absolute_Mode)
				Queue->Z  = unit_millimeters_value;
			else
				Queue->Z  += unit_millimeters_value;
			break;
		case 'I':
			word_bit = WORD_I; 
			Queue->I = unit_millimeters_value;	
			break;		
		case 'J':
			word_bit = WORD_J;
			Queue->J = unit_millimeters_value;	
			break;		
		case 'K':
			break;		
		case 'E': 
			break;
		case 'F':
			break;
		case 'P':
		case 'S':  
			break;
		case 'R':
		case 'G':
		case 'N':
		case 'M':
			break;
		default:
			FAIL(GCSTATUS_UNSUPPORTED_PARAM);
		}
		if(word_bit)
		{
			if (GetBit(Queue->ValueWords,word_bit)) { FAIL(FAIL_WORD_REPEATED); } // [Word repeated]
			SetBit(Queue->ValueWords,word_bit);			
		}

	}
	
	return(DK.GC_Status);
}












/*

u8 TransformGCode(u8 *line,GCodeStruct * Queue) 
{
	u8 letter;  //字母
	s32 value;
	u8 int_value;
	u8 char_counter = 1; //计数标志
  u8 word_bit; //跟踪变量的位值			 Bit-value for assigning tracking variables	

	mymemset(Queue,0,sizeof(GCodeStruct));
	while(line[char_counter] != 0)  //直到字符串结尾
	{
		letter=line[char_counter];
		char_counter++;
		if(letter!=0x20) //空格
		{
			value = 0;
			read_s32(line, &char_counter, &value);
			int_value=value/pow(10,USE_DIGITS);
			switch(letter)
			{
				case 'G':
					SetBit(Queue->ValueWords,WORD_GM);
					switch(int_value)
					{
							
						case 0:Queue->Action = POS;break;													//快速定位 G00		
						case 1:Queue->Action = LINE;break;												//直线插补 G01
						case 2:Queue->Action = CW_ARC;break;											//顺圆插补 G02
						case 3:Queue->Action = CCW_ARC;break;											//逆圆插补 G03			
						case 21:Queue->Action = 21;	break;		
	//					case 30: XC=0,YC=0,XT=0,YT=0,Xe=0,Ye=0,L=0,F=0,L2=0,S=0,xyz[AXIS_X] =0,xyz[AXIS_Y] =0,xyz[AXIS_Z] =0;break; //设置为坐标零点			
	//					case 75: motion=4,xyz[AXIS_X] =0,xyz[AXIS_Y] =0,xyz[AXIS_Z] =0; break; //回到坐标原点		
						case 90:Queue->Action = 90;	break;						
					}
					break;
				case 'M':
					SetBit(Queue->ValueWords,WORD_GM);
					switch(int_value)
					{
						case 3://正转，对应Z轴向下
							Queue->Action = 103;
							break;				
						case 4://反转
							Queue->Action = 104;					
							break;	
						case 5://停止，对应Z轴向上
							Queue->Action = 105;					
							break;				
					}
					break;
				case 0x0A:					
				case 0x0D:
						return 0;	
//					break;							
				default: 
					switch(letter)
					{
//						case 'F': word_bit = WORD_F; Queue->F= int_value;	 break;
						case 'I': word_bit = WORD_I; Queue->I= value;  break;
						case 'J': word_bit = WORD_J; Queue->J = value; break;
						case 'K': word_bit = WORD_GM;  break;
						case 'L': word_bit = WORD_GM;  break;
						case 'N': word_bit = WORD_N; Queue->N = int_value; break;
						case 'P': word_bit = WORD_GM;  break;
						case 'R': word_bit = WORD_GM;  break;
						case 'S': word_bit = WORD_GM;  break;
						case 'T': word_bit = WORD_GM; break; // G_Code->values.t = int_value;
						case 'X': word_bit = WORD_X; Queue->X = value; break;
						case 'Y': word_bit = WORD_Y; Queue->Y = value; break;
						case 'Z': word_bit = WORD_Z; Queue->Z = value; break;
						default: FAIL(STATUS_GCODE_UNSUPPORTED_COMMAND);
					} 
					if (GetBit(Queue->ValueWords,word_bit)) { FAIL(FAIL_WORD_REPEATED); } // [Word repeated]
					
//					if (bit(word_bit) & (bit(WORD_F)|bit(WORD_N)|bit(WORD_P)|bit(WORD_T)|bit(WORD_S)) ) 
//					{
//						if (value < 0.0) { FAIL(STATUS_NEGATIVE_VALUE); } // [Word value cannot be negative]
//					}
					SetBit(Queue->ValueWords,word_bit);
					break;
			}				
		}
	}
	if(Queue->ValueWords)  { FAIL(FAIL_WORD_NOGCODE); }
	return 0;
}
*/
