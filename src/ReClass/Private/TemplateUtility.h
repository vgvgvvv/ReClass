#pragma once


namespace ReClassSystem
{
    namespace Detail
    {
        template<int Condition, typename TIfTrue, typename TIfFalse>
        struct ChooseClass;

        template<typename TIfTrue, typename TIfFalse>
        struct ChooseClass<true, TIfTrue, TIfFalse>
        {
            using Result = TIfTrue;
        };

        template<typename TIfTrue, typename TIfFalse>
        struct ChooseClass<false, TIfTrue, TIfFalse>
        {
            using Result = TIfFalse;
        };
    }

    template<typename T1, typename T2>
    using IsSame = std::is_same<T1, T2>;

    template<typename TClass, typename TBaseClass>
    using IsDerivedFrom = std::is_base_of<TBaseClass, TClass>;

    template<bool TTest, class Ty = void>
    using EnableIfType = std::enable_if_t<TTest, Ty>;
}