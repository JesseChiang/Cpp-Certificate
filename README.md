# C++ Programming for Financial Engineering
### Level 9: Group A&B Exact Pricing Methods



#### Contents

[TOC]

<div STYLE="page-break-after: always;"></div>

### 1. Design (justification of deisions)

The project contains in total 9 files, including:

- 5 .hpp file: `Option`/`EuropeanOption`/`AmricanOption`/`Mesher`/`ImproperOptionDataException`.cpp
- 4 .cpp file: `EuropeanOption`/`AmricanOption`/`TestEuropeanOption`/`TestAmericanOption`.cpp

#### 1.0. Globals

##### 1.0.1. Enum Class: *Type*

- *Type* is the option type of the class. The code is placed in `Option.hpp` file.

  Sine there are only two type of options, instead of using number indicator (e.g. +1 for call and -1 for put) or string (e.g. "call", "put"), it better to make the *Type* an enum class so that it's type-safe and unnecessary conversions could be avoid.

  ```C++
  enum Class Type {
  	put, call
  }
  ```

##### 1.0.2. Struct: *EuropeanOptionData*

- Instead of making the option information as class member, it's preferred to store them in the a  sturcture for resuablity purpose. In real life, the data are usually collect separately from databases. Structing a data option so that we can store each records of data in an object as an object data.

  ```c++
  struct EuropeanOptionData {
  	double m_S; double m_K; double m_T; double m_r; double m_sig; double m_b;
    EuropeanOptionData(double S, double K, double T, double r, double sig, double b) {
      if ((S <= 0.0) || (K <= 0.0) || (T <= 0.0) || (sig <= 0.0)) {
  			throw ImproperOptionDataException();
  		}
      m_S = S; m_K = K; m_T = T; m_r = r; m_sig = sig; m_b = b;
    }
  }
  ```

##### 1.0.3. Struct: *AmericanOptionData*

- For the same reason with *EuropeanOptionData*, the *AmericanOptionData* is stored in a structure. The reason we separate the European option data and the American option data is that, American option data do not have maturity time parameter in the perpetual American option model.

  ```C++
  struct AmericanOptionData {
  	double m_S; double m_K; double m_r; double m_sig; double m_b;
   	AmericanOptionData(double S, double K, double T, double r, double sig, double b) {
      if ((S <= 0.0) || (K <= 0.0) || (sig <= 0.0)) {
  			throw ImproperOptionDataException();
  		}
      m_S = S; m_K = K; m_r = r; m_sig = sig; m_b = b;
    }
  }
  ```

##### 1.0.2. Global Function: *Mesher()* `Mesher.hpp`

- *Mesher()* produces a mesh array of doubles separated by a mesh size h.

  Negative mesh size is supported when *end* is smaller than *begin*, for example, *begin*= 1, *end* = 0, and *h* = - 0.1, vector [1.0, 0.9, ..., 0.1, 0] is generated. But when (*end* - *begin*) / *h* is negative, a vector of single element [*begin*] is returned.

  ```C++
  vector<double> Mesher(double begin, double end, double h) {
  	int n = (end - begin) / h + 1;
  	n = (n < 0) ? 1 : n;
  	vector<double> mesh(n);
  	mesh[0] = begin;
  	for (int i = 1; i < n; i++) {
  		mesh[i] = mesh[i - 1] + h;
  	}
  	return mesh;
  }
  ```

<div STYLE="page-break-after: always;"></div>

#### 1.1. Base Class *Option*  `Option.hpp` 

| Class            | Option                    |
| ---------------- | :------------------------ |
| Member:          | - m_type                  |
| Member Function: | + toggle()<br>+ Price()=0 |

Both European option and American option are option, but they have different characteristics and the pricing model is different. To make it closer to the real life, The abstract base class *Option* is created. It has type as member field since the option must be either call or put. Since perpertual American option has different data with European option, it's preferred to make them as member field of the corresponding derived class.

