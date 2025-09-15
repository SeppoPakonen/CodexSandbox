#ifndef _Core_Types_h_
#define _Core_Types_h_

NAMESPACE_SDK_BEGIN


class RunningFlagSingle {
	bool running = false, stopped = true;
	
public:
	RunningFlagSingle() {}
	bool IsRunning() const {return running;}
	void SetStopped() {stopped = true;}
	void SetNotRunning() {running = false;}
	void Start() {running = true; stopped = false;}
	void Stop();
	
};

struct RunningFlag {
	int sleep_time = 100;
	bool running = false;
	Atomic workers_running;
	
	RunningFlag() {workers_running = 0;}
	void Start(int count) {Stop(); running = true; workers_running = count;}
	void Stop();
	void SetNotRunning() {running = false;}
	void IncreaseRunning() {workers_running++;}
	void DecreaseRunning() {workers_running--; if (workers_running == 0) running = false;}
	bool IsRunning() const {return running;}
	void Wait();
};













template <class T, int I>
struct FixedArray {
	static const int size = I;
	
	T vector[I];
	
	
	T&       operator[](int i)       {ASSERT(i >= 0 && i < size); return vector[i];}
	const T& operator[](int i) const {ASSERT(i >= 0 && i < size); return vector[i];}
	void operator=(const T& value) {for(int i = 0; i < I; i++) this->vector[i] = value;}
};


template <int I, class K=int, class V=double>
struct TopValueSorter {
	static const int size = I;
	
	K key[size];
	V value[size];
	int count = 0;
	
	
	TopValueSorter() {Reset();}
	void Reset() {
		count = 0;
		for(int i = 0; i < size; i++) {
			value[i] = -DBL_MAX;
			key[i] = -1;
		}
	}
	void Add(const K& key, const V& value) {
		if (value <= this->value[size-1])
			return;
		for(int i = 0; i < size; i++) {
			if (value > this->value[i]) {
				for(int j = size-1; j > i; j--) {
					this->value[j] = this->value[j-1];
					this->key[j]   = this->key[j-1];
				}
				this->value[i] = value;
				this->key[i] = key;
				count = min(count+1, size);
				break;
			}
		}
	}
	void Serialize(Stream& s) {
		for(int i = 0; i < size; i++)
			s % value[i] % key[i];
	}
};














template <class T>
class ParentArray {
	
public:
	typedef ParentArray<T> PA;
	
private:
	PA* parent = NULL;
	Array<T> sub;
	
protected:
	
	T& Add() {T& t = sub.Add(); t.parent = this; return t;}
	T& Insert(int i) {T& t = sub.Insert(i); t.parent = this; return t;}
	
	void Copy(const PA& src) {sub <<= src.sub; for(int i = 0; i < sub.GetCount(); i++) sub[i].parent = this;}
	
public:
	
	T* GetParent() const {return parent;}
	T& Add(T* n) {ASSERT(n->parent == NULL); T& t = sub.Add(n); t.parent = this; return t;}
	T& Insert(int i, T* n) {ASSERT(n->parent == NULL); T& t = sub.Insert(i, n); t.parent = this; return t;}
	
	void Clear() {sub.Clear();}
	void Remove(int i) {sub.Remove(i);}
	void Remove(int i, int count) {sub.Remove(i, count);}
	void RemoveLast() {sub.Remove(sub.GetCount()-1);}
	T* Detach(int i) {T* t = sub.Detach(i); t->parent = NULL; return t;}
	
	T& At(int i) {return sub[i];}
	T& AtTop(int i) {if (i < sub.GetCount()) return sub[i]; return sub.Top();}
	T& At(int i, T& n) {if (i < sub.GetCount()) return sub[i]; return n;}
	const T& At(int i) const {return sub[i];}
	T& Top() {return sub.Top();}
	
	int GetCount() const {return sub.GetCount();}
	bool IsEmpty() const {return sub.IsEmpty();}
	
	void SortByInt() {
		struct Sorter {
			bool operator() (const T& a, const T& b) const {return a.GetInt() < b.GetInt();}
		};
		Sort(sub, Sorter());
	}
	
	void Serialize(Stream& s) {
		s % sub;
		if (s.IsLoading())
			for(int i = 0; i < sub.GetCount(); i++)
				sub[i].parent = this;
	}
};




