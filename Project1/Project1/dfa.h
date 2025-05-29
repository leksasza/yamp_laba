#pragma once
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include "State.h"
using namespace std;
class dfa {
public:
	vector<State>DFA;
	int finite_state;
public:
	dfa() {
		DFA = {};
		finite_state = 0;
	};
	dfa(string file_name) { // конструктор по умолчанию считывает дка из файла по названию
		ifstream fin_dfa(file_name);
		int num;
		string temp;
		while (!fin_dfa.eof()) {
			vector<pair<int, string>>temp_vec; // Временный вектор для состояния, храним в нём номер возможного перехода и строку
			while (!fin_dfa.eof()) { // Считывание текущего состояния
				fin_dfa >> num; // считываем номер состояния в которое можем перейти
				if (num == -1) // -1 идентифицирует переход к следующему состоянию, если видим его выходим
					break;
				fin_dfa >> temp; // считываем по чему можем перейти в это состояние
				pair<int, string>p(num, temp);
				temp_vec.push_back(p);
			}
			State temp_state(temp_vec); // создаём состояние из временного вектора
			DFA.push_back(temp_state); // добавляем состояние в массив состояние
			temp_vec.clear();
		}
		finite_state = DFA.size() - 1;
	}
	State get_state(int index) {
		return DFA[index];
	}
};
