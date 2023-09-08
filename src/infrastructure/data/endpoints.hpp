#pragma once
#include <QString>

namespace infrastructure::data
{

inline const QString domain { "https://api.librumreader.com" };

inline const QString freeBooksDomain { "https://gutendex.com" };

// Authentication
inline const QString authenticationEndpoint { domain +
                                              "/authentication/login" };
inline const QString registrationEndpoint { domain +
                                            "/authentication/register" };
inline const QString checkIfEmailConfirmedEndpoint {
    domain + "/authentication/checkIfEmailConfirmed"
};

// App Info
inline const QString latestAppVersionEndpoint { domain +
                                                "/appinfo/latest-version" };
inline const QString binaryDownloadEndpoint {
    "https://librumstorageaccount.blob.core.windows.net/releases"
};

// Book storage
inline const QString bookCreationEndpoint { domain + "/book" };
inline const QString bookUpdateEndpoint { domain + "/book" };
inline const QString bookDeletionEndpoint { domain + "/book" };
inline const QString booksMetadataGetEndpoint { domain + "/book" };
inline const QString uploadBookDataEndpoint { domain + "/book/bookData" };
inline const QString downloadBookDataEndpoint { domain + "/book/bookData" };
inline const QString getBookCoverEndpoint { domain + "/book/cover" };
inline const QString changeBookCoverEndpoint { domain + "/book/cover" };
inline const QString deleteBookCoverEndpoint { domain + "/book/cover" };
inline const QString getUsedBookStorageEndpoint { domain +
                                                  "/book/usedBookStorage" };

// Free books storage
inline const QString getFreeBooksMetadataEndpoint { freeBooksDomain +
                                                    "/books" };

// User storage
inline const QString userGetEndpoint { domain + "/user" };
inline const QString userPatchEndpoint { domain + "/user" };
inline const QString userDeleteEndpoint { domain + "/user" };
inline const QString userForgotPasswordEndpoint { domain +
                                                  "/user/forgotPassword" };
inline const QString userChangePasswordEndpoint { domain + "/user" };
inline const QString userProfilePictureEndpoint { domain +
                                                  "/user/profilePicture" };

// Tag storage
inline const QString tagUpdateEndpoint { domain + "/tag" };
inline const QString tagDeletionEndpoint { domain + "/tag" };

}  // namespace infrastructure::data
