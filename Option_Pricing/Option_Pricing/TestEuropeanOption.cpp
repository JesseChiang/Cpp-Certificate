#include "EuropeanOption.hpp"
#include "Mesher.hpp"
#include <iostream>
#include <iomanip>

int main() {
	try {
		// Batches
		EuropeanOption batch1(60, 65, 0.25, 0.08, 0.30, 0.08);
		EuropeanOption batch2(100, 100, 1.0, 0.0, 0.2, 0.0);
		EuropeanOption batch3(5, 10, 1.0, 0.12, 0.50, 0.12);
		EuropeanOption batch4(100, 100, 30.0, 0.08, 0.30, 0.08);

		/* A.I. Exact Solutions of One-Factor Plain Options */

		// a)
		cout << "=== A.I.(a) ===" << endl;
		// bath 1
		cout << "Price of batch 1: Call = " << batch1.Price() << ", Put = ";
		batch1.toggle(); // toggle to put option
		cout << batch1.Price() << endl;
		batch1.toggle(); // toggle back to call option
		// batch 2
		cout << "Price of batch 2: Call = " << batch2.Price() << ", Put = ";
		batch2.toggle(); // toggle to put option
		cout << batch2.Price() << endl;
		batch2.toggle(); // toggle back to call option
		// batch 3
		cout << "Price of batch 3: Call = " << batch3.Price() << ", Put = ";
		batch3.toggle(); // toggle to put option
		cout << batch3.Price() << endl;
		batch3.toggle(); // toggle back to call option
		// batch 4
		cout << "Price of batch 4: Call = " << batch4.Price() << ", Put = ";
		batch4.toggle(); // toggle to put option
		cout << batch4.Price() << endl;
		batch4.toggle(); // toggle back to call option
		cout << endl;

		// b)
		cout << "=== A.I.(b) ===" << endl;
		// batch 1
		cout << "Using parity, batch 1 put = " << batch1.PutCallParity(2.13337);
		cout << ", the given put price satisfies parity: " << ((batch1.ISPutCallParity(5.84628, 0.00001)) ? "True" : "False") << endl;
		// batch 2
		cout << "Using parity, batch 2 put = " << batch2.PutCallParity(7.96557);
		cout << ", the given put price satisfies parity: " << ((batch2.ISPutCallParity(7.96557, 0.00001)) ? "True" : "False") << endl;
		// batch 3
		cout << "Using parity, batch 3 put = " << batch3.PutCallParity(0.204058);
		cout << ", the given put price satisfies parity: " << ((batch3.ISPutCallParity(4.07326, 0.00001)) ? "True" : "False") << endl;
		// batch 4
		cout << "Using parity, batch 4 put = " << batch4.PutCallParity(92.17570);
		cout << ", the given put price satisfies parity: " << ((batch4.ISPutCallParity(1.24750, 0.00001)) ? "True" : "False") << endl;
		cout << endl;

		//c) batch 1 data as an example
		cout << "=== A.I.(c) ===" << endl;
		vector<double> mesh_S = Mesher(55.0, 65.0, 1.0);
		vector<double> batch1_price_S = batch1.Price(mesh_S, 0);
		cout << "Stock\tOption" << endl;
		for (int i = 0; i < (int)mesh_S.size(); i++) {
			cout << mesh_S[i] << "\t" << batch1_price_S[i] << endl;
		}
		cout << endl;

		//d) batch 1 data as an example
		cout << "=== A.I.(d) ===" << endl;
		vector<vector<double>> mesh_S_K_T_r_sig_b;
		vector<int> paras_S_K_T_r_sig_b;
		mesh_S_K_T_r_sig_b.push_back(mesh_S); paras_S_K_T_r_sig_b.push_back(0);
		mesh_S_K_T_r_sig_b.push_back(Mesher(60.0, 70.0, 1.0)); paras_S_K_T_r_sig_b.push_back(1);
		mesh_S_K_T_r_sig_b.push_back(Mesher(0.20, 0.301, 0.01)); paras_S_K_T_r_sig_b.push_back(2);
		mesh_S_K_T_r_sig_b.push_back(Mesher(0.03, 0.13, 0.01)); paras_S_K_T_r_sig_b.push_back(3);
		mesh_S_K_T_r_sig_b.push_back(Mesher(0.10, 0.50, 0.04)); paras_S_K_T_r_sig_b.push_back(4);
		mesh_S_K_T_r_sig_b.push_back(Mesher(0.03, 0.13, 0.01)); paras_S_K_T_r_sig_b.push_back(5);
		vector<vector<double>> batch1_price_S_K_T_r_sig_b = batch1.Price(mesh_S_K_T_r_sig_b, paras_S_K_T_r_sig_b);
		cout << "S\tCall\tK\tCall\tT\tCall\tr\tCall\tsig\tCall\tb\tCall" << endl;
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 6; j++) {
				cout << setprecision(5) << mesh_S_K_T_r_sig_b[j][i] << "\t" << batch1_price_S_K_T_r_sig_b[j][i] << "\t";
			}
			cout << endl;
		}
		cout << endl;

		/* A.II. Option Sensitivities, aka the Greeks */

		//a)
		cout << "=== A.II.(a) ===" << endl;
		EuropeanOption option(105, 100, 0.5, 0.1, 0.36, 0);
		double call_price = option.Price();
		double call_delta = option.Delta();
		double call_gamma = option.Gamma();
		double call_vega = option.Vega();
		double call_theta = option.Theta();
		option.toggle(); // change option type to put
		double put_price = option.Price();
		double put_delta = option.Delta();
		double put_gamma = option.Gamma();
		double put_vega = option.Vega();
		double put_theta = option.Theta();
		option.toggle(); // change back 

		cout << "\tPrice\tDelta\tGamma    \tVega\tTheta" << endl;
		cout << "Call:\t"
			<< call_price << "\t"
			<< call_delta << "\t"
			<< call_gamma << "\t"
			<< call_vega << "\t"
			<< call_theta << endl;
		cout << "Put:\t"
			<< put_price << "\t"
			<< put_delta << "\t"
			<< put_gamma << "\t"
			<< put_vega << "\t"
			<< put_theta << endl;

		cout << endl;

		//b)
		cout << "=== A.II.(b) ===" << endl;
		mesh_S = Mesher(100, 110, 1.0);
		vector<double> option_delta_S = option.Delta(mesh_S, 0);
		cout << "Stock\tDelta" << endl;
		for (int i = 0; i < (int)mesh_S.size(); i++) {
			cout << mesh_S[i] << "\t" << option_delta_S[i] << endl;
		}
		cout << endl;

		//c)
		cout << "=== A.II.(c) ===" << endl;
		vector<vector<double>> mesh_S_K_T_r_sig;
		vector<int> paras_S_K_T_r_sig;
		mesh_S_K_T_r_sig.push_back(mesh_S); paras_S_K_T_r_sig.push_back(0);
		mesh_S_K_T_r_sig.push_back(Mesher(95.0, 105.0, 1.0)); paras_S_K_T_r_sig.push_back(1);
		mesh_S_K_T_r_sig.push_back(Mesher(0.45, 0.55, 0.01)); paras_S_K_T_r_sig.push_back(2);
		mesh_S_K_T_r_sig.push_back(Mesher(0.05, 0.151, 0.01)); paras_S_K_T_r_sig.push_back(3);
		mesh_S_K_T_r_sig.push_back(Mesher(0.16, 0.56, 0.04)); paras_S_K_T_r_sig.push_back(4);
		vector<vector<double>> option_delta_S_K_T_r_sig = option.Price(mesh_S_K_T_r_sig, paras_S_K_T_r_sig);
		cout << "S\tDelta\tK\tDelta\tT\tDelta\tr\tDelta\tsig\tDelta" << endl;
		for (int i = 0; i < 11; i++) {
			for (int j = 0; j < 5; j++) {
				cout << setprecision(5) << mesh_S_K_T_r_sig[j][i] << "\t" << option_delta_S_K_T_r_sig[j][i] << "\t";
			}
			cout << endl;
		}
		cout << endl;

		//d)
		cout << "=== A.II.(d) ===" << endl;
		cout << "== Table 1: Comparing h" << endl;
		cout << "Approx\tDelta    \tErr       \tGamma      \tErr" << endl;
		for (int i = 1; i < 11; i++) {
			double h = pow(10, -i);
			cout << setprecision(8) << "h=1e" << -i << "\t" << option.ApproxDelta(h) << "\t" << abs(option.ApproxDelta(h) - call_delta) << "\t"
				<< option.ApproxGamma(h) << "\t" << abs(option.ApproxGamma(h) - call_gamma) << endl;
		}
		cout << "== Table 2: Vector Output S Range (h=0.001)" << endl;
		vector<double> option_approxdelta_S = option.ApproxDelta(0.001, mesh_S, 0);
		vector<double> option_gamma_S = option.Gamma(mesh_S, 0);
		vector<double> option_approxgamma_S = option.ApproxGamma(0.001, mesh_S, 0);
		cout << "S\tDelta    \tErr       \tGamma    \tErr" << endl;
		for (int i = 0; i < (int)mesh_S.size(); i++) {
			cout << mesh_S[i] << "\t"
				<< option_approxdelta_S[i] << "\t" << abs(option_approxdelta_S[i] - option_delta_S[i]) << "\t"
				<< option_approxgamma_S[i] << "\t" << abs(option_approxgamma_S[i] - option_gamma_S[i]) << endl;
		}
	}
	catch (ImproperOptionDataException & err) { // catch the improper option data exception
		cout << err.GetMessage() << endl;
	}
	catch (...) {
		cout << "Error: unknown error!" << endl;
	}
	return 0;
}

