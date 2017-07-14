dnl $Id$
dnl config.m4 for extension magox

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(magox, for magox support,
dnl Make sure that the comment is aligned:
dnl [  --with-magox             Include magox support])

dnl Otherwise use enable:

PHP_ARG_ENABLE(magox, whether to enable magox support,
Make sure that the comment is aligned:
[  --enable-magox           Enable magox support])

if test "$PHP_MAGOX" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-magox -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/magox.h"  # you most likely want to change this
  dnl if test -r $PHP_MAGOX/$SEARCH_FOR; then # path given as parameter
  dnl   MAGOX_DIR=$PHP_MAGOX
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for magox files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       MAGOX_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$MAGOX_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the magox distribution])
  dnl fi

  dnl # --with-magox -> add include path
  dnl PHP_ADD_INCLUDE($MAGOX_DIR/include)

  dnl # --with-magox -> check for lib and symbol presence
  dnl LIBNAME=magox # you may want to change this
  dnl LIBSYMBOL=magox # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $MAGOX_DIR/$PHP_LIBDIR, MAGOX_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_MAGOXLIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong magox lib version or lib not found])
  dnl ],[
  dnl   -L$MAGOX_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(MAGOX_SHARED_LIBADD)

  PHP_NEW_EXTENSION(magox, magox.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
