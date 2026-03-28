#pragma once

#include <utility>

template <class T>
class Interval
{
public:
    using value_type = T;

    const T& start() const { return mStart; }
    const T& end() const { return mEnd; }

    Interval& start(const T& v)
    {
        mStart = v;
        return validate();
    }

    Interval& end(const T& v)
    {
        mEnd = v;
        return validate();
    }

    T length() const { return mEnd - mStart; }
    bool proper() const { return mStart < mEnd; }

private:
    Interval& validate()
    {
        if (mStart > mEnd)
        {
            std::swap(mStart, mEnd);
        }
        return *this;
    }

    T mStart = T(0);
    T mEnd = T(0);
};
