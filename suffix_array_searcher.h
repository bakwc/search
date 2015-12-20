#include "searcher.h"

class TSuffixArraySearcher: public ISearcher {
    using TIndexElement = std::pair<const std::string*, std::string>;
public:
    void Add(const std::string& document) final;
    void Del(const std::string& document) final;
    void Prepare() final;
    std::vector<std::string> Search(const std::string& query, size_t resultsNumber) final;
private:
    TStringSet Documents;
    std::vector<TIndexElement> Index;
};
