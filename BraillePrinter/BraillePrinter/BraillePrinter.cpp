// BrailePrinter.cpp : Defines the entry point for the console application.
//

#include "parsebraille.h"
#include <iostream>
#include <thread>
#include <string>
void digitalWrite(int,int);
void delay(int);

template<typename T>
struct make_signed {
	using type=T;
};
template<>
struct make_signed<unsigned int> {
	using type=int;
};
template<>
struct make_signed<unsigned short> {
	using type=short;
};
template<>
struct make_signed<unsigned char> {
	using type=signed char;
};
template<>
struct make_signed<char> {
	using type=signed char;
};
template<>
struct make_signed<long> {
	using type=long;
};

int main(int argc,char** argv)
{
	if(argc<3)
	{
		std::cout<<"Need input file and output file";
		return 0;
	}
	auto il=parse_braille::parse_file("example.txt");
	std::fstream fout("out.txt",std::ios::binary|std::ios::out);
	for(std::size_t i=0;i<il.size;++i)
	{
		fout.write((char const*)il.data.get(),il.size*sizeof(decltype((il.data[0]))));
		//std::cout<<(int)il.data[i]<<'\n';
	}
	//std::this_thread::sleep_for(std::chrono::seconds(10000));
	return 0;
}


