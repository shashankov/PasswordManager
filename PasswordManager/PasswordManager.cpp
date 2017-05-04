#include "stdafx.h"
#include <Windows.h>
#include <Wincrypt.h>
#include "compressedSuffixTrie.h"
using namespace System;
using namespace System::Windows::Forms;

/*
* Modes
* 0 - Use Case Sensitive Alphanumeric + Special Characters
* 1 - Use Case Sensitive Alphanumeric
* 2 - Use Case Sensitive Alphabets
* 3 - Use Only LowerCase Alphabets
* 4 - Use Only Numbers
*/
string randPswdGen(unsigned int length, int mode = 0) {

	//Initialize Cryptographic Random Function parameters
	HCRYPTPROV hCryptProv;
	CryptAcquireContext(&hCryptProv, NULL, NULL, PROV_RSA_FULL, 0);
	srand(time(NULL));

	int specialLength = length * (mode == 0 ? 20 : 0) / 100;
	BYTE* specialChars = new BYTE[specialLength];
	CryptGenRandom(hCryptProv, specialLength, specialChars);
	for (int i = 0; i < specialLength; i++) {
		specialChars[i] = specialChars[i] % 15 + 33;
	}

	int numericLength = length * (mode == 4 ? 100 : (mode == 0 ? 30 : (mode > 1 ? 0 : 40))) / 100;
	BYTE* numericChars = new BYTE[numericLength];
	CryptGenRandom(hCryptProv, numericLength, numericChars);
	for (int i = 0; i < numericLength; i++) {
		numericChars[i] = numericChars[i] % 10 + 48;
	}

	int upperLength = length * (mode > 2 ? 0 : (mode < 2 ? 25 : 50)) / 100;
	BYTE* upperChars = new BYTE[upperLength];
	CryptGenRandom(hCryptProv, upperLength, upperChars);
	for (int i = 0; i < upperLength; i++) {
		upperChars[i] = upperChars[i] % 26 + 65;
	}

	int lowerLength = length - specialLength - numericLength - upperLength;
	BYTE* lowerChars = new BYTE[lowerLength];
	CryptGenRandom(hCryptProv, lowerLength, lowerChars);
	for (int i = 0; i < lowerLength; i++) {
		lowerChars[i] = lowerChars[i] % 26 + 97;
	}

	string password = "";
	for (int i = 0; i < length; i++) {
		if (i < specialLength)
			password += (char)specialChars[i];
		else if (i < numericLength + specialLength)
			password += (char)numericChars[i - specialLength];
		else if (i < length - lowerLength)
			password += (char)upperChars[i - specialLength - numericLength];
		else
			password += (char)lowerChars[i - specialLength - numericLength - upperLength];
	}
	random_shuffle(password.begin(), password.end());
	return password;
}

compressedSuffixTrie *usernameTrie, *descriptionTrie;
map<int, pair<int, char*>> passwords;

