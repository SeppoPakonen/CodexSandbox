#ifndef _Core_Vector_h_
#define _Core_Vector_h_

NAMESPACE_SDK_BEGIN

template <int size>
struct Data_S_ : Moveable< Data_S_<size> > {
	byte filler[size];
};





template <class T>
class Pick {
	T* var = NULL;
public:
	Pick(T& var) : var(&var) {}
	Pick(Pick&& p) : var(p.var) {p.var = NULL;}
	Pick(const Pick& p) : var(p.var) {}
	T& Get() const {return *var;}
};

template <class T>
Pick<T> pick(T& o) {return Pick<T>(o);}

template <class T>
void Swap(T& a, T& b) {
	uint8 tmp[sizeof(T)];
	MemoryCopy(tmp, &a, sizeof(T));
	MemoryCopy(&a, &b, sizeof(T));
	MemoryCopy(&b, tmp, sizeof(T));
}

template <class T>
inline void Reverse(T& t) {
	int count = t.GetCount();
	int count_2 = count / 2;
	int a = 0, b = count - 1;
	for (int i = 0; i < count_2; i++) {
		Swap(t[a++], t[b--]);
	}
}

template <class T>
class One {
	T* obj = NULL;

public:
	One() {}
	One(One&& o) {obj = o.obj; o.obj = NULL;}
	One(T* obj) : obj(obj) {}
	~One() { Clear(); }

	void Create() { Clear(); obj = new T(); }
	void Clear() { if (obj) { delete obj; obj = NULL; } }
	void operator=(T* obj) { Clear(); this->obj = obj; }
	bool Is() const { return obj != NULL; }
	bool IsEmpty() const { return obj == NULL; }
	T* operator->() {ASSERT(obj); return obj;}
	const T* operator->() const {ASSERT(obj); return obj;}
	T& operator*() {ASSERT(obj); return *obj;}
	const T& operator*() const {ASSERT(obj); return *obj;}
	T* Get() {return obj;}
	const T* Get() const {return obj;}
	T& Value() {return *obj;}
	T* Detach() {ASSERT(obj); T* tmp = obj; obj = NULL; return tmp;}
	operator bool() const {return !IsEmpty();}
	bool operator!=(void* ptr) {return ptr != obj;}
	operator T&() {ASSERT(obj); return *obj;}
	operator const T&() const {ASSERT(obj); return *obj;}
	
};


class NullOpt {};

template <class T>
class Optional {
	T* obj = NULL;

public:
	Optional() {}
	Optional(Optional&& o) {obj = o.obj; o.obj = NULL;}
	Optional(const Optional& opt) {if (opt.obj) this->obj = new T(*opt.obj);}
	Optional(const T& obj) {this->obj = new T(obj);}
	Optional(const Nuller& n) {}
	//Optional(const std::nullopt_t& n) {}
	Optional(const NullOpt& n) {}
	Optional(T* obj) : obj(obj) {}
	Optional(const Pick<T>& n) {*this = n;}
	template <class K> Optional(const K& arg) {this->obj = new T(arg);}
	~Optional() { Clear(); }

	void Create() { Clear(); obj = new T(); }
	void Clear() { if (obj) { delete obj; obj = NULL; } }
	void operator=(const NullOpt& obj) { Clear();}
	void operator=(const T& obj) { Clear(); this->obj = new T(obj); }
	void operator=(const Optional& o) { Clear(); if (!o.IsEmpty()) {this->obj = new T(*o.obj); }}
	void operator=(const Pick<Optional<T>>& n) { Clear(); if (!n.Get().IsEmpty()) {Pick(n.Get());}}
	void Pick(Optional& o) {obj = o.obj; o.obj = NULL;}
	bool IsEmpty() const { return obj == NULL; }
	T& GetValue() const {return *obj;}
	T* operator->() {ASSERT(obj); return obj;}
	operator bool () const {return obj != NULL;}
	T& operator*() {return *obj;}
};

template <class T>
Optional<T> MakeOptional(T value) {Optional<T> o; o = value; return o;}

/*template <class T>
class Unique {
	static One<T> obj;

public:
	Unique() {}
	Unique(Unique&& o) {obj = o.obj;}
	~Unique() { Clear(); }

	void Create() {if (obj.IsEmpty()) obj.Create();}
	void Clear() { obj.Clear(); }
	void Reset() {Clear(); Create();}
	bool IsEmpty() const { return obj.IsEmpty(); }
	T* operator->() {return obj.Get();}
};*/



