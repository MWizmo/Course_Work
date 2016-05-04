#include "stdafx.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <ctime>
#include <Windows.h>
#include <vector>
using namespace std;

string Field[25];  //Доска для игры,выполненная в виде массива 
int Chips_of_Players[2];

void Rules() {
	system("cls");
	cout << "     ---Правила---\n";
	cout << "-Игроки ходят по очереди, передвигая свои фишки против часовой стрелки.\n\n-Перед каждым ходом игрок бросает две кости. Выпавшие очки определяют возможные ходы.\n\n-За один ход делается от одного до четырёх передвижений шашки. В каждом из них игрок может передвинуть любую свою шашку на такое количество пунктов, которое выпало на одной из костей. Например, если выпало 2 и 4 очка, игрок может за этот ход передвинуть одну (любую) из шашек на 2 пункта, другую — на 4 пункта, либо передвинуть одну шашку сначала на 2, затем — на 4 пункта (или, наоборот, сначала на 4 потом на 2).\n\n-Если на обеих костях выпадает одинаковое число очков (дубль), то выпавшие очки удваиваются, и игрок получает возможность сделать 4 перемещения.\n\n-Каждое перемещение шашки должно делаться на полное количество очков, выпавшее на кости (если выпало 4 очка, то пойти шашкой на 1, 2 или 3 пункта нельзя — можно только на полные 4).\n\n-Игрок может ставить свои фишки только в свободные ячейки или в ячейки, занятые фишками этого же игрока.\n\n-Если разрешённых перемещений для выпавшей комбинации очков нет, игрок пропускает ход. Но если возможность сделать хотя бы один ход есть, игрок не может отказаться от неё, даже если данный ход ему невыгоден.\n\n-Когда все шашки игрока в процессе движения по доске попадают в свой дом, следующими ходами игрок может начать выставлять их за доску(out). Шашка может быть выставлена за доску, когда номер пункта, на котором она стоит, равен числу очков, выпавших на одной из костей (то есть шашку, стоящую на крайнем пункте, можно выставлять, если выпала единица, на втором от края — если выпала двойка). Если все шашки в доме находятся ближе к краю доски, чем выпавшее число очков, то может выставляться за доску шашка из пункта с наибольшим номером.\n\n-Выигрывает тот, кто первым выставил все свои шашки за борт.\n\n";
	_getch();
}

void Field_Creation() {  //Создание игровой доски
	Field[0] = "-------------------------------------------------";
	Field[24] = Field[0];
	Field[1] = "|   |   |   |   |   |   |   |   |   |   |   |   |";
	Field[23] = Field[22] = Field[2] = Field[1];
	for (int i = 3; i <= 21; i++) {
		Field[i] = "|                       |                       |";
	}
}

void Field_Display() {     //Вывод поля на экран
	cout << " 12  11  10   9   8   7   6   5   4   3   2   1  " << endl;
	for (int i = 0; i < 25; i++) {
		cout << Field[i] << endl;
	}
	cout << "  13  14  15  16  17  18  19  20  21  22  23  24 " << endl;
}

int First_Step() {    //Выбор,кто ходит первым
	srand(clock());
	int Random_Choice = rand() % 2 + 1;
	return Random_Choice;
}

int Throw_Block() {  //Имитация броска кубиков
	srand(clock());
	return (rand() % 6 + 1);
}

void Put_Chip_to_Position(int position, int place, char chip) { //Функция для выставления фишек на начальные позиции
	if (position <= 12) {
		Field[place][Field[0].size() - 4 * position + 1] = chip;
	}
	else {
		Field[24 - place][4 * (position % 12) - 2] = chip;
	}
}

void Delete_Chip(int position) {  //Удаление одной фишки из ячейки(возможно,с вершины горки)
	int place;     //Номер строки,в которой находится вершина горки
	if (position <= 12) {   //Если ячейка находится в верхней половине
		position = Field[0].size() - 4 * position + 1;  //Индекс середины ячейки в строке
		place = 1;
		for (int i = 2; i <= 16; i++) {
			if (Field[i][position] != ' ') {  //Поиск вершины идет до обнаружения пустоты
				place++;
			}
			else break;
		}
	}
	else { //Если ячейка находится в нижней половине
		position = 4 * (position % 12) - 2;
		place = 23;
		for (int i = 23; i >= 8; i--) {
			if (Field[i][position] != ' ') {
				place--;
			}
			else {
				place++;  //Magic
				break;
			}
		}
	}
	if (Field[place][position] == ' ') {   //Исключение удаления из пустой ячейки
		cout << "-Эта ячейка пуста,попробуйте еще раз-" << endl;
	}
	else
		Field[place][position] = ' ';
}

