#include "stdafx.h"
#include "compressedSuffixTrie.h"

//Node of the Suffix Trie
//Consists of Children nodes and corresponding edge strings
//and indices of those elements whose suffixes end on it
struct compressedSuffixTrieNode {
	vector<pair<compressedNode*, string>> children;
	list<int> indices;

	//Used to create a node from a string
	//which has been produced by the store function
	//Any other string input may lead to undesirable results
	void fill(string storedString) {
		//Store the input string as a string stream
		stringstream ss;
		ss << storedString;

		int indicesLength, indice, childrenLength, stringLength;
		ss >> indicesLength;	//Get length of list of indices
		//Fill the list from the input string
		for (int i = 0; i < indicesLength; i++) {
			ss >> indice;
			this->indices.push_back(indice);
		}

		ss >> childrenLength;	//Get the length of the vector children
		//Fill the vector by creating new child nodes
		this->children.reserve(childrenLength);
		char* child;
		for (int i = 0; i < childrenLength; i++) {
			ss >> stringLength;	//Read the length of the string on the edge
			ss.ignore(1);		//Ignore a space character
			child = new char[stringLength + 1];
			ss.get(child, stringLength + 1, char(0));		//Get the string - Is not Character Delimited
			compressedNode* newNode = new compressedNode;	//Create new node
			children.push_back(make_pair(newNode, string(child)));	//Push new node into the vector
			delete child;	//Delete the dynamically created memory
		}
	}

	//Function which converts this structure to a string
	//Children nodes are not stored by it and hence all
	//information about the children nodes are lost apart
	//from the edge strings which gets stored.
	string store() {
		//Store the indices list
		string toReturn = to_string(this->indices.size()) + " ";
		for (list<int>::iterator i = indices.begin(); i != indices.end(); i++)
			toReturn += to_string(*i) + " ";

		//Store the children vector
		toReturn += to_string(children.size()) + " ";
		for (vector<pair<compressedNode*, string>>::iterator i = children.begin(); i != children.end(); i++) {
			toReturn += to_string((*i).second.length()) + " ";	//Length of the string
			toReturn += (*i).second;
		}
		//Return the string
		return toReturn;
	}
};

//Returns the uncommon suffix among the two input strings
//The strings are compared from the starting and whenever
//a diveregence is detected, the strings are split and the 
//unequal parts are returned as a pair.
pair<string, string> inString(string query, string word) {
	pair<string, string> toReturn;
	int i = 0;
	//Compare the two strings
	for (i = 0; i < min(query.length(), word.length()); i++) {
		if (query[i] != word[i])
			break;
	}
	//Extarct the substring and return the pair
	toReturn.first = query.substr(i, query.length());
	toReturn.second = word.substr(i, word.length());
	return toReturn;
}

void compressedSuffixTrie::addSuffix(string suffix, int index) {
	//Start from the root node
	compressedNode* currentNode = this->root;	//Stores the node in the current context
	while (!suffix.empty()) {					//Suffix is emptied or spliced at several places to exit the while loop
		if (currentNode->children.empty()) {	//If no children exist of the current node
			compressedNode* newNode = new compressedNode;					//A new node must be added
			currentNode->children.push_back(make_pair(newNode, suffix));	//With the current suffix as the connecting edge
			currentNode = newNode;	//Used later to add the index
			break;					//Break from the while statement
		}
		//Loop over all the children
		for (vector<pair<compressedNode*, string>>::iterator j = currentNode->children.begin(); j != currentNode->children.end(); j++) {
			pair<string, string> uncommon = inString(suffix, (*j).second);	//Get the uncommon part from the edge string
			if (uncommon.first.empty() && uncommon.second.empty()) {	//If Both the strings match exactly
				currentNode = (*j).first;	//Used later to add the index
				suffix = "";				//Exit the while loop later
				break;						//Exit the for loop
			}
			else if (uncommon.first.empty()) {	//Suffix contaning in the edge string => Edge must be split by adding a node
				compressedNode* newNode = new compressedNode;	//Create the new node
				compressedNode* previous = (*j).first;			//Previous child node will now be the child of the new node
				newNode->children.push_back(make_pair(previous, uncommon.second));
				(*j).first = newNode;	//Replace the child of the currentNode by the new node created
				(*j).second = suffix;	//Add the suffix to the edge
				currentNode = newNode;	//Used to add the index
				suffix = "";			//Break from the while loop later
				break;					//Break from the for loop
			}
			else if (uncommon.second.empty()) { //If edge string contained in suffix
				currentNode = (*j).first;		//Need to move further lower
				suffix = uncommon.first;		//The new search string would be the leftover part of the suffix
				break;							//Break from the for loop
			}
			else {								//If only partly common
				if (uncommon.first == suffix) {	//If nothing in common
					if (!(j + 1 == currentNode->children.end()))	//If not the last child
						continue;
					compressedNode* newNode = new compressedNode;	//Else not child found => new node must be added
					currentNode->children.push_back(make_pair(newNode, suffix));
					currentNode = newNode;
					suffix = "";
					break;
				}
				//If partially common string was found, the edge must be split and a bramch added from the splitting node
				compressedNode* newNode1 = new compressedNode, *newNode2 = new compressedNode;	//Create the splitting and new leaf node
				compressedNode* previous = (*j).first;	//The previous child node
				newNode1->children.push_back(make_pair(previous, uncommon.second));		//Add the previous child node as the child of the splitting node
				newNode1->children.push_back(make_pair(newNode2, uncommon.first));		//Add the new child node as the child of the splitting node
				currentNode->children.erase(j);	//Remove the child
				currentNode->children.push_back(make_pair(newNode1, suffix.substr(0, suffix.length() - uncommon.first.length())));	//Add the new child
				currentNode = newNode2;	//Used to add the index to the leaf node
				suffix = "";			//Break from the while loop later
				break;					//Break from the for loop
			}
		}
	}
	currentNode->indices.push_back(index);	//Add the index
}

