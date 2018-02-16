#include<iostream>
#include<Windows.h>
#include<assert.h>
#include<stack>
using namespace std;


struct pos
{
	pos(const int& x, const int& y)
	:row(x)
	,col(y)
	{}

	int row;
	int col;
};

template<size_t M,size_t N>
class maze
{
public:
	maze(const int array[M][N])
	{
		assert(array != NULL);

		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				arr[i][j] = array[i][j];
			}
		}
	}

	maze(const maze& T)
	{
		assert(T.arr != NULL);

		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				arr[i][j] = T.arr;
			}
		}
	}

	void printmixpath()
	{
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
	void getmixpath(pos enter,int count = 0)
	{
		pos cur = enter;
		pos next = cur;
		p.push(cur);

		if (cur.row == M - 1 || cur.col == N - 1)
		{
			cout << "找到一个出口为" << "[" << cur.row << "," << cur.col << "]" << endl;;
			arr[cur.row][cur.col] = count;
			if (q.empty() || q.size() > p.size())
			{
				q = p;
			}
		}

		arr[cur.row][cur.col] = ++count;

		

		//向上试探
		next.row -= 1;
		if (CheckAccess(cur, next))
		{
			getmixpath(next, arr[cur.row][cur.col]);
		}

		//向右试探
		next = cur;
		next.col += 1;
		if (CheckAccess(cur, next))
		{
			getmixpath(next, arr[cur.row][cur.col]);
		}

		//向下试探
		next = cur;
		next.row += 1;
		if (CheckAccess(cur, next))
		{
			getmixpath(next, arr[cur.row][cur.col]);
		}

		//向左试探
		next = cur;
		next.col -= 1;
		if (CheckAccess(cur, next))
		{
			getmixpath(next, arr[cur.row][cur.col]);
		}

		p.pop();
		return;
	}

	bool CheckAccess(pos cur, pos next)
	{
		if ((next.row >= 0 && next.row < M)
			&& (next.col >= 0 && next.col < N)
			&& arr[next.row][next.col] == 0 || arr[cur.row][cur.col] < arr[next.row][next.col])
		{
			return true;
		}
		return false;
	}

	void print()
	{
		assert(arr != NULL);
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				cout << arr[i][j] << "  ";
			}
			cout << endl;
		}
	}
private:
	int arr[M][N];
	stack<pos> p;   //比较容器
	stack<pos> q;  //最终保存容器
};


void Test()
{
	int arr[10][10] =
	{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
		1, 1, 1, 0, 1, 0, 1, 1, 1, 1,
		1, 1, 1, 0, 1, 0, 1, 1, 1, 1,
		1, 1, 1, 0, 1, 0, 1, 1, 1, 1,
		1, 1, 1, 0, 1, 0, 1, 1, 1, 1,
		1, 1, 1, 0, 1, 0, 1, 1, 1, 1,
		1, 1, 1, 0, 1, 0, 1, 1, 1, 1,
		1, 1, 1, 0, 1, 0, 1, 1, 1, 1
	};

	maze<10, 10> arr1(arr);
	arr1.getmixpath(pos(2,0));
	arr1.printmixpath();
	arr1.print();
	system("pause");
}

