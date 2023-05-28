// Hangman.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <vector>
#include <algorithm>
#include <cctype>

const int c_MaxWrong = 8;

char askYesNo(std::string question);
unsigned int getDictionarySize(std::string _filename);
std::string getWordFromFile(std::string _filename, const unsigned int _dictionarySize);
//std::string getWordFromFile(std::string _filename);
void startHangman(std::string _theWord);

int main()
{
    std::string filename = "";
    std::cout << "Enter word dictionary filename: ";
    std::cin >> filename;

    const unsigned int dictionarySize = getDictionarySize(filename);

    //const std::string theWord = getWordFromFile(filename);    // word to guess
    if (dictionarySize != 0)
    {
        do
        {
            system("cls");
            std::string temp = getWordFromFile(filename, dictionarySize); 
            std::transform(temp.begin(), temp.end(), temp.begin(), [](unsigned char c) {return std::tolower(c); }); // convert word to lowercase in case some of the word is not in lowercase
            const std::string theWord = temp ;    // word to guess
            
            if (!theWord.empty())
            {
                startHangman(theWord);
            }
            else
                break;
        } while (askYesNo("Do you want to play again?") != 'n');
    }
}

char askYesNo(std::string question) {
    char answer;
    do
    {
        std::cout << question << " (y/n): ";
        std::cin >> answer;
        answer = tolower(answer);
    } while (answer != 'y' && answer != 'n');
    return answer;
}

/// <summary>
/// Get the number of lines of given word dictionary
/// </summary>
/// <param name="_filename">dictionary filename including the file extension</param>
/// <returns>number of lines in the word dictionary, return 0 if file can't be opened</returns>
unsigned int getDictionarySize(std::string _filename) {
    std::string line = "";
    int lineCount = 0;

    std::ifstream dictionary(_filename);

    if (dictionary.is_open())
    {
        while (dictionary.peek() != EOF) {
            std::getline(dictionary, line);
            lineCount++;
        }
        dictionary.close();
        return lineCount;
    }
    else
        std::cout << "\nCouldn't open the file\n";
    
    return 0;
}

/// <summary>
/// Get random word from word dictionary
/// </summary>
/// <param name="_filename">dictionary filename including the file extension </param>
/// <param name="_dictionarySize">size of the dictionary</param>
/// <returns>random wrod from word dictionary </returns>
std::string getWordFromFile(std::string _filename, const unsigned int _dictionarySize) {
    //rng
    std::random_device rd;
    std::mt19937 mt(rd());

    std::string line = "";
    int lineCount = 0;

    std::ifstream dictionary(_filename);

    if (dictionary.is_open()) {
        std::uniform_int_distribution<int> dist(0, _dictionarySize); //set range of random number
        int theWordLine = dist(mt);

        int curLine = 0;
        line == "";

        while (curLine <= theWordLine) {
            std::getline(dictionary, line);
            curLine++;
        }
    }

    return line;
}

//std::string getWordFromFile(std::string _filename) {
//
//    //rng
//    std::random_device rd;
//    std::mt19937 mt(rd());
//
//    std::string line = "";
//    int lineCount = 0;
//
//    std::ifstream dictionary(_filename);
//
//    if (dictionary.is_open())
//    {
//        while (dictionary.peek() != EOF) {
//            std::getline(dictionary, line);
//            lineCount++;
//        }
//
//        //std::cout << lineCount << std::endl;
//
//        dictionary.clear();
//        dictionary.seekg(std::ios::beg);
//
//        std::uniform_int_distribution<int> dist(0, lineCount); //set range of random number
//        int theWordLine = dist(mt);
//
//        int curLine = 0;
//        line == "";
//
//        while (curLine <= theWordLine) {
//            std::getline(dictionary, line);
//            curLine++;
//        }
//        dictionary.close();
//    }
//    else
//        std::cout << "Couldn't open the file\n\n";
//
//    return line;
//}

void startHangman(const std::string _theWord) {
    int wrongCount = 0;									// number of incorrect guesses
    std::string guessSoFar(_theWord.size(), '*');	// word guessed so far
    std::string usedLetters = "";					// letters already guessed

    std::cout << "Welcome to Hangman. Good Luck!\n\n";

    // main loop

    while ((wrongCount < c_MaxWrong) && (guessSoFar != _theWord))
    {
        std::cout << "You have " << (c_MaxWrong - wrongCount);
        std::cout << " incorrect guesses left.\n\n";
        std::cout << "You've used the following letters:\n" << usedLetters << "\n\n";
        std::cout << "So far, the word is:\n" << guessSoFar << "\n\n";

        char guess;
        std::cout << "Enter your guess: ";
        std::cin >> guess;
        guess = tolower(guess); //make lowercase since secret word in lowercase

        //if input is number or letter already used
        while (isdigit(guess) ||  usedLetters.find(guess) != std::string::npos)
        {
            if (isdigit(guess))
                std::cout << "\nPlease input alphabet only.\n\n";
            else
                std::cout << "\nYou've already guessed " << guess << "\n\n";
            std::cout << "Enter your guess: ";
            std::cin >> guess;
            guess = tolower(guess);
        }

        //add the letter to the list if it's a new letter
        usedLetters += guess;

        system("cls");

        //letter is in the hidden word
        if (_theWord.find(guess) != std::string::npos)
        {
            std::cout << "That's right! " << guess << " is in the word.\n\n";

            // update guess so far to include newly guessed letter
            for (unsigned int i = 0; i < _theWord.length(); ++i)
            {
                if (_theWord[i] == guess)
                {
                    guessSoFar[i] = guess;
                }
            }
        }
        //letter is not in the hidden word
        else
        {
            std::cout << "Sorry, " << guess << " isn't in the word.\n\n";
            wrongCount++;
        }
    }

    //handle result
    if (wrongCount == c_MaxWrong)
        std::cout << "You've been hanged!\n";
    else
        std::cout << "You guessed it!\n";

    std::cout << "\nThe word was " << _theWord << ".\n\n";
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
