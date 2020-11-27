#include "LexicalAnalysis.h"

using namespace std;
map<string,int> KEYWORD_TAB;
map<string, int> TYPE_TAB;
map<string, int> BASE_TAB;
map<string, int> Operator;
map<string, int> Seperator;


int CID = 0;
void init_type() {
	ifstream in("C:\\Users\\王正\\Desktop\\Message\\类型.txt");
	string word;
	while (getline(in, word)) {
		TYPE_TAB[word] = CID++;
	}
	in.close();
}

void init_baseword() {
	ifstream in("C:\\Users\\王正\\Desktop\\Message\\关键字.txt");
	string word;
	while (getline(in, word)) {
		BASE_TAB[word] = CID++;
	}
	in.close();
}

void init_operator() {
	ifstream in("C:\\Users\\王正\\Desktop\\Message\\运算符.txt");
	string word;
	while (getline(in, word)) {
		Operator[word] = CID++;
	}
	in.close();
}
void init_seperator() {
	ifstream in("C:\\Users\\王正\\Desktop\\Message\\分隔符.txt");
	string word;
	while (getline(in, word)) {
		Seperator[word] = CID++;
	}
	in.close();
}
void init_keyword() {
	ifstream in("C:\\Users\\王正\\Desktop\\Message\\关键字.txt");
	string word;
	while (getline(in, word)) {
		KEYWORD_TAB[word] = CID++;
	}
	in.close();

}
namespace Lex {
	void init()
	{
		init_baseword();
		init_type();
		init_seperator();
		init_operator();
		init_keyword();
	}
};
