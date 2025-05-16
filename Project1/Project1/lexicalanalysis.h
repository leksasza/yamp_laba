#pragma once
#include<vector>
#include<string>
#include <fstream>
#include<sstream>
#include "hashtable.h"
#include "State.h"
#include"dfa.h"
#include"hashitem.h"
using namespace std;
class lexical_analysis {
private:
	/* Хранит: ДКА, вектор строк в котором хранится весь текст файла, строку с сохранённым ключевым словом,
	количество ошибок, хэш-таблицу и вектор строк в котором перечисляются ошибки*/
	vector<string>program;
	vector<string>key_words;
	vector<string>deviders;
	//int index_of_word;
	dfa program_dfa;
	string save_word;
	int mistakes_num = 0;
	hash_table ht;
	vector<string>mistakes;
public:
	friend class parser;
	lexical_analysis() {};
	hash_table get_ht() {
		return ht;
	}
	lexical_analysis operator=(lexical_analysis l2)
	{
		key_words = { "return", "int", "float", "Type" };
		deviders = { "(", ")", "%", "/", "+", "*", ";", "{","}",",","=" };
		dfa dftemp("lexical_dfa.txt");
		program_dfa = dftemp;
		program = l2.program;
		return *this;
	}
	lexical_analysis(vector<string>Program) {
		program = Program;
		key_words = { "return", "int", "float", "Type"};
		deviders = { "(", ")", "%", "/", "+", "*", ";", "{","}",",","="};
		dfa dftemp("lexical_dfa.txt");
		program_dfa = dftemp;
		//index_of_word = 0;
	}
	string Get_Word(int index) {
		//index_of_word++;
		if (index < program.size())
			return program[index];
		else
			return "out of range";
	}
	int Get_Size() {
		return program.size();
	}
	pair<int, string> Bypass(string curr_str, int q) { // проход
		// проверяем является ли слово ключевым или знаком
		auto itr = find(key_words.begin(), key_words.end(), curr_str);
		if (itr != key_words.end()) {
			ht.insert_element(curr_str, "key word");
			pair<int, string>ret(q, "key word");
			return ret;
		}
		itr = find(deviders.begin(), deviders.end(), curr_str);
		if (itr != deviders.end()) {
			ht.insert_element(curr_str, "deviders");
			pair<int, string>ret(q, "deviders");
			return ret;
		}
		State current_state = program_dfa.get_state(q);
		for (pair<int, string> state: current_state.vec) {
			if (state.second == "char") {
				if (isalpha(curr_str[0]))
				for (int j = 0; j < curr_str.length(); j++) {
					if (!isalpha(curr_str[j]) && !isdigit(curr_str[j])) {
						if (j == curr_str.length() - 2 && curr_str[j] == '(' && curr_str[j + 1] == ')') {
							ht.insert_element(curr_str, "Id");
							pair<int, string>ret(state.first, "Id");
							return ret;
						}
						else
							break;
					}
					if (j == curr_str.length() - 1) {
						ht.insert_element(curr_str, "Id");
						pair<int, string>ret(state.first, "Id");
						return ret;
					}
				}
			}
			if (state.second == "digit") {
				for (int j = 0; j < curr_str.length(); j++) {
					if (!isdigit(curr_str[j]) && curr_str[j]!='.')
						break;
					if (j == curr_str.length() - 1)
					{
						auto itr2 = find(curr_str.begin(), curr_str.end(), '.');
						if (itr2 != curr_str.end()) {
							pair<int, string>ret(state.first, "float");
							return ret;
						}
						else {
							pair<int, string>ret(state.first, "int");
							return ret;
						}
					}
				}
			}
		}
		mistakes_num++;
		mistakes.push_back("Incorrect symbol: " + curr_str);
		return pair<int, string>(q, "no type");
	}
	friend ostream& operator <<(ostream& out, const lexical_analysis& l_a) {
		out << l_a.ht << '\n';
		out << "Mistakes amount: " << l_a.mistakes_num << "\n\n";
		for (int i = 0; i < l_a.mistakes.size(); i++)
			out << l_a.mistakes[i] << '\n';
		return out;
	};
};