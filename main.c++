#include "Vector.h++"

using namespace std;


void status(Vector &v) {
	cout << "cap: " << v.capacity() << "\tsize: " << v.size() << "\t ar:";

	if (v.isEmpty())
		cout << " <empty>";

	else for (unsigned i = 0; i < v.size(); i++)
		cout << " " << v.at(i);

	cout << endl;
}


int main() {

	Vector v;
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

	return 0;
}
