#ifndef _Core_Time_h_
#define _Core_Time_h_

NAMESPACE_SDK_BEGIN


class Time {
	struct tm t;

public:
	Time() {}
	Time(const Time& t) { MemoryCopy((void*)& this->t, (void*)& t.t, sizeof(struct tm)); }
	Time(int y, int m, int d) {Set(y,m,d,0,0,0);}
	Time(int y, int m, int d, int h, int min, int s) {Set(y,m,d,h,min,s);}
	String ToString() const;

	void Set(int y, int m, int d, int h, int min, int s) {
		SetYear(y);
		month() = m;
		day() = d;
		hour() = h;
		minute() = min;
		second() = s;
	}
	struct tm& Std() { return t; }

	const int& second() const {return t.tm_sec;}
	const int& minute() const {return t.tm_min;}
	const int& hour() const {return t.tm_hour;}
	const int& day() const {return t.tm_mday;}
	const int& month() const {return t.tm_mon;}
	
	int& second() {return t.tm_sec;}
	int& minute() {return t.tm_min;}
	int& hour() {return t.tm_hour;}
	int& day() {return t.tm_mday;}
	int& month() {return t.tm_mon;}
	int  year() const {return 1900 + t.tm_year;}
	
	int64 Get() const;
	void Set(long t);
	void SetYear(int i) {t.tm_year = i - 1900;}
};

inline Time operator+(const Time& t, int64 seconds) {
	Time o;
	o.Set(t.Get() + seconds);
	return o;
}

Time GetSysTime();

String GetTimeString(Time t);
dword Timestamp(const Time& t);
dword TimestampNow();
Time TimeFromTimestamp(int64 seconds);
String TimeDiffString(int64 seconds);

bool IsLeapYear(int year);

class TimeStop {
	high_resolution_clock::time_point start;
	
public:
	
	TimeStop() {
		Reset();
	}
	
	void Reset() {
		start = high_resolution_clock::now();;
	}
	
	int Elapsed() const {
		return (int)(ElapsedSeconds() * 1000);
	}
	
	double ElapsedSeconds() const {
		std::chrono::high_resolution_clock::time_point stop = high_resolution_clock::now();
		duration<double> time_span = duration_cast<duration<double> >(stop - start);
		return time_span.count();
	}
	
	int ResetElapsed() {
		return (int)(ResetElapsedSeconds() * 1000);
	}
	
	double ResetElapsedSeconds() {
		std::chrono::high_resolution_clock::time_point stop = high_resolution_clock::now();
		duration<double> time_span = duration_cast<duration<double> >(stop - start);
		start = stop;
		return time_span.count();
	}
	
	String ToString() {
		#if 0
		String o;
		int s = Elapsed();
		int ms = s % 1000;
		s /= 1000;
		int m = s / 60;
		s %= 60;
		int h = m / 60;
		m %= 60;
		int d = h / 24;
		h %= 24;
		if (d)
			o << d << " days";
		if (d || h)
			o << h << " hours";
		if (d || h || m)
			o << m << " minutes";
		if (d || h || m || s)
			o << s << " seconds";
		if (d || h || m || s || ms)
			o << ms << " milliseconds";
		return o;
		#endif
		std::chrono::high_resolution_clock::time_point stop = high_resolution_clock::now();
		duration<double> time_span = duration_cast<duration<double> >(stop - start);
		return DblStr(time_span.count()) + " seconds";
	}
};




void  SetTimeCallback(int delay_ms, Callback cb, void *id = NULL); // delay_ms < 0 -> periodic
void  KillTimeCallback(void *id);
bool  ExistsTimeCallback(void *id);
dword GetTimeClick();
void AddGlobalTicks(uint32 ticks);
uint32 GetGlobalTicks();

inline void  PostCallback(Callback cb, void *id = NULL)  { SetTimeCallback(1, cb, id); }


class TimeCallback
{
public:
	~TimeCallback()                               { Kill(); (void)dummy; }

	void Set(int delay, Callback cb)     { SetTimeCallback(delay, cb, this); }
	void Post(Callback cb)               { PostCallback(cb, this); }
	void Kill()                                   { KillTimeCallback(this); }
	void KillSet(int delay, Callback cb) { Kill(); Set(delay, cb); }
	void KillPost(Callback cb)           { Kill(); Post(cb); }

private:
	byte dummy;
};





struct OnlineFrequency {
	TimeStop timer;
	int ticks = 0;
	
	OnlineFrequency() {}
	void Reset() {timer.Reset(); ticks = 0;}
	double Get() const {return (double)ticks / timer.Elapsed() * 1000;}
	void Tick() {ticks++;}
	
	operator double() const {return Get();}
	double operator++() {Tick(); return Get();}
};

struct OnlineFrequencyWindow {
	OnlineFrequency freq0, freq1;
	bool which = 0;
	double interval = 0.5;
	
	OnlineFrequencyWindow() {}
	OnlineFrequency& GetActive()   {return which ? freq1 : freq0;}
	OnlineFrequency& GetInactive() {return which ? freq0 : freq1;}
	void SwitchActive() {which = !which;}
	void Tick() {
		freq0.Tick();
		freq1.Tick();
		if (GetInactive().timer.Elapsed() >= interval) {
			SwitchActive();
			GetActive().Reset();
		}
	}
	double Get() const {return (which ? freq1 : freq0).Get();}
	
	operator double() const {return Get();}
	double operator++() {Tick(); return Get();}
};





String GetTimeDurationString(Time begin, Time end);

NAMESPACE_SDK_END

#endif
