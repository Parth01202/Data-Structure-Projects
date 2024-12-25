#include <gtest/gtest.h>

#include "search.h"
#include "tests/build_index.h"
#include "tests/clean_token.h"
#include "tests/find_query_matches.h"
#include "tests/gather_tokens.h"

TEST(CleanToken, ExampleEmptyTest) {
    cout << "Hi! This shows up in the terminal." << endl;
}
TEST(CleanToken, NoPunctuation) {    //test a token without any punctuation
    EXPECT_EQ(cleanToken("hello"), "hello");
}

TEST(CleanToken,  punctuationinmiddleandatstart) { //token with  punctuation in middle, and the start
    EXPECT_EQ(cleanToken("..mo.nday"), "mo.nday");
}

TEST(CleanToken,  punctuationinmiddleandatEND) { //token with  punctuation in middle, and at the end
    EXPECT_EQ(cleanToken("mo.nday.."), "mo.nday");
}

TEST(CleanToken, LeadingPunctuation) { //token with punctuation at the beginning
    EXPECT_EQ(cleanToken("!monday"), "monday");
}

TEST(CleanToken, TrailingPunctuation) {//test a token with punctuation at the end
    EXPECT_EQ(cleanToken("monday!"), "monday");
}

TEST(CleanToken, NoLetters) {//test a token with no letters    
    EXPECT_EQ(cleanToken("*&%"), "");
}

TEST(CleanToken, UppercaseLetters) {    //test a token with uppercase letters
    EXPECT_EQ(cleanToken("MONday"), "monday");
}

TEST(CleanToken, PunctuationBothEnds) {    //test a token with punctuation at both ends
    EXPECT_EQ(cleanToken("!monday!"), "monday");
}

TEST(CleanToken, PunctuationMiddle) {    //test a token with punctuation in the middle
    EXPECT_EQ(cleanToken("mon!day"), "mon!day");
}

TEST(CleanToken, NumbersAtEnd) {    //test a token with numbers at the end
    EXPECT_EQ(cleanToken("monday222"), "monday222");
}

TEST(CleanToken, OneLetterLong) {    //test a token that is one letter long
    EXPECT_EQ(cleanToken("e"), "e");
}

TEST(CleanToken, LotsOfPunctuation) {     //test a token with lots of punctuation
    EXPECT_EQ(cleanToken("......e"), "e");
    EXPECT_EQ(cleanToken("e......"), "e");
    EXPECT_EQ(cleanToken("......e......"), "e");
}

TEST(CleanToken, NonAlphanumericCharacters) {
    // Test a token with characters that are not letters, digits, or punctuation
    EXPECT_EQ(cleanToken("*$#222"), "");
}

TEST(GatherTokens, NoLeadingSpaces) {
    // Test text with leading spaces
    set<string> tokens = gatherTokens("  hi parth");
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens.count("hi"), 1);
    EXPECT_EQ(tokens.count("parth"), 1);
}

TEST(GatherTokens, NoTrailingSpaces) {
    // Test text with trailing spaces
    set<string> tokens = gatherTokens("hi parth  ");
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens.count("hi"), 1);
    EXPECT_EQ(tokens.count("parth"), 1);
}

TEST(GatherTokens, MultipleSpacesBetweenWords) {
    // Test text with multiple spaces between words
    set<string> tokens = gatherTokens("hi    parth");
    EXPECT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens.count("hi"), 1);
    EXPECT_EQ(tokens.count("parth"), 1);
}

TEST(BuildIndex, Tinytxt) {
    string filename = "tiny.txt";
    map<string, set<string>> studentIndex;
    int studentNumProcesed = buildIndex(filename, studentIndex);
    EXPECT_EQ(studentNumProcesed, 4);

}

TEST(BuildIndex, invalidfilename) {
    string filename = "abc.txt";
    map<string, set<string>> studentIndex;
    int studentNumProcesed = buildIndex(filename, studentIndex);
    EXPECT_EQ(studentNumProcesed, 0);

}


TEST(FindQueryMatches, FirstSearchTermNotInMap) { // tests the findQueryMatches function when no search phrases are found in the map
    map<string, set<string>> index = {
        {"term1", {"url1", "url2"}},
        {"term2", {"url2", "url3"}}
    };
    EXPECT_EQ(findQueryMatches(index, "term4 term3"), set<string>{});
}

TEST(FindQueryMatches, SearchTermModifiedWithPlusNotInMap) {
    map<string, set<string>> index = {
        {"term1", {"url1", "url2"}},
        {"term2", {"url2", "url3"}}
    };
    EXPECT_EQ(findQueryMatches(index, "term1 +term3"), set<string>{});
}

TEST(FindQueryMatches, SearchTermModifiedWithMinusNotInMap) {
    map<string, set<string>> index = {
        {"term1", {"url1", "url2"}},
        {"term2", {"url2", "url3"}}
    };
    EXPECT_EQ(findQueryMatches(index, "term4 -term3"), set<string>{});
}

TEST(FindQueryMatches, UnmodifiedSearchTermNotInMap) {
    map<string, set<string>> index = {
        {"term1", {"url1", "url2"}},
        {"term2", {"url2", "url3"}}
    };
    EXPECT_EQ(findQueryMatches(index, "term3"), set<string>{});
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