template <class K>
class Vector : Moveable<Vector<K>> {
	K* data = NULL;
	int reserved = 0;
	int count = 0;


	void IncreaseReserved() {
		uint64 new_reserved = 1;
		while (new_reserved <= reserved)
			new_reserved <<= 1;
		if (new_reserved >= INT_MAX)
			new_reserved = INT_MAX;
		Reserve((int)new_reserved);
	}
	
	void Pick0(Vector& v) { data = v.data; v.data = 0; reserved = v.reserved; v.reserved = 0; count = v.count; v.count = 0; }
	
public:
	template <int I>
	struct Iterator0 {
		K* kit = NULL;
		Iterator0() {}
		Iterator0(K* kit) : kit(kit) {}
		Iterator0(const Iterator0& it) {*this = it;}
		void operator=(const Iterator0& it) {kit = it.kit;}
		Iterator0& operator++() {kit += I; return *this;}
		Iterator0 operator++(int i) {Iterator ret = *this; kit += I; return ret;}
		void operator--(int i) {kit -= I;}
		void operator+=(int i) {kit += i;}
		K* operator->() const {return kit;}
		K* Get() const {return kit;}
		operator K*() const {return kit;}
		K& operator*() const {return *kit;}
		bool operator!=(const Iterator0& it) const {return it.kit != kit;}
		K& operator()() const {return *kit;}
	};
	typedef Iterator0<+1> Iterator;
	typedef Iterator0<-1> RIterator;

	Vector(std::initializer_list<K> l) {
		Reserve(l.size());
		auto cur = l.begin();
		auto end = l.end();
		while (cur != end) {
			new (&data[count++]) K(*cur++);
		}
	}
	Vector() {}
	Vector(Vector&& v) {Pick0(v);}
	Vector(const Pick<Vector>& pick) {Vector& v = pick.Get(); Swap(data, v.data); Swap(reserved, v.reserved); Swap(count, v.count);}
	Vector(const Vector& v) {*this <<= v;}
	Vector(int i) {SetCount(i);}
	~Vector() {
		Clear();
		return;
		AssertMoveable((K*)0);  // T must be moveable
	}
	
	void Serialize(Stream& s);
	void operator=(const Pick<Vector>& pick) {Clear(); Vector& v = pick.Get(); Swap(data, v.data); Swap(reserved, v.reserved); Swap(count, v.count);}
	void operator=(Vector&& v) {Pick0(v);}
	
	
	Iterator Begin() {return Iterator(data);}
	Iterator End() {return Iterator(data+count);}
	const Iterator Begin() const {return Iterator(data);}
	const Iterator End() const {return Iterator(data+count);}
	Iterator begin() {return Begin();}
	Iterator end() {return End();}
	const Iterator begin() const {return Begin();}
	const Iterator end() const {return End();}
	RIterator RBegin() {return RIterator(data+count-1);}
	RIterator REnd() {return RIterator(data-1);}
	const RIterator RBegin() const {return RIterator(data+count-1);}
	const RIterator REnd() const {return RIterator(data-1);}
	RIterator rbegin() {return RBegin();}
	RIterator rend() {return REnd();}
	const RIterator rbegin() const {return RBegin();}
	const RIterator rend() const {return REnd();}
	K* Get() const {return data;}
	
	K& Add() {
		if (count + 1 > reserved) IncreaseReserved();
		if (count >= reserved)
			throw MemoryLimitExc("Vector maximum size exceeded");
		new (&data[count]) K();
		return data[count++];
	}
	K& Add(const K& v) {
		if (count + 1 > reserved) IncreaseReserved();
		if (count >= reserved)
			throw MemoryLimitExc("Vector maximum size exceeded");
		new (&data[count]) K(v);
		return data[count++];
	}
	K& AddPick(const Pick<K>& v) {
		if (count + 1 > reserved) IncreaseReserved();
		if (count >= reserved)
			throw MemoryLimitExc("Vector maximum size exceeded");
		new (&data[count]) K(v);
		return data[count++];
	}
	Vector& operator<<(const K& v) { Add(v); return *this; }

