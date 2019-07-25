/*2. Написать «Морской бой» для игры человека против компьютера. Предусмотреть за человека возможность автоматической (расстановку осуществляет кораблей компьютер случайным образом) и ручной расстановки своих
кораблей. Стоимость задания существенно повышается, если компьютер при стрельбе будет обладать логикой (т.е. не производить выстрелы «рандомайзом»).*/

#include <iostream>
#include <stdlib.h>
#include<Windows.h>
#include <time.h>
using namespace std;

int **Create(int, int); //создание динамического массива размером "n" на "m"

int Test_up(int **, int, int, int, int, int); //проверяет, можно ли расставлять корабли вверх от случайно определённой базовой координаты
int Test_down(int **, int, int, int, int, int); //проверяет, можно ли расставлять корабли вниз от случайно определённой базовой координаты
int Test_left(int **, int, int, int, int, int); //проверяет, можно ли расставлять корабли влево от случайно определённой базовой координаты
int Test_right(int **, int, int, int, int, int); //проверяет, можно ли расставлять корабли вправо от случайно определённой базовой координаты

void Rand(int **, int, int); //расстановка кораблей случайным образом

void Shuting_comp(int **, int, int, int &, int &, bool &, int &); //стрельба по кораблям (ходит компьютер)
void Shuting_player(int **, int, int, int &); //стрельба по кораблям (ходит игрок)

void Bufer_deck_hor(int **, int, int, int, int, int); //функция для расстановки выстрелов вокруг уничтоженного корабля (по горизонтали)
void Bufer_deck_ver(int **, int, int, int, int, int); //функция для расстановки выстрелов вокруг уничтоженного корабля (по вертикали)

int Test_killed_hor(int **, int, int, int, int, int); //функция для проверки по горизонтали, все ли палубы одного корабля уничтожены
int Test_killed_ver(int **, int, int, int, int, int); //функция для проверки по вертикали, все ли палубы одного корабля уничтожены

void Print_player(int **, int, int);
void Print_comp(int **, int, int);

int Test_win(int **, int, int);

void DEL(int **mas, int n) //удаление массива из памяти после окончания игры
{
	for (int i = 0; i<n; i++)
	{
		free(mas[i]);
	}
	free(mas);
}

int main()
{
	int n = 10;
	int m = 10;
	int win = 0; //переменная для отслеживания окончания игры (0 - игра продолжается, 1 - игра завершена (победил компьютер), 2 - игра завершена (победил игрок))
	bool repeat = 0; //переменная для отслеживания необходимости добить подбитый корабль (0 - добивать корабль не нужно, 1 - необходимо добить корабль)
	int shot_n_save, shot_m_save;
	int **Player = Create(n, m);
	Rand(Player, n, m);
	int **Comp = Create(n, m);
	Rand(Comp, n, m);
	srand(time(0));
	int select = rand() % 2;
	do
	{
		if (select == 0)
		{
			if (win == 0)
			{
				Shuting_comp(Player, n, m, shot_n_save, shot_m_save, repeat, win);
			}
			if (win == 0)
			{
				Shuting_player(Comp, n, m, win);
			}
		}
		else
		{
			if (win == 0)
			{
				Shuting_player(Comp, n, m, win);
			}
			if (win == 0)
			{
				Shuting_comp(Player, n, m, shot_n_save, shot_m_save, repeat, win);
			}
		}
		if (win==1)
		{
			cout << "Computer is win!\n";
		}
		if (win == 2)
		{
			cout << "Player is win!\n";
		}
	} 
	while (win == 0);
	DEL(Player, n);
	DEL(Comp, n);
	system("pause");
}

int **Create(int n, int m)
{
	int **mas = (int**)calloc(n, sizeof(int*));
	for (int i = 0; i<m; i++)
	{
		mas[i] = (int*)calloc(m, sizeof(int));
	}
	return mas;
}


