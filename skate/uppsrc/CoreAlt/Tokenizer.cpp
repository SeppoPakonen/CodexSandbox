#include "Core.h"

NAMESPACE_SDK_BEGIN



Tokenizer::Tokenizer() {
	
}

Token& Tokenizer::Add(int token_id) {
	struct Token& t = tokens.Add();
	t.loc = loc;
	t.end = loc;
	t.end.col++;
	t.type = token_id;
	return t;
}

/*Token& Tokenizer::Insert(int i, int type) {
	Token& t = tokens.Insert(i);
	t.type = type;
	return t;
}*/

void Tokenizer::PassToken(int tk) {
	if (!IsToken(tk)) throw Exc("Unexpected token");
	pass_cursor++;
}

bool Tokenizer::IsToken(int tk) {
	if (pass_cursor >= tokens.GetCount()) return false;
	return tokens[pass_cursor].type == tk;
}

bool Tokenizer::TryPassToken(int tk) {
	if (!IsToken(tk)) return false;
	pass_cursor++;
	return true;
}

String Tokenizer::ReadString() {
	if (!IsToken(TK_STRING)) throw Exc("Unexpected token");
	return tokens[pass_cursor++].str_value;
}

String Tokenizer::ReadId() {
	if (!IsToken(TK_ID)) throw Exc("Unexpected token");
	return tokens[pass_cursor++].str_value;
}

int64 Tokenizer::ReadInt() {
	if (!IsToken(TK_INTEGER)) throw Exc("Unexpected token");
	return StrInt64(tokens[pass_cursor++].str_value);
}

double Tokenizer::ReadDouble() {
	if (!IsToken(TK_DOUBLE) && !IsToken(TK_FLOAT)) throw Exc("Unexpected token");
	return StrDbl(tokens[pass_cursor++].str_value);
}