	void SetCount(int new_count) {
		ASSERT(new_count >= 0);
		if (new_count < 0) return;
		if (new_count > reserved) Reserve(new_count);
		if (new_count == count) return;
		if (new_count > count) {
			for (int i = count; i < new_count; i++)
				new (&data[i]) K();
		}
		else {
			for (int i = count - 1; i >= new_count; i--)
				data[i].~K();
		}
		count = new_count;
	}
	void SetCount(int new_count, const K& value) {
		ASSERT(new_count >= 0);
		if (new_count < 0) return;
		if (new_count > reserved) Reserve(new_count);
		if (new_count == count) return;
		if (new_count > count) {
			for (int i = count; i < new_count; i++)
				new (&data[i]) K(value);
		}
		else {
			for (int i = count - 1; i >= new_count; i--)
				data[i].~K();
		}
		count = new_count;
	}
	void operator <<=(const Vector& a) {
		SetCount(0);
		Reserve(a.GetCount());
		for (int i = 0; i < a.GetCount(); i++)
			new (&data[i]) K(a[i]);
		count = a.GetCount();
	}
	void Append(const Vector& a) {
		int begin = count;
		int new_count = count + a.GetCount();
		Reserve(new_count);
		for (int i = begin, j = 0; i < new_count; i++, j++)
			new (&data[i]) K(a[j]);
		count = new_count;
	}
	void IncreaseReserve(int more_reserved) {Reserve(GetCount() + more_reserved);}
	void Reserve(int new_reserved) {
		if (new_reserved <= reserved)
			return;
		K* new_data = (K*)malloc(new_reserved * sizeof(K));
		if (data) {
			if (count > 0)
				MemoryCopy((void*)new_data, (void*)data, sizeof(K) * count);
			free(data);
		}
		data = new_data;
		reserved = new_reserved;
	}
	int GetCount() const { return count; }
	int GetReserved() const { return reserved; }
	bool IsEmpty() const { return GetCount() == 0; }

	K& operator[](int i) {
		ASSERT(i >= 0 && i < count);
		return *(data + i);
	}
	const K& operator[](int i) const {
		ASSERT(i >= 0 && i < count);
		return *(data + i);
	}

	void Remove(int i) {
		ASSERT(i >= 0 && i < count);
		int tail = count - (i + 1);
		if (tail > 0)
			MemoryMove(data + i, data + i + 1, tail * sizeof(K));
		count--;
	}
	void Remove(int i, int count) {
		ASSERT(count > 0);
		ASSERT(i >= 0 && i + count <= this->count);
		int tail = this->count - (i + count);
		if (tail > 0)
			MemoryMove(data + i, data + i + count, tail * sizeof(K));
		this->count -= count;
	}
	void Remove(const Iterator& it) {
		K* begin = data;
		K* cur = it.Get();
		int pos = cur - begin;
		Remove(pos);
		// TODO: Check this
	}
	void Remove(const Iterator& it, const Iterator& end) {
		K* begin = data;
		K* cur = it.Get();
		K* cur_end = end.Get();
		int pos0 = cur - begin;
		int pos1 = cur_end - begin;
		int len = pos1 - pos0;
		if (!len) return;
		Remove(pos0, len);
		// TODO: Check this
	}
	void Remove(const Vector<int>& sorted_list) {Remove(sorted_list.Begin(), sorted_list.GetCount());}
	void Remove(const int* sorted_list, int n) {
		if(!n) return;
		int pos = *sorted_list;
		int npos = pos;
		for(;;) {
			ASSERT(pos < count);
			if(pos == *sorted_list) {
				(data + pos)->~K();
				pos++;
				sorted_list++;
				if(--n == 0) break;
				ASSERT(*sorted_list >= pos);
			}
			else
				*((Data_S_<sizeof(K)>*)data + npos++)
					= *((Data_S_<sizeof(K)>*)data + pos++);
		}
		while(pos < count)
			*((Data_S_<sizeof(K)>*)data + npos++) = *((Data_S_<sizeof(K)>*)data + pos++);
		count = npos;
	}
	K& Insert(int i) {
		if (count + 1 > reserved) IncreaseReserved();
		if (count >= reserved)
			throw MemoryLimitExc("Vector maximum size exceeded");
		int tail = this->count - i;
		if (tail > 0)
			MemoryMove(data + i + 1, data + i, tail * sizeof(K));
		new (&data[i]) K();
		count++;
		return data[i];
	}
	K& Insert(int i, const K& key) {
		if (count + 1 > reserved) IncreaseReserved();
		if (count >= reserved)
			throw MemoryLimitExc("Vector maximum size exceeded");
		int tail = this->count - i;
		if (tail > 0)
			MemoryMove(data + i + 1, data + i, tail * sizeof(K));
		new (&data[i]) K(key);
		count++;
		return data[i];
	}

	K& Top() { ASSERT(GetCount() > 0); return data[count - 1]; }

	K Pop() {
		ASSERT(GetCount() > 0);
		int i = count - 1;
		K ret(data[i]);
		data[i].~K();
		count--;
		return ret;
	}
	
