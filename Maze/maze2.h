#include<iostream>
#include<Windows.h>
#include<assert.h>
#include<stack>
using namespace std;


struct pos
{
	pos(const int& x, const int& y)
		:row(y)
		,col(x)
	{}

	int  col;
	int  row;
};

template<size_t x,size_t y>
class maze
{
public:

	maze(const int array[x][y])
	{
		assert(array != NULL);

		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				arr[i][j] = array[i][j];
			}
		}
		
	}

	maze(const maze& T)
	{
		assert(T.arr != NULL);

		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				arr[i][j] = T.arr[i][j];
			}
		}
	}

	void getmazepath(const pos& enter)
	{
		p.push(enter);
		pos cur = enter;
		pos next = cur;
		arr[cur.row][cur.col] = 2;

		if (cur.row == x - 1 || cur.col == y - 1)
		{
			arr[cur.row][cur.col] = 3;
			cout << "找到一个出口->" << "[" << cur.row << "," << cur.col << "]" << endl;

			if (q.empty() || q.size() > p.size())
			{
				q = p;
			}
		}

		//上
		next.row -= 1;
		if (CheckAccess(next))
		{
			getmazepath(next);
		}

		//右
		next = cur;
		next.col += 1;
		if (CheckAccess(next))
		{
			getmazepath(next);
		}

		//下
		next = cur;
		next.row += 1;
		if (CheckAccess(next))
		{
			getmazepath(next);
		}

		//左
		next = cur;
		next.col -= 1;
		if (CheckAccess(next))
		{
			getmazepath(next);
		}

		p.pop();
	}

	void printmixpath(const pos& enter)
	{
		getmazepath(enter);

		if (!q.empty())
		{
			cout << "出口" << " ";
			while (!q.empty())
			{
				pos cur = q.top();
				cout << "<-" << "[" << cur.row << "," << cur.col << "]" << " ";
				q.pop();
			}
			cout << " <-起点" << endl;

		}
	}


	void print()
	{
		assert(arr != NULL);

		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				cout << arr[i][j]<<"  ";
			}
			cout << endl;
		}
	}

	
	bool CheckAccess(pos next)
	{
		if ((next.row >= 0 && next.row < x) &&
			(next.col >= 0 && next.col < y) &&
			arr[next.row][next.col] == 0)
		{
			return true;
		}
		return false;
	}


private:
	int arr[x][y];
	stack<pos> p;    //容器
	stack<pos> q; 	//最终容器

};
void Test()
{
	int arr[10][10] =
	{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 0, 1, 1, 1, 1, 0, 1,
		1, 1, 1, 0, 1, 1, 1, 1, 0, 1,
		1, 1, 1, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 0, 1, 1, 1, 1
	};

	/*int arr[10][10] =
	{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 1, 1, 0, 1, 0, 1, 1, 0, 1,
		1, 1, 1, 0, 1, 0, 1, 1, 0, 1,
		1, 1, 1, 0, 1, 0, 1, 1, 1, 1,
		1, 1, 1, 0, 1, 0, 1, 1, 1, 1,
		1, 1, 1, 0, 1, 0, 1, 1, 1, 1,
		1, 1, 1, 0, 1, 0, 1, 1, 1, 1,
		1, 1, 1, 0, 1, 0, 1, 1, 1, 1
	};*/

	maze<10, 10> arr1(arr);

	arr1.printmixpath(pos(0,2));
	arr1.print();
	system("pause");
}