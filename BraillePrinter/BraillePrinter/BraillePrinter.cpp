// BrailePrinter.cpp : Defines the entry point for the console application.
//

#include "parsebraille.h"
#include <iostream>
#include <thread>
#include <string>

int main(int argc,char** argv)
{/*
	for(auto i=1;i<65;++i)
	{
		std::cout<<"posns["<<std::to_string(i)<<"]={,};\n";
	}
	while(1);*/
	if(argc<3)
	{
		std::cout<<"Need input file and output file";
		return 0;
	}
	auto il=parse_braille::parse_file("example.txt");
	std::fstream fout("out.txt",std::ios::binary|std::ios::out);
	for(size_t i=0;i<il.size;++i)
	{
		fout.write((char const*)il.data.get(),il.size*sizeof(decltype((il.data[0]))));
		//std::cout<<(int)il.data[i]<<'\n';
	}
	//std::this_thread::sleep_for(std::chrono::seconds(10000));
    return 0;
}