/*
=== A.I.(a) ===
Price of batch 1: Call = 2.13337, Put = 5.84628
Price of batch 2: Call = 7.96557, Put = 7.96557
Price of batch 3: Call = 0.204058, Put = 4.07326
Price of batch 4: Call = 92.1757, Put = 1.2475

=== A.I.(b) ===
Using parity, batch 1 put = 5.84628, the given put price satisfies parity: True
Using parity, batch 2 put = 7.96557, the given put price satisfies parity: True
Using parity, batch 3 put = 4.07326, the given put price satisfies parity: True
Using parity, batch 4 put = 1.2475, the given put price satisfies parity: True

=== A.I.(c) ===
Stock   Option
55      0.76652
56      0.965684
57      1.19971
58      1.47106
59      1.78175
60      2.13337
61      2.52699
62      2.96317
63      3.44196
64      3.96293
65      4.5252

=== A.I.(d) ===
S       Call    K       Call    T       Call    r       Call    sig     Call    b       Call
55      0.76652 60      4.1771  0.2     1.701   0.03    2.1602  0.1     0.1731  0.03    1.8674
56      0.96568 61      3.6858  0.21    1.7893  0.04    2.1548  0.14    0.46919 0.04    1.9185
57      1.1997  62      3.2371  0.22    1.8767  0.05    2.1494  0.18    0.83978 0.05    1.9706
58      1.4711  63      2.8299  0.23    1.9631  0.06    2.1441  0.22    1.2506  0.06    2.0238
59      1.7817  64      2.4627  0.24    2.0487  0.07    2.1387  0.26    1.6847  0.07    2.078
60      2.1334  65      2.1334  0.25    2.1334  0.08    2.1334  0.3     2.1334  0.08    2.1334
61      2.527   66      1.8399  0.26    2.2172  0.09    2.128   0.34    2.5916  0.09    2.1898
62      2.9632  67      1.5798  0.27    2.3003  0.1     2.1227  0.38    3.0562  0.1     2.2473
63      3.442   68      1.3507  0.28    2.3826  0.11    2.1174  0.42    3.5254  0.11    2.3059
64      3.9629  69      1.1499  0.29    2.4641  0.12    2.1121  0.46    3.9978  0.12    2.3657
65      4.5252  70      0.97498 0.3     2.5449  0.13    2.1069  0.5     4.4724  0.13    2.4265

=== A.II.(a) ===
		Price   Delta   Gamma           Vega    Theta
Call:   12.433  0.59463 0.013494        26.778  -8.3968
Put:    7.6767  -0.3566 0.013494        26.778  1.1155

=== A.II.(b) ===
Stock   Delta
100     0.52379
101     0.53846
102     0.55289
103     0.56708
104     0.58099
105     0.59463
106     0.60798
107     0.62102
108     0.63377
109     0.6462
110     0.65831

=== A.II.(c) ===
S       Delta   K       Delta   T       Delta   r       Delta   sig     Delta
100     9.6341  95      15.121  0.45    11.998  0.05    12.748  0.16    7.1787
101     10.165  96      14.553  0.46    12.088  0.06    12.684  0.2     8.1977
102     10.711  97      14      0.47    12.176  0.07    12.621  0.24    9.2412
103     11.271  98      13.463  0.48    12.263  0.08    12.558  0.28    10.298
104     11.845  99      12.94   0.49    12.348  0.09    12.495  0.32    11.363
105     12.433  100     12.433  0.5     12.433  0.1     12.433  0.36    12.433
106     13.034  101     11.94   0.51    12.516  0.11    12.371  0.4     13.505
107     13.649  102     11.462  0.52    12.599  0.12    12.309  0.44    14.578
108     14.276  103     10.999  0.53    12.68   0.13    12.248  0.48    15.651
109     14.916  104     10.55   0.54    12.76   0.14    12.187  0.52    16.724
110     15.568  105     10.116  0.55    12.839  0.15    12.126  0.56    17.795

=== A.II.(d) ===
== Table 1: Comparing h
Approx  Delta           Err             Gamma           Err
h=1e-1  0.59462818      4.82543e-07     0.013493629     8.2622827e-09
h=1e-2  0.59462865      4.8249018e-09   0.013493637     4.0592732e-11
h=1e-3  0.59462866      4.6146642e-11   0.013493647     9.9780598e-09
h=1e-4  0.59462866      3.9118486e-11   0.013493207     4.3055844e-07
h=1e-5  0.59462866      5.364984e-10    0.013855583     0.00036194624
h=1e-6  0.59462866      3.023398e-09    0.035527137     0.0220335
h=1e-7  0.59462863      2.8951025e-08   1.4210855       1.4075918
h=1e-8  0.5946287       4.2103248e-08   213.16282       213.14933
h=1e-9  0.59462479      3.8658818e-06   14210.855       14210.841
h=1e-10 0.59468874      6.0082964e-05   -710542.74      710542.75
== Table 2: Vector Output S Range (h=0.001)
S       Delta           Err             Gamma           Err
100     0.52378525      3.2287173e-11   0.014787354     1.3736804e-08
101     0.53845887      4.0849546e-11   0.014557152     6.8706406e-09
102     0.55289429      4.0867643e-11   0.014311219     3.023508e-08
103     0.5670765       4.2293169e-11   0.014050968     2.643978e-09
104     0.58099197      4.9842019e-11   0.013777971     1.2402532e-10
105     0.59462866      4.6146642e-11   0.013493647     9.9780598e-09
106     0.60797594      4.9654503e-11   0.013199383     1.0830386e-09
107     0.62102457      4.1144976e-11   0.012896571     3.4279327e-09
108     0.63376668      4.7409965e-11   0.012586561     2.5253705e-08
109     0.64619566      5.0000781e-11   0.012270526     9.2784351e-09
110     0.65830617      3.979872e-11    0.011949787     1.7662865e-09
*/

