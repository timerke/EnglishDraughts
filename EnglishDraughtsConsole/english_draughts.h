/*
Заголовочный файл english_draughts.h содержит определение
класса English_Draughts для объектов, представляющих игру
'Английские шашки'.
*/

#pragma once
#ifndef ENGLISHDRAUGHTS_H
#define ENGLISHDRAUGHTS_H

#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include "functions.h"


// Постоянные
const char* COLUMNS = "abcdefgh"; // названия столбцов доски
const char* ROWS = "87654321"; // названия строк доски
int N = 8; // количество клеток в строке и столбце


/**
 * Класс определяет игру в английские шашки.
 */
class English_Draughts
{
	std::vector<std::string> board; // доска с расположением шашек
	std::vector<std::string> trial_board; // пробная доска, на которой
	// проверяются ходы
	const char* CHECKER = "O0"; // шашки, которыми играют игроки 0 и 1
	const char* KING = "oq"; // дамки, которыми играют игроки 0 и 1
	int player; // номер игрока, который ходит, 0 или 1
	std::vector<std::pair<int, int>> cells; // ходы игрока по клеткам
	bool is_king; // true, если игрок ходит дамкой

public:
	// Конструктор класса
	English_Draughts();
	// Метод проверяет возможность хода
	bool check_move(std::vector<std::string>*);
	// Метод проверяет собственника шашки в клетке
	bool check_owner(std::string);
	// Метод из введенного ответа игрока получает название
	// клетки, шашкой на которой он собирается ходить, и ходы
	void determine_cell_and_move(std::string*, std::string*,
		std::vector<std::string>*);
	// Метод находит номер строки или столбца, на которой
	// расположена клетка
	int find_number(std::string, bool);
	// Метод получает ход от игрока
	void get_move();
	// Метод делает ход
	void make_move();
	// Метод реализует игру
	void play();
};


/**
 * Конструктор класса. Создается доска с исходных расположением
 * шашек. Первым будет ходить игрок 0, он играет черными шашками.
 */
English_Draughts::English_Draughts()
{
	// Расположение шашек на доске:
	// O - черная шашка, 0 - белая шашка
	board = { " O O O O", "O O O O ", " O O O O",
		"        ", "        ",
		"0 0 0 0 ", " 0 0 0 0", "0 0 0 0 " };
	// Первый ходит игрок под номер 0 черными шашками
	player = 0;
}


/**
 * Метод из введенного игроком ответа определяет название клетки,
 * шашкой на которой игрок хочет сделать ход, и ходы этой шашкой.
 * @param response: строка, введенная игроком;
 * @param cell: строка, в которую записывается название клетки,
 * из которой игрок будет ходить шашкой;
 * @param next_cells: вектор, в который будут записаны названия
 * клеток, по которым будет перемещаться шашка игрока.
 */
void English_Draughts::determine_cell_and_move(std::string* response,
	std::string* cell, std::vector<std::string>* next_cells)
{
	// Сначала определяется исходная клетка
	std::size_t pos_prev = 0;
	std::size_t pos = response->find_first_of(' ', pos_prev);
	*cell = response->substr(pos_prev, pos - pos_prev);
	// Потом определяются клетки для ходов
	pos_prev = pos + 1;
	do
	{
		pos = response->find_first_of(' ', pos_prev);
		next_cells->push_back(response->substr(pos_prev, pos - pos_prev));
	} while (pos != std::string::npos);
}


/**
 * Метод проверяет возможность хода игрока.
 * @param cells: название клетки, в которую игрок хочет сделать ход.
 * @return: true, если ход возможен, иначе false.
 */
