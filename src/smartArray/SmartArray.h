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

	void _changeCap(unsigned newCap);

public:
	/**
	 * @brief Default constructor. Creates an empty array with no allocation.
	 */
	SmartArray();
	/**
	 * @brief Reserve constructor. Allocates space for @p size elements but sets length to 0.
	 */
	SmartArray(unsigned size);
	/**
	 * @brief Fill constructor. Creates an array of @p size elements all set to @p elem.
	 */
	SmartArray(unsigned size, const T& elem);
	/**
	 * @brief Destructor. Destroys all elements and frees the allocation.
	 */
	~SmartArray();

	/**
	 * @brief Copy constructor. Deep-copies elements from @p other into a new allocation.
	 */
	SmartArray(const SmartArray& other);
	/**
	 * @brief Move constructor. Transfers ownership from @p other, leaving it empty.
	 */
	SmartArray(SmartArray&& other) noexcept;
	/**
	 * @brief Copy assignment. Replaces contents with a deep copy of @p other (copy-and-swap).
	 */
	SmartArray& operator=(const SmartArray& other);
	/**
	 * @brief Move assignment. Releases current allocation and takes ownership from @p other.
	 */
	SmartArray& operator=(SmartArray&& other) noexcept;

	/**
	 * @brief Returns the number of elements stored.
	 */
	unsigned size() const      { return len; }
	/**
	 * @brief Returns the number of elements for which memory is reserved.
	 */
	unsigned capacity() const  { return cap; }
	/**
	 * @brief Returns true if the array contains no elements.
	 */
	bool isEmpty() const       { return len == 0; }

	/**
	 * @brief Returns a pointer to the first element.
	 */
	T* begin()                 { return ar; }
	/**
	 * @brief Returns a pointer one past the last element.
	 */
	T* end()                   { return ar + len; }
	/**
	 * @brief Const overload of begin().
	 */
	const T* begin() const     { return ar; }
	/**
	 * @brief Const overload of end().
	 */
	const T* end()   const     { return ar + len; }

	/**
	 * @brief Returns a reference to the element at @p index.
	 * @throws std::out_of_range if index >= size.
	 */
	T& at(unsigned index);
	/**
	 * @brief Const overload of at().
	 * @throws std::out_of_range if index >= size.
	 */
	const T& at(unsigned index) const;
	/**
	 * @brief Returns a reference to the element at @p index. No bounds checking.
	 */
	T& operator[](unsigned index);
	/**
	 * @brief Const overload of operator[].
	 */
	const T& operator[](unsigned index) const;

	/**
	 * @brief Returns true if both arrays have the same size and equal elements.
	 */
	bool operator==(const SmartArray<T>& other) const;

	/**
	 * @brief Ensures capacity for at least @p newCap elements without changing size.
	 */
	void reserve(unsigned newCap);
	/**
	 * @brief Resizes the array to @p size, default-constructing new elements or destroying excess ones.
	 */
	void resize(unsigned size);
	/**
	 * @brief Resizes the array to @p size, filling new elements with @p elem or destroying excess ones.
	 */
	void resize(unsigned size, const T& elem);
	/**
	 * @brief Reduces capacity to match the current size.
	 */
	void shrinkToFit();

	/**
	 * @brief Appends @p elem to the end, growing capacity if needed.
	 */
	void pushBack(T elem);
	/**
	 * @brief Inserts @p elem at the front, shifting all elements right.
	 */
	void pushFront(T elem);
	/**
	 * @brief Inserts @p elem at @p index, shifting subsequent elements right.
	 * @throws std::out_of_range if index > size.
	 */
	void insert(unsigned index, T elem);

	/**
	 * @brief Removes all elements. Capacity is unchanged.
	 */
	void clear();
	/**
	 * @brief Removes the element at @p index, shifting subsequent elements left.
	 * @throws std::out_of_range if index >= size.
	 */
	void erase(unsigned index);
	/**
	 * @brief Removes @p amount elements starting at @p index.
	 * @throws std::out_of_range if index + amount > size.
	 */
	void erase(unsigned index, unsigned amount);
	/**
	 * @brief Removes the last element.
	 * @throws std::out_of_range if the array is empty.
	 */
	void popBack();
	/**
	 * @brief Removes the first element.
	 * @throws std::out_of_range if the array is empty.
	 */
	void popFront();
};


template <typename T>
void SmartArray<T>::_changeCap(unsigned newCap) {
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
SmartArray<T>::SmartArray(unsigned size, const T& elem) {
	// allocate space
	ar = static_cast<T*>(::operator new(size * sizeof(T)));

	cap = size;
	len = size;

	std::uninitialized_fill(ar, ar + size, elem);
}


template <typename T>
SmartArray<T>::SmartArray(const SmartArray& other) {
	ar = static_cast<T*>(::operator new(other.cap * sizeof(T)));
	cap = other.cap;
	len = other.len;
	std::uninitialized_copy(other.ar, other.ar + other.len, ar);
}

template <typename T>
SmartArray<T>::SmartArray(SmartArray&& other) noexcept
	: ar(other.ar), cap(other.cap), len(other.len) {
	other.ar  = nullptr;
	other.cap = 0;
	other.len = 0;
}


template <typename T>
SmartArray<T>& SmartArray<T>::operator=(const SmartArray& other) {
	if (this != &other) {
		SmartArray tmp(other);
		std::swap(ar, tmp.ar);
		std::swap(cap, tmp.cap);
		std::swap(len, tmp.len);
	}
	return *this;
}

template <typename T>
SmartArray<T>& SmartArray<T>::operator=(SmartArray&& other) noexcept {
	if (this != &other) {
		std::destroy(ar, ar + len);
		::operator delete(ar);
		ar  = other.ar;
		cap = other.cap;
		len = other.len;
		other.ar  = nullptr;
		other.cap = 0;
		other.len = 0;
	}
	return *this;
}


template <typename T>
SmartArray<T>::~SmartArray() {
	// destroy objects & free space
	std::destroy(ar, ar + len);
	::operator delete(ar);
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
T& SmartArray<T>::operator[](unsigned index) {
	return ar[index];
}

template <typename T>
const T& SmartArray<T>::operator[](unsigned index) const {
	return ar[index];
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
void SmartArray<T>::reserve(unsigned newCap) {
	if (newCap > cap)
		_changeCap(newCap);
}


template <typename T>
void SmartArray<T>::resize(unsigned size) {
	if (size > cap)
		_changeCap(size);

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
		_changeCap(size);

	if (size > len)
		// create objects
		std::uninitialized_fill(ar + len, ar + size, elem);
	else
		// destroy objects
		std::destroy(ar + size, ar + len);

	len = size;
}


template <typename T>
void SmartArray<T>::shrinkToFit() {
	if (cap > len)
		_changeCap(len);
}


template <typename T>
void SmartArray<T>::pushBack(T elem) {
	if (len >= cap)
		_changeCap(cap == 0 ? 1 : cap * GROWTH_FACTOR);

	new (ar + len) T(std::move(elem));
	len++;
}


template <typename T>
void SmartArray<T>::pushFront(T elem) {
	insert(0, std::move(elem));
}


template <typename T>
void SmartArray<T>::insert(unsigned index, T elem) {
	if (index > len)
		throw std::out_of_range("index out of range");

	if (len >= cap)
		_changeCap(cap == 0 ? 1 : cap * GROWTH_FACTOR);

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
void SmartArray<T>::clear() {
	// destroy all objects
	std::destroy(ar, ar + len);

	len = 0;
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
void SmartArray<T>::popFront() {
	erase(0);
}

#endif
