# define _CRT_SECURE_NO_WARNINGS 1
#include"ConmentConvert.h"    
int main()
{
	//
	FILE *pfread = NULL;
	FILE *pfwrite = NULL;
	printf("ת����ʼ\n");
	pfread = fopen(INPUTFILENAME, "r");
	if (NULL == pfread)
	{
		perror("open file for read\n");
		exit(EXIT_FAILURE);
	}
	pfwrite = fopen(OUTPUTFILENAME, "w");
	if (NULL == pfwrite)
	{
		fclose(pfread);
		perror("open file for write\n");
		exit(EXIT_FAILURE);
	}
	ConmentConvert(pfread, pfwrite);
	printf("ת������\n");
	system("pause");
	return 0;
}