bool Tokenizer::Load(String str, String path) {
	input = str;
	
	loc.file = GetCachedStringId(path);
	loc.line = 1;
	loc.col = 1;
	
	cursor = 0;
	
	while (cursor < input.GetCount()) {
		int chr = input[cursor];
		
		if (IsAlpha(chr) || chr == '_') {
			Token& t = Add(TK_ID);
			t.str_value.Cat(chr);
			Next();
			while (cursor < input.GetCount()) {
				int chr = input[cursor];
				if (IsAlpha(chr) || chr == '_' || IsDigit(chr)) {
					t.str_value.Cat(chr);
					Next();
				}
				else
					break;
			}
			t.end = loc;
		}
		else if (IsDigit(chr)) {
			String n;
			if (skip_separateunary && tokens.GetCount()) {
				while (tokens.GetCount() && tokens.Top().IsType(TK_MINUS)) {
					tokens.Pop();
					n.Cat('-');
				}
				while (tokens.GetCount() && tokens.Top().IsType(TK_PLUS)) {
					tokens.Pop();
				}
			}
			Token& tk = Add(TK_INTEGER);
			bool is_double = false, is_float = false;
			n.Cat(chr);
			Next();
			enum {INT, DEC, FRAC, SIGN, EXP, F, END, OCTHEX, OCT, HEX};
			int exp = chr == '0' ? OCTHEX : INT;
			while (cursor < input.GetCount()) {
				int chr = input[cursor];
				bool cat = false;
				
				if (exp == INT) {
					if (IsDigit(chr))
						cat = true;
					else if (chr == '.') {
						exp = FRAC;
						cat = true;
						is_double = true;
					}
					else if (chr == 'e' || chr == 'E') {
						exp = SIGN;
						cat = true;
						is_double = true;
					}
				}
				else if (exp == FRAC) {
					if (IsDigit(chr))
						cat = true;
					else if (chr == 'e' || chr == 'E') {
						exp = SIGN;
						cat = true;
						is_double = true;
					}
				}
				else if (exp == SIGN) {
					if (chr == '+' || chr == '-' || IsDigit(chr)) {
						exp = EXP;
						cat = true;
					}
					else if (chr == 'f' || chr == 'F') {
						exp = END;
						cat = true;
						is_float = true;
					}
				}
				else if (exp == EXP) {
					if (IsDigit(chr))
						cat = true;
					else if (chr == 'f' || chr == 'F') {
						exp = END;
						cat = true;
						is_float = true;
					}
				}
				else if (exp == OCTHEX) {
					if (chr == 'x' || chr == 'X') {
						exp = HEX;
						cat = true;
					}
					else if (chr >= '0' && chr <= '7') {
						exp = OCT;
						cat = true;
					}
					else if (chr == '.') {
						exp = FRAC;
						cat = true;
						is_double = true;
					}
				}
				else if (exp == HEX) {
					if ((chr >= '0' && chr <= '9') ||
						(chr >= 'a' && chr <= 'f') ||
						(chr >= 'A' && chr <= 'F')) {
						cat = true;
					}
				}
				else if (exp == OCT) {
					if (chr >= '0' && chr <= '7') {
						cat = true;
					}
				}
				
				if (cat) {
					Next();
					n.Cat(chr);
					if (exp == END) break;
				}
				else break;
			}
			
			tk.str_value = n;
			
			if (exp == HEX)
				tk.type = TK_HEX;
			else if (exp == OCT)
				tk.type = TK_OCT;
			else if (is_double) {
				if (!is_float)
					tk.type = TK_DOUBLE;
				else
					tk.type = TK_FLOAT;
			}
			
			tk.end = loc;
		}
		else if (chr == '!') {
			Token& tk = Add(TK_EXCLAMATION);
			Next();
			tk.end = loc;
		}
		else if (chr == '\"') {
			Token& tk = Add(TK_STRING);
			int begin_line = loc.line;
			int begin_col = loc.col;
			Next();
			String str;
			bool success = false;
			while (cursor < input.GetCount()) {
				int chr = input[cursor];
				String tmp;
				tmp.Cat(chr);
				if (chr == '\n') {
					AddError(tk.loc, "no newline allowed in string literal");
					Next();
					loc.line++;
					loc.col = 1;
				}
				else if (chr == '\"') {
					Next();
					success = true;
					break;
				}
				else if (chr == '\\') {
					AppendString(str);
				}
				else {
					str.Cat(chr);
					Next();
				}
			}
			tk.end = loc;
			
			if (success) {
				tk.str_value = str;
			}
			else {
				AddError(tk.loc, "no finishing \" in string literal");
				return false;
			}
		}
		else if (chr == '#') {
			Token& tk = Add(TK_NUMBERSIGN);
			Next();
			tk.end = loc;
		}
		else if (chr == '%') {
			Token& tk = Add(TK_PERCENT);
			Next();
			tk.end = loc;
		}
		else if (chr == '&') {
			Token& tk = Add(TK_AMPERSAND);
			Next();
			tk.end = loc;
		}
		else if (chr == '/') {
			int begin_line = loc.line, begin_col = loc.col;
			int chr1 = cursor+1 < input.GetCount() ? input[cursor+1] : 0;
			if (chr1 == '*') {
				Token* tk = NULL;
				if (!skip_comments)
					tk = &Add(TK_BLOCK_COMMENT);
				Next();
				Next();
				String c;
				bool succ = false;
				while (cursor < input.GetCount()) {
					chr = input[cursor];
					if (chr == '*') {
						chr1 = cursor+1 < input.GetCount() ? input[cursor+1] : 0;
						if (chr1 == '/') {
							Next();
							Next();
							if (tk) tk->str_value = c;
							succ = true;
							break;
						}
						else c.Cat(chr);
					}
					else c.Cat(chr);
					Next();
				}
				if (tk) {
					tk->end = loc;
					if (!succ)
						AddError(tk->loc, "unterminated /* comment");
				}
			}
			else if (chr1 == '/') {
				Token* tk = NULL;
				if (!skip_comments)
					tk = &Add(TK_COMMENT);
				Next();
				Next();
				String c;
				while (cursor < input.GetCount()) {
					chr = input[cursor];
					if (chr == '\n') {
						if (tk) tk->str_value = c;
						break;
					}
					else c.Cat(chr);
					Next();
				}
				if (tk) tk->end = loc;
			}
			else {
				Token& tk = Add(TK_DIV);
				Next();
				tk.end = loc;
			}
		}
		else if (chr == '{') {
			Add(TK_BRACKET_BEGIN);
			Next();
		}
		else if (chr == '}') {
			Add(TK_BRACKET_END);
			Next();
		}
		else if (chr == '(') {
			Add(TK_PARENTHESIS_BEGIN);
			Next();
		}
		else if (chr == ')') {
			Add(TK_PARENTHESIS_END);
			Next();
		}
		else if (chr == '[') {
			Add(TK_SQUARE_BEGIN);
			Next();
		}
		else if (chr == ']') {
			Add(TK_SQUARE_END);
			Next();
		}
		else if (chr == '=') {
			Add(TK_ASS);
			Next();
		}
		else if (chr == '+') {
			Add(TK_PLUS);
			Next();
		}
		else if (chr == '?') {
			Add(TK_QUESTION);
			Next();
		}
		else if (chr == '\\') {
			Add(TK_SOLIDUS);
			Next();
		}
		else if (chr == '^') {
			Add(TK_ACCENT);
			Next();
		}
		else if (chr == '~') {
			Add(TK_TILDE);
			Next();
		}
		else if (chr == '*') {
			Add(TK_MUL);
			Next();
		}
		else if (chr == '@') {
			Add(TK_AT);
			Next();
		}
		else if (chr == '\'') {
			Token& tk = Add(TK_CHAR);
			Next();
			String str;
			bool success = false;
			while (cursor < input.GetCount()) {
				int chr = input[cursor];
				if (chr == '\n') {
					AddError(tk.loc, "no newline allowed in char literal");
					Next();
					loc.line++;
					loc.col = 1;
				}
				else if (chr == '\'') {
					Next();
					success = true;
					break;
				}
				else if (chr == '\\') {
					AppendString(str);
				}
				else {
					str.Cat(chr);
					Next();
				}
			}
			tk.end = loc;
			
			if (success) {
				tk.str_value = str;
			}
			else {
				AddError(tk.loc, "no finishing \' in char literal");
				return false;
			}
		}
		else if (chr == '-') {
			Add(TK_MINUS);
			Next();
		}
		else if (chr == ';') {
			Add(TK_SEMICOLON);
			Next();
		}
		else if (chr == ',') {
			Add(TK_COMMA);
			Next();
		}
		else if (chr == '.') {
			Add(TK_PUNCT);
			Next();
		}
		else if (chr == ':') {
			Add(TK_COLON);
			Next();
		}
		else if (chr == '<') {
			Add(TK_LESS);
			Next();
		}
		else if (chr == '>') {
			Add(TK_GREATER);
			Next();
		}
		else if (chr == '|') {
			Add(TK_OR);
			Next();
		}
		else if (chr == '\n') {
			if (!skip_newlines) {
				Token& tk = Add(TK_NEWLINE);
				tk.end.col = 1; tk.end.line++;
			}
			Next();
			loc.line++;
			loc.col = 1;
		}
		else if (chr == '\t') {
			if (!tokens.IsEmpty())
				tokens.Top().spaces += tab_size;
			Next();
			loc.col += tab_size - 1;
		}
		else if (IsSpace(chr)) {
			if (!tokens.IsEmpty())
				tokens.Top().spaces++;
			Next();
		}
		else if (chr == 0)
			break;
		else {
			byte b = chr;
			if (b >= 0x80 && b <= 0xFF) {
				Next();
				while (cursor < input.GetCount()) {
					b = input[cursor];
					if (b >= 0x80 && b <= 0xFF)
						cursor++;
					else
						break;
				}
				//AddWarning("treating Unicode character as whitespace", line, col);
				if (!tokens.IsEmpty())
					tokens.Top().spaces++;
			}
			else {
				String msg = "unexpected character '";
				msg.Cat(chr);
				msg.Cat('\'');
				AddError(loc, msg);
				Next();
			}
		}
	}
	Add(TK_EOF);
	
	return messages.IsEmpty();
}

