#include <iostream>
#include <string>
#include <sstream>
#include <random>
#include <fstream>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

#define A 5870873747  //pirminiai skaiciai 
#define B 6863349307   
#define C 9698977879   
#define seed 4033081603 

string hashFunction(string text)
{
    unsigned long long h = seed;
    if (text.length()==0)
    {
        return "ac5171ee1fcf8ad4a986e4d6ab3c0e3c2eb38b79fb557376ea497bee17f8dd7e";//random hashas
    }
    
    if (text.length()<32)
    {
        unsigned int sum = 0;
        for (int i = 0; i < text.length(); i++)
        {
            sum+=text[i];
        }
        srand(text.length()+sum);
        for (int i = text.length()-1; i < 32; i++)
        {
            text+=std::to_string(256+rand()%C);
        }    
    }
    std::stringstream ss;
    for (int i = 0; i < text.length(); i++)
    {
        h = (h * A) ^ (text[i] * B);
        ss << std::hex <<h%C;
        
    }
    string hash = "";
    string all_chars = ss.str();
    for (int i = all_chars.length()-1; i > all_chars.length()-65 ; i--)
    {
        hash+=all_chars[i];
    }
    return hash;
}   

int main(int argc, char const *argv[])
{
    string text="";
    for (int i = 1; i < argc; i++)
    {
        std::fstream f(argv[i],std::ios::in);
        try
        {
            if (!f)
            {
                throw "Nepavyko atidaryti failo!";
            }
        }
        catch (const char *zinute)
        {
            std::cerr << zinute << '\n';
            exit(1);
        }
        std::stringstream ss;
        ss << f.rdbuf();
        text = ss.str();
        cout << hashFunction(text)<< endl;
    }
    if (argc==1)
    {
        cin >> text;
        cout << hashFunction(text)<< endl;
    }
    // for (int i = 0; i < 65; i++)
    // {
    //   text =hashFunction(text);
    //   cout << text << endl;
    // }
    return 0;
}
