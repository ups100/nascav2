#include <iostream>
using namespace std;

#include "include/FactoryRegistrator.h"

struct Namer
{
static	const char *getName() { return "zajebiaszczo"; }
};

class Base{};

struct Factory
{
static	void registerFunc(const char *name, Base* (*fun)()) { cout<<name<<endl;}
};

REGISTER_SUBCLASS(Derived, Base, Namer, Factory)
{
public:
	Derived() {}
};

int main(void)
{
return 0;
}
