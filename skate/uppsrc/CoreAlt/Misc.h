#ifndef _Core_Misc_h_
#define _Core_Misc_h_

NAMESPACE_SDK_BEGIN


struct OrderedLinkedItem {
	typedef OrderedLinkedItem Item;
	
	Item* prev = NULL;
	Item* next = NULL;
	dword value = 0;
	dword key = 0;
	
	
};

/*
	OrderedLinkedList
	- can has many duplicate "key" values, but only single "value" value.
	- e.g.
		- "key" is some measured sorter value, where lower is better
		- "value" is unique identifier
		- by popping the begin pointer, you can process the unique identifier with best sorter
		  value
*/
struct OrderedLinkedList {
	typedef OrderedLinkedItem Item;
	
	Array<Item> items;
	Vector<Item*> begins;
	Vector<Item*> unused;
	Item* begin = NULL, *end = NULL;
	dword min = 0, max = 0, step = 0;
	Mutex lock;
	dword total_added = 0;
	dword count = 0;
	
	
	void Clear();
	void Zero();
	void Init(dword min, dword max, dword step);
	Item& Alloc();
	void Free(Item* t);
	int CountDistanceNotZero(Item* a, Item* b);
	bool IsOrdered() const;
	void InsertBetween(Item* it, Item* prev, Item* next);
	void RemoveBetween(Item* it);
	Item* FindWhereNext(dword key, Item* prev);
	Item* FindClosest(dword begin_i, dword key);
	dword GetBeginPos(dword key);
	void Add(dword key, dword value);
	void Remove(dword key, dword value);
	void Remove(Item* it);
	void RemoveUnsafe(Item* it);
	dword GetMinKey() const {return begin ? begin->key : 0;}
	dword GetMaxKey() const {return   end ?   end->key : 0;}
	bool PopBegin(dword& key, dword& value);
};

void OrderedLinkedListTest();












struct HugePersistent {
	
	// Persistent
	String filename;
	int64 data_size = 0;
	int64 persistent_size = 0;
	
	
	// Persistent in own file
	void* data = NULL;
	
	
	
	~HugePersistent() {Free();}
	void Alloc() {Free(); ASSERT(data_size > 0); data = MemoryAlloc(data_size);}
	void Free() {if (data) {MemoryFree(data); data = NULL;}}
	void StoreData();
	void LoadData();
	
	
	void Serialize(Stream& s) {
		s	% filename
			% data_size
			% persistent_size
			;
	}
};









class RecyclerBase {
	
public:
	virtual ~RecyclerBase() {}
	virtual void Clear() {}
	
};

template <class T>
class Recycler : public RecyclerBase {
	Array<T> arr;
	Vector<T*> used;
	Mutex arr_lock, used_lock;
	
	
	T* Used() {
		if (used.IsEmpty()) return NULL;
		
		T* e = NULL;
		used_lock.Enter();
		if (!used.IsEmpty())
			e = used.Pop();
		used_lock.Leave();
		
		Refurbish(*e);
		
		return e;
	}
	
	T& New() {
		arr_lock.Enter();
		T& e = arr.Add(NewOperator());
		arr_lock.Leave();
		return e;
	}
	
protected:
	virtual T* NewOperator() = 0;
	virtual void Refurbish(T& o) {}
	virtual void Cleanup(T& o) {}
	
public:
	
	virtual void Clear() {
		arr_lock.Enter();
		used_lock.Enter();
		
		arr.Clear();
		used.Clear();
		
		used_lock.Leave();
		arr_lock.Leave();
	}
	
	T& Create() {T* e = Used(); if (e) return *e; return New();}
	void Recycle(T& e) {
		Cleanup(e);
		used_lock.Enter();
		used.Add(&e);
		used_lock.Leave();
	}
	void Reserve(int i) {arr.Reserve(i);}
	
	Array<T>& GetArray() {return arr;}
	
};


NAMESPACE_SDK_END

#endif
