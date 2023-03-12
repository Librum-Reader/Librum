#pragma once
#include <QString>
#include <rapidfuzz/fuzz.hpp>

namespace string_utils
{

double fuzzCompare(const QString& lhs,
                          const QString& rhs)
{
    // If rhs is a sub-string of lhs, return a high ratio
    auto substringPos = lhs.toLower().indexOf(rhs.toLower());
    if(substringPos != -1)
    {
        // The further at the front, the better the ratio should be
        double ratio = 100 - substringPos;
        // A difference in length of the strings should reduce the score
        ratio -= std::abs(lhs.length() - rhs.length()) * 0.1;
        
        return ratio;
    }
    
    return rapidfuzz::fuzz::ratio(rhs.toStdString(),
                                  lhs.toStdString());
}

bool lexicographicallyLess(const QString& left, const QString& right)
{
    if(left.isEmpty())
        return false;
    
    if(right.isEmpty())
        return true;
    
    return left.toLower() < right.toLower();
}

}