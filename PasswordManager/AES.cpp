#include "stdafx.h"
#include "AES.h"

unsigned char AES::S_Box[256] = { 0 };
unsigned char AES::inverse_S_Box[256] = { 0 };
unsigned char AES::rcon[256] = { 0 };
unsigned char AES::mul_table[6][256] = { { 1 } };
unsigned char round_keys_256[15][16];
const unsigned char AES::mul_nums[6] = { 2, 3, 9, 11, 13, 14 };

void AES::generate_S_Box()
{
	if (Rijndael_GF::inverse_lookup[0] == 1)
	{
		ifstream input("inverse_lookup.BIN", ifstream::binary);
		if (input.is_open())
		{
			input.read((char *)Rijndael_GF::inverse_lookup, 256);
		}
		else
			Rijndael_GF::generate_inverse();
	}
	
	for (int iterator = 0; iterator < 256; iterator++)
	{
		unsigned char s = Rijndael_GF::inverse_lookup[iterator], x = s;
		for (int loop = 0; loop < 4; loop++)
		{
			s = (s << 1) + ((s >= 128)?1:0);
			x = x ^ s;
		}
		S_Box[iterator] = x ^ 0x63;
		inverse_S_Box[x ^ 0x63] = iterator;

		ofstream print("S_Box.BIN", ofstream::binary);
		print.write((char *)S_Box, 256);
		print.close();
	}
}

void AES::generate_rcon()
{
	Rijndael_GF rcon_base = Rijndael_GF(2);
	rcon[1] = 1;
	for (int i = 2; i < 256; i++)
	{
		rcon[i] = (rcon_base * Rijndael_GF(rcon[i - 1])).value;
	}
	rcon[0] = (rcon_base * Rijndael_GF(rcon[255])).value;

	ofstream print("rcon.BIN", ofstream::binary);
	print.write((char *)rcon, 256);
	print.close();
}

void AES::generate_mul_tables()
{
	ofstream print("mul_tables.BIN", ofstream::binary);

	for (int i = 0; i < 6; i++)
	{
		Rijndael_GF num = Rijndael_GF(mul_nums[i]);
		for (int j = 0; j < 256; j++)
		{
			mul_table[i][j] = (num * Rijndael_GF(j)).value;
		}
		print.write((char *)mul_table[i], 256);
	}

	print.close();
}

void AES::Rijndael_Key_Core(unsigned char* io, int iteration)
{
	if (rcon[0] == 0)
	{
		ifstream input("rcon.BIN", ifstream::binary);
		if (input.is_open())
			input.read((char *)rcon, 256);
		else
			generate_rcon();
	}
	if (S_Box[0] == 0)
	{
		ifstream input("S_Box.BIN", ifstream::binary);
		if (input.is_open())
		{
			input.read((char *)S_Box, 256);
			for (int i = 0; i < 256; i++)
				inverse_S_Box[S_Box[i]] = i;
		}
		else
			generate_S_Box();
	}

	//Perform the rotate
	unsigned char temp = io[3];
	io[3] = io[2];
	io[2] = io[1];
	io[1] = io[0];
	io[0] = temp;

	//Use the Substitution Box
	for (int i = 0; i < 4; i++)
		io[i] = S_Box[io[i]];

	//Apply RCON XOR Operation
	io[3] = io[3] ^ rcon[iteration];
}

void AES::expand_key(unsigned char *key, unsigned char extended_key_array[][16], int key_size)
{
	//key_size is assumed to be the number of bits

	int key_length = key_size / 8;
	int num_keys = (key_size == 128) ? 11 : ((key_size == 192) ? 13 : 15);
	int extended_length = num_keys * 16;
	unsigned char* output = new unsigned char [extended_length];

	int iteration = 1;
	int extended = key_length;

	for (int i = 0; i < key_length; i++)
		output[i] = key[i];

	while (extended != extended_length)
	{
		//Done for all key sizes
		unsigned char next[4];
		for (int i = 0; i < 4; i++)
			next[i] = output[extended - 4 + i];
		//Depends on the key size
		if (extended % key_length == 0)
		{
			Rijndael_Key_Core(next, iteration);
			iteration++;
		}

		//Only for the 256 Bit Key
		if ((key_size == 256) && (extended % key_length == 16))
			for (int i = 0; i < 4; i++)
				next[i] = S_Box[next[i]];

		//Done for all key sizes
		for (int i = 0; i < 4; i++)
			output[extended + i] = next[i] ^ output[extended - key_length + i];
		extended += 4;

		//
	}
	for (int i = 0; i < num_keys; i++)
		for (int j = 0; j < 16; j++)
			extended_key_array[i][j] = output[i*16 + j];

	delete[] output;
}

