#pragma once
#include<string>
#include<vector>
class node {
public:
	string data;
	vector<node*> children;
	//конструкторы
	node() : data(""), children(std::vector<node*>()) {};
	node(std::string s) : data(s), children(std::vector<node*>()) {};
	node(const node& node) : data(node.data), children(node.children) {};
	//добавление в вектор
	void add_to_children(string s, node*& elem) {
		node temp(s);
		auto pointer = &elem->children;
		pointer->push_back(new node(temp));
	}
};
class tree {
private:
	node* root;
	//добавление узла
	void add_node(string, string, node*&);
	node* find_node(std::string info, node*&);
	//печать дерева
	void print(ofstream& fout, node*, int);
	void print_on_screen(node* node, int blank_space);
	void insert(tree& tree1, std::string ins_pos, node*& cur_pos);

public:
	tree() { root = new node(node("FUNCTION")); }
	tree(string s) { root = new node(node(s)); }
	void add_node(std::string info, std::string key) { add_node(info, key, root); }
	node* find_node(std::string info) { find_node(info, root); }
	void insert(tree& tree1, std::string ins_pos) { insert(tree1, ins_pos, root); };
	void print(std::ofstream& fout) { print(fout, root, 0); }
	void replicate(node*& first, node*& second);
	void cleaner() { root->children.clear(); }
	void print_on_screen() { print_on_screen(root, 0); }
	friend class Semantic;

};
void tree::replicate(node*& first, node*& second) {
	second = new node(*first);
	for (size_t i = 0; i < first->children.size(); i++)
		replicate(first->children[i], second->children[i]);
}
void tree::insert(tree& tree1, std::string add_pos, node*& cur_pos) {
	if (!cur_pos) 
		return;
	if (cur_pos->children.size() == 0) {
		if (cur_pos->data == add_pos) {
			cur_pos = nullptr;
			replicate(tree1.root, cur_pos);
			return;
		}
	}
	for (auto& elem : cur_pos->children)
		insert(tree1, add_pos, elem);
}
void tree::add_node(string info, string name, node*& node) {
	if (node->data == name)
		node->add_to_children(info, node);
	else
		for (size_t i = 0; i < node->children.size(); i++)
			add_node(info, name, node->children[i]);
}
//bool ifNodeExists(string key, node*& node)
//{

//}
node* tree::find_node(string info, node*& node) {
	if (node->data == info)
		return node;
}
void tree::print_on_screen(node* node, int blank_space) {
	for (int i = 0; i < blank_space; i++)
		cout << "-----|";
	cout << node->data << '\n';
	for (auto& elem : node->children)
		print_on_screen(elem, blank_space + 1);
};
void tree::print(ofstream& fout, node* node, int blank_space) {
	for (int i = 0; i < blank_space; i++)
		fout << "-----|";
	fout << node->data << '\n';
	for (auto& elem : node->children)
		print(fout, elem, blank_space + 1);
};
