#include<iostream>
#include<vector>
#include"LexicalAnalysis.h"
#include "SyntaxAnalysis.hpp"
extern map<string, Symple> N_SYMPLE_SET;

int main() {
	using namespace std;
	Lex::init();
	Syntax::init();
	createSelect();
	extern map<string, map<Symple, vector<Symple>>> predict_table;
	for (auto it = predict_table.begin();it != predict_table.end();it++) {
		string input =(*it).first;
		map<Symple, vector<Symple>> st= (*it).second;
		cout << "输入:"<<input << "替换:";
		for (auto it = st.begin();it != st.end();it++) {
			cout << (*it).first.name<<"->";
			for (Symple sig:(*it).second) {
				cout << sig.name << " ";
			}
			cout << "     ";
		}
		cout << endl;
	}
	// 创建词法分析器
	LexAna le = LexAna("C:\\Users\\王正\\Desktop\\Message\\program.c");
	// 创建语法分析起
	Syntax::PDA pda;
	while (true)
	{
		try
		{
			vector<Token*> ts = le.scanner();
			if (ts.size() == 0) {
				break;
			}
			else {
				pda.parser(ts);
			}
		}
		catch (const char * s)
		{
			cout << s<<endl;
			break;
		}
		cout << "------------------"<<endl;
		
	}
	return 0;
}