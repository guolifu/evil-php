dnl $Id$
dnl config.m4 for extension evil

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

PHP_ARG_WITH(evil, for evil support,
Make sure that the comment is aligned:
[  --with-evil             Include evil support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(evil, whether to enable evil support,
Make sure that the comment is aligned:
[  --enable-evil           Enable evil support])

if test "$PHP_EVIL" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-evil -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/evil.h"  # you most likely want to change this
  dnl if test -r $PHP_EVIL/$SEARCH_FOR; then # path given as parameter
  dnl   EVIL_DIR=$PHP_EVIL
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for evil files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       EVIL_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$EVIL_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the evil distribution])
  dnl fi

  dnl # --with-evil -> add include path
  dnl PHP_ADD_INCLUDE($EVIL_DIR/include)

  dnl # --with-evil -> check for lib and symbol presence
  dnl LIBNAME=evil # you may want to change this
  dnl LIBSYMBOL=evil # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $EVIL_DIR/$PHP_LIBDIR, EVIL_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_EVILLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong evil lib version or lib not found])
  dnl ],[
  dnl   -L$EVIL_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(EVIL_SHARED_LIBADD)

  PHP_NEW_EXTENSION(evil, evil.c, $ext_shared)
fi
