#ifndef SMARTARRAY_H
#define SMARTARRAY_H

#include <iostream>
#include <memory>
#include <stdexcept>

template <typename T>
class SmartArray {
private:
	static constexpr unsigned GROWTH_FACTOR = 2;

	T *ar;
	unsigned cap;
	unsigned len;

	void grow(unsigned newCap) {
		// allocate extra space
		T *newAr = static_cast<T*>(::operator new(newCap * sizeof(T)));

		// copy data
		std::uninitialized_move(ar, ar + len, newAr);

		// destroy objects & free space
		std::destroy(ar, ar + len);
		::operator delete(ar);

		ar = newAr;
		cap = newCap;
	}

public:
	SmartArray();
	SmartArray(unsigned size);
	SmartArray(unsigned size, const T& elem);
	~SmartArray();

	unsigned size() const		{ return len; }
	unsigned capacity() const	{ return cap; }
	bool isEmpty() const		{ return len == 0; }

	T& at(unsigned index);
	const T& at(unsigned index) const;
	T& operator[](unsigned index);
	const T& operator[](unsigned index) const;

	bool operator==(const SmartArray<T>& other) const;

	void reserve(unsigned newCap);
	void resize(unsigned size);
	void resize(unsigned size, const T& elem);
	void shrinkToFit();

	void clear();

	void pushBack(T elem);
	void pushFront(T elem);
	void insert(unsigned index, T elem);

	void erase(unsigned index);
	void erase(unsigned index, unsigned amount);
	void popBack();
	void popFront();

};

template <typename T>
std::ostream& operator<<(std::ostream& os, const SmartArray<T>& a) {
	os << "[";
	for (unsigned i = 0; i < a.size(); i++) {
		if (i > 0) os << ", ";
		os << a[i];
	}
	os << "]";
	return os;
}


template <typename T>
SmartArray<T>::SmartArray() {
	// no allocation until first push
	ar = static_cast<T*>(::operator new(0));
	cap = 0;
	len = 0;
}


template <typename T>
SmartArray<T>::SmartArray(unsigned size) {
	// allocate space
	ar = static_cast<T*>(::operator new(size * sizeof(T)));
	cap = size;
	len = 0;
}


template <typename T>
SmartArray<T>::~SmartArray() {
	// destroy objects & free space
	std::destroy(ar, ar + len);
	::operator delete(ar);
}


template <typename T>
T& SmartArray<T>::operator[](unsigned index) {
	return ar[index];
}

template <typename T>
const T& SmartArray<T>::operator[](unsigned index) const {
	return ar[index];
}


template <typename T>
T& SmartArray<T>::at(unsigned index) {
	if (index >= len)
		throw std::out_of_range("index out of range");
	return ar[index];
}

template <typename T>
const T& SmartArray<T>::at(unsigned index) const {
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

	if (size > len)
		// create objects
		std::uninitialized_value_construct(ar + len, ar + size);
	else
		// destroy objects
		std::destroy(ar + size, ar + len);

	len = size;
}


template <typename T>
void SmartArray<T>::resize(unsigned size, const T& elem) {
	if (size > cap)
		grow(size);

	if (size > len)
		// create objects
		std::uninitialized_fill(ar + len, ar + size, elem);
	else
		// destroy objects
		std::destroy(ar + size, ar + len);

	len = size;
}


template <typename T>
void SmartArray<T>::clear() {
	// destroy all objects
	std::destroy(ar, ar + len);

	len = 0;
}


template <typename T>
void SmartArray<T>::pushBack(T elem) {
	if (len >= cap)
		grow(cap == 0 ? 1 : cap * GROWTH_FACTOR);

	new (ar + len) T(std::move(elem));
	len++;
}


template <typename T>
void SmartArray<T>::insert(unsigned index, T elem) {
	if (index > len)
		throw std::out_of_range("index out of range");

	if (len >= cap)
		grow(cap == 0 ? 1 : cap * GROWTH_FACTOR);

	if (index == len) {
		new (ar + len) T(std::move(elem));
	} else {
		// Move-construct the last element into the last uninitialized slot at ar[len]
		new (ar + len) T(std::move(ar[len - 1]));
		// Shift remaining elements right using move-assign (all initialized)
		std::move_backward(ar + index, ar + len - 1, ar + len);
		ar[index] = std::move(elem);
	}
	len++;
}


template <typename T>
void SmartArray<T>::erase(unsigned index) {
	if (index >= len)
		throw std::out_of_range("index out of range");

	std::move(ar + index + 1, ar + len, ar + index);

	// destroy last object
	std::destroy_at(ar + --len);
}


template <typename T>
void SmartArray<T>::erase(unsigned index, unsigned amount) {
	if (amount == 0)
		return;
	if (index > len || amount > len - index)
		throw std::out_of_range("index out of range");
	std::move(ar + index + amount, ar + len, ar + index);

	// destroy objects
	std::destroy(ar + len - amount, ar + len);

	len -= amount;
}


template <typename T>
void SmartArray<T>::popBack() {
	if (len == 0)
		throw std::out_of_range("vector is empty");

	// destroy last object
	std::destroy_at(ar + --len);
}


template <typename T>
SmartArray<T>::SmartArray(unsigned size, const T& elem) {
	// allocate space
	ar = static_cast<T*>(::operator new(size * sizeof(T)));

	cap = size;
	len = size;

	for (unsigned i = 0; i < size; i++)
		new (ar + i) T(elem);
}


template <typename T>
bool SmartArray<T>::operator==(const SmartArray<T>& other) const {
	if (len != other.len)
		return false;
	for (unsigned i = 0; i < len; i++)
		if (ar[i] != other.ar[i])
			return false;
	return true;
}


template <typename T>
void SmartArray<T>::shrinkToFit() {
	if (cap > len)
		grow(len);
}


template <typename T>
void SmartArray<T>::pushFront(T elem) {
	insert(0, std::move(elem));
}


template <typename T>
void SmartArray<T>::popFront() {
	erase(0);
}

#endif
