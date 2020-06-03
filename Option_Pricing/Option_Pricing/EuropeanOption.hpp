#ifndef EuropeanOption_HPP
#define EuropeanOption_HPP
#include <vector>
#include "Option.hpp"
#include "ImproperOptionDataException.hpp"

struct EuropeanOptionData {
	double m_S;		// asset price
	double m_K;		// strike price
	double m_T;		// exercise (maturity) date
	double m_r;		// risk-free interest rate
	double m_sig;	// constant volatility
	double m_b;		// cost of carry
	EuropeanOptionData(double S, double K, double T, double r, double sig, double b) {
		if ((S <= 0.0) || (K <= 0.0) || (T <= 0.0) || (sig <= 0.0)) {
			throw ImproperOptionDataException();
		}
		m_S = S;
		m_K = K;
		m_T = T;
		m_r = r;
		m_sig = sig;
		m_b = b;
	}
};

class EuropeanOption : public Option {
private:
	EuropeanOptionData m_data;
	double price(double S, double K, double T, double r, double sig, double b, const Type& type) const;
	double delta(double S, double K, double T, double r, double sig, double b, const Type& type) const;
	double gamma(double S, double K, double T, double r, double sig, double b, const Type& type) const;
	double approx_delta(double h, double S, double K, double T, double r, double sig, double b, const Type& type) const;
	double approx_gamma(double h, double S, double K, double T, double r, double sig, double b, const Type& type) const;
public:
	EuropeanOption() : Option(), m_data(60,65,0.25,0.08,0.30,0.25) {};
	EuropeanOption(double S, double K, double T, double r, double sig, double b) : Option(), m_data(S, K, T, r, sig, b) {};
	EuropeanOption(double S, double K, double T, double r, double sig, double b, const Type& type) : Option(type), m_data(S,K,T,r,sig,b) {};
	EuropeanOption(const EuropeanOptionData& data) : Option(), m_data(data)  {};
	EuropeanOption(const EuropeanOptionData& data, const Type& type) : Option(type), m_data(data) {};
	EuropeanOption(const EuropeanOption& source): Option(source), m_data(source.m_data) {};
	virtual ~EuropeanOption() {};
	
	EuropeanOption& operator = (const EuropeanOption& source);

	double Price() const;
	vector<double> Price(const vector<double>& vec, int para) const;
	vector<vector<double>> Price(const vector<vector<double>>& mat, const vector<int>& paras) const;
	
	double Delta() const;
	double ApproxDelta(double h) const;
	vector<double> Delta(const vector<double>& vec, int para) const;
	vector<double> ApproxDelta(double h, const vector<double>& vec, int para) const;
	vector<vector<double>> Delta(const vector<vector<double>>& mat, const vector<int>& paras) const;
	vector<vector<double>> ApproxDelta(double h, const vector<vector<double>>& mat, const vector<int>& paras) const;

	double Gamma() const;
	double ApproxGamma(double h) const;
	vector<double> Gamma(const vector<double>& vec, int para) const;
	vector<double> ApproxGamma(double h, const vector<double>& vec, int para) const;
	vector<vector<double>> Gamma(const vector<vector<double>>& mat, const vector<int>& paras) const;
	vector<vector<double>> ApproxGamma(double h, const vector<vector<double>>& mat, const vector<int>& paras) const;
	
	double Vega()  const;
	
	double Theta() const;
	
	double PutCallParity(double price) const;
	bool ISPutCallParity(double price, double tol) const;
	
	//template <double (*f)(double S, double K, double T, double r, double sig, double b, const Type & type)>
	//double Value() const;

};

inline EuropeanOption& EuropeanOption::operator = (const EuropeanOption& source) {
	if (this == &source) return *this;
	Option::operator = (source);
	m_data = source.m_data;
	return *this;
}

#endif
