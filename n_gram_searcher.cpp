#include "n_gram_searcher.h"


TNgramSearcher::TNgramSearcher(int ngrams)
    : NGrams(ngrams)
{
}

void TNgramSearcher::Add(const std::string& document) {
    TStringPtr ptr(new std::string(document));
    const std::string* docPtr = ptr.get();
    Documents.insert(std::move(ptr));
    for (int i = 1; i <= NGrams; ++i) {
        for (int j = 0; j <= int(document.size()) - i; ++j) {
            std::string key = document.substr(j, i);
            Index[key].insert(docPtr);
        }
    }
}

void TNgramSearcher::Del(const std::string& document) {
}

std::vector<std::string> TNgramSearcher::Search(const std::string& query, size_t resultsNumber) {
    std::vector<const TStringPtrSet*> results;
    results.reserve(resultsNumber);
    int ngrams = std::min(NGrams, (int)query.size());

    for (int j = 0; j <= int(query.size()) - ngrams; ++j) {
        std::string key = query.substr(j, ngrams);
        auto it = Index.find(key);
        if (it != Index.end()) {
            results.push_back(&it->second);
        }
    }

    std::vector<std::string> searchResults;
    if (results.empty()) {
        return searchResults;
    }

    searchResults.reserve(results.size());

    std::sort(results.begin(), results.end(), [](const TStringPtrSet* a, const TStringPtrSet* b) {
        return a->size() < b->size();
    });
    const TStringPtrSet* baseResult = results[0];
    for (auto&& it = baseResult->begin(); it != baseResult->end(); ++it) {
        const std::string* document = *it;
        if (document->find(query) == std::string::npos) {
            continue;
        }
        bool match = true;
        for (size_t j = 1; j < results.size(); ++j) {
            const TStringPtrSet* currentResult = results[j];
            if (currentResult->find(document) == currentResult->end()) {
                match = false;
                break;
            }
        }
        if (match) {
            searchResults.push_back(*document);
            if (searchResults.size() >= resultsNumber) {
                break;
            }
        }
    }
    return searchResults;
}

