#include "free_books_controller.hpp"

namespace adapters::controllers
{

FreeBooksController::FreeBooksController(
    application::IFreeBooksService* freeBooksService) :
    m_freeBooksService(freeBooksService),
    m_freeBooksModel(&m_freeBooksService->getFreeBooks())
{
    // getting api info
    connect(m_freeBooksService, &application::IFreeBooksService::apiInfoReady,
            &m_freeBooksModel, &data_models::FreeBooksModel::setApiInfo);

    // getting books metadata
    connect(
        m_freeBooksService,
        &application::IFreeBooksService::fetchingFirstMetadataPageSuccessful,
        this, &FreeBooksController::proccessFetchingFirstMetadataPageResult);

    connect(&m_freeBooksModel,
            &adapters::data_models::FreeBooksModel::fetchBooksMetadataPage,
            m_freeBooksService,
            &application::IFreeBooksService::fetchBooksMetadataPage);

    // getting book media
    connect(m_freeBooksService,
            &application::IFreeBooksService::gettingBookFinished, this,
            &FreeBooksController::gettingBookFinished);

    // book insertion
    connect(m_freeBooksService,
            &application::IFreeBooksService::bookInsertionStarted,
            &m_freeBooksModel, &data_models::FreeBooksModel::startInsertingRow);

    connect(m_freeBooksService,
            &application::IFreeBooksService::bookInsertionEnded,
            &m_freeBooksModel, &data_models::FreeBooksModel::endInsertingRow);


    // book deletion
    connect(m_freeBooksService,
            &application::IFreeBooksService::bookDeletionStarted,
            &m_freeBooksModel, &data_models::FreeBooksModel::startDeletingBook);

    connect(m_freeBooksService,
            &application::IFreeBooksService::bookDeletionEnded,
            &m_freeBooksModel, &data_models::FreeBooksModel::endDeletingBook);


    // book clearing
    connect(m_freeBooksService,
            &application::IFreeBooksService::bookClearingStarted,
            &m_freeBooksModel, &data_models::FreeBooksModel::startBookClearing);

    connect(m_freeBooksService,
            &application::IFreeBooksService::bookClearingEnded,
            &m_freeBooksModel, &data_models::FreeBooksModel::endBookClearing);


    // data changed
    connect(m_freeBooksService, &application::IFreeBooksService::dataChanged,
            &m_freeBooksModel, &data_models::FreeBooksModel::refreshBook);

    // download book media progress changed
    connect(
        m_freeBooksService,
        &application::IFreeBooksService::downloadingBookMediaProgressChanged,
        &m_freeBooksModel,
        &data_models::FreeBooksModel::downloadingBookMediaProgressChanged);
}

void FreeBooksController::fetchFirstBooksMetadataPage()
{
    if(!m_isFirstBooksMetadataPageFetchingAllowed)
        return;

    deleteAllBooks();
    m_freeBooksService->fetchFirstBooksMetadataPageWithFilter(
        m_filterAuthorsAndTitle);
}

void FreeBooksController::getBookMedia(const int id, const QString& url)
{
    m_freeBooksService->getBookMedia(id, url);
}

void FreeBooksController::getBookCover(const int id)
{
    m_freeBooksService->getBookCover(id);
}

void FreeBooksController::deleteBookCover(const int id)
{
    m_freeBooksService->deleteBookCover(id);
}

void FreeBooksController::setFilterAuthorsAndTitle(
    const QString& authorsAndTitle)
{
    m_filterAuthorsAndTitle = authorsAndTitle;
    m_isFirstBooksMetadataPageFetchingAllowed = true;
}

void FreeBooksController::clearAllFilters()
{
    m_filterAuthorsAndTitle.clear();
    m_isFirstBooksMetadataPageFetchingAllowed = true;
}

data_models::FreeBooksModel* FreeBooksController::getFreeBooksModel()
{
    return &m_freeBooksModel;
}

void FreeBooksController::proccessFetchingFirstMetadataPageResult(
    const bool result)
{
    m_isFirstBooksMetadataPageFetchingAllowed = !result;
}

void FreeBooksController::deleteAllBooks()
{
    m_freeBooksService->deleteAllBooks();
    m_freeBooksModel.clear();
}

}  // namespace adapters::controllers
