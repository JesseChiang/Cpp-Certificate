#include "AmericanOption.hpp"

double AmericanOption::price(double S, double K, double r, double sig, double b, const Type& type) const {
	double id = (type == Type::call) ? 1.0 : (-1.0);
	double y = 0.5 - b / pow(sig, 2) + id * sqrt(pow((b / pow(sig, 2) - 0.5), 2) + 2 * r / pow(sig, 2));
	return id * K / (y - 1) * pow((y - 1) / y * S / K, y);
}

double AmericanOption::Price() const {
	return price(m_data.m_S, m_data.m_K, m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
}

vector<double> AmericanOption::Price(vector<double> vec, int para) const {
	vector<double> result(vec.size());
	switch (para) {
	case 0:
		for (int i = 0; i < result.size(); i++) {
			result[i] = price(vec[i], m_data.m_K, m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 1:
		for (int i = 0; i < result.size(); i++) {
			result[i] = price(m_data.m_S, vec[i], m_data.m_r, m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 2:
		for (int i = 0; i < result.size(); i++) {
			result[i] = price(m_data.m_S, m_data.m_K, vec[i], m_data.m_sig, m_data.m_b, m_type);
		}
		break;
	case 3:
		for (int i = 0; i < result.size(); i++) {
			result[i] = price(m_data.m_S, m_data.m_K, m_data.m_r, vec[i], m_data.m_b, m_type);
		}
		break;
	case 4:
		for (int i = 0; i < result.size(); i++) {
			result[i] = price(m_data.m_S, m_data.m_K, m_data.m_r, m_data.m_sig, vec[i], m_type);
		}
		break;
	}
	return result;
}

vector<vector<double>> AmericanOption::Price(vector<vector<double>> mat, vector<int> paras) const {
	vector<vector<double>> result;
	for (int i = 0; i < paras.size(); i++) {
		result.push_back(Price(mat[i], paras[i]));
	}
	return result;
}