The *Option* class includes constructors, destructor, assignment operator and *toggle()* member function. The *Price()* function is virtual because we cannot compute the price without the option data.

##### 1.1.1. Constructor

- Default constructor: default type is call.

  ```C++
  Option() { m_type = Type::call; }; 
  ```

- Constructor with option type argument

  ```C++
  Option(const Type& type) { m_type = type; };
  ```

- Copy constructor

  ```C++
  Option(const Option& source) { m_type = source.m_type; };
  ```

##### 1.1.2. Destructor

- Distructor: declare as virtual as it has derived calsses

  ```c++
  virtual ~Option() {};
  ```

##### 1.1.3. Operator

- Assignment operator

  ```C++
  Option& operator = (const Option& source);
  ```

##### 1.1.3. Function

- ***toggle()***: member fuction that changes the type of the option.

  ```C++
  void toggle() { 
  	m_type = (m_type == Type::call) ? Type::put : Type::call; 
  };
  ```

- ***Price()***: pure virtual member function.

  ```C++
  virtual double Price() const = 0;
  ```


<div STYLE="page-break-after: always;"></div>

#### 1.2. Derived Class *EuropeanOption* `EuropeanOption.hpp/cpp`

| Class            | EuropeanOption                                               |
| ---------------- | ------------------------------------------------------------ |
| Member:          | - m_data                                                     |
| Member Function: | + price()<br>+ Price()<br>+ delta()<br>+ Delta()<br>+ approx_delta<br>+ ApproxDelta()<br>+ gamma()<br>+ Gamma()<br>+ approx_gamma()<br>+ ApproxGamma()<br>+ Vega()<br>+ Theta()<br>+ PutCallParity<br>+ IsPutCallParity() |

*EuropeanOption* Class is a derived class of *Option* Class, it inherites the option type member field from the base class and have its own member field: the option data. The getters and setters are omitted for simplicity.

##### 1.2.1. Constructor

- Defualt costructor: using batch 1 data as default

  ```C++
  EuropeanOption() : Option(), m_data(60,65,0.25,0.08,0.30,0.25) {};
  ```

- Construtctor with option data

  Without type: default type is call

  ```C++
  EuropeanOption(double S, double K, double T, double r, double sig, double b) : Option(), m_data(S, K, T, r, sig, b) {};
  ```

  ```C++
  EuropeanOption(const EuropeanOptionData& data) : Option(), m_data(data) {};
  ```

  With type:

  ```C++
  EuropeanOption(double S, double K, double T, double r, double sig, double b, const Type& type) : Option(type), m_data(S,K,T,r,sig,b) {};
  ```

  ```C++
  EuropeanOption(const EuropeanOptionData& data, const Type* type) : Option(type), m_data(data) {};
  ```

- Copy constructor

  ```C++
  EuropeanOption(const EuropeanOption& source): Option(source), m_data(source.m_data) {};
  ```

##### 1.2.2. Distrutor

- Distructor

  ```C++
  virtual ~EuropeanOption() {};
  ```

##### 1.2.3. Operator 

- Assignment operator

  ```C++
  EuropeanOption& operator = (const EuropeanOption& source);
  ```

##### 1.2.4. Function

In computing the option price and sensitivities, as well as implementing the put call parity, we can generalize the computation for call and put to a same formula with the help of indicator. We set *id* indicating call (*id*=1) and put (*id*=-1), then the pricing formula becomes: 
$$
V(id) = id\times[Se^{(b-r)T}N(id\times d_1)-Ke^{-rT}N(id\times d_2)]
$$
The put call parity pricing can be expresses as:
$$
V(id) = V(-id)+id\times(S-Ke^{-rT})
$$
Formula for sensitivities can be derived by differentiating the pricing formula.

Basically the design for the pricing and sensitivity are analogous: first, define a private function that takes the option data as input and returns the price/sensitivity; second, define a global function that calls the private function to get the price of the option; third, overload the global function that also calls the private function to deal with vector input; finally, overload the global function that calls the overloaded global function which takes vector input to deal with matrix input.

