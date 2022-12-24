#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <bitset>
#include <cstring>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <boost/algorithm/hex.hpp>

#define COUNT_GENERATE_PRIMES 32000

using namespace std;

//author source code in C#: Алексей @NeverWalkAloner (habr.com)
//https://habr.com/ru/post/191240/
// Russian GOST 34.10.2018 openAccess
//author translate in C++ Abduykov Z.M
//TODO: 1) Using library for large arithmetic 2) Need test code

/**************USING BIG INTEGER(#Library boost)**************/
typedef unsigned int uBig_int; 
typedef int big_int;
typedef unsigned char uByte;
/************************************************************/

/***********************Generate primes number***************/
vector<uBig_int> primes;
void createPrimes()
{
	primes.push_back(2);
	for (big_int i = 3; i < COUNT_GENERATE_PRIMES; i += 2)
	{
		if ((i > 10) && (i % 10 == 5))
			continue;
		for (big_int j = 0; j < primes.size(); j++)
		{
			if (j * j - 1 > i)
			{
				primes.push_back(i);
				break;
			}
			if (i % j == 0)
				break;
			else
				primes.push_back(i);
		}
	}
}
bool primality_check(uBig_int filed)
{
	vector<uBig_int> multipliers;
	for (big_int i = 2; i <= static_cast<big_int>(sqrt(filed)); i++)
	{
		if (filed % i == 0)
		{
			multipliers.push_back(i);
			multipliers.push_back(filed / i);
		}
	}

	if (multipliers.size() != 0)
		return true;
	else
		return false;
}
/***********************************************************/

/******************Mod Inverse*****************************/
big_int modulo(big_int number, big_int modul)
{
	big_int r = number % modul;
	if (((modul > 0) && (r < 0)) || ((modul < 0) && (r > 0)))
		r += modul;
	return r;
}
big_int modInverse(big_int number, big_int modul)
{
	number = modulo(number, modul);
	for (big_int x = 1; x < modul; x++)
	{
		if (modulo(number * x, modul) == 1)
			return x;
	}
	return 0;
}
/***********************************************************/

/******************Work with HEX****************************/
//Convert.ToByte in C#
vector<uByte> hexStringToByte(string hex)
{
	vector<uByte> data;
	string temp = "";
	for (big_int i = 0; i < hex.length()/2; i++)
	{
		temp = hex.substr(i * 2, 2);
		char byte = (char)strtol(temp.c_str(), NULL, 16);
		data.push_back(byte);
	}
	return data;
}
string byteToHexString(const vector<uByte>& x)
{
	string strHex = "";
	ostringstream byteTohex;
	for (big_int i = 0; i < x.size(); i++)
	{
		if (big_int(x[i]) >= 0 && big_int(x[i]) <= 9)
			byteTohex << setfill('0') << setw(2) << hex << big_int(x[i]);
		else
			byteTohex << hex << big_int(x[i]);
	}
	strHex = byteTohex.str();

	return strHex;
}
//for number r and s
string bigIntToHexString(const vector<uBig_int>& x)
{
	string strHex = "";
	ostringstream intToHex;
	for (big_int i = 0; i < x.size(); i++)
	{
		if (x[i] >= 0 && x[i] <= 9)
			intToHex << setfill('0') << setw(2) << hex << x[i];
		else
			intToHex << hex << x[i];
	}
	strHex = intToHex.str();

	return strHex;
}
uBig_int hexByteToInt(const vector<uByte>& x)
{
	/*
	* Example
	* C# BigInteger(byte[] X) => X = [5,4,3,2,1] =>   4328719365 (0x0102030405)
	*/
	reverse(x.begin(), x.end());
	string strHex = byteToHexString(x);
	uBig_int temp = 0;
	stringstream hexToInt;
	hexToInt << hex << strHex;
	hexToInt >> temp;
	return temp;
}
uBig_int hexStringToInt(string hexString)
{
	uBig_int number = 0;
	stringstream hexToInt;
	hexToInt << hex << hexString;
	hexToInt >> number;
	return number;
}
/***********************************************************/

//# num = 000111 => return 3
uBig_int countOfSetBits(uBig_int num)
{
	num = num - ((num >> 1) & 0x55555555);        // add pairs of bits
	num = (num & 0x33333333) + ((num >> 2) & 0x33333333);  // quads
	num = (num + (num >> 4)) & 0x0F0F0F0F;        // groups of 8
	return (num * 0x01010101) >> 24;          // horizontal sum of bytes
}
void putBin(vector<uByte>& view, bitset<32> bin)
{
	bool firstZero = true;
	for (int i = bin.size() - 1; i >= 0; i--)
	{
		if (bin[i] != 0)
			firstZero = false;
		if (firstZero == false)
			view.push_back(bin[i]);

	}
}
uBig_int power(uBig_int num, uBig_int digree)
{
	uBig_int mult = 1;
	for (big_int i = 0; i < digree; i++)
	{
		mult *= num;
	}
	return mult;
}
uBig_int calculateAlpha(const vector<uByte>& h)
{
	vector<big_int> tempHash;
	for (uByte i = 0; i < h.size(); i++)//32/64
		tempHash.push_back(int(h[i]));

	vector<uByte> view;
	for (uByte i = 0; i < tempHash.size(); i++)
	{
		bitset<32> b(tempHash[i]);
		putBin(view, b);
	}
	cout << " Bin code hash: ";
	for (big_int i = 0; i < view.size(); i++)
		cout << big_int(view[i]);
	cout << endl;

	uBig_int sum = 0;
	for (big_int i = view.size() - 1, digree = 0; i >= 0; i--, digree++)
		if (big_int(view[i]) != 0)
			sum += big_int(view[i]) * power(2, digree);
	return sum;
}

