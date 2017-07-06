/*
  +----------------------------------------------------------------------+
  | PHP Version 7                                                        |
  +----------------------------------------------------------------------+
  | Copyright (c) 1997-2017 The PHP Group                                |
  +----------------------------------------------------------------------+
  | This source file is subject to version 3.01 of the PHP license,      |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://www.php.net/license/3_01.txt                                  |
  | If you did not receive a copy of the PHP license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | license@php.net so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author:                                                              |
  +----------------------------------------------------------------------+
*/

/* $Id$ */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_panda.h"

/* If you declare any globals in php_panda.h uncomment this:
ZEND_DECLARE_MODULE_GLOBALS(panda)
*/

/* True global resources - no need for thread safety here */
static int le_panda;

/* {{{ PHP_INI
 */
/* Remove comments and fill if you need to have entries in php.ini
PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("panda.global_value",      "42", PHP_INI_ALL, OnUpdateLong, global_value, zend_panda_globals, panda_globals)
    STD_PHP_INI_ENTRY("panda.global_string", "foobar", PHP_INI_ALL, OnUpdateString, global_string, zend_panda_globals, panda_globals)
PHP_INI_END()
*/
/* }}} */

/* Remove the following function when you have successfully modified config.m4
   so that your module can be compiled into PHP, it exists only for testing
   purposes. */

/* Every user-visible function in PHP should document itself in the source */
/* {{{ proto string confirm_panda_compiled(string arg)
   Return a string to confirm that the module is compiled in */
PHP_FUNCTION(panda)
{
		zend_string *strg;
		strg = strpprintf(0, "hello panda");
		RETURN_STR(strg);
}

/**
 *
 * @param execute_data
 * @param return_value
 *
 *
 * 获取参数

在PHP7中提供了两种获取参数的方法。zend_parse_parameters和FAST ZPP方式。
zend_parse_parameters

在PHP7之前一直使用zend_parse_parameters函数获取参数。这个函数的作用，就是把传入的参数转换为PHP内核中相应的类型，方便在PHP扩展中使用。 参数说明：
第一个参数，参数个数。一般就使用ZEND_NUM_ARGS()，不需要改变。
第二个参数，格式化字符串。这个格式化字符串的作用就是，指定传入参数与PHP内核类型的转换关系。
代码中 S|z 的含义就是：

S 表示参数是一个字符串。要把传入的参数转换为zend_string类型。
| 表示之后的参数是可选。可以传，也可以不传。
z 表示参数是多种类型。要把传入的参数转换为zval类型。
除此之外，还有一些specifier，需要注意：

！如果接收了一个PHP语言里的null变量，则直接把其转成C语言里的NULL，而不是封装成IS_NULL类型的zval。
/ 如果传递过来的变量与别的变量共用一个zval，而且不是引用，则进行强制分离，新的zval的is_refgc==0, and refcountgc==1.
更多格式化字符串的含义可以查看官方网站。https://wiki.php.net/rfc/fast_zpp

FAST ZPP

在PHP7中新提供的方式。是为了提高参数解析的性能。对应经常使用的方法，建议使用FAST ZPP方式。 使用方式： 以ZEND_PARSE_PARAMETERS_START(1, 2)开头。 第一个参数表示必传的参数个数，第二个参数表示最多传入的参数个数。 以ZEND_PARSE_PARAMETERS_END();结束。 中间是传入参数的解析。 值得注意的是，一般FAST ZPP的宏方法与zend_parse_parameters的specifier是一一对应的。如：
Z_PARAM_OPTIONAL 对应 |
Z_PARAM_STR 对应 S
但是，Z_PARAM_ZVAL_EX方法比较特殊。它对应两个specifier，分别是 ! 和 / 。! 对应宏方法的第二个参数。/ 对应宏方法的第三个参数。如果想开启，只要设置为1即可。

FAST ZPP相应的宏方法可以查看官方网站 https://wiki.php.net/rfc/fast_zpp#proposal

返回值

方法的返回值是使用RETURN_开头的宏方法进行返回的。常用的宏方法有：
RETURN_NULL()        返回null
RETURN_LONG(l)        返回整型
RETURN_DOUBLE(d) 返回浮点型
RETURN_STR(s)        返回一个字符串。参数是一个zend_string * 指针
RETURN_STRING(s)    返回一个字符串。参数是一个char * 指针
RETURN_STRINGL(s, l) 返回一个字符串。第二个参数是字符串长度。
RETURN_EMPTY_STRING()    返回一个空字符串。
RETURN_ARR(r)        返回一个数组。参数是zend_array *指针。
RETURN_OBJ(r)         返回一个对象。参数是zend_object *指针。
RETURN_ZVAL(zv, copy, dtor) 返回任意类型。参数是 zval *指针。
RETURN_FALSE        返回false
RETURN_TRUE            返回true
更多宏方法请查看 Zend/zend_API.h中的相关代码。
 */
