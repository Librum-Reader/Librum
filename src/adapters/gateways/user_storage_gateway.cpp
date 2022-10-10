#include "user_storage_gateway.hpp"


namespace adapters::gateways
{

UserStorageGateway::UserStorageGateway(IUserStorageAccess* userStorageAccess)
    : m_userStorageAccess(userStorageAccess)
{
}

} // namespace adapters::gateways