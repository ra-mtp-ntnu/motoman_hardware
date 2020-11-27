#ifndef MOTOMAN_HARDWARE__VISIBILITY_CONTROL_H_
#define MOTOMAN_HARDWARE__VISIBILITY_CONTROL_H_

// This logic was borrowed (then namespaced) from the examples on the gcc wiki:
//     https://gcc.gnu.org/wiki/Visibility

#if defined _WIN32 || defined __CYGWIN__
  #ifdef __GNUC__
    #define MOTOMAN_HARDWARE_EXPORT __attribute__ ((dllexport))
    #define MOTOMAN_HARDWARE_IMPORT __attribute__ ((dllimport))
  #else
    #define MOTOMAN_HARDWARE_EXPORT __declspec(dllexport)
    #define MOTOMAN_HARDWARE_IMPORT __declspec(dllimport)
  #endif
  #ifdef MOTOMAN_HARDWARE_BUILDING_LIBRARY
    #define MOTOMAN_HARDWARE_PUBLIC MOTOMAN_HARDWARE_EXPORT
  #else
    #define MOTOMAN_HARDWARE_PUBLIC MOTOMAN_HARDWARE_IMPORT
  #endif
  #define MOTOMAN_HARDWARE_PUBLIC_TYPE MOTOMAN_HARDWARE_PUBLIC
  #define MOTOMAN_HARDWARE_LOCAL
#else
  #define MOTOMAN_HARDWARE_EXPORT __attribute__ ((visibility("default")))
  #define MOTOMAN_HARDWARE_IMPORT
  #if __GNUC__ >= 4
    #define MOTOMAN_HARDWARE_PUBLIC __attribute__ ((visibility("default")))
    #define MOTOMAN_HARDWARE_LOCAL  __attribute__ ((visibility("hidden")))
  #else
    #define MOTOMAN_HARDWARE_PUBLIC
    #define MOTOMAN_HARDWARE_LOCAL
  #endif
  #define MOTOMAN_HARDWARE_PUBLIC_TYPE
#endif

#endif  // MOTOMAN_HARDWARE__VISIBILITY_CONTROL_H_
