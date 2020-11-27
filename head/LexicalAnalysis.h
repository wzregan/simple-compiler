#pragma once
#include <set>
#include <fstream>
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>
#include <vector>
#define END_ACCEPT 20
#define END_NO_ACCEPT 21
#define ERR 22
using namespace std;
namespace Lex {
	void init();
};
extern map<string, int> KEYWORD_TAB;
extern map<string, int> TYPE_TAB;
extern map<string, int> BASE_TAB;
extern map<string, int> Operator;
extern map<string, int> Seperator;


#define TYPE_DEFINE 1001
#define CONSTANT 1002
#define IDENTIFIER 1003
#define OPERATOR 1004
#define Separator 1005
#define KeyWord 1005


class Token
{
public:
	string token;
	int Line;
	int TYPE;
	Token(int line):Line(line){
		token = "";
	}
public:
	void append(char ch) {
		token += ch;
	}
public:
	bool finished = false;
	void finish() {
		if (finished) {
			finished = true;
			return;
		}
		cout << "Line" << Line << ":";
		if (TYPE_TAB.count(token) != 0) {
			cout << "类型->";
			TYPE = TYPE_DEFINE;
		}
		else if (isConstant(token)) {
			cout << "常量->";
			TYPE = CONSTANT;
		}
		else if (Operator.count(token) != 0) {
			cout << "操作符->";
			TYPE = OPERATOR;
		}
		else if (Seperator.count(token) != 0) {
			cout << "分隔符->";
			TYPE = Separator;
		}else if (KEYWORD_TAB.count(token)!=0) {
			cout << "关键字->";
			TYPE = Separator;

		}
		else {
			cout << "标识符->";
			TYPE = IDENTIFIER;
		}
		cout << token<<endl;
	}
public:
	bool isConstant(string str) {
		for (int i = 0; i < str.size();i++) {
			char c = str[i];
			if (c != '.' && !(c >= '0' && c <= '9')) {
				return false;
			}
		}
		return true;
	}
};


class LexAna {
private:
	ifstream in;
	int cursor = 0;
	int Line = 0;
	string Line_content;
	vector<Token*> Line_vector;
private:
	int State = 0;
	Token *token=NULL;
public:
	LexAna(string filename):in(filename),Line_content(""),Line(0){
		next_line();
	}
public:
	bool next_line() {
		if (in.eof()) {
			Line_content = "";
			return true;
		}
		cursor=0;
		Line++;
		token = new Token(Line);
		getline(in,Line_content);
		Line_content += '\n';
		return false;
	}
	vector<Token*> scanner() {
		if (Line_content=="") {
			return vector<Token*>();
		}
		token = new Token(Line);
		while (true)
		{
			if (cursor>=Line_content.size()) {
				next_line();
				vector<Token*> res = Line_vector;
				Token *t=new Token(Line);
				t->append('#');
				res.push_back(t);
				Line_vector.clear();
				return res;
			}
			char ch = Line_content[cursor++];
			switch (State)
			{
			case 0:
				S0(ch);
				break;
			case 1:
				S1(ch);
				break;
			case 2:
				S2(ch);
				break;
			case 3:
				S3(ch);
				break;
			case 4:
				S4(ch);
				break;
			case 5:
				S5(ch);
				break;
			}
			if (State !=END_ACCEPT && State!=END_NO_ACCEPT && State!=0) {
				token->append(ch);
			}else if (State == END_ACCEPT) {
				token->append(ch);
				end();
			}
			else if (State == END_NO_ACCEPT) {
				end();
				back();
			}
		}
	}
	void back() {
		cursor--;
	}
private:
	bool isNum(int ch) {
		return (ch >= 48 && ch <= 57);
	}
	bool isNoDisplayCharactor(int ch) {
		return (ch >= 0 && ch <= 32) || (ch == 127);
	}

private: //终止状态
	void end() {
		token->finish();
		Line_vector.push_back(token);
		State = 0;
		token = new Token(Line);
	}
	
private: //非终止状态
	void S0(char ch) {
		if (isalpha(ch)) {
			State = 1;
		}
		else if (isNum(ch)) {
			State = 2;
		}
		else if (ch=='-' || ch == '+') {
			State = 4;
		}
		else if (ch=='*' || ch =='/') {
			State = 5;
		}
		else if (ch==',' || ch==';'||ch=='('||ch=='{'||ch==')'||ch=='}'||ch=='['||ch==']' || ch=='=') {
			State = END_ACCEPT;
		}
	}

	//变量名
	void S1(char ch) {
		if (isalpha(ch) || isNum(ch)) {
			//State保持不变
		}
		else{
			State = END_NO_ACCEPT;
		}
	}

	//数字
	void S2(char ch) {
		if (isNum(ch)) {
			//State保持不变		
		}else if(ch=='.'){
			State = 3;
		}
		else{
			State = END_NO_ACCEPT;
		}
	}


	void S3(char ch) {
		if (isNum(ch)) {
			State = 2;
		}
		else {
			State =ERR;
		}
	}


	void S4(char ch) {
		if (isNum(ch)) {
			State = 2;
		}
		else if(ch=='='){
			State = END_ACCEPT;
		}
		else {
			State = END_NO_ACCEPT;
		}
	}

	void S5(char ch) {
		if (ch=='=') {
			State = END_ACCEPT;
		}
		else {
			State = ERR;
		}
	}
};

