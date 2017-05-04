#pragma once
#include <stdint.h>

class AES
{
public:
	static unsigned char S_Box[256];
	static unsigned char inverse_S_Box[256];
	static unsigned char rcon[256];
	static unsigned char mul_table[6][256];
	static unsigned char round_keys[15][16];
	static const unsigned char mul_nums[6];

	/*
	 * Function: encrypt
	 * Parameters:
	 *		unsigned char* message	: Array containing the message to be encrypted
	 *								  also contains the output after encryption. Must
									  be of size as given by the encrypted size funtion.
	 *		int size				: Size to input message to be encrypted in bytes
	 *		unsigned char key		: Encryption Key
	 *		int key_size			: Key Size {128, 192, 256}			- Default: 128
	 *		int mode				: Cipher Block Mode of Operation	- Default: 1
	 *		unsigned char[16] IV	: Initialization Vector				- Default: { 0 }
	 *
	 * Performs AES encryption on the entire messgae block supplied to it using the
	 * key and key_size specified as the input and in the block cipher mode based on 
	 * the input parameter. Some modes require an Initialization Vector and it too
	 * must be provided.
	 * If the mode to be used does not require an initialization vector then the 
	 * parameter need not be provided.
	 *
	 * Modes Table
	 * Mode No.	Mode Name				Mode Code
	 *	0		Electronic CodeBook		ECB
	 *	1		Cipher Block Chaining	CBC
	 *	2		Propagating CBC			PCBC
	 *	3		Cipher Feedback			CFB
	 *	4		Output Feedback			OFB
	 *	5		Counter					CTR
	*/
	static void encrypt(unsigned char*, int, unsigned char*, int = 128, int = 1, unsigned char[16] = { char(0) });

	static void encrypt_CBC_256(unsigned char * message, int size, unsigned char * key, unsigned char IV[16]);

	/*
	* Function: decrypt
	* Parameters:
	*		unsigned char* message	: Array containing the message to be decrypted
	*								  and also contains the output after decrytion
	*		int size				: Size to input message in bytes
	*		unsigned char key		: Encryption Key
	*		int key_size			: Key Size {128, 192, 256}			- Default: 128
	*		int mode				: Cipher Block Mode of Operation	- Default: 1
	* Returns:
	*		int decrypted_size		: The size of the decryted output stored in the same
									  input message array
	* Performs AES decryption on the entire messgae block supplied to it using the
	* key and key_size specified as the input and in the block cipher mode based on
	* the input parameter. No initialization vector is necessary assuming the encrypted
	* message was created by the encrypt function of this AES class
	*
	* Modes Table
	* Mode No.	Mode Name				Mode Code
	*	0		Electronic CodeBook		ECB
	*	1		Cipher Block Chaining		CBC
	*	2		Propagating CBC			PCBC
	*	3		Cipher Feedback			CFB
	*	4		Output Feedback			OFB
	*	5		Counter					CTR
	*/
	static int decrypt(unsigned char*, int, unsigned char*, int = 128, int = 1);

	static int decrypt_CBC_256(unsigned char * message, int size, unsigned char * key);

	/*
	* Function: encrypt_size
	* Parameters:
	*		int size	: Size of message to be encrypted
	*		int mode	: Mode of encrytion
	* Returns:
	*		int encrypted_size	: Size of the encrypted output	
	*
	* The function provides the size of the output
	* after the encryption is performed on a messgage with
	* length equal to the parameter size and using the mode
	* specified in the mode parameter
	*
	* The array size of the input to the encrypt function 
	* must be at least the output of this function
	*/
	static int encrypt_size(int, int);
	
	static void createIV(unsigned char IV[16]);

private:
	AES();
	~AES();
	static void generate_S_Box();
	static void generate_rcon();
	static void generate_mul_tables();
	static void Rijndael_Key_Core(unsigned char*, int);
	static void expand_key(unsigned char*, unsigned char[][16], int);
	static void expand_key256(unsigned char * key, unsigned char extended_key_array[][16]);
	static void subBytes(unsigned char[4][4], bool inverse = false);
	static void shiftRows(unsigned char[4][4], bool inverse = false);
	static void mixColumns(unsigned char[4], bool inverse = false);
	static void round_encrypt(unsigned char[4][4], unsigned char[16], bool last = false);
	static void round_decrypt(unsigned char[4][4], unsigned char[16], bool first = false);
	static void encrypt_block(unsigned char[4][4], unsigned char round_keys[][16], int);
	static void decrypt_block(unsigned char[4][4], unsigned char round_keys[][16], int);
	static void int_to_array(uint64_t, unsigned char*);
};

