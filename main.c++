#include "Vector.h++"

using namespace std;


void status(Vector &v) {
	cout << "cap: " << v.reserved() << "\tsize: " << v.size() << "\t ar:";

	if (v.empty())
		cout << " <empty>";

	else for (int i = 0; i < v.size(); i++)
		cout << " " << v.at(i);

	cout << endl;
}


int main() {

	Vector v;
	v.test();
	status(v);

	v.resize(-1);
	status(v);

	for (int i = 1; i <= 20; i++) v.push(i);
	status(v);

	v.insert(-55, 4);
	status(v);

	v.insert(0, 0);
	status(v);

	v.insert(50, v.size());
	status(v);

	return 0;
}