class Stribog
{
    // Реализация на С https://xakep.ru/2016/07/20/hash-gost-34-11-2012/
    //Автор: Евгений Дроботун
    //Реализация на C# https://habr.com/ru/post/188152/
    //Автор: Алексей @NeverWalkAloner
public:
	Stribog(int typeHash)
	{
		memset(N, 0x00, 64); // взять ячейку памяти 64 байта и проинициализировать 0  N
		memset(Sigma, 0x00, 64);// взять ячейку памяти 64 байта и проинициализировать 0  S

		// memcpy копирует 64 байтa первого блока памяти, на который ссылается указатель initialize, во второй блок памяти, на который ссылается указатель h.
		//Заполняем h
		if (typeHash == 512)
			memcpy(h, initialize_zero_for512, 64);
		else
			memcpy(h, initialize_one_for256, 64);

		this->hash512 = typeHash == 512 ? true: false;
	}

	/*unsigned char**/vector<unsigned char> getHash(unsigned char* message)
	{
		stribog(message, sizeof(message));
		unsigned int hashSize = hash512 == true ? 64 : 32;
		for (int i = 0; i < hashSize; i++)
			hash.push_back(h[i]);
		return this->hash;
		//return this->h;
	}

	void showHash(vector<unsigned char> h)
	{
		unsigned char i;
		unsigned size;
		if (hash512 == true)
			size = 64;
		else
			size = 32;
		
		for (i = 0; i < size; i++)
			cout << hex << int(h[i]);

		cout << endl;
	}


private:
	//Присвоить начальные значения внутренних переменных: Для хеш - функции с длиной выхода 512 бит: h = iv = 0x0, 256 = 0х1
	const unsigned char initialize_zero_for512[64] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	};
	const unsigned char initialize_one_for256[64] = {
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
		0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01
	};

	//Каждый байт из 512 бит послед зам битом табл S
	const unsigned char pi[256] = {
		0xfc, 0xee, 0xdd, 0x11, 0xcf, 0x6e, 0x31, 0x16,
		0xfb, 0xc4, 0xfa, 0xda, 0x23, 0xc5, 0x04, 0x4d,
		0xe9, 0x77, 0xf0, 0xdb, 0x93, 0x2e, 0x99, 0xba,
		0x17, 0x36, 0xf1, 0xbb, 0x14, 0xcd, 0x5f, 0xc1,
		0xf9, 0x18, 0x65, 0x5a, 0xe2, 0x5c, 0xef, 0x21,
		0x81, 0x1c, 0x3c, 0x42, 0x8b, 0x01, 0x8e, 0x4f,
		0x05, 0x84, 0x02, 0xae, 0xe3, 0x6a, 0x8f, 0xa0,
		0x06, 0x0b, 0xed, 0x98, 0x7f, 0xd4, 0xd3, 0x1f,
		0xeb, 0x34, 0x2c, 0x51, 0xea, 0xc8, 0x48, 0xab,
		0xf2, 0x2a, 0x68, 0xa2, 0xfd, 0x3a, 0xce, 0xcc,
		0xb5, 0x70, 0x0e, 0x56, 0x08, 0x0c, 0x76, 0x12,
		0xbf, 0x72, 0x13, 0x47, 0x9c, 0xb7, 0x5d, 0x87,
		0x15, 0xa1, 0x96, 0x29, 0x10, 0x7b, 0x9a, 0xc7,
		0xf3, 0x91, 0x78, 0x6f, 0x9d, 0x9e, 0xb2, 0xb1,
		0x32, 0x75, 0x19, 0x3d, 0xff, 0x35, 0x8a, 0x7e,
		0x6d, 0x54, 0xc6, 0x80, 0xc3, 0xbd, 0x0d, 0x57,
		0xdf, 0xf5, 0x24, 0xa9, 0x3e, 0xa8, 0x43, 0xc9,
		0xd7, 0x79, 0xd6, 0xf6, 0x7c, 0x22, 0xb9, 0x03,
		0xe0, 0x0f, 0xec, 0xde, 0x7a, 0x94, 0xb0, 0xbc,
		0xdc, 0xe8, 0x28, 0x50, 0x4e, 0x33, 0x0a, 0x4a,
		0xa7, 0x97, 0x60, 0x73, 0x1e, 0x00, 0x62, 0x44,
		0x1a, 0xb8, 0x38, 0x82, 0x64, 0x9f, 0x26, 0x41,
		0xad, 0x45, 0x46, 0x92, 0x27, 0x5e, 0x55, 0x2f,
		0x8c, 0xa3, 0xa5, 0x7d, 0x69, 0xd5, 0x95, 0x3b,
		0x07, 0x58, 0xb3, 0x40, 0x86, 0xac, 0x1d, 0xf7,
		0x30, 0x37, 0x6b, 0xe4, 0x88, 0xd9, 0xe7, 0x89,
		0xe1, 0x1b, 0x83, 0x49, 0x4c, 0x3f, 0xf8, 0xfe,
		0x8d, 0x53, 0xaa, 0x90, 0xca, 0xd8, 0x85, 0x61,
		0x20, 0x71, 0x67, 0xa4, 0x2d, 0x2b, 0x09, 0x5b,
		0xcb, 0x9b, 0x25, 0xd0, 0xbe, 0xe5, 0x6c, 0x52,
		0x59, 0xa6, 0x74, 0xd2, 0xe6, 0xf4, 0xb4, 0xc0,
		0xd1, 0x66, 0xaf, 0xc2, 0x39, 0x4b, 0x63, 0xb6
	};
	// Представляет собой умножение 64-битного входного вектора на бинарную матрицу A размерами 64x64.
	const unsigned long long A[64] = {
		0x8e20faa72ba0b470, 0x47107ddd9b505a38,
		0xad08b0e0c3282d1c, 0xd8045870ef14980e,
		0x6c022c38f90a4c07, 0x3601161cf205268d,
		0x1b8e0b0e798c13c8, 0x83478b07b2468764,
		0xa011d380818e8f40, 0x5086e740ce47c920,
		0x2843fd2067adea10, 0x14aff010bdd87508,
		0x0ad97808d06cb404, 0x05e23c0468365a02,
		0x8c711e02341b2d01, 0x46b60f011a83988e,
		0x90dab52a387ae76f, 0x486dd4151c3dfdb9,
		0x24b86a840e90f0d2, 0x125c354207487869,
		0x092e94218d243cba, 0x8a174a9ec8121e5d,
		0x4585254f64090fa0, 0xaccc9ca9328a8950,
		0x9d4df05d5f661451, 0xc0a878a0a1330aa6,
		0x60543c50de970553, 0x302a1e286fc58ca7,
		0x18150f14b9ec46dd, 0x0c84890ad27623e0,
		0x0642ca05693b9f70, 0x0321658cba93c138,
		0x86275df09ce8aaa8, 0x439da0784e745554,
		0xafc0503c273aa42a, 0xd960281e9d1d5215,
		0xe230140fc0802984, 0x71180a8960409a42,
		0xb60c05ca30204d21, 0x5b068c651810a89e,
		0x456c34887a3805b9, 0xac361a443d1c8cd2,
		0x561b0d22900e4669, 0x2b838811480723ba,
		0x9bcf4486248d9f5d, 0xc3e9224312c8c1a0,
		0xeffa11af0964ee50, 0xf97d86d98a327728,
		0xe4fa2054a80b329c, 0x727d102a548b194e,
		0x39b008152acb8227, 0x9258048415eb419d,
		0x492c024284fbaec0, 0xaa16012142f35760,
		0x550b8e9e21f7a530, 0xa48b474f9ef5dc18,
		0x70a6a56e2440598e, 0x3853dc371220a247,
		0x1ca76e95091051ad, 0x0edd37c48a08a6d8,
		0x07e095624504536c, 0x8d70c431ac02a736,
		0xc83862965601dd1b, 0x641c314b2b8ee083
	};

	// набор 512 битных значений
	unsigned char C[12][64] = {
		{
			0xb1, 0x08, 0x5b, 0xda, 0x1e, 0xca, 0xda, 0xe9,
			0xeb, 0xcb, 0x2f, 0x81, 0xc0, 0x65, 0x7c, 0x1f,
			0x2f, 0x6a, 0x76, 0x43, 0x2e, 0x45, 0xd0, 0x16,
			0x71, 0x4e, 0xb8, 0x8d, 0x75, 0x85, 0xc4, 0xfc,
			0x4b, 0x7c, 0xe0, 0x91, 0x92, 0x67, 0x69, 0x01,
			0xa2, 0x42, 0x2a, 0x08, 0xa4, 0x60, 0xd3, 0x15,
			0x05, 0x76, 0x74, 0x36, 0xcc, 0x74, 0x4d, 0x23,
			0xdd, 0x80, 0x65, 0x59, 0xf2, 0xa6, 0x45, 0x07
		}, {
			0x6f, 0xa3, 0xb5, 0x8a, 0xa9, 0x9d, 0x2f, 0x1a,
			0x4f, 0xe3, 0x9d, 0x46, 0x0f, 0x70, 0xb5, 0xd7,
			0xf3, 0xfe, 0xea, 0x72, 0x0a, 0x23, 0x2b, 0x98,
			0x61, 0xd5, 0x5e, 0x0f, 0x16, 0xb5, 0x01, 0x31,
			0x9a, 0xb5, 0x17, 0x6b, 0x12, 0xd6, 0x99, 0x58,
			0x5c, 0xb5, 0x61, 0xc2, 0xdb, 0x0a, 0xa7, 0xca,
			0x55, 0xdd, 0xa2, 0x1b, 0xd7, 0xcb, 0xcd, 0x56,
			0xe6, 0x79, 0x04, 0x70, 0x21, 0xb1, 0x9b, 0xb7
		}, {
			0xf5, 0x74, 0xdc, 0xac, 0x2b, 0xce, 0x2f, 0xc7,
			0x0a, 0x39, 0xfc, 0x28, 0x6a, 0x3d, 0x84, 0x35,
			0x06, 0xf1, 0x5e, 0x5f, 0x52, 0x9c, 0x1f, 0x8b,
			0xf2, 0xea, 0x75, 0x14, 0xb1, 0x29, 0x7b, 0x7b,
			0xd3, 0xe2, 0x0f, 0xe4, 0x90, 0x35, 0x9e, 0xb1,
			0xc1, 0xc9, 0x3a, 0x37, 0x60, 0x62, 0xdb, 0x09,
			0xc2, 0xb6, 0xf4, 0x43, 0x86, 0x7a, 0xdb, 0x31,
			0x99, 0x1e, 0x96, 0xf5, 0x0a, 0xba, 0x0a, 0xb2
		}, {
			0xef, 0x1f, 0xdf, 0xb3, 0xe8, 0x15, 0x66, 0xd2,
			0xf9, 0x48, 0xe1, 0xa0, 0x5d, 0x71, 0xe4, 0xdd,
			0x48, 0x8e, 0x85, 0x7e, 0x33, 0x5c, 0x3c, 0x7d,
			0x9d, 0x72, 0x1c, 0xad, 0x68, 0x5e, 0x35, 0x3f,
			0xa9, 0xd7, 0x2c, 0x82, 0xed, 0x03, 0xd6, 0x75,
			0xd8, 0xb7, 0x13, 0x33, 0x93, 0x52, 0x03, 0xbe,
			0x34, 0x53, 0xea, 0xa1, 0x93, 0xe8, 0x37, 0xf1,
			0x22, 0x0c, 0xbe, 0xbc, 0x84, 0xe3, 0xd1, 0x2e
		}, {
			0x4b, 0xea, 0x6b, 0xac, 0xad, 0x47, 0x47, 0x99,
			0x9a, 0x3f, 0x41, 0x0c, 0x6c, 0xa9, 0x23, 0x63,
			0x7f, 0x15, 0x1c, 0x1f, 0x16, 0x86, 0x10, 0x4a,
			0x35, 0x9e, 0x35, 0xd7, 0x80, 0x0f, 0xff, 0xbd,
			0xbf, 0xcd, 0x17, 0x47, 0x25, 0x3a, 0xf5, 0xa3,
			0xdf, 0xff, 0x00, 0xb7, 0x23, 0x27, 0x1a, 0x16,
			0x7a, 0x56, 0xa2, 0x7e, 0xa9, 0xea, 0x63, 0xf5,
			0x60, 0x17, 0x58, 0xfd, 0x7c, 0x6c, 0xfe, 0x57
		}, {
			0xae, 0x4f, 0xae, 0xae, 0x1d, 0x3a, 0xd3, 0xd9,
			0x6f, 0xa4, 0xc3, 0x3b, 0x7a, 0x30, 0x39, 0xc0,
			0x2d, 0x66, 0xc4, 0xf9, 0x51, 0x42, 0xa4, 0x6c,
			0x18, 0x7f, 0x9a, 0xb4, 0x9a, 0xf0, 0x8e, 0xc6,
			0xcf, 0xfa, 0xa6, 0xb7, 0x1c, 0x9a, 0xb7, 0xb4,
			0x0a, 0xf2, 0x1f, 0x66, 0xc2, 0xbe, 0xc6, 0xb6,
			0xbf, 0x71, 0xc5, 0x72, 0x36, 0x90, 0x4f, 0x35,
			0xfa, 0x68, 0x40, 0x7a, 0x46, 0x64, 0x7d, 0x6e
		}, {
			0xf4, 0xc7, 0x0e, 0x16, 0xee, 0xaa, 0xc5, 0xec,
			0x51, 0xac, 0x86, 0xfe, 0xbf, 0x24, 0x09, 0x54,
			0x39, 0x9e, 0xc6, 0xc7, 0xe6, 0xbf, 0x87, 0xc9,
			0xd3, 0x47, 0x3e, 0x33, 0x19, 0x7a, 0x93, 0xc9,
			0x09, 0x92, 0xab, 0xc5, 0x2d, 0x82, 0x2c, 0x37,
			0x06, 0x47, 0x69, 0x83, 0x28, 0x4a, 0x05, 0x04,
			0x35, 0x17, 0x45, 0x4c, 0xa2, 0x3c, 0x4a, 0xf3,
			0x88, 0x86, 0x56, 0x4d, 0x3a, 0x14, 0xd4, 0x93
		}, {
			0x9b, 0x1f, 0x5b, 0x42, 0x4d, 0x93, 0xc9, 0xa7,
			0x03, 0xe7, 0xaa, 0x02, 0x0c, 0x6e, 0x41, 0x41,
			0x4e, 0xb7, 0xf8, 0x71, 0x9c, 0x36, 0xde, 0x1e,
			0x89, 0xb4, 0x44, 0x3b, 0x4d, 0xdb, 0xc4, 0x9a,
			0xf4, 0x89, 0x2b, 0xcb, 0x92, 0x9b, 0x06, 0x90,
			0x69, 0xd1, 0x8d, 0x2b, 0xd1, 0xa5, 0xc4, 0x2f,
			0x36, 0xac, 0xc2, 0x35, 0x59, 0x51, 0xa8, 0xd9,
			0xa4, 0x7f, 0x0d, 0xd4, 0xbf, 0x02, 0xe7, 0x1e
		}, {
			0x37, 0x8f, 0x5a, 0x54, 0x16, 0x31, 0x22, 0x9b,
			0x94, 0x4c, 0x9a, 0xd8, 0xec, 0x16, 0x5f, 0xde,
			0x3a, 0x7d, 0x3a, 0x1b, 0x25, 0x89, 0x42, 0x24,
			0x3c, 0xd9, 0x55, 0xb7, 0xe0, 0x0d, 0x09, 0x84,
			0x80, 0x0a, 0x44, 0x0b, 0xdb, 0xb2, 0xce, 0xb1,
			0x7b, 0x2b, 0x8a, 0x9a, 0xa6, 0x07, 0x9c, 0x54,
			0x0e, 0x38, 0xdc, 0x92, 0xcb, 0x1f, 0x2a, 0x60,
			0x72, 0x61, 0x44, 0x51, 0x83, 0x23, 0x5a, 0xdb
		}, {
			0xab, 0xbe, 0xde, 0xa6, 0x80, 0x05, 0x6f, 0x52,
			0x38, 0x2a, 0xe5, 0x48, 0xb2, 0xe4, 0xf3, 0xf3,
			0x89, 0x41, 0xe7, 0x1c, 0xff, 0x8a, 0x78, 0xdb,
			0x1f, 0xff, 0xe1, 0x8a, 0x1b, 0x33, 0x61, 0x03,
			0x9f, 0xe7, 0x67, 0x02, 0xaf, 0x69, 0x33, 0x4b,
			0x7a, 0x1e, 0x6c, 0x30, 0x3b, 0x76, 0x52, 0xf4,
			0x36, 0x98, 0xfa, 0xd1, 0x15, 0x3b, 0xb6, 0xc3,
			0x74, 0xb4, 0xc7, 0xfb, 0x98, 0x45, 0x9c, 0xed
		}, {
			0x7b, 0xcd, 0x9e, 0xd0, 0xef, 0xc8, 0x89, 0xfb,
			0x30, 0x02, 0xc6, 0xcd, 0x63, 0x5a, 0xfe, 0x94,
			0xd8, 0xfa, 0x6b, 0xbb, 0xeb, 0xab, 0x07, 0x61,
			0x20, 0x01, 0x80, 0x21, 0x14, 0x84, 0x66, 0x79,
			0x8a, 0x1d, 0x71, 0xef, 0xea, 0x48, 0xb9, 0xca,
			0xef, 0xba, 0xcd, 0x1d, 0x7d, 0x47, 0x6e, 0x98,
			0xde, 0xa2, 0x59, 0x4a, 0xc0, 0x6f, 0xd8, 0x5d,
			0x6b, 0xca, 0xa4, 0xcd, 0x81, 0xf3, 0x2d, 0x1b
		}, {
			0x37, 0x8e, 0xe7, 0x67, 0xf1, 0x16, 0x31, 0xba,
			0xd2, 0x13, 0x80, 0xb0, 0x04, 0x49, 0xb1, 0x7a,
			0xcd, 0xa4, 0x3c, 0x32, 0xbc, 0xdf, 0x1d, 0x77,
			0xf8, 0x20, 0x12, 0xd4, 0x30, 0x21, 0x9f, 0x9b,
			0x5d, 0x80, 0xef, 0x9d, 0x18, 0x91, 0xcc, 0x86,
			0xe7, 0x1d, 0xa4, 0xaa, 0x88, 0xe1, 0x28, 0x52,
			0xfa, 0xf4, 0x17, 0xd5, 0xd9, 0xb2, 0x1b, 0x99,
			0x48, 0xbc, 0x92, 0x4a, 0xf1, 0x1b, 0xd7, 0x20
		}
	};

	//P-преобразование. Функция перестановки. Для каждой пары байт из входной последовательности происходит замена одного байта другим.
	const unsigned char tau[64]
	{
		0, 8, 16, 24, 32, 40, 48, 56,
		1, 9, 17, 25, 33, 41, 49, 57,
		2, 10, 18, 26, 34, 42, 50, 58,
		3, 11, 19, 27, 35, 43, 51, 59,
		4, 12, 20, 28, 36, 44, 52, 60,
		5, 13, 21, 29, 37, 45, 53, 61,
		6, 14, 22, 30, 38, 46, 54, 62,
		7, 15, 23, 31, 39, 47, 55, 63
	};

	unsigned char N[64];
	unsigned char h[64];
	vector<unsigned char> hash;
	unsigned char Sigma[64];
	bool hash512 = false;

	void addModulo512(unsigned char* newVal, unsigned char* source, unsigned char* add)
	{
		unsigned short intern = 0;
		//TODO: сдвиги влияют на глобальную переменную класса hash512, temp устраняет проблему
		bool temp = hash512 == true ? true : false;
		//(N+512) mod 2^512
		for (unsigned char i = 64; i > 0; i--)
		{
			intern = source[i] + add[i] + (intern >> 8);
			newVal[i] = intern & 0xff;//(unsigned char)intern;
		}
		this->hash512 = temp;
	}

	//X[k](a) = k ^ a
	void xor512(unsigned char* K, unsigned char* a, unsigned char* b)
	{
		for (unsigned char i = 0; i < 64; i++)
			K[i] = a[i] ^ b[i];

	}

	void S(unsigned char* K)
	{
		for (unsigned char i = 0; i < 64; i++)
			K[i] = pi[K[i]];

	}
	void P(unsigned char* K)
	{
		for (unsigned char i = 0; i < 64; i++)
			K[i] = K[tau[i]];

	}
	void L(unsigned char* K)
	{
		//На восьмибитный порции
		unsigned long long internal_out[8];
		memset(internal_out, 0x00, 64);
		//очередной бит равен 1, то ксорим очередное знач матр А с предыдущим
		for (int i = 7; i >= 0; i--)
			for (int j = 63; j >= 0; j--)
				if (((unsigned long long)K[i] >> j) & 1)
					internal_out[i] ^= A[63 - j];

		memcpy(K, internal_out, 64);
	}


	void keySchedule(unsigned char* K, unsigned char i)
	{
		xor512(K, K, C[i - 1]);//X
		S(K);
		P(K);
		L(K);
	}
	void E(unsigned char* state, unsigned char* k, unsigned char* m)
	{
		unsigned char K[64];

		memcpy(K, k, 64);

		xor512(state, K, m);

		for (unsigned char i = 1; i < 13; i++) {
			S(state);
			P(state);
			L(state);

			keySchedule(K, i);

			xor512(state, K, state);
		}
	}

	//функция сжатия
	void g(unsigned char* K, unsigned char* N, unsigned char* m)
	{
		unsigned char t[64];
		memcpy(t, K, 64);

		//K = h ^ N , вместо перем K перезапишем h
		xor512(K, K, N);

		S(K);// K = S(K)
		P(K);//K = P(K);
		L(K); // K = L(K);

		E(K, K, m);

		xor512(K, K, t); // t = h ^ t, в качестве t = K ,  
		xor512(K, K, m);// G = t ^ m, G = K, t = K
	}
	//[_g_0}

	void stribog(unsigned char* message, unsigned long long lenMessage)
	{
		unsigned char m[64];// последние 512 бит сообщения M

		unsigned char N_0[64] =
		{
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
				0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
		};
		unsigned char paddingMesToLength512 = 0;

		//BitConverter.GetBytes
		unsigned char N_512[64];
		for (int i = 0; i < 64; i++)
			N_512[i] = (unsigned char)((64 >> (8 * i)) & 0xff);
		reverse(begin(N_512), end(N_512));

		//if len > 512
		while (lenMessage >= 64)
		{
			memcpy(m, message + lenMessage - 64, 64);//коп 64 байта 1 блока на кот ссылается смс кот не превышает 64 байта в m

			g(h, N, m);

			addModulo512(N, N, N_512);
			addModulo512(Sigma, Sigma, m);

			lenMessage -= 64;
		}

		paddingMesToLength512 = 64 - lenMessage;

		if (paddingMesToLength512)
		{
			memset(m, 0x00, paddingMesToLength512 - 1);
			memset(m + paddingMesToLength512 - 1, 0x01, 1);
			memcpy(m + paddingMesToLength512, message, lenMessage);
		}

		g(h, N, m);

		unsigned char* mesLen = new unsigned char[lenMessage * 8];
		for (int i = 0; i < lenMessage * 8; i++)
			mesLen[i] = (unsigned char)(((lenMessage * 8) >> (8 * i)) & 0xff);
		reverse(mesLen + 0, mesLen + (lenMessage * 8));

		addModulo512(Sigma, Sigma, mesLen);
		addModulo512(Sigma, Sigma, m);

		g(h, N_0, N);
		g(h, N_0, Sigma);
	}

};
class myPoint
{
public:

	big_int x;
	big_int y;
	uBig_int a;
	uBig_int b;
	uBig_int modP;

	myPoint()
	{
		this->x = 0;
		this->y = 0;
		this->a = 0;
		this->b = 0;
		this->modP = 0;
	}
	myPoint(const myPoint &p)
	{
		this->x = p.x;
		this->y = p.y;
		this->a = p.a;
		this->b = p.b;
		this->modP = p.modP;
	}

	myPoint operator+(const myPoint &p2)
	{
		myPoint p3;
		p3.a = this->a;
		p3.b = this->b;
		p3.modP = this->modP;

		uBig_int dy = 0, dx = 0, lamda = 0;
		if (this->x != p2.x)
		{
			dy = p2.y - this->y;
			dx = p2.x - this->x;
			if (dx < 0)
				dx += this->modP;
			if (dy < 0)
				dy += this->modP;

			lamda = (dy * modInverse(dx, this->modP)) % this->modP;
			if (lamda < 0)
				lamda += this->modP;

			p3.x = (lamda * lamda - this->x - p2.x) % this->modP;
			p3.y = (lamda * (this->x - p3.x) - this->y) % this->modP;
			if (p3.x < 0)
				p3.x += this->modP;
			if (p3.y < 0)
				p3.y += this->modP;
		}
		return p3;
	}
	static myPoint calculatePointDoubling(const myPoint &p1)
	{
		uBig_int dy = 0, dx = 0, lamda = 0;
		myPoint p2;
		p2.a = p1.a;
		p2.b = p1.b;
		p2.modP = p1.modP;

		dy = 2 * p1.y;
		dx = p1.x * p1.x * 3 + p1.a;
		if (dx < 0)
			dx += p1.modP;
		if (dy < 0)
			dy += p1.modP;

		lamda = (dx * modInverse(dy, p1.modP)) % p1.modP;

		p2.x = (lamda * lamda - 2 * p1.x) % p1.modP;
		p2.x = (lamda * (p1.x - p2.x) - p1.y) % p1.modP;
		if (p2.x < 0)
			p2.x += p1.modP;
		if (p2.y < 0)
			p2.y += p1.modP;

		return p2;
	}
	static myPoint calculatePointMulty(uBig_int num, myPoint p1)
	{
		myPoint temp(p1);

		num = num - 1;
		while (num != 0)
		{
			if ((num % 2) != 0)
			{
				if ((temp.x == p1.x) || (temp.y == p1.y))
				{
					temp = calculatePointDoubling(temp);
				}
				else
				{
					temp = temp.operator+(p1);
				}
				num = num - 1;
			}
			num = num / 2;
			p1 = calculatePointDoubling(p1);
		}
		return temp;
	}
};
class DSGOST
{
public:
	DSGOST(uBig_int p,uBig_int a, uBig_int b, uBig_int q, vector<uByte> xG)
	{
		this->a = a;
		this->b = b;
		this->p = p;
		this->q = q;
		this->xG.clear();
		this->xG.insert(this->xG.begin(), xG.begin(), xG.end());
	}