void AES::expand_key256(unsigned char *key, unsigned char extended_key_array[][16])
{
	unsigned char output[240];

	int iteration = 1;
	int extended = 32;

	for (int i = 0; i < 32; i++)
		output[i] = key[i];

	while (extended != 240){
		unsigned char next[4];
		for (int i = 0; i < 4; i++)
			next[i] = output[extended - 4 + i];

		if (extended % 32 == 0){
			Rijndael_Key_Core(next, iteration);
			iteration++;
		}

		if (extended % 32 == 16)
			for (int i = 0; i < 4; i++)
				next[i] = S_Box[next[i]];

		for (int i = 0; i < 4; i++)
			output[extended + i] = next[i] ^ output[extended - 32 + i];
		extended += 4;

	}
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 16; j++)
			extended_key_array[i][j] = output[i * 16 + j];
}

void AES::subBytes(unsigned char input[4][4], bool inverse)
{
	//Substitution Using the S BOX
	//Input 4x4 matrix of bytes representing the 128 bits
	if (S_Box[0] == 0)
	{
		ifstream input("S_Box.BIN", ifstream::binary);
		if (input.is_open())
		{
			input.read((char *)S_Box, 256);
			for (int i = 0; i < 256; i++)
				inverse_S_Box[S_Box[i]] = i;
		}
		else
			generate_S_Box();
	}
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (inverse)
				input[i][j] = inverse_S_Box[input[i][j]];
			else
				input[i][j] = S_Box[input[i][j]];
}

void AES::shiftRows(unsigned char input[4][4], bool inverse)
{
	for (int i = 0; i < 4; i++)
	{
		unsigned char temp[4] = { 0 };
		for (int j = 0; j < 4; j++)
		{
			if (inverse)
				temp[(j + i) % 4] = input[i][j];
			else
				temp[(4 + j - i) % 4] = input[i][j];
		}
		for (int j = 0; j < 4; j++)
			input[i][j] = temp[j];
	}
}

void AES::mixColumns(unsigned char input[4], bool inverse)
{
	if (mul_table[0][0] == 1)
	{
		ifstream input("mul_tables.BIN", ifstream::binary);
		if (input.is_open())
			for (int i = 0; i < 6; i++)
				input.read((char *)mul_table[i], 256);
		else
			generate_mul_tables();
	}
	unsigned char input0[4], input1[4], input2[4], input3[4];
	for (int i = 0; i < 4; i++)
	{
		input0[i] = inverse ? mul_table[5][input[i]] : mul_table[0][input[i]];
		input1[i] = inverse ? mul_table[3][input[i]] : mul_table[1][input[i]];
		input2[i] = inverse ? mul_table[4][input[i]] : input[i];
		input3[i] = inverse ? mul_table[2][input[i]] : input[i];
	}
	input[0] = input0[0] ^ input1[1] ^ input2[2] ^ input3[3];
	input[1] = input3[0] ^ input0[1] ^ input1[2] ^ input2[3];
	input[2] = input2[0] ^ input3[1] ^ input0[2] ^ input1[3];
	input[3] = input1[0] ^ input2[1] ^ input3[2] ^ input0[3];
}