void Rand(int **mas, int n, int m)
{
	srand(time(NULL));
	int number_of_ships = 1; //переменная для определения нужного количества кораблей
	int number_of_decks = 4; //переменная для определения нужного количества палуб
	while (number_of_decks != 0)
	{
		for (int ships = 0; ships < number_of_ships;)
		{
			int ii = rand() % 10;
			int jj = rand() % 10;
			int down = Test_down(mas, n, m, ii, jj, number_of_decks);
			int up = Test_up(mas, n, m, ii, jj, number_of_decks);
			int left = Test_left(mas, n, m, ii, jj, number_of_decks);
			int right = Test_right(mas, n, m, ii, jj, number_of_decks);
			if (up == 1 && down == 1 || left == 1 && right == 1)
			{
				bool f = 0;
				int select;
				while (f != 1)
				{
					select = rand() % 2;
					if (select == 0 && right == 1 && left == 1)
					{
						f = 1;
					}
					if (select == 1 && up == 1 && down == 1)
					{
						f = 1;
					}
				}
				if (select == 0)
				{
					int k = 0; //переменная для хранения количества кораблей
					for (int j = jj; j < (jj + number_of_decks) && j < m; j++)  //цикл для построения палуб
					{
						mas[ii][j] = number_of_decks;
						k++;
					}
					if (k != number_of_decks)  //если построены не все необходимые палубы - начинаем строить в обратном направлении до тех пор...
					{
						for (int j = jj; k != number_of_decks;)
						{
							j--;
							mas[ii][j] = number_of_decks;
							k++;
						}
					}
				}
				else
				{
					int k = 0;
					for (int i = ii; i < (ii + number_of_decks) && i < n; i++)
					{
						mas[i][jj] = number_of_decks;
						k++;
					}
					if (k != number_of_decks)
					{
						for (int i = ii; k != number_of_decks;)
						{
							i--;
							mas[i][jj] = number_of_decks;
							k++;
						}
					}
				}
				ships++;
			}
		}
		number_of_ships++;
		number_of_decks--;
	}
}


int Test_up(int **mas, int n, int m, int ii, int jj, int number_of_decks)
{
	int f = 1;
	int i, j;
	for (ii == (n - 1) ? i = ii : i = (ii + 1); i >= (ii - number_of_decks) && i >= 0; i--)
	{
		for (jj == 0 ? j = jj : j = (jj - 1); j <= (jj + 1) && j >= 0 && j < m; j++)
		{
			if (mas[i][j] == 1 || mas[i][j] == 2 || mas[i][j] == 3 || mas[i][j] == 4)
			{
				f = 0;
				break;
			}
		}
	}
	return f;
}

int Test_down(int **mas, int n, int m, int ii, int jj, int number_of_decks)
{
	int f = 1;
	int i, j;
	for (ii == 0 ? i = ii : i = (ii - 1); i <= (ii + number_of_decks) && i<n; i++)
	{
		for (jj == 0 ? j = jj : j = (jj - 1); j <= (jj + 1) && j >= 0 && j < m; j++)
		{
			if (mas[i][j] == 1 || mas[i][j] == 2 || mas[i][j] == 3 || mas[i][j] == 4)
			{
				f = 0;
				break;
			}
		}
	}
	return f;
}

int Test_left(int **mas, int n, int m, int ii, int jj, int number_of_decks)
{
	int f = 1;
	int i, j;
	for (ii == 0 ? i = ii : i = (ii - 1); i <= (ii + 1) && i >= 0 && i < n; i++)
	{
		for (jj == (m - 1) ? j = jj : j = (jj + 1); j >= (jj - number_of_decks) && j >= 0; j--)
		{
			if (mas[i][j] == 1 || mas[i][j] == 2 || mas[i][j] == 3 || mas[i][j] == 4)
			{
				f = 0;
				break;
			}
		}
	}
	return f;
}

int Test_right(int **mas, int n, int m, int ii, int jj, int number_of_decks)
{
	int f = 1;
	int i, j;
	for (ii == 0 ? i = ii : i = (ii - 1); i <= (ii + 1) && i >= 0 && i < n; i++)
	{
		for (jj == 0 ? j = jj : j = (jj - 1); j <= (jj + number_of_decks) && j<m; j++)
		{
			if (mas[i][j] == 1 || mas[i][j] == 2 || mas[i][j] == 3 || mas[i][j] == 4)
			{
				f = 0;
				break;
			}
		}
	}
	return f;
}

