#pragma once


namespace application
{

    enum class BookOperationStatus
    {
        Success,
        BookDoesNotExist,
        BookAlreadyExists,
        PropertyDoesNotExist,
        TagDoesNotExist,
        TagAlreadyExists
    };

} // namespace application