1. ##### For Pricing

- ***price()***: private function for calculating the option price using pricing parameters

  ```C++
  double price(double S, double K, double T, double r, double sig, double b, const Type& type) const {
    normal_distribution<> normal(0.0, 1.0);
  	double id = (type == Type::call) ? 1.0 : (-1.0);
  	double d1 = (log(S / K) + (b + pow(sig, 2) / 2.0) * T) / (sig * sqrt(T));
  	double d2 = d1 - sig * sqrt(T);
  	return id * (S * exp((b - r) * T) * cdf(normal, id * d1) - K * exp(-r * T) * cdf(normal, id * d2));
  };
  ```

- ***Price()***: public function that invoke *price()* for pricing

  *Price()* calls *price()* that computes the option price with *m_data* member field

  ```C++
  double Price() const;
  ```

  Overloaded *Price()* that takes vector input (invoke *price()* inside the function)

  ```C++
  vector<double> Price(const vector<double>& vec, int para) const;
  ```

  Overloaded *Price()* that takes matrix input (invoke *Price()* with vector input inside the function)

  ```C++
  vector<vector<double>> Price(const vector<vector<double>>& mat, const vector<int>& paras) const;
  ```

2. ##### For Sensitivity

- Delta

  - ***delta()***: private function for calculating the option price using pricing parameters

    ```C++
    double delta(double S, double K, double T, double r, double sig, double b, const Type& type) const {
      normal_distribution<> normal(0.0, 1.0);
    	double id = (type == Type::call) ? 1.0 : (-1.0);
    	double d1 = (log(S / K) + (b + pow(sig, 2) / 2.0) * T) / (sig * sqrt(T));
    	return id * exp((b - r) * T) * cdf(normal, id * d1);
    };
    ```

  - ***Delta()***: public function that invoke *delta()*

    *Delta()* calls *delta()* that computes the option delta with *m_data* member field

    ```C++
    double Delta() const;
    ```

    Overloaded *Delta()* that takes vector input (invoke *delta()* inside the function)

    ```C++
    vector<double> Delta(const vector<double>& vec, int para) const;
    ```

    Overloaded *Delta()* that takes matrix input (invoke *Delta()* with vector input inside the function)

    ```C++
    vector<vector<double>> Delta(const vector<vector<double>>& mat, const vector<int>& paras) const;
    ```

  - ***approx_delta()***: private function for calculating the option delta using finite difference method.

    ```C++
    double approx_delta(double h, double S, double K, double T, double r, double sig, double b, const Type& type) const {
      double V1 = price(S + h, K, T, r, sig, b, type);
    	double V2 = price(S - h, K, T, r, sig, b, type);
    	return (V1 - V2) / (2.0 * h);
    };
    ```

  - ***ApproxDelta()***

    The design of *ApproxDelta()* are almost identical to the *Delta()* except for *delta()* is replaced by *approx_delta()*.

    ```C++
    double ApproxDelta(double h) const;
    ```

    ```C++
    vector<double> ApproxDelta(double h, const vector<double>& vec, int para) const;
    ```

    ```C++
    vector<vector<double>> ApproxDelta(double h, const vector<vector<double>>& mat, const vector<int>& paras) const;
    ```

