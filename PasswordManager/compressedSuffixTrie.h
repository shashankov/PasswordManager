#pragma once
#include "stdafx.h"

typedef struct compressedSuffixTrieNode compressedNode;
class compressedSuffixTrie
{
private:
	compressedNode* root;
	map<int, string> elements;
	list<int> deletedIndices;

	void addSuffix(string suffix, int index);
	void removeSuffix(string suffix, int index);
	list<int> getIndicesBelow(compressedNode* start);
	void deleteNodesBelow(compressedNode* start);

public:

	//Empty constructor: To be used in most cases
	compressedSuffixTrie();
	//Constructor which serially inserts the strings in elements
	compressedSuffixTrie(vector<string> elements);
	//Constructor which inserts only one element into the trie
	compressedSuffixTrie(string element);

	//Used to add the element string into the suffix trie
	//Return Value: Index of the element added in the trie
	int addElement(string element);

	//Removes element based on the input string
	//Returns whether the remove was successful or not
	bool removeElement(string element);

	//Removes element based on index
	//Return whether the remove was successful or not
	bool removeElement(int index);

	//Return indices of elements containing the input substring
	//Return value is a list on integers
	list<int> indexSearch(string subString);

	//Returns strings of elements containing the input substring
	//Return value is a vector of strings
	vector<string> stringSearch(string subString);

	//Returns the string representation of the class to be stored
	string store();

	//Assumes storedString to be created by the store() function
	//and also that the suffix trie is initially empty
	//Rebuilds the suffix trie from the string at a fasted pace
	void fill(string storedString);

	//Used to access string at input index
	//Will throw an exception if the index does not exist
	//Use other functions to ascertain the correctness of index
	string operator[](int index);

	//Returns true if the index exists in the trie
	bool exists(int index);

	//Destructor destroys all the nested nodes recursively
	~compressedSuffixTrie();
};

