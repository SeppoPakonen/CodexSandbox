#ifndef _Core_Math_h_
#define _Core_Math_h_

NAMESPACE_SDK_BEGIN










// Fast math
double exp_fast_lb(double a);
double exp_fast_ub(double a);
double exp_fast(double a);
double better_exp_fast(double a);
double exp_fast_schraudolph(double a);
float expf_fast_ub(float a);
float expf_fast(float a);
double better_expf_fast(float a);
float expf_fast_lb(float a);
double log_fast_ankerl(double a);
double log_fast_ub(double a);
double log_fast(double a);
double log_fast_lb(double a);
float logf_fast_ub(float a);
float logf_fast(float a);
float logf_fast_lb(float a);
double pow_fast_ankerl(double a, double b);
float powf_fast(float a, float b);
float powf_fast_lb(float a, float b);
float powf_fast_ub(float a, float b);
double pow_fast_ub(double a, double b);
double pow_fast(double a, double b);
double pow_fast_lb(double a, double b);
double pow_fast_precise_ankerl(double a, double b);
double pow_fast_precise(double a, double b);
double better_pow_fast_precise(double a, double b);
float powf_fast_precise(float a, float b);
float better_powf_fast_precise(float a, float b);


inline double FastLn(double a) {return log_fast(a);}
inline double FastExp(double a) {return exp_fast(a);}
inline double FastPow(double a, double b) {return pow_fast(a, b);}
inline double FastSqrt(double a) {return pow_fast(a, 0.5);}


struct FastTrig {
	static const int accuracy = 10000;
	static constexpr double PI2 = 2.0 * M_PI;
	
	Vector<double> sin, cos, tan;
	FastTrig();
	int Pos(double a) const;
	double Sin(double a) const {return sin[Pos(a)];}
	double Cos(double a) const {return cos[Pos(a)];}
	double Tan(double a) const {return tan[Pos(a)];}
};

inline FastTrig& GetFastTrig() {return Single<FastTrig>();}

inline double FastSin(double a) {return GetFastTrig().Sin(a);}
inline double FastCos(double a) {return GetFastTrig().Cos(a);}
inline double FastTan(double a) {return GetFastTrig().Tan(a);}

float FastAtan2(float y, float x);

double nround (double n, unsigned int c);

double StandardNormalCDF(double x);
double NormalPDF(double x, double mean, double dev);
double NormalPDFRelative(double value, double mean, double deviation);
double NormalCDF(double value, double mean, double deviation);

inline int Choose(int n, int k) {
	// A fast way to calculate binomial coefficients by Andrew Dalke (contrib).
	if (0 <= k && k <= n) {
		int64 ntok = 1;
		int64 ktok = 1;
		int count = min(k, n - k) + 1;
		for (int t = 1; t < count; t++) {
			ntok *= n;
			ktok *= t;
			n -= 1;
		}
		ASSERT(ktok != 0);
		return (int)(ntok / ktok);
	}
	else
		return 0;
}

struct OnlineAverage : Moveable<OnlineAverage> {
	double mean;
	int64 count;
	SpinLock lock;
	OnlineAverage() : mean(0), count(0) {}
	
	void Clear() {
		mean = 0.0;
		count = 0;
	}
	
	void Resize(int64 i) {count = i;}
	
	void Add(double a) {
		lock.Enter();
		if (count == 0) {
			mean = a;
		}
		else {
			double delta = a - mean;
			mean += delta / count;
		}
		count++;
		lock.Leave();
	}
	
	double GetMean() const {
		return mean;
	}
	
	void Serialize(Stream& s) {s % mean % count;}
	
	bool operator()(const OnlineAverage& a, const OnlineAverage& b) const {return a.mean < b.mean;}
};

struct OnlineVariance : Moveable<OnlineVariance> {
	double k = 0;
	double count = 0;
	double ex = 0;
	double ex2 = 0;
	
	String ToString() const;
	
	void Clear() {
		k = 0;
		count = 0;
		ex = 0;
		ex2 = 0;
	}
	
	void Resize(double d) {
		if (count > 0.0) {
			ex /= count;
			ex2 /= count;
			count = 1.0;
			ex *= d;
			ex2 *= d;
			count = d;
		}
	}
	
	void Add(double x) {
	    if (count == 0)
	        k = x;
	    count = count + 1;
	    double diff = x - k;
	    ex += diff;
	    ex2 += diff * diff;
	}
	