- Gamma

  - ***gamma()***: private function for calculating the option gamma using pricing parameters

    ```C++
    double gamma(double S, double K, double T, double r, double sig, double b, const Type& type) const {
      normal_distribution<> normal(0.0, 1.0);
    	double d1 = (log(S / K) + (b + pow(sig, 2) / 2.0) * T) / (sig * sqrt(T));
    	return pdf(normal, d1) * exp((b - r) * T) / (S * sig * sqrt(T));
    };
    ```

  - ***Gamma()***: public function that invoke *gamma()*

    *Gamma()* calls *gamma()* that computes the option gamma with *m_data* member field

    ```C++
    double Gamma() const;
    ```

    Overloaded *Gamma()* that takes vector input (invoke *gamma()* inside the function)

    ```C++
    vector<double> Gamma(const vector<double>& vec, int para) const;
    ```

    Overloaded *Gamma()* that takes matrix input (invoke *Gamma()* with vector input inside the function)

    ```C++
    vector<vector<double>> Gamma(const vector<vector<double>>& mat, const vector<int>& paras) const;
    ```

  - ***approx_gamma()***: private function for calculating the option gamma using finite difference method.

    ```C++
    double approx_gamma(double h, double S, double K, double T, double r, double sig, double b, const Type& type) const {
      double V1 = price(S - h, K, T, r, sig, b, type);
    	double V2 = price(S, K, T, r, sig, b, type);
    	double V3 = price(S + h, K, T, r, sig, b, type);	
    	return (V1 - 2 * V2 + V3) / pow(h, 2);
    };
    ```

  - ***ApproxGamma()***

    The design of *ApproxGamma()* are almost identical to the *Gamma()* except for *gamma()* is replaced by *approx_gamma()*

    ```C++
    double ApproxGamma(double h) const;
    ```

    ```C++
    vector<double> ApproxGamma(double h, const vector<double>& vec, int para) const;
    ```

    ```C++
    vector<vector<double>> ApproxGamma(double h, const vector<vector<double>>& mat, const vector<int>& paras) const;
    ```

- Vega

  - ***Vega()***: implements the vega formula for the *m_data*

    ```C++
    double Vega() const {
      normal_distribution<> normal(0.0, 1.0);
    	double d1 = (log(m_data.m_S / m_data.m_K) + (m_data.m_b + pow(m_data.m_sig, 2) / 2.0) * m_data.m_T) / (m_data.m_sig * sqrt(m_data.m_T));
    	return m_data.m_S * sqrt(m_data.m_T) * exp((m_data.m_b - m_data.m_r) * m_data.m_T) * pdf(normal, d1);
    };
    ```

    It can be extended to accept vector and matrix input with the same design as delta and gamma.

- Theta

  - ***Theta()***: implements the theta formula for the *m_data*

    ```C++
    double Theta() const {
      normal_distribution<> normal(0.0, 1.0);
    	double id = (m_type == Type::call) ? 1.0 : (-1.0);
    	double d1 = (log(m_data.m_S / m_data.m_K) + (m_data.m_b + pow(m_data.m_sig, 2) / 2.0) * m_data.m_T) / (m_data.m_sig * sqrt(m_data.m_T));
    	double d2 = d1 - m_data.m_sig * sqrt(m_data.m_T);
    	return -m_data.m_S * m_data.m_sig * exp((m_data.m_b - m_data.m_r) * m_data.m_T) * pdf(normal, d1) / (2 * sqrt(m_data.m_T)) - (m_data.m_b - m_data.m_r) * m_data.m_S * exp((m_data.m_b - m_data.m_r) * m_data.m_T) * cdf(normal, d1) - id * m_data.m_r * m_data.m_K * exp(-m_data.m_r * m_data.m_T) * cdf(normal, id * d2);
    };
    ```

    It can be extended to accept vector and matrix input with the same design as delta and gamma.

3. ##### For Put-call parity

- ***PutCallParity()***: computing the price using put-call parity without calling *price()*

  ```C++
  double PutCallParity(double price) const {
    double id = (m_type == Type::call) ? 1.0 : (-1.0);
  	return price - id * (m_data.m_S - m_data.m_K * exp(-m_data.m_r * m_data.m_T));
  };
  ```

- ***IsPutCallParity()***: test if the price of the other type satisfy the put-call parity given the *m_data*

  ```C++
  bool ISPutCallParity(double price, double tol) const {
    double id = (m_type == Type::call) ? 1.0 : (-1.0);
  	double p = price + id * (m_data.m_S - m_data.m_K * exp(-m_data.m_r * m_data.m_T));
  	return abs(Price()-p) <= tol;
  };
  ```


<div STYLE="page-break-after: always;"></div>

