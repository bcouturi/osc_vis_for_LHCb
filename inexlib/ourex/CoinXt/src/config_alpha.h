/* G.Barrand :
   copied from config_linux.h
   and adapted by hand for alpha_OSF1
*/

/* src/config.h.  Generated by configure.  */
/* src/config.h.in.  Generated from configure.ac by autoheader.  */


#ifndef SOXT_INTERNAL
#error this is a private header file
#endif /* !SOXT_INTERNAL */


/* The CPP compiler has a variable containing the current function name */
#define HAVE_CPP_COMPILER_FUNCTION_NAME_VAR __func__

/* The C compiler has a variable containing the current function name */
#define HAVE_C_COMPILER_FUNCTION_NAME_VAR __func__

/* Define to 1 if you have the <dlfcn.h> header file. */
#define HAVE_DLFCN_H 1

/* Define to use GetEnvironmentVariable() instead of getenv() */
/* #undef HAVE_GETENVIRONMENTVARIABLE */

/* define if the GLU header should be included as GL/glu.h */
/* #undef HAVE_GL_GLU_H */

/* define if the GL header should be included as GL/gl.h */
#define HAVE_GL_GL_H 1

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to enable Linux Joystick driver support */
/*G.Barrand #define HAVE_JOYSTICK_LINUX  */

/* define this if -lXmu can be used */
#define HAVE_LIBXMU 

/* Define this to use libXpm */
#define HAVE_LIBXPM 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* define if the GLU header should be included as OpenGL/glu.h */
/* #undef HAVE_OPENGL_GLU_H */

/* define if the GL header should be included as OpenGL/gl.h */
/* #undef HAVE_OPENGL_GL_H */

/* define if pthread's struct timespec uses nsecs and not usecs */
/* #undef HAVE_PTHREAD_TIMESPEC_NSEC */

/* Define to enable use of Inventor feature */
#define HAVE_SOCAMERA_SETSTEREOMODE 1

/* Define to enable use of Inventor feature */
#define HAVE_SOKEYBOARDEVENT_DELETE 1

/* Define to enable use of Inventor feature */
#define HAVE_SOMOUSEBUTTONEVENT_BUTTON5 1

/* Define to enable use of the Open Inventor SoPolygonOffset node */
#define HAVE_SOPOLYGONOFFSET 1

/* Define to enable use of Inventor feature */
#define HAVE_SORTED_LAYERS_BLEND 1

/* Define to enable use of the Open Inventor SoVRMLMaterial VRML node */
#define HAVE_SOVRMLMATERIAL 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Define to 1 if you have the <windows.h> header file. */
/* #undef HAVE_WINDOWS_H */

/* Name of package */
#define PACKAGE "SoXt"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT "coin-support@coin3d.org"

/* Define to the full name of this package. */
#define PACKAGE_NAME "SoXt"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "SoXt 1.2.2"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "soxt"

/* Define to the version of this package. */
#define PACKAGE_VERSION "1.2.2"

/* define this to the SoXt beta version letter */
/* #undef SOXT_BETA_VERSION */

/* Define this to device handle to fallback on */
#define SOXT_LINUX_JOYSTICK_DEVICE "/dev/js0"

/* SoXt major version */
#define SOXT_MAJOR_VERSION 1

/* SoXt micro version */
#define SOXT_MICRO_VERSION 2

/* SoXt minor version */
#define SOXT_MINOR_VERSION 2

/* Version string for SoXt (major.minor.micro) */
#define SOXT_VERSION "1.2.2"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "1.2.2"

/* Define this if you want defaults to be linked into SoXt */
#define WITH_STATIC_DEFAULTS 

/* Define to 1 if the X Window System is missing or not being used. */
/* #undef X_DISPLAY_MISSING */
