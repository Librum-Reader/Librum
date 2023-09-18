#pragma once
#include <mupdf/classes2.h>
#include <QString>

namespace application::core::utils
{

struct SearchHit
{
    int pageNumber;
    mupdf::FzQuad rect;
};

/**
 * The BookSearcher class searches for text in a book and provides the search
 * results.
 */
class BookSearcher
{
public:
    BookSearcher(mupdf::FzDocument* fzDocument);

    void search(const QString& text);
    void clearSearch();
    SearchHit firstSearchHit();
    SearchHit nextSearchHit();
    SearchHit previousSearchHit();

private:
    void extractSearchHitsFromBook(std::vector<SearchHit>& results,
                                   const char* text) const;

    mupdf::FzDocument* m_fzDocument;
    std::vector<SearchHit> m_searchHits;
    SearchHit m_invalidSearchHit { -1, mupdf::FzQuad() };
    int m_currentSearchHit;
};

}  // namespace application::core::utils