#pragma once
#include "tree.h"
#include <string>
#include "lexicalanalysis.h"
#include<stack>
using namespace std;
class Parser
{
private:
	vector<string>program; // ������ ������ ����� ���������
	int pos; // ������� � ������
	int mistakes_num; // ����� ������
	tree parser_tree; // ������
	tree temp_tree;
	tree sub_tree;
	vector<string>mistakes; // ������ ������ 
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
		// �������� �����, ����������, ���������, ���
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
		temp_tree.add_node("Type", "Begin"); // ����� ������������ ���������� ����

		// ��������� �� ��������� �����. �������� ����������� ����������. �� ���������� ���� q ��������� �������� � ��� ������������ ������
		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;

		Type(temp_tree); // �������� ����
		FunctionName(); // ����� ������������ ����������
		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;
		if (curr_word == "{") { // ��������� ��������� �� �������� ���������, ��� �������������� �� ������
			temp_tree.add_node(curr_word, "Begin");
		}
		else {
			mistakes_num++;
			mistakes.push_back("'{' expected");
		}
	}
	void Type(tree& tr) {
		// ���� ����� ��� ��� �����, ��������� ��� � ����������� ����, ����� ������ ������
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
		Id(temp_tree); // �������� ������ ����
	}
	void Descriptions(tree& tr) { // ���������� (�������� ����������)
		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;

		tree subtr = tree("Descr"); // ������� ��������� ��� ������� ������������ ����������
		Descr(subtr);
		tr.add_node("Descr", "Descriptions");
		tr.insert(subtr, "Descr");
		subtr.cleaner();

		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;
		
		// �� ��� ��� ���� ��������� ��� ��� �����, ���������� ��������� ���������� ��� ������������ ����������
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

		Type(tr); // ����� ������������ ���, �������� ���
		
		tree subtr = tree("VarList");
		tr.add_node("VarList", "Descr");
		VarList(subtr); // �������� �������
		tr.insert(subtr, "VarList");
		subtr.cleaner();

		if (curr_word != ";") { // ��������� ��������� �� ������������ ���������� ������ � �������
		//	tr.add_node(";", "Id");
		//}
		//else {
			mistakes_num++;
			mistakes.push_back("';' expected");
		}
	}
	void VarList(tree&tr) {
		tr.add_node("Id", "VarList"); // ������� ������������ ��
		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;
		Id(tr); // �������� ��, ��������� �������� �� ����� ��������� ����������
		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;
		while (curr_word == ",") { // �� ��� ��� ���� ���� ������������, ���� ��������� �������, �������� ����
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
			if (q_str.second == "Id") // ���� ����� ������ ������������ ����������� �������� ��� ����� ����, �� ��������� ����
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
	void Operators(tree& tr) { // ��������� -- �������� 
		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;
		
		tree subtr("Op"); // ������� ��������� �� � �������� ��� ���� ������� ��
		Op(subtr);
		tr.add_node("Op", "Operators");
		tr.insert(subtr, "Op");
		subtr.cleaner();

		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;

		// �� ��� ��� ���� �� �������� ����� ������ ���� ���� �� ����� �� ������� ������, ������� ���������� ��
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
		Id(tr); // �������� ����
		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;
		if (curr_word == "=") { // ������ ������ ���� ������, ����� ������, ����� ���������
			tr.add_node("=", "Op");
			tree expr_tree("Expr");
			Expr(expr_tree); // ������� ��������� ����� � �������� ��� ���� ������� �����
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
		tree subtr("Term"); // ������� ��������� ����
		Term(subtr);
		tr.add_node("Term", "Expr"); // � ��������� ��� ��� �����
		tr.insert(subtr, "Term");
		subtr.cleaner();
		// ������������ �������� ���� ����� �������� (;)
		if (curr_word == "+") { // �� ����� ����� ���� ���������� ���� + ����� � ���� - �����
			tr.add_node("+", "Expr");
			subtr = tree("Expr");
			Expr(subtr);

			tr.add_node("Expr", "Expr"); // ������� � ��������� ��������� ����� � �����
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
		tree subtr("SimpleExpr"); // ������� ��������� ����������
		SimpleExpr(subtr);
		tr.add_node("SimpleExpr", "Term");
		tr.insert(subtr, "SimpleExpr");
		subtr.cleaner();

		curr_word = la.Get_Word(pos);
		q_str = la.Bypass(curr_word, q_str.first);
		pos++;

		// ������������ �������� *, /, %, �������� ��� ��� ����
		// ��� �� ��� � ����� ����� ���� ���������� ����� ����� * ����, ������� ������� ��������� ���� ��� ����
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
	void SimpleExpr(tree& tr) { // ���������� ��� ���� ( ����� ), ���� �����, ���� ��
		curr_word = la.Get_Word(pos);
		if (isalpha(curr_word[0])) {  // ����
			tr.add_node("Id", "SimpleExpr");
			curr_word = la.Get_Word(pos);
			q_str = la.Bypass(curr_word, q_str.first);
			pos++;
			Id(tr);
		}
		else if (isdigit(curr_word[0])) { // �����
			tr.add_node("Const", "SimpleExpr");
			Const(tr);
		}
		else if (curr_word == "(") { // �������� ��������
			tr.add_node("(", "SimpleExpr");
			tree expr_tree("Expr"); // ������� � �������� �����, ��������� � ������
			curr_word = la.Get_Word(pos);
			q_str = la.Bypass(curr_word, q_str.first);
			pos++;
			Expr(expr_tree);
			tr.add_node("Expr", "SimpleExpr");
			tr.insert(expr_tree, "Expr");
			
			expr_tree.cleaner();

			if (curr_word != ")") { // ���� �������� ����� ����� �����������, ��������� � ������, ���� ��� -- ������
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
			if (q_str.second == "float") { // ���� ����� ������������ ������� �������� �����, �� ��������� ���� "����������" � �����, � ����� ��� "����������"
				tr.add_node("DoubleNumber", "Const");
				tr.add_node(curr_word, "DoubleNumber");
			}
			else if (q_str.second == "int") // �� �� ����� ��� ���
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
		// ��� ��� ������ ���� ; }
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