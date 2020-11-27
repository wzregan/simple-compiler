#pragma once
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <set>
#include<algorithm>
#define  T_SYMPLE 2001
#define  N_SYMPLE 2002
#define	 E 2003
#define START_SYMPLE "开始"
using namespace std;
class Symple {
public:
	string name;
	int ID;
	bool empty;
	bool start = false;
public:
	Symple() {

	}
	Symple(string n,int i):name(n),ID(i),empty(false){
		if (n=="$") {
			empty = true;
		}
	}
	bool operator<(const Symple& d) const {
		return name < d.name;
	}
	bool operator==(const Symple& d) const {
		return name == d.name;
	}
	bool operator!=(const Symple& d) const {
		return name!=d.name;
	}

	

private:


};

class derivation {
public:
	Symple left;
	vector<vector<Symple>> right;
	derivation() {

	}
	derivation(pair<vector<Symple>, vector<int>> p):left(p.first.front().name, p.first.front().ID){
		vector<Symple> & temp_list = p.first;
		vector<int> splitindex = p.second;

		int start = 1;
		int index = 0;
		for (int i = 0;i < splitindex.size();i++) {
			index= splitindex[i];
			vector<Symple>::iterator start_it = temp_list.begin() + start;
			vector<Symple>::iterator end_it = temp_list.begin()+index;
			vector<Symple> res1 = vector<Symple>(start_it, end_it);
			start = index;
			right.push_back(res1);
			//A ( B )  C  void
		}
		vector<Symple>::iterator start_it = temp_list.begin() + start;
		vector<Symple>::iterator end_it = temp_list.end();
		vector<Symple> res1 = vector<Symple>(start_it, end_it);
		right.push_back(res1);
	}
public:
	bool Right_contain(Symple symple) {
		for (vector<Symple> symples:right) {
			for (Symple s : symples) {
				if (s==symple) {
					return true;
				}
			}
		}
		return false;

	}
	
};


map<string,Symple> N_SYMPLE_SET;
map<Symple,derivation> der;




namespace Syntax {

	pair<vector<Symple>,vector<int>> parser(string line) {
		int cursor = 0;
		int state = 0;
		string token = "";
		vector<int> res;
		vector<Symple> res_list;
		bool head=false;
		while (cursor<line.size())
		{
			if (line[cursor]==':') {
				cursor++;
				continue;
			}
			if (state == 0 && line[cursor]=='<' ) {
				state = 1;
			}
			else if (state==0 && line[cursor]!='<' && line[cursor] != '|') {
				token += line[cursor];
				state = 2;
			}
			else if (state ==1&&line[cursor]!='>') {
				token += line[cursor];
			}
			else if (state ==1 && line[cursor]=='>') {
				res_list.push_back(Symple(token, N_SYMPLE));
				cout<<"非终结符：" << token<<endl;
				state = 0;
				token = "";
			}
			else if (state == 2 && line[cursor]!=' ' && line[cursor] != '|' && line[cursor] != '<') {
				token += line[cursor];
			}
			else if(state == 2 &&!(line[cursor] != ' ' && line[cursor] != '|' && line[cursor] != '<')){
				res_list.push_back(Symple(token, T_SYMPLE));
				cout << "终结符：" << token << endl;
				state = 0;
				token = "";
				if (line[cursor] == '<'|| line[cursor] == '|') cursor--;
			}else if (state==0 && line[cursor]=='|') {
				res.push_back(res_list.size());
			}
			cursor++;
		}
		if (token.size()>0&&line[cursor-1]!='>') {
			res_list.push_back(Symple(token, T_SYMPLE));
			cout << "终结符：" << token << endl;
		}
		else if(token.size()>0) {
			res_list.push_back(Symple(token, N_SYMPLE));
			cout << "非终结符：" << token << endl;
		}
		return pair<vector<Symple>,vector<int>>(res_list,res);
	}
	
