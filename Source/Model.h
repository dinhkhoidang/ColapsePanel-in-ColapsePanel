#include <iostream>
#include <vector>
#include <string>  //for std::string

using std::string;
using namespace std;

class Person {

public:
	Person(string name1, string birth1, string death1, string info1);
	string getName();
	string getInfo();
	string getDeath();
	string getBirth();
	void setName(string na);
	void setInfo(string in);
	void setBirth(string bi);
	void setDeath(string de);
private: string name, info,birth, death;
		
};
