#include "suffix_array_searcher.h"


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

inline bool startsWith(const std::string& haystack, const std::string& needle) {
    return needle.length() <= haystack.length()
        && std::equal(needle.begin(), needle.end(), haystack.begin());
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
    std::unordered_set<const std::string*> resultsSet;
    for (int i = mid + 1; i < Index.size(); ++i) {
        const TIndexElement& e = Index[i];
        if (!startsWith(e.second, query)) {
            break;
        }
        resultsSet.insert(e.first);
        if (resultsSet.size() >= resultsNumber) {
            break;
        }
    }
    for (int i = mid; i >= 0; --i) {
        const TIndexElement& e = Index[i];
        if (!startsWith(e.second, query)) {
            break;
        }
        resultsSet.insert(e.first);
        if (resultsSet.size() >= resultsNumber) {
            break;
        }
    }
    std::vector<std::string> results;
    for (auto&& e: resultsSet) {
        results.push_back(*e);
    }
    return results;
}
