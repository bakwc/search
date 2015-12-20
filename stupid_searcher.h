#include "searcher.h"

class TStupidSearcher: public ISearcher {
public:
    void Add(const std::string& document) final;
    void Del(const std::string& document) final;
    std::vector<std::string> Search(const std::string& query, size_t resultsNumber) final;
private:
    std::vector<std::string> Words;
};