bool Is_This_Turn_Possible(int position1, int position2, char chip) { //Проверка,можно ли совершить ход из pos1 в pos2
	if ((position1 > position2) && (position1 <= 19)) //Назад ходить нельзя
		return false;
	if (position1 == position2) return false;
	//Происходит проверка,какие фишки находятся в каждой из ячеек
	char pos1, pos2; //Координаты центров обрабатываемых ячеек
	if (position1 <= 12) {
		pos1 = Field[1][Field[0].size() - 4 * position1 + 1]; //Если первая ячейка в верхней половине
	}
	else {
		if (position1 == 24) {
			pos1 = Field[23][Field[0].size() - 3];
		}
		else
			pos1 = Field[23][4 * (position1 % 12) - 2]; //Если в нижней
	}

	if (position2 <= 12) {
		pos2 = Field[1][Field[0].size() - 4 * position2 + 1];  //Если вторая ячейка в верхней половине
	}
	else {
		if (position2 == 24) {
			pos2 = Field[23][Field[0].size() - 3];
		}
		else
			pos2 = Field[23][4 * (position2 % 12) - 2];  //Если в нижней
	}
	if (pos1 != chip) return false;
	if (pos1 == ' ')  //Из пустой ячейки ходить нельзя
		return false;
	if (pos2 == ' ') return true;  //В пустую ячейку можно ходить хоть чем
	if (pos1 == pos2) return true;  //Нельзя класть разные фишки друг на друга,а одинаковые можно
	else return false;
}

bool Is_This_Finish_Possible(int position, vector<int> &vec, int home) { //Проверка,можно ли вывести фишку с поля
	if ((position >= home) && (position <= home + 5))  //Находится ли фишка в своем доме
	{
		if (home == 19) {  //Для первого игрока
			for (int i = 0; i < vec.size(); i++) {
				if ((25 - position) <= vec[i]) { //Если расстояние до конца строки меньше длины возможного хода,
					swap(vec[i], vec[vec.size() - 1]);  // удаляем фишку с поля и убираем ход из вектора
					vec.pop_back();
					return true;
				}
			}
			return false;
		}
		else {
			for (int i = 0; i < vec.size(); i++) {  //Аналогично для второго игрока
				if ((13 - position) <= vec[i]) {
					swap(vec[i], vec[vec.size() - 1]);
					vec.pop_back();
					return true;
				}
			}
			return false;
		}
	}
}

void Turn(int position1, int position2,char chip) {
	int place1, place2, i;
	if (!Is_This_Turn_Possible(position1, position2,chip)) { //Проверка возможности хода
		cout << "~~Данный ход невозможен,попробуйте еще раз~~" << endl;
	}
	else     //Поиск координат фишки в поле. position-координата в строке, place-номер строки. Вначале определяется координаты середин ячеек в строке,затем ищется строка,в которой находится самая верхняя фишка,чтобы затем переместить именно ее
	{
		if (position1 <= 12) {
			position1 = Field[0].size() - 4 * position1 + 1;
			place1 = 1;
			for (i = 2; i <= 16; i++) {
				if (Field[i][position1] != ' ') {
					place1++;
				}
				else break;

			}
		}
		else {
			if (position1 == 24) {
				position1 = Field[0].size() - 3;
			}
			else
				position1 = 4 * (position1 % 12) - 2;
			place1 = 23;
			for (i = 23; i >= 8; i--) {
				if (Field[i][position1] != ' ') {
					place1--;
				}
				else {
					place1++;  //Magic
					break;
				}
			}
		}

		if (position2 <= 12) {
			position2 = Field[0].size() - 4 * position2 + 1;
			place2 = 1;
			for (i = 1; i <= 15; i++) {   //Magic
				if (Field[i][position2] != ' ') {
					place2++;
				}
				else break;
			}
		}
		else {
			if (position2 == 24) {
				position2 = Field[0].size() - 3;
			}
			else
				position2 = 4 * (position2 % 12) - 2;
			place2 = 23;
			for (i = 23; i >= 8; i--) {
				if (Field[i][position2] != ' ') {
					place2--;
				}
				else break;

			}
		}

		if (Field[place1][position1] != chip) {
			cout << "~~Вы не можете ходить чужими фишками!~~" << endl;
			return;
		}
		else
		swap(Field[place1][position1], Field[place2][position2]); //Если все выполнено верно,то меняется соответствующая фишка из первой ячейки и пустота из второй ячейки
	}
}