	void init() {
		ifstream in("C:\\Users\\王正\\Desktop\\Message\\简单文法.txt");
		string line;
		while (getline(in, line))
		{
			pair<vector<Symple>, vector<int>> res= Syntax::parser(line);
			vector<Symple> l1= res.first;
			for (vector<Symple>::iterator i = l1.begin();i != l1.end();i++) {
				Symple s = *i;
				if (s.ID==N_SYMPLE) {
					N_SYMPLE_SET[s.name]=s;
				}
			}
			derivation d = derivation(res);
			der[d.left] = d;
		}
		cout << "一共有非终结符:" << N_SYMPLE_SET.size()<<endl;
	}


}

//函数：求First集
map<Symple, list<Symple>> ALL_FIRST;
list<Symple> First(Symple symple) {
	if (symple.ID==T_SYMPLE) {
		list<Symple> res = list<Symple>();
		res.push_back(symple);
		return res;
	}
	if (ALL_FIRST.count(symple)>0) {
		return ALL_FIRST[symple];
	}
	derivation d = der[symple];
	vector<vector<Symple>> r= d.right;
	list<Symple> first_unin;
	bool flag = false;
	for (vector<Symple> vec :r) {
		int cursor = 0;
		while (cursor<vec.size())
		{
			Symple s = vec[cursor++];
			if (s.ID == T_SYMPLE ) {
				first_unin.push_back(s);
				flag = false;
				break;
			}
			else if (s.ID == N_SYMPLE) {
				list<Symple> s_fir = First(s);
				list<Symple>::iterator it = find(s_fir.begin(), s_fir.end(), Symple("$", T_SYMPLE));
				if (it == s_fir.end()) {
					flag = false;
					first_unin.splice(first_unin.begin(), s_fir);
					break;
				}
				else {
					flag = true;
					s_fir.remove(Symple("$", T_SYMPLE));
					first_unin.splice(first_unin.begin(), s_fir);
				}
			}
		}
		if (cursor==vec.size() && flag==true) {
			first_unin.push_back(Symple("$", T_SYMPLE));
		}
	}
	first_unin.unique();
	ALL_FIRST[symple] = first_unin;
	return first_unin;
}


map<Symple, list<Symple>> ALL_Follow;

//函数求Follow集
list<Symple> Follow(Symple symple_) {
	if (ALL_Follow.count(symple_)>0) {
		return ALL_Follow[symple_];
	}
	list<Symple> follow_unin;
	if (symple_.start == true) {
		follow_unin.push_back(Symple("#",T_SYMPLE));
	}
	map<Symple, derivation>::iterator it_start=der.begin();
	map<Symple, derivation>::iterator it_end = der.end();
	vector<derivation> target;
	for (;it_start != it_end;it_start++) {
		derivation d = (*it_start).second;
		if (d.Right_contain(symple_)) {
			target.push_back(d);
		}
	}
	for (derivation d : target) {
		vector<vector<Symple>> r = d.right;
		for (vector<Symple> vec : r) {
			for (int i = 0;i < vec.size();i++) {
				Symple symple = vec[i];
				if (symple.ID == T_SYMPLE && symple != symple_) {
					continue;
				}
				else if (symple == symple_) {
					if (i + 1 == vec.size() && d.left!=symple) {
						list<Symple> list_symple = Follow(d.left);
						list_symple.remove(Symple("$", T_SYMPLE));
						follow_unin.splice(follow_unin.begin(), list_symple);
					}
					else if (i + 1 < vec.size() && vec[i + 1].ID == T_SYMPLE) {
						follow_unin.push_back(vec[i+1]);
					}
					else if (i + 1 < vec.size() && vec[i + 1].ID == N_SYMPLE) {
						list<Symple> temp = First(vec[i + 1]);
						auto it = find(temp.begin(),temp.end(),Symple("$",T_SYMPLE));
						if (it==temp.end()) {
							follow_unin.splice(follow_unin.begin(), temp);
						}
						else{
							temp.remove(Symple("$", T_SYMPLE));
							follow_unin.splice(follow_unin.begin(), temp);
							list<Symple> list_symple = Follow(d.left);
							follow_unin.splice(follow_unin.begin(), list_symple);
						}
					}
				}
			}
		}
	}
	if (symple_.name==START_SYMPLE) {
		follow_unin.push_back(Symple("#",T_SYMPLE));
	}
	follow_unin.sort();
	follow_unin.unique();
	ALL_Follow[symple_] = follow_unin;
	return follow_unin;
}