#### 1.3. Derived Class *American Options* `AmericanOption.hpp/cpp`

| Class            | AmericanOption         |
| ---------------- | :--------------------- |
| Member:          | - m_data               |
| Member Function: | + price()<br>+ Price() |

*AmericanOption* Class is a derived class of *Option* Class, it inherites the option type member field from the base class and have its own member field: the option data. The getters and setters are omitted for simplicity.

##### 1.3.1. Constructor, destructor & operator

The constructor, destructor and operator are analogous to the *EuropeanOption* class, omitted for simplicity.

##### 1.3.2. Function

- ***price()***: private function for calculating the option price using pricing parameters

  ```C++
  double price(double S, double K, double r, double sig, double b, const Type& type) const {
  	double id = (type == Type::call) ? 1.0 : (-1.0);
  	double y = 0.5 - b / pow(sig, 2) + id * sqrt(pow((b / pow(sig, 2) - 0.5), 2) + 2 * r / pow(sig, 2));
  	return id * K / (y - 1) * pow((y - 1) / y * S / K, y);
  };
  ```

- ***Price()***: public function that invoke *price()* for pricing

  The function overloading for *Price()* are exactly the same with *EuropeanOption* class, omiited as well.

  ```C++
  double Price() const;
  ```

  ```C++
  vector<double> Price(const vector<double>& vec, int para) const;
  ```

  ```C++
  vector<vector<double>> Price(const vector<vector<double>>& mat, const vector<int>& paras) const;
  ```

<div STYLE="page-break-after: always;"></div>

#### 1.4. Exception Handling `ImproperOptionDataException.hpp`

Since the B-S formula only holds for the positive prices, so the exception handling functionality is added to check the option data is correctly input. 

For the *EuropeanOptionData*, the *ImproperOptionDataException* is thrown when:

```C++
if ((S <= 0.0) || (K <= 0.0) || (T <= 0.0) || (sig <= 0.0)) {
	throw ImproperOptionDataException();
}
```

For the *AmericanOptionData*, the *ImproperOptionDataException* is thrown when:

```C++
if ((S <= 0.0) || (K <= 0.0) || (sig <= 0.0)) {
	throw ImproperOptionDataException();
}
```

The Error Message is: 

```C++
inline string ImproperOptionDataException::GetMessage() const {
	stringstream stream;
	stream << "Error: improper option data!";
	return stream.str();
}
```

<div STYLE="page-break-after: always;"></div>

### 2. Results (answer to questions)

#### A.I. Exact Solutions of One-Factor Plain Options

> 1. **a)** Implement the above formulae for call and put option pricing using the data sets Batch 1 to Batch 4. Check your answers, as you will need them when we discuss numerical methods for option pricing.

The detailed implemation of the formulae is implemented insided the *EuropeanOption* Class, the private function *price()* to be specific. Checking the calculated option prices with the option prices given, they are exactly the same:

```html
=== A.I.(a) ===
Price of batch 1: Call = 2.13337, Put = 5.84628
Price of batch 2: Call = 7.96557, Put = 7.96557
Price of batch 3: Call = 0.204058, Put = 4.07326
Price of batch 4: Call = 92.1757, Put = 1.2475
```

> 2. **b)** Apply the put-call parity relationship to compute call and put option prices. For example, given the call price, compute the put price based on this formula using Batches 1 to 4. Check your answers with the prices from part a). Note that there are two useful ways to implement parity: As a mechanism to calculate the call (or put) price for a corresponding put (or call) price, or as a mechanism to check if a given set of put/call prices satisfy parity. The ideal submission will neatly implement both approaches.

The two machanisms are implemented inside the *EuropeanOption* Class as *PutCallParity()*, *IsPutCallParity()*, resectively. Checking the option prices for the 4 bathches, all of them satisfies the realtionship: 

