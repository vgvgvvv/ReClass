#pragma once
#include "ReClassPrefix.h"

#if RECLASS_FOR_UNREAL
#include "ReClass/Private/TypeSupport.Unreal.inl"
#endif // RECLASS_FOR_UNREAL
#include "ReClass/Private/TypeSupport.PureCpp.h"

namespace ReClassSystem
{
    /**
  * @brief If Object Is Valid (Support Raw And UObject)
  * @tparam TClassName Target Type
  * @param Obj Target Object
  * @return
  */
    template <typename TClassName>
    bool IsValidRawOrValidUObject(TClassName* Obj)
    {
        return Detail::BaseObjectWrapper<TClassName>::IsValidRawOrValidUObject(Obj);
    }

    /**
     * @brief Create New Instance , If UObject Return nullptr
     * @tparam TClassName TargetClass
     * @return
     */
    template <typename TClassName>
    TClassName* New()
    {
        return Detail::BaseObjectWrapper<TClassName>::New();
    }

    /**
     * \brief
     * \tparam TClassName TargetClass
     * \return ClassPtr
     */
    template <typename TClassName>
    const Class* GetReClassPtr()
    {
        return Detail::GetReClassWrapper<TClassName>::GetReClassPtr();
    }

    template <typename TClassName>
    bool IsReClass()
    {
        return Detail::GetReClassWrapper<TClassName>::IsReClass();
    }
}