#pragma once

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <sstream>
#include <cctype>


using namespace std;

/**
 * Cleans a specific token by:
 * - Removing leading and trailing punctuation
 * - Converting to lowercase
 * If the token has no letters, returns the empty string.
 */

string cleanToken(string s) {
    if (s.empty()) return ""; // here if the input string is empty, return the empty string

    size_t beginindex = 0;    //this command removes leading punctuation
    while (beginindex < s.length() && ispunct(s[beginindex])) {
        ++beginindex;
    }
    size_t endindex = s.length() - 1;     //this command removes trailing punctuation
    while (endindex > beginindex && ispunct(s[endindex])) {
        --endindex;
    }

    string outcome;
    bool alpha = false;
    for (size_t i = beginindex; i <= endindex; ++i) {     // this loop converts lowercase and exclude non-alphabetic characters
        char c = s[i];
        if (isalpha(c)) {
            outcome += tolower(c);
            alpha = true;
        } else {
            outcome += c;
        }
    }
    if (alpha) {
        return outcome;
    } else {
        return "";
    }
}

/**
 * Returns a set of the cleaned tokens in the given text.
 */
set<string> gatherTokens(string text) {
    set<string>  uniqueTokens;
    istringstream textStream(text);
    string currentToken;

    while (textStream >> currentToken) {   // this command cleans the token
        string clearToken =cleanToken(currentToken);

        if (!clearToken.empty()) {
            uniqueTokens.insert(clearToken);
        }
    }
    //returns a set of unique cleaned tokens
    return uniqueTokens;
}

/**
 * Builds the inverted index from the given data file. The existing contents
 * of `index` are left in place.
 */
int buildIndex(string filename, map<string, set<string>>& index) {

    ifstream inputfile(filename);     // checks if the file is successfully opened or not
    if (!inputfile.is_open()) {
        return 0;
    }

    string url, content;     // this declares variables to store URL and content

    int linesProcessed = 0;     // initialize a counter for the number of lines that are processed

    while (getline(inputfile, url) && getline(inputfile, content)) {    //reads the file line by line, with each pair of lines representing the URL and content
        set<string> tokens = gatherTokens(content);

        for (const string& token : tokens) { // iterate through each token in the set, updating the index.
            index[token].insert(url);
        }
        linesProcessed++;
    }
    inputfile.close();
    return linesProcessed; //returns the total number of lines processed

}

/**
 * Runs a search query on the provided index, and returns a set of results.
 *
 * Search terms are processed using set operations (union, intersection, difference).
 */
set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
   istringstream iss(sentence); // here we made a input string stream to extract tokens from the input text
    string token;
    set<string> outcome;

    while (iss >> token) {
        bool isFirstQueryText = false;
        if (token[0] == '-') {
            isFirstQueryText = true;
            token = token.substr(1);
        }
        //clean token 
        set<string> tokenMatches = index[cleanToken(token)]; //this function will obtain the list of URLs connected with the cleansed token
        
        if (isFirstQueryText) {   // this function checks if negation is required or not
            set<string> uniqueData;
            set_difference(outcome.begin(), outcome.end(), tokenMatches.begin(), tokenMatches.end(), inserter(uniqueData, uniqueData.begin()));
            outcome = uniqueData;
        } else {     // here it tests if the 'token' begins with '+', which implies intersection
                if (token[0] == '+') {
                    set<string> uniqueData;
                    set_intersection(outcome.begin(), outcome.end(), tokenMatches.begin(), tokenMatches.end(), inserter(uniqueData, uniqueData.begin()));
                    outcome = uniqueData;
                } else {
                    set<string> uniqueData;
                    set_union(outcome.begin(), outcome.end(), tokenMatches.begin(), tokenMatches.end(), inserter(uniqueData, uniqueData.begin()));
                    outcome = uniqueData;
                }
        }
    }
    return outcome;
}

/**
 * Runs the main command loop for the search program
 */
void searchEngine(string  filename) {
    map<string,  set<string>> index; // here we set up a map to store the inverted index
    int findString =buildIndex(filename, index);
    if (findString ==  0) {
        cout << "Invalid filename." << endl;
        cout << "Stand by while building index..." << endl;
    } else {
        cout << "Stand by while building index..." << endl;
    }
    cout << "Indexed " << findString << " pages containing " <<index.size() << " unique terms" << endl;
    cout << endl;

    string userInputText;
    do {
        cout << "Enter query sentence (press enter to quit): ";
        getline(cin,  userInputText);

        if (!userInputText.empty()) {   // here we call findQueryMatches to retrieve pages matching the user's query
            set<string> searchMatches =findQueryMatches(index, userInputText);   // this command line checks if there are no matching pages and display a message accordingly
            if (searchMatches.empty()) {
                cout << "Found 0 matching pages" << endl;
            } else {
                cout << "Found " <<searchMatches.size() <<  " matching pages" << endl;
                for (const string& url : searchMatches) {
                    cout << url << endl;
                }
            }
            cout << endl;
        }
    } while (!userInputText.empty());

    cout << "Thank you for searching!" << endl;
}