string MarshalString(System::String ^ s) {
	using namespace Runtime::InteropServices;
	const char* chars =
		(const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
	string os = chars;
	Marshal::FreeHGlobal(IntPtr((void*)chars));
	return os;
}

void readPasswords(ifstream &ifile) {
	int index, size;
	char *pass;
	while (!ifile.eof()) {
		ifile >> index;
		if (ifile.eof())
			return;
		ifile >> size;
		ifile.ignore(1);
		pass = new char[size];
		ifile.read(pass, size);
		passwords[index] = make_pair(size, pass);
	}
	ifile.close();
}

void writePasswords(ofstream &ofile) {
	for (map<int, pair<int, char*>>::iterator i = passwords.begin(); i != passwords.end(); i++) {
		ofile << to_string(i->first) + " " + to_string(i->second.first) + " ";
		ofile.write((const char*)i->second.second, i->second.first);
	}
	ofile.close();
}

void PasswordManager::PasswordManager::putTrieInFile(ofstream &ofile, string input)
{
	int encryptedLength = AES::encrypt_size(input.length(), 1);
	unsigned char* message = new unsigned char[encryptedLength];
	strncpy((char *)message, input.c_str(), input.length());
	unsigned char IV[16];
	AES::createIV(IV);
	AES::encrypt_CBC_256(message, input.length(), this->usernameSHA, IV);
	ofile.write((const char*)message, encryptedLength);
	ofile.close();
	delete message;
}

System::Void PasswordManager::PasswordManager::checkBoxNewUser_CheckedChanged(System::Object ^ sender, System::EventArgs ^ e)
{
	if(this->checkBoxNewUser->Checked)
	{
		this->buttonSign->Text = L"Sign up";
		this->newUser = true;
	}
	else
	{
		this->buttonSign->Text = L"Sign In";
		this->newUser = false;

	}
	return System::Void();
}

void PasswordManager::PasswordManager::createUser(string username, string password) {
	char *usernameChar = &username[0], *passwordChar = &password[0];
	unsigned char usernameSHA[33] = { 0 }, passwordSHA[32], IV[16];

	SHA256((unsigned char*)usernameChar, username.length(), usernameSHA);
	SHA256((unsigned char*)passwordChar, password.length(), passwordSHA);

	pair<vector<string>, vector<string>> userData = this->getAllUserData();
	bool notFound = userData.first.end() == (find(userData.first.begin(), userData.first.end(), string((char *)usernameSHA, (char *)&usernameSHA[33])));
	if (notFound)
	{
		int lastIndex = userData.first.end()- userData.first.begin();
		ofstream ofile("userLog.bin", ios::app | ios::binary);
		string message = "correctPassword";
		message.resize(32, char(0));
		char *encryptedMessage = &message[0];
		AES::createIV(IV);
		AES::encrypt_CBC_256((unsigned char*)encryptedMessage, 15, passwordSHA, IV);

		ofile.write((char*)usernameSHA, 32);
		ofile.write(encryptedMessage, 32);
		ofile.close();

		this->usernameSHA = new unsigned char[32];
		strncpy((char *)this->usernameSHA, (const char*)usernameSHA, 32);

		create_directory("./" + to_string(lastIndex));
		usernameTrie = new compressedSuffixTrie();
		ofile.open("./" + to_string(lastIndex) + "/usnm.trie", ios::binary);
		this->putTrieInFile(ofile, usernameTrie->store());
		ofile.open("./" + to_string(lastIndex) + "/dscrp.trie", ios::binary);
		this->putTrieInFile(ofile, usernameTrie->store());
		ofile.close();
		delete usernameTrie;
		delete this->usernameSHA;

		ofile.open("./" + to_string(lastIndex) + "/pswd.map");
		ofile.close();
	}
	else
	{
		this->labelMessage->Text = L"Username Not Available";
		this->textBoxUserID->Text = L"";
		this->textBoxPassword->Text = L"";
	}
	
	return;
}

pair<vector<string>, vector<string>> PasswordManager::PasswordManager::getAllUserData()
{
	ifstream ifile("userLog.bin", ios::binary);
	pair<vector<string>, vector<string>> userData;
	if (!ifile.is_open())
		return userData;
	char readString[33] = { 0 };
	while (!ifile.eof()) {
		ifile.read(readString, 32);
		if (ifile.eof())
			break;
		userData.first.push_back(string(readString, &readString[33]));
		ifile.read(readString, 32);
		userData.second.push_back(string(readString, &readString[33]));
	}
	ifile.close();
	return userData;
}

void PasswordManager::PasswordManager::loginUser(std::string username, std::string password)
{
	char *usernameChar = &username[0], *passwordChar = &password[0];
	unsigned char usernameSHA[33] = { 0 }, passwordSHA[32], IV[16];

	SHA256((unsigned char*)usernameChar, username.length(), usernameSHA);
	SHA256((unsigned char*)passwordChar, password.length(), passwordSHA);

	pair<vector<string>, vector<string>> userData = this->getAllUserData();

	vector<string>::iterator it = find(userData.first.begin(), userData.first.end(), string((char *)usernameSHA, (char *)&usernameSHA[33]));
	if (it == userData.first.end())
	{
		this->labelMessage->Text = L"User Not Found.. Please Sign Up";
		return;
	}
	int index = it - userData.first.begin();
	char* encryptedMessage = new char[userData.second[index].length()];
	strncpy(encryptedMessage, userData.second[index].c_str(), userData.second[index].length());
	int messageSize = AES::decrypt_CBC_256((unsigned char*)encryptedMessage, 32, passwordSHA);
	encryptedMessage[messageSize] = 0;

	if (string(encryptedMessage).compare("correctPassword") == 0)
	{
		this->currentUserIndex = index;
		this->labelMessage->Text = L"Login Successful";
		this->passwordSHA = new unsigned char[32];
		this->usernameSHA = new unsigned char[32];
		strncpy((char *)this->passwordSHA, (const char*)passwordSHA, 32);
		strncpy((char *)this->usernameSHA, (const char*)usernameSHA, 32);
		usernameTrie = new compressedSuffixTrie();
		descriptionTrie = new compressedSuffixTrie();
		Application::DoEvents();

		ifstream usernameFile("./" + to_string(index) + "/usnm.trie", ios::binary), descriptionFile("./" + to_string(index) + "/dscrp.trie", ios::binary);
		struct stat output;
		stat(("./" + to_string(index) + "/usnm.trie").c_str(), &output);
		int input_size = output.st_size;
		usernameTrie->fill(this->getTrieFromFile(usernameFile, input_size));

		this->labelMessage->Text = L"Fetching User Data! Please Wait...";
		Application::DoEvents();

		usernameFile.close();
		stat(("./" + to_string(index) + "/dscrp.trie").c_str(), &output);
		input_size = output.st_size;
		descriptionTrie->fill(this->getTrieFromFile(descriptionFile, input_size));

		ifstream ifile("./" + to_string(index) + "/pswd.map", ios::binary);
		readPasswords(ifile);

		this->labelMessage->Text = L"Data Fetch Complete!";
		Application::DoEvents();

		this->panel1->Visible = false;
		this->panel2->Visible = true;
	}
	else
	{
		this->labelMessage->Text = L"Incorrect Password";
		this->textBoxPassword->Text = L"";
	}
	delete encryptedMessage;
}

string PasswordManager::PasswordManager::getTrieFromFile(ifstream &ifile, int fileSize) {
	unsigned char* message = new unsigned char[fileSize];
	ifile.read((char*)message, fileSize);
	int decryptedSize = AES::decrypt_CBC_256(message, fileSize, this->usernameSHA);
	message[decryptedSize] = 0;
	string toReturn = string((char*)message);
	ifile.close();
	delete message;
	return toReturn;
}

void PasswordManager::PasswordManager::search()
{
	// search indices from both Tries and append in one list
	string searchString = MarshalString(this->textBoxSearch->Text);
	//if (searchString.empty())
	//	return System::Void();

	list<int> indices = usernameTrie->indexSearch(searchString);
	indices.splice(indices.end(), descriptionTrie->indexSearch(searchString));
	indices.sort();
	indices.unique();
	int len = indices.size();
	// delete previous search results
	if (!(this->searchIndices == NULL)) {
		delete this->searchIndices;
		this->searchIndices = NULL;
	}

	// new search results
	this->searchIndices = new int[len];
	this->dataGridView->Rows->Clear();
	int i = 0;
	for (list<int>::iterator it = indices.begin(); it != indices.end(); it++, i++)
	{
		this->dataGridView->Rows->Add(gcnew String(((*usernameTrie)[*it]).c_str()), gcnew String(((*descriptionTrie)[*it]).c_str()));
		this->searchIndices[i] = *it;
	}
	this->dataGridView->Refresh();
}

System::Void PasswordManager::PasswordManager::buttonSign_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	this->labelMessage->Text = L"";
	if (this->newUser)
	{
		this->createUser(MarshalString(this->textBoxUserID->Text), MarshalString(this->textBoxPassword->Text));
	}
	else
	{
		this->loginUser(MarshalString(this->textBoxUserID->Text), MarshalString(this->textBoxPassword->Text));
	}
	return System::Void();
}

