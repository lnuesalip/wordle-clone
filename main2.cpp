#include <iostream>
#include <cstring>
#include <algorithm>
#include <fstream>
#include <vector>
#include <random>
#include <map>

class RandomAnswer {
    public:
        std::vector<std::string> wordList; // initialize wordList

        std::string generateAnswer() {
            std::string word;
            std::ifstream wordBank;
            wordBank.open ("word_bank.txt");

            if (wordBank.is_open()) {
                for(word; getline(wordBank, word); ) {
                    transform(word.begin(), word.end(), word.begin(), ::toupper); // converts each word from word_bank into uppercase, transform takes first array's beginning and end, and stores the transformed array at the pointer of the next array
                    wordList.push_back(word); // add each line as an element in a vector
                }
            }
            
            auto rd = std::random_device {}; // generates a random number, type is auto (determines data type based on intializer)
            std::shuffle(wordList.begin(), wordList.end(), rd); // shuffles vector wordList (rd shuffles the vector differently each time the code is run)

            return wordList[0]; // choose the word that is at the beginning of the shuffled vector each time
        }

        // determines if the current guess is in the word bank
        bool validWord(std::string currentGuess) {
                    bool valid = false;
                    int len = size(wordList);

                    for (int i = 0; i <= len; i ++) { // iterate through word bank
                        if (currentGuess == wordList[i]) { // if guess matches a word in the word bank, it is valid
                            return valid = true;
                        }
                    }
                    return valid;
        }
};

class Game {
    public:
        std::string correctAnswer;
        int attemptsRemaining = 6;
        bool doContinue = true;

        // keeps track of whether the letter is in the right position, or not, or not in the word
        // 0 = default, white // 1 = in the word, orange // 2 = in position, green // 3 = not in word, grey
        std::map<char, char> alphabetCheck = {
            {'A', '0'}, {'B', '0'}, {'C', '0'}, {'D', '0'}, {'E', '0'}, {'F', '0'}, {'G', '0'},
            {'H', '0'}, {'I', '0'}, {'J', '0'}, {'K', '0'}, {'L', '0'}, {'M', '0'}, {'N', '0'},
            {'O', '0'}, {'P', '0'}, {'Q', '0'}, {'R', '0'}, {'S', '0'}, {'T', '0'}, {'U', '0'},
            {'V', '0'}, {'W', '0'}, {'X', '0'}, {'Y', '0'}, {'Z', '0'},
        };

        Game (std::string acorrectAnswer) { // constructor function, when initializing the instance
            correctAnswer = acorrectAnswer;
        }

        // check if the game should continue by determining if there are enough attempts remaining or determining if the user correctly guessed the word
        bool checkContinue (std::string currentGuess) {
            attemptsRemaining -= 1;

            if ((correctAnswer == currentGuess) || attemptsRemaining <= 0) { // the game ends if either of these are true, but the nested if-else determines what the game end message is
                if (correctAnswer == currentGuess) std::cout << "\nYou guessed the correct answer.\n";
                else { 
                    std::cout << "\nYou ran out of tries without guessing the correct answer.";
                    std::cout << "\nThe correct answer was " << correctAnswer << "\n";
                }
                doContinue = false;
            }
            else doContinue = true; // if neither condition is true then the game still has attempts remaining and the user has yet to guess the word

            return doContinue;
        }

