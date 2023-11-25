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
 * Reads the contents of a file, concatenates them into a single string,
 * and then tokenizes this string into words. Each word is returned as
 * an element in a vector.
 *
 * @param filename The path to the file to be read.
 * @return A vector of strings, where each element is a word from the file.
 * @throws std::runtime_error if the file cannot be opened.
 */
auto readFile = [](const std::string& filename) -> std::vector<std::string>
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file: " + filename);
    }

    std::string content;
    std::string line;
    while (std::getline(file, line))
    {
        content += line + "\n";  // Append each line to 'content' with a newline character
    }
    // still functional? tokenize in readFile function?
    std::vector<std::string> tokenizedText = tokenize(content);
    return tokenizedText;
};

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

/**
 * Calculates the density of a set of terms within a vector of words.
 * Density is the count of term occurrences divided by the total number of words.
 *
 * @param chapter The vector of words representing the chapter text.
 * @param terms The set of terms to calculate the density for.
 * @return The density of the terms as a double.
 */
auto calculateDensity = [](const std::vector<std::string>& chapter, const std::set<std::string>& terms) -> double 
{
    auto count = std::count_if(chapter.begin(), chapter.end(), [&terms](const std::string& word)
    {
        return terms.find(word) != terms.end();
    });
    double termOccurrences = count;
    double totalWords = chapter.size();
    return totalWords > 0 ? termOccurrences / totalWords : 0.0;
};

int main() 
{
    std::string bookPath = "../../../../files/war_and_peace.txt";
    std::string warTermsPath = "../../../../files/war_terms.txt";
    std::string peaceTermsPath = "../../../../files/peace_terms.txt";
    try 
    {
        auto tokenizedBookLines = readFile(bookPath);
        auto tokenizedWarTerms = readFile(warTermsPath);
        auto tokenizedPeaceTerms = readFile(peaceTermsPath);

        std::cout << "\nRead and tokenized files successfully\n";

        // Convert the term lists to sets for efficient searching
        std::set<std::string> warTermsSet(tokenizedWarTerms.begin(), tokenizedWarTerms.end());
        std::set<std::string> peaceTermsSet(tokenizedPeaceTerms.begin(), tokenizedPeaceTerms.end());

        if (!tokenizedBookLines.empty()) 
        {
            // Filter words based on war terms
            auto warFilteredWords = filterWords(tokenizedBookLines, tokenizedWarTerms);
            auto warWordCounts = countOccurrences(warFilteredWords);
            double warDensity = calculateDensity(tokenizedBookLines, warTermsSet);

            // Filter words based on peace terms
            auto peaceFilteredWords = filterWords(tokenizedBookLines, tokenizedPeaceTerms);
            auto peaceWordCounts = countOccurrences(peaceFilteredWords);
            double peaceDensity = calculateDensity(tokenizedBookLines, peaceTermsSet);

            // Output results
            std::cout << "\nWar word counts:\n";
            for (const auto& pair : warWordCounts)
            {
                std::cout << pair.first << ": " << pair.second << '\n';
            }
            std::cout << "War word density: " << warDensity << std::endl;

            std::cout << "\nPeace word counts:\n";
            for (const auto& pair : peaceWordCounts)
            {
                std::cout << pair.first << ": " << pair.second << '\n';
            }
            std::cout << "Peace word density: " << peaceDensity << std::endl;
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