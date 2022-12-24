#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <numeric>
#include <cmath>
#include <boost/multiprecision/cpp_int.hpp>

using boost::multiprecision::cpp_int;
using namespace std;

boost::multiprecision::cpp_int power(boost::multiprecision::cpp_int a, boost::multiprecision::cpp_int b, unsigned n) // a^b mod n
{
    boost::multiprecision::cpp_int temp = a;
    boost::multiprecision::cpp_int sum = temp;
    for (unsigned i = 1; i < b; i++)
        sum *= temp;
    if(sum < 0 )
        throw (runtime_error("Negative number!"));
    return sum % n;
    /*for (unsigned i = 1; i < b; i++)
    {
        for (unsigned j = 1; j < a; j++)
        {
            sum += temp;
            if (sum > n)
                sum -= n;
        }
        temp = sum;
    }
    return temp;*/
}

boost::multiprecision::cpp_int mul(boost::multiprecision::cpp_int a, boost::multiprecision::cpp_int b, unsigned n) //a*b mod n
{
   // boost::multiprecision::cpp_int sum = 0;
    
   /* for (unsigned i = 0; i < b; i++)
    {
        sum += a;
        if (sum >= n)
        {
            sum -= n;
        }
    }*/

    return (a*b)%n;
}

unsigned gcd(unsigned x, unsigned y)
{
    while (y != 0)
    {
        unsigned r = x % y;
        x = y;
        y = r;
    }
    return x;
}


void primality_check(unsigned p, unsigned g)
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

    vector<unsigned> g_factors;
    for (unsigned i = 2; i <= static_cast<unsigned>(sqrt(g)); i++)
    {
        if (g % i == 0)
        {
            g_factors.push_back(i);
            g_factors.push_back(g / i);
        }
    }

    if (p_factors.size() != 0 || g_factors.size() != 0)
        throw (runtime_error("none prime!"));
}

boost::multiprecision::cpp_int calculate_k(boost::multiprecision::cpp_int k,unsigned p)
{
    k = rand() % (p - 2) + 1;
    while (gcd(k, p-1) != 1)
        k++;

    if(k<0)
        throw (runtime_error("K < 0 !"));
    return k;
}

//p simple, 0 < g < p -1, 0 < x < p - 1, m - cipher message m < p
void encrypt(unsigned p, unsigned g, unsigned x)
{
    ifstream inf("asciSymbols.txt");
    ofstream out("encrypt.txt");

    // Y = (g^x) mod p
    boost::multiprecision::cpp_int y = power(g, x, p);

    cout << " Public key (p,g,y)= " << p << " " << g << " " << y << endl;
    cout << " Private key = " << x << endl;

    while (!inf.eof())
    {
        boost::multiprecision::cpp_int m = 0;
        inf >> m;
        if (m > 0)
        {
            cout << (char)m;

            boost::multiprecision::cpp_int k = 0;
            k = calculate_k(k, p); // Сессионный ключ (1 < k < p-1) взаимно простое с p-1
            boost::multiprecision::cpp_int a = power(g, k, p); // A = (g^k) mod p
            boost::multiprecision::cpp_int b = mul(power(y, k, p), m, p); // B = (Y^k)*M mod p
            cout << " a = " << a << " b=" << b << endl;
            out << a << " " << b << " ";
        }
    }

    cout << endl;
    inf.close();
    out.close();
}

void decrypt(unsigned p, unsigned x)
{
    ifstream inp("encrypt.txt");
    ofstream out("decrypt.txt");

    cout << " Decrypt " << endl;

    while (!inp.eof())
    {
        boost::multiprecision::cpp_int a = 0, b = 0;
        inp >> a;
        inp >> b;

        if (a != 0 && b != 0)
        {
            //cout << a << " " << b << endl;
            boost::multiprecision::cpp_int deM = mul(b, power(a, p - 1 - x, p), p); //m=b*(a^x)^(-1)mod p=b*a^(p-1-x)mod p or M=B*A^(p-1-x) mod p
            char m = static_cast<char>(deM);
            out << m;
            cout << m;
        }
    }

    cout << endl;
    inp.close();
    out.close();
}
int main()
{
    string text = "Iron man!";
    ofstream asciSymbols("asciSymbols.txt");

    for (int i = 0; i < text.length(); i++)
    {
        char sym = text[i];
        asciSymbols << (int)sym << " ";
    }
    asciSymbols.close();
    /*
    * Выбирается большое простое число p & g, 1<g<p
    * Выбирается случайное число X 1<X<p
    * 
    */
    unsigned p = 0, g = 0, x = 0;
    p = 593;
    g = 127;
    x = 32;
    if (g > p)
        throw (runtime_error("G grater than p!"));

    primality_check(p, g);

    srand(time(NULL));
    encrypt(p,g,x);
    decrypt(p, x);
}