void Shuting_comp(int **mas, int n, int m, int &shot_n_save, int &shot_m_save, bool &repeat, int &win)  //shot_n_save и int shot_m_save - переменные для хранения последнего удачного выстрела 
//(при условии, что корабль нужно добить); win - переменная для отслеживания окончания игры (0 - игра продолжается, 1 - игра завершена (закончились корабли))
{
	
	srand(time(NULL));
	int shot_n, shot_m;
	bool f=1; //переменная для отслеживания попадания (0 - промахнулись, 1 - попали и стреляем повторно)
	do
	{
		system("cls");
		cout << "   Computer motion\n\n";
		Print_player(mas, n, m);
		cout << "\n";
		bool control=0; //переменная для хранения нашего хода (0 - туда уже ранее стреляли, 1 - стреляем туда впервые)
		do
		{
			if (repeat == 1)
			{
				if (shot_m_save>0 && shot_m_save<(m - 1) && mas[shot_n_save][shot_m_save + 1] >= 0 && mas[shot_n_save][shot_m_save + 1]<5 && mas[shot_n_save][shot_m_save - 1]<0)
				{
					shot_n = shot_n_save;
					shot_m = shot_m_save + 1;
				}
				else if (shot_n_save<(n - 1) && shot_n_save>0 && mas[shot_n_save - 1][shot_m_save] >= 0 && mas[shot_n_save - 1][shot_m_save]<5 && mas[shot_n_save + 1][shot_m_save]<0)
				{
					shot_n = shot_n_save - 1;
					shot_m = shot_m_save;
				}
				else if (shot_m_save>0 && shot_m_save<(m - 1) && mas[shot_n_save][shot_m_save - 1] >= 0 && mas[shot_n_save][shot_m_save - 1]<5 && mas[shot_n_save][shot_m_save + 1]<0)
				{
					shot_n = shot_n_save;
					shot_m = shot_m_save - 1;
				}
				else if (shot_n_save<(n - 1) && shot_n_save>0 && mas[shot_n_save + 1][shot_m_save] >= 0 && mas[shot_n_save + 1][shot_m_save]<5 && mas[shot_n_save - 1][shot_m_save]<0)
				{
					shot_n = shot_n_save + 1;
					shot_m = shot_m_save;
				}
				else if (shot_m_save>1 && shot_m_save<(m - 1) && mas[shot_n_save][shot_m_save + 1] ==5 && mas[shot_n_save][shot_m_save - 1]<0)
				{
					shot_n = shot_n_save;
					shot_m = shot_m_save-2;
				}
				else if (shot_n_save<(n - 2) && shot_n_save>0 && mas[shot_n_save - 1][shot_m_save] == 5 && mas[shot_n_save + 1][shot_m_save]<0)
				{
					shot_n = shot_n_save + 2;
					shot_m = shot_m_save;
				}
				else if (shot_m_save>0 && shot_m_save<(m - 2) && mas[shot_n_save][shot_m_save - 1] == 5 && mas[shot_n_save][shot_m_save + 1]<0)
				{
					shot_n = shot_n_save;
					shot_m = shot_m_save + 2;
				}
				else if (shot_n_save<(n - 1) && shot_n_save>1 && mas[shot_n_save + 1][shot_m_save] == 5 && mas[shot_n_save - 1][shot_m_save]<0)
				{
					shot_n = shot_n_save - 2;
					shot_m = shot_m_save;
				}
				else if (shot_m_save<(m-1) && mas[shot_n_save][shot_m_save + 1]>=0 && mas[shot_n_save][shot_m_save + 1]<5)
				{
					shot_n = shot_n_save;
					shot_m = shot_m_save+1;
				}
				else if (shot_n_save>0 && mas[shot_n_save - 1][shot_m_save] >= 0 && mas[shot_n_save - 1][shot_m_save]<5)
				{
					shot_n = shot_n_save-1;
					shot_m = shot_m_save;
				}
				else if (shot_m_save>0 && mas[shot_n_save][shot_m_save - 1] >= 0 && mas[shot_n_save][shot_m_save - 1]<5)
				{
					shot_n = shot_n_save;
					shot_m = shot_m_save-1;
				}
				else if (shot_n_save<(n - 1) && mas[shot_n_save + 1][shot_m_save] >= 0 && mas[shot_n_save + 1][shot_m_save]<5)
				{
					shot_n = shot_n_save+1;
					shot_m = shot_m_save;
				}
				else
				{
					shot_n = rand() % n;
					shot_m = rand() % m; 
					repeat = 0;
				}
			}
			else
			{
				
				shot_n = rand() % n;
				shot_m = rand() % m;
			}
			if (mas[shot_n][shot_m] >=0 && mas[shot_n][shot_m] <5) //если компьютер туда ещё не стрелял - производим выстрел и...
			{
				control = 1;
			}
		
		} while (control != 1); //цикл для отслеживания хода (чтобы два раза не стрелять в одно и то же место)
		switch (shot_m) //...озвучиваем ход
		{
		case 0:
			cout << "A" << shot_n + 1 << "\n";
			break;
		case 1:
			cout << "B" << shot_n + 1 << "\n";
			break;
		case 2:
			cout << "C" << shot_n + 1 << "\n";
			break;
		case 3:
			cout << "D" << shot_n + 1 << "\n";
			break;
		case 4:
			cout << "E" << shot_n + 1 << "\n";
			break;
		case 5:
			cout << "F" << shot_n + 1 << "\n";
			break;
		case 6:
			cout << "G" << shot_n + 1 << "\n";
			break;
		case 7:
			cout << "H" << shot_n + 1 << "\n";
			break;
		case 8:
			cout << "I" << shot_n + 1 << "\n";
			break;
		case 9:
			cout << "J" << shot_n + 1 << "\n";
			break;
		}
		if (mas[shot_n][shot_m] == 0) //если поле пустое (на нем нет кораблей)-...
		{
			cout << "Slip\n"; //...выводим сообщение "Промах",...
			mas[shot_n][shot_m] = 5; //записываем ход и...
			f = 0; //...пропускаем ход
		}
		if (mas[shot_n][shot_m] > 0 && mas[shot_n][shot_m] <5) //если мы попали в корабль - ...
		{
			int tmp = mas[shot_n][shot_m]; //запоминаем палубность корабля,
			mas[shot_n][shot_m] *= -1; //меняем значение убитого корабля на противоложное и проверяем уничтожен ли он.
			if (Test_killed_hor(mas, n, m, shot_n, shot_m, tmp) == tmp || Test_killed_ver(mas, n, m, shot_n, shot_m, tmp) == tmp) //Если уничтожен,...
			{
				cout << "Killed\n"; //..выводим сообщение убит,...
				if (Test_killed_hor(mas, n, m, shot_n, shot_m, tmp) == tmp)
				{
					Bufer_deck_hor(mas, n, m, shot_n, shot_m, tmp); //заменяем граничащие с убитым кораблём поля на "5", чтобы повторно туда не стрелять,...
					repeat = 0;
				}
				else
				{
					Bufer_deck_ver(mas, n, m, shot_n, shot_m, tmp);
					repeat = 0;
				}
				if (Test_win(mas, n, m) == 0) //проверяем остались ли ещё корабли и...
				{
					f = 0; //...если нет - пропускаем ход 
					win = 1; //...завершаем игру
				}
				else
				{
					f = 1; //...если остались - повторяем ход
				}

			}
			else //Если корабль не уничтожен,...
			{
				shot_n_save = shot_n; //запоминаем текущий ход (удачный выстрел),...
				shot_m_save = shot_m;
				repeat = 1;
				cout << "Hitting\n";//...выводим сообщещие попал и ...
				f = 1; //повторяем ход
				
			}
		}
		//system("pause");
		Sleep(1000);
	} 
	while (f != 0); 
}

