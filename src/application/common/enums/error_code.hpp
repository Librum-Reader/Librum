#pragma once
#include <QObject>
#include "application_export.hpp"

// Each enum needs to be in a separate namespace due to a Qt bug reported at:
// https://bugreports.qt.io/browse/QTBUG-81792
//
// This causes not being able to register multiple enums to the Qt type system
// and to QML (Q_NAMESPACE and Q_ENUM_NS), if they are in the same namespace.
// The work around is to create a separate namespace for each enum.
namespace application::error_codes
{

Q_NAMESPACE_EXPORT(APPLICATION_EXPORT)

enum class APPLICATION_EXPORT ErrorCode
{
    NoError = -1,
    Unknown,  // 0
    EmailOrPasswordIsWrong,  // 1
    UserWithEmailAlreadyExists,  // 2
    ProvidedDataIsInvalid,  // 3
    NoBookWithThisIdWasFound,  // 4
    InsufficientBookStorageSpace,  // 5
    TagWithNameAlreadyExists,  // 6
    NoTagWithThisNameExists,  // 7
    InvalidEmailAddressFormat,  // 8
    EmailAddressTooShort,  // 9
    EmailAddressTooLong,  // 10
    PasswordTooShort,  // 11
    PasswordTooLong,  // 12
    FirstNameTooShort,  // 13
    FirstNameTooLong,  // 14
    LastNameTooShort,  // 15
    LastNameTooLong,  // 16
    UserDoesNotExist,  // 17
    AccountNotConfirmed,  // 18
    TooManyRequests,  // 19
    AiExplanationLimitReached,  // 20
    AiRequestTooLong,  // 21
    UserHasNoRootFolder,  // 22
    NameTooShort,  // 23
    NameTooLong,  // 24
    AutomaticLoginFailed,  // 25
};

Q_ENUM_NS(ErrorCode)

}  // namespace application::error_codes

// Because the enum shouldn't be in a separate namespace in the first place,
// make the namespace available to all of its users to avoid syntactic clutter.
using namespace application::error_codes;
