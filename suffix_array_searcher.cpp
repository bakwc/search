#include "suffix_array_searcher.h"

#include <iostream>

void TSuffixArraySearcher::Add(const std::string& document) {
    TStringPtr ptr(new std::string(document));
    const std::string* docPtr = ptr.get();
    Documents.insert(std::move(ptr));

    for (size_t i = 0; i < document.size(); ++i) {
        Index.push_back(std::make_pair(docPtr, document.substr(i)));
    }
}

void TSuffixArraySearcher::Del(const std::string& document) {
}

void TSuffixArraySearcher::Prepare() {
    std::sort(Index.begin(), Index.end(), [](const TIndexElement& a, TIndexElement& b) {
       return a.second < b.second;
    });
}

std::vector<std::string> TSuffixArraySearcher::Search(const std::string& query, size_t resultsNumber) {
    long long first = 0;
    long long last = Index.size();
    long long mid = 0;
    while (first <= last) {
        mid = (first + last) / 2;
        if (query > Index[mid].second) {
            first = mid + 1;
        } else if (query < Index[mid].second) {
            last = mid - 1;
        } else {
            break;
        }
    }
    std::vector<std::string> results;
    for (size_t i = first; i < Index.size(); ++i) {
        const TIndexElement& e = Index[i];
        if (e.second.find(query) == std::string::npos) {
            if (i == first) {
                continue;
            }
            break;
        }
        results.push_back(*e.first);
        if (results.size() >= resultsNumber) {
            break;
        }
    }
    return results;
}
