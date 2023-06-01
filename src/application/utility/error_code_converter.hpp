#pragma once
#include <QDebug>
#include "error_code.hpp"

namespace application::utility::error_code_converter
{

QString getMessageForErrorCode(ErrorCode code)
{
    switch(code)
    {
    case ErrorCode::Unknown:
        return "An unknown error occured";
    case ErrorCode::EmailOrPasswordIsWrong:
        return "Your email or password is wrong";
    case ErrorCode::UserWithEmailAlreadyExists:
        return "A user with this email already exists";
    case ErrorCode::ProvidedDataIsInvalid:
        return "The provided data was invalid";
    case ErrorCode::NoBookWithThisIdWasFound:
        return "No book with this ID was found";
    case ErrorCode::InsufficientBookStorageSpace:
        return "Book storage space is insufficient";
    case ErrorCode::TagWithNameAlreadyExists:
        return "A tag with this name already exists";
    case ErrorCode::NoTagWithThisNameExists:
        return "No tag with this name exists";
    case ErrorCode::InvalidEmailAddressFormat:
        return "Invalid email address format";
    case ErrorCode::EmailAddressTooShort:
        return "The provided email address is too short";
    case ErrorCode::EmailAddressTooLong:
        return "The provided email address is too long";
    case ErrorCode::PasswordTooShort:
        return "The provided password is too short";
    case ErrorCode::PasswordTooLong:
        return "The provided password is too long";
    case ErrorCode::FirstNameTooShort:
        return "The provided first name is too short";
    case ErrorCode::FirstNameTooLong:
        return "The provided first name is too long";
    case ErrorCode::LastNameTooShort:
        return "The provided last name is too short";
    case ErrorCode::LastNameTooLong:
        return "The provided last name is too long";
    case ErrorCode::NoError:
        [[fallthrough]];
    default:
        qWarning() << "Failed converting invalid api error code";
        return "Internal client error";
    }
}

}  // namespace application::utility::error_code_converter