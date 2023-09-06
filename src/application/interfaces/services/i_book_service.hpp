#pragma once
#include <QDir>
#include <QObject>
#include <QString>
#include <QUuid>
#include "application_export.hpp"
#include "mupdf/classes.h"

namespace application
{

/**
 * The BookService is set up with one specific book and operates only on it. It
 * is able to get and change information on one specific book.
 */
class APPLICATION_EXPORT IBookService : public QObject
{
    Q_OBJECT

public:
    virtual ~IBookService() noexcept = default;

    virtual void setUp(QUuid uuid) = 0;
    virtual mupdf::FzDocument* getFzDocument() = 0;
};

}  // namespace application