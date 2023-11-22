#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <numeric>
#include <functional>
#include <set>

#include "FPROG_war_and_peace.h"


/**
 * Reads the contents of a file and returns them as a vector of strings.
 * Each line of the file becomes a single string in the vector.
 *
 * @param filename The path to the file to be read.
 * @return A vector of strings where each element is a line from the file.
 * @throws std::runtime_error if the file cannot be opened.
 */
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

/**
 * Tokenizes a string into words using whitespace as the delimiter.
 *
 * @param str The string to be tokenized.
 * @return A vector of words extracted from the string.
 */
auto tokenize = [](const std::string& str) -> std::vector<std::string> 
    {
    std::istringstream iss(str);
    return std::vector<std::string>
    {
        std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}
    };
};

// alternative:
// auto tokenize = [](const std::string& str) -> std::vector<std::string> {
//     std::istringstream iss(str);
//     std::vector<std::string> result;
//     std::istream_iterator<std::string> it(iss);
//     std::istream_iterator<std::string> end;
// 
//     for (; it != end; ++it) {
//         result.push_back(*it);
//     }
// 
//     return result;
// };

/**
 * Filters a list of words based on a set of terms.
 * Only words that are present in the terms set are kept.
 *
 * @param words The vector of words to filter. (book)
 * @param terms The set of terms to filter against. (filter-list)
 * @return A vector of filtered words that are present in the terms set.
 */
auto filterWords = [](const std::vector<std::string>& words, const std::vector<std::string>& filterList) -> std::vector<std::string> {
    std::set<std::string> filterSet(filterList.begin(), filterList.end());
    std::vector<std::string> filteredWords;
    // if the word is found in 'filterSet', it will be added to 'filteredWords'
    std::copy_if(words.begin(), words.end(), std::back_inserter(filteredWords),
        [&filterSet](const std::string& word)
        {
            // if 'find' does not return 'end()', the word is in the set
            return filterSet.find(word) != filterSet.end();
        });
    return filteredWords;
 };

int main()
{
    std::string book = "../../../../files/war_and_peace.txt";
    std::string warWordList = "../../../../files/war_terms.txt";
    std::string peaceWordList = "../../../../files/peace_terms.txt";
    try 
    {
        auto bookLines = readFile(book);
        auto warTerms = readFile(warWordList);
        auto peaceTerms = readFile(peaceWordList);

        std::cout << "\nread files successfully\n";

        // test filterWords on first chapter
        if (!bookLines.empty()) 
        {
            auto chapterEnd = std::find_if(bookLines.begin(), bookLines.end(), [](const std::string& line) 
            {
                return line.find("CHAPTER") != std::string::npos;
            });
            std::vector<std::string> firstChapter(bookLines.begin(), chapterEnd);

            // Tokenize the first chapter
            std::vector<std::string> firstChapterWords;
            for (const auto& line : firstChapter) 
            {
                auto wordsInLine = tokenize(line);
                firstChapterWords.insert(firstChapterWords.end(), wordsInLine.begin(), wordsInLine.end());
            }

            // Filter words based on war terms
            std::cout << "Filtering words based on war terms...\n";
            auto warFilteredWords = filterWords(firstChapterWords, warTerms);
            for (const auto& word : warFilteredWords) 
            {
                std::cout << word << ' ';
            }
            std::cout << "\n";

            // Filter words based on peace terms
            std::cout << "Filtering words based on peace terms...\n";
            auto peaceFilteredWords = filterWords(firstChapterWords, peaceTerms);
            for (const auto& word : peaceFilteredWords) 
            {
                std::cout << word << ' ';
            }
            std::cout << "\n";
        }
        else 
        {
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
