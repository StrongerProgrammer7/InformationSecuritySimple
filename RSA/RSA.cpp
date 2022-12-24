#include <iostream>
#include <string>
#include <cmath>
#include <vector>
#include <numeric>
#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::cpp_int;
using namespace std;

unsigned p; 
unsigned q;
unsigned n; //n=p*q
unsigned phi_n; //phi_n=(p-1)*(q-1)
unsigned e; // 1 < e < phi_n; e * phi_n coprime
unsigned d; // (e*d) mod phi_n =1
//unsigned m; //secr message

//boost::multiprecision::cpp_int c;//ciphertext
//boost::multiprecision::cpp_int m_0; //result of decryption
//e - encrypt, d- decrypt
// p q phi n d - close key
// e n n - public key

/*----------------------Key generate ------------------------------*/
void prompter()
{
	cout << " Enter prime number p: " << endl;
	cin >> p;
	cout << " Enter prime number q: " << endl;
	cin >> q;

}

void primality_check()
{
	vector<unsigned> p_factors;
	for (unsigned i = 2; i <= static_cast<unsigned>(sqrt(p)); i++)
	{
		if (p % i == 0)
		{
			p_factors.push_back(i);
			p_factors.push_back(p / i);
		}
	}

	vector<unsigned> q_factors;
	for (unsigned i = 2; i <= static_cast<unsigned>(sqrt(q)); i++)
	{
		if (q % i == 0)
		{
			q_factors.push_back(i);
			q_factors.push_back(q / i);
		}
	}

	if (p_factors.size() != 0 || q_factors.size() != 0)
		throw (runtime_error("none prime!"));
}

unsigned calculate_n()
{
	n = p * q;
	return n;
}

unsigned calculate_phi()
{
	phi_n = (p - 1) * (q - 1);
	return phi_n;
}

unsigned gcd(unsigned x, unsigned phi)
{
	while (phi != 0)
	{
		unsigned r = x % phi;
		x = phi;
		phi = r;
	}
	return x;
}

unsigned calculate_e()
{
	unsigned i = 2;
	while (gcd(i, phi_n) != 1)
	{
		i++;
	}
	e = i;
	if (e >= phi_n)
		throw (runtime_error("e is greater than or equal to phi_n"));
	return e;
}
unsigned calculate_d()
{
	unsigned k = 1;
	while (((k * phi_n) + 1) % e != 0)
		k++;
	d = ((k * phi_n) + 1) / e;

	return d;
}


/*---------------------- end Key generate ------------------------------*/

boost::multiprecision::cpp_int encrypt(unsigned message)
{
	//m^e mod n = cipher
	//cout << " Enter message " << endl;
	//cin >> m;
	boost::multiprecision::cpp_int c = boost::multiprecision::cpp_int(boost::multiprecision::cpp_int(pow(boost::multiprecision::cpp_int(message),e))%n);
	return c;

}

boost::multiprecision::cpp_int decrypt(boost::multiprecision::cpp_int cipher)
{
	//c^d mod n = text

	boost::multiprecision::cpp_int m_0 = boost::multiprecision::cpp_int(pow(boost::multiprecision::cpp_int(cipher), d)) % n;
	return m_0;
}

int main()
{
   //Encrypt c=(M^e) Mod n
	//Decrypt M = (c^d) mod n
	try
	{
		//53 61 
		prompter();
		primality_check();
		cout << "n = " << calculate_n() << endl;
		cout << " phi " << calculate_phi() << endl;
		cout << " e " << calculate_e() << endl;
		cout << " d " << calculate_d() << endl;

		string text = "Iron man";
		boost::multiprecision::cpp_int arrayEncrypt[1000];
		for (int i = 0; i < 1000; i++)
			arrayEncrypt[i] = 0;
		
		for (int i = 0; i < text.length(); i++)
		{
			char sym = text[i];
			arrayEncrypt[i] = encrypt((int)sym);
		}

		//ecrypt		
		cout << " Encrypt " << endl;
		for (int i = 0; i < text.length(); i++)
			cout << arrayEncrypt[i] << " ";

		cout << endl;
		string decryptText = "";
		//decrypt
		for (int i = 0; i < text.length(); i++)
			decryptText += (char)decrypt(arrayEncrypt[i]);
		
		cout << decryptText << endl;
		/*cout << " Ecrypt " << encrypt(89) << endl;
		cout << " Decrypt " << decrypt(encrypt(89)) << endl;*/
	}
	catch (runtime_error & err)
	{
		cout << err.what() << endl;
	}
}