        // function to check whether each letter is in the word, in the correct position, or not in the word
        // also outputs the result according to this ^
        void checkGuess (std::string currentGuess) {
            char listCheck[5] = {'0', '0', '0', '0', '0'}; // not using a map because each index should be flagged independently

            // keeps count of how many times a letter (from the current guess) appears in the CORRECT ANSWER
            std::map<char, int> letterCount;
            for (int i = 0; i < 5; i ++) {
                letterCount[currentGuess[i]] = std::count(correctAnswer.begin(), correctAnswer.end(), currentGuess[i]);
            } // count how many times the letter appears in the correct answer, store the value in the map with the key being the letter being counted (key:value -> currentLetter: # of times it appears in the answer)

            // checking if letters are IN THE WORD, IN THE CORRECT POSITION, or NOT IN THE WORD
            for (int i = 0; i < 5; i ++) {
                
                if (currentGuess[i] == correctAnswer[i]) { // if the letter is in the right spot, set it to 2 so it will print out as green
                    listCheck[i] = '2';
                    letterCount[currentGuess[i]] -= 1; // decrement the lettercount so that if the letter appears again in the CURRENT GUESS, it does not flag it again (if it does not appear again in the correct answer)
                    alphabetCheck[currentGuess[i]] = '2';
                    std::cout << "\033[1;32m" << currentGuess[i] << "\033[0m ";
                }

                else if (correctAnswer.find(currentGuess[i]) != std::string::npos && letterCount[currentGuess[i]] > 0) { // .find() returns the position of the first match, returns npos if no matches are found
                    // if the letter has not been flagged as green (and it still needs to be flagged because it appears atleast once in the answer), flag it as orange
                    listCheck[i] = '1';
                    letterCount[currentGuess[i]] -= 1; // decrement lettercount so that if the letter appears again in the CURRENT GUESS, it does not flag it again (if it does not appear again in the correct answer)
                    alphabetCheck[currentGuess[i]] = '1'; // sets the alphabet to the correct color for when it prints
                    std::cout << "\033[1;38;5;208m" << currentGuess[i] << "\033[0m ";
                }
                
                else {
                    listCheck[i] = '3'; // letter does not appear in the answer or letterCount of 0 indicates that the letters have already been flagged -> flag it as GREY
                    alphabetCheck[currentGuess[i]] = '3';
                    std::cout << "\033[1;30m" << currentGuess[i] << "\033[0m ";
                }
            }
            std::cout << "\n\n";

            // prints out the alphabet with the approriate colors, does not get reset for every guess so that the user can keep track of what letters they guessed before
            for (auto& elem : alphabetCheck) { // auto is a variable type, x is an interator that points to an element of the map
                switch (elem.second) { // elem.second refers to the value
                    case '0':
                        std::cout << "\033[1;37m" << elem.first << "\033[0m "; // elem.first refers to the key // white
                        break;
                    case '1':
                        std::cout << "\033[1;38;5;208m" << elem.first << "\033[0m "; // orange
                        break;
                    case '2':
                        std::cout << "\033[1;32m" << elem.first << "\033[0m "; // green
                        break;
                    case '3':
                        std::cout << "\033[1;30m" << elem.first << "\033[0m "; // grey
                        break;
                }
            }

            std::cout << "\n";
        }
};

int main () {
    bool again = true;
    while (again) {
        // initialize user attempt for input
        std::string currentGuess;

        // outputted text for user instructions
        std::cout << "\n\033[1;4;37mWORDLE\033[0m\n";
        std::cout << "You have a max of 6 attempts.\nThe solution has 5 letters.\n";

        // generate correct answer from txt file
        // RandomAnswer wordBank;
        // std::string correctAnswer = wordBank.generateAnswer();

        // FOR TESTING TEST CASES: APPLE, ABBEY, CRAZE
        RandomAnswer wordBank;
        wordBank.generateAnswer();
        std::string correctAnswer = "ABBEY";

        Game start(correctAnswer); // create game instance

        // loop until the game class decides it shouldn't continue anymore
        int i = 1;
        while (start.doContinue) {
            std::cout << "\nEnter guess #" << i << ": ";
            std::getline(std::cin >> std::ws, currentGuess); //ws function skips whitespace

            transform(currentGuess.begin(), currentGuess.end(), currentGuess.begin(), ::toupper); // converts to uppercase (applies inputted function to each element in the range, stores in inputted array)

            // input validation
            if (currentGuess.length() != 5) { // check length matches 5
                std::cout << "Your guess must be 5 letters. Try again.\n";
                continue; // doesn't count as a guess
            }
            else if (wordBank.validWord(currentGuess) == false) { // check if word is in the word bank
                std::cout << "That is not a valid word. Try again.\n";
                continue; // doesn't count as a guess
            }
        
            // output results first
            start.checkGuess(currentGuess);  // prints results
            start.checkContinue(currentGuess); // check if game should continue, and if it ends then output result
            // ^ this function also updates doContinue, so it decides if the loop continues

            i ++; // lets user know how many guesses they have
        }
        
        // allows user to play again
        std::string x;
        std::cout << "\nPress 0 to play again, or press anything else to exit: ";
        std::cin >> x;
        if (x == "0") continue;
        else break;
        
    }
    
    return 0;
}

//word list source: https://gist.github.com/cfreshman/a03ef2cba789d8cf00c08f767e0fad7b
//wordle solver: https://word.tips/wordle/