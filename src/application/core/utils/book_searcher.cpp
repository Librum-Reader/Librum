#include "book_searcher.hpp"

namespace application::core::utils
{

BookSearcher::BookSearcher(mupdf::FzDocument* fzDocument) :
    m_fzDocument(fzDocument)
{
}

void BookSearcher::search(const QString& text)
{
    clearSearch();
    extractSearchHitsFromBook(m_searchHits, text.toStdString().c_str());
}

void BookSearcher::clearSearch()
{
    m_searchHits.clear();
    m_currentSearchHit = -1;
}

SearchHit BookSearcher::firstSearchHit()
{
    if(m_searchHits.empty())
        return m_invalidSearchHit;

    auto hit = m_searchHits.front();
    m_currentSearchHit = 0;

    return hit;
}

SearchHit BookSearcher::nextSearchHit()
{
    if(m_currentSearchHit == -1 || m_searchHits.empty())
        return m_invalidSearchHit;

    // Wrap to the beginning once you are over the end
    ++m_currentSearchHit;
    if(m_currentSearchHit >= m_searchHits.size())
    {
        m_currentSearchHit = 0;
    }

    auto hit = m_searchHits.at(m_currentSearchHit);
    return hit;
}

SearchHit BookSearcher::previousSearchHit()
{
    if(m_currentSearchHit == -1 || m_searchHits.empty())
        return m_invalidSearchHit;

    // Wrap to the beginning once you are over the end
    --m_currentSearchHit;
    if(m_currentSearchHit <= 0)
    {
        m_currentSearchHit = m_searchHits.size() - 1;
    }

    auto hit = m_searchHits.at(m_currentSearchHit);
    return hit;
}

void BookSearcher::extractSearchHitsFromBook(std::vector<SearchHit>& results,
                                             const char* text) const
{
    mupdf::FzStextOptions options;
    const int maxHits = 1000;

    for(int i = 0; i < m_fzDocument->fz_count_pages(); ++i)
    {
        mupdf::FzStextPage textPage(*m_fzDocument, i, options);
        int hitMarks[maxHits];
        auto hits = textPage.search_stext_page(text, hitMarks, maxHits);

        results.reserve(hits.size());
        for(auto& hit : hits)
        {
            SearchHit searchHit {
                .pageNumber = i,
                .rect = hit,
            };
            results.emplace_back(searchHit);
        }
    }
}

}  // namespace application::core::utils