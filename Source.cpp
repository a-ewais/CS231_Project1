#include<iostream>
#include<fstream>
#include<bitset>
using namespace std;
ofstream output;
string out = "out.txt";
int j;

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
	static unsigned int pc = 0x00400000;
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
		address = instWord & 0x03ffffff;
		address = address << 2;
		address = address | (pc & 0xf0000000);
		if (opcode == 3)
		{
			output << "0x" << hex << pc << " jal " << "0x" << hex << address << endl;
		}
		else if (opcode == 2)
		{

			output << "0x" << hex << pc << " j " << "0x" << hex << address << endl;
		}
	}
	else if (0 != opcode && 2 != opcode && 3 != opcode && 16 != opcode && 17 != opcode && 18 != opcode && 19 != opcode)
	{
		//I-type

		rt = (instWord >> 16) & 0x1f;
		rs = (instWord >> 21) & 0x1f;
		imm = (instWord & 0x0000FFFF);


		if (opcode == 8)
		{
			output << "0x" << hex << pc << " addi " << regs[rt].secname << "," << regs[rs].secname << "," << dec << imm << endl;
		}
		else if (opcode == 9)
		{
			output << "0x" << hex << pc << " addiu " << regs[rt].secname << "," << regs[rs].secname << "," << dec << imm << endl;
		}
		else if (opcode == 12)
		{
			output << "0x" << hex << pc << " andi " << regs[rt].secname << "," << regs[rs].secname << "," << dec << imm << endl;
		}
		else if (opcode == 13)
		{
			output << "0x" << hex << pc << " ori " << regs[rt].secname << "," << regs[rs].secname << "," << dec << imm << endl;
		}
		else if (opcode == 14)
		{
			output << "0x" << hex << pc << " xori " << regs[rt].secname << "," << regs[rs].secname << "," << dec << imm << endl;
		}
		else if (opcode == 4)
		{
			output << "0x" << hex << pc << " beq " << regs[rt].secname << "," << regs[rs].secname << "," << dec << imm << endl;
		}
		else if (opcode == 5)
		{
			output << "0x" << hex << pc << " bne " << regs[rt].secname << "," << regs[rs].secname << "," << dec << imm << endl;
		}
		else if (opcode == 10)
		{
			output << "0x" << hex << pc << " slti " << regs[rt].secname << "," << regs[rs].secname << "," << dec << imm << endl;
		}
		else if (opcode == 15)
		{
			output << "0x" << hex << pc << " lui " << regs[rt].secname << "," << "0x" << imm << endl;
		}
		else if (opcode == 43)
		{
			output << "0x" << hex << pc << " sw " << regs[rt].secname << "," << dec << imm << "(" << regs[rs].secname << ")" << endl;
		}

		else if (opcode == 35)
		{
			output << "0x" << hex << pc << " lw " << regs[rt].secname << "," << dec << imm << "(" << regs[rs].secname << ")" << endl;
		}
		else if (opcode == 32)
		{
			output << "0x" << hex << pc << " lb " << regs[rt].secname << "," << dec << imm << "(" << regs[rs].secname << ")" << endl;
		}
		else if (opcode == 40)
		{
			output << "0x" << hex << pc << " sb " << regs[rt].secname << "," << dec << imm << "(" << regs[rs].secname << ")" << endl;
		}
		else if (opcode == 41)
		{
			output << "0x" << hex << pc << " sh " << regs[rt].secname << "," << dec << imm << "(" << regs[rs].secname << ")" << endl;
		}
		else if (opcode == 33)
		{
			output << "0x" << hex << pc << " lh " << regs[rt].secname << "," << dec << imm << "(" << regs[rs].secname << ")" << endl;
		}

	}

	else
	{
		// The opcode is not known
		cout << "0x" << hex << pc << " - Unkown Instruction" << endl;
	}

	pc += 4;
}
void debug(unsigned int instWord)
{
	unsigned int rd, rs, rt, func, shamt, imm, opcode;
	unsigned int address;
	int eximm;
	static unsigned int pc = 0x00400000;
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
		address = instWord & 0x03ffffff;
		address = address << 2;
		address = address | (pc & 0xf0000000);
		j = (pc - 0x00400000) / 4;
		if (opcode == 3)
			regs[31].num = pc + 4;
		pc = address;
		j = (pc - 0x00400000) / 4;
	}
	else
	{
		//I-type

		rt = (instWord >> 16) & 0x1f;
		rs = (instWord >> 21) & 0x1f;
		imm = (instWord & 0x0000FFFF);


		if (opcode == 8)
		{//addi
			if (imm & 0x8000)
				imm = imm & 0xffff0000;
			regs[rt].num = regs[rs].num + imm;

		}
		else if (opcode == 9)
		{//addiu
			if (imm & 0x8000)
				imm = imm & 0xffff0000;
			regs[rt].num = regs[rs].num + imm;
		}
		else if (opcode == 12)
		{//andi			
			regs[rt].num = regs[rs].num & imm;
		}
		else if (opcode == 13)
		{//ori			
			regs[rt].num = regs[rs].num | imm;
		}
		else if (opcode == 14)
		{//xori
			if (imm & 0x8000)
				imm = imm & 0xffff0000;
			regs[rt].num = regs[rs].num ^ imm;
		}
		else if (opcode == 4)
		{//beq

			if (regs[rt].num == regs[rs].num)
				pc += imm * 4;
		}
		else if (opcode == 5)
		{//bne
			if (regs[rt].num != regs[rs].num)
				pc += imm * 4;
		}
		else if (opcode == 10)
		{//slti
			if (regs[rs].num < imm)
				regs[rt].num = 1;
			else
				regs[rt].num = 0;

		}
		else if (opcode == 15)
		{//lui

			imm = imm << 16;
			regs[rt].num = regs[rt].num | imm;

		}
		else if (opcode == 43)
		{//sw

			imm = (imm & 0x8000) ? (0xFFFF0000 | imm) : imm;
			imm *= 4;
			eximm = regs[rs].num + imm - 0x10010000;

			memory[eximm++] = regs[rt].num & 0x000000ff;
			memory[eximm++] = (regs[rt].num & 0x0000ff00) >> 8;
			memory[eximm++] = (regs[rt].num & 0x00ff0000) >> 16;
			memory[eximm] = (regs[rt].num & 0xff000000) >> 24;
		}

		else if (opcode == 35)
		{//lw

			address = regs[rs].num + imm * 4;
			if (imm & 0x8000)
				imm = imm & 0xffff0000;
			regs[rt].num = 0;
			regs[rt].num |= memory[address++ - 0x10010000];
			regs[rt].num |= (memory[address++ - 0x10010000] << 8);
			regs[rt].num |= (memory[address++ - 0x10010000] << 16);
			regs[rt].num |= (memory[address - 0x10010000] << 24);



		}
		else if (opcode == 32)
		{//lb
			if (imm & 0x8000)
				imm = imm | 0xffff0000;
			address = regs[rs].num + imm;
			regs[rt].num = memory[address - 0x10010000];

		}
		else if (opcode == 40)
		{//sb
			if (imm & 0x8000)
				imm = imm & 0xffff0000;
			address = regs[rs].num + imm;
			memory[address - 0x10010000] = regs[rt].num & 0x000000ff;

		}
		else if (opcode == 41)
		{//sh

			address = regs[rs].num + imm * 2;
			if (imm & 0x8000)
				imm = imm & 0xffff0000;
			memory[address++ - 0x10010000] = regs[rt].num & 0x000000ff;
			memory[address - 0x10010000] = (regs[rt].num & 0x0000ff00) >> 8;

		}
		else if (opcode == 33)
		{//lh

			address = regs[rs].num + imm * 2;
			if (imm & 0x8000)
				imm = imm & 0xffff0000;
			regs[rt].num = 0;
			regs[rt].num |= memory[address++ - 0x10010000];
			regs[rt].num |= (memory[address - 0x10010000] << 8);

		}
	}
	pc += 4;
}
void main()
{
	ifstream infile;
	string in = "sss.bin";
	unsigned int instruction[1024];
	fillregsec();
	infile.open(in.c_str(), ios::out | ios::binary);
	if (infile.is_open())
	{
		int i = 0;
		output.open(out.c_str());
		while (infile.read((char*)&instruction[i], 4))
		{
			decode(instruction[i++]);
		}
		output.close();
		for (j = 0; j < i; j++)
		{
			debug(instruction[j]);
		}
	}
	infile.close();
	cout << endl;
	system("pause");
}
