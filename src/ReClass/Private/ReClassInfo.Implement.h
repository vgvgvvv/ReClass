#pragma once

#define RECLASS_SET_DEFAULT_CLASS_CTOR_IMPLEMENT(CLASS_NAME) \
    self->SetCtorAndDest([](){ return ReClassSystem::New<CLASS_NAME>(); }, [](void* Self){ delete static_cast<CLASS_NAME*>(Self); });

#define RECLASS_SET_CLASS_CTOR_DEST_IMPLEMENT(CTOR_FUNC, DEST_FUNC) \
    self->SetCtorAndDest(CTOR_FUNC, DEST_FUNC);

#define RECLASS_CLASS_IMPLEMENT_IMPLEMENT(CLASS_NAME) \
    self->AppendInterface(ToInternalClassInfo<SelfType, CLASS_NAME>());

#if RECLASS_SUPPORT_DYNAMIC_CLASS
#define RECLASS_DEFINE_CLASS_DYNAMIC_IMPLEMENT(CLASS_NAME)							\
    private: \
        void ____STATIC_ASSERT_TYPE_##CLASS_NAME() \
        { \
            static_assert(ReClassSystem::IsSame<decltype(this), CLASS_NAME*>::value, "Type " #CLASS_NAME " must same with self class"); \
        } \
    public:																\
        typedef CLASS_NAME SelfClass;										\
        typedef void SuperClass;											\
        static const ReClassSystem::Class& RECLASS_STATIC_CLASS_FUNCNAME() { return GetSelfClass(); }		\
        virtual const ReClassSystem::Class& RECLASS_GET_CLASS_FUNCNAME() const { return CLASS_NAME::RECLASS_STATIC_CLASS_FUNCNAME(); }	\
        private: \
        static ReClassSystem::Class& GetSelfClass() \
        { \
            static ReClassSystem::Class selfClass(sizeof(CLASS_NAME), \
                []() { return ReClassSystem::Class::NullInternalClassInfo; },														\
                #CLASS_NAME, \
                true, \
                ReClassSystem::EClassFlag::None, \
                [](ReClassSystem::Class* self){	 \
                    RECLASS_SET_DEFAULT_CLASS_CTOR_IMPLEMENT(CLASS_NAME) \
                });	 \
            return selfClass; \
        }
#else
    #define RECLASS_DEFINE_CLASS_DYNAMIC_IMPLEMENT(...)
#endif

#pragma region DefineClass

#define RECLASS_DECLARE_CLASS_IMPLEMENT(CLASS_NAME, ...) \
    private: \
        void ____STATIC_ASSERT_TYPE_##CLASS_NAME() \
        { \
            static_assert(ReClassSystem::IsSame<decltype(this), CLASS_NAME*>::value, "Type " #CLASS_NAME " must same with self class"); \
        } \
    public:\
        typedef CLASS_NAME SelfClass; \
        typedef void SuperClass; \
        static const ReClassSystem::Class& RECLASS_STATIC_CLASS_FUNCNAME() { return CLASS_NAME##selfClass; } \
        virtual const ReClassSystem::Class& RECLASS_GET_CLASS_FUNCNAME() const { return CLASS_NAME::RECLASS_STATIC_CLASS_FUNCNAME(); } \
        private:\
        __VA_ARGS__ static ReClassSystem::Class CLASS_NAME##selfClass;

#define RECLASS_DEFINE_CLASS_BEGIN_IMPLEMENT(CLASS_NAME, flag) \
    ReClassSystem::Class CLASS_NAME::CLASS_NAME##selfClass( \
        sizeof(CLASS_NAME), \
        [](){ return ReClassSystem::Class::NullInternalClassInfo; }, \
        #CLASS_NAME, \
        false, \
        flag, \
        [](ReClassSystem::Class* self){\
            using SelfType = CLASS_NAME;

#define RECLASS_DEFINE_CLASS_END_IMPLEMENT() \
    });

#define RECLASS_DEFINE_CLASS_IMPLEMENT(CLASS_NAME) \
    RECLASS_DEFINE_CLASS_BEGIN_IMPLEMENT(CLASS_NAME, ReClassSystem::EClassFlag::None)\
    RECLASS_SET_DEFAULT_CLASS_CTOR_IMPLEMENT(CLASS_NAME);\
    RECLASS_DEFINE_CLASS_END_IMPLEMENT()

#define RECLASS_DEFINE_CLASS_WITHOUT_NEW_IMPLEMENT(CLASS_NAME) \
    RECLASS_DEFINE_CLASS_BEGIN_IMPLEMENT(CLASS_NAME, ReClassSystem::EClassFlag::None)\
    RECLASS_DEFINE_CLASS_END_IMPLEMENT()

#define RECLASS_DEFINE_CLASS_IMP_WITH_FLAG_IMPLEMENT(CLASS_NAME, flag) \
    RECLASS_DEFINE_CLASS_BEGIN_IMPLEMENT(CLASS_NAME, flag)\
    RECLASS_SET_DEFAULT_CLASS_CTOR_IMPLEMENT(CLASS_NAME);\
    RECLASS_DEFINE_CLASS_END_IMPLEMENT()

#pragma endregion // DefineClass

#pragma region Register Class
#define RECLASS_REGISTER_CLASS_BEGIN_IMPLEMENT(CLASS_NAME, flag) \
    static ReClassSystem::Class CLASS_NAME##Register( \
    sizeof(CLASS_NAME), \
    [](){ return ReClassSystem::Class::NullInternalClassInfo; }, \
    #CLASS_NAME, \
    false, \
    flag, \
    [](ReClassSystem::Class* self){\
    using SelfType = CLASS_NAME;

#define RECLASS_REGISTER_CLASS_END_IMPLEMENT() \
    });

#define RECLASS_REGISTER_CLASS_IMPLEMENT(CLASS_NAME) \
    RECLASS_REGISTER_CLASS_BEGIN_IMPLEMENT(CLASS_NAME, ReClassSystem::EClassFlag::None) \
    RECLASS_SET_DEFAULT_CLASS_CTOR_IMPLEMENT(CLASS_NAME);\
    RECLASS_REGISTER_CLASS_END_IMPLEMENT()

#define RECLASS_REGISTER_CLASS_WITHOUT_NEW_IMPLEMENT(CLASS_NAME)\
    RECLASS_REGISTER_CLASS_BEGIN_IMPLEMENT(CLASS_NAME, ReClassSystem::EClassFlag::None) \
    RECLASS_REGISTER_CLASS_END_IMPLEMENT()


#define RECLASS_REGISTER_CLASS_WITH_FLAG_IMPLEMENT(CLASS_NAME, flag) \
    RECLASS_REGISTER_CLASS_BEGIN_IMPLEMENT(CLASS_NAME, flag) \
    RECLASS_SET_DEFAULT_CLASS_CTOR_IMPLEMENT(CLASS_NAME);\
    RECLASS_REGISTER_CLASS_END_IMPLEMENT()

#pragma endregion // Register Class


#pragma region Define Driven Class
#if RECLASS_SUPPORT_DYNAMIC_CLASS
#define RECLASS_DEFINE_DERIVED_CLASS_DYNAMIC_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME) \
private: \
    void ____STATIC_ASSERT_TYPE_##CLASS_NAME() \
    { \
    static_assert(ReClassSystem::IsSame<decltype(this), CLASS_NAME*>::value, "Type " #CLASS_NAME " must same with self class"); \
    } \
public:	\
    typedef CLASS_NAME SelfClass; \
    typedef BASE_CLASS_NAME SuperClass; \
    static const ReClassSystem::Class& RECLASS_STATIC_CLASS_FUNCNAME() { return GetSelfClass(); } \
    virtual const ReClassSystem::Class& RECLASS_GET_CLASS_FUNCNAME() const override { return CLASS_NAME::RECLASS_STATIC_CLASS_FUNCNAME(); }	\
    private:																	\
    static ReClassSystem::Class& GetSelfClass() \
    { \
        static_assert(ReClassSystem::IsDerivedFrom<CLASS_NAME, BASE_CLASS_NAME>::value, "Type " #CLASS_NAME " must drive from " #BASE_CLASS_NAME); \
        static ReClassSystem::Class selfClass( \
            sizeof(CLASS_NAME),													\
            []() { return ReClassSystem::Class::ToInternalClassInfo<CLASS_NAME, BASE_CLASS_NAME>(); }, \
            #CLASS_NAME, \
            true, \
            ReClassSystem::EClassFlag::None, \
            [](ReClassSystem::Class* self){	\
                RECLASS_SET_DEFAULT_CLASS_CTOR_IMPLEMENT(CLASS_NAME) \
            }); \
        return selfClass; \
    }
#else
    #define RECLASS_DEFINE_DERIVED_CLASS_DYNAMIC_IMPLEMENT(...)
#endif


#define RECLASS_DECLARE_DERIVED_CLASS_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME, ...) \
    private: \
        void ____STATIC_ASSERT_TYPE_##CLASS_NAME() \
        { \
            static_assert(ReClassSystem::IsSame<decltype(this), CLASS_NAME*>::value, "Type " #CLASS_NAME " must same with self class"); \
        } \
    public:\
        typedef CLASS_NAME SelfClass; \
        typedef BASE_CLASS_NAME SuperClass; \
        static const ReClassSystem::Class& RECLASS_STATIC_CLASS_FUNCNAME() { return CLASS_NAME##selfClass; } \
        virtual const ReClassSystem::Class& RECLASS_GET_CLASS_FUNCNAME() const override  { return CLASS_NAME::RECLASS_STATIC_CLASS_FUNCNAME(); }	\
    private:\
        __VA_ARGS__ static ReClassSystem::Class CLASS_NAME##selfClass;

#define RECLASS_DEFINE_DERIVED_CLASS_BEGIN_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME, FLAG) \
    static_assert(ReClassSystem::IsDerivedFrom<CLASS_NAME, BASE_CLASS_NAME>::value, "type " #CLASS_NAME " must derived from " #BASE_CLASS_NAME); \
    ReClassSystem::Class CLASS_NAME::CLASS_NAME##selfClass(sizeof(CLASS_NAME), \
    [](){ return ReClassSystem::Class::ToInternalClassInfo<CLASS_NAME, BASE_CLASS_NAME>(); }, \
    #CLASS_NAME, \
    false, \
    FLAG, \
    [](ReClassSystem::Class* self){\
        using SelfType = CLASS_NAME;


#define RECLASS_DEFINE_DERIVED_CLASS_END_IMPLEMENT() \
    });

#define RECLASS_DEFINE_DERIVED_CLASS_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME) \
    RECLASS_DEFINE_DERIVED_CLASS_BEGIN_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME, ReClassSystem::EClassFlag::None)\
    RECLASS_SET_DEFAULT_CLASS_CTOR_IMPLEMENT(CLASS_NAME);\
    RECLASS_DEFINE_DERIVED_CLASS_END_IMPLEMENT()

#define RECLASS_DEFINE_DERIVED_CLASS_WITHOUT_NEW_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME) \
    RECLASS_DEFINE_DERIVED_CLASS_BEGIN_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME, ReClassSystem::EClassFlag::None)\
    RECLASS_DEFINE_DERIVED_CLASS_END_IMPLEMENT()

#define RECLASS_DEFINE_DERIVED_CLASS_WITH_FLAG_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME, FLAG) \
    RECLASS_DEFINE_DERIVED_CLASS_BEGIN_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME, FLAG)\
    RECLASS_SET_DEFAULT_CLASS_CTOR_IMPLEMENT(CLASS_NAME);\
    RECLASS_DEFINE_DERIVED_CLASS_END_IMPLEMENT()


#pragma endregion

#pragma region Register Class
#define RECLASS_REGISTER_DERIVED_CLASS_BEGIN_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME, FLAG) \
    static ReClassSystem::Class CLASS_NAME##Register( \
    sizeof(CLASS_NAME), \
    [](){ return ReClassSystem::Class::ToInternalClassInfo(CLASS_NAME, BASE_CLASS_NAME); }, \
    #CLASS_NAME, \
    false, \
    FLAG, \
    [](ReClassSystem::Class* self){\
    using SelfType = CLASS_NAME;

#define RECLASS_REGISTER_DERIVED_CLASS_END_IMPLEMENT() \
    });

#define RECLASS_REGISTER_DERIVED_CLASS_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME) \
    RECLASS_REGISTER_DERIVED_CLASS_BEGIN_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME, ReClassSystem::EClassFlag::None) \
    RECLASS_SET_DEFAULT_CLASS_CTOR_IMPLEMENT(CLASS_NAME);\
    RECLASS_REGISTER_DERIVED_CLASS_END_IMPLEMENT()

