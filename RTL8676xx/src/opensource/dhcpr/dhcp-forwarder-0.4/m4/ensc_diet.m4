dnl $Id: ensc_diet.m4,v 1.1.1.1 2003/11/25 22:14:04 jharrell Exp $

dnl Copyright (C) 2002 Enrico Scholz <enrico.scholz@informatik.tu-chemnitz.de>
dnl  
dnl This program is free software; you can redistribute it and/or modify
dnl it under the terms of the GNU General Public License as published by
dnl the Free Software Foundation; version 2 of the License.
dnl  
dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
dnl GNU General Public License for more details.
dnl  
dnl You should have received a copy of the GNU General Public License
dnl along with this program; if not, write to the Free Software
dnl Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
dnl  
dnl  
dnl As a special exception to the GNU General Public License, if you
dnl distribute this file as part of a program that contains a configuration
dnl script generated by Autoconf, you may include it under the same
dnl distribution terms that you use for the rest of that program.
dnl  

AC_DEFUN([ENSC_DIET],
[
	AC_MSG_CHECKING([whether to use dietlibc])

	if test x"${DIET+set}" != xset; then
		DIET=${ensc_cv_prog_diet}
	fi

	AC_ARG_ENABLE(dietlibc,
		      [AC_HELP_STRING([--enable-dietlibc],
				      [use dietlibc (yes)])],
	              [ case x"$enableval" in
				xyes)	ensc_use_dietlibc="yes";;
				x|xno)	ensc_use_dietlibc="no";;
				*)	AC_MSG_ERROR([Invalid value "$enableval" for '--enable-dietlibc'])
			esac ],
		      [	if which ${DIET:-diet} >/dev/null 2>&1; then
				ensc_use_dietlibc=yes
			else
				ensc_use_dietlibc=no
			fi ])
	
	if test x"${ensc_use_dietlibc}" = xyes; then
		: ${DIET:='diet'}
		ensc_cv_prog_diet="${DIET}"
		AC_MSG_RESULT([yes; using '${DIET}' wrapper])
	else
		DIET=
		AC_MSG_RESULT(no)
	fi

	AM_CONDITIONAL(ENSC_ENABLE_DIET, test x"${DIET}" != x)
	AC_SUBST(DIET)
])

AC_DEFUN([ENSC_DIET_CHECK_IN_ADDR_T],
[
	AC_REQUIRE([ENSC_DIET])

	if test x"${DIET}" != x; then
		AC_CACHE_CHECK([whether in_addr_t exists in dietlibc],
		       [ensc_cv_type_diet_in_addr_t],
		       [AC_LANG_PUSH(C)
			old_cc="${CC}"
			CC="${DIET} ${CC}"
	                AC_TRY_COMPILE([#include <netinet/in.h>
	                               ],
	                               [in_addr_t	foo],
		                       [ensc_cv_type_diet_in_addr_t=yes],
	    	                       [ensc_cv_type_diet_in_addr_t=no])
			CC="${old_cc}"
			AC_LANG_POP(C)
	               ])
	
	
		if test x"${ensc_cv_type_diet_in_addr_t}" = xyes; then
			AC_DEFINE([DIET_HAS_IN_ADDR_T], 1,
	        	          [Define if dietlibc defines the in_addr_t type])
		fi
	fi

	ENSC_TYPE_IN_ADDR_T
])