	void RemoveLast() {
		ASSERT(GetCount() > 0);
		data[--count].~K();
	}

	void Clear() {
		if (!reserved) return;
		K* it = data;
		K* end = data + count;
		while (it != end)
			it++->~K();
		count = 0;
		free(data);
		data = NULL;
		reserved = 0;
	}
};

template <class K>
class Array {
	typedef Vector<K*> VectorK;
	VectorK l;

public:
	struct Iterator {
		typename Vector<K*>::Iterator kit;
		Iterator() {}
		Iterator(typename Vector<K*>::Iterator kit) : kit(kit) {}
		Iterator(const Iterator& it) {*this = it;}
		void operator=(const Iterator& it) {kit = it.kit;}
		void operator++() {kit++;}
		void operator++(int i) {kit++;}
		void operator--(int i) {kit--;}
		void operator+=(int i) {kit += i;}
		K* operator->() const {return kit.Get();}
		K* Get() const {return *kit.Get();}
		bool operator!=(const Iterator& it) const {return it.kit != kit;}
		operator K*() const {return Get();}
		K& operator()() const {return *Get();}
	};

	Array() {}
	Array(std::initializer_list<K> list) {
		auto it = list.begin();
		auto end = list.end();
		while (it != end)
			Add(*it++);
	}
	~Array() { Clear(); }

	Iterator Begin() {return Iterator(l.Begin());}
	Iterator End() {return Iterator(l.End());}
	const Iterator Begin() const {return Iterator(l.Begin());}
	const Iterator End() const {return Iterator(l.End());}
	Iterator begin() {return Begin();}
	Iterator end() {return End();}
	const Iterator begin() const {return Begin();}
	const Iterator end() const {return End();}
	
	void Reserve(int i) {l.Reserve(i);}
	void SetCount(int new_count) {
		int count = l.GetCount();
		if (new_count == count) return;
		if (new_count > count) {
			l.SetCount(new_count);
			for (int i = count; i < new_count; i++)
				l[i] = new K();
		}
		else {
			for (int i = count - 1; i >= new_count; i--)
				delete l[i];
			l.SetCount(new_count);
		}
	}
	K& Add() { K* k = new K(); l.Add(k); return *k; }
	K& Add(const K& v) { K* k = new K(v); l.Add(k); return *k; }
	K& Add(K* k) { l.Add(k); return *k; }
	K& Insert(int i) {return *l.Insert(i, new K());}
	K& Insert(int i, const K& key) {return *l.Insert(i, new K(key));}
	int GetCount() const { return l.GetCount(); }
	bool IsEmpty() const { return GetCount() == 0; }
	K* Detach(int i) {K* o = l[i]; l.Remove(i); return o;}

	K& operator[](int i) {
		ASSERT(i >= 0 && i < l.GetCount());
		K** it = l.Get();
		it = it + i;
		return **it;
	}
	const K& operator[](int i) const {
		ASSERT(i >= 0 && i < l.GetCount());
		return **(l.Get() + i);
	}

	void Remove(int i) {
		ASSERT(i >= 0 && i < l.GetCount());
		delete *(l.Get() + i);
		l.Remove(i);
	}
	void Remove(const Vector<int>& sorted_list) {Remove(sorted_list.Begin(), sorted_list.GetCount());}
	void Remove(const int* sorted_list, int n) {
		if(!n) return;
		const int* it  = sorted_list;
		const int* end = sorted_list + n;
		K** vector = l.Get();
		while (it != end)
			(*(vector + *it++))->~K();
		l.Remove(sorted_list, n);
	}
	void RemoveLast() {ASSERT(GetCount()); Remove(GetCount()-1);}
	void Clear() {
		K** ptr = l.Get();
		K** end = ptr + l.GetCount();
		while (ptr != end)
			delete *(ptr++);
		l.Clear();
	}

	K& Top() {
		ASSERT(GetCount() > 0);
		return **(l.Get() + l.GetCount() - 1);
	}
	void operator <<=(const Array& a) {
		Clear();
		l.SetCount(a.GetCount(), 0);
		for (int i = 0; i < a.GetCount(); i++)
			l[i] = new K(*a.l[i]);
	}
};

