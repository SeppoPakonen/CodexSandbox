#ifndef _Core_Value_h_
#define _Core_Value_h_

NAMESPACE_SDK_BEGIN


class Value;


int RegisterTypeNo__(const char *type);

template <class T>
int StaticTypeNo() {
	static int typeno = RegisterTypeNo__(typeid(T).name());
	return typeno;
}


const dword VOID_V    = 0;

const dword INT_V     = 1;
const dword DOUBLE_V  = 2;
const dword STRING_V  = 3;
//const dword DATE_V    = 4;
const dword TIME_V    = 5;

const dword ERROR_V   = 6;

const dword VALUE_V   = 7;

const dword WSTRING_V = 8;

const dword VALUEARRAY_V = 9;

const dword INT64_V  = 10;
const dword BOOL_V   = 11;

const dword VALUEMAP_V   = 12;

const dword VALUE_ARRAY_AND_MAP_V   = 13;

const dword UNKNOWN_V = (dword)0xffffffff;

template <class T>
inline dword ValueTypeNo(const T *)                 { return StaticTypeNo<T>() + 0x8000000; }

template<> inline dword ValueTypeNo(const void*)    { return VOID_V; }
template<> inline dword ValueTypeNo(const int*)     { return INT_V; }
template<> inline dword ValueTypeNo(const int64*)   { return INT64_V; }
template<> inline dword ValueTypeNo(const double*)  { return DOUBLE_V; }
template<> inline dword ValueTypeNo(const bool*)    { return BOOL_V; }
template<> inline dword ValueTypeNo(const String*)  { return STRING_V; }
template<> inline dword ValueTypeNo(const WString*) { return WSTRING_V; }
//template<> inline dword ValueTypeNo(const Date*)    { return DATE_V; }
template<> inline dword ValueTypeNo(const Time*)    { return TIME_V; }
template<> inline dword ValueTypeNo(const Value*)   { return VALUE_V; }



class ValueObject {
protected:
	dword type = VOID_V;
	
public:
	virtual ~ValueObject() = default;
	virtual void* Get() {return 0;}
	virtual String ToString() const {return "VOID";}
	virtual int64 ToInt() const {return 0;}
	dword GetType() const {return type;}
};

template <class T>
class ValueTemplate : public ValueObject {
	bool is_owned = false;
	T* ptr = NULL;
	
public:
	ValueTemplate() {ptr = new T(); is_owned = true; type = ValueTypeNo<T>(0);}
	ValueTemplate(T* ptr) : ptr(ptr) {type = ValueTypeNo<T>(0);}
	ValueTemplate(const T& obj) {ptr = new T(obj); is_owned = true; type = ValueTypeNo<T>(0);}
	~ValueTemplate() {if (is_owned && ptr) {delete ptr; ptr = 0; is_owned = false; type = VOID_V;}}
	virtual void* Get() {return ptr;}
	virtual String ToString() const {if (ptr) return ::SDK::ToString(*ptr); return "NULL";}
	virtual int64 ToInt() const {if (ptr) return ::SDK::ToInt(*ptr); return 0;}
};

class ValueMap;
class ValueArray;

class Value : Moveable<Value> {
	Shared<ValueObject> obj;
	
public:
	
	Value() {}
	Value(const Value& v) {*this = v;}
	Value(const char* str) {String& s = Create<String>(); s = str;}
	template <class T> Value(const T& o) {obj.WrapObject(new ValueTemplate<T>(o));}
	
	void Clear() {obj.Clear();}
	template <class T> T& Create() {ValueTemplate<T>* o = new ValueTemplate<T>(); T* t = (T*)o->Get(); obj.WrapObject(o); return *t;}
	template <class T> T& Create(const T& arg) {ValueTemplate<T>* o = new ValueTemplate<T>(arg); T* t = (T*)o->Get(); obj.WrapObject(o); return *t;}
	void DeepCopyArrayMap(Value v);
	ValueMap&   CreateMap();
	ValueArray& CreateArray();
	
	bool operator==(const Value& v) const {return obj == v.obj;}
	
	dword GetType() const {if (obj) return obj->GetType(); return VOID_V;}
	template <class T> Value&	operator=(const Value& v) {obj = v.obj; return *this;}
	template <class T> Value&	operator=(T* o) {Set<T>(o); return *this;}
	template <class T> Value&	operator=(const T& o) {Set<T>(o); return *this;}
	
	template <class T> void     Set(T* o) {obj = new ValueTemplate<T>(o);}
	template <class T> void     Set(const T& o) {obj.WrapObject(new ValueTemplate<T>(o));}
	template <class T> bool		Is() const {if (obj) return obj->GetType() == ValueTypeNo<T>(0); return false;}
	template <class T> T*		Try() const;
	
	template <class T> operator T&() const;
	template <class T> T& Get() const;
	const ValueMap* TryGetMap() const;
	const ValueArray* TryGetArray() const;
	ValueMap* TryGetMap();
	ValueArray* TryGetArray();
	const ValueMap& GetMap() const;
	const ValueArray& GetArray() const;
	ValueMap& GetMap();
	ValueArray& GetArray();
	Value* GetMapSub(String key, Value* def=NULL);
	Value* AddMapSub(String key, Value* def=NULL);
	Value* GetAddMapSub(String key, Value* def=NULL);
	ValueObject* GetObject() const {return obj.Get();}
	
