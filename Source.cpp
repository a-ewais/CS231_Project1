#include<iostream>
#include<fstream>
#include<bitset>
using namespace std;
ofstream output;
string out = "out.txt";

struct reg{
	long int num = 0;
	string secname;
};
reg regs[32];
char memory[8 * 1024];
void fillregsec()
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


void decode(unsigned int instWord)
{
	unsigned int rd, rs, rt, func, shamt, imm, opcode;
	unsigned int address;
	opcode = instWord >> 26;
	if (opcode == 0)
	{
		//R-type
		func = instWord & 0x3F;
		shamt = (instWord >> 6) & 0x1f;
		rd = (instWord >> 11) & 0x1f;
		rt = (instWord >> 16) & 0x1f;
		rs = (instWord >> 21) & 0x1f;

	}
	else if (opcode == 2 || opcode == 3)
	{
		//j type
	}
	else
	{
		//I-type

		rt = (instWord >> 16) & 0x1f;
		rs = (instWord >> 21) & 0x1f;
		imm = (instWord & 0x0000FFFF);


		if (opcode == 8)
		{
			regs[rt].num = regs[rs].num + imm;
			output << "addi " << regs[rt].secname << "," << regs[rs].secname << "," << imm << endl;
		}
		else if (opcode == 9)
		{
			regs[rt].num = regs[rs].num + imm;
			output << "addiu " << regs[rt].secname << "," << regs[rs].secname << "," << imm << endl;
		}
		else if (opcode == 12)
		{
			regs[rt].num = regs[rs].num & imm;
			output << "andi " << regs[rt].secname << "," << regs[rs].secname << "," << imm << endl;
		}
		else if (opcode == 13)
		{
			regs[rt].num = regs[rs].num | imm;
			output << "ori " << regs[rt].secname << "," << regs[rs].secname << "," << imm << endl;
		}


	}
}

void main()
{
	ifstream infile;
	string in = "sss.bin";
	output.open(out.c_str());
	unsigned int instruction;
	fillregsec();
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
