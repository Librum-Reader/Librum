#include "free_books_controller.hpp"

namespace adapters::controllers
{

FreeBooksController::FreeBooksController(
    application::IFreeBooksService* freeBooksService) :
    m_freeBooksService(freeBooksService),
    m_freeBooksModel(&m_freeBooksService->getFreeBooks())
{
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

void FreeBooksController::getBooksMetadata(const QString& author,
                                           const QString& title)
{
    m_freeBooksService->getBooksMetadata(author, title);
}

void FreeBooksController::getBookMedia(const int id, const QString& url)
{
    m_freeBooksService->getBookMedia(id, url);
}

data_models::FreeBooksModel* FreeBooksController::getFreeBooksModel()
{
    return &m_freeBooksModel;
}

}  // namespace adapters::controllers