void compressedSuffixTrie::removeSuffix(string suffix, int index) {
	//Searched for the suffix and removes the index from the list when found
	//Private Function: Check for existance performed by the public caller
	compressedNode* currentNode = this->root;	//Current context node - starting with the root
	//Till the suffix is found completely
	while (!suffix.empty()) {
		//Loop through all the child nodes
		for (vector<pair<compressedNode*, string>>::iterator j = currentNode->children.begin(); j != currentNode->children.end(); j++) {
			pair<string, string> uncommon = inString(suffix, (*j).second);
			if (uncommon.first.empty()) {	//Implies exact match because suffix exists
				(*j).first->indices.remove(index);	//Remove the index
				suffix = "";	//Get out of the loops
				break;
			}
			else if (uncommon.second.empty()) {	//If edge string contained in suffix
				currentNode = (*j).first;		//Get to the next node
				suffix = uncommon.first;		//and shorted the suffix to be searched for
				break;
			}
		}
	}
}

list<int> compressedSuffixTrie::getIndicesBelow(compressedNode * start) {
	//Gets indices in all the nodes which are children of the start node
	if (start->children.size() == 0)
		return start->indices;
	else {
		list<int> returnIndices = start->indices;
		for (vector<pair<compressedNode*, string>>::iterator i = start->children.begin(); i != start->children.end(); i++) {
			returnIndices.splice(returnIndices.end(), getIndicesBelow((*i).first));	//Concatenate strings
		}
		returnIndices.sort();		//Sort the list for unique function
		returnIndices.unique();		//Remove duplicated
		return returnIndices;		//Return the indices
	}
}

void compressedSuffixTrie::deleteNodesBelow(compressedNode * start) {
	//Deletes in a recursive depth first manner all the nodes below start
	//Private function used only by the destructor
	for (vector<pair<compressedNode*, string>>::iterator i = start->children.begin(); i != start->children.end(); i++) {
		deleteNodesBelow((*i).first);
	}
	delete start;
}

compressedSuffixTrie::compressedSuffixTrie(){
	this->root = new compressedNode();
}

compressedSuffixTrie::compressedSuffixTrie(vector<string> elements){
	this->root = new compressedNode();
	for (int i = 0; i < elements.size(); i++) {
		this->addElement(elements[i]);
	}
}

compressedSuffixTrie::compressedSuffixTrie(string element){
	this->root = new compressedNode();
	this->addElement(element);
}

int compressedSuffixTrie::addElement(string element)
{
	//Generated index: If there exists a deleted index then it is reused else the next int is chosen
	int index = this->deletedIndices.empty() ? this->elements.size() : deletedIndices.front();
	if (!deletedIndices.empty())
		deletedIndices.pop_front();	//Pop after selection
	//Add all possible suffixes of the string
	for (int i = element.length() - 1; i >= 0; i--) {
		addSuffix(element.substr(i, element.length()), index);
	}
	//Insert the element added in the element map
	elements.insert(make_pair(index, element));
	return index;
}

bool compressedSuffixTrie::removeElement(string element) {
	//Removes element based on the input string
	//If more then one element with the same string exist 
	//The first index is deleted

	//First the string is searched in the trie
	list<int> indices = this->indexSearch(element);
	if (indices.empty()) {
		return false;
	}
	int index = -1;
	//Then the index which exactly matched the input element is searched
	for (list<int>::iterator i = indices.begin(); i != indices.end(); i++) {
		if (this->elements[(*i)] == element) {
			index = *i;
			break;
		}
	}
	if (index == -1) {
		return false;
	}
	//And removed
	for (int i = element.length() - 1; i >= 0; i--) {
		removeSuffix(element.substr(i, element.length()), indices.front());
	}
	this->deletedIndices.push_back(indices.front());
	return true;
}