template <class K>
class Index : Moveable<Index<K>> {
	Vector<uint32> hashes;
	Vector<K> values;

public:
	struct Iterator {
		typename Vector<K>::Iterator kit;
		Iterator() {}
		Iterator(typename Vector<K>::Iterator kit) : kit(kit) {}
		Iterator(const Iterator& it) {*this = it;}
		void operator=(const Iterator& it) {kit = it.kit;}
		void operator++() {kit++;}
		void operator--() {kit--;}
		void operator++(int i) {kit++;}
		void operator--(int i) {kit--;}
		void operator+=(int i) {kit += i;}
		K* operator->()const  {return kit;}
		K* Get() const {return kit.Get();}
		bool operator!=(const Iterator& it) const {return kit != it.kit;}
		K& operator*() const {return *Get();}
		operator K*() const {return Get();}
	};
	
	Index() {}
	Index(std::initializer_list<K> l) {
		hashes.Reserve(l.size());
		values.Reserve(l.size());
		auto cur = l.begin();
		auto end = l.end();
		while (cur != end) {
			uint32 hash = GetHashValue(*cur);
			if (FindHash(hash) >= 0)
				throw Exc("Index has value already");
			hashes.Add(hash);
			values.Add(*cur++);
		}
	}

	Iterator Begin() {return Iterator(values.Begin());}
	Iterator begin() {return Iterator(values.Begin());}
	Iterator End() {return Iterator(values.End());}
	Iterator end() {return Iterator(values.End());}
	const Iterator Begin() const {return Iterator(values.Begin());}
	const Iterator End() const {return Iterator(values.End());}
	K* Get() {return values.Get();}
	
	const K& operator[](int i) const {
		ASSERT(i >= 0 && i < values.GetCount());
		return values[i];
	}

	int GetCount() const { return values.GetCount(); }
	const K& Add(const K& value) {
		uint32 hash = GetHashValue(value);
		if (FindHash(hash) >= 0)
			throw Exc("Index has value already");
		hashes.Add(hash);
		return values.Add(value);
	}
	K& Insert(int i, const K& value) {
		uint32 hash = GetHashValue(value);
		if (FindHash(hash) >= 0)
			throw Exc("Index has value already");
		hashes.Insert(i, hash);
		return values.Insert(i, value);
	}
	int Find(const K& key) const { return FindHash(GetHashValue(key)); }
	int FindAdd(const K& key) {int i = Find(key); if (i >= 0) return i; i = GetCount(); Add(key); return i;}

	int FindHash(uint32 hash) const {
		for (int i = 0; i < hashes.GetCount(); i++) {
			if (hashes[i] == hash)
				return i;
		}
		return -1;
	}

	void Clear() { hashes.Clear(); values.Clear(); }
	void Remove(int i) { hashes.Remove(i); values.Remove(i); }
	void RemoveLast() {ASSERT(GetCount()); if (GetCount()) Remove(GetCount()-1);}
	
	
	void operator <<=(const Index& a) {
		hashes <<= a.hashes;
		values <<= a.values;
	}
};

template <class K, class V, template<typename> class Array>
class Map : Moveable<Map<K,V,Array>> {
	Index<K> keys;
	Array<V> values;

public:
	struct Iterator {
		typedef RefPair<K, V> Ret;
		struct Index<K>::Iterator kit;
		struct Array<V>::Iterator vit;
		Iterator() {}
		Iterator(struct Index<K>::Iterator kit, struct Array<V>::Iterator vit) : kit(kit), vit(vit) {}
		Iterator(const Iterator& it) {*this = it;}
		void operator=(const Iterator& it) {kit = it.kit; vit = it.vit;}
		void operator++() {kit++; vit++;}
		void operator++(int i) {kit++; vit++;}
		void operator--(int i) {kit--; vit--;}
		void operator+=(int i) {kit += i; vit += i;}
		Ret operator->() {return Get();}
		Ret Get() const {return Ret(*kit.Get(), *vit.Get());}
		V& GetValue() const {return Get().second;}
		bool operator!=(const Iterator& it) const {return vit != it.vit;}
		bool operator==(const Iterator& it) const {return !(vit != it.vit);}
		Ret operator()() {return Ret(*kit.Get(), *vit.Get());}
		Ret operator*() const {return Ret(*kit.Get(), *vit.Get());}
	};
	
	Map() {}
	Map(Map&& m) {Swap(keys, m.keys); Swap(values, m.values);}
	Map(const Pick<Map<K, V, Array>>& p) {auto& a = p.Get(); Swap(keys, a.keys); Swap(values, a.values);}
	Map(const std::initializer_list<std::tuple<K, V>> list) {
		auto it = list.begin();
		auto end = list.end();
		while (it != end) {Add(std::get<0>(*it), std::get<1>(*it)); it++;}
	}
	~Map() { Clear(); }
	
