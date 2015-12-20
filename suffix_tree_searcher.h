#include "searcher.h"

#include <memory>
#include <unordered_map>
#include <unordered_set>

class TSuffixTreeNode {
    using TSuffixTreeNodePtr = std::unique_ptr<TSuffixTreeNode>;
public:
    void Add(const std::string& substring, const void* element);
    std::vector<const void*> Search(const std::string& substring, size_t resultsNumber);
private:
    std::unordered_map<char, TSuffixTreeNodePtr> Children;
    std::unordered_set<const void*> Elements;
};

class TSuffixTreeSearcher: public ISearcher {
public:
    void Add(const std::string& document) final;
    void Del(const std::string& document) final;
    std::vector<std::string> Search(const std::string& query, size_t resultsNumber) final;
private:
    TSuffixTreeNode SuffixTree;
    TStringSet Documents;
};
