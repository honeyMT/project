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


//排序
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

//居中函数  将输入的字符串居中
//void showtopic(char *topic)
//{
//	char space[] = "                                       ";
//	printf("%s %s\n", space + (strlen(topic) / 2), topic);
//}



//创建一个结点
void *create(int rows, int cols)
{
	point *p;
	p = (point *)malloc(sizeof(point));
	p->rows = rows;
	p->cols = cols;
	p->next = NULL;
	return p;
}


//打印坐标
void showonepoint(point head)
{
	printf("(%d %d)", head.rows, head.cols);
}


//打印所有坐标
void showpoints(point head)
{
	point *p;
	printf("\n当前坐标如下：\n");
	for (p = head.next; p; p = p->next)
	{
		showonepoint(*p);
	}
}


//创建链接链表
void  initpointlink(point *head)
{
	int rows;
	int cols;
	point *p;
	if (head->next == NULL)
	{
		showtopic("请输入录入坐标（输入（0 0）结束录入）");
		scanf("%d %d", &rows, &cols);
		fflush(stdin);
		while (rows&&cols)
		{
			p = create(rows, cols);
			head->next = p;
			head = p;
			showtopic("请输入录入坐标（输入（0 0）结束录入）");
			scanf("%d %d", &rows, &cols);
			fflush(stdin);
		}
	}
	else
		showtopic("\n你不能对已有链表初始化\n");
}

//销毁链表
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


//查找一个坐标
point *found(point head, int oldrows, int oldcols)
{
	point *p;
	point *q = NULL;
	for (p = head.next; p && ((p->cols != oldcols) || (p->rows != oldrows)); p = p->next)
		q = p;
	return q;


	//若指定点为第一个，返回值为NULL  
	//若指定点为除了第一个以外的其他节点，则该函数的返回值为目标的前一个指定点。  
	//若指定点不存在，则该函数的返回值指向末节点。  
}


//插入一个坐标
void insert(point *head)   //采用左插入节点方式  
{
	int newrows;
	int newcols;
	int oldrows;
	int oldcols;
	point *p;
	point *pro;
	showtopic("\n当前坐标如下，请参考插入位置进行左插入\n");
	showpoints(*head);
	showtopic("\n请输入将要插入的位置\n");
	scanf("%d %d", &oldrows, &oldcols);
	fflush(stdin);
	pro = found(*head, oldrows, oldcols);
	showtopic("\n请输入你要插入的数据\n");
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

//删除一个坐标
void *delete(point *head)
{
	int rows;
	int cols;
	point *p;
	point *pro;
	showpoints(*head);
	showtopic("\n请输入你所需要删除节点的rows和cols值\n");
	scanf("%d %d", &rows, &cols);
	fflush(stdin);
	pro = found(*head, rows, cols);
	if (pro && pro->next == NULL)        //这种表达式，如果前面为假后面直接都不用看了，如果你调换了他两个的条件的位置，会引发一个bug。  
	{
		showtopic("\n要删除的节点不存在(按任意键继续....)\n");
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
		showtopic("\n删除成功！！！！(按任意键继续)");
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
		showtopic("*****张某人的坐标管理系统*****\n\n\n");
		showtopic("开始输入坐标->  1\n");
		showtopic("查找坐标->  2\n");
		showtopic("插入坐标->  3\n");
		showtopic("删除坐标->  4\n");
		showtopic("排序坐标->  5\n");
		showtopic("清除数据->  6\n");
		showtopic("退出该系统-> 0\n");
		scanf("%d", &input);
		fflush(stdin);
		switch (input)
		{
		case 1:
			system("cls");
			showtopic("\n请依次录入有效坐标\n\n\n");
			initpointlink(&pointlinkhead1);
			showpoints(pointlinkhead1);
			showtopic("\n录入成功(按任意键继续）\n");
			getch();
			break;
		case 2:
			system("cls");
			showtopic("\n请输入你查找的坐标\n");
			scanf("%d %d", &rows, &cols);
			printf("%s\n", *found(pointlinkhead1, rows, cols));  //暂时无法使用  
			break;
		case 3:
			system("cls");
			insert(&pointlinkhead1);
			showpoints(pointlinkhead1);
			showtopic("\n插入成功！！！！(按任意键继续)");
			getch();
			break;
		case 4:
			system("cls");
			delete(&pointlinkhead1);
			break;
		case 5:
			system("cls");
			ordering(pointlinkhead1);
			showtopic("\n排序成功！！！！！！(按任意键继续....）\n");
			showpoints(pointlinkhead1);
			getch();
			break;
		case 6:
			system("cls");
			destroy(&pointlinkhead1);
			showtopic("\n清除数据成功！！！！(按任意键继续)");
			getch();
			break;
		default:
			showtopic("输入有误请重新输入\n");
			break;




		}
	} while (input);
	return 0;
}
