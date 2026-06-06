#ifndef VECTOR_H_
#define VECTOR_H_

#include <iostream>
#include <stdexcept>

#define INITIAL_CAP		16u
#define GROWTH_FACTOR	2u


template <typename T>
class Vector {
private:
	T *ar;
	unsigned cap;
	unsigned len;

public:
	Vector();

	unsigned size()		{ return len; }
	unsigned capacity()	{ return cap; }
	bool isEmpty()		{ return len == 0; }

	T at(unsigned index);

	void reserve(unsigned newCap);

	void pushBack(T elem);
	void insert(unsigned index, T elem);

};


template <typename T>
Vector<T>::Vector() {
	ar = new T[INITIAL_CAP];
	cap = INITIAL_CAP;
	len = 0;
}


template <typename T>
T Vector<T>::at(unsigned index) {
	if (index >= len)
		throw std::out_of_range("index out of range");
	return ar[index];
}


template <typename T>
void Vector<T>::reserve(unsigned newCap) {
	if (newCap <= cap)
		return;

	T *newAr = new T[newCap];
	std::move(ar, ar+len, newAr);

	delete[] ar;

	ar = newAr;
	cap = newCap;
}


template <typename T>
void Vector<T>::pushBack(T elem) {
	if (len >= cap)
		reserve(std::max(INITIAL_CAP, cap * GROWTH_FACTOR));

	ar[len++] = elem;
}


template <typename T>
void Vector<T>::insert(unsigned index, T elem) {
	if (index > len)
		throw std::out_of_range("index out of range");

	if (len >= cap)
		reserve(std::max(INITIAL_CAP, cap * GROWTH_FACTOR));

	std::move(ar+index, ar+len, ar+index+1);

	ar[index] = elem;
	len++;
}

#endif