PHP_FUNCTION(default_value)
{
    zend_string     *type;
    zval            *value = NULL;

#ifndef FAST_ZPP
    /* Get function parameters and do error-checking. */
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "S|z", &type, &value) == FAILURE) {
        return;
    }
#else
    ZEND_PARSE_PARAMETERS_START(1, 2)
            Z_PARAM_STR(type)
            Z_PARAM_OPTIONAL
            Z_PARAM_ZVAL_EX(value, 0, 1)
    ZEND_PARSE_PARAMETERS_END();
#endif

    if (ZSTR_LEN(type) == 3 && strncmp(ZSTR_VAL(type), "int", 3) == 0 && value == NULL) {
        RETURN_LONG(0);
    } else if (ZSTR_LEN(type) == 3 && strncmp(ZSTR_VAL(type), "int", 3) == 0 && value != NULL) {
        RETURN_ZVAL(value, 0, 1);
    } else if (ZSTR_LEN(type) == 4 && strncmp(ZSTR_VAL(type), "bool", 4) == 0 && value == NULL) {
        RETURN_FALSE;
    } else if (ZSTR_LEN(type) == 4 && strncmp(ZSTR_VAL(type), "bool", 4) == 0 && value != NULL) {
        RETURN_ZVAL(value, 0, 1);
    } else if (ZSTR_LEN(type) == 3 && strncmp(ZSTR_VAL(type), "str", 3) == 0 && value == NULL) {
        RETURN_EMPTY_STRING();
    } else if (ZSTR_LEN(type) == 3 && strncmp(ZSTR_VAL(type), "str", 3) == 0 && value != NULL) {
        RETURN_ZVAL(value, 0, 1);
    }
    RETURN_NULL();
}

PHP_FUNCTION(confirm_panda_compiled)
{
	char *arg = NULL;
	size_t arg_len, len;
	zend_string *strg;

	if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &arg, &arg_len) == FAILURE) {
		return;
	}

	strg = strpprintf(0, "Congratulations! You have successfully modified ext/%.78s/config.m4. Module %.78s is now compiled into PHP.", "panda", arg);

	RETURN_STR(strg);
}
/* }}} */
/* The previous line is meant for vim and emacs, so it can correctly fold and
   unfold functions in source code. See the corresponding marks just before
   function definition, where the functions purpose is also documented. Please
   follow this convention for the convenience of others editing your code.
*/


/* {{{ php_panda_init_globals
 */
/* Uncomment this function if you have INI entries
static void php_panda_init_globals(zend_panda_globals *panda_globals)
{
	panda_globals->global_value = 0;
	panda_globals->global_string = NULL;
}
*/
/* }}} */

/* {{{ PHP_MINIT_FUNCTION
 */
PHP_MINIT_FUNCTION(panda)
{
	/* If you have INI entries, uncomment these lines
	REGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MSHUTDOWN_FUNCTION
 */
PHP_MSHUTDOWN_FUNCTION(panda)
{
	/* uncomment this line if you have INI entries
	UNREGISTER_INI_ENTRIES();
	*/
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request start */
/* {{{ PHP_RINIT_FUNCTION
 */
PHP_RINIT_FUNCTION(panda)
{
#if defined(COMPILE_DL_PANDA) && defined(ZTS)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif
	return SUCCESS;
}
/* }}} */

/* Remove if there's nothing to do at request end */
/* {{{ PHP_RSHUTDOWN_FUNCTION
 */
PHP_RSHUTDOWN_FUNCTION(panda)
{
	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION
 */
PHP_MINFO_FUNCTION(panda)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "panda support", "enabled");
	php_info_print_table_end();

	/* Remove comments if you have entries in php.ini
	DISPLAY_INI_ENTRIES();
	*/
}
/* }}} */

/* {{{ panda_functions[]
 *
 * Every user visible function must have an entry in panda_functions[].
 */
const zend_function_entry panda_functions[] = {
	PHP_FE(panda, NULL)
	PHP_FE(confirm_panda_compiled,	NULL)		/* For testing, remove later. */
	PHP_FE_END	/* Must be the last line in panda_functions[] */
};
/* }}} */

/* {{{ panda_module_entry
 */
zend_module_entry panda_module_entry = {
	STANDARD_MODULE_HEADER,
	"panda",
	panda_functions,
	PHP_MINIT(panda),
	PHP_MSHUTDOWN(panda),
	PHP_RINIT(panda),		/* Replace with NULL if there's nothing to do at request start */
	PHP_RSHUTDOWN(panda),	/* Replace with NULL if there's nothing to do at request end */
	PHP_MINFO(panda),
	PHP_PANDA_VERSION,
	STANDARD_MODULE_PROPERTIES
};
/* }}} */

#ifdef COMPILE_DL_PANDA
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(panda)
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
