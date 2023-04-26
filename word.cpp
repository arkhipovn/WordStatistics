#include "word.h"

bool operator <(const Word &l, const Word &r)
{
    return l.count < r.count;
}

bool operator >(const Word &l, const Word &r)
{
    return r < l;
}

Word &Word::operator =(const Word &r)
{
    if(this == &r) {
        return *this;
    }
    text = r.text;
    count = r.count;
    return *this;
}
