#include "ReTypes.h"
#include "ReClass.h"

#define XSTRINGIFY(s) #s
#define STRINGIFY(s) XSTRINGIFY(s)

namespace ReClassSystem
{
    /* void is special. */
    struct Void : public Type {
        using Type::Type;
        virtual std::string ToString(void const* instance) const noexcept
        {
            return "void";
        }
    };
    Type const* GetTypeImpl(TypeTag<void>) noexcept
    {
        static Void voidtype{ 0, "void" };
        return &voidtype;
    }

#define DEFINE_GET_TYPE(T, NAME) \
struct NAME : public Type { \
    using Type::Type;  \
    virtual std::string ToString(void const* instance) const noexcept \
    { \
        return std::to_string(*(T const*)instance); \
    } \
}; \
Type const* GetTypeImpl(TypeTag<T>) noexcept \
{ \
    static NAME NAME##type{ sizeof(T), STRINGIFY(T),}; \
    return &NAME##type; \
}

    DEFINE_GET_TYPE(bool, Bool)
    DEFINE_GET_TYPE(char, Char)
    DEFINE_GET_TYPE(short, Short)
    DEFINE_GET_TYPE(int, Int)
    DEFINE_GET_TYPE(long, Long)
    DEFINE_GET_TYPE(long long, LongLong)
    DEFINE_GET_TYPE(float, Float)
    DEFINE_GET_TYPE(double, Double)
    DEFINE_GET_TYPE(long double, LongDouble)
    DEFINE_GET_TYPE(unsigned char, UnsignedChar)
    DEFINE_GET_TYPE(unsigned short, UnsignedShort)
    DEFINE_GET_TYPE(unsigned int, UnsignedInt)
    DEFINE_GET_TYPE(unsigned long, UnsignedLong)
    DEFINE_GET_TYPE(unsigned long long, UnsignedLongLong)

#undef DEFINE_GET_TYPE

}