void Shuting_player(int **mas, int n, int m, int &win)  //win - переменная для отслеживания окончания игры (0 - игра продолжается, 2 - игра завершена (закончились корабли))
{

	int shot_n, shot_m, shot_n_enter;
	char shot_m_enter;
	bool f=1; //переменная для отслеживания попадания (0 - промахнулись, 1 - попали и стреляем повторно)
	do
	{
		system("cls");
		cout << "   Motion of player\n\n";
		Print_comp(mas, n, m);
		cout << "\n";
		bool control = 1; //переменная для контроля праильности ввода координат
		while (control != 0)
		{
			cout << "Enter the coordinates for the shot:\n";
			cout << "- horizontal - ";
			cin >> shot_m_enter;
			if (shot_m_enter > 64 && shot_m_enter<75 || shot_m_enter>96 && shot_m_enter < 107)
			{
				control = 0;
			}
		}
		control = 1;
		while (control != 0)
		{
			cout << "- vertical - ";
			cin >> shot_n_enter;
			if (shot_n_enter>0 && shot_n_enter < 11)
			{
				control = 0;
			}
		}
		shot_n = shot_n_enter-1;
		switch (shot_m_enter) 
		{
		case 'A':
			shot_m=0;
			break;
		case 'B':
			shot_m = 1;
			break;
		case 'C':
			shot_m=2;
			break;
		case 'D':
			shot_m=3;
			break;
		case 'E':
			shot_m=4;
			break;
		case 'F':
			shot_m=5;
			break;
		case 'G':
			shot_m=6;
			break;
		case 'H':
			shot_m=7;
			break;
			case 'I':
			shot_m=8;
			break;
		case 'J':
			shot_m=9;
			break;
		case 'a':
			shot_m = 0;
			break;
		case 'b':
			shot_m = 1;
			break;
		case 'c':
			shot_m = 2;
			break;
		case 'd':
			shot_m = 3;
			break;
		case 'e':
			shot_m = 4;
			break;
		case 'f':
			shot_m = 5;
			break;
		case 'g':
			shot_m = 6;
			break;
		case 'h':
			shot_m = 7;
			break;
		case 'i':
			shot_m = 8;
			break;
		case 'j':
			shot_m = 9;
			break;
		}
		if (mas[shot_n][shot_m] == 0) //если поле пустое (на нем нет кораблей)-...
		{
			cout << "Slip\n"; //...выводим сообщение "Промах",...
			mas[shot_n][shot_m] = 5; //записываем ход и...
			f = 0; //...пропускаем ход
		}
		if (mas[shot_n][shot_m] > 0 && mas[shot_n][shot_m] <5) //если мы попали в корабль - ...
		{
			int tmp = mas[shot_n][shot_m]; //запоминаем палубность корабля,
			mas[shot_n][shot_m] *= -1; //меняем значение убитого корабля на противоложное и проверяем уничтожен ли он.
			if (Test_killed_hor(mas, n, m, shot_n, shot_m, tmp) == tmp || Test_killed_ver(mas, n, m, shot_n, shot_m, tmp) == tmp) //Если уничтожен,...
			{
				cout << "Killed\n"; //..выводим сообщение убит,...
				if (Test_killed_hor(mas, n, m, shot_n, shot_m, tmp) == tmp)
				{
					Bufer_deck_hor(mas, n, m, shot_n, shot_m, tmp); //заменяем граничащие с убитым кораблём поля на "5", чтобы повторно туда не стрелять,...
				}
				else
				{
					Bufer_deck_ver(mas, n, m, shot_n, shot_m, tmp);
				}
				if (Test_win(mas, n, m) == 0) //проверяем остались ли ещё корабли и...
				{
					f = 0; //...если нет - пропускаем ход 
					win = 2; //...завершаем игру
				}
				else
				{
					f = 1; //...если остались - повторяем ход
				}

			}
			else //Если корабль не уничтожен,...
			{
				cout << "Hitting\n";//...выводим сообщещие попал и ...
				f = 1; //повторяем ход
			}
		}
		system("pause");
	} 
	while (f != 0);
}

