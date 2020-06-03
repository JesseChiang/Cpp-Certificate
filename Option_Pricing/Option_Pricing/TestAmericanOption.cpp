#include "AmericanOption.hpp"
#include "Mesher.hpp"
#include <iostream>
#include <iomanip>

int main() {
	try {
		AmericanOption option(110, 100, 0.1, 0.1, 0.02);

		/* B. Perpertual American Options */

		//b)
		cout << "=== B.(b) ===" << endl;
		cout << "Price of option: Call=" << option.Price();
		option.toggle();
		cout << ", Put=" << option.Price() << endl;
		cout << endl;

		//c)
		cout << "=== B.(c) ===" << endl;
		vector<double> mesh_S = Mesher(105.0, 115.0, 1.0);
		vector<double> option_callprice_S = option.Price(mesh_S, 0);
		option.toggle();
		vector<double> option_putprice_S = option.Price(mesh_S, 0);
		cout << "Stock\tCall\tPut" << endl;
		for (int i = 0; i < mesh_S.size(); i++) {
			cout << mesh_S[i] << "\t" << option_callprice_S[i] << "\t" << option_putprice_S[i] << endl;
		}
		cout << endl;

		//d)
		cout << "=== B.(d) ===" << endl;
		vector<vector<double>> mesh_S_K_r_sig_b;
		vector<int> paras_S_K_r_sig_b;
		mesh_S_K_r_sig_b.push_back(mesh_S); paras_S_K_r_sig_b.push_back(0);
		mesh_S_K_r_sig_b.push_back(Mesher(95.0, 105.0, 1.0)); paras_S_K_r_sig_b.push_back(1);
		mesh_S_K_r_sig_b.push_back(Mesher(0.05, 0.151, 0.01)); paras_S_K_r_sig_b.push_back(2);
		mesh_S_K_r_sig_b.push_back(Mesher(0.05, 0.151, 0.01)); paras_S_K_r_sig_b.push_back(3);
		mesh_S_K_r_sig_b.push_back(Mesher(0.00, 0.04, 0.004)); paras_S_K_r_sig_b.push_back(4);
		vector<vector<double>> option_price_S_K_r_sig_b = option.Price(mesh_S_K_r_sig_b, paras_S_K_r_sig_b);
		cout << "S\tCall\tK\tCall\tr\tCall\tsig\tCall\tb\tCall" << endl;
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 5; j++) {
				cout << setprecision(5) << mesh_S_K_r_sig_b[j][i] << "\t" << option_price_S_K_r_sig_b[j][i] << "\t";
			}
			cout << endl;
		}
		cout << endl;
	} 
	catch (ImproperOptionDataException & err) {
		cout << err.GetMessage() << endl;
	}
	catch (...) {
		cout << "Error: unknown error!" << endl;
	}
	return 0;
}

/*
=== B.(b) ===
Price of option: Call=18.5035, Put=3.03106

=== B.(c) ===
Stock   Call    Put
105     4.04761 15.9316
106     3.81598 16.4249
107     3.5996  16.9286
108     3.39733 17.4429
109     3.20813 17.9678
110     3.03106 18.5035
111     2.86523 19.0501
112     2.70985 19.6078
113     2.56416 20.1765
114     2.42748 20.7566
115     2.29919 21.3481

=== B.(d) ===
S       Call    K       Call    r       Call    sig     Call    b       Call
105     15.932  95      20.732  0.05    30.25   0.05    14.955  0       13.193
106     16.425  96      20.256  0.06    26.096  0.06    15.59   0.004   14.001
107     16.929  97      19.796  0.07    23.292  0.07    16.273  0.008   14.93
108     17.443  98      19.351  0.08    21.262  0.08    16.991  0.012   15.986
109     17.968  99      18.92   0.09    19.719  0.09    17.737  0.016   17.176
110     18.503  100     18.503  0.1     18.503  0.1     18.503  0.02    18.503
111     19.05   101     18.1    0.11    17.52   0.11    19.286  0.024   19.974
112     19.608  102     17.709  0.12    16.708  0.12    20.079  0.028   21.591
113     20.177  103     17.33   0.13    16.025  0.13    20.882  0.032   23.36
114     20.757  104     16.963  0.14    15.442  0.14    21.691  0.036   25.288
115     21.348  105     16.606  0.15    14.939  0.15    22.505  0.04    27.382
*/