```html
=== A.I.(b) ===
Using parity, batch 1 put = 5.84628, the given put price satisfies parity: True
Using parity, batch 2 put = 7.96557, the given put price satisfies parity: True
Using parity, batch 3 put = 4.07326, the given put price satisfies parity: True
Using parity, batch 4 put = 1.2475, the given put price satisfies parity: True
```

> 3. **c)** Say we wish to compute option prices for a monotonically increasing range of underlying values of S, for example 10, 11, 12, …, 50. To this end, the output will be a vector. This entails calling the option pricing formulae for each value S and each computed option price will be stored in a std::vector\<double\> object. It will be useful to write a global function that produces a mesh array of doubles separated by a mesh size h.

This functionality is implemented by overloading the *Price()* function of *EuropeanOption* Class. It takes a vector and an indicator as input - the vector is the mesh array and the indicator indicats which pricing parameter to change: {0 : S, 1 : K, 2 : T, 3 : r, 4 : sig, 5 : b }.

For better display, here we use the stock prices from 55 to 65 instead of 10 to 50 as a demostrasion (the latter one is too long to print all of them out as a table):

```html
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
```

> 4. **d)** Now we wish to extend part c and compute option prices as a function of i) expiry time, ii) volatility, or iii) any of the option pricing parameters. Essentially, the purpose here is to be able to input a matrix (vector of vectors) of option parameters and receive a matrix of option prices as the result. Encapsulate this functionality in the most flexible/robust way you can think of.

This functionality is implemented by overloading the *Price()* function of *EuropeanOption* Class. It takes a matrix and an indicator vector as input. The indicator vector indicats which pricing parameters to change.

The matrix is of the form $\left[\begin{array}{ccc}S_1&K_1&T_1&\cdots\\S_2&K_2&T_2&\cdots\\\vdots&\vdots&\vdots&\vdots\\S_N&K_N&T_N&\cdots\end{array}\right]$, output matrix is of the form $\left[\begin{array}{ccc}V_{S_1}&V_{K_1}&V_{T_1}&\cdots\\V_{S_2}&V_{K_2}&V_{T_2}&\cdots\\\vdots&\vdots&\vdots&\vdots\\V_{S_N}&V_{K_N}&V_{T_N}&\cdots\end{array}\right]$

The following is a demostrasion using the call price of batch 1, we demostrate the result in the form of:
$$
\left[\begin{array}{ccccccc}S_1&V_{S_1}&K_1&V_{K_1}&T_1&V_{T_1}&\cdots\\S_2&V_{S_2}&K_2&V_{K_2}&T_2&V_{T_2}&\cdots\\\vdots&\vdots&\vdots&\vdots&\vdots&\vdots&\vdots\\S_N&V_{S_N}&K_N&V_{K_N}&T_N&V_{T_N}&\cdots\end{array}\right]
$$

```html
=== A.I.(d) ===
S    Call    K    Call    T      Call    r      Call    sig    Call    b      Call
55   0.76652 60   4.1771  0.2    1.701   0.03   2.1602  0.1    0.1731  0.03   1.8674
56   0.96568 61   3.6858  0.21   1.7893  0.04   2.1548  0.14   0.46919 0.04   1.9185
57   1.1997  62   3.2371  0.22   1.8767  0.05   2.1494  0.18   0.83978 0.05   1.9706
58   1.4711  63   2.8299  0.23   1.9631  0.06   2.1441  0.22   1.2506  0.06   2.0238
59   1.7817  64   2.4627  0.24   2.0487  0.07   2.1387  0.26   1.6847  0.07   2.078
60   2.1334  65   2.1334  0.25   2.1334  0.08   2.1334  0.3    2.1334  0.08   2.133
61   2.527   66   1.8399  0.26   2.2172  0.09   2.128   0.34   2.5916  0.09   2.1898
62   2.9632  67   1.5798  0.27   2.3003  0.1    2.1227  0.38   3.0562  0.1    2.2473
63   3.442   68   1.3507  0.28   2.3826  0.11   2.1174  0.42   3.5254  0.11   2.3059
64   3.9629  69   1.1499  0.29   2.4641  0.12   2.1121  0.46   3.9978  0.12   2.3657
65   4.5252  70   0.97498 0.3    2.5449  0.13   2.1069  0.5    4.4724  0.13   2.4265
```

