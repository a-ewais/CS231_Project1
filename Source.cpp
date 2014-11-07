#include<iostream>
#include<fstream>
#include<bitset>
using namespace std;
struct addressname{
	unsigned int address;
	char name;
};
ofstream output;
string out = "out.txt";
int j = 0;
int m = 1;

addressname *label;

struct reg{
	long int num;   //Can't initialize to 0
	string secname;
};

reg regs[32] = { 0 };   //initialize all members of the struct to 0, works without this?
char memory[8 * 1024];
int exist(int address)
{
	for (int i = 1; i <= m; i++)
	{
		if (address == label[i].address)
		{
			return i;
		}
	}

	return 0;
}
void labels(unsigned int inst[], int i)		//gets the adresses that would be labeled
{
	unsigned int pc = 0x00400000;
	label = new addressname[i];
	int c = 65;


	for (int z = 0; z < i; z++)
	{
		int opcode = inst[z] >> 26;

		if (opcode == 2 || opcode == 3)
		{
			//j type
			int address;
			address = inst[z] & 0x03ffffff;
			address = address << 2;
			address = address | (pc & 0xf0000000);
			if (exist(address) == 0)
			{
				label[m].name = char(c);
				c++;
				label[m].address = address;
				m++;
			}
		}
		else if (opcode == 4 || opcode == 5)
		{

			int imm = (inst[z] & 0x0000FFFF);
			if (exist(pc + (imm * 4)) == 0)
			{
				label[m].name = char(c);
				c++;
				label[m].address = (pc + (imm * 4) + 4);
				m++;

			}
		}
		pc += 4;
	}
}

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

	unsigned int rd, rs, rt, func, imm, shamt, opcode;
	unsigned int address;
  int imm_signed;
	static unsigned int pc = 0x00400000;

	opcode = instWord >> 26;
	if (exist(pc))
	{
		output << label[exist(pc)].name << ":" << endl;
	}
	if (opcode == 0)
	{
		//R-type
		func = instWord & 0x3F;
		shamt = (instWord >> 6) & 0x1f;
		rd = (instWord >> 11) & 0x1f;
		rt = (instWord >> 16) & 0x1f;
		rs = (instWord >> 21) & 0x1f;

		switch (func)
		{
		case 0x20:  //add
			output << "0x" << hex << pc << "\tadd\t" << regs[rd].secname << ", " << regs[rs].secname << ", " << regs[rt].secname << endl;
			break;
		case 0x21:  //addu
      if(rs == 0)
			  output << "0x" << hex << pc << "\tmove\t" << regs[rd].secname << ", " << regs[rt].secname << endl;
      else
        output << "0x" << hex << pc << "\taddu\t" << regs[rd].secname << ", " << regs[rs].secname << ", " << regs[rt].secname << endl;
			break;
		case 0x22:  //sub
			output << "0x" << hex << pc << "\tsub\t" << regs[rd].secname << ", " << regs[rs].secname << ", " << regs[rt].secname << endl;
			break;
		case 0x24:  //and
			output << "0x" << hex << pc << "\tand\t" << regs[rd].secname << ", " << regs[rs].secname << ", " << regs[rt].secname << endl;
			break;
		case 0x25:  //or
			output << "0x" << hex << pc << "\tor\t" << regs[rd].secname << ", " << regs[rs].secname << ", " << regs[rt].secname << endl;
			break;
		case 0x26: //xor
			output << "0x" << hex << pc << "\txor\t" << regs[rd].secname << ", " << regs[rs].secname << ", " << regs[rt].secname << endl;
			break;
		case 0x2a: //slt
			output << "0x" << hex << pc << "\tslt\t" << regs[rd].secname << ", " << regs[rs].secname << ", " << regs[rt].secname << endl;
			break;
		case 0x08: //jr
			output << "0x" << hex << pc << "\tjr\t" << regs[rs].secname << endl;
			break;
		case 0x02: //srl
			output << "0x" << hex << pc << "\tsrl\t" << regs[rd].secname << ", " << regs[rt].secname << ", " << shamt << endl;
			break;
		case 0x00: //sll
			output << "0x" << hex << pc << "\tsll\t" << regs[rd].secname << ", " << regs[rt].secname << ", " << shamt << endl;
			break;
		case 12: //syscall
			output << "0x" << hex << pc << "\tsyscall" << endl;
			break;
		}

	}
	else if (opcode == 2 || opcode == 3)
	{
		//j type
		address = instWord & 0x03ffffff;
		address = address << 2;
		address = address | (pc & 0xf0000000);
		if (opcode == 3)
		{
			output << "0x" << hex << pc << "\tjal\t" << label[exist(address)].name << endl;
		}
		else if (opcode == 2)
		{

			output << "0x" << hex << pc << "\tj\t" << label[exist(address)].name << endl;
		}
	}
	else if (0 != opcode && 2 != opcode && 3 != opcode && 16 != opcode && 17 != opcode && 18 != opcode && 19 != opcode)
	{
		//I-type

		rt = (instWord >> 16) & 0x1f;
		rs = (instWord >> 21) & 0x1f;
		imm = (instWord & 0x0000FFFF);
    imm_signed = short(imm);    //imm is always <= to 16 bits to this always works

		if (opcode == 8)
		{
      output << "0x" << hex << pc << "\taddi\t" << regs[rt].secname << "," << regs[rs].secname << "," << dec << imm_signed << endl;
		}
		else if (opcode == 9)
		{
			output << "0x" << hex << pc << "\taddiu\t" << regs[rt].secname << "," << regs[rs].secname << "," << dec << imm_signed << endl;
		}
		else if (opcode == 12)
		{
			output << "0x" << hex << pc << "\tandi\t" << regs[rt].secname << "," << regs[rs].secname << "," << dec << imm << endl;
		}
		else if (opcode == 13)
		{
			output << "0x" << hex << pc << "\tori\t" << regs[rt].secname << "," << regs[rs].secname << "," << dec << imm << endl;
		}
		else if (opcode == 14)
		{
			output << "0x" << hex << pc << "\txori\t" << regs[rt].secname << "," << regs[rs].secname << "," << dec << imm << endl;
		}
		else if (opcode == 4)
		{

			output << "0x" << hex << pc << "\tbeq\t" << regs[rs].secname << "," << regs[rt].secname << "," << label[exist(pc + 4 + (imm * 4))].name << endl;

		}
		else if (opcode == 5)
		{

			output << "0x" << hex << pc << "\tbne\t" << regs[rs].secname << "," << regs[rt].secname << "," << label[exist(pc + 4 + (imm * 4))].name << endl;

		}
		else if (opcode == 10)
		{
			output << "0x" << hex << pc << "\tslti\t" << regs[rt].secname << "," << regs[rs].secname << "," << dec << imm_signed << endl;
		}
		else if (opcode == 15)
		{
			output << "0x" << hex << pc << "\tlui\t" << regs[rt].secname << "," << dec << imm_signed << endl;
		}
		else if (opcode == 43)
		{
			output << "0x" << hex << pc << "\tsw\t" << regs[rt].secname << "," << dec << imm_signed << "(" << regs[rs].secname << ")" << endl;
		}

		else if (opcode == 35)
		{
			output << "0x" << hex << pc << "\tlw\t" << regs[rt].secname << "," << dec << imm_signed << "(" << regs[rs].secname << ")" << endl;
		}
		else if (opcode == 32)
		{
			output << "0x" << hex << pc << "\tlb\t" << regs[rt].secname << "," << dec << imm_signed << "(" << regs[rs].secname << ")" << endl;
		}
		else if (opcode == 40)
		{
			output << "0x" << hex << pc << "\tsb\t" << regs[rt].secname << "," << dec << imm_signed << "(" << regs[rs].secname << ")" << endl;
		}
		else if (opcode == 41)
		{
			output << "0x" << hex << pc << "\tsh\t" << regs[rt].secname << "," << dec << imm_signed << "(" << regs[rs].secname << ")" << endl;
		}
		else if (opcode == 33)
		{
			output << "0x" << hex << pc << "\tlh\t" << regs[rt].secname << "," << dec << imm_signed << "(" << regs[rs].secname << ")" << endl;
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

		switch (func)
		{
		case 0x20:  //add
			regs[rd].num = regs[rs].num + regs[rt].num;
			break;
		case 0x21:  //addu
			regs[rd].num = regs[rs].num + regs[rt].num;
			break;
		case 0x22:  //sub
			regs[rd].num = regs[rs].num - regs[rt].num;
			break;
		case 0x24:  //and
			regs[rd].num = regs[rs].num & regs[rt].num;
			break;
		case 0x25:  //or
			regs[rd].num = regs[rs].num | regs[rt].num;
			break;
		case 0x26: //xor
			regs[rd].num = regs[rs].num ^ regs[rt].num;
			break;
		case 0x2a: //slt
			regs[rd].num = (regs[rs].num < regs[rt].num) ? 1 : 0;
			break;
		case 0x08: //jr
			pc = regs[rs].num;
			break;
		case 0x02: //srl
			regs[rd].num = regs[rs].num >> regs[rt].num;
			break;
		case 0x00: //sll
			regs[rd].num = regs[rs].num << regs[rt].num;
			break;
		case 12:  //syscall
			switch (regs[2].num)
			{
			case 1:
				cout << regs[4].num << endl;
				break;
			case 5:
				cin >> regs[12].num;
				break;
			case 11:
				cout << char(regs[4].num) << endl;
				break;
			case 12:
				cin >> regs[4].num;
				break;
			}
			break;
		}
	}
	else if (opcode == 2 || opcode == 3)
	{
		//j type
		address = instWord & 0x03ffffff;
		address = address << 2;
		address = address | (pc & 0xf0000000);
		if (opcode == 3)
			regs[31].num = pc + 4;
		pc = address - 4;
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
	j = (pc - 0x00400000) / 4;
}
void main()
{
	unsigned int instruction[1024];
	ifstream infile;
	string in = "evenodd";

	fillregsec();   //Initialize secondary names

	infile.open(in.c_str(), ios::out | ios::binary);

	if (infile.is_open())
	{
		int i = 0;
		output.open(out.c_str());

		while (infile.read((char*)&instruction[i++], 4))
		{

		}

		labels(instruction, i);

		for (int b = 0; b < i; b++)
		{
			decode(instruction[b]);
		}
		output.close();
		for (; j < i;)
		{
			debug(instruction[j]);
		}
	}

	infile.close();
	cout << endl;
	system("pause");
}