int Test_killed_hor(int **mas, int n, int m, int ii, int jj, int number_of_decks) //функция для проверки по горизонтали, все ли палубы одного корабля уничтожены
{
	int f = 0;
	int i = ii;
	int j;
	for ((jj - number_of_decks + 1) <= 0 ? j = 0 : j = (jj - number_of_decks + 1); j < (jj + number_of_decks) && j < m; j++)
	{
		if (mas[i][j] == (-number_of_decks))
		{
			f++;
		}
	}
	return f;
}

int Test_killed_ver(int **mas, int n, int m, int ii, int jj, int number_of_decks) //функция для проверки по вертикали, все ли палубы одного корабля уничтожены
{
	int f = 0;
	int i;
	int j=jj;
	for ((ii - number_of_decks + 1) <= 0 ? i = 0 : i = (ii - number_of_decks + 1); i < (ii + number_of_decks) && i < n; i++)
	{
		if (mas[i][j] == (-number_of_decks))
		{
			f++;
		}
	}
	return f;
}


void Bufer_deck_hor(int **mas, int n, int m, int ii, int jj, int number_of_decks) //функция ограничения ходов по горизонтали в случае, если корабль уничтожен
{
	int i = ii;
	int j;
	for ((jj - number_of_decks + 1) <= 0 ? j = 0 : j = (jj - number_of_decks + 1); j < (jj + number_of_decks) && j < m; j++)
	{
		if (mas[i][j] == -number_of_decks)
		{
			if (j>0 && mas[i][j - 1] != -number_of_decks)
			{
				mas[i][j - 1] = 5;
				if (i > 0)
				{
					mas[i - 1][j - 1] = 5;
				}
				if (i <(n - 1))
				{
					mas[i + 1][j - 1] = 5;
				}
			}
			if (j<(m - 1) && mas[i][j + 1] != -number_of_decks)
			{
				mas[i][j + 1] = 5;
				if (i > 0)
				{
					mas[i - 1][j + 1] = 5;
				}
				if (i <(n - 1))
				{
					mas[i + 1][j + 1] = 5;
				}
			}
			if (i>0)
			{
				mas[i - 1][j] = 5;
			}
			if (i<(n - 1))
			{
				mas[i + 1][j] = 5;
			}
		}
	}
}

