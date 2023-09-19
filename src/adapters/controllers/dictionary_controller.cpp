#include "dictionary_controller.hpp"

namespace adapters::controllers
{

DictionaryController::DictionaryController(
    application::IDictionaryService* userService) :
    m_userService(userService)
{
}

}  // namespace adapters::controllers