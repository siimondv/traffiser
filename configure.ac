AC_PREREQ(2.60)
AC_CONFIG_AUX_DIR([generated])
AC_INIT(traffiser, [0.1.0], simon.delgd@gmail.com)
AM_MAINTAINER_MODE([disable])


AM_INIT_AUTOMAKE([-Wall -Werror])

dnl if test "$CXXFLAGS" = ""; then
dnl CXXFLAGS="-Os -Wall"
dnl fi


AC_CHECK_PROG(BASH_F, bash, "yes")
echo "BASH_F= $BASH_F"
if test "$BASH_F" != "yes";
then
  AC_MSG_ERROR(["cannot find bash shell, download and build it: see www.gnu.org/software/bash/"])
fi
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CXX (gcc)
AC_PROG_CPP
AC_PROG_INSTALL
AC_CHECK_HEADERS([sys/time.h])
AC_C_CONST
AC_C_INLINE
AC_FUNC_STRFTIME
AC_HEADER_STDBOOL
AC_HEADER_TIME
AC_STRUCT_TM
AC_CHECK_HEADER(netinet/ip.h, ,
  AC_MSG_ERROR(["cannot find netinet/ip.h"]))
AC_PROG_LIBTOOL
AC_CHECK_LIB(pcap, pcap_open_live,
  [],
  [AC_MSG_ERROR([pcap_open_live not found])]
)

AX_BOOST_BASE([1.46])

AX_BOOST_REGEX
AX_BOOST_PROGRAM_OPTIONS
AX_BOOST_IOSTREAMS


AC_OUTPUT(Makefile src/Makefile )