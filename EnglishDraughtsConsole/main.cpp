/*
Основной модуль приложения "Английские шашки".
*/

#include <clocale>
#include <iostream>
#include "english_draughts.h"
#include "functions.h"


int main()
{
	setlocale(LC_ALL, "Rus");
	// Приветствие и начало игры
	std::cout << "Добро пожаловать в 'Английские шашки'!\n";
	show_menu();
	//
	English_Draughts draughts;
	draughts.play();
	system("pause");
}