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

    connect(m_freeBooksService,
            &application::IFreeBooksService::receivedNoMetadata, this,
            &FreeBooksController::foundNoBooks);

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
    if(m_firstPageIsLoaded)
        return;

    deleteAllBooks();
    m_freeBooksService->fetchFirstBooksMetadataPageWithFilter(
        m_filterAuthorsAndTitle);
    emit startedFetchingFistMetadataPage();
}

void FreeBooksController::getBookMedia(const int id, const QString& url)
{
    m_freeBooksService->getBookMedia(id, url);
}

void FreeBooksController::getBookCover(const int id)
{
// There is a problem with book covers (jpegs) on linux with Qt 6.5.2, so don't
// load them for now.
#ifdef Q_OS_WIN
    m_freeBooksService->getBookCover(id);
#endif
}

void FreeBooksController::deleteBookCover(const int id)
{
    m_freeBooksService->deleteBookCover(id);
}

void FreeBooksController::setFilterAuthorsAndTitle(
    const QString& authorsAndTitle)
{
    m_filterAuthorsAndTitle = authorsAndTitle;
    m_firstPageIsLoaded = false;
}

QString FreeBooksController::getFilterAuthorsAndTitle()
{
    return m_filterAuthorsAndTitle;
}

void FreeBooksController::clearAllFilters()
{
    m_filterAuthorsAndTitle.clear();
    m_firstPageIsLoaded = false;
}

data_models::FreeBooksModel* FreeBooksController::getFreeBooksModel()
{
    return &m_freeBooksModel;
}

void FreeBooksController::proccessFetchingFirstMetadataPageResult(bool result)
{
    m_firstPageIsLoaded = result;
    emit fetchingFirstMetadataPageSuccessful(result);
}

void FreeBooksController::deleteAllBooks()
{
    m_freeBooksService->deleteAllBooks();
    m_freeBooksModel.clear();
}

}  // namespace adapters::controllers
