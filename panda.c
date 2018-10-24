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
  | Author:  Panda  <itwujunze@gmail.com>                                |
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
#include <stdio.h>

ZEND_BEGIN_ARG_INFO(default_value_arg_info, 0)
                ZEND_ARG_INFO(0, type)
                ZEND_ARG_INFO(0, value)
ZEND_END_ARG_INFO()


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
    STD_PHP_INI_ENTRY("panda.number",      "100", PHP_INI_ALL, OnUpdateLong, global_value, zend_panda_globals, panda_globals)
    STD_PHP_INI_ENTRY("panda.string", "ab", PHP_INI_ALL, OnUpdateString, global_string, zend_panda_globals, panda_globals)
    STD_PHP_INI_ENTRY("panda.boolean", "0", PHP_INI_ALL, OnUpdateBool, global_string, zend_panda_globals, panda_globals)
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
 * 实现扩展函数的传参
 *
 * 参考文档  https://github.com/pangudashu/php7-internal/blob/master/7/func.md#762-函数参数解析
 *
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


PHP_FUNCTION(get_size)
{
        zval *val;
        size_t size;
        zend_string *result;
        HashTable *myht;

        if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &val) == FAILURE) {
            return;
        }

        if (Z_TYPE_P(val) == IS_STRING) {
            result = strpprintf(0, "string size is %d", Z_STRLEN_P(val));
        } else if (Z_TYPE_P(val) == IS_ARRAY) {
            myht = Z_ARRVAL_P(val);
            result = strpprintf(0, "array size is %d", zend_array_count(myht));
        } else {
            result = strpprintf(0, "can not support");
        }

        RETURN_STR(result);
}

PHP_FUNCTION(str_concat)
{

        zend_string *prefix, *subject, *result;
        zval *string;

        if (zend_parse_parameters(ZEND_NUM_ARGS(), "Sz", &prefix, &string) == FAILURE) {
            return;
        }

        subject = zval_get_string(string);
        if (zend_binary_strncmp(ZSTR_VAL(prefix), ZSTR_LEN(prefix), ZSTR_VAL(subject), ZSTR_LEN(subject), ZSTR_LEN(prefix)) == 0) {
            RETURN_STR(subject);
        }
        result = strpprintf(0, "%s %s", ZSTR_VAL(prefix), ZSTR_VAL(subject));
        RETURN_STR(result);
}

PHP_FUNCTION(arr_concat)
{
    zval *arr, *prefix, *entry, *prefix_entry, value;
    zend_string *string_key, *result;
    zend_ulong num_key;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "aa", &arr, &prefix) == FAILURE) {
        return;
    }

    array_init_size(return_value, zend_hash_num_elements(Z_ARRVAL_P(arr)));

    ZEND_HASH_FOREACH_KEY_VAL(Z_ARRVAL_P(arr), num_key, string_key, entry) {
                if (string_key && zend_hash_exists(Z_ARRVAL_P(prefix), string_key)) {
                    prefix_entry = zend_hash_find(Z_ARRVAL_P(prefix), string_key);
                    if (Z_TYPE_P(entry) == IS_STRING && prefix_entry != NULL && Z_TYPE_P(prefix_entry) == IS_STRING) {
                        result = strpprintf(0, "%s%s", Z_STRVAL_P(prefix_entry), Z_STRVAL_P(entry));
                        ZVAL_STR(&value, result);
                        zend_hash_update(Z_ARRVAL_P(return_value), string_key, &value);
                    }
                } else if (string_key == NULL && zend_hash_index_exists(Z_ARRVAL_P(prefix), num_key)){
                    prefix_entry = zend_hash_index_find(Z_ARRVAL_P(prefix), num_key);
                    if (Z_TYPE_P(entry) == IS_STRING && prefix_entry != NULL && Z_TYPE_P(prefix_entry) == IS_STRING) {
                        result = strpprintf(0, "%s%s", Z_STRVAL_P(prefix_entry), Z_STRVAL_P(entry));
                        ZVAL_STR(&value, result);
                        zend_hash_index_update(Z_ARRVAL_P(return_value), num_key, &value);
                    }
                } else if (string_key) {
                    zend_hash_update(Z_ARRVAL_P(return_value), string_key, entry);
                    zval_add_ref(entry);
                } else  {
                    zend_hash_index_update(Z_ARRVAL_P(return_value), num_key, entry);
                    zval_add_ref(entry);
                }
            }ZEND_HASH_FOREACH_END();

}


