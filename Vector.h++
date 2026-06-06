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

	void grow(unsigned newCap) {
		T *newAr = new T[newCap];
		std::move(ar, ar+len, newAr);
		delete[] ar;
		ar = newAr;
		cap = newCap;
	}

public:
	Vector();
	Vector(unsigned size);

	unsigned size()		{ return len; }
	unsigned capacity()	{ return cap; }
	bool isEmpty()		{ return len == 0; }

	T at(unsigned index);
	T operator[](unsigned index);

	void reserve(unsigned newCap);
	void resize(unsigned size);
	void resize(unsigned size, T elem);

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
Vector<T>::Vector(unsigned size) {
	ar = new T[size];
	cap = size;
	len = 0;
}


template <typename T>
T Vector<T>::operator[](unsigned index) {
	return ar[index];
}


template <typename T>
T Vector<T>::at(unsigned index) {
	if (index >= len)
		throw std::out_of_range("index out of range");
	return ar[index];
}


template <typename T>
void Vector<T>::reserve(unsigned newCap) {
	if (newCap > cap)
		grow(newCap);
}


template <typename T>
void Vector<T>::resize(unsigned size) {
	if (size > cap)
		grow(size);
	for (unsigned i = len; i < size; i++)
		ar[i] = T{};
	len = size;
}


template <typename T>
void Vector<T>::resize(unsigned size, T elem) {
	if (size > cap)
		grow(size);
	for (unsigned i = len; i < size; i++)
		ar[i] = elem;
	len = size;
}


template <typename T>
void Vector<T>::pushBack(T elem) {
	if (len >= cap)
		grow(std::max(INITIAL_CAP, cap * GROWTH_FACTOR));

	ar[len++] = elem;
}


template <typename T>
void Vector<T>::insert(unsigned index, T elem) {
	if (index > len)
		throw std::out_of_range("index out of range");

	if (len >= cap)
		grow(std::max(INITIAL_CAP, cap * GROWTH_FACTOR));

	std::move(ar+index, ar+len, ar+index+1);

	ar[index] = elem;
	len++;
}

#endif
