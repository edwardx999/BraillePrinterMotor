#ifndef PARSEBRAILLE_H
#define PARSEBRAILLE_H
#include <fstream>
#include <string>
#include <streambuf>
#include <memory>
/*
Source:
http://www.brailleauthority.org/ueb/symbols_list.pdf
*/
namespace parse_braille {
	typedef unsigned char dot_pattern;
	/*
		null=0,
		a-z=1-16,
		d followed by a string of numbers: numbers refer to blackened dots,
		d-sequences are in the order specified by the braille authority
	*/
	enum dot_patterns:dot_pattern {
		d0,
		d1,d12,d14,d145,d15,d124,d1245,d125,d24,d245,
		d13,d123,d134,d1345,d135,d1234,d12345,d1235,d234,d2345,
		d136,d1236,d1346,d1356,d13456,d12346,d123456,d12356,d2346,d23456,
		d16,d126,d146,d1456,d156,d1246,d12456,d1256,d246,d2456,
		d2,d23,d25,d256,d26,d235,d2356,d236,d35,d356,
		d34,d346,d3456,d345,d3,d36,
		d4,d45,d456,d5,d46,d56,d6,
		NUMBER=d3456,
		null=0,
		a=d1,b,c,d,e,f,g,h,i,j,k,l,m,n,o,p,q,r,s,t,u,v,w,x,y,z,
		one=a,two,three,four,five,six,seven,eight,nine,zero,
		CAPITAL=d6,
		exclamation_point=d235,bang=exclamation_point,exclamation_mark=exclamation_point,
		double_quote_marker=d45,left_double_quote=d236,quotation_mark=left_double_quote,right_double_quote=d356,
		crosshatch_marker=d456,crosshatch,octothorpe=crosshatch,pound_sign=octothorpe,hash=octothorpe,hashtag=octothorpe,
		dollar_marker=d4,dollar=d234,
		percent_marker=d46,percent=d356,
		apostrophe=d3,single_quote=apostrophe,
		paren_marker=d5,left_paren=d126,right_paren=d345,
		asterisk_marker=d5,asterisk=d35,star=asterisk,
		plus_marker=d5,plus=d235,
		comma=d2,
		minus_marker=d5,hyphen_marker=minus_marker,hyphen=d36,minus=hyphen,
		period=d256,
		slash_marker=d456,
		forward_slash_marker=slash_marker,forward_slash=d34,
		colon=d25,
		semicolon=d23,
		less_than_marker=d4,angle_bracket_marker=less_than_marker,left_angle_bracket_marker=less_than_marker,less_than=d126,left_angle_bracket=less_than,
		equals_marker=d5,equals=d2356,
		greater_than_marker=angle_bracket_marker,right_angle_bracket_marker=greater_than_marker,greater_than=d345,right_angle_bracket=greater_than,
		question_mark=d236,
		at_marker=d4,at=d1,
		square_bracket_marker=d46,
		left_square_bracket=d126,
		back_slash_marker=slash_marker,
		back_slash=d16,
		right_square_bracket=d345,
		caret_marker=d35,caret=d26,
		underscore_marker=d46,underscore=d36,
		grave_marker=d45,grave=d16,
		curly_bracket_marker=d456,
		left_curly_bracket=d126,
		vertical_bar_marker=d456,vertical_bar=d1256,
		right_curly_bracket=d345,
		tilde_marker=d4,tilde=d35,
		ampersand_marker=d4,ampersand=d12346,
		space=65,
		new_line,
		tab,
	};

	struct braille_symbol {
		unsigned char size;
		dot_pattern patterns[5];
	};
	typedef braille_symbol braille_word;
	struct instruction_list {
		std::unique_ptr<dot_pattern[]> data;
		size_t size;
	};
	namespace symbol_constants {
		extern braille_symbol const a;
		extern braille_symbol const b;
		extern braille_symbol const c;
		extern braille_symbol const d;
		extern braille_symbol const e;
		extern braille_symbol const f;
		extern braille_symbol const g;
		extern braille_symbol const h;
		extern braille_symbol const i;
		extern braille_symbol const j;
		extern braille_symbol const k;
		extern braille_symbol const l;
		extern braille_symbol const m;
		extern braille_symbol const n;
		extern braille_symbol const o;
		extern braille_symbol const p;
		extern braille_symbol const q;
		extern braille_symbol const r;
		extern braille_symbol const s;
		extern braille_symbol const t;
		extern braille_symbol const u;
		extern braille_symbol const v;
		extern braille_symbol const w;
		extern braille_symbol const x;
		extern braille_symbol const y;
		extern braille_symbol const z;
		extern braille_symbol const A;
		extern braille_symbol const B;
		extern braille_symbol const C;
		extern braille_symbol const D;
		extern braille_symbol const E;
		extern braille_symbol const F;
		extern braille_symbol const G;
		extern braille_symbol const H;
		extern braille_symbol const I;
		extern braille_symbol const J;
		extern braille_symbol const K;
		extern braille_symbol const L;
		extern braille_symbol const M;
		extern braille_symbol const N;
		extern braille_symbol const O;
		extern braille_symbol const P;
		extern braille_symbol const Q;
		extern braille_symbol const R;
		extern braille_symbol const S;
		extern braille_symbol const T;
		extern braille_symbol const U;
		extern braille_symbol const V;
		extern braille_symbol const W;
		extern braille_symbol const X;
		extern braille_symbol const Y;
		extern braille_symbol const Z;
		extern braille_symbol const one;
		extern braille_symbol const two;
		extern braille_symbol const three;
		extern braille_symbol const four;
		extern braille_symbol const five;
		extern braille_symbol const six;
		extern braille_symbol const seven;
		extern braille_symbol const eight;
		extern braille_symbol const nine;
		extern braille_symbol const zero;
	}
	namespace sconsts=symbol_constants;

	/*
		Converts an ASCII string into a series of braille dot_patterns and returns the size of the output.
	*/
	size_t parse_string(char const* input,unsigned char* output);

	instruction_list parse_file(char const* filename);

	inline instruction_list parse_file(char const* filename)
	{
		std::ifstream file(filename);
		std::string input((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());
		instruction_list ret={std::make_unique<dot_pattern[]>(2*input.size())};
		ret.size=parse_string(input.c_str(),ret.data.get());
		return ret;
	}
};
#endif
