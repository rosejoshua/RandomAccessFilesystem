#include <myclass.h>
#include <iostream>
MyClass::MyClass(double x, double y)
{
	this->x = x;
	this->y = y;
}

double MyClass::getX() {return x;}
double MyClass::getY() {return y;}
void MyClass::setX(double x) {this->x = x;}
void MyClass::setY(double y) {this->y = y;}

void MyClass::print()
{
	std::cout << "(" << x << ", " << y << ")" << std::endl;
}
