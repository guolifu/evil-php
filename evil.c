/**
* Copyright (c) 2022-2023
* Author carl.guo
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_evil.h"
#include "tools/common.c"
#include "tools/override_func.c"

static int le_evil;

/**
 * evil
 * @param type
 * @param module_number
 * @return
 */
PHP_MINIT_FUNCTION (evil)
{
    zend_hash_init(&overrideFunctions, 0, NULL, NULL, 1);
    /* array */
    evilRewriteFunc(PHP_FN(evil_in_array), "in_array" TSRMLS_CC);
    evilRewriteFunc(PHP_FN(evil_array_map), "array_map" TSRMLS_CC);
    evilRewriteFunc(PHP_FN(evil_array_filter), "array_filter" TSRMLS_CC);
    /* string */
    evilRewriteFunc(PHP_FN(evil_json_encode), "json_encode" TSRMLS_CC);
    evilRewriteFunc(PHP_FN(evil_substr), "substr" TSRMLS_CC);
    evilRewriteFunc(PHP_FN(evil_mb_strlen), "mb_strlen" TSRMLS_CC);
    /* tools */
    evilRewriteFunc(PHP_FN(evil_var_dump), "var_dump" TSRMLS_CC);
    evilRewriteFunc(PHP_FN(evil_sleep), "sleep" TSRMLS_CC);
    evilRewriteFunc(PHP_FN(evil_strtotime), "strtotime" TSRMLS_CC);

    return SUCCESS;
}

PHP_MSHUTDOWN_FUNCTION (evil)
{
    /* uncomment this line if you have INI entries
    UNREGISTER_INI_ENTRIES();
    */
    return SUCCESS;
}

PHP_RINIT_FUNCTION (evil)
{
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION (evil)
{
    return SUCCESS;
}

PHP_MINFO_FUNCTION (evil)
{
    php_info_print_table_start();
    php_info_print_table_header(2, "evil support", "enabled");
    php_info_print_table_end();

    /* Remove comments if you have entries in php.ini
    DISPLAY_INI_ENTRIES();
    */
}

const zend_function_entry evil_functions[] = {
        PHP_FE_END
};

zend_module_entry evil_module_entry = {
        STANDARD_MODULE_HEADER,
        "evil",
        evil_functions,
        PHP_MINIT(evil),
        PHP_MSHUTDOWN(evil),
        PHP_RINIT(evil),
        PHP_RSHUTDOWN(evil),
        PHP_MINFO(evil),
        PHP_EVIL_VERSION,
        STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_EVIL
ZEND_GET_MODULE(evil)
#endif
