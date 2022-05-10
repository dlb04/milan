#include "lexer.h"
#include <iostream>

namespace milan {

	std::ostream& operator<<(std::ostream& os, const Token& tk) {
		os << "<\"" << tk._repr << "\"," << Token::kind_as_str(tk._kind) << '>';
		return os;
	}

	static inline bool is_blank(char ch) {
		return ch == ' ' || ch == '\n' || ch == '\t' || ch == '\v';
	}

	Token TokenStream::read_alphanumeric() {
		assert(isalpha(chr()));

		size_t start{ _current_char };
		while (can_read() && isalnum(chr()) || chr() == '_')
			advance();

		size_t len{ _current_char - start };

		Token tk{_src.substr(start, len),
			TokenKind::Identifier, {} };

		tk.identifier_or_keyword();
		return tk;
	}

	Token TokenStream::read_number() {
		size_t start{ _current_char };
		while (can_read() && isdigit(chr()))
			advance();

		size_t len{ _current_char - start };
		return Token(_src.substr(start, len), TokenKind::Int, {});
	}

	void TokenStream::dump_tokens() {
		Token tk{ next() };

		while (tk._kind != TokenKind::EoF) {
			std::cout << tk << std::endl;
			tk = next();
		}
	}

	Token TokenStream::lex() {
		while (_current_char < _src.size()) {
			auto ch{ chr()};
			if (is_blank(ch)) {
				advance();
			}
			else if (ch == ';') {
				advance();
				return Token(";", TokenKind::Semicolon, {});
			}
			else if (ch == '{') {
				advance();
				return Token("{", TokenKind::LBrace, {});
			}
			else if (ch == '}') {
				advance();
				return Token("}", TokenKind::RBrace, {});
			}
			else if (ch == '(') {
				advance();
				return Token("(", TokenKind::LParen, {});
			}
			else if (ch == ')') {
				advance();
				return Token(")", TokenKind::RParen, {});
			}
			// multicharacter tokens
			else if (isalpha(ch)) {
				return read_alphanumeric();
			}
			else if (isdigit(ch)) {
				return read_number();
			}
			else {
				std::cerr << "unknow token" << std::endl;
				abort();
			}
		}
		return Token::eof();
	}
};