bool English_Draughts::check_move(std::vector<std::string>* next_cells)
{
	// Пробная доска становится такой, как настоящая
	trial_board = board;
	// Флаг, показывающий, что игрок берет шашки противника
	bool take = false;
	int column, row; // столбец и строка клетки
	int enemy = player == 0 ? 1 : 0; // противник игрока
	for (std::vector<std::string>::iterator cell = next_cells->begin();
		cell != next_cells->end(); cell++)
	{
		// Определяем номер строки, на которой расположена клетка,
		// в которую нужно переместить шашку игрока
		row = find_number(*cell, true);
		if (row == N)
		{
			// Игрок некорректно ввел номер строки, на которой
			// расположена клетка
			return false;
		}
		// Определяем номер столбца, на которой расположена клетка,
		// в которую нужно переместить шашку игрока
		column = find_number(*cell, false);
		if (column == N)
		{
			// Игрок некорректно ввел номер столбца, на котором
			// расположена клетка
			return false;
		}
		// Проверяем, что клетка доступна для хода
		if (trial_board[row][column] != ' ')
		{
			// Клетка занята другой шашкой
			return false;
		}
		// Проверяем, что простая шашка ходит только вперед:
		// у игрока 0 шашки могут двигаться вниз, у игрока 1 - вверх
		int column_step = column - cells.at(cells.size() - 1).second;
		int row_step = row - cells.at(cells.size() - 1).first;
		if (!is_king && !player && row_step < 0)
		{
			// Шашка движется в неправильном направлении у игрока 0
			return false;
		}
		if (!is_king && player && row_step > 0)
		{
			// Шашка движется в неправильном направлении у игрока 1
			return false;
		}
		// Проверяем, что шаг - одна клетка или через одну по диагонали
		if (abs(row_step) > 2 || abs(column_step) > 2 ||
			abs(row_step) != abs(column_step))
		{
			// Шашка делает ход не по диагонали или слишком большой
			return false;
		}
		// Если шашка сделала перешагнула через клетку, в этой клетке
		// должна быть шашка противника
		if (abs(row_step) == 2 &&
			trial_board[row - row_step / 2][column - column_step / 2] != CHECKER[enemy]
			&& trial_board[row - row_step / 2][column - column_step / 2] != KING[enemy])
		{
			// Шашка не перешагнула через шашку противника
			return false;
		}
		// Игрок может ходить на одну клетку только один раз
		if (abs(row_step) == 1 && next_cells->size() > 1)
		{
			// Если игрок делает несколько шагов без взятия шашек
			// противника, это ошибка
			return false;
		}
		// Делаем ход:
		// убираем шашку с предыдущей клетка
		trial_board[row - row_step][column - column_step] = ' ';
		if (abs(row_step) == 2)
		{
			// Если надо, съедаем шашку противника
			trial_board[row - row_step / 2][column - column_step / 2] = ' ';
			take = true;
		}
		// Перемещаем шашку на новую клетку
		trial_board[row][column] = CHECKER[player];
	}
	// Проверяем, что в конечной точке игрок не может взять
	// шашку противника, если до этого брал шашки противника
	if (take)
	{
		// Для игрока 0
		if (!player && 0 <= row + 2 && row + 2 < N &&
			0 <= column + 2 && column + 2 < N &&
			(board[row + 1][column + 1] == CHECKER[enemy] || 
				board[row + 1][column + 1] == CHECKER[enemy]) &&
			board[row + 2][column + 2] == ' ')
		{
			// Игрок может взять шашку противника, но не взял
			return false;
		}
		if (!player && 0 <= row + 2 && row + 2 < N &&
			0 <= column - 2 && column - 2 < N &&
			(board[row + 1][column - 1] == CHECKER[enemy] ||
				board[row + 1][column - 1] == CHECKER[enemy]) &&
			board[row + 2][column - 2] == ' ')
		{
			// Игрок может взять шашку противника, но не взял
			return false;
		}
		// Для игрока 0 с дамкой
		if (!player && is_king && 0 <= row - 2 && row - 2 < N &&
			0 <= column + 2 && column + 2 < N &&
			(board[row - 1][column + 1] == CHECKER[enemy] ||
				board[row - 1][column + 1] == CHECKER[enemy]) &&
			board[row - 2][column + 2] == ' ')
		{
			// Игрок может взять шашку противника, но не взял
			return false;
		}
		if (!player && is_king && 0 <= row - 2 && row - 2 < N &&
			0 <= column - 2 && column - 2 < N &&
			(board[row - 1][column - 1] == CHECKER[enemy] ||
				board[row - 1][column - 1] == CHECKER[enemy]) &&
			board[row - 2][column - 2] == ' ')
		{
			// Игрок может взять шашку противника, но не взял
			return false;
		}
		// Для игрока 1
		if (player && 0 <= row - 2 && row - 2 < N &&
			0 <= column + 2 && column + 2 < N &&
			(board[row - 1][column + 1] == CHECKER[enemy] ||
				board[row - 1][column + 1] == CHECKER[enemy]) &&
			board[row - 2][column + 2] == ' ')
		{
			// Игрок может взять шашку противника, но не взял
			return false;
		}
		if (player && 0 <= row - 2 && row - 2 < N &&
			0 <= column - 2 && column - 2 < N &&
			(board[row - 1][column - 1] == CHECKER[enemy] ||
				board[row - 1][column - 1] == CHECKER[enemy]) &&
			board[row - 2][column - 2] == ' ')
		{
			// Игрок может взять шашку противника, но не взял
			return false;
		}
		// Для игрока 1 с дамкой
		if (player && is_king && 0 <= row + 2 && row + 2 < N &&
			0 <= column + 2 && column + 2 < N &&
			(board[row + 1][column + 1] == CHECKER[enemy] ||
				board[row + 1][column + 1] == CHECKER[enemy]) &&
			board[row + 2][column + 2] == ' ')
		{
			// Игрок может взять шашку противника, но не взял
			return false;
		}
		if (player && is_king && 0 <= row + 2 && row + 2 < N &&
			0 <= column - 2 && column - 2 < N &&
			(board[row + 1][column - 1] == CHECKER[enemy] ||
				board[row + 1][column - 1] == CHECKER[enemy]) &&
			board[row + 2][column - 2] == ' ')
		{
			// Игрок может взять шашку противника, но не взял
			return false;
		}
	}
	return true;
}


