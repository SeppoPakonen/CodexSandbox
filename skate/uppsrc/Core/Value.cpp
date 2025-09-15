#include "Core.h"

NAMESPACE_SDK_BEGIN

int RegisterTypeNo__(const char *type) {
	static Index<String> idx;
	return idx.FindAdd(type);
}



ValueMap& Value::CreateMap() {
	Create<ValueMap>();
	return GetMap();
}

ValueArray& Value::CreateArray() {
	Create<ValueArray>();
	return GetArray();
}

bool Value::IsArray() const {
	return Is<ValueArray>();
}

bool Value::IsMap() const {
	return Is<ValueMap>();
}

bool Value::IsArrayMapComb() const {
	return Is<ValueArrayMapComb>();
}

const ValueArray* Value::TryGetArray() const {
	if (IsArray())
		return &Get<ValueArray>();
	else if (IsArrayMapComb())
		return &Get<ValueArrayMapComb>().arr;
	else
		return NULL;
}

const ValueMap* Value::TryGetMap() const {
	if (IsMap())
		return &Get<ValueMap>();
	else if (IsArrayMapComb())
		return &Get<ValueArrayMapComb>().map;
	else
		return NULL;
}

ValueArray* Value::TryGetArray() {
	if (IsArray())
		return &Get<ValueArray>();
	else if (IsArrayMapComb())
		return &Get<ValueArrayMapComb>().arr;
	else
		return NULL;
}

ValueMap* Value::TryGetMap() {
	if (IsMap())
		return &Get<ValueMap>();
	else if (IsArrayMapComb())
		return &Get<ValueArrayMapComb>().map;
	else
		return NULL;
}

const ValueMap& Value::GetMap() const {
	if (!Is<ValueMap>())
		throw Exc("Not a ValueMap");
	return Get<ValueMap>();
}

const ValueArray& Value::GetArray() const {
	if (!Is<ValueArray>())
		throw Exc("Not a ValueArray");
	return Get<ValueArray>();
}

ValueMap& Value::GetMap() {
	if (!Is<ValueMap>())
		throw Exc("Not a ValueMap");
	return Get<ValueMap>();
}

ValueArray& Value::GetArray() {
	if (!Is<ValueArray>())
		throw Exc("Not a ValueArray");
	return Get<ValueArray>();
}

Value* Value::GetMapSub(String key, Value* def) {
	if (IsMap() || IsArrayMapComb()) {
		ValueMap& map = IsMap() ? Get<ValueMap>() : Get<ValueArrayMapComb>().map;
		int i = map.Find(key);
		if (i >= 0)
			return &map.At(i);
		else
			return NULL;
	}
	else return NULL;
}

Value* Value::AddMapSub(String key, Value* def) {
	if (IsMap() || IsArrayMapComb()) {
		ValueMap& map = IsMap() ? Get<ValueMap>() : Get<ValueArrayMapComb>().map;
		int i = map.Find(key);
		if (i >= 0)
			return NULL;
		else if (def)
			return &map.Add(key, *def);
		else
			return &map.Add(key);
	}
	else return NULL;
}

Value* Value::GetAddMapSub(String key, Value* def) {
	if (IsMap() || IsArrayMapComb()) {
		ValueMap& map = IsMap() ? Get<ValueMap>() : Get<ValueArrayMapComb>().map;
		int i = map.Find(key);
		if (i >= 0)
			return &map.At(i);
		else if (def)
			return &map.Add(key, *def);
		else
			return &map.Add(key);
	}
	else return NULL;
}

void Value::DeepCopyArrayMap(Value v) {
	if (v.IsArray())
		CreateArray().DeepCopyArrayMap(v.GetArray());
	else if (v.IsMap())
		CreateMap().DeepCopyArrayMap(v.GetMap());
	else *this = v;
}




void ValueArray::DeepCopyArrayMap(ValueArray& arr) {
	Clear();
	SetCount(arr.GetCount());
	for(int i = 0; i < GetCount(); i++) {
		Value& from = arr.At(i);
		Value& dst = At(i);
		dst.DeepCopyArrayMap(from);
	}
}




