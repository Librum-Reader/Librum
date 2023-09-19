#pragma once
#include <QObject>
#include <QString>
#include <QVariantList>
#include "adapters_export.hpp"

namespace adapters::dtos
{

/**
 * An example of the JSON object:
 *  {
        "partOfSpeech": "Noun",
        "definitions": [
            {
                "definition": "First definiton of the noun"
            },
            {
                "definition": "Second definition"
            }
            {
                "definition": "Third definition",
                "examples": [
                    "An example",
                    "Another example"
                ]
            }
        ]
    },
    {
        "partOfSpeech": "Verb",
        "definitions": [
            {
                "definition": "A definition of the verb",
                "examples": [
                    "Only example"
                ]
            },
            ...
        ]
    }
 */


struct ADAPTERS_EXPORT DictionaryEntryDto
{
    Q_GADGET

public:
    QVariantList wordTypes;  // WordTypeDtos
};

struct ADAPTERS_EXPORT WordTypeDto
{
    Q_GADGET
    Q_PROPERTY(QString partOfSpeech MEMBER partOfSpeech)
    Q_PROPERTY(QVariantList definitions MEMBER definitions)

public:
    QString partOfSpeech;
    QVariantList definitions;  // WordDefinitionDtos
};

struct ADAPTERS_EXPORT WordDefinitionDto
{
    Q_GADGET
    Q_PROPERTY(QString definition MEMBER definition)
    Q_PROPERTY(QVariantList examples MEMBER examples)

public:
    QString definition;
    QVariantList examples;  // QStrings
};

}  // namespace adapters::dtos

Q_DECLARE_METATYPE(adapters::dtos::DictionaryEntryDto)
Q_DECLARE_METATYPE(adapters::dtos::WordTypeDto)
Q_DECLARE_METATYPE(adapters::dtos::WordDefinitionDto)