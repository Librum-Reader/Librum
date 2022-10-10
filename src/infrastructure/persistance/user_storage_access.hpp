#pragma once
#include "i_user_storage_access.hpp"


namespace infrastructure::persistence
{

class UserStorageAccess : public adapters::IUserStorageAccess
{
    Q_OBJECT
    
public:
    UserStorageAccess();
};

} // namespace infrastructure::persistence