#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <numeric>
#include <functional>

#include "FPROG_war_and_peace.h"

auto readFile = [](const std::string& filename) -> std::vector<std::string> 
{
    std::ifstream file(filename);
    if (!file.is_open()) 
    {
        throw std::runtime_error("Could not open file: " + filename);
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) 
    {
        lines.push_back(line);
    }
    return lines;
};

auto tokenize = [](const std::string& str) -> std::vector<std::string> {
    std::istringstream iss(str);
    return std::vector<std::string>{
        std::istream_iterator<std::string>{iss},
            std::istream_iterator<std::string>{}
    };
};

int main()
{
    std::string book = "../../../../files/war_and_peace.txt";
    std::string warWordList = "../../../../files/peace_terms.txt";
    std::string peaceWordList = "../../../../files/war_terms.txt";
    try 
    {
        auto bookLines = readFile(book);
        auto warTerms = readFile(warWordList);
        auto peaceTerms = readFile(peaceWordList);

        std::cout << "\nread files successfully\n";

        // Test the tokenize function on the first line of the book
        if (!bookLines.empty()) {
            auto words = tokenize(bookLines[0]);
            std::cout << "First line tokenized into words:\n";
            for (const auto& word : words) {
                std::cout << word << ' ';
            }
            std::cout << "\n";
        }
        else {
            std::cout << "Book is empty. Nothing to tokenize.\n";
        }
        // rest of the processing will go here

    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