	uBig_int genPrivateKey(big_int bitSize)
	{
		uBig_int d = 0;
		do
		{
			d = rand() % q + 1;
		} while (d <= 0);
		return d;
	}

	myPoint genPublicKey(uBig_int d) // Q = dP
	{
		myPoint G = GDecompression();
		myPoint Q;
		Q = myPoint::calculatePointMulty(d, G);
		return Q;
	}

	//Выполняет модульное деление числа, возведенного в степень другого числа.
	big_int modPow(big_int number, big_int exponent, big_int modulus)
	{
		big_int a = power(number, (exponent - 1) / 2);
		return a % modulus;
	}
	// вычисление элемента лежандра
	big_int legendre(big_int a, big_int q)
	{
		return modPow(a,(q-1)/2,q);
	}
	//функция вычисления квадратоного корня по модулю простого числа q
	big_int modSqrt(big_int a, big_int q)
	{
		big_int b = 0;
		do
		{
			//b.genRandomBits(255, new Random());
			b = rand() % 255 + 1;
		} while (legendre(b, q) == 1);
		big_int s = 0;
		big_int t = q - 1;
		while ((t & 1) != 1)
		{
			s++;
			t = t >> 1;
		}
		big_int invA = modInverse(a, q);
		big_int c = modPow(b, t, q);
		big_int r = modPow(a, (t + 1) / 2, q);
		big_int d = 0;
		for (int i = 0; i < s; i++)
		{
			big_int temp = 2;
			temp = modPow(temp, (s - i - 1), q);
			d = modPow((modPow(r, 2, q) * invA), temp, q);
			if (d == (q - 1))
				r = (r * c) % q;
			c = modPow(c, 2, q);
		}
		return r;
	}

