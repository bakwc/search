#include "suffix_tree_searcher.h"

#include <iostream>

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
    const std::vector<const void*> baseResults = SuffixTree.Search(query, resultsNumber);
    for (size_t i = 0; i < baseResults.size(); ++i) {
        results.push_back(*(const std::string*)baseResults[i]);
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

std::vector<const void*> TSuffixTreeNode::Search(const std::string& substring, size_t resultsNumber) {
    std::vector<const void*> results;
    if (substring.empty()) {
        for (auto&& e: Elements) {
            results.push_back(e);
            if (results.size() >= resultsNumber) {
                return results;
            }
        }
        for (auto&& c: Children) {
            auto&& r = c.second->Search(substring, resultsNumber - results.size());
            for (auto&& e: r) {
                results.push_back(e);
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
