#pragma once
#include "tree.h"
#include <string>
#include "lexicalanalysis.h"
#include<stack>
using namespace std;
class Parser
{
private:
	vector<string>program; // вектор хранит текст программы
	int pos; // позиция в тексте
	int mistakes_num; // колво ошибок
	tree parser_tree; // дерево
	tree temp_tree;
	tree sub_tree;
	vector<string>mistakes; // вектор ошибок 
	string curr_word; 
	lexical_analysis la;
	int q;
	pair<int, string>q_str;
public:
	Parser() {};
	Parser(vector<string>Program) {
		program = Program;
		lexical_analysis l(program);
		la = l;
		pos = 0;
		mistakes_num = 0;
		q = 0;
		parser_tree = tree("Function");
		temp_tree = tree("Begin");
		q_str = pair<int,string>(0, "no type");
		Parse();

	}
	void Parse() {
		// вызываем биген, дескрипшнс, операторс, энд
		parser_tree.add_node("Begin", "Function");
		Begin();
		parser_tree.insert(temp_tree, "Begin");
		temp_tree.cleaner();

		temp_tree = tree("Descriptions");
		parser_tree.add_node("Descriptions", "Function");
		Descriptions(temp_tree);
		parser_tree.insert(temp_tree, "Descriptions");
		temp_tree.cleaner();

		temp_tree = tree("Operators");
		Operators(temp_tree);
		parser_tree.add_node("Operators", "Function");
		parser_tree.insert(temp_tree, "Operators");
		temp_tree.cleaner();

		temp_tree = tree("End");
		parser_tree.add_node("End", "Function");
		End();
		parser_tree.insert(temp_tree, "End");
		temp_tree.cleaner();
	}
	void Begin() {
		temp_tree.add_node("Type", "Begin"); // бегин образовывает нетерминал тайп

		// двигаемся на следующее слово. вызываем лексический анализатор. он возвращает пару q состояние автомата и тип обработанной строки
		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;

		Type(temp_tree); // вызываем тайп
		FunctionName(); // бегин образовывает функшннейм
		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;
		if (curr_word == "{") { // проверяем кончается ли фигурной скобочкой, она образовывается от бегина
			temp_tree.add_node(curr_word, "Begin");
		}
		else {
			mistakes_num++;
			mistakes.push_back("'{' expected");
		}
	}
	void Type(tree& tr) {
		// если слово инт или флоат, добавляем его к нетерминалу тайп, иначе выдаем ошибку
		if (curr_word == "int" || curr_word == "float")
			tr.add_node(curr_word, "Type");
		else {
			mistakes_num++;
			mistakes.push_back("Type expected");
		}
	}
	void FunctionName() {
		temp_tree.add_node("FunctionName", "Begin");
		temp_tree.add_node("Id", "FunctionName");
		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;
		Id(temp_tree); // вызываем фунцию айди
	}
	void Descriptions(tree& tr) { // дескрипшнс (описания переменных)
		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;

		tree subtr = tree("Descr"); // создаем поддерево для первого перечисления переменных
		Descr(subtr);
		tr.add_node("Descr", "Descriptions");
		tr.insert(subtr, "Descr");
		subtr.cleaner();

		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;
		
		// до тех пор пока встречаем инт или флоат, продолжаем создавать поддеревья для перечисления переменных
		while (curr_word == "int" || curr_word== "float") {
			subtr = tree("Descr");
			Descr(subtr);
			tr.add_node("Descr", "Descriptions");
			tr.insert(subtr, "Descr");
			subtr.cleaner();
		}
	}
	void Descr(tree&tr) {
		tr.add_node("Type", "Descr");

		Type(tr); // дескр образовывает тип, вызываем тип
		
		tree subtr = tree("VarList");
		tr.add_node("VarList", "Descr");
		VarList(subtr); // вызываем варлист
		tr.insert(subtr, "VarList");
		subtr.cleaner();

		if (curr_word != ";") { // проверяем кончается ли перечисление переменных точкой с запятой
		//	tr.add_node(";", "Id");
		//}
		//else {
			mistakes_num++;
			mistakes.push_back("';' expected");
		}
	}
	void VarList(tree&tr) {
		tr.add_node("Id", "VarList"); // варлист образовывает ид
		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;
		Id(tr); // вызываем ид, проверяем является ли слово названием переменной
		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;
		while (curr_word == ",") { // до тех пор пока идет перечисление, пока встречаем запятые, вызываем айди
			curr_word = la.Get_Word(pos);
			q_str = la.Bypass(curr_word, q_str.first);
			pos++;
			Id(tr);
			curr_word = la.Get_Word(pos);
			q_str = la.Bypass(curr_word, q_str.first);
			pos++;
		}
	}
	void Id(tree &tr) {
		if (curr_word != "out of range") {
			if (q_str.second == "Id") // если после вызова лексического анализатора получаем сто слово айди, то добавляем узел
				tr.add_node(curr_word, "Id");
			else
			{
				mistakes_num++;
				mistakes.push_back("Id expected");
			}
		}
		else {
			mistakes_num++;
			mistakes.push_back("Out of range");
		}
	}
	void Operators(tree& tr) { // операторс -- операции 
		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;
		
		tree subtr("Op"); // создаем поддерево оп и вызываем для него функцию оп
		Op(subtr);
		tr.add_node("Op", "Operators");
		tr.insert(subtr, "Op");
		subtr.cleaner();

		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;

		// до тех пор пока не встречам слово ретерн либо пока не вышли за пределы текста, создаем поддеревья оп
		while (curr_word != "return" && curr_word != "out of range") {
			subtr = tree("Op");
			Op(subtr);
			tr.add_node("Op", "Operators");
			tr.insert(subtr, "Op");
			subtr.cleaner();
			curr_word = la.Get_Word(pos);
			q_str = la.Bypass(curr_word, q_str.first);
			pos++;
		}
	}
	void Op(tree& tr) {
		tr.add_node("Id", "Op");
		Id(tr); // вызываем айди
		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;
		if (curr_word == "=") { // дальше должен идти пробел, иначе ошибка, потом выражение
			tr.add_node("=", "Op");
			tree expr_tree("Expr");
			Expr(expr_tree); // создаем поддерево экспр и вызываем для него функцию экспр
			tr.add_node("Expr", "Op");
			tr.insert(expr_tree, "Expr");
			expr_tree.cleaner();
		}
		else {
			mistakes_num++;
			mistakes.push_back("'=' expected");
		}
	}
	void Expr(tree& tr) {
		tree subtr("Term"); // создаем поддерево терм
		Term(subtr);
		tr.add_node("Term", "Expr"); // и добавляем его под экспр
		tr.insert(subtr, "Term");
		subtr.cleaner();
		// обрабатываем операции либо конец операции (;)
		if (curr_word == "+") { // от экспр может быть образовано терм + экспр и терм - экспр
			tr.add_node("+", "Expr");
			subtr = tree("Expr");
			Expr(subtr);

			tr.add_node("Expr", "Expr"); // создаем и добавляем поддерево экспр к экспр
			tr.insert(subtr, "Expr");

			subtr.cleaner();
		}
		else if (curr_word == "-") {
			tr.add_node("-", "Expr");
			subtr = tree("Expr");
			Expr(subtr);

			tr.add_node("Expr", "Expr");
			tr.insert(subtr, "Expr");

			subtr.cleaner();
		}
		//else if (curr_word == ";") {
			//tr.add_node(";", "Expr");
		//}
		else {
			mistakes_num++;
			mistakes.push_back("'+', '-' or ';' expected");
		}
	}
	void Term(tree &tr) {
		tree subtr("SimpleExpr"); // создаем поддерево симплэкспр
		SimpleExpr(subtr);
		tr.add_node("SimpleExpr", "Term");
		tr.insert(subtr, "SimpleExpr");
		subtr.cleaner();

		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;

		// обрабатываем операции *, /, %, вызываем для них терм
		// так же как с экспр может быть образовано симпл экспр * терм, поэтому создаем поддерево терм под терм
		if (curr_word == "*") {
			tr.add_node("*", "Term");
			tree subtr("Term");
			Term(subtr);
			
			tr.add_node("Term", "Term");
			tr.insert(subtr, "Term");
			
			subtr.cleaner();
		}
		else if (curr_word == "/") {
			tr.add_node("/", "Term");
			tree subtr("Term");
			Term(subtr);

			tr.add_node("Term", "Term");
			tr.insert(subtr, "Term");

			subtr.cleaner();
		}
		else if (curr_word == "%") {
			tr.add_node("%", "Term");
			tree subtr("Term");
			Term(subtr);

			tr.add_node("Term", "Term");
			tr.insert(subtr, "Term");

			subtr.cleaner();
		}
	}
	void SimpleExpr(tree& tr) { // симплэкспр это либо ( экспр ), либо конст, либо ид
		curr_word = la.Get_Word(pos);
		if (isalpha(curr_word[0])) {  // айди
			tr.add_node("Id", "SimpleExpr");
			curr_word = la.Get_Word(pos);
			q_str = la.Bypass(curr_word, q_str.first);
			pos++;
			Id(tr);
		}
		else if (isdigit(curr_word[0])) { // конст
			tr.add_node("Const", "SimpleExpr");
			Const(tr);
		}
		else if (curr_word == "(") { // открытая скобочка
			tr.add_node("(", "SimpleExpr");
			tree expr_tree("Expr"); // создаем и вызываем экспр, вставляем в дерево
			curr_word = la.Get_Word(pos);
			q_str = la.Bypass(curr_word, q_str.first);
			pos++;
			Expr(expr_tree);
			tr.add_node("Expr", "SimpleExpr");
			tr.insert(expr_tree, "Expr");
			
			expr_tree.cleaner();

			if (curr_word != ")") { // если скобочка после этого закрывается, добавялем в дерево, если нет -- ошибка
				mistakes_num++;
				mistakes.push_back("')' expected");
			}
			else {
				tr.add_node(")", "SimpleExpr");
			}
		}
	}
	void Const(tree &tr) {
		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;
		if (curr_word != "out of range") {
			if (q_str.second == "float") { // если после лексического анализа получаем флоат, то добавляем узел "даблнамбер" к конст, и цифру под "даблнамбер"
				tr.add_node("DoubleNumber", "Const");
				tr.add_node(curr_word, "DoubleNumber");
			}
			else if (q_str.second == "int") // то же самое для инт
			{
				tr.add_node("IntNumber", "Const");
				tr.add_node(curr_word, "IntNumber");
			}
			else {
				mistakes_num++;
				mistakes.push_back("Const expected");
			}
		}
		else {
			mistakes_num++;
			mistakes.push_back("Out of range");
		}
	}
	void End() {
		// энд это ретерн айди ; }
		if (curr_word == "return") {
			temp_tree.add_node("return", "End");
			temp_tree.add_node("Id", "End");
			curr_word = la.Get_Word(pos);
			q_str = la.Bypass(curr_word, q_str.first);
			pos++;
			Id(temp_tree);
			curr_word = la.Get_Word(pos);
			q_str = la.Bypass(curr_word, q_str.first);
			pos++;
			if (curr_word == ";") {
				temp_tree.add_node(";", "End");
				curr_word = la.Get_Word(pos);
				q_str = la.Bypass(curr_word, q_str.first);
				pos++;
				if (curr_word == "}") {
					temp_tree.add_node("}", "End");
				}
				else {
					mistakes_num++;
					mistakes.push_back("';' expected");
				}
			}
			else {
				mistakes_num++;
				mistakes.push_back("';' expected");
			}
		}
		else {
			mistakes_num++;
			mistakes.push_back("Return expected");
		}
	}
	void Print_Tree(ofstream& fout2) {
		parser_tree.print(fout2);
	};
	tree get_tree() {
		return parser_tree;
	};
	hash_table get_ht() {
		return la.get_ht();
	}
	friend ostream& operator <<(ostream& out, const Parser& p) {
		out << p.la;
		out << "Mistakes amount: " << p.mistakes_num << "\n\n";
		for (int i = 0; i < p.mistakes.size(); i++)
			out << p.mistakes[i] << '\n';
		return out;
	};
};