void AES::round_encrypt(unsigned char io[4][4], unsigned char round_key[16], bool last)
{
	subBytes(io);
	shiftRows(io);
	if (!last)
		for (int i = 0; i < 4; i++)
		{
			unsigned char column[4] = { io[0][i],io[1][i],io[2][i],io[3][i] };
			mixColumns(column);
			for (int j = 0; j < 4; j++)
				io[j][i] = column[j];
		}
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			io[i][j] = io[i][j] ^ round_key[i * 4 + j];
}

void AES::round_decrypt(unsigned char io[4][4], unsigned char round_key[16], bool first)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			io[i][j] = io[i][j] ^ round_key[i * 4 + j];

	if (!first)
		for (int i = 0; i < 4; i++)
		{
			unsigned char column[4] = { io[0][i],io[1][i],io[2][i],io[3][i] };
			mixColumns(column, true);
			for (int j = 0; j < 4; j++)
				io[j][i] = column[j];
		}
	shiftRows(io, true);
	subBytes(io, true);
}

void AES::encrypt_block(unsigned char io[4][4], unsigned char round_keys[][16], int num_keys)
{
	int round = 0;
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			io[i][j] = io[i][j] ^ round_keys[round][i * 4 + j];
	round++;

	while (round != num_keys)
	{
		round_encrypt(io, round_keys[round], (round == (num_keys - 1)));
		round++;
	}
}

void AES::decrypt_block(unsigned char io[4][4], unsigned char round_keys[][16], int num_keys)
{
	int round = num_keys - 1;
	while (round != 0)
	{
		round_decrypt(io, round_keys[round], (round == (num_keys - 1)));
		round--;
	}

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			io[i][j] = io[i][j] ^ round_keys[round][i * 4 + j];
}

void AES::int_to_array(uint64_t input, unsigned char *output)
{
	for (int i = 0; i < 8; i++)
	{
		output[i] = input % 256;
		input /= 256;
	}
}

void AES::encrypt(unsigned char *message, int size, unsigned char *key, int key_size, int mode, unsigned char IV[16])
{
	int num_keys = (key_size == 128) ? 11 : ((key_size == 192) ? 13 : 15);
	unsigned char (*round_keys)[16] = new unsigned char[num_keys][16];
	AES::expand_key(key, round_keys, key_size);

	int num_blocks = 1 + size / 16;
	if (mode < 3)
	{
		for (int i = size; i < num_blocks*16; i++)
			message[i] = num_blocks * 16 - size;
		size = num_blocks * 16;
	}
	unsigned char* blocks = new unsigned char [((mode == 0) ? num_blocks : num_blocks + 1) * 16];
	for (int i = 0; i < num_blocks; i++)
		for (int j = 0; j < 16; j++)
			blocks[i*16 + j] = ((i * 16 + j) < size) ? message[i * 16 + j] : 0;
	if (mode != 0)
		for (int j = 0; j < 16; j++)
			blocks[num_blocks * 16 + j] = IV[j];
	/*
	* Modes Table
	* Mode No.	Mode Name				Mode Code
	*	0		Electronic CodeBook		ECB
	*	1		Cipher Block Chaining	CBC
	*	2		Propagating CBC			PCBC
	*	3		Cipher Feedback			CFB
	*	4		Output Feedback			OFB
	*	5		Counter					CTR
	*/
	unsigned char block_io[4][4];
	uint64_t counter = 0;
	for (int i = 0; i < num_blocks; i++)
	{
		if (mode == 5)
			int_to_array(counter, &IV[8]);
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				if (i == 0)
					block_io[j][k] = (mode == 0) ? blocks[i * 16 + j * 4 + k] : ((mode < 3) ? IV[j * 4 + k] ^ blocks[i * 16 + j * 4 + k] : IV[j * 4 + k]);
				else
					block_io[j][k] = (mode == 0) ? blocks[i * 16 + j * 4 + k] : ((mode == 1) ? (block_io[j][k] ^ blocks[i * 16 + j * 4 + k]) : (
						(mode == 2) ? (block_io[j][k] ^ blocks[i * 16 + j * 4 + k] ^ message[i * 16 + j * 4 + k]) : (
							(mode == 3) ? blocks[i * 16 - 16 + j * 4 + k] : ((mode == 4) ? block_io[j][k]: IV[j * 4 + k]))));
			}
		}
		encrypt_block(block_io, round_keys, num_keys);
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				blocks[i * 16 + j * 4 + k] = (mode < 3) ? block_io[j][k] : (blocks[i * 16 + j * 4 + k] ^ block_io[j][k]);
		counter++;
	}

	if (mode != 0)
	{
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				block_io[j][k] = blocks[num_blocks * 16 + j * 4 + k];
		encrypt_block(block_io, round_keys, num_keys);
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				blocks[num_blocks * 16 + j * 4 + k] = block_io[j][k];
	}
	for (int i = 0; i < ((mode == 0) ? num_blocks : num_blocks + 1); i++)
	{
		for (int j = 0; j < 16; j++)
		{
			if ((i * 16 + j) < size)
				message[i * 16 + j] = blocks[i * 16 + j];
			if (i == num_blocks)
				message[size + j] = blocks[i * 16 + j];
		}
	}

	delete[] round_keys;
	delete[] blocks;
}

