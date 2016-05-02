#include "stdafx.h"
#include <iostream>
#include <string>
#include <conio.h>
#include <ctime>
using namespace std;

string Field[25];  //����� ��� ����,����������� � ���� ������� 
int Chips_of_Players[2];

void Rules() {
	system("cls");
	cout << "     ---�������---\n";
	cout << "-������ ����� �� �������, ���������� ���� ����� ������ ������� �������.\n\n-����� ������ ����� ����� ������� ��� �����. �������� ���� ���������� ��������� ����.\n\n-�� ���� ��� �������� �� ������ �� ������ ������������ �����. � ������ �� ��� ����� ����� ����������� ����� ���� ����� �� ����� ���������� �������, ������� ������ �� ����� �� ������. ��������, ���� ������ 2 � 4 ����, ����� ����� �� ���� ��� ����������� ���� (�����) �� ����� �� 2 ������, ������ � �� 4 ������, ���� ����������� ���� ����� ������� �� 2, ����� � �� 4 ������ (���, ��������, ������� �� 4 ����� �� 2).\n\n-���� �� ����� ������ �������� ���������� ����� ����� (�����), �� �������� ���� �����������, � ����� �������� ����������� ������� 4 �����������.\n\n-������ ����������� ����� ������ �������� �� ������ ���������� �����, �������� �� ����� (���� ������ 4 ����, �� ����� ������ �� 1, 2 ��� 3 ������ ������ � ����� ������ �� ������ 4).\n\n-����� ����� ������� ���� ����� ������ � ��������� ������ ��� � ������, ������� ������� ����� �� ������.\n\n-���� ����������� ����������� ��� �������� ���������� ����� ���, ����� ���������� ���. �� ���� ����������� ������� ���� �� ���� ��� ����, ����� �� ����� ���������� �� ��, ���� ���� ������ ��� ��� ���������.\n\n-����� ��� ����� ������ � �������� �������� �� ����� �������� � ���� ���, ���������� ������ ����� ����� ������ ���������� �� �� �����(out). ����� ����� ���� ���������� �� �����, ����� ����� ������, �� ������� ��� �����, ����� ����� �����, �������� �� ����� �� ������ (�� ���� �����, ������� �� ������� ������, ����� ����������, ���� ������ �������, �� ������ �� ���� � ���� ������ ������). ���� ��� ����� � ���� ��������� ����� � ���� �����, ��� �������� ����� �����, �� ����� ������������ �� ����� ����� �� ������ � ���������� �������.\n\n-���������� ���, ��� ������ �������� ��� ���� ����� �� ����.\n\n";
	_getch();
}

void Field_Creation() {  //�������� ������� �����
	Field[0] = "-------------------------------------------------";
	Field[24] = Field[0];
	Field[1] = "|   |   |   |   |   |   |   |   |   |   |   |   |";
	Field[23] = Field[22] = Field[2] = Field[1];
	for (int i = 3; i <= 21; i++) {
		Field[i] = "|                       |                       |";
	}
}

void Field_Display() {     //����� ���� �� �����
	cout << " 12  11  10   9   8   7   6   5   4   3   2   1  " << endl;
	for (int i = 0; i < 25; i++) {
		cout << Field[i] << endl;
	}
	cout << "  13  14  15  16  17  18  19  20  21  22  23  24 " << endl;
}

int First_Step() {    //�����,��� ����� ������
	srand(clock());
	int Random_Choice = rand() % 2 + 1;
	return Random_Choice;
}

int Throw_Block() {  //�������� ������ �������
	srand(clock());
	return (rand() % 6 + 1);
}

void Put_Chip_to_Position(int position, int place, char chip) { //������� ��� ����������� ����� �� ��������� �������
	if (position <= 12) {
		Field[place][Field[0].size() - 4 * position + 1] = chip;
	}
	else {
		Field[24 - place][4 * (position % 12) - 2] = chip;
	}
}

bool Is_This_Turn_Possible(int position1, int position2, char chip) { //��������,����� �� ��������� ��� �� pos1 � pos2
	if ((position1 > position2) && (position1 <= 19)) //����� ������ ������
		return false;
	if (position1 == position2) return false;
	//���������� ��������,����� ����� ��������� � ������ �� �����
	char pos1, pos2; //���������� ������� �������������� �����
	if (position1 <= 12) {
		pos1 = Field[1][Field[0].size() - 4 * position1 + 1]; //���� ������ ������ � ������� ��������
	}
	else {
		if (position1 == 24) {
			pos1 = Field[23][Field[0].size() - 3];
		}
		else
			pos1 = Field[23][4 * (position1 % 12) - 2]; //���� � ������
	}

	if (position2 <= 12) {
		pos2 = Field[1][Field[0].size() - 4 * position2 + 1];  //���� ������ ������ � ������� ��������
	}
	else {
		if (position2 == 24) {
			pos2 = Field[23][Field[0].size() - 3];
		}
		else
			pos2 = Field[23][4 * (position2 % 12) - 2];  //���� � ������
	}
	if (pos1 != chip) return false;
	if (pos1 == ' ')  //�� ������ ������ ������ ������
		return false;
	if (pos2 == ' ') return true;  //� ������ ������ ����� ������ ���� ���
	if (pos1 == pos2) return true;  //������ ������ ������ ����� ���� �� �����,� ���������� �����
	else return false;
}

void Turn(int position1, int position2,char chip) {
	int place1, place2, i;
	if (!Is_This_Turn_Possible(position1, position2,chip)) { //�������� ����������� ����
		cout << "~~������ ��� ����������,���������� ��� ���~~" << endl;
	}
	else     //����� ��������� ����� � ����. position-���������� � ������, place-����� ������. ������� ������������ ���������� ������� ����� � ������,����� ������ ������,� ������� ��������� ����� ������� �����,����� ����� ����������� ������ ��
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

		swap(Field[place1][position1], Field[place2][position2]); //���� ��� ��������� �����,�� �������� ��������������� ����� �� ������ ������ � ������� �� ������ ������
	}
}

void Start_Game(int who_is_first) {
	Field_Creation();
	if (who_is_first == 1) {  //��������� ����� ������,������� ����� ������
		for (int i = 1; i <= 15; i++) {
			Put_Chip_to_Position(1, i, 'O');  //����������� ���� �����
			Put_Chip_to_Position(13, i, 'X'); //                        �� ����
		}
	}
	else {
		for (int i = 1; i <= 15; i++) {
			Put_Chip_to_Position(1, i, 'X');
			Put_Chip_to_Position(13, i, 'O');
		}
	}
	Chips_of_Players[0] = Chips_of_Players[1] = 15;
	Field_Display();  //����� �� ����� ������� � ���� �����
}

void Game() {
	cout << "--����� ���������� � ���� '�����'.--\n\n ������ ��������� �������?(y/n)\n";
	char request;
	cin >> request;
	if (request == 'y') {
		Rules();
	}
	system("cls");
	int Who_is_first;
	char chip1, chip2; //����� ������� � ������� ������
	
	if (First_Step() == 1) {
		cout << "   -- ������� ����� ������ (O) -- " << endl;
		Who_is_first = 1;
		chip1 = 'O';
		chip2 = 'X';
	}                                //����� ����� ��� ������� ������ � ����������� �� ����,��� ������ �����
	else {
		cout << "   -- ������� ����� �������� (X) --" << endl;
		Who_is_first = 2;
		chip1 = 'X';
		chip2 = 'O';
	}
	cout << endl;
	Start_Game(Who_is_first);
}

int main()
{
	setlocale(LC_ALL, "rus");
	Game();
	system("pause");
    return 0;
}

