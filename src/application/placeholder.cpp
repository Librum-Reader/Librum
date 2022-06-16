#include "placeholder.hpp"

namespace application
{

Placeholder::Placeholder()
{
}

void Placeholder::doSomething(int x)
{
    x = 3;
    
    for(int i { 0 }; i < x; ++i)
        ;
}

}