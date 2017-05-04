/******************************************************************************
	Filename: SHA256.h
******************************************************************************/
#pragma once

// Header Files //
#include "stdafx.h"
#define SHA256_BLOCK_SIZE 32

typedef unsigned int uint;

void SHA256_transform(uint *state, unsigned char data[]);
void SHA256(unsigned char* messsage, int msglen, unsigned char* hash);