# define _CRT_SECURE_NO_WARNINGS 1
#include<stdio.h>  
#include<time.h>  
#include<stdlib.h>  
#include<Windows.h>  
#include<malloc.h>  
#include<string.h>  


typedef struct point{
	int rows;
	int cols;
	struct point *next;
}point;


void intipointlink(point *head);
void showpoints(point head);
void showonepoint(point head);
void destroy(point *head);
void insert(point *heads);
point *found(point head, int oldrows, int oldcols);
void *create(int rows, int cols);
void *delete(point head, int rows, int cols);
void showtopic(char *topic);
void *ordering(point head);


//����
void *ordering(point head)
{
	point *pi, *pj, *pt, t;
	for (pi = head.next; pi; pi = pi->next)
	for (pj = pi->next; pj; pj = pj->next)
	if (pi->rows > pj->rows)
	{
		t = *pi;
		*pi = *pj;
		*pj = t;


		pt = pi->next;
		pi->next = pj->next;
		pj->next = pt;
	}
	return pi;
}

//���к���  ��������ַ�������
//void showtopic(char *topic)
//{
//	char space[] = "                                       ";
//	printf("%s %s\n", space + (strlen(topic) / 2), topic);
//}



//����һ�����
void *create(int rows, int cols)
{
	point *p;
	p = (point *)malloc(sizeof(point));
	p->rows = rows;
	p->cols = cols;
	p->next = NULL;
	return p;
}


//��ӡ����
void showonepoint(point head)
{
	printf("(%d %d)", head.rows, head.cols);
}


//��ӡ��������
void showpoints(point head)
{
	point *p;
	printf("\n��ǰ�������£�\n");
	for (p = head.next; p; p = p->next)
	{
		showonepoint(*p);
	}
}


//������������
void  initpointlink(point *head)
{
	int rows;
	int cols;
	point *p;
	if (head->next == NULL)
	{
		showtopic("������¼�����꣨���루0 0������¼�룩");
		scanf("%d %d", &rows, &cols);
		fflush(stdin);
		while (rows&&cols)
		{
			p = create(rows, cols);
			head->next = p;
			head = p;
			showtopic("������¼�����꣨���루0 0������¼�룩");
			scanf("%d %d", &rows, &cols);
			fflush(stdin);
		}
	}
	else
		showtopic("\n�㲻�ܶ����������ʼ��\n");
}

//��������
void destroy(point *head)
{
	point *p;
	while (head->next)
	{
		p = head->next;
		head->next = p->next;
		free(p);
	}
}


//����һ������
point *found(point head, int oldrows, int oldcols)
{
	point *p;
	point *q = NULL;
	for (p = head.next; p && ((p->cols != oldcols) || (p->rows != oldrows)); p = p->next)
		q = p;
	return q;


	//��ָ����Ϊ��һ��������ֵΪNULL  
	//��ָ����Ϊ���˵�һ������������ڵ㣬��ú����ķ���ֵΪĿ���ǰһ��ָ���㡣  
	//��ָ���㲻���ڣ���ú����ķ���ֵָ��ĩ�ڵ㡣  
}


//����һ������
void insert(point *head)   //���������ڵ㷽ʽ  
{
	int newrows;
	int newcols;
	int oldrows;
	int oldcols;
	point *p;
	point *pro;
	showtopic("\n��ǰ�������£���ο�����λ�ý��������\n");
	showpoints(*head);
	showtopic("\n�����뽫Ҫ�����λ��\n");
	scanf("%d %d", &oldrows, &oldcols);
	fflush(stdin);
	pro = found(*head, oldrows, oldcols);
	showtopic("\n��������Ҫ���������\n");
	scanf("%d %d", &newrows, &newcols);
	fflush(stdin);
	p = create(newrows, newcols);
	if (NULL == pro)
	{
		pro = head;
	}
	p->next = pro->next;
	pro->next = p;
}

//ɾ��һ������
void *delete(point *head)
{
	int rows;
	int cols;
	point *p;
	point *pro;
	showpoints(*head);
	showtopic("\n������������Ҫɾ���ڵ��rows��colsֵ\n");
	scanf("%d %d", &rows, &cols);
	fflush(stdin);
	pro = found(*head, rows, cols);
	if (pro && pro->next == NULL)        //���ֱ��ʽ�����ǰ��Ϊ�ٺ���ֱ�Ӷ����ÿ��ˣ�������������������������λ�ã�������һ��bug��  
	{
		showtopic("\nҪɾ���Ľڵ㲻����(�����������....)\n");
		getch();
	}
	else
	{
		if (NULL == pro)
		{
			pro = head;
		}
		p = pro->next;
		pro->next = p->next;
		free(p);
		showpoints(*head);
		showtopic("\nɾ���ɹ���������(�����������)");
		getch();
	}
	return pro;
}





int main()
{
	int rows;
	int cols;
	int input = 0;
	point pointlinkhead1 = { 0, 0, NULL };
	do
	{
		system("cls");
		printf("\n\n\n");
		showtopic("*****��ĳ�˵��������ϵͳ*****\n\n\n");
		showtopic("��ʼ��������->  1\n");
		showtopic("��������->  2\n");
		showtopic("��������->  3\n");
		showtopic("ɾ������->  4\n");
		showtopic("��������->  5\n");
		showtopic("�������->  6\n");
		showtopic("�˳���ϵͳ-> 0\n");
		scanf("%d", &input);
		fflush(stdin);
		switch (input)
		{
		case 1:
			system("cls");
			showtopic("\n������¼����Ч����\n\n\n");
			initpointlink(&pointlinkhead1);
			showpoints(pointlinkhead1);
			showtopic("\n¼��ɹ�(�������������\n");
			getch();
			break;
		case 2:
			system("cls");
			showtopic("\n����������ҵ�����\n");
			scanf("%d %d", &rows, &cols);
			printf("%s\n", *found(pointlinkhead1, rows, cols));  //��ʱ�޷�ʹ��  
			break;
		case 3:
			system("cls");
			insert(&pointlinkhead1);
			showpoints(pointlinkhead1);
			showtopic("\n����ɹ���������(�����������)");
			getch();
			break;
		case 4:
			system("cls");
			delete(&pointlinkhead1);
			break;
		case 5:
			system("cls");
			ordering(pointlinkhead1);
			showtopic("\n����ɹ�������������(�����������....��\n");
			showpoints(pointlinkhead1);
			getch();
			break;
		case 6:
			system("cls");
			destroy(&pointlinkhead1);
			showtopic("\n������ݳɹ���������(�����������)");
			getch();
			break;
		default:
			showtopic("������������������\n");
			break;




		}
	} while (input);
	return 0;
}