	const Index<K>& GetKeys() const {return keys;}
	const Array<V>& GetValues() const {return values;}
	Array<V>& GetValues() {return values;}
	
	V& Add(const K& key) {
		keys.Add(key);
		return values.Add();
	}

	V& Add(const K& key, const V& value) {
		keys.Add(key);
		return values.Add(value);
	}
	
	V& Add(const K& key, V* value) {
		keys.Add(key);
		return values.Add(value);
	}
	
	V& GetAdd(const K& key) {
		int i = keys.Find(key);
		if (i >= 0) return values[i];
		keys.Add(key);
		return values.Add();
	}

	Iterator InsertIterator(const Iterator& it, const K& key, const V& value) {int pos = GetPos(it); Insert(pos, key, value); Iterator cur = Begin(); cur += pos; return cur;}
	Iterator InsertIterator(const Iterator& it, const K& key, V* value) {int pos = GetPos(it); Insert(pos, key, value); Iterator cur = Begin(); cur += pos; return cur;}
	Iterator InsertIterator(const Iterator& it, const K& key) {int pos = GetPos(it); Insert(pos, key); Iterator cur = Begin(); cur += pos; return cur;}
	
	V& Insert(int i, const K& key) {keys.Insert(i, key); return values.Insert(i);}
	V& Insert(int i, const K& key, const V& value) {keys.Insert(i, key); return values.Insert(i, value);}
	
	const K& GetKey(int i) const { return keys[i]; }

	const V& At(int i) const {
		ASSERT(i >= 0 && i < values.GetCount());
		return values[i];
	}

	V& At(int i) {
		ASSERT(i >= 0 && i < values.GetCount());
		return values[i];
	}
	
	const V& operator[](int i) const {return At(i);}
	V& operator[](int i) {return At(i);}
	
	V& Top() { ASSERT(GetCount() > 0); return values.Top(); }

	int Find(const K& key) const { return keys.Find(key); }
	Iterator FindIterator(const K& key) const { Iterator it = Begin(); int pos = Find(key); it += pos == -1 ? GetCount() : pos; return it;}
	V& Get(const K& key) { return values[Find(key)]; }
	V  Get(const K& key, V value) {int i = Find(key); if (i < 0) return value; return values[i]; }
	int GetCount() const { return keys.GetCount(); }
	bool IsEmpty() const { return GetCount() == 0; }

	int GetPos(const Iterator& it) {
		K* begin = keys.Get();
		K* cur = &it.Get().first;
		int pos = cur - begin;
		// TODO: Check this
		return pos;
	}
	void RemoveKey(const K& key) {int i = Find(key); if (i >= 0) Remove(i);}
	void Remove(int i) { ASSERT(i >= 0 && i < keys.GetCount()); keys.Remove(i); values.Remove(i); }
	void Remove(const Iterator& it) {Remove(GetPos(it));}
	void Clear() {
		keys.Clear();
		values.Clear();
	}
	
	Iterator Begin() {return Iterator(keys.Begin(), values.Begin());}
	Iterator End() {return Iterator(keys.End(), values.End());}
	const Iterator Begin() const {return Iterator(keys.Begin(), values.Begin());}
	const Iterator End() const {return Iterator(keys.End(), values.End());}
	Iterator begin() {return Begin();}
	Iterator end() {return End();}
	const Iterator begin() const {return Begin();}
	const Iterator end() const {return End();}
	
	
	void operator <<=(const Map& a) {
		keys <<= a.keys;
		values <<= a.values;
	}
};

template <class K, class V> using VectorMap = Map<K, V, Vector>;
template <class K, class V> using ArrayMap = Map<K, V, Array>;

/*template <class K, class V>
class ArrayMapOrdered {
	
};*/
template <class K, class V> using ArrayMapOrdered = ArrayMap<K, V>;

Vector<String> Split(String to_split, String split_str, bool ignore_empty=true);
String Join(const Vector<String>& v, String join_str, bool ignore_empty=true);


template<class InputIterator, class UnaryPredicate>
InputIterator FindIf(InputIterator first, InputIterator last, UnaryPredicate pred) {
	while (first != last) {
		if (pred(*first))
			return first;
			
		++first;
	}
	
	return last;
}


template <class T, class UnaryPredicate>
T RemoveIf(T first, T last, UnaryPredicate p) {
	first = FindIf(first, last, p);
    if (first != last)
        for(T i = first; ++i != last; )
            if (!p(*i))
                *first++ = Pick<T>(*i);
    return first;
}







