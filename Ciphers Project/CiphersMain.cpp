#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const string ALPHABET = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

// Function prototypes at the top of the file for function decomposition

/**
 * Print instructions for using the program.
 */
void printMenu();
/**
 * Returns the 0-based index in the English alphabet where `c` appears,
 * or -1 if `c` is not an uppercase letter in the alphabet.
 *
 * For example:
 *  - `findIndexInAlphabet('A')` returns 0
 *  - `findIndexInAlphabet('D')` returns 3
 *  - `findIndexInAlphabet('+')` returns -1
 *  - `findIndexInAlphabet('a')` returns -1
 */
int findIndexInAlphabet(char c);
/**
 * Returns `c` rotated by `amount` many characters forward. If we run out
 * of letters in the alphabet, wrap around back to 'A'. This method
 * assumes that `c` is an uppercase letter.
 * For example:
 *  - `rot('A', 0)` returns 'A'
 *  - `rot('A', 1)` returns 'B'
 *  - `rot('Z', 1)` returns 'A'
 *  - `rot('A', 10)` returns 'K'
 *  - `rot('J', 25)` returns 'I'
 */
char rot(char c, int amount);
/**
 * Returns a new string in which every character has been rotated by `amount`
 * letters. Lowercase letters are converted to uppercase and rotated.
 * Non-alphabetic characters are left alone.
 * For example:
 *  `rot("A", 0)` returns "A"
 *  `rot("AA", 3)` returns "DD"
 *  `rot("HELLO", 0)` returns "HELLO"
 *  `rot("HELLO", 1)` returns "IFMMP"
 *  `rot("oh HEL-LO!", 1)` returns "PI IFM-MP!"
 */
string rot(string line, int amount);
string prepareKey(const string& key);
string vigenereEncrypt(const string& text, const string& key);
string cipherDecryptSingle(const string& word, int shift);
string cipherDecryptMulti(const string& text, int shift);

string cipherDecryptMulti(const string& text, int shift, const vector<string>& dictionary);
void cipherDecryptSingle(const string& word, const vector<string>& dictionary);
bool DictionaryWords(const string& word, const vector<string>& dictionary);


int main() {
    
    vector<string> dictionary;
    ifstream dictionaryFile("dictionary.txt");

    if (dictionaryFile.is_open()) {
        string word;
        while (dictionaryFile >> word) {
            dictionary.push_back(word);
        }
        dictionaryFile.close();
    } else {
        cerr << "Error: Unable to open dictionary.txt" << endl;
        return 1;
    }
    string command;

    cout << "Welcome to Ciphers!" << endl;
    cout << "-------------------" << endl;
    cout << endl;

    do {
         printMenu();
        cout << endl << "Enter a command (case does not matter): ";
        // Use getline for all user input to avoid needing to handle
        // input buffer issues relating to using both >> and getline

        getline(cin, command);
        cout << endl;

        if (command == "C" || command =="c") {
            string input;
             int shiftAmount;

            cout << "Enter the text to encrypt:" << endl;
            getline(cin, input);

            cout << "Enter the number of characters to rotate by:" << endl;
            cin >> shiftAmount;
            cin.ignore();  


            cout << rot(input, shiftAmount) << endl;
        }
          else if (command == "D" || command == "d") {
            string input;
            int shiftAmount;
            string word;

            cout << "Enter the text to Caesar-cipher decrypt:" << endl;
            getline(cin, input);
            if ((input.find(' ') == string::npos) && find(dictionary.begin(), dictionary.end(), word) != dictionary.end()){
                cipherDecryptSingle(input, dictionary);
            }

            else{
                for (shiftAmount = 26; shiftAmount > 0; shiftAmount--) {
                    string decryptedText = cipherDecryptMulti(input, shiftAmount, dictionary);

                    if (!decryptedText.empty()) {
                        cout << decryptedText << endl;
                        // break;  // Stop after finding a valid decryption
                    }
                }
            }

            if (shiftAmount == 27) {
                cout << "No good decryptions found" << endl;
            }

        }
        

         else if (command == "V" || command == "v") {
            // this will Encrypt with Vigenère Cipher
            string input, vigenereKey;

            cout << "Enter text to encrypt:" << endl;
            getline(cin, input);

            cout << "Enter the Vigenère key:" << endl;
            getline(cin, vigenereKey);

            string preparedKey = prepareKey(vigenereKey);
            string resultText = vigenereEncrypt(input, preparedKey);

            for (size_t i = 0; i < resultText.length(); ++i) {
                resultText[i] = toupper(resultText[i]);
            }
            cout << resultText << endl;
        }
        cout << endl;

    } while (command != "x" && command != "X");

    return 0;
}