PHP_FUNCTION(call_function)
{
    zval            *obj = NULL;
    zval            *fun = NULL;
    zval            *param = NULL;
    zval            retval;
    zval            args[1];

#ifndef FAST_ZPP
    /* Get function parameters and do error-checking. */
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zzz", &obj, &fun, &param) == FAILURE) {
        return;
    }
#else
    ZEND_PARSE_PARAMETERS_START(3, 3)
            Z_PARAM_ZVAL(obj)
            Z_PARAM_ZVAL(fun)
            Z_PARAM_ZVAL(param)
    ZEND_PARSE_PARAMETERS_END();
#endif

    args[0] = *param;
    if (obj == NULL || Z_TYPE_P(obj) == IS_NULL) {
        /**
         * call_user_function_ex方法用于调用函数和方法。参数说明如下：
         * 第一个参数：方法表。通常情况下，写 EG(function_table)
         * 第二个参数：对象。如果不是调用对象的方法，而是调用函数，填写 NULL
         * 第三个参数：方法名。
         * 第四个参数：返回值。
         * 第五个参数：参数个数。
         * 第六个参数：参数值。是一个 zval 数组。
         * 第七个参数：参数是否进行分离操作。详细的，你可以搜索下 PHP 参数分离。查看相关文章
         * 第八个参数：符号表。一般情况写设置为 NULL 即可。
         *
         */
        call_user_function_ex(EG(function_table), NULL, fun, &retval, 1, args, 0, NULL);
    } else {
        call_user_function_ex(EG(function_table), obj, fun, &retval, 1, args, 0, NULL);
    }
    RETURN_ZVAL(&retval, 0, 1);
}

PHP_FUNCTION(show_ini)
{
    zval arr;
    array_init(&arr);
    add_assoc_long_ex(&arr, "panda.number", 10, PANDA_G(global_number));
    add_assoc_string_ex(&arr, "panda.string", 10, PANDA_G(global_string));
    add_assoc_bool_ex(&arr, "panda.boolean", 11, PANDA_G(global_boolean));
    RETURN_ZVAL(&arr, 0, 1);
}

static void panda_hash_destroy(HashTable *ht)
{
    zend_string *key;
    zval *element;
    if (((ht)->u.flags & HASH_FLAG_INITIALIZED)) {
        ZEND_HASH_FOREACH_STR_KEY_VAL(ht, key, element) {
                    if (key) {
                        free(key);
                    }
                    switch (Z_TYPE_P(element)) {
                        case IS_STRING:
                            free(Z_PTR_P(element));
                            break;
                        case IS_ARRAY:
                            say_hash_destroy(Z_ARRVAL_P(element));
                            break;
                    }
                } ZEND_HASH_FOREACH_END();
        free(HT_GET_DATA_ADDR(ht));
    }
    free(ht);
}

