// FIXME: This should be multi-include protected and allow undefining things

#define CACHE_VALID(PROPERTY)    this->template isCacheValid<PROPERTY>()
#define VALIDATE_CACHE(PROPERTY) this->template validateCache<PROPERTY>()

#define WRITE_CACHE(PROPERTY, CACHE, VALUE)                                 \
    CACHE = VALUE;                                                          \
    VALIDATE_CACHE(PROPERTY);

#define SAME_AS_CACHE(PROPERTY, CACHE, VALUE)                               \
    (CACHE_VALID(PROPERTY) && CACHE == VALUE)

#define SYNC_CACHE(PROPERTY, TYPE, CACHE)                                   \
    if (! CACHE_VALID(PROPERTY)) {                                          \
        API::template getHardwareState<PROPERTY>(CACHE);                    \
        VALIDATE_CACHE(PROPERTY);                                           \
    }

#define SYNC_CACHE_AND_RETURN(PROPERTY, TYPE, CACHE) {                      \
    SYNC_CACHE(PROPERTY, TYPE, CACHE);                                      \
    return CACHE;                                                           \
}

#define SYNC_CACHE_WITH_ID(PROPERTY, TYPE, CACHE)                           \
    if (! CACHE_VALID(PROPERTY)) {                                          \
        API::template getHardwareState<PROPERTY>(this->id(), CACHE);        \
        VALIDATE_CACHE(PROPERTY);                                           \
    }

#define SYNC_CACHE_WITH_ID_AND_RETURN(PROPERTY, TYPE, CACHE) {              \
    SYNC_CACHE_WITH_ID(PROPERTY, TYPE, CACHE);                              \
    return CACHE;                                                           \
}

#define WRITE_THRU_CACHE(PROPERTY, TYPE, CACHE, VALUE) {                    \
    CACHE = VALUE;                                                          \
    API::template setHardwareState<PROPERTY>(CACHE);                        \
    VALIDATE_CACHE(PROPERTY);                                               \
}

#define WRITE_THRU_CACHE_WITH_ID(PROPERTY, TYPE, CACHE, VALUE) {            \
    CACHE = VALUE;                                                          \
    API::template setHardwareState<PROPERTY>(this->id(), CACHE);            \
    VALIDATE_CACHE(PROPERTY);                                               \
}


// This macro creates a boolean-valued renderer property. Specifically,
// it creates a boolean cache variable called _is<NAME>Enabled, and a pair
// of accessor functions:
//      bool is<NAME>Enabled() const
//      void enable<NAME>(bool)
#define TOGGLE_PROPERTY(NAME)                                           \
public:                                                                 \
    bool is ## NAME ## Enabled() const {                                \
        SYNC_CACHE_AND_RETURN(API::NAME, bool, _is ## NAME ## Enabled); \
    }                                                                   \
    void set ## NAME ## Enabled(bool enabled) {                         \
        WRITE_THRU_CACHE(API::NAME, bool, _is ## NAME ## Enabled, enabled);\
    }                                                                   \
protected:                                                              \
    mutable bool _is ## NAME ## Enabled;


// This macro creates a renderer property passed by reference (appropriate
// for matrices, etc.). Specifically, it creates a cache variable of
// the appropriate type called _<NAME> and a pair of accessor functions:
//      const TYPE & <NAME>() const
//      void set<CNAME>(TYPE)
#define REF_PROPERTY(TYPE, NAME, CNAME)                                 \
public:                                                                 \
    const TYPE & NAME() const {                                         \
        SYNC_CACHE_AND_RETURN(API::NAME, TYPE, _ ## NAME);              \
    }                                                                   \
    void set ## CNAME(const TYPE & value) {                             \
        WRITE_THRU_CACHE(API::NAME, TYPE, _ ## NAME, value);            \
    }                                                                   \
protected:                                                              \
    mutable TYPE _ ## NAME;


// This macro creates a renderer property passed by value (appropriate
// for int, float, etc.). Specifically, it creates a cache variable of
// the appropriate type called _<LNAME> and a pair of accessor functions:
//      TYPE <LNAME>() const
//      void set<NAME>(TYPE)
#define VALUE_PROPERTY(TYPE, NAME, LNAME)                               \
public:                                                                 \
    TYPE LNAME() const {                                                \
        SYNC_CACHE_AND_RETURN(API::NAME, TYPE, _ ## LNAME);             \
    }                                                                   \
    void set ## NAME(TYPE value) {                                      \
        WRITE_THRU_CACHE(API::NAME, TYPE, _ ## LNAME, value);           \
    }                                                                   \
protected:                                                              \
    mutable TYPE _ ## LNAME;


// This macro creates a
#define ARRAY_PROPERTY(NAME, LNAME)                                     \
public:                                                                 \
    /* Array pointer accessor */                                        \
    void const * LNAME ## Pointer() const {                             \
        SYNC_CACHE_AND_RETURN(API::NAME ## Pointer,                     \
                              void const *, _ ## LNAME ## Pointer);     \
    }                                                                   \
                                                                        \
    /* Stride accessor */                                               \
    DifferenceType LNAME ## Stride() const {                            \
        SYNC_CACHE_AND_RETURN(API::NAME ## Stride,                      \
                              DifferenceType, _ ## LNAME ## Stride);    \
    }                                                                   \
                                                                        \
    /* DataArray version */                                             \
    template <typename ElementType>                                     \
    void set ## NAME(const DataArray<ElementType> & array) {            \
        set ## NAME(array.elements(), 0);                               \
    }                                                                   \
                                                                        \
    /* Raw array pointer version */                                     \
    template <typename ElementType>                                     \
    void set ## NAME(ElementType const * elements, DifferenceType stride = 0) {\
        if (! (SAME_AS_CACHE(API::NAME ## Pointer,                      \
                             _ ## LNAME ## Pointer, elements) &&        \
               SAME_AS_CACHE(API::NAME ## Stride,                       \
                             _ ## LNAME ## Stride, stride))) {          \
            WRITE_CACHE(API::NAME ## Pointer, _ ## LNAME ## Pointer, elements);  \
            WRITE_CACHE(API::NAME ## Stride,  _ ## LNAME ## Stride,  stride);    \
            API::template setHardwareState<API::NAME ## Pointer,        \
                                           ElementType>(elements, stride);\
        }                                                               \
    }                                                                   \
                                                                        \
protected:                                                              \
    mutable void const * _ ## LNAME ## Pointer;                         \
    mutable SizeType _ ## LNAME ## Stride;

#if 0
    template <typename ElementType>                                     \
    void set ## NAME(ElementType const * elements, DifferenceType stride = 0) {\
        if (! (SAME_AS_CACHE(API::NAME ## Pointer,                      \
                             _ ## LNAME ## Pointer, elements) &&        \
               SAME_AS_CACHE(API::NAME ## Stride,                       \
                             _ ## LNAME ## Stride, stride))) {          \
            WRITE_CACHE(API::NAME ## Pointer, _ ## LNAME ## Pointer, elements);  \
            WRITE_CACHE(API::NAME ## Stride,  _ ## LNAME ## Stride,  stride);    \
            API::setDataArray<API::NAME ## Pointer, ElementType>(elements, stride);    \
        }                                                               \
    }                                                                   \

#endif