bool contain_NULLSymle(list<Symple> &fi) {
	auto it = find(fi.begin(),fi.end(), Symple("$", T_SYMPLE));
	return it != fi.end();
}

// 创建Select集合
map<Symple, vector<Symple>> select;
map<string, map<Symple, vector<Symple>>> predict_table;

// 创建预测分析表
void CreatePredictiveAnalysisTable(list<Symple> input, Symple nsym, vector<Symple> predict) {
	for (Symple i : input) {
		map < Symple, vector<Symple>> p2 = predict_table[i.name];
		p2[nsym] = predict;
		predict_table[i.name] = p2;
	}
}

void createSelect() {
	auto it = der.begin();
	auto it_e = der.end();
	for (;it != it_e;it++) {
		derivation d= (*it).second;
		vector<vector<Symple>> s = d.right;
		list<Symple> sub_select;
		for (vector<Symple> symples:s) {
			for (Symple s_:symples) {
				if (contain_NULLSymle(sub_select)) {
					sub_select.remove(Symple("$", T_SYMPLE));
				}
				list<Symple> fi = First(s_);
				sub_select.splice(sub_select.begin(), fi);
				if (!contain_NULLSymle(fi)) {
					break;
				}
			}
			if (contain_NULLSymle(sub_select)) {
				list<Symple> fi = Follow(d.left);
				sub_select.splice(sub_select.begin(), fi);
				sub_select.remove(Symple("$", T_SYMPLE));
			}
			CreatePredictiveAnalysisTable(sub_select,d.left,symples);
			sub_select.clear();
		}
	}
}
#include <stack>
namespace Syntax{
	class PDA {
		stack<Symple> H;
		deque<Token> input;
	public:
		PDA() {
			H.push(Symple("#", T_SYMPLE));
			H.push(Symple("开始", N_SYMPLE));

		}
	public:
		void parser(vector<Token*> line) {
			try
			{
				for (Token* t : line) {
					input.push_back(*t);
				}
				while (input.size()>0)
				{
					Symple s = H.top();
					if (s.ID == N_SYMPLE) {
						s1(s);
					}
					else {
						s2(s);
					}
				}
			}
			catch (const const char *s)
			{
				throw s;
			}
			cout << "语法正确" << endl;
		}
	private:
		// 处理终结符号
		void s1(Symple s) {
			Token t= input.front();
			string token =t.token;
			if (token == "#") {
				input.pop_front();
				return;
			}
			if (predict_table.count(token)==0) {
				cout << token << ",";
				throw "输入未知符号";
			}
			else {
				map<Symple,vector<Symple>> mvs= predict_table[token];
				if (mvs.count(s)==0) {
					cout << token << "不应该被输入" <<"，应该输入"<<s.name <<endl;
					throw "语法错误!";
				}
				H.pop();
				vector<Symple> vs = mvs[s];
				for (int i = vs.size() - 1;i >=0;i--) {
					if (vs[i].name!="$") {
						H.push(vs[i]);
					}
				}
			}
		}
		//  处理非终结符号
		void s2(Symple s) {
			Token t = input.front();
			string token = t.token;
			if (token == "#") {
				input.pop_front();
				return;
			}
			if (s.name == token) {
				input.pop_front();
				H.pop();
			}
			else {
				cout << "应该输入:" << s.name << ",但是您确输入" << token << endl;
				throw "语法错误！";
			}
		}
	};
}

