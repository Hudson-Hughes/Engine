#pragma once

#include <map>
#include <string>

using namespace std;

class Person {
	int health = 100;
	int height = 72;
public:
	float strength = 1.0f;
	string name = "Harrison";
};

class Man : public Person {
	string hairstyle = "Pomp";
};

///Component
struct Girl : public Person {
public:
	string hairstyle = "Braids";
};

struct sister {
	double c;
	string b;
	int a;
};

struct hambly {
	int a;
	string b;
	double c;
};

struct trevor {
	int a;
	string b;
	map<double, int> m;
};