#include <QObject>
#include <QString>
#include "i_pdf_to_rum_converter.hpp"


namespace application::converters
{

class PdfToRumConverter : public IPdfToRumConverter
{
    Q_OBJECT
    
public:
    void convertToRum(QString path) override;
};

} // namespace application::converts