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
        return "The provided data is invalid";
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
        return "The email address is too short";
    case ErrorCode::EmailAddressTooLong:
        return "The email address is too long";
    case ErrorCode::PasswordTooShort:
        return "The password is too short";
    case ErrorCode::PasswordTooLong:
        return "The password is too long";
    case ErrorCode::FirstNameTooShort:
        return "First name is too short";
    case ErrorCode::FirstNameTooLong:
        return "First name is too long";
    case ErrorCode::LastNameTooShort:
        return "Last name is too short";
    case ErrorCode::LastNameTooLong:
        return "Last name is too long";
    case ErrorCode::UserDoesNotExist:
        return "No user with this email exists";
    case ErrorCode::AccountNotConfirmed:
        return "Your account is not confirmed";
    case ErrorCode::TooManyRequests:
        return "Too many requests, please try again later";
    case ErrorCode::AiExplanationLimitReached:
        return "AI explanation limit reached";
    case ErrorCode::AiRequestTooLong:
        return "AI request too long";
    case ErrorCode::UserHasNoRootFolder:
        return "User has no root folder";
    case ErrorCode::NameTooShort:
        return "Name is too short";
    case ErrorCode::NameTooLong:
        return "Name is too long";
    case ErrorCode::NoError:
        [[fallthrough]];
    default:
        qWarning() << "Failed converting invalid api error code";
        return "Internal client error";
    }
}

}  // namespace application::utility::error_code_converter