void ValueMap::DeepCopyArrayMap(ValueMap& map) {
	Clear();
	for(int i = 0; i < map.GetCount(); i++) {
		String key = map.GetKey(i);
		Value& from = map.At(i);
		Value& dst = Add(key);
		dst.DeepCopyArrayMap(from);
	}
}




void ValueArrayMapComb::DeepCopyArrayMap(ValueArrayMapComb& am) {
	Clear();
	arr.SetCount(am.arr.GetCount());
	for(int i = 0; i < arr.GetCount(); i++) {
		Value& from = am.arr.At(i);
		Value& dst = arr.At(i);
		dst.DeepCopyArrayMap(from);
	}
	for(int i = 0; i < am.map.GetCount(); i++) {
		String key = am.map.GetKey(i);
		Value& from = am.map.At(i);
		Value& dst = map.Add(key);
		dst.DeepCopyArrayMap(from);
	}
}




String GetValueTreeString(const Value& v, String key, int indent) {
	thread_local static Index<void*> visited;
	if (!indent)
		visited.Clear();
	
	String s;
	for(int i = 0; i < indent; i++)
		s.Cat('\t');
	s << key;
	
	void* obj = v.GetObject();
	if (obj) {
		if (visited.Find(obj) >= 0)
			return s + ": <recursive>\n";
		visited.Add(obj);
	}
	
	if (v.IsMap()) {
		s.Cat('\n');
		const ValueMap& map = v.Get<ValueMap>();
		for(int i = 0; i < map.GetCount(); i++) {
			String key = map.GetKey(i);
			const Value& v0 = map[i];
			s << GetValueTreeString(v0, key, indent+1);
		}
	}
	else if (v.IsArray()) {
		s.Cat('\n');
		const ValueArray& arr = v.Get<ValueArray>();
		for(int i = 0; i < arr.GetCount(); i++) {
			String key = IntStr(i);
			const Value& v0 = arr[i];
			s << GetValueTreeString(v0, key, indent+1);
		}
	}
	else if (v.IsArrayMapComb()) {
		s.Cat('\n');
		const ValueArrayMapComb& comb = v.Get<ValueArrayMapComb>();
		for(int i = 0; i < comb.map.GetCount(); i++) {
			String key = comb.map.GetKey(i);
			const Value& v0 = comb.map[i];
			s << GetValueTreeString(v0, key, indent+1);
		}
		for(int i = 0; i < comb.arr.GetCount(); i++) {
			String key = IntStr(i);
			const Value& v0 = comb.arr[i];
			s << GetValueTreeString(v0, key, indent+1);
		}
	}
	else {
		s << ": " << v.ToString() << "\n";
	}
	
	if (obj)
		visited.RemoveLast();
	
	return s;
}





String Format(String pattern, Value v0, Value v1, Value v2, Value v3, Value v4, Value v5, Value v6, Value v7) {
	static const int MAX_ARGS = 8;
	Value* v[MAX_ARGS] = {&v0, &v1, &v2, &v3, &v4, &v5, &v6, &v7};
	
	if (pattern.IsEmpty()) return "";
	
	int arg = 0;
	String out;
	const char* it  = pattern.Begin();
	const char* end = pattern.End();
	while (it != end) {
		int chr = *it++;
		
		if (chr == '\\' && *it == '%') {
			chr = *it++;
			out.Cat(chr);
		}
		else if (chr == '\%') {
			enum {
				MAIN_SWITCH
			};
			enum {
				TYPE_INVALID,
				TYPE_VALUE,
			};
			int state = MAIN_SWITCH;
			int type = TYPE_INVALID;
			while (it != end) {
				chr = *it++;
				if (state == MAIN_SWITCH) {
					if (chr == 'v') {
						type = TYPE_VALUE;
						break;
					}
					else break;
				}
				else break;
			}
			if (type == TYPE_INVALID)
				out << "<invalid>";
			else if (type == TYPE_VALUE) {
				if (arg < MAX_ARGS)
					out << v[arg++]->ToString();
				else
					out << "<arg overflow " << arg++ << ">";
			}
		}
		else {
			out.Cat(chr);
		}
	}
	return out;
}

NAMESPACE_SDK_END