void AES::encrypt_CBC_256(unsigned char *message, int size, unsigned char *key, unsigned char IV[16])
{
	AES::expand_key256(key, round_keys_256);

	int num_blocks = 1 + size / 16;
	for (int i = size; i < num_blocks * 16; i++)
		message[i] = num_blocks * 16 - size;
	size = num_blocks * 16;

	unsigned char* blocks = new unsigned char[(num_blocks + 1) * 16];
	for (int i = 0; i < num_blocks; i++)
		for (int j = 0; j < 16; j++)
			blocks[i * 16 + j] = ((i * 16 + j) < size) ? message[i * 16 + j] : 0;
	for (int j = 0; j < 16; j++)
		blocks[num_blocks * 16 + j] = IV[j];
	
	unsigned char block_io[4][4];
	uint64_t counter = 0;
	for (int i = 0; i < num_blocks; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				if (i == 0)
					block_io[j][k] = IV[j * 4 + k] ^ blocks[i * 16 + j * 4 + k];
				else
					block_io[j][k] = (block_io[j][k] ^ blocks[i * 16 + j * 4 + k]);
			}
		}
		encrypt_block(block_io, round_keys_256, 15);
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				blocks[i * 16 + j * 4 + k] = block_io[j][k];
		counter++;
	}

	for (int j = 0; j < 4; j++)
		for (int k = 0; k < 4; k++)
			block_io[j][k] = blocks[num_blocks * 16 + j * 4 + k];
	encrypt_block(block_io, round_keys_256, 15);
	for (int j = 0; j < 4; j++)
		for (int k = 0; k < 4; k++)
			blocks[num_blocks * 16 + j * 4 + k] = block_io[j][k];
	
	for (int i = 0; i <= num_blocks; i++) {
		for (int j = 0; j < 16; j++) {
			if ((i * 16 + j) < size)
				message[i * 16 + j] = blocks[i * 16 + j];
			if (i == num_blocks)
				message[size + j] = blocks[i * 16 + j];
		}
	}

	delete[] blocks;
}