	bool IsVoid() const {if (obj) return obj->GetType() == VOID_V; return true;}
	bool IsArray() const;
	bool IsMap() const;
	bool IsArrayMapComb() const;
	bool IsString() const {return !obj.IsEmpty() && obj->GetType() == STRING_V;}
	bool IsInt() const {return !obj.IsEmpty() && obj->GetType() == INT_V;}
	bool IsInt64() const {return !obj.IsEmpty() && obj->GetType() == INT64_V;}
	
	String ToString() const {if (obj) return obj->ToString(); return "NULL";}
	int64 ToInt() const {if (obj) return obj->ToInt(); return 0;}
};

template <class T> T* Value::Try() const {
	if (obj && obj->GetType() == ValueTypeNo<T>(0))
		return (T*)obj->Get();
	return NULL;
}

template <class T> Value::operator T&() const {
	if (obj && obj->GetType() == ValueTypeNo<T>(0))
		return *(T*)obj->Get();
	throw Exc("Unexpected value type");
}

template <class T> T& Value::Get() const {
	if (obj && obj->GetType() == ValueTypeNo<T>(0))
		return *(T*)obj->Get();
	throw Exc("Unexpected value type");
}

class ValueArray {
	typedef Array<Value> A;
	A values;
	
public:
	
	ValueArray() {}
	ValueArray(const ValueArray& m) {*this = m;}
	void operator=(const ValueArray& m) {values <<= m.values;}
	
	void Clear() {values.Clear();}
	Value& Add() {return values.Add();}
	Value& Add(const Value& v) {return values.Add(v);}
	Value& Insert(int i) {return values.Insert(i);}
	Value& Insert(int i, const Value& v) {return values.Insert(i, v);}
	int GetCount() const {return values.GetCount();}
	Value& operator[](int i) {return values[i];}
	Value& At(int i) {return values[i];}
	const Value& operator[](int i) const {return values[i];}
	const Value& At(int i) const {return values[i];}
	bool IsEmpty() const {return values.IsEmpty();}
	void SetCount(int i) {values.SetCount(i);}
	void DeepCopyArrayMap(ValueArray& arr);
	
	String ToString() const {String s; s << "ValueArray(" << values.GetCount() << ")"; return s;}
	int64 ToInt() const {return values.GetCount();}
	
	A::Iterator begin() {return values.begin();}
	A::Iterator end() {return values.end();}
	const A::Iterator begin() const {return values.begin();}
	const A::Iterator end() const {return values.end();}
};


class ValueMap {
	Vector<String> keys;
	Array<Value> values;
	
public:
	
	ValueMap() {}
	ValueMap(const ValueMap& m) {*this = m;}
	void operator=(const ValueMap& m) {keys <<= m.keys; values <<= m.values;}
	
	Value& Add(String key, Value v) {keys.Add(key); return values.Add(v);}
	Value& Add(String key) {keys.Add(key); return values.Add();}
	Value& GetAdd(String key) {int i = Find(key); if (i >= 0) return values[i]; return Add(key);}
	Value& GetAdd(String key, const Value& def) {int i = Find(key); if (i >= 0) return values[i]; return Add(key, def);}
	Value TryGet(String key, Value def=Value()) {int i = Find(key); if (i >= 0) return At(i); else return def;}
	Value* TryFind(String key) {int i = Find(key); if (i >= 0) return &At(i); return NULL;}
	int Find(String key) const {for(int i = 0; i < keys.GetCount(); i++) if (keys[i] == key) return i; return -1;}
	Value& Get(String key) {int i = Find(key); if (i == -1) throw Exc("Unexpected key"); return values[i];}
	int GetPos(Value* v) const {for(int i = 0; i < values.GetCount(); i++) if (&values[i] == v) return i; return -1;}
	
	void Clear() {keys.Clear(); values.Clear();}
	int GetCount() const {return values.GetCount();}
	Value& operator[](int i) {return values[i];}
	Value& At(int i) {return values[i];}
	const Value& operator[](int i) const {return values[i];}
	const Value& At(int i) const {return values[i];}
	String GetKey(int i) const {return keys[i];}
	void DeepCopyArrayMap(ValueMap& map);
	
	String ToString() const {String s; s << "ValueMap(" << keys.GetCount() << ")"; return s;}
	int64 ToInt() const {return keys.GetCount();}
};

struct ValueArrayMapComb {
	ValueArray arr;
	ValueMap   map;
	
	
	ValueArrayMapComb() {}
	ValueArrayMapComb(const ValueArrayMapComb& v) {*this = v;}
	void operator=(const ValueArrayMapComb& v) {arr = v.arr; map = v.map;}
	void DeepCopyArrayMap(ValueArrayMapComb& am);
	
	void Clear() {arr.Clear(); map.Clear();}
	int GetTotal() const {return arr.GetCount() + map.GetCount();}
	String ToString() const {String s; s << "ValueArrayMapComb(" << arr.GetCount() << ", " << map.GetCount() << ")"; return s;}
	int64 ToInt() const {return GetTotal();}
};


template<> inline dword ValueTypeNo(const ValueArray*)          { return VALUEARRAY_V; }
template<> inline dword ValueTypeNo(const ValueMap*)            { return VALUEMAP_V; }
template<> inline dword ValueTypeNo(const ValueArrayMapComb*)   { return VALUE_ARRAY_AND_MAP_V; }

String GetValueTreeString(const Value& v, String key="", int indent=0);


String Format(String pattern, Value v0=Value(), Value v1=Value(), Value v2=Value(), Value v3=Value(), Value v4=Value(), Value v5=Value(), Value v6=Value(), Value v7=Value());


NAMESPACE_SDK_END

#endif
