#pragma once

#include <string>
#include <vector>

class ISearcher {
public:
    virtual void Add(const std::string& document) = 0;
    virtual void Del(const std::string& document) = 0;
    virtual void Prepare() {}
    virtual std::vector<std::string> Search(const std::string& query, size_t resultsNumber) = 0;
};
