#include <iostream>
#include<fstream>
#include<string>
#include<vector>
#include<cctype>
#include"hashtable.h"
#include"lexicalanalysis.h"
#include"State.h"
#include"parser.h"
#include"Semantic.h"
using namespace std;
int main() {
	setlocale(LC_ALL, "Russian");
	vector<string>Program;
	string FileName;
	cout << "Введите название файла" << endl;
	cin >> FileName;
	ifstream fin(FileName);
	while (!fin.is_open())
	{
		cout << "Некорректное название файла. Попробуйте ещё раз" << endl;
		cin >> FileName;
		fin.open(FileName);
		if (fin.is_open())
			break;
	}
	string temp;
	while (fin >> temp)
		Program.push_back(temp);
	fin.close();
	Parser parser(Program);
	ofstream fout("hashtable.txt");
	fout << parser;
	fout.close();
	ofstream fout1("tree.txt");
	parser.Print_Tree(fout1);
	fout1.close();
	ofstream fout2("reversenotation.txt");
	Semantic sc(parser.get_tree(), parser.get_ht());
	fout2 << sc;
	fout2.close();
	return 0;
}