#include <iostream>
#include <string>
#include <sstream>
#include <random>
#include <fstream>
#include <vector>
#include <chrono>
#include <cstring>
#include <bitset>
#include <iomanip>
#include "cryptoLib/hex.h"
#include "cryptoLib/files.h"
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include "cryptoLib/md5.h"
#include "cryptoLib/sha.h"

using std::bitset;
using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using namespace CryptoPP;

#define A 5870873747 //pirminiai skaiciai
#define B 6863349307
#define C 9698977879
#define seed 4033081603

const int diffN = 2;
const int randomN = 2;
const int SymbolsN = 10000;

int CompareStrings(const string &a, const string &b)
{
    int matched = 0;
    for (int i = 0; i < a.length(); i++)
    {
        if (a[i] == b[i])
        {
            matched++;
        }
    }
    return matched;
}

string hashFunction(string text)
{
    unsigned long long h = seed;
    if (text.length() == 0)
    {
        return "ac5171ee1fcf8ad4a986e4d6ab3c0e3c2eb38b79fb557376ea497bee17f8dd7e"; //random hashas
    }
    if (text.length() < 32)
    {
        unsigned long long num = 263239;
        for (int i = 0; i < text.length(); i++)
        {
            num = (num * 5) ^ (text[i]);
        }
        srand(num);
        for (int i = text.length() - 1; i < 32; i++)
        {
            text += std::to_string(rand() % num);
        }
    }
    std::stringstream ss;
    for (int i = 0; i < text.length(); i++)
    {
        h = (h * A) ^ (text[i] * B);
        ss << std::hex << h % C;
    }
    string hash = "";
    string all_chars = ss.str();
    for (int i = all_chars.length() - 1; i > all_chars.length() - 65; i--)
    {
        hash += all_chars[i];
    }
    return hash;
}
void CreateTestFiles()
{
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(33, 126);
    std::fstream f;
    std::stringstream ss;
    f.open("1a.txt", std::ios::out);
    ss << "a";
    f << ss.str();
    f.close();
    ss.clear();
    f.open("1g.txt", std::ios::out);
    ss << "g";
    f << ss.str();
    f.close();
    ss.clear();
    string FileName = "random1.txt";
    int nr = 0;
    for (int i = 0; i < randomN; i++)
    {
        for (int j = 0; j < SymbolsN; j++)
        {
            ss << (char)dist(mt);
        }
        FileName[6] = char(++nr) + '0';
        f.open(FileName, std::ios::out);
        f << ss.str();
        f.close();
        ss.clear();
    }
    int previous = 65;
    nr = 1;
    FileName = "differentByOne1.txt";
    for (int i = 0; i < SymbolsN; i++)
    {
        ss << (char)dist(mt);
    }
    f.open(FileName, std::ios::out);
    string temp = ss.str();
    temp[500] = 'a';
    f << temp;
    f.close();
    for (int i = 0; i < diffN; i++)
    {
        FileName[14] = char(nr) + '0';
        f.open(FileName, std::ios::out);
        if (i != 2)
        {
            temp[500] = char(++previous);
        }
        f << temp;
        f.close();
        nr++;
    }
    ss.clear();
    f.open("empty.txt", std::ios::out);
    f.close();
}

