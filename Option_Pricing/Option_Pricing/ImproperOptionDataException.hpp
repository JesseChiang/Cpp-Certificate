#ifndef ImproperOptionDataException_HPP
#define ImproperOptionDataException_HPP

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class ImproperOptionDataException {
public:
	ImproperOptionDataException()  {};
	ImproperOptionDataException(int index) {};
	~ImproperOptionDataException() {};
	string GetMessage() const;

};
inline string ImproperOptionDataException::GetMessage() const {
	stringstream stream;
	stream << "Error: improper option data!";
	return stream.str();
}

#endif
