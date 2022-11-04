#include "BaseTypes.h"
#include "Class.h"
#include <format>

#define XSTRINGIFY(s) #s
#define STRINGIFY(s) XSTRINGIFY(s)

/* void is special. */
struct Void : public Type {
    using Type::Type;
    virtual std::string ToString(void const* instance) const noexcept
    {
        return std::format("void");
    }
};
static Void voidtype{ 0, "void" };
Type const* GetTypeImpl(TypeTag<void>) noexcept
{
    return &voidtype;
}

#define DEFINE_GET_TYPE(T, NAME, FMT) \
struct NAME : public Type { \
\
    using Type::Type;  \
    virtual std::string ToString(void const* instance) const noexcept \
    { \
        return std::format(FMT, *(T const*)instance); \
    } \
}; \
static NAME NAME##type{ sizeof(T), STRINGIFY(T),}; \
Type const* GetTypeImpl(TypeTag<T>) noexcept \
{ \
	return &NAME##type; \
}

DEFINE_GET_TYPE(bool, Bool, "%d")                             
DEFINE_GET_TYPE(char, Char, "%d")                             
DEFINE_GET_TYPE(short, Short, "%d")                           
DEFINE_GET_TYPE(int, Int, "%d")                               
DEFINE_GET_TYPE(long, Long, "%ld")                            
DEFINE_GET_TYPE(long long, LongLong, "%lld")                  
DEFINE_GET_TYPE(float, Float, "%.9g")                         
DEFINE_GET_TYPE(double, Double, "%.17g")                      
DEFINE_GET_TYPE(long double, LongDouble, "%.21Lg")            
DEFINE_GET_TYPE(unsigned char, UnsignedChar, "%u")            
DEFINE_GET_TYPE(unsigned short, UnsignedShort, "%u")          
DEFINE_GET_TYPE(unsigned int, UnsignedInt, "%u")              
DEFINE_GET_TYPE(unsigned long, UnsignedLong, "%lu")           
DEFINE_GET_TYPE(unsigned long long, UnsignedLongLong, "%llu")

#undef DEFINE_GET_TYPE