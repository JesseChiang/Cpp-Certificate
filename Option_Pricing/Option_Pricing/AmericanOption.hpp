#ifndef AmericanOption_HPP
#define AmericanOption_HPP

#include "Option.hpp"
#include "ImproperOptionDataException.hpp"
#include <vector>

using namespace std;

struct AmericanOptionData {
	double m_S;		// asset price
	double m_K;		// strike price
	double m_r;		// risk-free interest rate
	double m_sig;	// constant volatility
	double m_b;		// cost of carry
	AmericanOptionData(double S, double K, double r, double sig, double b) {
		if ((S <= 0.0) || (K <= 0.0) || (sig <= 0.0)) {
			throw ImproperOptionDataException();
		}
		m_S = S;
		m_K = K;
		m_r = r;
		m_sig = sig;
		m_b = b;
	}
};

class AmericanOption : public Option {
private:
	AmericanOptionData m_data;
	double price(double S, double K, double r, double sig, double b, const Type& type) const;
public:
	AmericanOption() : Option(), m_data(60, 65, 0.08, 0.30, 0.25) {};
	AmericanOption(double S, double K, double r, double sig, double b) : Option(), m_data(S, K, r, sig, b) {};
	AmericanOption(double S, double K, double r, double sig, double b, Type& type) : Option(type), m_data(S, K, r, sig, b) {};
	AmericanOption(const AmericanOption& source) : Option(source), m_data(source.m_data) {};
	virtual ~AmericanOption() {};

	AmericanOption& operator = (const AmericanOption& source);

	double Price() const;
	vector<double> Price(vector<double> vec, int para) const;
	vector<vector<double>> Price(vector<vector<double>> mat, vector<int> paras) const;
};

inline AmericanOption& AmericanOption::operator = (const AmericanOption& source) {
	if (this == &source) return *this;
	Option::operator = (source);
	m_data = source.m_data;
	return *this;
};

#endif