void printMenu() {
    cout << "Ciphers Menu" << endl;
    cout << "------------" << endl;
    // Caesar Ciphers
    cout << "C - Encrypt with Caesar Cipher" << endl;
    cout << "D - Decrypt Caesar Cipher" << endl;
    // Vigenere
    cout << "V - Encrypt with Vigenère" << endl;
    // Utility
    cout << "X - Exit Program" << endl;
}

int findIndexInAlphabet(char c) { // this function finds the index of a character in the alphabet
     int alphaIndex =ALPHABET.find(c);
    if (alphaIndex !=  string::npos) {
        return alphaIndex;
    }
    return -1; 
}

char rot(char c, int amount) {// this function rotates a character by a certain amount

    if (isalpha(c) && isupper(c)) {
        return ALPHABET[(findIndexInAlphabet(c) +amount) % 26];
    }
    return c; 

}

 string rot(string line, int amount) { // this function rotates characters in a string by a certain amount
     for (int i = 0; i < line.length(); i++) {
         if (isalpha(line[i])) {
             line[i] =toupper(line[i]);
             line[i] =rot(line[i],amount);
        }
    }
    return line;
}

string prepareKey(const string&  key) { // this function will prepare the key for Vigenère encryption
    string preparedKey;
    for (size_t i = 0; i < key.length(); ++i) {
        if (isalpha(key[i])) {
            preparedKey +=toupper(key[i]);
        }
    }

    return preparedKey;
}

string vigenereEncrypt(const string& input, const string& key) { // this functio will perform Vigenère encryption
    string result;
    int keyIndex = 0;

    for (size_t j = 0; j < input.length(); j++) {
        if (isalpha(input[j])) {
            char base = isupper(input[j]) ? 'A' : 'a';
            char encryptText = (input[j] - base + key[keyIndex] - 'A') % 26 + base;
            result.push_back(encryptText);
            keyIndex = (keyIndex + 1) % key.length();
        } else {
            result += input[j];
        }
    }
    return result;
}

void cipherDecryptSingle(const string& word, const vector<string>& dictionary) {
    for (int shift = 0; shift <=25; ++shift) {
        string decryptedWord = rot(word, shift - 26);
        if (DictionaryWords(decryptedWord, dictionary)) {
            cout << decryptedWord << endl;
        }
    }
}

string cipherDecryptMulti(const string& text, int shift, const vector<string>& dictionary) {
    string decryptedText = rot(text,  26 -shift);
    string currentWord;
    size_t validWordCount =  0;
    size_t wordCount =0;

    for (size_t i = 0; i < decryptedText.length();  ++i) {
    char c = decryptedText[i];
        if (isalpha(c)) {
            currentWord +=c;
        } else {
            if (!currentWord.empty() && DictionaryWords(currentWord,dictionary)) {
                validWordCount++;
            }
            if (!currentWord.empty()) {
                wordCount++;
            }
            currentWord.clear();
        }
    }

    if (!currentWord.empty()) {   // this if loop checks if the lasst word exists or not 
        wordCount++;
        if (DictionaryWords(currentWord, dictionary)) {
            validWordCount++;
        }
    }

    if (validWordCount > wordCount /2) { // Validate the decryption using dictionary words.

        return decryptedText;
    } else {
        return "";
    }
    
}

bool DictionaryWords(const string& word,const vector<string>&  dictionary) {
    return find(dictionary.begin(),  dictionary.end(), word) !=dictionary.end();
}