#pragma once

#include <string>
#include <vector>
#include <unordered_set>

class ISearcher {
public:
    virtual void Add(const std::string& document) = 0;
    virtual void Del(const std::string& document) = 0;
    virtual void Prepare() {}
    virtual std::vector<std::string> Search(const std::string& query, size_t resultsNumber) = 0;
};

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

using TStringSet = std::unordered_set<TStringPtr, StrPtrHash, StrPtrEq>;
