#include <iostream>
using namespace std;

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