// Never moveable
template <class T>
class Slot {
	T* ptr = NULL;
	
public:
	
	
	virtual ~Slot() {Clear();}
	
	bool IsEmpty() const {return ptr == NULL;}
	void Set(T* ptr) {
		ASSERT(ptr && ptr->IsAttached(this));
		Clear();
		this->ptr = ptr;
	}
	void Clear() {
		if (ptr) {
			T* p = ptr;
			ptr = NULL;
			p->Detach(this);
		}
	}
	void __ZeroPtr() {ptr = NULL;}
	
	T* operator->() const {return ptr;}
	operator bool() const {return ptr != NULL;}
	T& Get() const {return *ptr;}
	T* GetPtr() const {return ptr;}
	
	T& operator*() const {return *ptr;}
	
};

// Never moveable
template <class T>
class Attachable {
	typedef Slot<T> SlotT;
	
	Vector<SlotT*> slots;
	
	
public:
	
	virtual ~Attachable() {DetachAll();}
	
	void Attach(SlotT& s) {
		ASSERT(s.IsEmpty());
		ASSERT(!IsAttached(&s));
		static_assert(std::is_base_of<Attachable<T>, T>(), "The class T must inherit Attachable<T>");
		//T* o = reinterpret_cast<T*>(this);
		T* o = dynamic_cast<T*>(this);
		ASSERT(o);
		slots.Add(&s);
		s.Set(o);
	}
	void DetachAll() {
		for (SlotT* s : slots)
			s->__ZeroPtr();
		slots.Clear();
	}
	bool IsAttached(SlotT* ptr) {
		for(SlotT* p : slots)
			if (p == ptr)
				return true;
		return false;
	}
	void Detach(SlotT* ptr) {
		int rem_count = 0;
		for(int i = 0; i < slots.GetCount(); i++) {
			SlotT* s = slots[i];
			if (s == ptr) {
				slots.Remove(i--);
				rem_count++;
			}
		}
		ASSERT(rem_count == 1);
	}
	int GetCount() const {return slots.GetCount();}
	SlotT& GetSlot(int i) const {return *slots[i];}
	
	
	
};







struct WeakBase {
	virtual void SetDeleted() = 0;
};

template <class T> class Weak;

struct RefBase {
	void* obj = NULL;
	Vector<WeakBase*> weaks;
	AtomicInt refs;
	RefBase() {refs = 1;}
	virtual ~RefBase() {}
	void Inc() {refs++;}
	void Dec() {
		refs--;
		if (refs <= 0) {
			for(int i = 0; i < weaks.GetCount(); i++)weaks[i]->SetDeleted();
			delete this;
		}
	}
	void IncWeak(WeakBase* w) {weaks.Add(w);}
	void DecWeak(WeakBase* w) {for(int i = 0; i < weaks.GetCount(); i++) if (weaks[i] == w) {weaks.Remove(i--);}}
};

template <class T>
struct RefTemplate : public RefBase {
	~RefTemplate() {if (obj) delete ((T*)obj); obj = NULL;}
};

template <class T>
class Shared : Moveable<Shared<T>> {
	
protected:
	friend class Weak<T>;
	RefBase* r = NULL;
	T* o = NULL;

public:
	Shared() {}
	Shared(Shared&& s) {r = s.r; s.r = NULL; o = s.o; s.o = NULL;}
	Shared(const Shared& o) {*this = o;}
	Shared(const Pick<Shared<T>>& pick) {Swap(pick.Get(), *this);}
	~Shared() { Clear(); }
	
	void Create() { Clear(); r = new RefTemplate<T>(); o = new T(); r->obj = o;}
	void Clear() { if (r) { r->Dec(); r = NULL; o = NULL;} }
	void operator=(const Shared<T>& s) {if (r == s.r) return; SetPtr(s.o, s.r);}
	bool IsEmpty() const { return r == NULL; }
	T* operator->() {if (r) return o; return NULL;}
	T* operator->() const {if (r) return o; return NULL;}
	T* Get() const {if (r) return o; return NULL;}
	operator bool() const {return !IsEmpty();}
	T& operator*() const {return *Get();}
	bool operator==(const T* ptr) const {if (r) return o == ptr; return false;}
	bool operator==(const Shared& s) const {if (r && s.r) return o == s.o; return false;}
	template <class K> Shared<K> As() {
		static_assert(std::is_base_of<T, K>() || std::is_base_of<K, T>(), "K -> T or T -> K inheritance is required");
		
		if (o) {
			K* ptr = dynamic_cast<K*>(o);
			if (ptr) {
				Shared<K> s;
				s.SetPtr(ptr, r);
				return s;
			}
		}
		return Shared<K>();
	}
	void SetPtr(T* o, RefBase* r) {
		Shared<T> tmp; Swap(*this, tmp); // don't unref until new ref!!!111!1
		this->o = o;
		this->r = r;
		if (r) r->Inc();
	}
	Shared& WrapObject(T* obj) {
		Clear();
		if (obj) {
			r = new RefTemplate<T>();
			r->obj = obj;
			o = obj;
		}
		return *this;
	}
	const RefBase* GetBase() const {return r;}
};

