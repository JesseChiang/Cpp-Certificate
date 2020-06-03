#ifndef Option_HPP
#define Option_HPP
#include <string>
using namespace std;

// what if the type was wrong
enum class Type {
	put, call
};

class Option {
protected: 
	Type m_type;	// option type: put or call
public:
	// default constructor
	Option() { m_type = Type::call; }; // default type: call
	// overloaded constructor with arguments
	Option(const Type& type);
	// copy constructor
	Option(const Option& source);
	// distructor
	virtual ~Option() {};
	
	// assignment operator
	Option& operator = (const Option& source);

	void toggle() { m_type = (m_type == Type::call) ? Type::put : Type::call; };
	// string ToString() const;

	virtual double Price() const = 0;
};

inline Option::Option(const Type& type) {
	m_type = type;
}

inline Option::Option(const Option& source) {
	m_type = source.m_type;
}

inline Option& Option::operator = (const Option& source) {
	if (this == &source) return *this;
	m_type = source.m_type;
	return *this;
}


#endif