System::Void PasswordManager::PasswordManager::button1_MouseDown(System::Object ^ sender, System::Windows::Forms::MouseEventArgs ^ e)
{
	this->textBoxPassword->PasswordChar = '\0';
	this->textBoxPassword->UseSystemPasswordChar = false;
	Application::DoEvents();
	return System::Void();
}

System::Void PasswordManager::PasswordManager::button1_MouseLeave(System::Object ^ sender, System::EventArgs ^ e)
{
	this->textBoxPassword->UseSystemPasswordChar = true;
	Application::DoEvents();
	return System::Void();
}

System::Void PasswordManager::PasswordManager::button1_MouseUp(System::Object ^ sender, System::Windows::Forms::MouseEventArgs ^ e)
{
	this->textBoxPassword->UseSystemPasswordChar = true;
	Application::DoEvents();
	return System::Void();
}

System::Void PasswordManager::PasswordManager::buttonRandomGenerate_Click(System::Object ^ sender, System::EventArgs ^ e) {
	int mode, length;
	try {
		length = System::Convert::ToInt32(this->maskedTextBoxRandomLength->Text);
		mode = this->listBoxRules->SelectedIndex;
	}
	catch (...) {
		length = 0;
		mode = 0;
	}
	this->textBoxAddPassword->Text = gcnew String(randPswdGen(length, mode).c_str());
	return System::Void();
}

