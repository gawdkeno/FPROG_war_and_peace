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
#include <cctype> // for std::ispunct
#include <iterator> // for std::istream_iterator

#include "FPROG_war_and_peace.h"

/**
 * Tokenizes a string into words using whitespace as the delimiter and removes punctuation from each word.
 *
 * @param str The string to be tokenized.
 * @return A vector of words extracted from the string, with punctuation removed from each word.
 */
auto tokenize = [](const std::string& str) -> std::vector<std::string> 
{
    std::istringstream iss(str);
    std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

    std::transform(tokens.begin(), tokens.end(), tokens.begin(), [](std::string word) 
    {
        word.erase(std::remove_if(word.begin(), word.end(), ::ispunct), word.end());
        return word;
    });

    return tokens;
};

/**
 * Reads the contents of a file into a single string and then tokenizes this string into words.
 * Each word is returned as an element in a vector.
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

    // Read the entire content of the file into a string using istreambuf_iterator
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    // Tokenize the content into words
    return tokenize(content);
};

/**
 * Filters a vector of words based on a set of terms.
 * Only words that are present in the terms set are kept, including duplicates.
 *
 * @param chapter The vector of words to filter.
 * @param terms The set of terms to filter against.
 * @return A vector of filtered words that are present in the terms set.
 */
auto filterWords = [](const std::vector<std::string>& chapter, const std::set<std::string>& terms) -> std::vector<std::string> 
{
    std::vector<std::string> filteredWords;
    std::copy_if(chapter.begin(), chapter.end(), std::back_inserter(filteredWords), [&terms](const std::string& word)
    {
        return terms.find(word) != terms.end();
    });
    return filteredWords;
};

/**
 * Counts the occurrences of each word in a list.
 *
 * @param words The vector of words to count occurrences in.
 * @return A map where each key is a word and the value is the count of occurrences.
 */
auto countOccurrences = [](const std::vector<std::string>& words) -> std::map<std::string, int>
{
    std::map<std::string, int> wordCount;
    std::for_each(words.begin(), words.end(), [&wordCount](const std::string& word) 
    {
        wordCount[word]++;
    });
    return wordCount;
};

/**
 * Calculates the density of a vector of terms within a vector of words.
 * Density is the sum of the occurrences of specified terms divided by the total number of word occurrences.
 * It uses the countOccurrences function to count all word occurrences and then accumulates counts for the specified terms.
 *
 * @param chapter The vector of words representing the chapter text.
 * @param terms The vector of terms to calculate the density for.
 * @return The density of the terms as a double, calculated as the proportion of the term occurrences out of the total word occurrences.
 */
auto calculateDensity = [&](const std::vector<std::string>& chapter, const std::vector<std::string>& terms) -> double
{
    auto wordCounts = countOccurrences(chapter);
    double termOccurrences = std::accumulate(terms.begin(), terms.end(), 0.0, [&wordCounts](double sum, const std::string& term) 
    {
        auto it = wordCounts.find(term);
        return sum + (it != wordCounts.end() ? it->second : 0);
    });

    double totalWords = chapter.size();
    return totalWords > 0 ? termOccurrences / totalWords : 0.0;
};

/**
 * Splits a tokenized book text into chapters based on a specific keyword.
 * Each chapter is represented as a vector of words, and all chapters are contained within a vector.
 *
 * The function assumes that each new chapter starts with the word "CHAPTER".
 * It creates a new chapter every time this keyword is encountered, except at the start of the text.
 *
 * @param bookLines A vector of words representing the entire book text.
 * @return A vector of chapters, where each chapter is a vector of words.
 */
auto splitIntoChapters = [](const std::vector<std::string>& bookLines) -> std::vector<std::vector<std::string>>
{
    std::vector<std::vector<std::string>> chapters;
    std::vector<std::string> currentChapter;
    for (const auto& word : bookLines)
    {
        if (word == "CHAPTER" && !currentChapter.empty())
        {
            chapters.push_back(currentChapter);
            currentChapter.clear();
        }
        currentChapter.push_back(word);
    }
    if (!currentChapter.empty()) // last chapter
    {
        chapters.push_back(currentChapter);
    }
    return chapters;
};

double calculateSimilarity() 
{
    std::ifstream infile1("../../../../files/our_output.txt"), infile2("../../../../files/should_output.txt");
    if (!infile1.is_open() || !infile2.is_open())
    {
        std::cerr << "Error opening files!" << std::endl;
        return -1;
    }

    std::string line1, line2;
    int similarLines = 0, totalLines = 0;

    while (getline(infile1, line1) && getline(infile2, line2)) 
    {
        totalLines++;
        if (line1 == line2) 
        {
            similarLines++;
        }
    }

    infile1.close();
    infile2.close();

    double similarity = (totalLines > 0) ? (static_cast<double>(similarLines) / totalLines) * 100 : 0;
    if (similarity >= 0)
    {
        std::cout << "The files are " << similarity << "% similar." << std::endl;
    }
}

int main()
{
    std::string bookPath = "../../../../files/war_and_peace.txt";
    std::string warTermsPath = "../../../../files/war_terms.txt";
    std::string peaceTermsPath = "../../../../files/peace_terms.txt";

    // Create an ofstream for output file
    std::ofstream outputFile("../../../../files/our_output.txt");

    try
    {
        auto tokenizedBookLines = readFile(bookPath);
        auto tokenizedWarTerms = readFile(warTermsPath);
        auto tokenizedPeaceTerms = readFile(peaceTermsPath);

        std::set<std::string> warTermsSet(tokenizedWarTerms.begin(), tokenizedWarTerms.end());
        std::set<std::string> peaceTermsSet(tokenizedPeaceTerms.begin(), tokenizedPeaceTerms.end());

        auto chapters = splitIntoChapters(tokenizedBookLines);

        for (size_t i = 0; i < chapters.size(); ++i)
        {
            const auto& chapter = chapters[i];

            // Filter occurrences of war and peace words for the funtction calculateDensity to look for
            auto warFilteredWords = filterWords(chapter, warTermsSet);
            auto peaceFilteredWords = filterWords(chapter, peaceTermsSet);

            // Calculate Density with the found words
            double warDensity = calculateDensity(chapter, warFilteredWords);
            double peaceDensity = calculateDensity(chapter, peaceFilteredWords);

            std::string category = warDensity > peaceDensity ? "war" : "peace";

            // Print results for the chapter
            std::string outputLine = "Chapter " + std::to_string(i + 1) + ": " + category + "-related\n";
            std::cout << outputLine;
            outputFile << outputLine;

            // Count occurrences of filtered words
            auto warWordCounts = countOccurrences(std::vector<std::string>(warFilteredWords.begin(), warFilteredWords.end()));
            auto peaceWordCounts = countOccurrences(std::vector<std::string>(peaceFilteredWords.begin(), peaceFilteredWords.end()));

            // Print war word counts
            std::cout << "War word counts:\n";
            for (const auto& pair : warWordCounts)
            {
                std::cout << pair.first << ": " << pair.second << '\n';
            }
            std::cout << "War word density: " << warDensity << std::endl;

            // Print peace word counts
            std::cout << "Peace word counts:\n";
            for (const auto& pair : peaceWordCounts)
            {
                std::cout << pair.first << ": " << pair.second << '\n';
            }
            std::cout << "Peace word density: " << peaceDensity << std::endl;

            std::cout << "----------------------\n";
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        // Make sure to close the file before returning
        outputFile.close();
        return EXIT_FAILURE;
    }

    // Close the output file
    outputFile.close();

    calculateSimilarity();

    return EXIT_SUCCESS;
}