void Round(char chip, int number,int home) {
	int  block1, block2, i; //Показания кубиков
	cout << "--Ход игрока №" << number << endl;
	block1 = Throw_Block();
	Sleep(100);  //Для как можно большего рандома
	block2 = Throw_Block();
	vector <int> points;  //Вектор,хранящий оставшиеся ходы
	if (block1 == block2) {
		cout << "Показания кубиков : " << block1 << " и " << block2 << ". Дубль! Вы можете сделать четыре хода" << endl;
		for (i = 0; i < 4; i++)  //Заполнение в случае дубля
			points.push_back(block1);
	}
	else {  //Заполение при обычном броске
		cout << "Показания кубиков : " << block1 << " и " << block2 << "." << endl;
		points.push_back(block1);
		points.push_back(block2);
	}

	while (!points.empty()) {
		//Тело раунда
	}
}

void Start_Game(int who_is_first) {
	Field_Creation();
	if (who_is_first == 1) {  //Рандомный выбор игрока,который ходит первым
		for (int i = 1; i <= 15; i++) {
			Put_Chip_to_Position(1, i, 'O');  //Выставление всех фишек
			Put_Chip_to_Position(13, i, 'X'); //                        на поле
		}
	}
	else {
		for (int i = 1; i <= 15; i++) {
			Put_Chip_to_Position(1, i, 'X');
			Put_Chip_to_Position(13, i, 'O');
		}
	}
	Chips_of_Players[0] = Chips_of_Players[1] = 15;
	Field_Display();  //Вывод на экран готовой к игре доски
}

void Game() {
	cout << "--Добро пожаловать в игру 'Нарды'.--\n\n Хотите прочитать правила?(y/n)\n";
	char request;
	cin >> request;
	if (request == 'y') {
		Rules();
	}
	system("cls");
	cout << "\n       --Важно! Правила ввода--\n-Для передвижения фишки(хода) необходимо ввести через пробел номера ячеек: 1-из которой вы хотите передвинуть фишку,2-в которую вы хотите поместить фишку\n-Если вы не можете сделать ни одного хода,пропишите 0 0\n-Если ваши фишки находятся в доме, и вы хотите вывести их с доски,пропишите через пробел номер позиции,из которой вы хотите убрать фишку, и ноль\n\n";
	_getch();
	system("cls");

	int Who_is_first;
	char chip1, chip2; //Фишки первого и второго игрока
	int home1 = 19, home2 = 7; //Позиции,соответствующие началу "дома" для соответствующих фишек
	
	if (First_Step() == 1) {
		cout << "   -- Первыми ходят нолики (O) -- " << endl;
		Who_is_first = 1;
		chip1 = 'O';
		chip2 = 'X';
	}                                //Выбор фишек для каждого игрока в зависимости от того,кто первым ходит
	else {
		cout << "   -- Первыми ходят крестики (X) --" << endl;
		Who_is_first = 2;
		chip1 = 'X';
		chip2 = 'O';
	}
	cout << endl;
	Start_Game(Who_is_first);

	int current_player = 0;
	for (; ; ) {   //Собственно игровой процесс
		current_player = current_player % 2 + 1;
		if (current_player == 1) {
			Round(chip1, 1, home1);
		}
		else {
			Round(chip2, 2, home2);
		}
		if (Chips_of_Players[0] == 0) {  //Условие победы
			cout << "--Победил первый игрок. Поздравляем!--" << endl;
			break;
		}
		if (Chips_of_Players[1] == 0) {
			cout << "--Победил второй игрок. Поздравляем!--" << endl;
			break;
		}
	}
}

int main()
{
	setlocale(LC_ALL, "rus");
	Game();
	system("pause");
    return 0;
}