<div STYLE="page-break-after: always;"></div>

#### A.II. Option Sensitivities, aka Greeks

> 1. **a)** Implement the above formulae for gamma for call and put future option pricing using the data set: K = 100, S = 105, T = 0.5, r = 0.1, b = 0 and sig = 0.36. (exact delta call = 0.5946, delta put = -0.3566).

The detailed implemation of the formulae is implemented in *delta()*, *gamma()*, *Vega()*, *Theta()* of the *EuropeanOption* class.

```html
=== A.II.(a) ===
		    Price   Delta   Gamma      Vega    Theta
Call:   12.433  0.59463 0.013494   26.778  -8.3968
Put:    7.6767  -0.3566 0.013494   26.778  1.1155
```

> 2. **b)** We now use the code in part a to compute call delta price for a monotonically increasing range of underlying values of S, for example 10, 11, 12, …, 50. To this end, the output will be a vector and it entails calling the above formula for a call delta for each value S and each computed option price will be store in a std::vector\<double\> object. It will be useful to reuse the above global function that produces a mesh array of double separated by a mesh size h.

This functionality is implemented by overloading the *Price()* function of *EuropeanOption* Class. It takes a vector and an indicator as input - the indicator indicats which pricing parameter to change: {0 : S, 1 : K, 2 : T, 3 : r, 4 : sig, 5 : b }.

Again, for better display, here we use the stock prices from 100 to 110 instead of 10 to 50 as a demostrasion:

```html
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
```

> 3. **c)** Incorporate this into your above matrix pricer code, so you can input a matrix of option parameters and receive a matrix of either Delta or Gamma as the result.

This functionality is implemented by overloading the *Delta()* and *Gamma()* function of *EuropeanOption* Class. It takes a matrix and an indicator vector as input - the indicator vector indicats which pricing parameters to change.

The matrix is of the form $\left[\begin{array}{ccc}S_1&K_1&T_1&\cdots\\S_2&K_2&T_2&\cdots\\\vdots&\vdots&\vdots&\vdots\\S_N&K_N&T_N&\cdots\end{array}\right]$, output matrix is of the form $\left[\begin{array}{ccc}\Delta_{S_1}&\Delta_{K_1}&\Delta_{T_1}&\cdots\\\Delta_{S_2}&\Delta_{K_2}&\Delta_{T_2}&\cdots\\\vdots&\vdots&\vdots&\vdots\\\Delta_{S_N}&\Delta{K_N}&\Delta_{T_N}&\cdots\end{array}\right]$

This functionality is implemented by overloading the *Price()* function of *EuropeanOption* Class. It takes a matrix and an indicator vector as input. The indicator vector indicats which pricing parameters to change.

The matrix is of the form $\left[\begin{array}{ccc}S_1&K_1&T_1&\cdots\\S_2&K_2&T_2&\cdots\\\vdots&\vdots&\vdots&\vdots\\S_N&K_N&T_N&\cdots\end{array}\right]$, output matrix is of the form $\left[\begin{array}{ccc}V_{S_1}&V_{K_1}&V_{T_1}&\cdots\\V_{S_2}&V_{K_2}&V_{T_2}&\cdots\\\vdots&\vdots&\vdots&\vdots\\V_{S_N}&V_{K_N}&V_{T_N}&\cdots\end{array}\right]$

We demostrate the result in the form of:
$$
\left[\begin{array}{ccccccc}S_1&\Delta_{S_1}&K_1&\Delta_{K_1}&T_1&\Delta_{T_1}&\cdots\\S_2&\Delta_{S_2}&K_2&\Delta_{K_2}&T_2&\Delta_{T_2}&\cdots\\\vdots&\vdots&\vdots&\vdots&\vdots&\vdots&\vdots\\S_N&\Delta_{S_N}&K_N&\Delta_{K_N}&T_N&\Delta_{T_N}&\cdots\end{array}\right]
$$