#define RECLASS_REGISTER_DERIVED_CLASS_WITHOUT_NEW_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME) \
    RECLASS_REGISTER_DERIVED_CLASS_BEGIN_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME, ReClassSystem::EClassFlag::None) \
    RECLASS_REGISTER_DERIVED_CLASS_END_IMPLEMENT()

#define RECLASS_REGISTER_DERIVED_CLASS_WITH_FLAG_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME, FLAG) \
    RECLASS_REGISTER_DERIVED_CLASS_BEGIN_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME, FLAG) \
    RECLASS_SET_DEFAULT_CLASS_CTOR_IMPLEMENT(CLASS_NAME); \
    RECLASS_REGISTER_DERIVED_CLASS_END_IMPLEMENT()

#pragma endregion // Register Class

#pragma region Define Interface

#define RECLASS_DECLARE_INTERFACE_IMPLEMENT(CLASS_NAME, ...) \
    RECLASS_DECLARE_CLASS_IMPLEMENT(CLASS_NAME, __VA_ARGS__)

#define RECLASS_DEFINE_INTERFACE_IMPLEMENT(CLASS_NAME) \
    RECLASS_DEFINE_CLASS_WITHOUT_NEW_IMPLEMENT(CLASS_NAME)

#define RECLASS_DEFINE_INTERFACE_BEGIN_IMPLEMENT(CLASS_NAME) \
    RECLASS_DEFINE_CLASS_BEGIN_IMPLEMENT(CLASS_NAME)

#define RECLASS_DEFINE_INTERFACE_END_IMPLEMENT(CLASS_NAME) \
    RECLASS_DEFINE_CLASS_END_IMPLEMENT(CLASS_NAME)

#define RECLASS_DECLARE_DERIVED_INTERFACE_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME, ...) \
    RECLASS_DEFINE_DERIVED_CLASS_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME, __VA_ARGS__)

#define RECLASS_DEFINE_DERIVED_INTERFACE_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME) \
    RECLASS_DEFINE_DERIVED_CLASS_WITHOUT_NEW_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME)

#define RECLASS_DEFINE_DERIVED_INTERFACE_BEGIN_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME) \
    RECLASS_DEFINE_DERIVED_CLASS_BEGIN_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME)

#define RECLASS_DEFINE_DERIVED_INTERFACE_END_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME) \
    RECLASS_DEFINE_DERIVED_CLASS_END_IMPLEMENT(CLASS_NAME, BASE_CLASS_NAME)


#pragma endregion // Define Interface