	string signGen(vector<uByte> h, uBig_int d)
	{
		uBig_int alpha = calculateAlpha(h);
		uBig_int e = alpha % q;
		if (e == 0)
			e = 1;
		uBig_int k = 0;
		myPoint C;

		big_int r = 0;
		big_int s = 0;
		do
		{
			do
			{
				//k.genRandomBits(n.bitCount(), new Random()); BigInteger
				k = rand() % 255 + 1;
			} while ((k < 0) || (k > q));
			C = myPoint::calculatePointMulty(k, G);
			r = C.x % q;
			s = ((r * d) + (k * e)) % q;
		} while ((r == 0) || (s == 0));

		vector<uBig_int> tempVector;
		tempVector.push_back(r);
		string rVector = padding(bigIntToHexString(tempVector), countOfSetBits(q) / 4);
		tempVector.pop_back();
		tempVector.push_back(s);
		string sVector = padding(bigIntToHexString(tempVector), countOfSetBits(q) / 4);
		return rVector + sVector;
	}

	bool signVerification(vector<uByte> H, string sign, myPoint Q)
	{
		string rVector = sign.substr(0, countOfSetBits(q) / 4);
		string sVector = sign.substr(countOfSetBits(q) / 4, countOfSetBits(q) / 4);
		big_int r = hexStringToInt(rVector);
		big_int s = hexStringToInt(sVector);
		
		if ((r < 1) || (r > (q - 1)) || (s < 1) || (s > (q - 1)))
			return false;
		uBig_int alpha = calculateAlpha(H);
		uBig_int e = alpha % q;
		if (e == 0)
			e = 1;
		big_int v = modInverse(e, q);
		big_int z1 = (s * v) % q;
		big_int z2 = ((-r * v)) % q;// author has q + ((-r ..) ?
		this->G = GDecompression();
		myPoint A = myPoint::calculatePointMulty(z1, G);
		myPoint B = myPoint::calculatePointMulty(z2, Q);
		myPoint C = A + B;
		uBig_int R = C.x % q;
		if (R == r)
			return true;
		else
			return false;
	}

private:
	uBig_int p = 0;
	uBig_int a = 0;
	uBig_int b = 0;
	uBig_int q = 0;
	vector<uByte> xG;
	myPoint G;

	
	//Восстанавливаем координату y из координаты x и бита четности y 
	myPoint GDecompression()
	{
		uByte y = xG[0];
		vector<uByte> x;
		x.resize(xG.size() - 1);
		copy(begin(xG), end(xG), begin(x));
		x.erase(x.begin());
		big_int xCord = hexByteToInt(x);
		big_int temp = (xCord * xCord * xCord + a * xCord + b) % p;
		big_int beta = modSqrt(temp, p);
		big_int yCord = 0;
		if ((beta % 2) == (y % 2))
			yCord = beta;
		else
			yCord = p - beta;
		myPoint G;
		G.a = a;
		G.b = b;
		G.modP = p;
		G.x = xCord;
		G.y = yCord;
		this->G = G;
		return G;
	}

