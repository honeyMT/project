#include<iostream>
#include<Windows.h>
#include<assert.h>
#include<list>
#include<stdlib.h>

using namespace std;

struct snakeNode
{
	int _x;
	int _y;

	snakeNode(int x, int y)
		:_x(x)
		, _y(y)
	{}

	bool operator ==(snakeNode T)
	{
		return (_x == T._x && _y == T._y);
	}

	bool operator !=(snakeNode T)
	{
		return (_x != T._x || _y != T._y);
	}
};

template<size_t M, size_t N>
class Retrosnake
{
public:
	typedef snakeNode Node;

	void makeMap()
	{
		memset(_arr, ' ',sizeof(1 * M * N));

		for (int i = 0; i < M; i++)
		{
			_arr[i][0] = '*';
		}
		for (int i = 0; i < M; i++)
		{
			_arr[i][N - 1] = '*';
		}
		for (int i = 0; i < N; i++)
		{
			_arr[0][i] = '*';
		}
		for (int i = 0; i < N; i++)
		{
			_arr[M - 1][i] = '*';
		}
	}


	void game()
	{
		makeMap();
		Node next(1, 3);
		Node Eat = makeEat();
		while (1)
		{
			char Move;
			cin >> Move;		
			system("cls");
			if (Move == 'w')
			{
				next._x--;
				if (CheckgameOK(next) == false)
				{
					cout << "game is over" << endl;
					return;
				}
				if (CheckMoveOK(next) == true)
				{
					snakeMove(next,Eat);
				}
			}
			else if (Move == 'a')
			{
				next._y--;
				if (CheckgameOK(next) == false)
				{
					cout << "game is over" << endl;
					return;
				}
				if (CheckMoveOK(next) == true)
				{
					snakeMove(next, Eat);
				}
			}
			else if (Move == 'd')
			{
				next._y++;
				if (CheckgameOK(next) == false)
				{
					cout << "game is over" << endl;
					return;
				}
				if (CheckMoveOK(next) == true)
				{
					snakeMove(next, Eat);
				}
			}


			else if (Move == 's')
			{
				next._x++;
				if (CheckgameOK(next) == false)
				{
					cout << "game is over" << endl;
					return;
				}
				if (CheckMoveOK(next) == true)
				{
					snakeMove(next, Eat);
				}
			}
			else
			{
				fflush(stdin);
			}
		}
	}

protected:
	void print()
	{
		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				cout << _arr[i][j];
			}
			cout << endl;
		}
	}

	void snackSet(Node next,Node& eat)
	{

		if (_snake.size() == 0)
		{
			_snake.push_front(Node(1, 1));
			_snake.push_front(Node(1, 2));
			_snake.push_front(Node(1, 3));
			_snake.push_front(Node(1, 4));
		}
		_snake.push_front(next);

		if (next != eat)
		{
			_arr[_snake.back()._x][_snake.back()._y] = ' ';
			_snake.pop_back();
		}	
		else
		{
			eat = makeEat();
		}

		list<Node>::iterator t1 = _snake.begin();

		while (t1 != _snake.end())
		{
			_arr[t1->_x][t1->_y] = '#';
			++t1;
		}
	}

	void snakeMove(Node next,Node& Eat)
	{
		snackSet(next,Eat);
		print();
	}

	Node makeEat()
	{
		while (1)
		{
			int rows = rand() % M;
			int cols = rand() % N;
			if (_arr[rows][cols] != '*' && _arr[rows][cols] != '#')
			{
				_arr[rows][cols] = '@';
				return Node(rows, cols);
			}
		}
	}

	bool CheckgameOK(Node next)
	{
		if (_arr[next._x][next._y] != '*')
		{
			return true;
		}
		return false;
	}

	bool CheckMoveOK(Node next)
	{
		if (_arr[next._x][next._y] != '#')
		{
			return true;
		}
		return false;
	}

private:
	char _arr[M][N];
	list<Node> _snake;

};

void Test()
{
	Retrosnake<10,20> T;
	T.game();
	system("pause");
}