//释放数组和字符串
static void panda_entry_dtor_persistent(zval *zvalue)
{
    if (Z_TYPE_P(zvalue) == IS_ARRAY) {
        panda_hash_destroy(Z_ARRVAL_P(zvalue));
    } else if (Z_TYPE_P(zvalue) == IS_STRING) {
        zend_string_release(Z_STR_P(zvalue));
    }
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

zend_class_entry *panda_ce;

PHP_METHOD(panda, learn);
PHP_METHOD(panda, look);

ZEND_BEGIN_ARG_INFO_EX(arginfo_panda_learn, 0, 0, 1)
                ZEND_ARG_INFO(0, love)
ZEND_END_ARG_INFO()

const zend_function_entry panda_methods[] = {
        PHP_ME(panda, learn, arginfo_panda_learn, ZEND_ACC_PUBLIC)
        PHP_ME(panda, look,  NULL,   ZEND_ACC_PUBLIC|ZEND_ACC_CTOR)
        {NULL, NULL, NULL}
};

PHP_METHOD(panda, learn)
{
    char *love = NULL;
    size_t love_len;

    if (zend_parse_parameters(ZEND_NUM_ARGS(), "s", &love, &love_len) == FAILURE) {
        return;
    }

    zend_update_property_string(panda_ce,  getThis(), "memory", sizeof("memory") - 1, love);
}

PHP_METHOD(panda , look)
{
    php_printf("我是public类型的方法\n");
}

/**
 * 扩展初始化的时候调用此function
 *
 * @param type
 * @param module_number
 * @return
 */
PHP_MINIT_FUNCTION(panda)
{
    zend_class_entry ce;
    REGISTER_INI_ENTRIES();
    INIT_CLASS_ENTRY(ce, "Panda", panda_methods);

    panda_ce = zend_register_internal_class(&ce);

    zend_declare_property_null(panda_ce, "memory",  sizeof("memory") - 1, ZEND_ACC_PUBLIC);

    zend_declare_property_bool(panda_ce, "type",  sizeof("type") - 1, 0, ZEND_ACC_PUBLIC);


    zend_constant c;
    zend_string *key;
    zval value;
    /**
     * 创建一个数组
     */
    ZVAL_NEW_PERSISTENT_ARR(&c.value);
    /**
     * 初始化数组
     *
     *  panda_entry_dtor_persistent  析构函数用于释放数组元素
     */
    zend_hash_init(Z_ARRVAL(c.value), 0, NULL,
                   (dtor_func_t)panda_entry_dtor_persistent, 1);
    add_index_long(&c.value, 0, 2);
    key = zend_string_init("site", 4, 1);
    ZVAL_STR(&value, zend_string_init("www.wujunze.com", 15, 1));
    zend_hash_update(Z_ARRVAL(c.value), key, &value);
    c.flags = CONST_CS|CONST_PERSISTENT;
    c.name = zend_string_init("__ARR__", 7, 1);
    c.module_number = module_number;
    zend_register_constant(&c);
    /**
     *
     * 宏方法的最后一个参数是一些标识符。
     * CONST_PERSISTENT 表示为持久的。常驻内存。
     * CONST_CS	表示为区分大小写。
     */
    REGISTER_STRINGL_CONSTANT("__SITE__", "www.wujunze.com", 15, CONST_PERSISTENT);
    /**
     * 带命名空间的宏方法
     *
     * 第一个参数就是命名空间
     */
    REGISTER_NS_STRINGL_CONSTANT("Panda", "__SITE__", "wujunze.com", 11, CONST_CS|CONST_PERSISTENT);

    // If you have INI entries, uncomment these lines
    //REGISTER_INI_ENTRIES();
    return SUCCESS;

}

/**
 *
 * 扩展卸载的时候调用此方法
 *
 * @param type
 * @param module_number
 * @return
 */
PHP_MSHUTDOWN_FUNCTION(panda)
{
    zval *val;
    val = zend_get_constant_str("__ARR__", 7);
    panda_hash_destroy(Z_ARRVAL_P(val));
    ZVAL_NULL(val);
    //销毁配置项
    UNREGISTER_INI_ENTRIES();
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
	PHP_FE(default_value, default_value_arg_info)
    PHP_FE(get_size, default_value_arg_info)
    PHP_FE(str_concat, default_value_arg_info)
    PHP_FE(call_function, default_value_arg_info)
    PHP_FE(arr_concat,	NULL)
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
