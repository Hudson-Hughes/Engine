#define min(x,y) (((x) < (y)) ? (x) : (y))


#include <iostream>

using namespace std;

double min(double x, double y){
	return x < y ? x : y ;
}

int runBlas(){

double a = 34234.39082344;
double b = 12133231.3402;

double c = min(a, b);

return int(c);

}

int main()
{
	runBlas();
        return 0;
}