#ifndef GCC4_HASHED_STRING_H_INCLUDED
#define GCC4_HASHED_STRING_H_INCLUDED

#include <string>

class HashedString
{
public:

    explicit HashedString(char const* const pIdentString) :
        m_Ident(hash_name(pIdentString)),
        m_IdentStr(pIdentString)
    {}

    unsigned long GetHashValue() const {
        return reinterpret_cast<unsigned long>(m_Ident);
    }
    const std::string& GetStr() const {
        return m_IdentStr;
    }

    static void* hash_name(char const* pIdentStr);

    bool operator<(HashedString const& o) const {
        bool r = (GetHashValue() < o.GetHashValue());
        return r;
    }
    bool operator==(HashedString const& o) const {
        bool r = (GetHashValue() == o.GetHashValue());
        return r;
    }

private:
    void* m_Ident;
    std::string m_IdentStr;
};

#endif // GCC4_HASHED_STRING_H_INCLUDED