int main(int argc, char const *argv[])
{
    std::ifstream f;
    f.open("1a.txt");
    if (!f)
    {
        CreateTestFiles();
    }

    string text = "";
    if (argc != 1)
    {

        if (strcmp(argv[1], "-k") != 0 && strcmp(argv[1], "-pairs") != 0 && strcmp(argv[1], "-pairsDiff") != 0 && strcmp(argv[1], "-md5") != 0 && strcmp(argv[1], "-sha256") != 0 && strcmp(argv[1], "-f") != 0)
        {
            cout << "Invalid command!" << endl;
            exit(1);
        }

        if (strcmp(argv[1], "-f") == 0)
        {
            for (int i = 2; i < argc; i++)
            {
                std::fstream f(argv[i], std::ios::in);
                try
                {
                    if (!f)
                    {
                        throw "Nepavyko atidaryti failo";
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
                cout << argv[i] << ": " << hashFunction(text) << endl;
            }
        }
        if (strcmp(argv[1], "-k") == 0)
        {
            std::fstream f("konstitucija.txt", std::ios::in);
            try
            {
                if (!f)
                {
                    throw "Nepavyko atidaryti failo";
                }
            }
            catch (const char *zinute)
            {
                std::cerr << zinute << '\n';
                exit(1);
            }
            std::stringstream ss;
            ss << f.rdbuf();
            string line;
            auto start = std::chrono::high_resolution_clock::now();
            while (getline(ss, line))
            {
                hashFunction(line);
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;
            cout << "Konstitucijos hash'avimas po eilutę užtruko: " << diff.count() << " s" << endl;
        }
        if (strcmp(argv[1], "-pairs") == 0)
        {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> dist(33, 126);
            vector<string> pairs_10;
            vector<string> pairs_100;
            vector<string> pairs_500;
            vector<string> pairs_1000;
            for (int i = 0; i < 50000; i++)
            {
                string temp;
                for (int j = 0; j < 10; j++)
                {
                    temp += char(dist(mt));
                }
                pairs_10.push_back(temp);
            }
            for (int i = 0; i < 50000; i++)
            {
                string temp;
                for (int j = 0; j < 100; j++)
                {
                    temp += char(dist(mt));
                }
                pairs_100.push_back(temp);
            }
            for (int i = 0; i < 50000; i++)
            {
                string temp;
                for (int j = 0; j < 500; j++)
                {
                    temp += char(dist(mt));
                }
                pairs_500.push_back(temp);
            }
            for (int i = 0; i < 50000; i++)
            {
                string temp;
                for (int j = 0; j < 1000; j++)
                {
                    temp += char(dist(mt));
                }
                pairs_1000.push_back(temp);
            }
            int matched = 0;
            for (int i = 0; i < 50000; i += 2)
            {
                if (hashFunction(pairs_10[i]) == hashFunction(pairs_10[i + 1]))
                {
                    matched++;
                }
            }
            for (int i = 0; i < 50000; i += 2)
            {
                if (hashFunction(pairs_100[i]) == hashFunction(pairs_100[i + 1]))
                {
                    matched++;
                }
            }
            for (int i = 0; i < 50000; i += 2)
            {
                if (hashFunction(pairs_500[i]) == hashFunction(pairs_500[i + 1]))
                {
                    matched++;
                }
            }
            for (int i = 0; i < 50000; i += 2)
            {
                if (hashFunction(pairs_1000[i]) == hashFunction(pairs_1000[i + 1]))
                {
                    matched++;
                }
            }
            cout << "Iš viso sutapo: " << matched << endl;
            cout << "Sutapimo procentas: " << matched / 100000.0 * 100 << endl;
        }
        if (strcmp(argv[1], "-pairsDiff") == 0)
        {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> dist(33, 126);
            vector<string> pairs_10;
            vector<string> pairs_100;
            vector<string> pairs_500;
            vector<string> pairs_1000;
            string temp;
            char previous;
            for (int i = 1; i <= 50000; i++)
            {

                if (i % 2 == 0)
                {

                    previous == '!' ? temp[0] = char(int(previous) + 1) : previous == '~' ? temp[0] = char(int(previous) - 1)
                                                                                          : temp[0] = char(int(previous) + 1);
                    pairs_10.push_back(temp);
                    continue;
                }
                temp.clear();
                for (int j = 0; j < 10; j++)
                {
                    temp += char(dist(mt));
                }
                previous = temp[0];
                pairs_10.push_back(temp);
            }
            for (int i = 1; i <= 50000; i++)
            {

                if (i % 2 == 0)
                {

                    previous == '!' ? temp[0] = char(int(previous) + 1) : previous == '~' ? temp[0] = char(int(previous) - 1)
                                                                                          : temp[0] = char(int(previous) + 1);
                    pairs_100.push_back(temp);
                    continue;
                }
                temp.clear();
                for (int j = 0; j < 100; j++)
                {
                    temp += char(dist(mt));
                }
                previous = temp[0];
                pairs_100.push_back(temp);
            }
            for (int i = 1; i <= 50000; i++)
            {

                if (i % 2 == 0)
                {

                    previous == '!' ? temp[0] = char(int(previous) + 1) : previous == '~' ? temp[0] = char(int(previous) - 1)
                                                                                          : temp[0] = char(int(previous) + 1);
                    pairs_500.push_back(temp);
                    continue;
                }
                temp.clear();
                for (int j = 0; j < 500; j++)
                {
                    temp += char(dist(mt));
                }
                previous = temp[0];
                pairs_500.push_back(temp);
            }
            for (int i = 1; i <= 50000; i++)
            {

                if (i % 2 == 0)
                {

                    previous == '!' ? temp[0] = char(int(previous) + 1) : previous == '~' ? temp[0] = char(int(previous) - 1)
                                                                                          : temp[0] = char(int(previous) + 1);
                    pairs_1000.push_back(temp);
                    continue;
                }
                temp.clear();
                for (int j = 0; j < 1000; j++)
                {
                    temp += char(dist(mt));
                }
                previous = temp[0];
                pairs_1000.push_back(temp);
            }
            int matched_hex = 0;
            int matched_binary = 0;
            string h1 = hashFunction(pairs_10[0]);
            string h2 = hashFunction(pairs_10[1]);
            int hex_min, hex_max = CompareStrings(h1, h2);
            matched_hex += hex_max;
            string h1Inbits = "";
            string h2Inbits = "";
            for (int i = 0; i < 64; i++)
            {
                h1Inbits += bitset<4>(h1[i]).to_string();
            }
            for (int i = 0; i < 64; i++)
            {
                h2Inbits += bitset<4>(h2[i]).to_string();
            }
            int binary_min, binary_max = CompareStrings(h1Inbits, h2Inbits);
            matched_binary += binary_max;
            for (int i = 2; i < 50000; i += 2)
            {

                string hash1 = hashFunction(pairs_10[i]);
                string hash2 = hashFunction(pairs_10[i + 1]);
                int num = CompareStrings(hash1, hash2);
                if (num > hex_max)
                {
                    hex_max = num;
                }
                if (num < hex_min)
                {
                    hex_min = num;
                }
                matched_hex += num;
                string hash1Inbits = "";
                string hash2Inbits = "";
                for (int i = 0; i < 64; i++)
                {
                    hash1Inbits += bitset<4>(hash1[i]).to_string();
                }
                for (int i = 0; i < 64; i++)
                {
                    hash2Inbits += bitset<4>(hash2[i]).to_string();
                }
                int num2 = CompareStrings(hash1Inbits, hash2Inbits);;
                if (num2 > binary_max)
                {
                    binary_max = num2;
                }
                if (num2 < binary_min)
                {
                    binary_min = num2;
                }
                matched_binary += num2;
            }
            for (int i = 0; i < 50000; i += 2)
            {
                string hash1 = hashFunction(pairs_10[i]);
                string hash2 = hashFunction(pairs_10[i + 1]);
                int num = CompareStrings(hash1, hash2);
                if (num > hex_max)
                {
                    hex_max = num;
                }
                if (num < hex_min)
                {
                    hex_min = num;
                }
                matched_hex += num;
                string hash1Inbits = "";
                string hash2Inbits = "";
                for (int i = 0; i < 64; i++)
                {
                    hash1Inbits += bitset<4>(hash1[i]).to_string();
                }
                for (int i = 0; i < 64; i++)
                {
                    hash2Inbits += bitset<4>(hash2[i]).to_string();
                }
                int num2 = CompareStrings(hash1Inbits, hash2Inbits);;
                if (num2 > binary_max)
                {
                    binary_max = num2;
                }
                if (num2 < binary_min)
                {
                    binary_min = num2;
                }
                matched_binary += num2;
            }
            for (int i = 0; i < 50000; i += 2)
            {
                string hash1 = hashFunction(pairs_10[i]);
                string hash2 = hashFunction(pairs_10[i + 1]);
                int num = CompareStrings(hash1, hash2);
                if (num > hex_max)
                {
                    hex_max = num;
                }
                if (num < hex_min)
                {
                    hex_min = num;
                }
                matched_hex += num;
                string hash1Inbits = "";
                string hash2Inbits = "";
                for (int i = 0; i < 64; i++)
                {
                    hash1Inbits += bitset<4>(hash1[i]).to_string();
                }
                for (int i = 0; i < 64; i++)
                {
                    hash2Inbits += bitset<4>(hash2[i]).to_string();
                }
                int num2 = CompareStrings(hash1Inbits, hash2Inbits);;
                if (num2 > binary_max)
                {
                    binary_max = num2;
                }
                if (num2 < binary_min)
                {
                    binary_min = num2;
                }
                matched_binary += num2;
            }
            for (int i = 0; i < 50000; i += 2)
            {
                string hash1 = hashFunction(pairs_10[i]);
                string hash2 = hashFunction(pairs_10[i + 1]);
                int num = CompareStrings(hash1, hash2);
                if (num > hex_max)
                {
                    hex_max = num;
                }
                if (num < hex_min)
                {
                    hex_min = num;
                }
                matched_hex += num;
                string hash1Inbits = "";
                string hash2Inbits = "";
                for (int i = 0; i < 64; i++)
                {
                    hash1Inbits += bitset<4>(hash1[i]).to_string();
                }
                for (int i = 0; i < 64; i++)
                {
                    hash2Inbits += bitset<4>(hash2[i]).to_string();
                }
                int num2 = CompareStrings(hash1Inbits, hash2Inbits);;
                if (num2 > binary_max)
                {
                    binary_max = num2;
                }
                if (num2 < binary_min)
                {
                    binary_min = num2;
                }
                matched_binary += num2;
            }
            cout << "Skirtingumas bitų lymenyje: " << 100 - ((matched_binary / (100000 * 256.0)) * 100) << endl;
            cout << "Minimalus: " << binary_min << ", maksimalus: " << binary_max << ", vidurkinis: " << std::fixed <<std::setprecision(2) << 1 - (matched_binary / (100000 * 256.0))<< endl;
            cout << "Skirtingumas hex'ų lymenyje: " << 100 - ((matched_hex / (100000 * 64.0)) * 100) << endl;
            cout << "Minimalus: " << hex_min << ", maksimalus: " << hex_max << ", vidurkinis: " << std::fixed <<std::setprecision(2) << 1 - (matched_hex / (100000 * 64.0))<< endl;
        }
        if (strcmp(argv[1], "-sha256") == 0)
        {
            std::fstream f(argv[2], std::ios::in);
            try
            {
                if (!f)
                {
                    throw "Nepavyko atidaryti failo";
                }
            }
            catch (const char *zinute)
            {
                std::cerr << zinute << '\n';
                exit(1);
            }
            std::stringstream ss;
            ss << f.rdbuf();
            string line;
            auto start = std::chrono::high_resolution_clock::now();
            while (getline(ss, line))
            {
                SHA256 hash;
                string digest;
                StringSource s(line, true, new HashFilter(hash, new HexEncoder(new StringSink(digest))));
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;
            cout << "Konstitucijos hash'avimas po eilutę su sha256 užtruko: " << diff.count() << " s" << endl;
        }
        if (strcmp(argv[1], "-md5") == 0)
        {
            std::fstream f(argv[2], std::ios::in);
            try
            {
                if (!f)
                {
                    throw "Nepavyko atidaryti failo";
                }
            }
            catch (const char *zinute)
            {
                std::cerr << zinute << '\n';
                exit(1);
            }
            std::stringstream ss;
            ss << f.rdbuf();
            string line;
            auto start = std::chrono::high_resolution_clock::now();
            while (getline(ss, line))
            {
                string digest;
                Weak::MD5 hash;
                StringSource s(line, true, new HashFilter(hash, new HexEncoder(new StringSink(digest))));
            }
            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> diff = end - start;
            cout << "Konstitucijos hash'avimas po eilutę su md5 užtruko: " << diff.count() << " s" << endl;
        }
    }
    if (argc == 1)
    {
        cin >> text;
        cout << text << ": " << hashFunction(text) << endl;
    }

    return 0;
}
