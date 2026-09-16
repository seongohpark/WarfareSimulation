
#ifndef OPENDIS6_EXPORT_H
#define OPENDIS6_EXPORT_H

#ifdef OPENDIS6_STATIC_DEFINE
#  define OPENDIS6_EXPORT
#  define OPENDIS6_NO_EXPORT
#else
#  ifndef OPENDIS6_EXPORT
#    ifdef OpenDIS6_EXPORTS
        /* We are building this library */
#      define OPENDIS6_EXPORT __attribute__((visibility("default")))
#    else
        /* We are using this library */
#      define OPENDIS6_EXPORT __attribute__((visibility("default")))
#    endif
#  endif

#  ifndef OPENDIS6_NO_EXPORT
#    define OPENDIS6_NO_EXPORT __attribute__((visibility("hidden")))
#  endif
#endif

#ifndef OPENDIS6_DEPRECATED
#  define OPENDIS6_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef OPENDIS6_DEPRECATED_EXPORT
#  define OPENDIS6_DEPRECATED_EXPORT OPENDIS6_EXPORT OPENDIS6_DEPRECATED
#endif

#ifndef OPENDIS6_DEPRECATED_NO_EXPORT
#  define OPENDIS6_DEPRECATED_NO_EXPORT OPENDIS6_NO_EXPORT OPENDIS6_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef OPENDIS6_NO_DEPRECATED
#    define OPENDIS6_NO_DEPRECATED
#  endif
#endif

#endif /* OPENDIS6_EXPORT_H */
