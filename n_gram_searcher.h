#pragma once

#include <unordered_map>
#include <unordered_set>
#include <memory>

#include "searcher.h"

class TNgramSearcher: public ISearcher {
    using TStringPtrSet = std::unordered_set<const std::string*>;
public:
    explicit TNgramSearcher(int ngrams = 4);
    void Add(const std::string& document) final;
    void Del(const std::string& document) final;
    std::vector<std::string> Search(const std::string& query, size_t resultsNumber) final;
private:
    int NGrams;
    TStringSet Documents;
    std::unordered_map<std::string, TStringPtrSet> Index;
};