System::Void PasswordManager::PasswordManager::buttonShowNewPassword_MouseDown(System::Object ^ sender, System::Windows::Forms::MouseEventArgs ^ e)
{
	this->textBoxAddPassword->PasswordChar = '\0';
	this->textBoxAddPassword->UseSystemPasswordChar = false;
	Application::DoEvents();
	return System::Void();
}

System::Void PasswordManager::PasswordManager::buttonShowNewPassword_MouseLeave(System::Object ^ sender, System::EventArgs ^ e)
{
	this->textBoxAddPassword->UseSystemPasswordChar = true;
	Application::DoEvents();
	return System::Void();
}

System::Void PasswordManager::PasswordManager::buttonShowNewPassword_MouseUp(System::Object ^ sender, System::Windows::Forms::MouseEventArgs ^ e)
{
	this->textBoxAddPassword->UseSystemPasswordChar = true;
	Application::DoEvents();
	return System::Void();
}

System::Void PasswordManager::PasswordManager::buttonAdd_Click(System::Object ^ sender, System::EventArgs ^ e) {
	string username, description, password;
	username = MarshalString(this->textBoxAddUsername->Text);
	description = MarshalString(this->textBoxAddDescription->Text);
	password = MarshalString(this->textBoxAddPassword->Text);

	if (username.empty() || description.empty() || password.empty()) {
		this->labelAddMessage->Text = L"Invalid Entries! Check and Try Again";
		return System::Void();
	}

	int index = usernameTrie->addElement(username);
	descriptionTrie->addElement(description);

	int encryptedLength = AES::encrypt_size(password.length(), 1);
	unsigned char* pass = new unsigned char[encryptedLength + 1], IV[16];
	AES::createIV(IV);
	strncpy((char *)pass, password.c_str(), password.length());
	AES::encrypt_CBC_256(pass, password.length(), this->passwordSHA, IV);
	passwords[index] = make_pair(encryptedLength, (char*)pass);
	this->labelAddMessage->Text = L"Entry Successfully Added";

	this->textBoxAddUsername->Text = L"";
	this->textBoxAddDescription->Text = L"";
	this->textBoxAddPassword->Text = L"";
	return System::Void();
}

System::Void PasswordManager::PasswordManager::buttonShowClose_Click(System::Object ^ sender, System::EventArgs ^ e) {
	this->panel3->Visible = false;
	this->panel2->Visible = true;
	this->panel2->Enabled = true;
	this->textBoxShowPassword->Text = L"";
	return System::Void();
}

System::Void PasswordManager::PasswordManager::buttonCopyUsername_Click(System::Object ^ sender, System::EventArgs ^ e) {
	try {
		Clipboard::SetText(this->textBoxShowUsername->Text);
	}
	catch (...) {}
	return System::Void();
}

System::Void PasswordManager::PasswordManager::buttonCopyPassword_Click(System::Object ^ sender, System::EventArgs ^ e) {
	try {
		Clipboard::SetText(this->textBoxShowPassword->Text);
	}
	catch (...) {}
	return System::Void();
}

System::Void PasswordManager::PasswordManager::buttonShowShowPassword_MouseDown(System::Object ^ sender, System::Windows::Forms::MouseEventArgs ^ e)
{
	this->textBoxShowPassword->PasswordChar = '\0';
	this->textBoxShowPassword->UseSystemPasswordChar = false;
	Application::DoEvents();
	return System::Void();
}

System::Void PasswordManager::PasswordManager::buttonShowShowPassword_MouseLeave(System::Object ^ sender, System::EventArgs ^ e)
{
	this->textBoxShowPassword->UseSystemPasswordChar = true;
	Application::DoEvents();
	return System::Void();
}

System::Void PasswordManager::PasswordManager::buttonShowShowPassword_MouseUp(System::Object ^ sender, System::Windows::Forms::MouseEventArgs ^ e)
{
	this->textBoxShowPassword->UseSystemPasswordChar = true;
	Application::DoEvents();
	return System::Void();
}

