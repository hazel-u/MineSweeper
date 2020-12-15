#include<SFML/Graphics.hpp>
#include<Windows.h>
#include<time.h>

#define LENGTH 15
#define WIDTH 15
#define BOMBS 1

using namespace std;
using namespace sf;

struct poz
{
	int row;
	int column;
};

int w = 32;
int user_board[LENGTH + 1][WIDTH + 1];
int board[LENGTH + 1][WIDTH + 1];
int firstMove = 0;
int BombsCnt = 0;
poz tail[10000];
int ltail, excttail = 0;
int bombsFound = 0;

void initialisationUserBoard()
{
	for (int i = 0; i < LENGTH; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			user_board[i][j] = 10;
		}
	}
}

void bombInitialisation(int a, int b)
{
	while (BombsCnt != BOMBS)
	{
		int x = rand() % LENGTH;
		int y = rand() % WIDTH;
		if (x != a && y != b)
		{
			if (board[x][y] != 9)
			{
				board[x][y] = 9;
				BombsCnt++;
			}
		}
	}
}

void numberGenerator()
{
	for (int i = 0; i < LENGTH; i++)
	{
		for (int j = 0; j < WIDTH; j++)
		{
			if (board[i][j] != 9)
			{
				int numBombs = 0;
				if (board[i - 1][j - 1] == 9)
				{
					numBombs++;
				}
				if (board[i - 1][j] == 9)
				{
					numBombs++;
				}
				if (board[i - 1][j + 1] == 9)
				{
					numBombs++;
				}
				if (board[i][j - 1] == 9)
				{
					numBombs++;
				}
				if (board[i][j + 1] == 9)
				{
					numBombs++;
				}
				if (board[i + 1][j - 1] == 9)
				{
					numBombs++;
				}
				if (board[i + 1][j] == 9)
				{
					numBombs++;
				}
				if (board[i + 1][j + 1] == 9)
				{
					numBombs++;
				}
				board[i][j] = numBombs;
			}
		}
	}
}

void verification(int i, int j)
{
	user_board[i][j] = 0;

	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			if (x == 0 && y == 0)
			{
				continue;
			}
			else if (board[i + x][j + y] == 0 && i + x > 0 && i + x <= LENGTH && j + y > 0 && j + y <= WIDTH)
			{
				tail[ltail].column = i + x;
				tail[ltail].row = j + y;
				ltail++;
			}
		}
	}
}

void firstMoove(int i, int j)
{
	for (int x = -1; x <= 1; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			if (board[i + x][j + y] != 9 && i + x > 0 && i + x <= LENGTH && j + y > 0 && j + y <= WIDTH)
			{
				if (board[i + x][j + y] == 0)
				{
					user_board[i + x][j + y] = 0;
					verification(i + x, j + y);
					while (excttail < ltail)
					{
						user_board[tail[excttail].column][tail[excttail].row] = 0;
						excttail++;
					}
				}
				else
				{
					user_board[i + x][j + y] = board[i + x][j + y];
				}
			}
		}
	}
}

int main()
{
	srand(time(0));
	RenderWindow app(VideoMode(480, 480), "MineSweeper!");

	Texture t;
	t.loadFromFile("images/tiles.jpg");
	Texture winSource;
	winSource.loadFromFile("images/win.jpg");
	Sprite s(t);

	initialisationUserBoard();

	while (app.isOpen())
	{
		Vector2i pos = Mouse::getPosition(app);

		int x = pos.x / w;
		int y = pos.y / w;

		Event e;

		while (app.pollEvent(e))
		{
			if (e.type == Event::Closed)
			{
				app.close();
			}
		}

		if (e.type == Event::MouseButtonPressed)
		{
			if (e.key.code == Mouse::Left)
			{
				if (firstMove == 0)
				{
					bombInitialisation(x, y);
					numberGenerator();
					firstMoove(x, y);
					firstMove = 1;
				}
				else if (board[x][y] == 0)
				{
					verification(x, y);
					while (excttail < ltail)
					{
						user_board[tail[excttail].column][tail[excttail].row] = 0;
						excttail++;
					}
				}
				else
				{
					user_board[x][y] = board[x][y];
				}
			}
			else if (e.key.code == Mouse::Right)
			{
				if (user_board[x][y] == 11)
				{
					user_board[x][y] = 10;
					if (board[x][y] == 9)
					{
						bombsFound--;
					}
				}
				else if (user_board[x][y] == 10)
				{
					user_board[x][y] = 11;
					if (board[x][y] == 9)
					{
						bombsFound++;
					}
				}
			}
		}
		app.clear(Color::White);
		if (bombsFound == BOMBS)
		{
			Sprite win(winSource);
			win.setTexture(winSource);
			app.draw(win);
			app.display();
			Sleep(3000);
			break;
		}
		else
		{
			for (int i = 0; i < LENGTH; i++)
			{
				for (int j = 0; j < WIDTH; j++)
				{
					if (user_board[x][y] == 9)
					{
						user_board[i][j] = board[i][j];
					}

					s.setTextureRect(IntRect(user_board[i][j] * w, 0, w, w));
					s.setPosition(i * w, j * w);
					app.draw(s);
				}
			}
			app.display();
		}
	}
	return 0;
}