void TestShared();


template <class T> inline Shared<T> MakeShared() {return Shared<T>(new T());}
template <class T, class P> inline Shared<T> MakeSharedBase() {Shared<P> s; s.Create(); return s.template As<T>();}

//template <class T, class A0, class A1, class A2> inline Shared<T> MakeShared(A0& a0, A1& a1, A2& a2) {return Shared<T>(new T(a0, a1, a2));}
template <class T, class A0> inline Shared<T> MakeShared(A0 a0) {return Shared<T>(new T(a0));}
template <class T, class A0, class A1> inline Shared<T> MakeShared(A0 a0, A1 a1) {return Shared<T>(new T(a0, a1));}
template <class T, class A0, class A1, class A2> inline Shared<T> MakeShared(A0 a0, A1 a1, A2 a2) {return Shared<T>(new T(a0, a1, a2));}
template <class T, class A0, class A1, class A2, class A3> inline Shared<T> MakeShared(A0 a0, A1 a1, A2 a2, A3 a3) {return Shared<T>(new T(a0, a1, a2, a3));}
template <class T, class A0, class A1, class A2, class A3, class A4> inline Shared<T> MakeShared(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4) {return Shared<T>(new T(a0, a1, a2, a3, a4));}
template <class T, class A0, class A1, class A2, class A3, class A4, class A5> inline Shared<T> MakeShared(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5) {return Shared<T>(new T(a0, a1, a2, a3, a4, a5));}
template <class T, class A0, class A1, class A2, class A3, class A4, class A5, class A6> inline Shared<T> MakeShared(A0 a0, A1 a1, A2 a2, A3 a3, A4 a4, A5 a5, A6 a6) {return Shared<T>(new T(a0, a1, a2, a3, a4, a5, a6));}



template <class T>
class Weak : public WeakBase {
	struct RefBase* r = NULL;
	T* o = NULL;
	
public:
	Weak() {}
	Weak(const Weak& w) : r(w.r), o(w.o) {if (r) r->IncWeak(this);}
	Weak(const Shared<T>& s) : r(s.r), o(s.o) {if (r) r->IncWeak(this);}
	~Weak() {Clear();}
	
	virtual void SetDeleted() {r = NULL;}
	void Clear() {if (r) r->DecWeak(this); r = NULL; o = NULL;}
	void operator=(const Shared<T>& s) { Clear(); r = s.r; o = s.o; if (r) r->IncWeak(this);}
	void operator=(Pick<Weak<T>>& p) { Clear(); r = p.Get().r; o = p.Get().o; if (r) r->IncWeak(this);}
	void operator=(const Weak<T>& p) { Clear(); r = p.r; o = p.o; if (r) r->IncWeak(this);}
	bool IsEmpty() const { return r == NULL; }
	T* operator->() {return o;}
	T* Get() {return o;}
	operator bool() {return !IsEmpty();}
	Shared<T> Enter() const {Shared<T> s; s.r = r; s.o = o; if (s.r) s.r->Inc(); return s;}
};


template <class T>
class EnableSharedFromThis {
	Weak<T> weak;
	
public:
	virtual ~EnableSharedFromThis() = default;
	
	void InitWeak(const Shared<T>& s) {ASSERT(weak.IsEmpty()); weak = s;}
	
	bool HasWeak() const {return !weak.IsEmpty();}
	
	Shared<T> GetSharedFromThis() {
		/*if (weak.IsEmpty()) {
			Shared<T> s(dynamic_cast<T*>(this));
			weak = s;
			return s;
		}*/
		//else {
		ASSERT(!weak.IsEmpty());
		return weak.Enter();
		//}
	}
	
	template <class V>
	Shared<V> AsShared() {return GetSharedFromThis().template As<V>();}
};

template <class T, class I>
Shared<T> StaticPointerCast(I it) {
	
}



NAMESPACE_SDK_END

#endif
