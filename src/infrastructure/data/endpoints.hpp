#pragma once
#include <QString>

namespace infrastructure::data
{

inline const QString domain { "https://localhost:7084" };

// Authentication
inline const QString authenticationEndpoint { domain + "/api/login" };
inline const QString registrationEndpoint { domain + "/api/register" };

// Book storage
inline const QString bookCreationEndpoint { domain + "/api/book" };
inline const QString bookUpdateEndpoint { domain + "/api/book" };
inline const QString bookDeletionEndpoint { domain + "/api/book" };
inline const QString booksMetadataGetEndpoint { domain + "/api/book" };
inline const QString uploadBookDataEndpoint { domain + "/api/book/bookData" };
inline const QString downloadBookDataEndpoint { domain + "/api/book/bookData" };
inline const QString changeBookCoverEndpoint { domain + "/api/book/cover" };
inline const QString deleteBookCoverEndpoint { domain + "/api/book/cover" };

// User storage
inline const QString userGetEndpoint { domain + "/api/user" };
inline const QString userPatchEndpoint { domain + "/api/user" };

// Tag storage
inline const QString tagUpdateEndpoint { domain + "/api/tag" };
inline const QString tagDeletionEndpoint { domain + "/api/tag" };


}  // namespace infrastructure::data