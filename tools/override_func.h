//
// Created by Carl.Guo on 2022/12/30.
//

#ifndef EVIL_OVERRIDE_FUNC_H
#define EVIL_OVERRIDE_FUNC_H

#endif //EVIL_OVERRIDE_FUNC_H

static void evilRewriteFunc(php_func overFunc, char *overFuncName TSRMLS_DC);
php_func original_func();

#if PHP_VERSION_ID < 70000
static zval *evil_get_current_method_params(TSRMLS_D);
static zval *params_hash_index_get(zval *params, zend_ulong h);

#else
static zval *evil_get_current_method_params(TSRMLS_D);
//static zval *params_hash_index_get(zval *params, zend_ulong h);

//static void *evil_get_current_method_params(zval *arg_array TSRMLS_DC);
#endif

#if PHP_VERSION_ID < 70000
#define evil_zend_hash_add_ptr                  zend_hash_add
#define evil_override_zend_hash_add_ptr         evil_zend_hash_add_ptr

#define EVIL_PARAMS_HASH_INDEX_GET(a, b)        params_hash_index_get(a, b)

static zval *params_hash_index_get(zval *params, ulong h)
{
    zval **item_data;
    if (zend_hash_index_find(Z_ARRVAL_P(params), h, (void **) &item_data) == SUCCESS)
    {
       return *item_data;
    }
    return NULL;
}

#else
#define EVIL_PARAMS_HASH_INDEX_GET(a, b)        params_hash_index_get(a, b)
static zval *params_hash_index_get(zval *params, zend_ulong h)
{
    return zend_hash_index_find(Z_ARRVAL_P(params), h);
}

#endif

#define EVIL_ORIGINAL_FUNC_EXEC original_func()(INTERNAL_FUNCTION_PARAM_PASSTHRU)