	void Add(double x, double weight) {
	    if (count == 0)
	        k = x;
	    count += weight;
	    double diff = x - k;
	    ex += diff * weight;
	    ex2 += diff * diff * weight;
	}
	
	void Remove(double x) {
	    count = count - 1;
	    double diff = x - k;
	    ex -= diff;
	    ex2 -= diff * diff;
	}
	
	double GetMean() const {
		if (count == 0.0)
			return 0.0;
		else
			return k + ex / count;
	}
	
	double GetVariance() const {return (ex2 - (ex * ex) / count) / (count - 1);}
	double GetDeviation() const {return sqrt(GetVariance());}
	double GetCDF() const {
		if (count == 0.0) return 0;
		return NormalCDF(0, GetMean(), GetDeviation());
	}
	double GetCDF(double limit, bool rside) const {
		if (count == 0.0) return 0;
		if (rside == 1)
			return 1 - NormalCDF(limit, GetMean(), GetDeviation());
		else
			return     NormalCDF(limit, GetMean(), GetDeviation());
	}
	
	
	void Serialize(Stream& s) {s % k % count % ex % ex2;}
	
};

class MovingOnlineVariance : Moveable<MovingOnlineVariance> {
	Vector<OnlineVariance> vec;
	int period;
	bool skip;
public:
	
	MovingOnlineVariance() {
		skip = false;
		period = 10;
		vec.Add();
	}
	
	OnlineVariance& Get() {return vec[0];}
	
	void SetPeriod(int i) {if (i > 0) {period = i; skip = false;} else {period = 1; skip = true; vec.SetCount(1);}}
	void AddResult(double d) {for(int i = 0; i < vec.GetCount(); i++) {vec[i].Add(d);}}
	void Next() {if (skip) return; while (vec.GetCount() >= period) {vec.Remove(0);} vec.Add();}
	void Clear() {vec.SetCount(1); vec[0].Clear();}
	
	double GetMean() {return Get().GetMean();}
	double GetDeviation() {return Get().GetDeviation();}
	double GetCDF(double cursor, int type) {return Get().GetCDF(cursor, type);}
	
};



struct StdDeviant : Moveable<StdDeviant> {
	double mean = 0, dev = 0;
	
	void Serialize(Stream& s) {s % mean % dev;}
	void Clear() {mean = 0; dev = 0;}
	
	double GetMean() const {return mean;}
	double GetDeviation() const {return dev;}
	double GetVariance() const {return dev * dev;}
	double GetCDF() const {
		return NormalCDF(0, GetMean(), GetDeviation());
	}
	double GetCDF(double limit, bool rside) const {
		if (rside == 1)
			return 1 - NormalCDF(limit, GetMean(), GetDeviation());
		else
			return     NormalCDF(limit, GetMean(), GetDeviation());
	}
	String ToString() const;
	operator double() const {return mean;}
	
	void operator+=(const StdDeviant& s) {mean += s.mean; dev += s.dev;}
	void operator-=(const StdDeviant& s) {mean -= s.mean; dev -= s.dev;}
	void operator*=(double d) {mean *= d; dev *= d;}
	void operator/=(double d) {mean /= d; dev /= d;}
	void operator=(double d) {mean = d; dev = 0;}
	bool operator<=(const StdDeviant& s) const {return mean <= s.GetMean();}
	bool operator>(const StdDeviant& s) const {return mean > s.GetMean();}
};











struct OnlineCovariance : Moveable<OnlineCovariance> {
	double meanx, meany, C;
	dword n;
	
	
	OnlineCovariance() {Clear();}
	OnlineCovariance(const OnlineCovariance& s) {*this = s;}
	void operator=(const OnlineCovariance& s) {meanx = s.meanx; meany = s.meany; C = s.C; n = s.n;}
	void Clear() {meanx = 0; meany = 0; C = 0; n = 0;}
	void Add(double x, double y);
	double Get() const {return C / n;}
	void Serialize(Stream& s) {s % meanx % meany % C % n;}
};

// https://www.statisticshowto.datasciencecentral.com/bivariate-normal-distribution/
struct OnlineBivariate : Moveable<OnlineBivariate> {
	OnlineVariance x, y;
	OnlineCovariance co;
	
	
	OnlineBivariate() {}
	OnlineBivariate(const OnlineBivariate& s) {*this = s;}
	void operator=(const OnlineBivariate& s) {x = s.x; y = s.y; co = s.co;}
	void Add(double x, double y);
	void Serialize(Stream& s) {s % x % y % co;}
	double GetPDF(double x, double y) const;
};