/**
 * Метод проверяет, что игрок, делающий ход, владеет шашкой,
 * расположенной в клетке.
 * @param cell: название клетки, из которой игрок хочет передвинуть
 * шашку.
 * @return: true, если игрок, делающий ход, владеет шашкой
 * в клетке, иначе false.
 */
bool English_Draughts::check_owner(std::string cell)
{
	// Определяем номер строки, на которой расположена клетка
	int row = find_number(cell, true);
	if (row == N)
	{
		// Игрок некорректно ввел номер строки, на которой
		// расположена клетка с шашкой
		return false;
	}
	// Определяем номер столбца, на которой расположена клетка
	int column = find_number(cell, false);
	if (column == N)
	{
		// Игрок некорректно ввел номер столбца, на котором
		// расположена клетка с шашкой
		return false;
	}
	// Проверяем, принадлежит ли шашка игроку
	if (board[row][column] == CHECKER[player])
	{
		// Шашка принадлежит игроку
		is_king = false;
		cells.push_back(std::make_pair(row, column));
		return true;
	}
	else if (board[row][column] == KING[player])
	{
		// Шашка принадлежит игроку, но шашка - дамка
		is_king = true;
		cells.push_back(std::make_pair(row, column));
		return true;
	}
	// Проверяем, является ли шашка
	return false;
}


/**
 * Метод возвращает номер столбца или строки по названию клетки.
 * @param cell: название клетки;
 * @param is_row: если параметр true, то ищется номер строки,
 * на которой находится клетка, иначе false.
 * @return: номер столбца или строки, на которой расположена
 * клетка.
 */
int English_Draughts::find_number(std::string cell, bool is_row)
{
	int number;
	for (number = 0; number < N; number++)
	{
		if (is_row && ROWS[number] == cell.at(0))
		{
			return number;
		}
		else if (!is_row && COLUMNS[number] == cell.at(1))
		{
			return number;
		}
	}
	return number;
}


/**
 * Метод получает ход от игрока.
 */
void English_Draughts::get_move()
{
	cells.clear(); // очищаем вектор от предыдущих ходов
	while (true)
	{
		std::cout << "Игрок " << player + 1 << ": ";
		std::string response; // ввод игрока
		std::getline(std::cin, response);
		// Название клетки, в которой находится шашка игрока
		std::string current_cell;
		// Названия клеток, в которые игрок собирается делать ходы
		std::vector<std::string> next_cells;
		// Получаем названия клеток из ввода игрока
		determine_cell_and_move(&response, &current_cell, &next_cells);
		// Проверяем, принадлежит ли шашка в клетке current_cell
		// игроку
		if (!check_owner(current_cell))
		{
			// Если шашка в клетке current_cell не принадлежит
			// игроку player
			std::cout << "Игрок " << player + 1 << " не может ходить из клетки "
				<< current_cell << "! Повторите ход.\n";
			continue;
		}
		// Проверяем, можно ли сделать введенные ходы
		if (!check_move(&next_cells))
		{
			// Если ход невозможен
			std::cout << "Игрок " << player + 1 << " не может ходить в клетку "
				<< 1 << "! Повторите ход.\n";
		}
		else
		{
			// Если ход возможен
			make_move();
			break;
		}
	}
	// Меняем игрока, кто должен ходить
	player = player == 0 ? 1 : 0;
}


/**
 * Метод передвигает шашку игрока по доске.
 */
void English_Draughts::make_move()
{
	board = trial_board;
}


/**
 * Метод реализует игру.
 */
void English_Draughts::play()
{
	int i = 1; // номер хода игроков
	while (true)
	{
		// Показываем игровую доску
		std::cout << "Игровая доска на ходе " << i << ":\n";
		show_board(&board);
		get_move();
	}
	
}

#endif
