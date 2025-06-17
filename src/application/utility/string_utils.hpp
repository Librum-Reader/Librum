#pragma once
#include <QList>
#include <QString>
#include <rapidfuzz/fuzz.hpp>

namespace string_utils
{


// Private
namespace
{

// Return a high similarity if rhs is a substring of lhs, else return 0.0
inline double substringCompare(const QString& lhs, const QString& rhs)
{
    auto substringPos = lhs.toLower().indexOf(rhs.toLower());
    if(substringPos != -1)
    {
        // The further at the front, the better the ratio should be
        double ratio = 100 - substringPos;
        // A difference in length of the strings should reduce the score
        ratio -= std::abs(lhs.length() - rhs.length()) * 0.1;

        return ratio;
    }

    return 0.0;
}

}  // namespace

inline double similarity(const QString& str, const QString& aim,
                         rapidfuzz::fuzz::CachedRatio<unsigned int>* scorer)
{
    // First check if str is a substring of aim
    auto similarity = string_utils::substringCompare(str, aim);

    // If it isn't a substring, calculate the similarity
    if(qFuzzyCompare(similarity, 0))
        similarity = scorer->similarity(str.toUcs4());

    return similarity;
}

inline bool lexicographicallyLess(const QString& left, const QString& right)
{
    if(left.isEmpty())
        return false;

    if(right.isEmpty())
        return true;

    return left.toLower() < right.toLower();
}

// QString is utf-16 and needs to be utf-32 for rapidfuzz
inline std::vector<uint32_t> toUtf32(const QString& str)
{
    std::vector<uint32_t> result;
    result.reserve(str.size());

    for (QChar ch : str) {
        result.push_back(ch.unicode());
    }

    return result;
}

}  // namespace string_utils
