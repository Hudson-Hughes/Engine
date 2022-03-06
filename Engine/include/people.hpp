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
	int b;
	int a;
};

struct hambly {
	int a;
	int b;
	// map<string, double> c = { {"CPU", 10}, {"GPU", 15}, {"RAM", 20}, };
	int c;
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(hambly, a, b, c);
};

struct trevor {
	int a;
	int b;
	int m;
};