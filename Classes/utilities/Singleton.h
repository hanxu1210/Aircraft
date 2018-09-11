#ifndef SINGLETON_H__INCLUDED
#define SINGLETON_H__INCLUDED


//------------------------------------------------------------------------
//
// Declare a dynamically allocated singleton.
//
// Singleton class implement their constructor and destructor
// has private members and are defined as not copiable.
//
// The singleton is mockable!
//
// There's 3 functions to manage the singleton:
// - the creator    CreateSingleton()
// - the accessor   Singleton()
// - the destroyer  DestroySingleton()
//
// .h:
//      class MyClass
//      {
//      public:
//          DECLARE_SINGLETON(MyClass);
//
//      public:
//          void Method()
//          {
//              m_n++;
//          }
//
//      private:
//          s32 m_n;
//      };
//
//
//      class MyClassParam
//      {
//      public:
//          DECLARE_SINGLETON_PARAM(MyClass, (Object * obj, s32 n), (obj, n));
//
//      public:
//          void Method()
//          {
//              m_n++;
//          }
//
//      private:
//          Object *    m_obj;
//          s32   m_n;
//      };
//
//
// .cpp:
//      IMPLEMENT_SINGLETON(MyClass);
//
//      MyClass::MyClass() :
//          m_n(0)
//      {
//      };
//
//      MyClass::~MyClass()
//      {
//      };
//
//
//      IMPLEMENT_SINGLETON(MyClassParam);
//
//      MyClassParam::MyClassParam(Object * obj, s32 n) :
//          m_obj(obj),
//          m_n(n)
//      {
//      };
//
//      MyClassParam::~MyClassParam()
//      {
//      };
//
//
// Usage:
//
//      void main()
//      {
//          MyClass::CreateSingleton();
//          ...
//          MyClass::Singleton().Method();
//          ...
//          MyClass::DestroySingleton();
//
//
//          MyClassParam::CreateSingleton(NULL, 2);
//          ...
//          MyClassParam::Singleton().Method();
//          ...
//          MyClassParam::DestroySingleton();
//      }
//
//------------------------------------------------------------------------

#define NOT_COPIABLE(Class)   \
        private:                           \
        Class(const Class&);           \
        Class& operator=(const Class&)

#define DECLARE_SINGLETON( ClassName )\
    DECLARE_SINGLETON_PARAM(ClassName, (), () )

#ifdef TARGET_FINAL
#define DECLARE_SINGLETON_PARAM( ClassName, CreatorSignature, ConstructorVariable )\
\
    public:\
        static bool IsSingletonCreated() {return ms_singletonInstance != NULL;}\
        DECLARE_SINGLETON_CREATOR(ClassName, CreatorSignature, ConstructorVariable )\
        DECLARE_SINGLETON_QUERY(ClassName)\
        DECLARE_SINGLETON_DESTROYER(ClassName)\
\
        static bool IsMocked() {return false;}\
\
    private:\
        ClassName CreatorSignature;\
        ~ClassName ();\
\
    private:\
        static ClassName * ms_singletonInstance;\
        NOT_COPIABLE(ClassName)
#else
#define DECLARE_SINGLETON_PARAM( ClassName, CreatorSignature, ConstructorVariable )\
\
    public:\
        static bool IsSingletonCreated() {return ms_singletonInstance != NULL;}\
        DECLARE_SINGLETON_CREATOR(ClassName, CreatorSignature, ConstructorVariable )\
        DECLARE_SINGLETON_QUERY(ClassName)\
        DECLARE_SINGLETON_DESTROYER(ClassName)\
\
        static bool IsMocked() {return ms_singletonMocked;}\
        DECLARE_SINGLETON_CREATOR_FROM_MOCK(ClassName)\
        DECLARE_SINGLETON_INVALIDATOR_FOR_MOCK(ClassName)\
\
    protected:\
        ClassName CreatorSignature;\
        virtual ~ClassName ();\
\
        typedef ClassName SingletonType;\
\
    private:\
        static ClassName * ms_singletonInstance;\
        static bool ms_singletonMocked;\
        NOT_COPIABLE(ClassName)
#endif


//------------------------------------------------------------------------
//
// Declare a statically allocated singleton.
//
// It acts exactly like both macros DECLARE_SINGLETON and 
// DECLARE_SINGLETON_PARAM but use static allocation instead
// of dynamic allocation and so, there's no destroyer function.
//
// The static singleton is also mockable!
// 
//------------------------------------------------------------------------

#define DECLARE_STATIC_SINGLETON( ClassName )\
    DECLARE_STATIC_SINGLETON_PARAM(ClassName, (), ())

#ifdef TARGET_FINAL
#define DECLARE_STATIC_SINGLETON_PARAM( ClassName, CreatorSignature, ConstructorVariable )\
\
    public:\
        static bool IsSingletonCreated() {return ms_singletonInstance != NULL;}\
        DECLARE_STATIC_SINGLETON_CREATOR(ClassName, CreatorSignature, ConstructorVariable)\
        DECLARE_SINGLETON_QUERY(ClassName)\
\
        static bool IsMocked() {return false;}\
    private:\
        ClassName CreatorSignature;\
        ~ClassName ();\
\
    private:\
        static ClassName * ms_singletonInstance;\
        NOT_COPIABLE(ClassName)
