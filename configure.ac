#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([mnslp_ipfix], [0.1], [la.marentes455@uniandes.edu.co])
AM_INIT_AUTOMAKE
AM_PATH_CPPUNIT(1.9.6)
LT_INIT
AC_CONFIG_SRCDIR([src/main.cpp])
AC_CONFIG_MACRO_DIR([m4])

## ------------------------Begin instantiation part 
## Meaning:
## An instantiating macro is one which defines one or more tags, 
## usually referring to files that are to be translated by the generated 
## configure scripts, from a template containing Autoconf substitution 
## variables.


AM_MAINTAINER_MODE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_AWK
AC_PROG_INSTALL

AC_LANG_CPLUSPLUS

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.

# Checks for libraries.
#
# os check
#

# Missing code check if the libipfix library is installed. 
# This suppose that .a file is in /usr/local/lib.
AC_LANG(C++)
#AC_CHECK_LIB([ipfix], 
#             [ipfix_init], 
#             [HAVE_IPFIX=1] [IPFIX_LIBS="-lipfix -lmisc"] [IPFIX_LDFLAGS="-L/user/local/lib"] [IPFIX_CPPFLAGS="-I/user/local/include"],  
#             AC_MSG_WARN([libipfix is not installed.]),
#             -lmisc)

AC_SUBST(IPFIX_CPPFLAGS)
AC_SUBST(IPFIX_LDFLAGS)
AC_SUBST(IPFIX_LIBS)


AC_CONFIG_FILES([Makefile
                 src/Makefile
                 test/Makefile])

AC_OUTPUT