System::Void PasswordManager::PasswordManager::buttonShow_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	unsigned char *password;
	int index;
	try {
		index = this->dataGridView->CurrentCell->RowIndex;
	}
	catch (...) {
		return System::Void();
	}
	pair<int, char*> data = passwords[this->searchIndices[index]];
	password = new unsigned char[data.first];
	strncpy((char *)password, (const char*)(data.second), data.first);
	int dcrlen = AES::decrypt_CBC_256(password, data.first, this->passwordSHA);
	password[dcrlen] = 0;
	this->textBoxShowPassword->Text = gcnew String((const char*)password);
	this->textBoxShowUsername->Text = this->dataGridView->CurrentRow->Cells[0]->FormattedValue->ToString();
	this->panel3->Visible = true;
	this->panel3->BringToFront();
	//this->panel2->Visible = false;
	this->panel2->Enabled = false;
	delete password;
	return System::Void();
}

System::Void PasswordManager::PasswordManager::buttonSignOut_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	//delete Tries 
	delete usernameTrie;
	usernameTrie = NULL;
	delete descriptionTrie;
	descriptionTrie = NULL;

	// delete passwords 
	for (map<int, pair<int, char*>>::iterator it = passwords.begin(); it != passwords.end(); it++)
		delete it->second.second;
	passwords.clear();
	delete this->passwordSHA;
	this->passwordSHA = NULL;
	delete this->usernameSHA;
	this->usernameSHA = NULL;
	delete this->searchIndices;
	this->searchIndices = NULL;

	this->labelMessage->Text = L"Succesfully Logged Out";
	// display first pannel
	this->panel1->Visible = true;
	this->panel2->Visible = false;

	this->dataGridView->Rows->Clear();
	this->textBoxSearch->Text = L"";
	this->textBoxAddUsername->Text = L"";
	this->textBoxAddDescription->Text = L"";
	this->textBoxAddPassword->Text = L"";
	this->textBoxPassword->Text = L"";

	return System::Void();
}

System::Void PasswordManager::PasswordManager::buttonSearch_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	this->search();
	return System::Void();
}

System::Void PasswordManager::PasswordManager::buttonSave_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	// tries
	ofstream ofile("./" + to_string(this->currentUserIndex) + "/usnm.trie", ios::binary);
	putTrieInFile(ofile, usernameTrie->store());
	ofile.open("./" + to_string(this->currentUserIndex) + "/dscrp.trie", ios::binary);
	putTrieInFile(ofile, descriptionTrie->store());
	
	// save passwords
	ofile.open("./" + to_string(this->currentUserIndex) + "/pswd.map", ios::binary);
	writePasswords(ofile);
	return System::Void();
}

System::Void PasswordManager::PasswordManager::buttonDelete_Click(System::Object ^ sender, System::EventArgs ^ e)
{
	int delIndex = this->dataGridView->CurrentRow->Index;
	int arraySize = this->dataGridView->Rows->Count;
	usernameTrie->removeElement(this->searchIndices[delIndex]);
	descriptionTrie->removeElement(this->searchIndices[delIndex]);
	passwords.erase(this->searchIndices[delIndex]);
	this->dataGridView->Rows->RemoveAt(delIndex);
	for (int i = delIndex; i < arraySize - 1; i++)
		this->searchIndices[i] = this->searchIndices[i + 1];
	this->dataGridView->Refresh();
	return System::Void();
}

System::Void PasswordManager::PasswordManager::textBoxSearch_KeyPress(System::Object ^ sender, System::Windows::Forms::KeyPressEventArgs ^ e)
{
	if (e->KeyChar == '\r')
		this->search();
	return System::Void();
}

System::Void PasswordManager::PasswordManager::textBoxPassword_KeyPress(System::Object ^ sender, System::Windows::Forms::KeyPressEventArgs ^ e)
{
	if (e->KeyChar == '\r') {
		this->labelMessage->Text = L"";
		if (this->newUser)
		{
			this->createUser(MarshalString(this->textBoxUserID->Text), MarshalString(this->textBoxPassword->Text));
		}
		else
		{
			this->loginUser(MarshalString(this->textBoxUserID->Text), MarshalString(this->textBoxPassword->Text));
		}
	}
	return System::Void();
}
