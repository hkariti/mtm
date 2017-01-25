#include <iostream>
using namespace std;

/*
template <char Letter>
class A {
public:
	void print() const { cout << "a" << "\b" << Letter << endl; }
};

void f(A<'c'>& a) {
	a.print();
}

int main() {
	f(A<'c'>());
}
*/

class A {
public:
	virtual void print() const { cout << "a" << endl; }
};

class B : public A {
public:
	void print() const override { cout << "b" << endl; }
};

void f(A& a) {
	a.print();
}

int main() {
	B b;
	f(b);
}
