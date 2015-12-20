#pragma once

#include <unordered_map>
#include <unordered_set>
#include <memory>

#include "searcher.h"

using TStringPtr = std::unique_ptr<std::string>;
struct StrPtrHash {
    unsigned long operator()(const TStringPtr& v) {
        return std::hash<std::string>()(*v);
    }
};
struct StrPtrEq {
    bool operator()(const TStringPtr& v1, const TStringPtr& v2) {
        return *v1 == *v2;
    }
};

class TNgramSearcher: public ISearcher {
    using TStringSet = std::unordered_set<TStringPtr, StrPtrHash, StrPtrEq>;
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
