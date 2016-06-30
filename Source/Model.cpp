
#include "Model.h"

	Person::Person(string name1, string birth1, string death1, string info1) {
		name = name1;
		birth = birth1;
		death = death1;
		info = info1;
	}
	string Person::getName() {
		return name;
	}
	string Person::getInfo() {
		return info;
	}
	string Person::getDeath() {
		return death;
	}
	string Person::getBirth() {
		return birth;
	}

	void Person::setName(string na)
	{
		name = na;
	}

	void Person::setInfo(string in)
	{
		info = in;
	}

	void Person::setBirth(string bi)
	{
		birth = bi;
	}

	void Person::setDeath(string de)
	{
		death = de;
	}

