#include "Vector.h++"

using namespace std;


template <typename T>
void status(Vector<T> &v) {
	cout << "cap: " << v.capacity() << "\tsize: " << v.size() << "\t ar:";

	if (v.isEmpty())
		cout << " <empty>";

	else for (unsigned i = 0; i < v.size(); i++)
		cout << " " << v.at(i);

	cout << endl;
}


int main() {

	Vector<int> v;
	status(v);

	v.reserve(8);
	status(v);

	for (int i = 1; i <= 20; i++) v.pushBack(i);
	status(v);

	v.insert(4, -55);
	status(v);

	v.insert(0, 0);
	status(v);

	v.insert(v.size(), 50);
	status(v);

	cout << "\n--- double ---" << endl;
	Vector<double> d;
	d.pushBack(1.1);
	d.pushBack(2.2);
	d.pushBack(3.3);
	status(d);

	return 0;
}