struct FastSortedIndex {
	Vector<Vector<int>> data;
	Vector<uint64> value_mask;
	int count = 0;
	int first_data_i = INT_MAX;
	
	void Reserve(int count) {
		if (count > data.GetCount()) {
			data.SetCount(count);
			int value_count = count / 64 + 1;
			value_mask.SetCount(value_count, 0);
		}
	}
	
	void Clear() {
		for(int i = 0; i < data.GetCount(); i++)
			data[i].SetCount(0);
		for(int i = 0; i < value_mask.GetCount(); i++)
			value_mask[i] = 0;
		count = 0;
		first_data_i = INT_MAX;
	}
	
	void Add(int key, int value) {
		Vector<int>& key_vec = data[key];
		if (key_vec.IsEmpty()) {
			int mask_i = key / 64;
			int64 bit_i = key % 64;
			uint64& v = value_mask[mask_i];
			uint64 add = 1LL << bit_i;
			v |= add;
		}
		key_vec.Add(value);
		count++;
		if (key < first_data_i)
			first_data_i = key;
	}
	
	struct Iterator {
		Vector<Vector<int>>* data;
		Vector<uint64>* value_mask;
		int i, data_i, data_vec_i;
		int count;
		
		int* Ptr() {return &(*data)[data_i][data_vec_i];}
		int Value() {
			Vector<int>& v = (*data)[data_i];
			ASSERT(data_vec_i >= 0 && data_vec_i < v.GetCount());
			return v[data_vec_i];
		}
		void operator++(int __) {
			if (data_i >= 0 && data_i < data->GetCount() && data_vec_i+1 < (*data)[data_i].GetCount()) {
				data_vec_i++;
				i++;
				return;
			}
			else {
				data_i++;
				data_vec_i = 0;
				while (data_i < data->GetCount()) {
					int mask_i = data_i / 64;
					int64 bit_i = data_i % 64;
					uint64 m = (*value_mask)[mask_i];
					if (m == 0) {
						data_i = (mask_i+1) * 64;
						continue;
					}
					while (bit_i < 64) {
						if (m & (1LL << bit_i)) {
							i++;
							return;
						}
						bit_i++;
						data_i++;
					}
				}
			}
			i = count;
			data_i = data->GetCount();
			data_vec_i = -1;
		}
		void operator--(int __) {
			if (data_i >= 0 && data_i < data->GetCount() && data_vec_i-1 >= 0) {
				data_vec_i--;
				i--;
				return;
			}
			else {
				data_i--;
				data_vec_i = -1;
				while (data_i >= 0) {
					int mask_i = data_i / 64;
					int64 bit_i = data_i % 64;
					uint64& m = (*value_mask)[mask_i];
					if (m == 0) {
						data_i = (mask_i) * 64 - 1;
						continue;
					}
					while (bit_i >= 0) {
						if (m & (1LL << bit_i)) {
							i--;
							data_vec_i = (*data)[data_i].GetCount()-1;
							return;
						}
						bit_i--;
						data_i--;
					}
				}
			}
			i = -1;
			data_i = -1;
			data_vec_i = -1;
		}
		operator bool() const {return i >= 0 && i < count;}
	};
	
	Iterator Begin() {
		Iterator i;
		i.data = &data;
		i.value_mask = &value_mask;
		i.i = -1; i.data_i = -1; i.data_vec_i = -1; i.count = count;
		i++;
		return i;
	}
	
	Iterator Last() {
		Iterator i;
		i.data = &data;
		i.value_mask = &value_mask;
		i.i = count; i.data_i = data.GetCount(); i.data_vec_i = -1; i.count = count;
		i--;
		return i;
	}
};













// Destroying objects is costly and pointless during iterating
template <class T>
struct CachingVector : Moveable<CachingVector<T>> {
	typedef Vector<T> Vec;
	Vector<T> data;
	int count = 0;


	const T& operator[](int i) const {return data[i];}
	T& operator[](int i) {return data[i];}
	int GetCount() const {return count;}
	void SetCount(int i) {if (data.GetCount() < i) data.SetCount(i); count = i;}
	typename Vec::iterator begin() {return data.begin();}
	typename Vec::iterator end() {return data.begin() + count;}
};

NAMESPACE_SDK_END

#endif
