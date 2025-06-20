#pragma once
#include <QString>

namespace infrastructure::data
{

inline const QString freeBooksDomain { "https://freebooks.librumreader.com" };

// Authentication
inline const QString loginEndpoint { "/authentication/login" };
inline const QString registerEndpoint { "/authentication/register" };
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
inline const QString aiTranslationEndpoint { "/Ai/translate" };

// Library storage
inline const QString bookCreateEndpoint { "/book" };
inline const QString bookUpdateEndpoint { "/book" };
inline const QString bookDeleteEndpoint { "/book" };
inline const QString getBooksMetadataEndpoint { "/book" };
inline const QString bookDataUploadEndpoint { "/book/bookData" };
inline const QString bookDataDownloadEndpoint { "/book/bookData" };
inline const QString bookCoverGetEndpoint { "/book/cover" };
inline const QString bookCoverChangeEndpoint { "/book/cover" };
inline const QString bookCoverDeleteEndpoint { "/book/cover" };
inline const QString usedBookStorageGetEndpoint { "/book/usedBookStorage" };

// Folder storage
inline const QString folderGetEndpoint { "/folder" };
inline const QString folderUpdateEndpoint { "/folder/update" };

// Free books storage
inline const QString getFreeBooksMetadataEndpoint { freeBooksDomain +
                                                    "/books" };

// User storage
inline const QString userGetEndpoint { "/user" };
inline const QString userUpdateEndpoint { "/user" };
inline const QString userDeleteEndpoint { "/user" };
inline const QString userProfilePictureEndpoint { "/user/profilePicture" };
inline const QString userPasswordForgotEndpoint { "/user/forgotPassword" };
inline const QString userPasswordChangeEndpoint { "/user" };

// Dictionary API
inline const QString dictionaryDefinitionEndpoint {
    "https://en.wiktionary.org/api/rest_v1/page/definition"
};

// Tag storage
inline const QString tagUpdateEndpoint { "/tag" };
inline const QString tagDeletionEndpoint { "/tag" };

}  // namespace infrastructure::data
