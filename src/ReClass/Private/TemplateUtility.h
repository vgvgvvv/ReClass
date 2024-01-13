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

        template<typename T1, typename T2>
        using IsSame = std::is_same<T1, T2>;

    }
}