bool compressedSuffixTrie::removeElement(int index) {
	//Removes element based on index
	if (this->elements.find(index) == this->elements.end())
		return false;
	string element = this->elements[index];
	this->elements.erase(index);

	for (int i = element.length() - 1; i >= 0; i--) {
		removeSuffix(element.substr(i, element.length()), index);
	}
	this->deletedIndices.push_back(index);
	return true;
}


list<int> compressedSuffixTrie::indexSearch(string subString) {
	//The substring search function which return indices
	compressedNode* currentNode = this->root;	//Node in the current context - start from the root node
	while (!subString.empty()) {	//Continue till the substring is searched entirely
		if (currentNode->children.empty()) {	//If no child of current node exists then the sub string does not exist
			return list<int>();
		}
		//Iterate through all the child nodes
		for (vector<pair<compressedNode*, string>>::iterator j = currentNode->children.begin(); j != currentNode->children.end(); j++) {
			pair<string, string> uncommon = inString(subString, (*j).second);
			if (uncommon.first.empty()) {	//Node containing the substring has been found
				currentNode = (*j).first;	//Set the current node to get the indices later
				subString = "";				//Exit the loops
				break;
			}
			else if (uncommon.second.empty()) {	//If a part of the substring is found
				currentNode = (*j).first;		//Get to the next level
				subString = uncommon.first;
				break;							//Break from the for loop
			}
			else if (j + 1 == currentNode->children.end())	//If searched the last child and still not found
				return list<int>();
		}
	}
	return getIndicesBelow(currentNode);	//Get all the elements containing the substring
}

vector<string> compressedSuffixTrie::stringSearch(string subString) {
	//Search which returns the string
	//Uses the index search followed by extracting the strings from the
	//element map
	list<int> indices = this->indexSearch(subString);
	vector<string> matchFound;
	matchFound.reserve(indices.size());
	for (list<int>::iterator i = indices.begin(); i != indices.end(); i++) {
		matchFound.push_back(this->elements[*i]);
	}
	return matchFound;
}

string compressedSuffixTrie::store() {
	string toReturn = to_string(this->deletedIndices.size()) + " ";
	//First store the delete indices
	for (list<int>::iterator i = this->deletedIndices.begin(); i != this->deletedIndices.end(); i++) {
		toReturn += to_string(*i) + " ";
	}

	//Store the elements map
	toReturn += to_string(this->elements.size()) + " ";
	for (map<int, string>::iterator i = this->elements.begin(); i != this->elements.end(); i++) {
		toReturn += to_string(i->first) + " " + to_string(i->second.length()) + " " + i->second;
	}

	//Store the Nodes in BFS Traversal Manner
	queue<compressedNode*> bfsQueue;
	bfsQueue.push(this->root);
	string nodeString;
	while (!bfsQueue.empty()) {
		compressedNode* node = bfsQueue.front();
		bfsQueue.pop();
		nodeString = node->store();
		toReturn += to_string(nodeString.length()) + " " + nodeString;
		for (vector<pair<compressedNode*, string>>::iterator i = node->children.begin(); i != node->children.end(); i++) {
			bfsQueue.push((*i).first);
		}
	}
	return toReturn;
}

void compressedSuffixTrie::fill(string storedString) {
	stringstream ss;
	ss << storedString;
	char skip[2];

	//Fill the queue
	int queueLength, queueElement;
	ss >> queueLength;
	for (int i = 0; i < queueLength; i++) {
		ss >> queueElement;
		this->deletedIndices.push_back(queueElement);
	}

	//Fill the map
	int mapLength, mapIndex, stringLength;
	char* mapString;
	ss >> mapLength;
	for (int i = 0; i < mapLength; i++) {
		ss >> mapIndex;
		ss >> stringLength;
		ss.get(skip, 2, char(0));
		mapString = new char[stringLength + 1];
		ss.get(mapString, stringLength + 1, char(0));
		this->elements.insert(make_pair(mapIndex, string(mapString)));
		delete mapString;
	}

	//Get the nodes!
	int nodeLength;
	char* nodeString;
	queue<compressedNode*> bfsNodes;
	bfsNodes.push(this->root);
	while (!bfsNodes.empty()) {
		ss >> nodeLength;
		ss.get(skip, 2, char(0));
		nodeString = new char[nodeLength + 1];
		ss.get(nodeString, nodeLength + 1, char(0));
		compressedNode* currentNode = bfsNodes.front();
		bfsNodes.pop();
		currentNode->fill(string(nodeString));
		for (vector<pair<compressedNode*, string>>::iterator i = currentNode->children.begin(); i != currentNode->children.end(); i++) {
			bfsNodes.push((*i).first);
		}
		delete nodeString;
	}
}

string compressedSuffixTrie::operator[](int index) {
	return this->elements[index];
}

bool compressedSuffixTrie::exists(int index) {
	if (this->elements.find(index) == this->elements.end())
		return false;
	return true;
}

compressedSuffixTrie::~compressedSuffixTrie() {
	this->deleteNodesBelow(this->root);
}
