#pragma once
#include <string>
#include<map>
#include <vector>

using namespace std;
class Tag
{
public:
	string name;
	map<string, string> attributes;
	vector<Tag*> childeren;
	string text;
	Tag* parent;

	Tag(string n, map<string, string> attr, string t)
	{
		name = n;
		attributes = attr;
		text = t;
	}
};

