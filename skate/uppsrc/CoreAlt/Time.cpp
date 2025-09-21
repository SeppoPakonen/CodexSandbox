#include "Core.h"

NAMESPACE_SDK_BEGIN

static int s_month[] = {
	31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

static int s_month_off[] = {
	  0,  31,  59,  90, 120, 151,
	181, 212, 243, 273, 304, 334
};

bool IsLeapYear(int year) {
	return ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0);
}

/*int64 Time::Get() const {
	int year = t.tm_year;
	int month = t.tm_mon;
	int day = t.tm_mday;
	int hour = t.tm_hour;
	int minute = t.tm_min;
	int second = t.tm_sec;
	
	int y400 = (year / 400 ) - 2;
	int ym = year - y400 * 400;
	int64 ret = y400 * (400 * 365 + 100 - 3) +
	        ym * 365 + s_month_off[month - 1] + (day - 1) +
	       (ym - 1) / 4 - (ym - 1) / 100 + (ym - 1) / 400 + 1 +
	       (month > 2) * IsLeapYear(ym);
	ret = ret * (int64)24 * 3600 + hour * 3600 + minute * 60 + second;
	return ret;
}*/

int64 Time::Get() const {return mktime((struct tm*)&t);}
void Time::Set(long t) {MemoryCopy(&this->t, localtime(&t), sizeof(struct tm));}











String GetTimeString(Time t) {
	String min = t.minute() < 10 ? "0" + IntStr(t.minute()) : IntStr(t.minute());
	return Format("%v.%v.%v %v:%v", (int)t.day(), (int)t.month(), (int)t.year(), (int)t.hour(), min);
}


dword Timestamp(const Time& t) {
	return (dword)(t.Get() - Time(1970,1,1).Get());
}

dword TimestampNow() {
	return (dword)(GetSysTime().Get() - Time(1970,1,1).Get());
}

Time TimeFromTimestamp(int64 seconds) {
	return Time(1970, 1, 1) + seconds;
}

String TimeDiffString(int64 seconds) {
	String out;
	
	if (seconds > -60 && seconds < 60) return "Now";
	
	if (seconds < 0)		{out += "-"; seconds *= -1;}
	else if (seconds > 0)	out += "+";
	
	int64 years, months, days, hours, minutes;
	#define DIV(x, y) x = seconds / (y); seconds = seconds % (y);
	DIV(years, 365*24*60*60);
	DIV(months, 30*24*60*60);
	DIV(days,    24*60*60);
	DIV(hours,      60*60);
	DIV(minutes,       60);
	#undef DIV
	#undef PRINT
	#define PRINT(x) if (x) out += " " + IntStr64(x) + " " #x;
	PRINT(years);
	PRINT(months);
	PRINT(days);
	PRINT(hours);
	PRINT(minutes);
	#undef PRINT
	return out;
}











uint32 global_ticks;

struct TimeCallbackItem : Moveable<TimeCallbackItem> {
	void* id;
	Callback cb;
	uint32 ticks;
	uint32 delay;
	bool is_periodical;
	byte epoch;
};

Array<TimeCallbackItem> __time_cbs;
Mutex __time_cbs_lock;

void SetTimeCallback(int delay_ms, Callback cb, void *id) {
	bool is_periodical = false;
	if (delay_ms < 0) {
		delay_ms *= -1;
		is_periodical = true;
	}
	
	for(int i = 0; i < __time_cbs.GetCount(); i++) {
		TimeCallbackItem& it = __time_cbs[i];
		if (it.id == id) {
			it.cb = cb;
			it.ticks = global_ticks + delay_ms;
			it.delay = delay_ms;
			it.is_periodical = is_periodical;
			it.epoch = it.ticks < global_ticks ? 1 : 0;
			return;
		}
	}
	
	
	__time_cbs_lock.Enter();
	
	TimeCallbackItem& it = __time_cbs.Add();
	it.id = id;
	it.cb = cb;
	it.ticks = global_ticks + delay_ms;
	it.delay = delay_ms;
	it.is_periodical = is_periodical;
	it.epoch = it.ticks < global_ticks ? 1 : 0;
	
	__time_cbs_lock.Leave();
}

void KillTimeCallback(void *id) {
	for(int i = 0; i < __time_cbs.GetCount(); i++) {
		TimeCallbackItem& it = __time_cbs[i];
		if (it.id == id) {
			__time_cbs_lock.Enter();
			__time_cbs.Remove(i);
			__time_cbs_lock.Leave();
			break;
		}
	}
}

bool ExistsTimeCallback(void *id) {
	for(int i = 0; i < __time_cbs.GetCount(); i++) {
		TimeCallbackItem& it = __time_cbs[i];
		if (it.id == id) {
			return true;
		}
	}
	return false;
}

void ExecuteTimeCallback() {
	Vector<int> remlist;
	
	for(int i = 0; i < __time_cbs.GetCount(); i++) {
		TimeCallbackItem& it = __time_cbs[i];
		if (it.epoch == 0 && it.ticks <= global_ticks) {
			it.cb.Execute();
			if (it.is_periodical) {
				it.ticks = global_ticks + it.delay;
				it.epoch = it.ticks < global_ticks ? 1 : 0;
			}
			else
				remlist.Add(i);
		}
	}
	// TODO remove vector of positions
	
	
	__time_cbs_lock.Enter();
	__time_cbs.Remove(remlist);
	__time_cbs_lock.Leave();
}

void DecreaseTimeCallbackEpoch() {
	__time_cbs_lock.Enter();
	
	for(int i = 0; i < __time_cbs.GetCount(); i++) {
		TimeCallbackItem& it = __time_cbs[i];
		if (it.epoch > 0)
			it.epoch--;
		else
			__time_cbs.Remove(i--);
	}
	
	__time_cbs_lock.Leave();
}

void AddGlobalTicks(uint32 diff) {
	uint32 prev_global_ticks = global_ticks;
	global_ticks += diff;
	
	// overflow
	if (global_ticks < prev_global_ticks)
		DecreaseTimeCallbackEpoch();
	
	ExecuteTimeCallback();
}

uint32 GetGlobalTicks() {return global_ticks;}

NAMESPACE_SDK_END
