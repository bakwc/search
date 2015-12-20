#include <iostream>
#include <fstream>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>

#include "n_gram_searcher.h"
#include "stupid_searcher.h"
#include "suffix_tree_searcher.h"
#include "suffix_array_searcher.h"

using namespace std;

void CheckSearchRes(const std::vector<std::string>& results,
                    const std::vector<std::string>& required)
{
    if (results.size() != required.size()) {
        std::cerr << "[ERROR] result size missmatch\n";
    }
    std::unordered_set<std::string> resSet(results.begin(), results.end());
    for (auto&& r: required) {
        if (resSet.find(r) == resSet.end()) {
            std::cerr << "[ERROR] Missing " << r << "\n";
        }
    }
}

void SearcherUT(ISearcher* searcher) {
    searcher->Add("aq");
    searcher->Add("abc def");
    searcher->Add("qwe dedo");
    searcher->Add("temp test abc");
    searcher->Add("iabovq");
    searcher->Add("q");
    searcher->Add("qp");
    searcher->Add("defqwerty");

    searcher->Prepare();

    CheckSearchRes(searcher->Search("qwe", 100), {"qwe dedo", "defqwerty"});
    CheckSearchRes(searcher->Search("bovq", 100), {"iabovq"});
    CheckSearchRes(searcher->Search("ololo", 100), {});
    CheckSearchRes(searcher->Search("q", 100), {"aq", "qwe dedo", "iabovq", "q", "qp", "defqwerty"});
    CheckSearchRes(searcher->Search("i", 100), {"iabovq"});
    CheckSearchRes(searcher->Search("emp test ab", 100), {"temp test abc"});
}

std::vector<std::string> LoadWords() {
    std::vector<std::string> res;
    std::ifstream in("words.txt");
    std::string word;
    while (in >> word) {
        res.push_back(word);
    }
    return res;
}

constexpr int DOCUMENTS_NUMBER = 100000;

void SearcherChecker(ISearcher* baseSearcher, ISearcher* searcher) {
//    SearcherUT(searcher);

    std::cerr << "[INFO] Loading words\n";
    std::vector<std::string> words = LoadWords();
    std::cerr << "[INFO] Loaded " << words.size() << " words\n";
    auto engine = std::default_random_engine();
    engine.seed(0);
    std::shuffle(std::begin(words), std::end(words), engine);

    std::vector<std::string> documents;
    std::vector<std::string> queries;

    for (size_t i = 0; i < DOCUMENTS_NUMBER; ++i) {
        documents.push_back(words[i]);
    }
    for (size_t i = DOCUMENTS_NUMBER - 100; i < DOCUMENTS_NUMBER + 100; ++i) {
        const std::string& word = words[i];
        queries.push_back(word);
        queries.push_back(word.substr(word.size() / 2, 1));

        if (word.size() >= 5) {
            size_t from = (word.size() - 3) / 2;
            queries.push_back(words[i].substr(from, 3));
        }

        if (word.size() >= 7) {
            size_t from = (word.size() - 5) / 2;
            queries.push_back(words[i].substr(from, 3));
        }
    }

    std::shuffle(std::begin(documents), std::end(documents), engine);
    std::shuffle(std::begin(queries), std::end(queries), engine);

    std::cerr << "[INFO] Prepare index\n";
    for (size_t i = 0; i < documents.size(); ++i) {
        baseSearcher->Add(documents[i]);
        searcher->Add(documents[i]);
    }
    baseSearcher->Prepare();
    searcher->Prepare();
    std::cerr << "[INFO] Start checking, queries: " << queries.size() << "\n";

    for (size_t i = 0; i < queries.size(); ++i) {
        std::string query = queries[i];
        std::vector<std::string> baseResults = baseSearcher->Search(query, documents.size() + 1);
        std::unordered_set<std::string> baseResultsSet(baseResults.begin(), baseResults.end());

        std::vector<std::string> results = searcher->Search(query, documents.size() + 1);
        std::unordered_set<std::string> resultsSet(results.begin(), results.end());

        if (baseResultsSet.size() != resultsSet.size()) {
            std::cerr << "[ERROR] baseSearcher and searcher results size missmatch, " << query << "\n";
            std::cerr << baseResultsSet.size() << " " << resultsSet.size() << "\n";
        }

        for (auto&& r: baseResultsSet) {
            if (resultsSet.find(r) == resultsSet.end()) {
                std::cerr << "[ERROR] missing " << r << " in results, query: " << query << "\n";
            }
        }

        for (auto&& r: resultsSet) {
            if (baseResultsSet.find(r) == baseResultsSet.end()) {
                std::cerr << "[ERROR] wrong result " << r << " in results, query: " << query << "\n";
            }
        }
    }
}

void SearcherBench(std::vector<std::pair<std::string, ISearcher*>> searchers) {
    std::cerr << "[INFO] Loading words\n";
    std::vector<std::string> words = LoadWords();
    std::cerr << "[INFO] Loaded " << words.size() << " words\n";
    auto engine = std::default_random_engine();
    engine.seed(0);
    std::shuffle(std::begin(words), std::end(words), engine);

    std::vector<std::string> documents;
    std::vector<std::string> queries;

    for (size_t i = 0; i < DOCUMENTS_NUMBER; ++i) {
        documents.push_back(words[i]);
    }
    for (size_t i = DOCUMENTS_NUMBER - 300; i < DOCUMENTS_NUMBER + 300; ++i) {
        const std::string& word = words[i];
        queries.push_back(word);
        queries.push_back(word.substr(word.size() / 2, 1));

        if (word.size() >= 5) {
            size_t from = (word.size() - 3) / 2;
            queries.push_back(words[i].substr(from, 3));
        }

        if (word.size() >= 7) {
            size_t from = (word.size() - 5) / 2;
            queries.push_back(words[i].substr(from, 3));
        }
    }

    std::shuffle(std::begin(documents), std::end(documents), engine);
    std::shuffle(std::begin(queries), std::end(queries), engine);

    std::cerr << "[INFO] Prepare index\n";
    for (size_t i = 0; i < documents.size(); ++i) {
        for (auto&& s: searchers) {
            s.second->Add(documents[i]);
        }
    }
    for (auto&& s: searchers) {
        s.second->Prepare();
    }
    std::cerr << "[INFO] Start searching, queries: " << queries.size() << "\n";

    for (auto&& s: searchers) {
        auto begin = std::chrono::high_resolution_clock::now();

        int totalSize = 0;
        int totalQueries = 0;

        for (size_t n = 0; n < 300; ++n) {
            totalQueries += queries.size();
            for (size_t i = 0; i < queries.size(); ++i) {
                totalSize += s.second->Search(queries[i], 10).size();
            }
        }

        auto end = std::chrono::high_resolution_clock::now();
        size_t ms = std::chrono::duration_cast<std::chrono::milliseconds>(end-begin).count();

        std::cerr << "[INFO] " << s.first << ": " << totalSize << " / " << totalQueries << " in " << ms << " ms, " << int(float(totalQueries) / ms * 1000.0) << "\n";
    }
}

int main() {
//    TStupidSearcher baseSearcher;
//    TSuffixArraySearcher searcher;
//    SearcherChecker(&baseSearcher, &searcher);

    TNgramSearcher nGramSearcher(12);
    TSuffixTreeSearcher suffixTreeSearcher;
    TSuffixArraySearcher suffixArraySearcher;
    SearcherBench({{"n-gram", &nGramSearcher},
                   {"suffix tree", &suffixTreeSearcher},
                   {"suffix array", &suffixArraySearcher}});

    return 0;
}
