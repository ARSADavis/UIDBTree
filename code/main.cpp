#include <iostream>
#include <string>
#include <fstream>

int main()
{
    std::string stringData = "This is data\nAnd this is data too";
    double doubleData = -57.34;

    std::fstream dataFile("test.txt", std::ios::trunc | std::ios::out);
    if (dataFile.is_open())
    {
        dataFile << stringData.length() << stringData << '\n' << doubleData;
        dataFile.close();
    }

    std::fstream readFile("test.txt", std::ios::in);
    if (readFile.is_open())
    {
        std::cout << "Current position in file (A): " << readFile.tellg() << "\n";
        std::size_t stringLength;
        readFile >> stringLength;
        std::cout << stringLength << "\n";
        std::cout << "Current position in file (B): " << readFile.tellg() << "\n";
        std::string readString(stringLength, ' ');
        readFile.read(&readString[0], stringLength);
        std::cout << readString << "\n";
        std::cout << "Current position in file (C): " << readFile.tellg() << "\n";
        double readDouble;
        readFile >> readDouble;
        std::cout << readDouble << "\n";
        std::cout << "Current position in file (D): " << readFile.tellg() << "\n";
        readFile.clear();
        std::cout << "Current position in file (E): " << readFile.tellg() << "\n";
        readFile.seekg(0, std::ios_base::cur);
        std::cout << "Current position in file (F): " << readFile.tellg() << "\n";
        readFile.seekg(0, std::ios_base::beg);
        std::cout << "Current position in file (G): " << readFile.tellg() << "\n";
        readFile.seekg(-1, std::ios_base::end);
        std::cout << "Current position in file (H): " << readFile.tellg() << "\n";
        std::cout << readFile.rdbuf() << "\n";
        std::cout << "Current position in file (I): " << readFile.tellg() << "\n";
        readFile.close();
    }

    std::cout << "\n\n";
    return 0;
}