void Tokenizer::AppendString(String& s) {
	int chr = input[cursor];
	if (chr == '\\') {
		int chr1 = cursor+1 < input.GetCount() ? input[cursor+1] : 0;
		if (chr1 == '\'') {
			Next(); Next();
			s.Cat(0x27);
		}
		else if (chr1 == '\"') {
			Next(); Next();
			s.Cat(0x22);
		}
		else if (chr1 == '?') {
			Next(); Next();
			s.Cat(0x3F);
		}
		else if (chr1 == '\\') {
			Next(); Next();
			s.Cat(0x5c);
		}
		else if (chr1 == 'a') {
			Next(); Next();
			s.Cat(0x07);
		}
		else if (chr1 == 'b') {
			Next(); Next();
			s.Cat(0x08);
		}
		else if (chr1 == 'f') {
			Next(); Next();
			s.Cat(0x0c);
		}
		else if (chr1 == 'n') {
			Next(); Next();
			s.Cat(0x0a);
		}
		else if (chr1 == 'r') {
			Next(); Next();
			s.Cat(0x0d);
		}
		else if (chr1 == 't') {
			Next(); Next();
			s.Cat(0x09);
		}
		else if (chr1 == 'v') {
			Next(); Next();
			s.Cat(0x0b);
		}
		else {
			s.Cat(chr);
			Next();
		}
	}
	else {
		s.Cat(chr);
		Next();
	}
}
