	string padding(string input, int size)
	{
		if (input.length() < size)
		{
			do
			{
				input = "0" + input;
			} while (input.length() < size);
		}
		return input;
	}
};


//****GOST => Digital signature = vector bin code Get bin code(beautiful view)*****************

vector<uByte> correctBin(big_int r, big_int s)
{
	bitset<32> binR(r);
	bitset<32> binS(s);
	//cout << binR << endl;
	//cout << binS << endl;
	vector<unsigned char> view;
	putBin(view, binR);
	putBin(view, binS);
	return view;

}
//****************************************************************

/*For Example simple*/
uBig_int p = 0; 
uBig_int a = 0, b = 0;
uBig_int q = 0;
uBig_int d = 0;
vector<uByte> sign;
/*******************/

int main()
{
	srand(time(0));
	createPrimes();

	//Example Habr
	uBig_int p = 6277101735386680763835789423207666416083908700390324961279;
	//a & b include Fp , where Fp - конечное простое поле, простые числа не мб отрицательными ! BigInteger a = new BigInteger("-3", 10);
	uBig_int a = -3;
	uBig_int b = hexStringToInt("64210519e59c80e70fa7e9ab72243049feb8deecc146b9b1");
	vector<uByte> xG = hexStringToByte("03188da80eb03090f67cbf20eb43a18800f4ff0afd82ff1012");
	uBig_int q = hexStringToInt("ffffffffffffffffffffffff99def836146bc9b1b4d22831"); // ГОСТ зависит от используемого хэша
	DSGOST DS(p, a, b, q, xG);
	uBig_int d = DS.genPrivateKey(192);
	myPoint Q = DS.genPublicKey(d);
	int typeHash = 256;
	unsigned char message[500] =
	{
		0x32,0x31,0x30,0x39,0x38,0x37,0x36,0x35,0x34,0x33,0x32,0x31,0x30,0x39,0x38,0x37,
		0x36,0x35,0x34,0x33,0x32,0x31,0x30,0x39,0x38,0x37,0x36,0x35,0x34,0x33,0x32,0x31,
		0x30,0x39,0x38,0x37,0x36,0x35,0x34,0x33,0x32,0x31,0x30,0x39,0x38,0x37,0x36,0x35,
		0x34,0x33,0x32,0x31,0x30,0x39,0x38,0x37,0x36,0x35,0x34,0x33,0x32,0x31,0x30
	};
	Stribog hashStribog(typeHash);
	vector<unsigned char> hash = hashStribog.getHash(message);
	hashStribog.showHash(hash);
	string sign = DS.signGen(hash, d);
	bool verification = DS.signVerification(hash, sign, Q);

	//Example simple
	/*
	uBig_int randomNumber = 0;
	randomNumber = rand() % primes.size() + 3;
	::p = primes[randomNumber];
	printf("p = %llu", ::p);
	cout << endl;

	do
	{
		do
		{
			::a = rand() % ::p + 1;
			::b = rand() % ::p + 1;
			if (::a == ::b)
				continue;
			if (primality_check(::a) == false && primality_check(::b) == false)
				break;
		} while (true);
		if ((4 * ::a * ::a * ::a + 27 * ::b * ::b) % ::p != 0)
			break;
	} while (true);
	printf("a = %llu ", ::a);
	printf("b = %llu ", ::b);
	cout << endl;
	
	myPoint P;
	do
	{
		P.x = rand() % ::p;
	} while (P.x == 0);

	printf("x1 = %lld ", P.x);
	cout << endl;

	P.y = (P.x * P.x * P.x + ::a * P.x + ::b) % ::p;
	P.y *= P.y;
	printf("y1 = %llu ", P.y);
	cout << endl;

	uBig_int J_E = ((1728 * 4 * ::a * ::a * ::a) / (4 * ::a * ::a * ::a + 27 * ::b * ::b)) % ::p;
	printf("J(E) = %llu ", J_E);
	cout << endl;
	if (J_E == 0 || J_E == 1728)
		throw (runtime_error("Invariant(J_E) violated condition (!=0 & != 1728)!"));
	myPoint p3;

	if (typeHash == 512)
	{
		
		do
		{
			//generate random between 2^508 and 2^518
			::q = rand() % power(2,typeHash+6) + power(2,typeHash-4);
			if (myPoint::calculatePointMulty(::q, P).x != 0 && myPoint::calculatePointMulty(::q, P).y != 0)
				continue;
		} while (primality_check(::q));
	}
	else
	{
		do
		{
			//generate random between 2^254 and 2^256
			::q = rand() % power(2, typeHash) + power(2, typeHash - 2);
			if (myPoint::calculatePointMulty(::q, P).x != 0 && myPoint::calculatePointMulty(::q, P).y != 0)
				continue;
		} while (primality_check(::q));
	}

	uBig_int n = static_cast<uBig_int>(rand() % LLONG_MAX) + 1;
	uBig_int m = n * q; //m != p
	while (m == ::p)
	{
		n = static_cast<uBig_int>(rand() % LLONG_MAX) + 1;
		m = n * ::q;
	}
	::d = rand() % ::q + 1;
	printf("d = %llu ", ::d);
	printf("q = %lld ", ::q);
	printf("n = %llu ", n);
	printf("m = %lld ", m);
	cout << endl;

	uBig_int alpha = calculateAlpha(hash);
	printf("alfa = %lld \n", alpha);
	uBig_int e = alpha % ::q;
	if (e == 0)
		e = 1;
	printf("e = %lld \n", e);

	uBig_int k = 0;
	big_int r = 0;
	big_int s = 0;
	myPoint G;
	do
	{
		k = rand() % (q-1) + 1;
		G = myPoint::calculatePointMulty(k, P);
		r = G.x % ::q;
		if (r == 0)
			continue;
		s = (r * d + k * e) % ::q;
		if (s != 0 && r != 0)
			break;
	} while (true);
	printf("k = %lld ", k);
	printf("r = %lld ", r);
	printf("s = %lld ", s);
	printf("x3 = %lld ", G.x);
	printf("y3 = %lld ", G.y);
	cout << endl;

	cout << " SIGN : ";
	::sign = correctBin(r,s);
	for (big_int i = 0; i < ::sign.size(); i++)
		cout << big_int(::sign[i]);
	cout << endl;
	
	//Check sing
	myPoint Qkey = myPoint::calculatePointMulty(::d, P);
	big_int v = 0;
	v = modInverse(e,::q);
	printf("v = %lld \n", v);

	big_int z1 = (s * v) % ::q;
	big_int z2 = ((-r * v) % ::q); 
	printf(" z1 = %lld \n", z1);
	printf(" z1 = %lld \n", z2);
	myPoint A = myPoint::calculatePointMulty(z1, P);
	myPoint B = myPoint::calculatePointMulty(z2, Qkey);
	myPoint C = A + B;
	printf("Cx = %lld \n", C.x);

	long long R = C.x % ::q;
	printf(" R = %lld ", R);
	if (R == r)
		cout << " Good!";
	else
		cout << "Bad!";
	*/
}