void Bufer_deck_ver(int **mas, int n, int m, int ii, int jj, int number_of_decks) //функция ограничения ходов по вертикали в случае, если корабль уничтожен
{
	int i;
	int j=jj;
	for ((ii - number_of_decks + 1) <= 0 ? i = 0 : i = (ii - number_of_decks + 1); i < (ii + number_of_decks) && i < m; i++)
	{
		if (mas[i][j] == -number_of_decks)
		{
			if (i>0 && mas[i-1][j] != -number_of_decks)
			{
				mas[i-1][j] = 5;
				if (j > 0)
				{
					mas[i - 1][j - 1] = 5;
				}
				if (j <(m-1))
				{
					mas[i - 1][j + 1] = 5;
				}
			}
			if (i<(n-1) && mas[i+1][j] != -number_of_decks)
			{
				mas[i+1][j] = 5;
				if (j > 0)
				{
					mas[i + 1][j - 1] = 5;
				}
				if (j <(n - 1))
				{
					mas[i + 1][j + 1] = 5;
				}
			}
			if (j>0)
			{
				mas[i][j-1] = 5;
			}
			if (j<(m-1))
			{
				mas[i][j+1] = 5;
			}
		}
	}
}

int Test_win(int **mas, int n, int m)
{
	int win = 0;
	for (int i = 0; i<n; i++)
	{
		for (int j = 0; j<m; j++)
		{
			if (mas[i][j]>0 && mas[i][j]<5)
			{
				win++;
			}
		}
	}
	return win;
}

void Print_player(int **mas, int n, int m)
{
	char shot = 120;
	char ship = 178;
	char killed_ship = 127;
	cout << "   " << "A " << "B " << "C " << "D " << "E " << "F " << "G " << "H " << "I " << "J " << "\n";
	for (int i = 0; i < n; i++)
	{
		if (i < 9)
		{
			cout << i + 1 << "  ";
		}
		else
		{
			cout << i + 1 << " ";
		}
		for (int j = 0; j < m; j++)
		{
			if (mas[i][j] == 0)
			{
				cout << "- ";
			}
			if (mas[i][j] == 5)
			{
				cout <<shot<< " ";
			}
			if (mas[i][j] >0 && mas[i][j]<5)
			{
				cout << ship<< " ";
			}
			if (mas[i][j]<0)
			{
				cout << killed_ship<<" ";
			}
		}
		cout << "\n";
	}
}

void Print_comp(int **mas, int n, int m)
{
	char shot = 120;
	char killed_ship = 127;
	cout << "   " << "A " << "B " << "C " << "D " << "E " << "F " << "G " << "H " << "I " << "J " << "\n";
	for (int i = 0; i < n; i++)
	{
		if (i < 9)
		{
			cout << i + 1 << "  ";
		}
		else
		{
			cout << i + 1 << " ";
		}
		for (int j = 0; j < m; j++)
		{
			if (mas[i][j] >= 0 && mas[i][j]<5)
			{
				cout<<"- ";
			}
			if (mas[i][j] == 5)
			{
				cout << shot << " ";
			}
			if (mas[i][j]<0)
			{
				cout << killed_ship << " ";
			}
		}
		cout << "\n";
	}
}