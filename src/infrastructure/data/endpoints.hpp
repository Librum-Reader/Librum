#pragma once
#include <QString>

namespace infrastructure::data
{

inline const QString freeBooksDomain { "https://freebooks.librumreader.com" };

// Authentication
inline const QString authenticationEndpoint { "/authentication/login" };
inline const QString registrationEndpoint { "/authentication/register" };
inline const QString checkIfEmailConfirmedEndpoint {
    "/authentication/checkIfEmailConfirmed"
};

// App Info
inline const QString latestAppVersionEndpoint { "/appinfo/latest-version" };

inline const QString binaryDownloadEndpoint {
    "https://librumstorageaccount.blob.core.windows.net/releases"
};

// AI endpoint
inline const QString aiCompletionEndpoint { "/Ai/complete" };

// Library storage
inline const QString bookCreationEndpoint { "/book" };
inline const QString bookUpdateEndpoint { "/book" };
inline const QString bookDeletionEndpoint { "/book" };
inline const QString booksMetadataGetEndpoint { "/book" };
inline const QString uploadBookDataEndpoint { "/book/bookData" };
inline const QString downloadBookDataEndpoint { "/book/bookData" };
inline const QString getBookCoverEndpoint { "/book/cover" };
inline const QString changeBookCoverEndpoint { "/book/cover" };
inline const QString deleteBookCoverEndpoint { "/book/cover" };
inline const QString getUsedBookStorageEndpoint { "/book/usedBookStorage" };

// Folder storage
inline const QString folderGetEndpoint { "/folder" };
inline const QString folderUpdateEndpoint { "/folder/update" };

// Free books storage
inline const QString getFreeBooksMetadataEndpoint { freeBooksDomain +
                                                    "/books" };

// User storage
inline const QString userGetEndpoint { "/user" };
inline const QString userPatchEndpoint { "/user" };
inline const QString userDeleteEndpoint { "/user" };
inline const QString userForgotPasswordEndpoint { "/user/forgotPassword" };
inline const QString userChangePasswordEndpoint { "/user" };
inline const QString userProfilePictureEndpoint { "/user/profilePicture" };

// Dictionary API
inline const QString dictionaryDefinitionEndpoint {
    "https://en.wiktionary.org/api/rest_v1/page/definition"
};

// Tag storage
inline const QString tagUpdateEndpoint { "/tag" };
inline const QString tagDeletionEndpoint { "/tag" };

}  // namespace infrastructure::data
