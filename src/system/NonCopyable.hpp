#ifndef NONCOPYABLE_HPP
#define NONCOPYABLE_HPP

class NonCopyable
{
protected:
    NonCopyable() {}
    ~NonCopyable() {}

    NonCopyable(const NonCopyable&) = delete;
    NonCopyable(const NonCopyable&&) = delete;
    NonCopyable& operator =(const NonCopyable&) = delete;
    NonCopyable& operator =(const NonCopyable&&) = delete;
};

#endif // !NONCOPYABLE_HPP