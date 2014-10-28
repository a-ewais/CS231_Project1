#include<iostream>
#include<fstream>
#include<bitset>
using namespace std;
ofstream output;
string out = "out.txt";//ok

struct reg{
	long int data=0;
	string secname;
};
reg regs[32];
void fillrigsec()
{
	regs[0].secname = "$zero";
	regs[1].secname = "$at";
	regs[2].secname = "$v0";
	regs[3].secname = "$v1";
	regs[4].secname = "$a0";
	regs[5].secname = "$a1";
	regs[6].secname = "$a2";
	regs[7].secname = "$a3";
	regs[8].secname = "$t0";
	regs[9].secname = "$t1";
	regs[10].secname = "$t2";
	regs[11].secname = "$t3";
	regs[12].secname = "$t4";
	regs[13].secname = "$t5";
	regs[14].secname = "$t6";
	regs[15].secname = "$t7";
	regs[16].secname = "$s0";
	regs[17].secname = "$s1";
	regs[18].secname = "$s2";
	regs[19].secname = "$s3";
	regs[20].secname = "$s4";
	regs[21].secname = "$s5";
	regs[22].secname = "$s6";
	regs[23].secname = "$s7";
	regs[24].secname = "$t8";
	regs[25].secname = "$t9";
	regs[26].secname = "$k0";
	regs[27].secname = "$k1";
	regs[28].secname = "$gp";
	regs[29].secname = "$sp";
	regs[30].secname = "$fp";
	regs[31].secname = "$ra";

}


void decode(unsigned int instruction)
{
	unsigned int rd, rs, rt, func, shamt, imm, opcode;
	unsigned int address;
	opcode=instruction >> 26;
	if (opcode == 0)
	{
		//R-type
		func = instruction & 0x0000003F;
		shamt = instruction & 0x000007c0;
		rd = instruction & 0x0000f800;
		rt = instruction & 0x001f0000;
		rs = instruction & 0x03e00000;
	}
	else if (opcode == 2 || opcode == 3)
	{
		//j type
	}
	else
	{
		//I-type
		rt = instruction & 0x001f0000;
		rs = instruction & 0x03e00000;
		imm = instruction & 0x0000ffff;
	}
}

void main()
{
	ifstream infile;
	string in="sss.bin";
	output.open(out.c_str());
	unsigned int instruction;
	infile.open(in.c_str(), ios::out | ios::binary);
	if (infile.is_open())
	{		
		while (infile.read((char*)&instruction, 4))
		{
			decode(instruction);
		}
	}
	
	infile.close();
	output.close();
	cout << endl;
	system("pause");
}
