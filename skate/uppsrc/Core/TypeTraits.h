#ifndef _Core_TypeTraits_h_
#define _Core_TypeTraits_h_

NAMESPACE_SDK_BEGIN


inline String Demangle(const char* name) {
	#ifdef __GNUG__
	int status = -4; // some arbitrary value to eliminate the compiler warning

    // enable c++11 by passing the flag -std=c++11 to g++
    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(name, NULL, NULL, &status),
        std::free
    };

    return (status==0) ? res.get() : name ;
    #endif
    return name;
}

class TypeId : std::reference_wrapper<const std::type_info>, Moveable<TypeId>
{
    TypeId() = delete;

public:
    using reference_wrapper::reference_wrapper;

    uint32 GetHashValue() const { return (uint32)get().hash_code(); }
    char const* name() const { return get().name(); }
    String DemangledName() const {return Demangle(name());}

    bool operator==(TypeId const& other) const
    {
        return get() == other.get();
    }

    bool operator!=(TypeId const& other) const
    {
        return !(*this == other);
    }

    bool operator<(TypeId const& other) const
    {
        return get().before(other);
    }
};


template<typename T>
using UnorderedTypeMap = ArrayMap<TypeId, T>;

template<typename T>
using UnorderedTypeVectorMap = VectorMap<TypeId, T>;

template<typename T>
using TypeMap = ArrayMapOrdered<TypeId, T>;




template<typename T1, typename T2>
T1& AsRef(T2& val)
{
    static_assert(sizeof(T1) == sizeof(T2), "Sizes should be the same");
    return *reinterpret_cast<T1*>(&val);
}

template<typename T1, typename T2>
const T1& AsRef(const T2& val)
{
    static_assert(sizeof(T1) == sizeof(T2), "Sizes should be the same");
    return *reinterpret_cast<const T1*>(&val);
}

template<typename Container, typename Predicate>
void EraseIf(Container* container, Predicate&& predicate)
{
	for(int i = 0; i < container->GetCount(); i++) {
		if (predicate((*container)[i]))
			container->Remove(i--);
	}
    //container->Remove(RemoveIf(container->begin(), container->end(), Pick(predicate)), container->end());
}

class Destroyable
{
public:
    virtual ~Destroyable() = default;

    virtual void Destroy() { m_destroyed = true; }
    virtual bool IsDestroyed() const { return m_destroyed; }

    template<typename Container>
    static void PruneFromContainer(Container* container)
    {
        EraseIf(container, [](const auto& obj)
        {
            return obj->IsDestroyed();
        });
    }

protected:
    bool m_destroyed{ false };
};


class Enableable
{
public:
    virtual ~Enableable() = default;

    virtual void SetEnabled(bool enable) { m_enabled = enable; }
    virtual bool IsEnabled() const { return m_enabled; }

protected:
    bool m_enabled{ true };
};


template<typename T, typename ProducerT>
class Factory
{
public:
    using Type = T;
    using Producer = ProducerT;

    void RegisterProducer(const TypeId& typeId, Producer producer)
    {
        auto it = m_producers.find(typeId);

        AssertFalse(it != m_producers.end(), "multiple registrations for the same type is not allowed");

        m_producers.insert(it, { typeId, Pick<Producer>(producer) });
    }

protected:
    UnorderedTypeMap<ProducerT> m_producers;
};







template<bool...> 
struct bool_pack;

template<bool... Bs>
using AllTrue = std::is_same<bool_pack<Bs..., true>, bool_pack<true, Bs...>>;
////////////////////////////////////////

template <typename Base, typename... Ts>
using AllBaseOf = AllTrue<std::is_base_of<Base, Ts>::value...>;

template <typename To, typename... Ts>
using AllConvertibleTo = AllTrue<std::is_convertible<Ts, To>::value...>;

template <typename T, typename... Ts>
using AllSame = AllTrue<std::is_same<T, Ts>::value...>;


NAMESPACE_SDK_END

#endif
