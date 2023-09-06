#include <QObject>
#include "adapters_export.hpp"
#include "mupdf/classes.h"
#pragma once

namespace adapters
{

/**
 * The BookController class is set up to work one one book at the time.
 * It has access to the IBookService interface, which makes it possible to get
 * information about a specific book such as getting its highlights, the
 * underyling FzDocument and similar.
 */
class ADAPTERS_EXPORT IBookController : public QObject
{
    Q_OBJECT

public:
    virtual ~IBookController() noexcept = default;

    Q_INVOKABLE virtual void setUp(QString uuid) = 0;
    virtual mupdf::FzDocument* getFzDocument() = 0;
};

}  // namespace adapters
