#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>

#define INITIAL_CAP		16
#define GROWTH_FACTOR	2


class Vector {
private:
	int *ar;
	int cap;
	int len;

public:
	Vector();

	int size()			{ return len; }
	int reserved()		{ return cap; }
	bool empty()		{ return len <= 0; }

	int at(int pos);

	void resize(int newCap);

	void push(int x);
	void insert(int x, int pos);

	void test();
};


Vector::Vector() {
	ar = new int[INITIAL_CAP];
	cap = INITIAL_CAP;
	len = 0;
}


int Vector::at(int pos) {
	if (pos >= len)
		return 0;		// TODO: throw error
	else
		return ar[pos];
}


void Vector::resize(int newCap) {
	if (newCap < len)
		newCap = len;

	int *newAr = new int[newCap];
	std::move(ar, ar+len, newAr);

	delete ar;

	ar = newAr;
	cap = newCap;
}


void Vector::push(int x) {
	if (len >= cap)
		resize(std::max(INITIAL_CAP, cap * GROWTH_FACTOR));
	
	ar[len++] = x;
}


void Vector::insert(int x, int pos) {
	if (pos < 0)
		pos = 0;		// TODO: throw error
	else if (pos > len)
		pos = len;		// TODO: throw error

	if (len >= cap)
		resize(std::max(INITIAL_CAP, cap * GROWTH_FACTOR));

	std::move(ar+pos, ar+len, ar+pos+1);

	ar[pos] = x;
	len++;
}


void Vector::test() {
	std::cout << "Hello World" << std::endl;
}

#endif
