#pragma once
#include <QObject>

namespace tests::application
{

// Need to extract TestEnum out into its own header file for MOC to work
class TestEnum : public QObject
{
    Q_OBJECT

public:
    enum class Enum
    {
        First = 0,
        Second,
        Third
    };

    Q_ENUM(Enum);
};

}  // namespace tests::application