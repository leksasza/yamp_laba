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
	dfa(string file_name) { // ����������� �� ��������� ��������� ��� �� ����� �� ��������
		ifstream fin_dfa(file_name);
		int num;
		string temp;
		while (!fin_dfa.eof()) {
			vector<pair<int, string>>temp_vec; // ��������� ������ ��� ���������, ������ � �� ����� ���������� �������� � ������
			while (!fin_dfa.eof()) { // ���������� �������� ���������
				fin_dfa >> num; // ��������� ����� ��������� � ������� ����� �������
				if (num == -1) // -1 �������������� ������� � ���������� ���������, ���� ����� ��� �������
					break;
				fin_dfa >> temp; // ��������� �� ���� ����� ������� � ��� ���������
				pair<int, string>p(num, temp);
				temp_vec.push_back(p);
			}
			State temp_state(temp_vec); // ������ ��������� �� ���������� �������
			DFA.push_back(temp_state); // ��������� ��������� � ������ ���������
			temp_vec.clear();
		}
		finite_state = DFA.size() - 1;
	}
	State get_state(int index) {
		return DFA[index];
	}
};