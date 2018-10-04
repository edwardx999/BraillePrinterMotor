
#include "parsebraille.h"

namespace parse_braille {
	size_t parse_string(char const* input,unsigned char* output)
	{
		bool found_double_quote=false;
		unsigned char* opos=output;
		char letter;
		while((letter=*input)!=0)
		{
			if(letter>='A'&&letter<='Z')
			{
				*opos=dot_patterns::CAPITAL;
				++opos;
				*opos=dot_patterns::a+(letter-'A');
			}
			else if(letter>='a'&&letter<='z')
			{
				*opos=dot_patterns::a+(letter-'a');
			}
			else if(letter>='1'&&letter<='9')
			{
				*opos=dot_patterns::NUMBER;
				opos++;
				*opos=dot_patterns::one+(letter-'1');
			}
			else
			{
				switch(letter)
				{
					case '!':
						*opos=dot_patterns::exclamation_mark;
						break;
					case '\"':
						*opos=dot_patterns::double_quote_marker;
						++opos;
						if(found_double_quote)
						{
							*opos=dot_patterns::right_double_quote;
						}
						else
						{
							*opos=dot_patterns::left_double_quote;
						}
						found_double_quote=!found_double_quote;
						break;
					case '#':
						*opos=dot_patterns::crosshatch_marker;
						++opos;
						*opos=dot_patterns::crosshatch;
						break;
					case '$':
						*opos=dot_patterns::dollar_marker;
						++opos;
						*opos=dot_patterns::dollar;
						break;
					case '%':
						*opos=dot_patterns::percent_marker;
						++opos;
						*opos=dot_patterns::percent;
						break;
					case '&':
						*opos=dot_patterns::ampersand_marker;
						++opos;
						*opos=dot_patterns::ampersand;
						break;
					case '\'':
						*opos=dot_patterns::apostrophe;
						break;
					case '(':
						*opos=dot_patterns::paren_marker;
						++opos;
						*opos=dot_patterns::left_paren;
						break;
					case ')':
						*opos=dot_patterns::paren_marker;
						++opos;
						*opos=dot_patterns::right_paren;
						break;
					case '*':
						*opos=dot_patterns::asterisk_marker;
						++opos;
						*opos=dot_patterns::asterisk;
						break;
					case '+':
						*opos=dot_patterns::plus_marker;
						++opos;
						*opos=dot_patterns::plus;
						break;
					case ',':
						*opos=dot_patterns::comma;
						break;
					case '-':
						*opos=dot_patterns::minus_marker;
						++opos;
						*opos=dot_patterns::minus;
						break;
					case '.':
						*opos=dot_patterns::period;
						break;
					case '/':
						*opos=dot_patterns::slash_marker;
						++opos;
						*opos=dot_patterns::forward_slash;
						break;
					case '0':
						*opos=dot_patterns::NUMBER;
						++opos;
						*opos=dot_patterns::zero;
						break;
					case ':':
						*opos=dot_patterns::colon;
						break;
					case ';':
						*opos=dot_patterns::semicolon;
						break;
					case '<':
						*opos=angle_bracket_marker;
						++opos;
						*opos=left_angle_bracket;
						break;
					case '=':
						*opos=dot_patterns::equals_marker;
						++opos;
						*opos=dot_patterns::equals;
						break;
					case '>':
						*opos=angle_bracket_marker;
						++opos;
						*opos=right_angle_bracket;
						break;
					case '?':
						*opos=dot_patterns::question_mark;
						break;
					case '@':
						*opos=dot_patterns::at_marker;
						++opos;
						*opos=dot_patterns::at;
						break;
					case '[':
						*opos=dot_patterns::square_bracket_marker;
						++opos;
						*opos=dot_patterns::left_square_bracket;
						break;
					case '\\':
						*opos=dot_patterns::slash_marker;
						++opos;
						*opos=dot_patterns::back_slash;
						break;
					case ']':
						*opos=dot_patterns::square_bracket_marker;
						++opos;
						*opos=dot_patterns::right_square_bracket;
						break;
					case '^':
						*opos=dot_patterns::caret_marker;
						++opos;
						*opos=dot_patterns::caret;
						break;
					case '_':
						*opos=dot_patterns::underscore_marker;
						++opos;
						*opos=dot_patterns::underscore;
						break;
					case '`':
						*opos=dot_patterns::grave_marker;
						++opos;
						*opos=dot_patterns::grave;
						break;
					case '{':
						*opos=dot_patterns::curly_bracket_marker;
						++opos;
						*opos=dot_patterns::left_curly_bracket;
						break;
					case '|':
						*opos=dot_patterns::vertical_bar_marker;
						++opos;
						*opos=dot_patterns::vertical_bar;
					case '}':
						*opos=dot_patterns::curly_bracket_marker;
						++opos;
						*opos=dot_patterns::right_curly_bracket;
						break;
					case '~':
						*opos=dot_patterns::tilde_marker;
						++opos;
						*opos=dot_patterns::tilde;
						break;
					case '\n':
					case '\r':
					{
						*opos=dot_patterns::new_line;
						char next=*(input+1);
						if(next=='\n'||next=='\r')
						{
							++input;
						}
						break;
					}
					default:
						*opos=dot_patterns::space;
				}
			}
			++opos;
			++input;
		}
		*opos='\0';
		return opos-output;
	}
	namespace symbol_constants {
		braille_symbol const a={1,{dot_patterns::a}};
		braille_symbol const b={1,{dot_patterns::b}};
		braille_symbol const c={1,{dot_patterns::c}};
		braille_symbol const d={1,{dot_patterns::d}};
		braille_symbol const e={1,{dot_patterns::e}};
		braille_symbol const f={1,{dot_patterns::f}};
		braille_symbol const g={1,{dot_patterns::g}};
		braille_symbol const h={1,{dot_patterns::h}};
		braille_symbol const i={1,{dot_patterns::i}};
		braille_symbol const j={1,{dot_patterns::j}};
		braille_symbol const k={1,{dot_patterns::k}};
		braille_symbol const l={1,{dot_patterns::l}};
		braille_symbol const m={1,{dot_patterns::m}};
		braille_symbol const n={1,{dot_patterns::n}};
		braille_symbol const o={1,{dot_patterns::o}};
		braille_symbol const p={1,{dot_patterns::p}};
		braille_symbol const q={1,{dot_patterns::q}};
		braille_symbol const r={1,{dot_patterns::r}};
		braille_symbol const s={1,{dot_patterns::s}};
		braille_symbol const t={1,{dot_patterns::t}};
		braille_symbol const u={1,{dot_patterns::u}};
		braille_symbol const v={1,{dot_patterns::v}};
		braille_symbol const w={1,{dot_patterns::w}};
		braille_symbol const x={1,{dot_patterns::x}};
		braille_symbol const y={1,{dot_patterns::y}};
		braille_symbol const z={1,{dot_patterns::z}};
		braille_symbol const A={2,{dot_patterns::CAPITAL,dot_patterns::a}};
		braille_symbol const B={2,{dot_patterns::CAPITAL,dot_patterns::b}};
		braille_symbol const C={2,{dot_patterns::CAPITAL,dot_patterns::c}};
		braille_symbol const D={2,{dot_patterns::CAPITAL,dot_patterns::d}};
		braille_symbol const E={2,{dot_patterns::CAPITAL,dot_patterns::e}};
		braille_symbol const F={2,{dot_patterns::CAPITAL,dot_patterns::f}};
		braille_symbol const G={2,{dot_patterns::CAPITAL,dot_patterns::g}};
		braille_symbol const H={2,{dot_patterns::CAPITAL,dot_patterns::h}};
		braille_symbol const I={2,{dot_patterns::CAPITAL,dot_patterns::i}};
		braille_symbol const J={2,{dot_patterns::CAPITAL,dot_patterns::j}};
		braille_symbol const K={2,{dot_patterns::CAPITAL,dot_patterns::k}};
		braille_symbol const L={2,{dot_patterns::CAPITAL,dot_patterns::l}};
		braille_symbol const M={2,{dot_patterns::CAPITAL,dot_patterns::m}};
		braille_symbol const N={2,{dot_patterns::CAPITAL,dot_patterns::n}};
		braille_symbol const O={2,{dot_patterns::CAPITAL,dot_patterns::o}};
		braille_symbol const P={2,{dot_patterns::CAPITAL,dot_patterns::p}};
		braille_symbol const Q={2,{dot_patterns::CAPITAL,dot_patterns::q}};
		braille_symbol const R={2,{dot_patterns::CAPITAL,dot_patterns::r}};
		braille_symbol const S={2,{dot_patterns::CAPITAL,dot_patterns::s}};
		braille_symbol const T={2,{dot_patterns::CAPITAL,dot_patterns::t}};
		braille_symbol const U={2,{dot_patterns::CAPITAL,dot_patterns::u}};
		braille_symbol const V={2,{dot_patterns::CAPITAL,dot_patterns::v}};
		braille_symbol const W={2,{dot_patterns::CAPITAL,dot_patterns::w}};
		braille_symbol const X={2,{dot_patterns::CAPITAL,dot_patterns::x}};
		braille_symbol const Y={2,{dot_patterns::CAPITAL,dot_patterns::y}};
		braille_symbol const Z={2,{dot_patterns::CAPITAL,dot_patterns::z}};
		braille_symbol const one={2,{dot_patterns::NUMBER,dot_patterns::one}};
		braille_symbol const two={2,{dot_patterns::NUMBER,dot_patterns::two}};
		braille_symbol const three={2,{dot_patterns::NUMBER,dot_patterns::three}};
		braille_symbol const four={2,{dot_patterns::NUMBER,dot_patterns::four}};
		braille_symbol const five={2,{dot_patterns::NUMBER,dot_patterns::five}};
		braille_symbol const six={2,{dot_patterns::NUMBER,dot_patterns::six}};
		braille_symbol const seven={2,{dot_patterns::NUMBER,dot_patterns::seven}};
		braille_symbol const eight={2,{dot_patterns::NUMBER,dot_patterns::eight}};
		braille_symbol const nine={2,{dot_patterns::NUMBER,dot_patterns::nine}};
		braille_symbol const zero={2,{dot_patterns::NUMBER,dot_patterns::zero}};
	}
}