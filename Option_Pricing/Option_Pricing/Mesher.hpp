#ifndef Mesher_HPP
#define Mesher_HPP

#include <vector>
using namespace std;

vector<double> Mesher(double begin, double end, double h) {
	int n = (int) ((end - begin) / h) + 1; // exception
	n = (n < 0) ? 1 : n;
	vector<double> mesh(n);
	mesh[0] = begin;
	for (int i = 1; i < n; i++) { // exception
		mesh[i] = mesh[i - 1] + h;
	}
	return mesh;
}

#endif