```html
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
```

> 4. **d)** The objective of this part is to perform the same calculations as in parts a and b, but now using divided differences. Compare the accuracy with various values of the parameter h (In general, smaller values of h produce better approximations but we need to avoid round-offer errors and subtraction of quantities that are very close to each other). Incorporate this into your well-designed class structure. 

Table 1 compares the accuracy with various values of parameter h. As h decreases from 1e-1 to 1e-10, the absolution error using finite difference approximation decreases at first and then increases due to the rounding-off errors and substraction of quantities that are very close. From the test result, the cutoff is 1e-4 for delta and 1e-2 for gamma, after which absolute error starts to grow.

Table 2 performs the same calculation as in parts a and b and generates a vector of deltas and gammas by change a specific pricing parameter (in this case is the stock price S) using ths finite difference approximation, with thier absolute errors presented along with them. The approximation is done by using h = 0.001, a compromise of 1e-2 and 1e-4.

```html
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
```

<div STYLE="page-break-after: always;"></div>

#### B. Perpertual Amrican Options

> 1. **a)** Program the above formulae, and incorporate into your well-designed options pricing classes.

The formulae is implemented in the *price()* function of *AmericanOption* class.

> 2. **b)** Test the data with K = 100, sig = 0.1, r = 0.1, b = 0.02, S = 110 (check C = 18.5035, P = 3.03106).

Checking the calculated option prices with the option prices given, they are exactly the same:

```html
=== B.(b) ===
Price of option: Call=18.5035, Put=3.03106
```

> 3. **c)** We now use the code in part a) to compute call and put option price for a monotonically increasing range of underlying values of S, for example 10, 11, 12, …, 50. To this end, the output will be a vector and this exercise entails calling the option pricing formulae in part a) for each value S and each computed option price will be stored in a std::vector\<double\> object. It will be useful to reuse the above global function that produces a mesh array of double separated by a mesh size h.

This functionality is implemented by overloading the *Price()* function of *EuropeanOption* Class. It takes a vector and an indicator as input - the vector is the mesh array and the indicator indicats which pricing parameter to change: {0 : S, 1 : K, 2 : T, 3 : r, 4 : sig, 5 : b }.

For better display, here we use the stock prices from 105 to 115 instead of 10 to 50 as a demostrasion (the latter one is too long to print all of them out as a table):

```html
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
```

> 4. **d)** Incorporate this into your above *matrix pricer* code, so you can input a matrix of option parameters and receive a matrix of Perpetual American option prices.

This functionality is implemented by overloading the *Price()* function of *AmericanOption* Class. It takes a matrix and an indicator vector as input. The indicator vector indicats which pricing parameters to change.

The matrix is of the form $\left[\begin{array}{ccc}S_1&K_1&T_1&\cdots\\S_2&K_2&T_2&\cdots\\\vdots&\vdots&\vdots&\vdots\\S_N&K_N&T_N&\cdots\end{array}\right]$, output matrix is of the form $\left[\begin{array}{ccc}V_{S_1}&V_{K_1}&V_{T_1}&\cdots\\V_{S_2}&V_{K_2}&V_{T_2}&\cdots\\\vdots&\vdots&\vdots&\vdots\\V_{S_N}&V_{K_N}&V_{T_N}&\cdots\end{array}\right]$

We demostrate the result in the form of:
$$
\left[\begin{array}{ccccccc}S_1&V_{S_1}&K_1&V_{K_1}&T_1&V_{T_1}&\cdots\\S_2&V_{S_2}&K_2&V_{K_2}&T_2&V_{T_2}&\cdots\\\vdots&\vdots&\vdots&\vdots&\vdots&\vdots&\vdots\\S_N&V_{S_N}&K_N&V_{K_N}&T_N&V_{T_N}&\cdots\end{array}\right]
$$

```html
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
```

