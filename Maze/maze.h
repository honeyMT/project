#include<iostream>
#include<Windows.h>
#include<assert.h>
#include<stack>
using namespace std;


struct pos
{
	pos(int i, int j)
	:row(i)
	, col(j)
	{}

	int col;
	int row;
};

template<int M, int  N>
class maze
{
public:
	maze(int array[M][N])
	{
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
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				arr[i][j] = T.arr[i][j];
			}
		}
	}
	void printminmazepath()
	{
		cout << "出口";
		while (!q.empty())
		{
			pos cur = q.top();
			cout << " < - " << "[" << cur.row << "," << cur.col << "]";
			q.pop();
		}
		cout << ":为径路短最" << endl;

	}

	void getmazepath(pos enter, int count)
	{
		pos cur = enter;
		pos next = cur;
		arr[enter.row][enter.col] = count + 1;
		p.push(cur);

		if (cur.row == N - 1)
		{
			if (q.empty() || q.size() > p.size())
			{
				cout << "找到一个出口为" << "[" << cur.row << "," << cur.col << "]" << endl;;
				q = p;
			}
		}
		//向上试探
		next.row -= 1;
		if (CheckAccess(cur, next))
		{
			getmazepath(next, arr[cur.row][cur.col]);
		}

		//向右试探
		next = cur;
		next.col += 1;
		if (CheckAccess(cur, next))
		{
			getmazepath(next, arr[cur.row][cur.col]);
		}

		//向下试探
		next = cur;
		next.row += 1;
		if (CheckAccess(cur, next))
		{
			getmazepath(next, arr[cur.row][cur.col]);
		}

		//向左试探
		next = cur;
		next.col -= 1;
		if (CheckAccess(cur, next))
		{
			getmazepath(next, arr[cur.row][cur.col]);
		}
		p.pop();
		return;
	}

	bool CheckAccess(pos cur, pos next)
	{
		if ((next.row >= 0 && next.row < M)
			&& (next.col >= 0 && next.col <= N)
			&& (arr[next.row][next.col] == 0 || arr[cur.row][cur.col] < arr[next.row][next.col]))
		{
			return true;
		}
		return false;
	}
	void print()
	{
		cout << "该迷宫的俯视图如下->" << endl;
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				cout << arr[i][j] << "  ";
			}
			cout << endl;
			cout << endl;
		}
	}


private:
	int arr[M][N];
	stack<pos> p;
	stack<pos> q;
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
	arr1.getmazepath(pos(2, 0), 1);
	arr1.print();
	arr1.printminmazepath();
	system("pause");
}



