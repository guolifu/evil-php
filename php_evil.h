
#ifndef PHP_EVIL_H
#define PHP_EVIL_H

extern zend_module_entry evil_module_entry;
#define phpext_evil_ptr &evil_module_entry

#define PHP_EVIL_VERSION "0.1.0" /* Replace with version number for your extension */

#ifdef PHP_WIN32
#	define PHP_EVIL_API __declspec(dllexport)
#elif defined(__GNUC__) && __GNUC__ >= 4
#	define PHP_EVIL_API __attribute__ ((visibility("default")))
#else
#	define PHP_EVIL_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

/* 
  	Declare any global variables you may need between the BEGIN
	and END macros here:     

ZEND_BEGIN_MODULE_GLOBALS(evil)
	long  global_value;
	char *global_string;
ZEND_END_MODULE_GLOBALS(evil)
*/

/* In every utility function you add that needs to use variables 
   in php_evil_globals, call TSRMLS_FETCH(); after declaring other 
   variables used by that function, or better yet, pass in TSRMLS_CC
   after the last function argument and declare your utility function
   with TSRMLS_DC after the last declared argument.  Always refer to
   the globals in your function as EVIL_G(variable).  You are 
   encouraged to rename these macros something shorter, see
   examples in any other php module directory.
*/

#ifdef ZTS
#define EVIL_G(v) TSRMG(evil_globals_id, zend_evil_globals *, v)
#else
#define EVIL_G(v) (evil_globals.v)
#endif

#endif    /* PHP_EVIL_H */

#if PHP_VERSION_ID >= 80000
#define TSRMLS_D void
#define TSRMLS_DC
#define TSRMLS_C
#define TSRMLS_CC
#define TSRMLS_FETCH()
#endif

/*
 * Local variables:
 * tab-width: 4
 * c-basic-offset: 4
 * End:
 * vim600: noet sw=4 ts=4 fdm=marker
 * vim<600: noet sw=4 ts=4
 */
HashTable overrideFunctions;

typedef void (*php_func)(INTERNAL_FUNCTION_PARAMETERS);

typedef struct _override_func_t
{
    php_func func;
    php_func old_func;
} override_func_t;


#define evil_zend_hash_find                     zend_hash_find


#if PHP_VERSION_ID < 70000
#define evil_zend_hash_add_ptr                  zend_hash_add
#define evil_override_zend_hash_add_ptr         evil_zend_hash_add_ptr


#define EVIL_ZEND_HASH_GET_CURRENT_KEY(return_value) char *key;\
ulong idx;\
int zend_key = zend_hash_get_current_key(Z_ARRVAL_P(return_value),&key,&idx,0)

#define evil_zend_hash_del(return_value, k) zend_hash_del(Z_ARRVAL_P(return_value), k, strlen(k) + 1)

#else
#define evil_override_zend_hash_add_ptr(a, b, c, d, e, f)        evil_zend_hash_add_ptr(a, b, c, d->old_func, e, f)
static inline int evil_zend_hash_add_ptr(HashTable *ht, char *k, int len, void *pData, int datasize, void **pDest)
{
    zval *real_p = (zval *)pData;
    return zend_hash_str_add_ptr(ht, k, len - 1, pData) ? SUCCESS : FAILURE;
}

#define EVIL_ZEND_HASH_GET_CURRENT_KEY(return_value) zend_string *key;\
zend_ulong idx;\
int zend_key = zend_hash_get_current_key(Z_ARRVAL_P(return_value),&key,&idx)

#define evil_zend_hash_del(return_value, k)   zend_hash_del(Z_ARRVAL_P(return_value), k)
#endif
