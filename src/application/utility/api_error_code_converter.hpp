#pragma once
#include <QDebug>
#include "api_error_codes.hpp"

namespace application::utility::ApiErrorCodeConverter
{

QString getMessageForErrorCode(application::api_error_codes::ApiErrorCodes code)
{
    using api_error_codes::ApiErrorCodes;

    switch(code)
    {
    case ApiErrorCodes::Unknown:
        return "An unknown error occured";
    case ApiErrorCodes::EmailOrPasswordIsWrong:
        return "Your email or password is wrong";
    case ApiErrorCodes::UserWithEmailAlreadyExists:
        return "A user with this email already exists";
    case ApiErrorCodes::ProvidedDataIsInvalid:
        return "The provided data was invalid";
    case ApiErrorCodes::NoBookWithThisIdWasFound:
        return "No book with this ID was found";
    case ApiErrorCodes::InsufficientBookStorageSpace:
        return "Book storage space is insufficient";
    case ApiErrorCodes::TagWithNameAlreadyExists:
        return "A tag with this name already exists";
    case ApiErrorCodes::NoTagWithThisNameExists:
        return "No tag with this name exists";
    case ApiErrorCodes::InvalidEmailAddressFormat:
        return "Invalid email address format";
    case ApiErrorCodes::EmailAddressTooShort:
        return "The provided email address is too short";
    case ApiErrorCodes::EmailAddressTooLong:
        return "The provided email address is too long";
    case ApiErrorCodes::PasswordTooShort:
        return "The provided password is too short";
    case ApiErrorCodes::PasswordTooLong:
        return "The provided password is too long";
    case ApiErrorCodes::FirstNameTooShort:
        return "The provided first name is too short";
    case ApiErrorCodes::FirstNameTooLong:
        return "The provided first name is too long";
    case ApiErrorCodes::LastNameTooShort:
        return "The provided last name is too short";
    case ApiErrorCodes::LastNameTooLong:
        return "The provided last name is too long";
    default:
        qWarning() << "Failed converting invalid api error code";
        return "Internal client error";
    }
}

}  // namespace application::utility::ApiErrorCodeConverter