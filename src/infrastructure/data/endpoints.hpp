#pragma once
#include <QString>

namespace infrastructure::data
{
// clang-format off


inline const QString baseUrl { "https://localhost:7084" };

// Authentication
inline const QString authenticationEndpoint { baseUrl + "/api/login" };
inline const QString registrationEndpoint { baseUrl + "/api/register" };

// Book storage
inline const QString bookCreationEndpoint { baseUrl + "/api/book/create" };
inline const QString bookUpdateEndpoint { baseUrl + "/api/book" };
inline const QString bookDeletionEndpoint { baseUrl + "/api/book" };
inline const QString getBooksMetadataEndpoint { baseUrl + "/api/book/get" };

// User storage
inline const QString getUserEndpoint { baseUrl + "/api/user" };
inline const QString patchUserEndpoint { baseUrl + "/api/user" };


// clang-format on
}  // namespace infrastructure::data