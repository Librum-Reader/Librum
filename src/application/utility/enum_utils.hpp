#pragma once
#include <QMetaEnum>
#include <QString>
#include <optional>

namespace application::utility
{

template<typename Enum>
QString getNameForEnumValue(Enum value)
{
    QMetaEnum meta = QMetaEnum::fromType<Enum>();
    auto text = meta.valueToKey(static_cast<int>(value));

    return QString::fromLatin1(text);
}

template<typename Enum>
std::optional<Enum> getValueForEnumName(const QString& name)
{
    QMetaEnum meta = QMetaEnum::fromType<Enum>();
    auto key = meta.keyToValue(name.toLatin1());
    if(key == -1)
        return std::nullopt;

    return static_cast<Enum>(key);
}

}  // namespace application::utility