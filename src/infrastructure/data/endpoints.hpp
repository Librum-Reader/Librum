#pragma once
#include <QString>

namespace infrastructure::data
{

inline const QString domain { "https://librum-dev.azurewebsites.net" };

inline const QString freeBooksDomain { "https://gutendex.com" };

// Authentication
inline const QString authenticationEndpoint { domain + "/api/login" };
inline const QString registrationEndpoint { domain + "/api/register" };
inline const QString checkIfEmailConfirmedEndpoint {
    domain + "/api/checkIfEmailConfirmed"
};

// Book storage
inline const QString bookCreationEndpoint { domain + "/api/book" };
inline const QString bookUpdateEndpoint { domain + "/api/book" };
inline const QString bookDeletionEndpoint { domain + "/api/book" };
inline const QString booksMetadataGetEndpoint { domain + "/api/book" };
inline const QString uploadBookDataEndpoint { domain + "/api/book/bookData" };
inline const QString downloadBookDataEndpoint { domain + "/api/book/bookData" };
inline const QString getBookCoverEndpoint { domain + "/api/book/cover" };
inline const QString changeBookCoverEndpoint { domain + "/api/book/cover" };
inline const QString deleteBookCoverEndpoint { domain + "/api/book/cover" };
inline const QString getUsedBookStorageEndpoint { domain +
                                                  "/api/book/usedBookStorage" };

// Free books storage
inline const QString getFreeBooksMetadataEndpoint { freeBooksDomain +
                                                    "/books" };

// User storage
inline const QString userGetEndpoint { domain + "/api/user" };
inline const QString userPatchEndpoint { domain + "/api/user" };
inline const QString userProfilePictureEndpoint { domain +
                                                  "/api/user/profilePicture" };

// Tag storage
inline const QString tagUpdateEndpoint { domain + "/api/tag" };
inline const QString tagDeletionEndpoint { domain + "/api/tag" };

}  // namespace infrastructure::data
