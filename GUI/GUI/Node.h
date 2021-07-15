#pragma once
#include <string>
#include<map>
#include <vector>
using namespace std;

map<char, int> frequency;
class Node
{
public:
	Node* parent;
	Node* left;
	Node* right;
	char key;
	int freq;
	string name;
	Node(Node* p, Node* l, Node* r, int k, int f)
	{
		parent = p;
		left = l;
		right = r;
		key = k;
		freq = f;
	}
};
