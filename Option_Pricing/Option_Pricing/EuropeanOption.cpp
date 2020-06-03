#include <cmath>
#include "EuropeanOption.hpp"
#include "ImproperOptionDataException.hpp"
#include <boost\math\distributions\normal.hpp>
#include <vector>
using namespace boost::math;

double EuropeanOption::price(double S, double K, double T, double r, double sig, double b, const Type& type) const {
	normal_distribution<> normal(0.0, 1.0);
	double id = (type == Type::call) ? 1.0 : (-1.0);
	double d1 = (log(S / K) + (b + pow(sig, 2) / 2.0) * T) / (sig * sqrt(T));
	double d2 = d1 - sig * sqrt(T);
	return id * (S * exp((b - r) * T) * cdf(normal, id * d1) - K * exp(-r * T) * cdf(normal, id * d2));
}

double EuropeanOption::Price() const {
	return price(m_data.m_S, m_data.m_K, m_data.m_T, m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
}

vector<double> EuropeanOption::Price(const vector<double>& vec, int para) const {
	vector<double> result(vec.size());
	switch (para) {
	case 0:
		for (int i = 0; i < result.size(); i++) {
			result[i] = price(vec[i], m_data.m_K, m_data.m_T, m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
			}
		break;
	case 1:
		for (int i = 0; i < result.size(); i++) {
			result[i] = price(m_data.m_S, vec[i], m_data.m_T, m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 2:
		for (int i = 0; i < result.size(); i++) {
			result[i] = price(m_data.m_S, m_data.m_K, vec[i], m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 3:
		for (int i = 0; i < result.size(); i++) {
			result[i] = price(m_data.m_S, m_data.m_K, m_data.m_T, vec[i], m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 4:
		for (int i = 0; i < result.size(); i++) {
			result[i] = price(m_data.m_S, m_data.m_K, m_data.m_T, m_data.m_r, vec[i], m_data.m_b, m_type);
		}
		break;
	case 5:
		for (int i = 0; i < result.size(); i++) {
			result[i] = price(m_data.m_S, m_data.m_K, m_data.m_T, m_data.m_r, m_data.m_sig, vec[i], m_type);
		}
		break;
	}
	return result;
}

vector<vector<double>> EuropeanOption::Price(const vector<vector<double>>& mat, const vector<int>& paras) const {
	vector<vector<double>> result;
	for (int i = 0; i  < paras.size(); i++) {
		result.push_back(Price(mat[i], paras[i]));
	}
	return result;
}

double EuropeanOption::delta(double S, double K, double T, double r, double sig, double b, const Type& type) const {
	normal_distribution<> normal(0.0, 1.0);
	double id = (type == Type::call) ? 1.0 : (-1.0);
	double d1 = (log(S / K) + (b + pow(sig, 2) / 2.0) * T) / (sig * sqrt(T));
	return id * exp((b - r) * T) * cdf(normal, id * d1);
}

double EuropeanOption::approx_delta(double h, double S, double K, double T, double r, double sig, double b, const Type& type) const {
	double V1 = price(S + h, K, T, r, sig, b, type);
	double V2 = price(S - h, K, T, r, sig, b, type);
	return (V1 - V2) / (2.0 * h);
}

double EuropeanOption::Delta() const {
	return delta(m_data.m_S, m_data.m_K, m_data.m_T, m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
}

double EuropeanOption::ApproxDelta(double h) const {
	return approx_delta(h, m_data.m_S, m_data.m_K, m_data.m_T, m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
}

vector<double> EuropeanOption::Delta(const vector<double>& vec, int para) const {
	vector<double> result(vec.size());
	switch (para) {
	case 0:
		for (int i = 0; i < result.size(); i++) {
			result[i] = delta(vec[i], m_data.m_K, m_data.m_T, m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 1:
		for (int i = 0; i < result.size(); i++) {
			result[i] = delta(m_data.m_S, vec[i], m_data.m_T, m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 2:
		for (int i = 0; i < result.size(); i++) {
			result[i] = delta(m_data.m_S, m_data.m_K, vec[i], m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 3:
		for (int i = 0; i < result.size(); i++) {
			result[i] = delta(m_data.m_S, m_data.m_K, m_data.m_T, vec[i], m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 4:
		for (int i = 0; i < result.size(); i++) {
			result[i] = delta(m_data.m_S, m_data.m_K, m_data.m_T, m_data.m_r, vec[i], m_data.m_b, m_type);
		}
		break;
	case 5:
		for (int i = 0; i < result.size(); i++) {
			result[i] = delta(m_data.m_S, m_data.m_K, m_data.m_T, m_data.m_r, m_data.m_sig, vec[i], m_type);
		}
		break;
	}
	return result;
}

vector<double> EuropeanOption::ApproxDelta(double h, const vector<double>& vec, int para) const {
	vector<double> result(vec.size());
	switch (para) {
	case 0:
		for (int i = 0; i < result.size(); i++) {
			result[i] = approx_delta(h, vec[i], m_data.m_K, m_data.m_T, m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 1:
		for (int i = 0; i < result.size(); i++) {
			result[i] = approx_delta(h, m_data.m_S, vec[i], m_data.m_T, m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 2:
		for (int i = 0; i < result.size(); i++) {
			result[i] = approx_delta(h, m_data.m_S, m_data.m_K, vec[i], m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 3:
		for (int i = 0; i < result.size(); i++) {
			result[i] = approx_delta(h, m_data.m_S, m_data.m_K, m_data.m_T, vec[i], m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 4:
		for (int i = 0; i < result.size(); i++) {
			result[i] = approx_delta(h, m_data.m_S, m_data.m_K, m_data.m_T, m_data.m_r, vec[i], m_data.m_b, m_type);
		}
		break;
	case 5:
		for (int i = 0; i < result.size(); i++) {
			result[i] = approx_delta(h, m_data.m_S, m_data.m_K, m_data.m_T, m_data.m_r, m_data.m_sig, vec[i], m_type);
		}
		break;
	}
	return result;
}

vector<vector<double>> EuropeanOption::Delta(const vector<vector<double>>& mat, const vector<int>& paras) const {
	vector<vector<double>> result;
	for (int i = 0; i < paras.size(); i++) {
		result.push_back(Delta(mat[i], paras[i]));
	}
	return result;
}

vector<vector<double>> EuropeanOption::ApproxDelta(double h, const vector<vector<double>>& mat, const vector<int>& paras) const {
	vector<vector<double>> result;
	for (int i = 0; i < paras.size(); i++) {
		result.push_back(ApproxDelta(h, mat[i], paras[i]));
	}
	return result;
}

double EuropeanOption::gamma(double S, double K, double T, double r, double sig, double b, const Type& type) const {
	normal_distribution<> normal(0.0, 1.0);
	double d1 = (log(S / K) + (b + pow(sig, 2) / 2.0) * T) / (sig * sqrt(T));
	return pdf(normal, d1) * exp((b - r) * T) / (S * sig * sqrt(T));
}

double EuropeanOption::approx_gamma(double h, double S, double K, double T, double r, double sig, double b, const Type& type) const {
	double V1 = price(S - h, K, T, r, sig, b, type);
	double V2 = price(S, K, T, r, sig, b, type);
	double V3 = price(S + h, K, T, r, sig, b, type);	
	return (V1 - 2 * V2 + V3) / pow(h, 2);
}

double EuropeanOption::Gamma() const {
	return gamma(m_data.m_S, m_data.m_K, m_data.m_T, m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
}

double EuropeanOption::ApproxGamma(double h) const {
	return approx_gamma(h, m_data.m_S, m_data.m_K, m_data.m_T, m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
}

vector<double> EuropeanOption::Gamma(const vector<double>& vec, int para) const {
	vector<double> result(vec.size());
	switch (para) {
	case 0:
		for (int i = 0; i < result.size(); i++) {
			result[i] = gamma(vec[i], m_data.m_K, m_data.m_T, m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 1:
		for (int i = 0; i < result.size(); i++) {
			result[i] = gamma(m_data.m_S, vec[i], m_data.m_T, m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 2:
		for (int i = 0; i < result.size(); i++) {
			result[i] = gamma(m_data.m_S, m_data.m_K, vec[i], m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 3:
		for (int i = 0; i < result.size(); i++) {
			result[i] = gamma(m_data.m_S, m_data.m_K, m_data.m_T, vec[i], m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 4:
		for (int i = 0; i < result.size(); i++) {
			result[i] = gamma(m_data.m_S, m_data.m_K, m_data.m_T, m_data.m_r, vec[i], m_data.m_b, m_type);
		}
		break;
	case 5:
		for (int i = 0; i < result.size(); i++) {
			result[i] = gamma(m_data.m_S, m_data.m_K, m_data.m_T, m_data.m_r, m_data.m_sig, vec[i], m_type);
		}
		break;
	}
	return result;
}

vector<double> EuropeanOption::ApproxGamma(double h, const vector<double>& vec, int para) const {
	vector<double> result(vec.size());
	switch (para) {
	case 0:
		for (int i = 0; i < result.size(); i++) {
			result[i] = approx_gamma(h, vec[i], m_data.m_K, m_data.m_T, m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 1:
		for (int i = 0; i < result.size(); i++) {
			result[i] = approx_gamma(h, m_data.m_S, vec[i], m_data.m_T, m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 2:
		for (int i = 0; i < result.size(); i++) {
			result[i] = approx_gamma(h, m_data.m_S, m_data.m_K, vec[i], m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 3:
		for (int i = 0; i < result.size(); i++) {
			result[i] = approx_gamma(h, m_data.m_S, m_data.m_K, m_data.m_T, vec[i], m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 4:
		for (int i = 0; i < result.size(); i++) {
			result[i] = approx_gamma(h, m_data.m_S, m_data.m_K, m_data.m_T, m_data.m_r, vec[i], m_data.m_b, m_type);
		}
		break;
	case 5:
		for (int i = 0; i < result.size(); i++) {
			result[i] = approx_gamma(h, m_data.m_S, m_data.m_K, m_data.m_T, m_data.m_r, m_data.m_sig, vec[i], m_type);
		}
		break;
	}
	return result;
}

vector<vector<double>> EuropeanOption::Gamma(const vector<vector<double>>& mat, const vector<int>& paras) const {
	vector<vector<double>> result;
	for (int i = 0; i < paras.size(); i++) {
		result.push_back(Gamma(mat[i], paras[i]));
	}
	return result;
}

vector<vector<double>> EuropeanOption::ApproxGamma(double h, const vector<vector<double>>& mat, const vector<int>& paras) const {
	vector<vector<double>> result;
	for (int i = 0; i < paras.size(); i++) {
		result.push_back(ApproxGamma(h, mat[i], paras[i]));
	}
	return result;
}

double EuropeanOption::Vega() const {
	normal_distribution<> normal(0.0, 1.0);
	double d1 = (log(m_data.m_S / m_data.m_K) + (m_data.m_b + pow(m_data.m_sig, 2) / 2.0) * m_data.m_T) / (m_data.m_sig * sqrt(m_data.m_T));
	return m_data.m_S * sqrt(m_data.m_T) * exp((m_data.m_b - m_data.m_r) * m_data.m_T) * pdf(normal, d1);
}

double EuropeanOption::Theta() const {
	normal_distribution<> normal(0.0, 1.0);
	double id = (m_type == Type::call) ? 1.0 : (-1.0);
	double d1 = (log(m_data.m_S / m_data.m_K) + (m_data.m_b + pow(m_data.m_sig, 2) / 2.0) * m_data.m_T) / (m_data.m_sig * sqrt(m_data.m_T));
	double d2 = d1 - m_data.m_sig * sqrt(m_data.m_T);
	return -m_data.m_S * m_data.m_sig * exp((m_data.m_b - m_data.m_r) * m_data.m_T) * pdf(normal, d1) / (2 * sqrt(m_data.m_T)) - (m_data.m_b - m_data.m_r) * m_data.m_S * exp((m_data.m_b - m_data.m_r) * m_data.m_T) * cdf(normal, d1) - id * m_data.m_r * m_data.m_K * exp(-m_data.m_r * m_data.m_T) * cdf(normal, id * d2);
}

double EuropeanOption::PutCallParity(double price) const {
	double id = (m_type == Type::call) ? 1.0 : (-1.0);
	return price - id * (m_data.m_S - m_data.m_K * exp(-m_data.m_r * m_data.m_T));
}

bool EuropeanOption::ISPutCallParity(double price, double tol) const {
	double id = (m_type == Type::call) ? 1.0 : (-1.0);
	double p = price + id * (m_data.m_S - m_data.m_K * exp(-m_data.m_r * m_data.m_T));
	return abs(Price()-p) <= tol;
}

