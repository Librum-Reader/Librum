#include "pdf_to_rum_converter.hpp"
#include <QDebug>


using namespace domain::rum_content;

namespace application::converters
{

void PdfToRumConverter::convertToRum(QString path)
{
    Book b;
    qDebug() << "converting ... " + path;
    
    emit conversionFinished(b);
}

} // namespace application::converters