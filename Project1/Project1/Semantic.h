#pragma once
#include "tree.h"
#include "parser.h"
#include "hashtable.h"
#include <sstream>
class Semantic {
private:
	tree parse;
	hash_table ht;
	vector<string>Mistakes;
	vector<string>reverse_notation;
	vector<pair<string, string>>variables;
	int mistakes_num;
	string curr_expr_type;
	string function_type;
public:
	Semantic(tree Parse, hash_table HT) {
		parse = Parse;
		ht = HT;
		function_type = "Unknown";
		curr_expr_type = "Unknown";
		begin();
		descriptions();
		operators();
		end();
	}
	void begin() {
		string first_st="";
		auto temp = parse.root->children[0];
		int i = 0;
		for (i; i < temp->children.size(); i++) {
			if (temp->children[i]->data == "Type") {
				if (temp->children[i]->children.size() > 0) {
					reverse_notation.push_back(temp->children[i]->children[0]->data + " ");
					function_type = temp->children[i]->children[0]->data;
				}
			}
			if (temp->children[i]->data == "FunctionName") {
				if (temp->children[i]->children.size() > 0) {
					if (temp->children[i]->children[0]->data=="Id" && temp->children[i]->children[0]->children.size() > 0)
						reverse_notation.push_back(temp->children[i]->children[0]->children[0] -> data + " ");
				}
			}
		}
		reverse_notation.push_back("Function ");
		reverse_notation.push_back("\n");
		//descriptions();
		//operators();
		
	}
	void descriptions() {
		auto temp = parse.root->children[1];
		if (temp->children.size() > 0) {
			for (int i = 0; i < temp->children.size(); i++) {
				descr(temp->children[i]);
				reverse_notation.push_back("\n");
			}
		}
		//operators();
	}
	void descr(node* tr) {
		int i = 0;
		string type = "";
		if (tr->children.size() > 0) {
			if (tr->children[0]->data == "Type")
				if (tr->children[0]->children.size() > 0)
					type = tr->children[0]->children[0]->data;
			if (tr->children.size()>1 && tr->children[1]->data == "VarList")
				varlist(tr->children[1], type);
		}
	}
	void varlist(node* tr, string type) {
		int i=0;
		if (tr->children.size()>0 && tr->children[0]->data == "Id")
			tr = tr->children[0];
		for (i; i < tr->children.size(); i++) {
			variables.push_back(pair<string, string>(tr->children[i]->data, type));
			reverse_notation.push_back(tr->children[i]->data + " ");
		}
		reverse_notation.push_back(type + " ");
		stringstream ss;
		ss << tr->children.size();
		string str = ss.str();
		reverse_notation.push_back(str + " ");
		reverse_notation.push_back("Descr ");
	}
	void operators() {
		auto temp = parse.root->children[2]; // operators
		if (temp->children.size() > 0) {
			for (int i = 0; i < temp->children.size(); i++) {
				op(temp->children[i]);
				reverse_notation.push_back("\n");
			}
		}
	}
	string return_type(string id) {
		for (pair<string, string> v : variables) {
			if (v.first == id)
				return v.second;
		}
		return "Error";
	}
	bool if_exists(string id) {
		for (pair<string, string> v : variables) {
			if (v.first == id)
				return true;
		}
		return false;
	}
	void op(node* tr) {
		string id1 = "Unknown";
		string type1 = "Unknown";
		if (tr->children.size() > 0 && tr->children[0]->data == "Id")
			if (tr->children[0]->children.size()>0)
				id1 = tr->children[0]->children[0]->data;
		if (!if_exists(id1)) {
			mistakes_num++;
			Mistakes.push_back("Variable " + id1 + " was not declared");
		}
		for (pair<string, string> v : variables) {
			if (v.first == id1) {
				type1 = v.second;
				break;
			}
		}
		if (tr->children.size() >= 2) {
			for (int i = 2; i < tr->children.size(); i++) {
				if (tr->children[i]->data == "Expr")
					expr(tr->children[i], type1);
			}
		}
		reverse_notation.push_back(id1 + " =");
	}
	void expr(node* tr, string type) {
		string sign = "";
		string expr_type = "Unknown";
		for (int i = 0; i < tr->children.size(); i++) {
			if (tr->children[i]->data == "Term")
				term(tr->children[i], type);
			else if (tr->children[i]->data == "Expr")
				expr(tr->children[i], type);
			else {
				int ind = ht.find_element(tr->children[i]->data);
				if (ind < ht.table.size() && ht.table[ind].value == "deviders") {
					sign = tr->children[i]->data;
					expr_type = curr_expr_type;
				}
			}
		}
		if (sign == "%" && (curr_expr_type != "int" || expr_type != "int")) {
			mistakes_num++;
			Mistakes.push_back("'%' only for integer numbers ");
		}
		reverse_notation.push_back(sign + " ");
	}
	void term(node* tr, string type) {
		string sign = "";
		string expr_type = "Unknown";
		for (int i = 0; i < tr->children.size(); i++) {
			if (tr->children[i]->data == "SimpleExpr")
				simpleexpr(tr->children[i], type);
			else if (tr->children[i]->data == "Expr")
				expr(tr->children[i], type);
			else if (tr->children[i]->data == "Term")
				term(tr->children[i], type);
			else {
				int ind = ht.find_element(tr->children[i]->data);
				if (ind < ht.table.size() && ht.table[ind].value == "deviders") {
					sign = tr->children[i]->data;
					expr_type = curr_expr_type;
				}
			}
		}
		if (sign == "%" && (curr_expr_type!="int" || expr_type != "int")) {
			mistakes_num++;
			Mistakes.push_back("'%' only for integer numbers ");
		}
		reverse_notation.push_back(sign + " ");
	}
	void simpleexpr(node* tr, string type) {
		if (tr->children.size() > 0) {
			if (tr->children[0]->data == "Id") {
				string id = tr->children[0]->children[0]->data;
				reverse_notation.push_back(id + " ");
				if (return_type(id)!=type) {
					mistakes_num++;
					Mistakes.push_back("Implict type conversion, expected " + type);
				}
				if (!if_exists(id)) {
					mistakes_num++;
					Mistakes.push_back("Variable " + id + " was not declared");
				}
				curr_expr_type = return_type(id);
			}
			else if (tr->children[0]->data == "Const") {
				if (tr->children[0]->children[0]->data=="IntNumber") {
					stringstream ss;
					ss << tr->children[0]->children[0]->children[0]->data;
					string str = ss.str();
					reverse_notation.push_back(str + " ");
					if (type != "int") {
						mistakes_num++;
						Mistakes.push_back("Implict type conversion, expected " + type);
					}
					curr_expr_type = "int";
				}
				else if (tr->children[0]->children[0]->data == "DoubleNumber") {
					reverse_notation.push_back(tr->children[0]->children[0]->children[0]->data + " ");
					if (type != "float") {
						mistakes_num++;
						Mistakes.push_back("Implict type conversion, expected " + type);
					}
					curr_expr_type = "float";
				}
			}
		}
	}
	void end() {
		string id = "Unknown";
		auto temp = parse.root->children[3];
		if (temp->children.size() > 1) {
			if (temp->children[1]->children.size()>0)
				id=temp->children[1]->children[0]->data;
			reverse_notation.push_back(id + " ");
			if (return_type(id) != function_type) {
				mistakes_num++;
				Mistakes.push_back("Variable type is inconsistent with function type, expected " + function_type);
			}
			if (!if_exists(id)) {
				mistakes_num++;
				Mistakes.push_back("Variable " + id + " was not declared");
			}
			reverse_notation.push_back(temp->children[0]->data + " ");
		}
		
		reverse_notation.push_back("Function_End");
	}
	friend ostream& operator <<(ostream& out, const Semantic& s) {
		for (int i = 0; i < s.reverse_notation.size(); i++) {
			out << s.reverse_notation[i];
		}
		out << "\nMistakes amount: " << s.mistakes_num << "\n\n";
		for (int i = 0; i < s.Mistakes.size(); i++)
			out << s.Mistakes[i] << '\n';
		return out;
	};
};