void CParser::Load(const Vector<Token>& tokens) {
	this->tokens = &tokens;
	pos = 0;
}

const Token& CParser::GetCurrent() const {
	if (!tokens)
		throw Exc("No token vector set");
	if (pos < 0 || pos >= tokens->GetCount())
		throw Exc("Invalid token position");
	return (*tokens)[pos];
}

void CParser::IgnoreNewline(bool b) {
	ignore_newline = b;
}

bool CParser::IsEnd() const {
	return !tokens || pos >= tokens->GetCount();
}

FileLocation CParser::GetLocation() const {
	return GetCurrent().loc;
}

String CParser::ReadId() {
	const Token& tk = GetCurrent();
	if (!tk.IsType(TK_ID))
		throw Exc("Token is not id");
	pos++;
	return tk.str_value;
}

int CParser::ReadInt() {
	const Token& tk = GetCurrent();
	pos++;
	if (tk.IsType(TK_OCT))
		return OctInt(tk.str_value);
	if (tk.IsType(TK_HEX))
		return HexInt(tk.str_value);
	if (tk.IsType(TK_INTEGER))
		return StrInt(tk.str_value);
	pos--;
	throw Exc("Token is not int");
}

void CParser::PassNewline() {
	const Token& tk = GetCurrent();
	if (tk.IsType(TK_NEWLINE))
		throw Exc("Token is not newline");
	pos++;
}

bool CParser::Id(String id) {
	const Token& tk = GetCurrent();
	if (tk.IsType(TK_ID) && tk.str_value == id) {
		pos++;
		return true;
	}
	return false;
}

bool CParser::Type(int i) {
	const Token& tk = GetCurrent();
	if (tk.IsType(i)) {
		pos++;
		return true;
	}
	return false;
}

bool CParser::Newline() {
	const Token& tk = GetCurrent();
	if (tk.IsType(TK_NEWLINE)) {
		pos++;
		return true;
	}
	return false;
}

bool CParser::IsId() const {
	return GetCurrent().IsType(TK_ID);
}

bool CParser::IsInt() const {
	return GetCurrent().IsType(TK_INTEGER);
}

bool CParser::IsType(int i) const {
	return GetCurrent().IsType(i);
}


NAMESPACE_SDK_END
