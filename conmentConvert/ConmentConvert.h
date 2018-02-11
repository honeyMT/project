#define _CRT_SECURE_NO_WARNINGS 1  

#ifndef __CONMENTCONVERT_H  
#define __CONMENTCONVERT_H  

#include<stdio.h>  
#include<stdlib.h>  
#include<string.h>  

#define INPUTFILENAME "hehe.txt"  
#define OUTPUTFILENAME "hehe.txt"  



enum STATE
{
	NUL_STATE,
	C_STATE,
	CPP_STATE,
	END_STATE

};



void ConmentConvert(FILE *pfRead, FILE *pfWrite);
void Do_NUL_State(FILE *pfRead, FILE *pfWrite);
void Do_C_State(FILE *pfRead, FILE *pfWrite);
void Do_CPP_State(FILE *pfRead, FILE *pfWrite);
#endif  