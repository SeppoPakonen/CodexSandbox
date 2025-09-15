#ifndef _Core_Tuple_h_
#define _Core_Tuple_h_

NAMESPACE_SDK_BEGIN

template<std::size_t N, typename T, typename... types>
struct GetNthType
{
    using type = typename GetNthType<N - 1, types...>::type;
};

template<typename T, typename... types>
struct GetNthType<0, T, types...>
{
    using type = T;
};

template<std::size_t N, typename... Args>
using GetType = typename GetNthType<N, Args...>::type;





template<typename First, typename... Rest>
struct Tuple : Moveable<Tuple<First, Rest...>> {
	Tuple() {}
	Tuple(First first, Rest... rest): rest(rest...), first(first) {}
	Tuple(First first, Tuple<Rest...> rest): rest(rest), first(first) {}
	Tuple(const Tuple& t) {*this = t;}
	void operator=(const Tuple& t) {first = t.first; rest = t.rest;}
	
	First first;
	Tuple<Rest...> rest;
	
	First& operator->() {return first;}
	template <class T> T* Find() const {if (typeid(&first) == typeid(T)) return (T*)&first; if (typeid(first) == typeid(T)) return (T*)&first; return rest.template Find<T>();}
	template <class T> void ForEach(T fn) const {fn(first); rest.ForEach(fn);}
};

template<typename First>
struct Tuple<First> : Moveable<Tuple<First>> {
	Tuple() {}
	Tuple(First first): first(first) {}
	Tuple(const Tuple& t) {*this = t;}
	void operator=(const Tuple& t) {first = t.first;}
	
	First first;
	
	First& operator->() {return first;}
	template <class T> T* Find() const {if (typeid(&first) == typeid(T)) return (T*)&first; if (typeid(first) == typeid(T)) return (T*)&first; return NULL;}
	template <class T> void ForEach(T fn) const {fn(first);}
};

template<int index, typename First, typename... Rest>
struct GetImpl {
	static auto value(const Tuple<First, Rest...>* t) -> decltype(GetImpl < index - 1, Rest... >::value(&t->rest)) {
		return GetImpl < index - 1, Rest... >::value(&t->rest);
	}
};

template<typename First, typename... Rest>
struct GetImpl<0, First, Rest...> {
	static First value(const Tuple<First, Rest...>* t) {
		return t->first;
	}
};

template<int index, typename First, typename... Rest>
auto Get(const Tuple<First, Rest...>& t) -> decltype(GetImpl<index, First, Rest...>::value(&t)) { //typename Type<index, First, Rest...>::value {
	return GetImpl<index, First, Rest...>::value(&t);
}

template <class T, class A> T Get(const A& a) {
	T* ptr = a.template Find<T>();
	if (!ptr) throw Exc("Couldn't find type from tuple");
	return *ptr;
}


template <class ... T>
Tuple<T...> MakeTuple(T... args) {
	return Tuple<T...>(args...);
}



/*template <class First, class T, class R>
Tuple<First, T::> TupleCat(First f, T rest) {
	//return Tuple<First, decltype(rest.rest)>(f.first, rest.rest);
}*/

template <class A, class B>
struct RefPair : Moveable<RefPair<A,B>> {
	A& first;
	B& second;

	RefPair(A& a, B& b) : first(a), second(b) {}
	RefPair(const RefPair& r) : first(r.first), second(r.second) {}
	
	RefPair* operator->(){return this;} // hack to allow returning object from Iterator
};

NAMESPACE_SDK_END

#endif
