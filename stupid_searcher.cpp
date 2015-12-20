#include "stupid_searcher.h"


void TStupidSearcher::Add(const std::string& document) {
    Words.push_back(document);
}

void TStupidSearcher::Del(const std::string& document) {
}

std::vector<std::string> TStupidSearcher::Search(const std::string& query, size_t resultsNumber) {
    std::vector<std::string> res;
    for (size_t i = 0; i < Words.size(); ++i) {
        const std::string& w = Words[i];
        if (w.find(query) != std::string::npos) {
            res.push_back(w);
            if (res.size() >= resultsNumber) {
                break;
            }
        }
    }
    return res;
}
