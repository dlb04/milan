#pragma once

#include <vector>
#include <string>
#include <memory>
#include <string_view>
#include <list>
#include <cassert>
#include <regex>
#include <ostream>

#define RETURN_CASE_LABEL_AS_CSTR(label) case label: return #label

namespace milan {
	enum TokenKind {
		Null,
		EoF,
		Identifier, // [_a - zA - Z][_a - zA - Z0 - 9]{ 0,30 }
		Int,  // [-+]?\d+
		Uint, // \d+
		LBrace, // {
		RBrace, // }
		LParen, // (
		RParen, // )
		Semicolon, // ;

		// List of keywords, keep this ordered with
		// the table at Token::identifier_or_keyword()
		KwFn,  // "fn"
		KwInt, // "int"
		KwVoid, // "void"
	};

	struct TokenMetadata {

	};

	struct Token {

		Token() = default;

		Token(const std::string& repr, TokenKind kind, TokenMetadata metadata)
			:_repr(repr),
			_kind(kind),
			_metadata(metadata)
		{}

		Token& operator=(Token&& tk) noexcept {
			_repr = std::move(tk._repr);
			_kind = std::move(tk._kind);
			_metadata = std::move(tk._metadata);

			return tk;
		}

		void identifier_or_keyword() {

			// keep this ordered
			const std::string keywords[] = {
				"fn",
				"int",
				"void",
			};
			auto f = std::find(std::begin(keywords), std::end(keywords), _repr);
			if (f != std::end(keywords))
				_kind = static_cast<TokenKind>(TokenKind::KwFn + (f - std::begin(keywords)));
			
			//for (size_t i = 0; i < kws; i++) {
			//	if (keywords[i] == _repr) {
			//		_kind = static_cast<TokenKind>(TokenKind::KwFn + i);
			//	}
			//}
		}

		Token(Token&& rhs) = default;

		std::string _repr;
		TokenKind _kind;
		TokenMetadata _metadata;

		static Token eof() {
			return Token("", TokenKind::EoF, {});
		}

		static const char* kind_as_str(TokenKind kind) {
			switch (kind) {
			RETURN_CASE_LABEL_AS_CSTR(Null);
			RETURN_CASE_LABEL_AS_CSTR(EoF);
			RETURN_CASE_LABEL_AS_CSTR(Identifier);
			RETURN_CASE_LABEL_AS_CSTR(Int);
			RETURN_CASE_LABEL_AS_CSTR(Uint);
			RETURN_CASE_LABEL_AS_CSTR(LBrace);
			RETURN_CASE_LABEL_AS_CSTR(RBrace);
			RETURN_CASE_LABEL_AS_CSTR(LParen);
			RETURN_CASE_LABEL_AS_CSTR(RParen);
			RETURN_CASE_LABEL_AS_CSTR(Semicolon);
			RETURN_CASE_LABEL_AS_CSTR(KwFn);
			RETURN_CASE_LABEL_AS_CSTR(KwInt);
			RETURN_CASE_LABEL_AS_CSTR(KwVoid);
			default: return "UNK";
			}
		}
	};

	class TokenStream {
	public:
		TokenStream(const std::string& src)
			:_has_peeked(false),
			_src(src),
			_peeked_token(),
			_peeked_token_offset(0),
			_current_char(0)
		{}

		~TokenStream() = default;

	public:

		// always call after a successful peek()
		void consume() {
			assert(_has_peeked == true && "consume() can only be called after peek()");
			_current_char = _peeked_token_offset;
			_has_peeked = false;
		}

		Token next() {
			assert(_has_peeked == false && "alway call consume() after a peek()");
			return lex();
		}
		
		// One-symbol look-ahead
		Token& peek() {
			auto old{ _current_char };
			_peeked_token = lex();
			_peeked_token_offset = _current_char;
			_current_char = old;
			_has_peeked = true;
		}

		void dump_tokens();

	private:
		char chr() { return _src[_current_char]; }
		void advance() { _current_char++; }
		bool can_read() { return _current_char < _src.size(); }

	private:
		Token lex();
		Token read_alphanumeric();
		Token read_number();

	private:
		bool _has_peeked;
		std::string _src;
		Token _peeked_token;
		size_t _peeked_token_offset;
		size_t _current_char;
	};
}