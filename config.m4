dnl $Id$
dnl config.m4 for extension panda

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary. This file will not work
dnl without editing.

dnl If your extension references something external, use with:

dnl PHP_ARG_WITH(panda, for panda support,
dnl Make sure that the comment is aligned:
dnl [  --with-panda             Include panda support])

dnl Otherwise use enable:

dnl PHP_ARG_ENABLE(panda, whether to enable panda support,
dnl Make sure that the comment is aligned:
dnl [  --enable-panda           Enable panda support])

if test "$PHP_PANDA" != "no"; then
  dnl Write more examples of tests here...

  dnl # --with-panda -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/panda.h"  # you most likely want to change this
  dnl if test -r $PHP_PANDA/$SEARCH_FOR; then # path given as parameter
  dnl   PANDA_DIR=$PHP_PANDA
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for panda files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       PANDA_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$PANDA_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the panda distribution])
  dnl fi

  dnl # --with-panda -> add include path
  dnl PHP_ADD_INCLUDE($PANDA_DIR/include)

  dnl # --with-panda -> check for lib and symbol presence
  dnl LIBNAME=panda # you may want to change this
  dnl LIBSYMBOL=panda # you most likely want to change this 

  dnl PHP_CHECK_LIBRARY($LIBNAME,$LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $PANDA_DIR/$PHP_LIBDIR, PANDA_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_PANDALIB,1,[ ])
  dnl ],[
  dnl   AC_MSG_ERROR([wrong panda lib version or lib not found])
  dnl ],[
  dnl   -L$PANDA_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(PANDA_SHARED_LIBADD)

  PHP_NEW_EXTENSION(panda, panda.c, $ext_shared,, -DZEND_ENABLE_STATIC_TSRMLS_CACHE=1)
fi
