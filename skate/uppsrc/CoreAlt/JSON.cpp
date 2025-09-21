#include "Core.h"


NAMESPACE_SDK_BEGIN

Value ParseMap(Tokenizer& tk);



Value ParseArray(Tokenizer& tk) {
	ValueArray arr;
	
	tk.PassToken('[');
	
	while (!tk.IsToken(']')) {
		
		if (tk.IsToken(TK_BRACKET_BEGIN)) {
			arr.Add(ParseMap(tk));
		}
		else if (tk.IsToken(TK_SQUARE_BEGIN)) {
			arr.Add(ParseArray(tk));
		}
		else if (tk.IsToken(TK_ID)) {
			String value = tk.ReadId();
			if (value == "true")
				arr.Add(true);
			else if (value == "false")
				arr.Add(false);
			else
				arr.Add(value);
		}
		else if (tk.IsToken(TK_INTEGER)) {
			int64 value = tk.ReadInt();
			arr.Add(value);
		}
		else if (tk.IsToken(TK_FLOAT) || tk.IsToken(TK_DOUBLE)) {
			double value = tk.ReadDouble();
			arr.Add(value);
		}
		else if (tk.IsToken(TK_STRING)) {
			String value = tk.ReadString();
			arr.Add(value);
		}
		else {
			throw Exc("Unexpected token");
		}
		
		tk.TryPassToken(TK_COMMA);
	}
	
	tk.PassToken(']');
	
	return arr;
}

Value ParseMap(Tokenizer& tk) {
	ValueMap vm;
	
	tk.PassToken(TK_BRACKET_BEGIN);
	
	while (!tk.IsToken(TK_BRACKET_END)) {
		String key = tk.ReadString();
		
		tk.PassToken(TK_COLON);
		
		if (tk.IsToken(TK_BRACKET_BEGIN)) {
			vm.Add(key, ParseMap(tk));
		}
		else if (tk.IsToken(TK_SQUARE_BEGIN)) {
			vm.Add(key, ParseArray(tk));
		}
		else if (tk.IsToken(TK_ID)) {
			String value = tk.ReadId();
			if (value == "true")
				vm.Add(key, true);
			else if (value == "false")
				vm.Add(key, false);
			else
				vm.Add(key, value);
		}
		else if (tk.IsToken(TK_INTEGER)) {
			int64 value = tk.ReadInt();
			vm.Add(key, value);
		}
		else if (tk.IsToken(TK_FLOAT) || tk.IsToken(TK_DOUBLE)) {
			double value = tk.ReadDouble();
			vm.Add(key, value);
		}
		else if (tk.IsToken(TK_STRING)) {
			String value = tk.ReadString();
			vm.Add(key, value);
		}
		else {
			throw Exc("Unexpected token");
		}
		
		tk.TryPassToken(TK_COMMA);
	}
	
	tk.PassToken(TK_BRACKET_END);
	
	return vm;
}

Value ParseJSON(String json) {
	Value v;
	
	Tokenizer tk;
	tk.SkipComments();
	tk.SkipNewLines();
	tk.SkipSeparateUnary();
	
	if (!tk.Load(json, "json")) {
		LOG("Warning: Json tokenization failed");
		for(int i = 0; i < tk.messages.GetCount(); i++) {
			ProcMsg& e = tk.messages[i];
			LOG("\t" << i << ": " << e.line << ":" << e.col << ": " << e.msg);
		}
	}
	
	try {
		v = ParseMap(tk);
	}
	catch (Exc e) {
		tk.Dump();
		LOG("Error: Json parsing failed at " << tk.GetPassCursor() << ": " << e);
	}
	
	return v;
}


NAMESPACE_SDK_END
