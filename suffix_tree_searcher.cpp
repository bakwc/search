#include "suffix_tree_searcher.h"


void TSuffixTreeSearcher::Add(const std::string& document) {
    TStringPtr ptr(new std::string(document));
    const std::string* docPtr = ptr.get();
    Documents.insert(std::move(ptr));

    for (size_t i = 0; i < document.size(); ++i) {
        SuffixTree.Add(document.substr(i), docPtr);
    }
}

void TSuffixTreeSearcher::Del(const std::string& document) {
}

std::vector<std::string> TSuffixTreeSearcher::Search(const std::string& query, size_t resultsNumber) {
    std::vector<std::string> results;
    const std::unordered_set<const void*> baseResults = SuffixTree.Search(query, resultsNumber);
    for (auto&& e: baseResults) {
        results.push_back(*(const std::string*)e);
    }
    return results;
}

void TSuffixTreeNode::Add(const std::string& substring, const void* element) {
    if (substring.empty())  {
        Elements.insert(element);
        return;
    }
    char first = substring[0];
    TSuffixTreeNodePtr& child = Children[first];
    if (!child) {
        child.reset(new TSuffixTreeNode());
    }
    child->Add(substring.substr(1), element);
}

std::unordered_set<const void*> TSuffixTreeNode::Search(const std::string& substring, size_t resultsNumber) {
    std::unordered_set<const void*> results;
    if (substring.empty()) {
        for (auto&& e: Elements) {
            results.insert(e);
            if (results.size() >= resultsNumber) {
                return results;
            }
        }
        for (auto&& c: Children) {
            auto&& r = c.second->Search(substring, resultsNumber - results.size());
            for (auto&& e: r) {
                results.insert(e);
                if (results.size() >= resultsNumber) {
                    return results;
                }
            }
        }
        return results;
    }
    char first = substring[0];
    auto&& it = Children.find(first);
    if (it == Children.end()) {
        return results;
    }
    return it->second->Search(substring.substr(1), resultsNumber);
}
