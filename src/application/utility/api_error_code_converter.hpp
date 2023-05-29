#pragma once

namespace application::utility::ApiErrorCodeConverter
{

QString getMessageForErrorCode(int code)
{
    switch(code)
    {
    case 1:
        return "Your email or password was wrong";
    case 2:
        return "A user with this email already exists";
    case 3:
        return "The provided data was invalid";
    case 4:
        return "No book with this ID was found";
    case 5:
        return "Book storage space is insufficient";
    case 6:
        return "A tag with this name already exists";
    case 7:
        return "No tag with this name exists";
    default:
        return "An unknown error occured";
    }
}

}  // namespace application::utility::ApiErrorCodeConverter