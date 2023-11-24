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
// auto tokenize = [](const std::string& str) -> std::vector<std::string> 
// {
//     std::istringstream iss(str);
//     std::vector<std::string> result;
//     std::istream_iterator<std::string> it(iss);
//     std::istream_iterator<std::string> end;
// 
//     for (; it != end; ++it) 
//     {
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
auto filterWords = [](const std::vector<std::string>& words, const std::vector<std::string>& filterList) -> std::vector<std::string> 
{
    std::set<std::string> filterSet(filterList.begin(), filterList.end());
    std::vector<std::string> filteredWords;
    // if the word is found in 'filterSet', it will be added to 'filteredWords'
    std::copy_if(words.begin(), words.end(), std::back_inserter(filteredWords), [&filterSet](const std::string& word)
    {
        // if 'find' does not return 'end()', the word is in the set
        return filterSet.find(word) != filterSet.end();
    });
    return filteredWords;
 };

/**
 * Counts the occurrences of each word in a list.
 *
 * @param words The vector of words to count occurrences in.
 * @return A map where each key is a word and the value is the count of occurrences.
 */
auto countOccurrences = [](const std::vector<std::string>& words) -> std::map<std::string, int> // still needs threading etc.
{
    std::map<std::string, int> wordCount;
    std::for_each(words.begin(), words.end(), [&wordCount](const std::string& word) 
    {
        wordCount[word]++;
    });
    return wordCount;
};

int main() 
{
    std::string bookPath = "../../../../files/war_and_peace.txt";
    std::string warTermsPath = "../../../../files/war_terms.txt";
    std::string peaceTermsPath = "../../../../files/peace_terms.txt";
    try 
    {
        auto bookLines = readFile(bookPath);
        auto warTerms = readFile(warTermsPath);
        auto peaceTerms = readFile(peaceTermsPath);

        std::cout << "\nRead files successfully\n";

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
            auto warFilteredWords = filterWords(firstChapterWords, warTerms);
            std::cout << "Filtering words based on war terms...\n";

            // Count occurrences of filtered war words
            auto warWordCounts = countOccurrences(warFilteredWords);
            std::cout << "War word counts:\n";
            for (const auto& pair : warWordCounts) 
            {
                std::cout << pair.first << ": " << pair.second << '\n';
            }

            auto peaceFilteredWords = filterWords(firstChapterWords, peaceTerms);
            std::cout << "Filtering words based on peace terms...\n";

            // Count occurrences of filtered peace words
            auto peaceWordCounts = countOccurrences(peaceFilteredWords);
            std::cout << "Peace word counts:\n";
            for (const auto& pair : peaceWordCounts)
            {
                std::cout << pair.first << ": " << pair.second << '\n';
            }

        }
        else 
        {
            std::cout << "Book is empty. Nothing to tokenize.\n";
        }

    }
    catch (const std::exception& e) 
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}