#else
#define DECLARE_STATIC_SINGLETON_PARAM( ClassName, CreatorSignature, ConstructorVariable )\
\
    public:\
        static bool IsSingletonCreated() {return ms_singletonInstance != NULL;}\
        DECLARE_STATIC_SINGLETON_CREATOR(ClassName, CreatorSignature, ConstructorVariable)\
        DECLARE_SINGLETON_QUERY(ClassName)\
\
        static bool IsMocked() {return ms_singletonMocked;}\
        DECLARE_SINGLETON_CREATOR_FROM_MOCK(ClassName)\
        DECLARE_SINGLETON_INVALIDATOR_FOR_MOCK(ClassName)\
\
    protected:\
        ClassName CreatorSignature;\
        mockable ~ClassName (); \
\
        typedef ClassName SingletonType;\
\
    private:\
        static ClassName * ms_singletonInstance;\
        static bool ms_singletonMocked;\
        NOT_COPIABLE(ClassName)
#endif


//------------------------------------------------------------------------
// 
// Implement a singleton.
// 
//------------------------------------------------------------------------
#ifdef TARGET_FINAL
#define IMPLEMENT_SINGLETON( ClassName )\
    ClassName * ClassName::ms_singletonInstance = NULL
#else
#define IMPLEMENT_SINGLETON( ClassName )\
    ClassName * ClassName::ms_singletonInstance = NULL;\
    bool ClassName::ms_singletonMocked = false
#endif


//------------------------------------------------------------------------
// 
// Declare the creator method for a dynamically allocated singleton.
// 
//------------------------------------------------------------------------
#define DECLARE_SINGLETON_CREATOR( ClassName, CreatorSignature, ConstructorVariable )\
\
    static void CreateSingleton CreatorSignature\
    {\
        XAssert(ms_singletonInstance == NULL, "The %s singleton is already created", #ClassName);\
\
        ms_singletonInstance = new ClassName;\
    }


//------------------------------------------------------------------------
// 
// Declare the creator method for a statically allocated singleton.
// 
//------------------------------------------------------------------------
#define DECLARE_STATIC_SINGLETON_CREATOR( ClassName, CreatorSignature, ConstructorVariable )\
\
    static void CreateSingleton CreatorSignature\
    {\
        XAssert(ms_singletonInstance == NULL, "The %s singleton is already created", #ClassName);\
\
        static ClassName stub = ClassName ConstructorVariable;\
\
        ms_singletonInstance = &stub;\
    }


//------------------------------------------------------------------------
// 
// Declare the query method to access the singleton.
// 
//------------------------------------------------------------------------
#define DECLARE_SINGLETON_QUERY( ClassName )\
\
    static ClassName * SingletonPtr()\
    {\
        return ms_singletonInstance;\
    }\
    static ClassName & Singleton()\
    {\
        XAssert(ms_singletonInstance, "The %s singleton is not created", #ClassName);\
        return *ms_singletonInstance;\
    }\
    static const ClassName * ConstSingletonPtr()\
    {\
        return SingletonPtr();\
    }\
    static const ClassName & ConstSingleton()\
    {\
        return Singleton();\
    }


//------------------------------------------------------------------------
// 
// Declare the destroyer method of the singleton.
// 
//------------------------------------------------------------------------
#define DECLARE_SINGLETON_DESTROYER( ClassName )\
\
    static void DestroySingleton()\
    {\
        if (!ms_singletonInstance) \
        {  \
            return; \
        } \
        \
        delete ms_singletonInstance;\
        ms_singletonInstance = NULL;\
    }


//------------------------------------------------------------------------
// 
// Declare the creator method to mock the singleton.
// 
//------------------------------------------------------------------------
#define DECLARE_SINGLETON_CREATOR_FROM_MOCK( ClassName )\
\
    static void CreateSingletonFromMock( ClassName& mock )\
    {\
        XAssert(ms_singletonInstance == NULL, "The %s singleton is already created", #ClassName);\
\
        ms_singletonInstance = &mock;\
        ms_singletonMocked = true;\
    }\
\
    static ClassName* ReplaceSingletonWithMock( ClassName& mock )\
    {\
        XAssert(ms_singletonInstance != NULL, "The %s singleton is not created", #ClassName);\
\
        ClassName* previousInstance = ms_singletonInstance;\
\
        ms_singletonInstance = &mock;\
        ms_singletonMocked = true;\
\
        return previousInstance;\
    }


//------------------------------------------------------------------------
// 
// Declare the invalidator method for a mocked singleton.
// 
//------------------------------------------------------------------------
#define DECLARE_SINGLETON_INVALIDATOR_FOR_MOCK( ClassName )\
\
    static void InvalidateMockedSingleton()\
    {\
        XAssert(ms_singletonInstance, "The %s singleton instance is invalid.", #ClassName);\
        XAssert(ms_singletonMocked, "The %s singleton is not mocked.", #ClassName);\
\
        ms_singletonInstance = NULL;\
        ms_singletonMocked = false;\
    }\
\
    static void RestoreMockedSingleton( ClassName* instance )\
    {\
    XAssert(instance, "The %s singleton instance is invalid.", #ClassName );\
\
        ms_singletonInstance = instance;\
        ms_singletonMocked = false;\
    }


//------------------------------------------------------------------------
// 
// Automatically creates the mocked singleton at construction and
// invalidates it at desctruction.
// 
//------------------------------------------------------------------------
#define SINGLETON_SCOPED_MOCK( ClassName )\
\
    ClassName()  { ClassName::CreateSingletonFromMock(*this); }\
    ~ClassName() { ClassName::InvalidateMockedSingleton(); }


#endif
