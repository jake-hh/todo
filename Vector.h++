#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>
#include <stdexcept>

#define INITIAL_CAP		16u
#define GROWTH_FACTOR	2u


class Vector {
private:
	int *ar;
	unsigned cap;
	unsigned len;

public:
	Vector();

	unsigned size()		{ return len; }
	unsigned capacity()	{ return cap; }
	bool isEmpty()		{ return len == 0; }

	int at(unsigned index);

	void reserve(unsigned newCap);

	void pushBack(int elem);
	void insert(unsigned index, int elem);

};


Vector::Vector() {
	ar = new int[INITIAL_CAP];
	cap = INITIAL_CAP;
	len = 0;
}


int Vector::at(unsigned index) {
	if (index >= len)
		throw std::out_of_range("index out of range");
	return ar[index];
}


void Vector::reserve(unsigned newCap) {
	if (newCap <= cap)
		return;

	int *newAr = new int[newCap];
	std::move(ar, ar+len, newAr);

	delete[] ar;

	ar = newAr;
	cap = newCap;
}


void Vector::pushBack(int elem) {
	if (len >= cap)
		reserve(std::max(INITIAL_CAP, cap * GROWTH_FACTOR));

	ar[len++] = elem;
}


void Vector::insert(unsigned index, int elem) {
	if (index > len)
		throw std::out_of_range("index out of range");

	if (len >= cap)
		reserve(std::max(INITIAL_CAP, cap * GROWTH_FACTOR));

	std::move(ar+index, ar+len, ar+index+1);

	ar[index] = elem;
	len++;
}

#endif
