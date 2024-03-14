#pragma once
#include <QObject>
#include "adapters_export.hpp"
#include "i_tools_controller.hpp"
#include "i_tools_service.hpp"

namespace adapters::controllers
{

class ADAPTERS_EXPORT ToolsController : public IToolsController
{
    Q_OBJECT

public:
    ToolsController(application::IToolsService* toolsService);

    void merge(const QString& destName,
               const QList<QString>& filePaths) override;
    void extractPages(const QString& destName, const QString& filePath,
                      const QString& separatorString) override;
    void imageToPdf(const QString& destName, const QString& filePath) override;

private:
    application::IToolsService* m_toolsService;
};

}  // namespace adapters::controllers
