#include <iostream>
#include "ConvertJSON.h"
#include "InvertedIndex.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <vector>
#include <gtest/gtest.h>
#include "SearchServer.h"


using namespace std;
void TestInvertedIndexFunctionality(
    const vector<string>& docs,
    const vector<string>& requests,
    const std::vector<vector<Entry>>& expected
    ) {
    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    for(auto& request : requests) {
        std::vector<Entry> word_count = idx.GetWordCount(request);
        result.push_back(word_count);
    }
    ASSERT_EQ(result, expected);
}
TEST(TestCaseInvertedIndex, TestBasic) {
    const vector<string> docs = {
        "london is the capital of great britain",
        "big ben is the nickname for the Great bell of the striking clock"
    };
    const vector<string> requests = {"london", "the"};
    const vector<vector<Entry>> expected = {
        {
            {0, 1}
        }, {
            {0, 1}, {1, 3}
        }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestBasic2) {
    const vector<string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };
    const vector<string> requests = {"milk", "water", "cappuchino"};
    const vector<vector<Entry>> expected = {
        {
            {0, 4}, {1, 1}, {2, 5}
        }, {
            {0, 2}, {1, 2}, {2, 5}
        }, {
            {3, 1}
        }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}
TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
    const vector<string> docs = {
        "a b c d e f g h i j k l",
        "statement"
    };
    const vector<string> requests = {"m", "statement"};
    const vector<vector<Entry>> expected = {
        {
        }, {
            {1, 1}
        }
    };
    TestInvertedIndexFunctionality(docs, requests, expected);
}


int main(int argc,char** argv){
        ConverterJSON conv;
        std::map<std::string, std::vector<Entry>> freqDictionary;
        std::vector<std::string> texts;
        std::vector<std::string> requests;
        InvertedIndex inv;
        inv.UpdateDocumentBase(conv.GetTextDocuments());
        try {
            SearchServer searchSer(inv);
            searchSer.search(searchSer.requestsInput());
            texts = conv.GetTextDocuments();
            requests = conv.GetRequests();
            conv.putAnswers({{{2,5.34},{8,12.7}},{{1,7.67},{2,34.67},{3,6.4}}});
        } catch (OpeningError ex) {
            std::cerr << ex.what() << std::endl;
            return 1;
        } catch (JsonFileContainingError ex) {
            std::cerr << ex.what() << std::endl;
            return 1;
        }
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();

        return 0;
    }
