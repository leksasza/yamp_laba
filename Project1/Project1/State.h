#pragma once
#include<vector>
#include<string>
using namespace std;
class State {
	friend class lexical_analysis;
	friend class dfa;
private:
	vector<pair<int, string>>vec; // состояние состоит из пары куда мы можем перейти по этому состоянию и по чему
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
