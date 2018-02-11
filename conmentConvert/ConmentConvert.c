# define _CRT_SECURE_NO_WARNINGS 1
#include"conmentconvert.h"  

enum STATE state = NUL_STATE;
int frist = 0;
int second = 0;
int third = 0;
void Do_NUL_State(FILE *pfRead, FILE *pfWrite)
{
	frist = fgetc(pfRead);
	switch (frist)
	{
	case '/':
		second = fgetc(pfRead);
		if (second == '*')
		{
			fputc('/', pfWrite);
			fputc('/', pfWrite);
			state = C_STATE;
		}

		else if (second == '/')
		{
			fputc(frist, pfWrite);
			fputc(second, pfWrite);
			state = CPP_STATE;
		}
		else//其他字符直接输出  
		{
			fputc(frist, pfWrite);
			fputc(second, pfWrite);
		}
		break;
	case EOF://文件结束标志  
		state = END_STATE;
		break;
	default:
		fputc(frist, pfWrite);//处理不是注释的字符  
		break;
	}
}

void Do_C_State(FILE *pfRead, FILE *pfWrite)
{
	frist = fgetc(pfRead);
	switch (frist)
	{
	case '*':
		second = fgetc(pfRead);
		switch (second)
		{
		case '*':                  //   6.连续的/***/问题    
			third = fgetc(pfRead);
			fputc(frist, pfWrite);
			if (third == '/')
			{
				state = NUL_STATE;
			}
			break;
		case '/':
			third = fgetc(pfRead);
			if (third == '/')       //5.连续的注释问题/**//**/    
			{
				ungetc('/', pfRead);
				state = NUL_STATE;
				break;
			}
			else
			{
				fputc(third, pfWrite);   //   /* int i = 0; */ \n int j = 0;    
				state = NUL_STATE;
				break;
			}
		default:                   //不是注释的字符    
			fputc(frist, pfWrite);
			fputc(second, pfWrite);
			break;
		}
		break;
	case '\n':
		fputc('\n', pfWrite);          //4.C注释中的多行注释     
		fputc('/', pfWrite);
		fputc('/', pfWrite);
		break;
	case EOF:
		state = END_STATE;
		break;
	default:
		fputc(frist, pfWrite);
		break;
	}
}

void Do_CPP_State(FILE *pfRead, FILE *pfWrite)
{
	frist = fgetc(pfRead);
	switch (frist)
	{
	case '\n':
		fputc(frist, pfWrite);
		state = NUL_STATE;
		break;
	case EOF:
		state = END_STATE;
		break;
	default:
		fputc(frist, pfWrite);
		break;
	}
}


void ConmentConvert(FILE *pfRead, FILE *pfWrite)
{
	while (state != END_STATE)
	{
		switch (state)
		{
		case NUL_STATE:
			Do_NUL_State(pfRead, pfWrite);
			break;
		case C_STATE:
			Do_C_State(pfRead, pfWrite);
			break;
		case CPP_STATE:
			Do_CPP_State(pfRead, pfWrite);
			break;
		case END_STATE:
			break;
		default:
			break;
		}
	}
}