int AES::decrypt(unsigned char *message, int size, unsigned char *key, int key_size, int mode)
{
	int num_keys = (key_size == 128) ? 11 : ((key_size == 192) ? 13 : 15);
	unsigned char (*round_keys)[16] = new unsigned char[num_keys][16];
	AES::expand_key(key, round_keys, key_size);

	unsigned char IV[16] = { 0 };
	if (mode != 0)
	{
		unsigned char temp[4][4];
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				temp[i][j] = message[size - 16 + i * 4 + j];
		decrypt_block(temp, round_keys, num_keys);
		for (int i = 0; i < 4; i++)
			for (int j = 0; j < 4; j++)
				IV[i * 4 + j] = temp[i][j];
		size -= 16;
	}
	if (mode > 3)
	{
		delete[] round_keys;
		encrypt(message, size, key, key_size, mode, IV);
		return size;
	}

	int num_blocks = (size / 16) + int(size % 16 != 0);
	unsigned char* blocks = new unsigned char[num_blocks * 16];
	for (int i = 0; i < num_blocks; i++)
		for (int j = 0; j < 16; j++)
			blocks[i * 16 + j] = ((i * 16 + j) < size) ? message[i * 16 + j] : 0;

	/*
	* Modes Table
	* Mode No.	Mode Name				Mode Code
	*	0		Electronic CodeBook		ECB
	*	1		Cipher Block Chaining	CBC
	*	2		Propagating CBC			PCBC
	*	3		Cipher Feedback			CFB
	*	4		Output Feedback			OFB
	*	5		Counter					CTR
	*/
	unsigned char block_io[4][4];
	for (int i = 0; i < num_blocks; i++)
	{
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
					block_io[j][k] = (mode != 3 || i > 0) ? blocks[i * 16 + j * 4 + k] : IV[j * 4 + k];

		(mode != 3) ? decrypt_block(block_io, round_keys, num_keys) : encrypt_block(block_io, round_keys, num_keys);
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				if (i == 0)
					blocks[i * 16 + j * 4 + k] = (mode == 0) ? block_io[j][k] : ((mode < 3) ? (IV[j * 4 + k] ^ block_io[j][k]) : (block_io[j][k] ^ blocks[i * 16 + j * 4 + k]));
				else
					blocks[i * 16 + j * 4 + k] = (mode == 0) ? block_io[j][k] : ((mode == 1) ? (block_io[j][k] ^ message[(i - 1) * 16 + j * 4 + k]): (
						(mode == 2) ? (block_io[j][k] ^ blocks[(i - 1) * 16 + j * 4 + k] ^ message[(i - 1) * 16 + j * 4 + k]) : (block_io[j][k] ^ blocks[i * 16 + j * 4 + k])));
			}
		}
	}

	for (int i = 0; i < num_blocks * 16; i++)
		message[i] = blocks[i];

	delete[] round_keys;
	delete[] blocks;
	
	return (mode == 3) ? size : (size - message[size - 1]);
}

int AES::decrypt_CBC_256(unsigned char *message, int size, unsigned char *key)
{
	AES::expand_key256(key, round_keys_256);

	unsigned char IV[16] = { 0 };
	unsigned char temp[4][4];
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			temp[i][j] = message[size - 16 + i * 4 + j];
	decrypt_block(temp, round_keys_256, 15);
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			IV[i * 4 + j] = temp[i][j];
	size -= 16;

	int num_blocks = (size / 16) + int(size % 16 != 0);
	unsigned char* blocks = new unsigned char[num_blocks * 16];
	for (int i = 0; i < num_blocks; i++)
		for (int j = 0; j < 16; j++)
			blocks[i * 16 + j] = ((i * 16 + j) < size) ? message[i * 16 + j] : 0;

	unsigned char block_io[4][4];
	for (int i = 0; i < num_blocks; i++) {
		for (int j = 0; j < 4; j++)
			for (int k = 0; k < 4; k++)
				block_io[j][k] = blocks[i * 16 + j * 4 + k];

		decrypt_block(block_io, round_keys_256, 15);
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				if (i == 0)
					blocks[i * 16 + j * 4 + k] = (IV[j * 4 + k] ^ block_io[j][k]);
				else
					blocks[i * 16 + j * 4 + k] = (block_io[j][k] ^ message[(i - 1) * 16 + j * 4 + k]);
			}
		}
	}

	for (int i = 0; i < num_blocks * 16; i++)
		message[i] = blocks[i];
	delete[] blocks;

	return (size - message[size - 1]);
}

int AES::encrypt_size(int message_size, int mode)
{
	if (mode < 3)
		message_size = (message_size + 16 - message_size % 16);
	return ((mode == 0) ? message_size : (message_size + 16));
}

void AES::createIV(unsigned char IV[16]){
	for(int i = 0; i < 16; i++){
		IV[i] = rand() % 256;
	}
	return;
}

AES::AES()
{
	//Empty Private Constructor. Never to be Used
}


AES::~AES()
{
	//Empty Destructor. No Instance Pointers exist.
}
