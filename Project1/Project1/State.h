#pragma once
#include<vector>
#include<string>
using namespace std;
class State {
	friend class lexical_analysis;
	friend class dfa;
private:
	vector<pair<int, string>>vec; // ��������� ������� �� ���� ���� �� ����� ������� �� ����� ��������� � �� ����
public:
	State() {};
	State(vector<pair<int, string>>v) {
		vec = v;
	}
	void Add_New_State(pair<int, string>p) {
		vec.push_back(p);
	}
	vector<pair<int, string>> get_vec() {
		return vec;
	}
};