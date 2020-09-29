#include <iostream>
#include <string>
#include <fstream>
#include <bitset>

using namespace std;

const short p[10] = { 0x49fe, 0xd3c6, 0x7326, 0x1234, 0xdefb, 0x5a8b, 0x1e61, 0x77ad, 0x94b2, 0x5731 };
short keyArray[10];

const short s[8][4] = {
	{0xabcb,0x7386,0x6ea3,0x4128},
	{0x689b,0x22c7,0x532d,0xa42b},
	{0x0e52,0xf025,0x8724,0x9ac0},
	{0x776a,0x629f,0x4510,0x3188},
	{0x1111,0xda99,0x52bc,0xdead},
	{0xdeaf,0xbeef,0x5810,0x7219},
	{0x733b,0x5ac7,0x2468,0x9753},
	{0x8263,0x820d,0xe72f,0x410c}
};

const int encryptionSize = 4;
const int keyCharSize = 20;

int encrypt(int value);
short function(short value);

void printInt(int value);
void printShort(short value);
void printChar(char value);

int main() {

	string inputFormat = "";
	string outputFormat = "";

	string key = "";

	string inputString = "";
	string outputString = "";

	string intputFile = "";
	string outputFile = "";

	std::ofstream outfile;

	//get the key
	while (key.compare("") == 0) {
		cout << "Please enter a key" << endl;
		getline(cin, key);
	}
	while (key.size() < keyCharSize) {
		key = key + key;
	}
	if (key.size() > keyCharSize) {
		key = key.substr(0, keyCharSize);
	}

	int keyCounter = 0;
	while (keyCounter < keyCharSize / 2){
		short value = 0;

		value = key.at(keyCounter);
		value = value << 8;
		value = value | key.at(keyCounter + 1);

		keyArray[keyCounter] = value ^ p[keyCounter];
		keyCounter++;
	}

	//input format
	while (inputFormat.compare("1") != 0 && inputFormat.compare("2") != 0) {
		cout << "Please pick an input format: \n 1. File Name \n 2. String" << endl;
		getline(cin, inputFormat);
	}

	//gets file input
	if (inputFormat.compare("1") == 0) {
		fstream fs;
		fs.open("");
		
		while (fs.fail()) {
			cout << "Input File Name: " << endl;

			getline(cin, intputFile);
			fs.open(intputFile.c_str());
		}
	}
	else if (inputFormat.compare("2") == 0) {
		//gets string input
		cout << "Input String: " << endl;
		getline(cin, inputString);
	}
	
	//output format
	while (outputFormat.compare("1") != 0 && outputFormat.compare("2") != 0) {
		cout << "Please pick an output format: \n 1. To File \n 2. To Screen" << endl;
		getline(cin, outputFormat);
	}

	//output file
	if (outputFormat.compare("1") == 0) {
		while (outputFile.compare("") == 0) {
			cout << "Name of File to Output: ";
			getline(cin, outputFile);
			cout << endl;
		}
		outfile.open(outputFile.c_str());
	}

	if (inputFormat.compare("1") == 0) {
		//file format

		std::ifstream is(intputFile.c_str(), ios::binary | ios::ate);     // open file

		int totalSize = is.tellg();
		is.clear();
		is.seekg(0, ios::beg);

		int blocks = totalSize / encryptionSize;
		int remainder = totalSize % encryptionSize;

		int blockCounter = 0;
		while (blockCounter < blocks) {

			int blockPeice = 0;

			int subBlockCounter = 0;
			while (subBlockCounter < encryptionSize) {

				blockPeice = blockPeice << 8;
				char c;
				is.get(c);
				blockPeice = blockPeice | c;

				subBlockCounter++;

			}

			int value = encrypt(blockPeice);

			if (inputFormat.compare("1") == 0) {
				outfile.write(reinterpret_cast<const char *>(&value), sizeof(value));
			}
			else if (inputFormat.compare("2") == 0) {
				cout << std::hex << blockPeice << " -> " << std::hex << value << endl;
			}
			blockCounter++;
		}

		if (remainder != 0) {

			int remainderCounter = 0;
			int remainderBlock = 0;

			while (remainderCounter < remainder) {

				remainderBlock = remainderBlock << 8;
				char c;
				is.get(c);
				remainderBlock = remainderBlock | c;

				remainderCounter++;
			}

			int value = encrypt(remainderBlock);
			
			if (inputFormat.compare("1") == 0) {
				outfile.write(reinterpret_cast<const char *>(&value), sizeof(value));
			}
			else if (inputFormat.compare("2") == 0) {
				cout << std::hex << remainderBlock << " -> " << std::hex << value << endl;
			}
		}

		is.close();
		outfile.close();
	}
	else {
		//string format
		cout << "string format" << endl;

		int blocks = inputString.size() / encryptionSize;
		int remainder = inputString.size() % encryptionSize;

		int blockCounter = 0;
		while (blockCounter < blocks) {

			int blockPeice = 0;

			int subBlockCounter = 0;
			while (subBlockCounter < encryptionSize) {

				blockPeice = blockPeice << 8;
				char c = inputString.at((blockCounter * encryptionSize) + subBlockCounter);
				blockPeice = blockPeice | c;

				subBlockCounter++;

			}

			int value = encrypt(blockPeice);

			if (inputFormat.compare("1") == 0) {
				outfile.write(reinterpret_cast<const char *>(&value), sizeof(value));
			}
			else if (inputFormat.compare("2") == 0) {
				cout << std::hex << blockPeice << " -> " << std::hex << value << endl;
			}
			blockCounter++;
		}

		if (remainder != 0) {

			int remainderCounter = 0;
			int remainderBlock = 0;

			while (remainderCounter < remainder) {

				remainderBlock = remainderBlock << 8;
				char c = inputString.at((blocks * encryptionSize) + remainderCounter);
				remainderBlock = remainderBlock | c;

				remainderCounter++;
			}

			int value = encrypt(remainderBlock);

			if (inputFormat.compare("1") == 0) {
				outfile.write(reinterpret_cast<const char *>(&value), sizeof(value));
			}
			else if (inputFormat.compare("2") == 0) {
				cout << std::hex << remainderBlock << " -> " << std::hex << value << endl;
			}
		}

		
	}
	cout << "complete" << endl;
	return 0;
}

int encrypt(int value) {
	
	short right = (short)value;
	short left = (short)(value >> 16);

	int numCycles = 8;
	int cycle = 0;
	
	//loop 8 times
	while (cycle < numCycles) {

		//xor
		left = keyArray[cycle] ^ left;
		short fun = function(left);
		right = fun ^ right;

		//swap
		short tmp = right;
		right = left;
		left = tmp;

		cycle++;
	}

	left = left ^ keyArray[8];
	right = right ^ keyArray[9];

	int result = left;
	result = result << 16;
	result = result | right;
	
	return result;
}

short function(short value) {

	int counter = 0;
	int numCycles = 8;

	short currentValue = 0;

	while (counter < numCycles) {

		int index = (value >> (counter * 2)) & 3;
		short value = s[counter][index];
		currentValue += value;

		counter++;
	}
	return value;
}

void printInt(int value) {
	std::bitset<32> y(value);
	std::cout << y << '\n';
}

void printShort(short value) {
	std::bitset<16> y(value);
	std::cout << y << '\n';
}
void printChar(char value) {
	std::bitset<8> y(value);
	std::cout << y << '\n';
}