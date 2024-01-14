#pragma once
#include "ReClassPrefix.h"

namespace ReClassSystem
{
    namespace Detail
    {
        template <typename TClassName, typename = void>
        struct BaseObjectWrapper
        {
            static bool IsValidRawOrValidUObject(TClassName* Obj)
            {
                return Obj != nullptr;
            }

            static TClassName* New()
            {
                return new TClassName;
            }
        };
    }

    namespace Detail
    {
        // SFINAE test
        template <typename T>
        class HasReClass
        {
            typedef char one;
            struct two { char dummy[2]; };

            template <typename C>
            static one test( decltype(&C::RECLASS_STATIC_CLASS_FUNCNAME) ) ;
            template <typename C>
            static two test(...);

        public:
            enum { Value = sizeof(test<T>(0)) == sizeof(char) };
        };

        template <typename T, typename = void>
        struct GetReClassWrapper
        {
            static const Class* GetReClassPtr()
            {
                return nullptr;
            }

            static bool IsReClass()
            {
                return false;
            }

        };

        template <typename T>
        struct GetReClassWrapper<T, EnableIfType<HasReClass<T>::Value>>
        {
            static const Class* GetReClassPtr()
            {
                return &T::RECLASS_STATIC_CLASS_FUNCNAME();
            }

            static bool IsReClass()
            {
                return true;
            }
        };
    }


}