#ifndef SMARTARRAY_H
#define SMARTARRAY_H

#include <iostream>
#include <stdexcept>

#define INITIAL_CAP		16u
#define GROWTH_FACTOR	2u


template <typename T>
class SmartArray {
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
	SmartArray();
	SmartArray(unsigned size);

	unsigned size()		{ return len; }
	unsigned capacity()	{ return cap; }
	bool isEmpty()		{ return len == 0; }

	T at(unsigned index);
	T operator[](unsigned index);

	void reserve(unsigned newCap);
	void resize(unsigned size);
	void resize(unsigned size, T elem);

	void clear();

	void pushBack(T elem);
	void insert(unsigned index, T elem);

	void erase(unsigned index);
	void erase(unsigned index, unsigned amount);
	void popBack();

};


template <typename T>
SmartArray<T>::SmartArray() {
	ar = new T[INITIAL_CAP];
	cap = INITIAL_CAP;
	len = 0;
}


template <typename T>
SmartArray<T>::SmartArray(unsigned size) {
	ar = new T[size];
	cap = size;
	len = 0;
}


template <typename T>
T SmartArray<T>::operator[](unsigned index) {
	return ar[index];
}


template <typename T>
T SmartArray<T>::at(unsigned index) {
	if (index >= len)
		throw std::out_of_range("index out of range");
	return ar[index];
}


template <typename T>
void SmartArray<T>::reserve(unsigned newCap) {
	if (newCap > cap)
		grow(newCap);
}


template <typename T>
void SmartArray<T>::resize(unsigned size) {
	if (size > cap)
		grow(size);
	for (unsigned i = len; i < size; i++)
		ar[i] = T{};
	for (unsigned i = size; i < len; i++)
		ar[i].~T();
	len = size;
}


template <typename T>
void SmartArray<T>::resize(unsigned size, T elem) {
	if (size > cap)
		grow(size);
	for (unsigned i = len; i < size; i++)
		ar[i] = elem;
	for (unsigned i = size; i < len; i++)
		ar[i].~T();
	len = size;
}


template <typename T>
void SmartArray<T>::clear() {
	for (unsigned i = 0; i < len; i++)
		ar[i].~T();
	len = 0;
}


template <typename T>
void SmartArray<T>::pushBack(T elem) {
	if (len >= cap)
		grow(std::max(INITIAL_CAP, cap * GROWTH_FACTOR));

	ar[len++] = elem;
}


template <typename T>
void SmartArray<T>::insert(unsigned index, T elem) {
	if (index > len)
		throw std::out_of_range("index out of range");

	if (len >= cap)
		grow(std::max(INITIAL_CAP, cap * GROWTH_FACTOR));

	std::move_backward(ar+index, ar+len, ar+len+1);

	ar[index] = elem;
	len++;
}


template <typename T>
void SmartArray<T>::erase(unsigned index) {
	if (index >= len)
		throw std::out_of_range("index out of range");
	std::move(ar+index+1, ar+len, ar+index);
	ar[--len].~T();
}


template <typename T>
void SmartArray<T>::erase(unsigned index, unsigned amount) {
	if (index > len || amount > len - index)
		throw std::out_of_range("index out of range");
	std::move(ar+index+amount, ar+len, ar+index);
	for (unsigned i = len - amount; i < len; i++)
		ar[i].~T();
	len -= amount;
}


template <typename T>
void SmartArray<T>::popBack() {
	if (len == 0)
		throw std::out_of_range("vector is empty");
	ar[--len].~T();
}

#endif
