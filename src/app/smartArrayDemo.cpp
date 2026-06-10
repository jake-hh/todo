#include "../smartArray/SmartArray.h"
#include <iostream>

using namespace std;


template <typename T>
void status(SmartArray<T> &v) {
	cout << "cap: " << v.capacity() << "\tsize: " << v.size() << "\tar:";

	if (v.isEmpty())
		cout << " <empty>";

	else for (unsigned i = 0; i < v.size(); i++)
		cout << " " << v[i];

	cout << endl;
}


int main() {

	// default constructor
	SmartArray<int> v;
	status(v);

	// reserve
	v.reserve(8);
	status(v);

	// pushBack
	for (int i = 1; i <= 5; i++) v.pushBack(i);
	status(v);

	// insert at middle, front, and back
	v.insert(2, 99);
	v.insert(0, 0);
	v.insert(v.size(), 50);
	status(v);

	// at and operator[]
	cout << "at(3): " << v.at(3) << "\tv[0]: " << v[0] << endl;

	// erase single
	v.erase(0);
	status(v);

	// erase range
	v.erase(1, 2);
	status(v);

	// popBack
	v.popBack();
	status(v);

	// resize grow with default value
	v.resize(6);
	status(v);

	// resize grow with explicit value
	v.resize(8, 42);
	status(v);

	// resize shrink
	v.resize(3);
	status(v);

	// clear
	v.clear();
	status(v);

	// sized constructor
	SmartArray<int> w(4);
	w.pushBack(10);
	w.pushBack(20);
	status(w);

	// double vector
	cout << "\n--- double ---" << endl;
	SmartArray<double> d;
	d.pushBack(1.1);
	d.pushBack(2.2);
	d.pushBack(3.3);
	status(d);

	return 0;
}
