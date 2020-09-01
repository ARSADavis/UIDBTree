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
        std::size_t stringLength;
        readFile >> stringLength;
        std::cout << stringLength << ": ";
        std::string readString(stringLength, ' ');
        readFile.read(&readString[0], stringLength);
        std::cout << readString << "\n";
        double readDouble;
        readFile >> readDouble;
        std::cout << readDouble << "\n";
        readFile.close();
    }

    std::cout << "\n\n";
    return 0;
}