inline double FindPowFrom1(double ratio, double outcome) {
	double a = FastLn(ratio);
	double b = FastLn(outcome);
	double res = b / a;
	return res;
}

inline double FindPowTo1(double begin, double ratio) {
	double a = FastLn(ratio);
	double b = FastLn(begin);
	double res = -(b / a);
	return res;
}







// TODO: optimize for large amount of values
class StepDistribution : Moveable<StepDistribution> {
	Vector<int> values;
	double mean, M2;
	double value_step;
	
public:
	StepDistribution() {
		mean = 0;
		M2 = 0;
		value_step = 0.01;
	}
	
	void Serialize(Stream& s) {s % values % mean % M2 % value_step;}
	
	void AddResult(double value) {
		double delta = value - mean;
        mean += delta / values.GetCount();
        M2 += delta*(value - mean);
		
		int valuei = (int)(value / value_step);
		for(int i = 0; i < values.GetCount(); i++) {
			if (valuei < values[i]) {
				values.Insert(i, valuei);
				return;
			}
		}
		values.Add(valuei);
	}
	
	void Clear() {
		values.SetCount(0);
		mean = 0;
		M2 = 0;
	}
	
	double Get(double prob) {
		ASSERT(prob >= 0.0 && prob <= 1.0);
		ASSERT(values.GetCount());
		int count = values.GetCount() - 1;
		if (count < 0)
			return 0.0;
		double pos_step = 1.0 / (double)count;
		int pos = (int)(prob / pos_step);
		if (pos >= count)
			return values.Top() * value_step;
		if (pos < 0)
			return values[0] * value_step;
		double begin = pos * pos_step;
		double v0 = values[pos] * value_step;
		double v1 = values[pos+1] * value_step;
		double f = (prob - begin) / pos_step;
		double linear_value = v0 * f + v1 * (1.0 - f);
		return linear_value;
	}
	
	int GetCount() const {return values.GetCount();}
	
	double GetMean() {
		return mean;
	}
};


class MovingStepDistribution : Moveable<MovingStepDistribution> {
	Vector<StepDistribution> vec;
	int period;
	bool skip;
public:
	
	MovingStepDistribution() {
		skip = false;
		period = 10;
		vec.Add();
	}
	
	StepDistribution& Get() {return vec[0];}
	
	void SetPeriod(int i) {if (i > 0) {period = i; skip = false;} else {period = 1; skip = true; vec.SetCount(1);}}
	void AddResult(double d) {for(int i = 0; i < vec.GetCount(); i++) {vec[i].AddResult(d);}}
	void Next() {if (skip) return; while (vec.GetCount() >= period) {vec.Remove(0);} vec.Add();}
	void Clear() {vec.SetCount(1); vec[0].Clear();}
	
	double GetMean() {return Get().GetMean();}
	double Get(double prob) {return Get().Get(prob);}
	
	void Serialize(Stream& s) {s % vec % period % skip;}
	
};







class RandomGaussian {
	std::default_random_engine generator;
	std::normal_distribution<double> distribution;
	
public:

	// weight normalization is done to equalize the output
	// variance of every neuron, otherwise neurons with a lot
	// of incoming connections have outputs of larger variance
	RandomGaussian(int length) : distribution(0, sqrt((float)(1.0 / (double)(length)))) {
		generator.seed(Random(1000000000));
	}
	double Get() {return distribution(generator);}
	operator double() {return distribution(generator);}
	
};

template <class T> // Workaround for GCC bug - specialization needed...
T& SingleRandomGaussianLock() {
	static T o;
	return o;
}

inline RandomGaussian& GetRandomGaussian(int length) {
	SpinLock& lock = SingleRandomGaussianLock<SpinLock>(); // workaround
	ArrayMap<int, RandomGaussian>& rands = Single<ArrayMap<int, RandomGaussian> >();
	lock.Enter();
	int i = rands.Find(length);
	RandomGaussian* r;
	if (i == -1) {
		r = &rands.Add(length, new RandomGaussian(length));
	} else {
		r = &rands[i];
	}
	lock.Leave();
	return *r;
}

NAMESPACE_SDK_END

#endif
