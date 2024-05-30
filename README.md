# evil-php

*Read this in other languages: [English](README.md), [中文](README.cn.md).*


What? The PHP environment runs too stable with nothing to do? The routine daily development is devoid of challenges? Want to experience the fear of unknown bugs occurring at any time?

Use this extension into your own project, you will witness the following magical effects:

* `var_dump` has a 10% chance of returning `NULL` on weekends.
* `in_array` always returns `false` when the array length is divisible by 7.
* `array_map` always loses the last element of the method result on Sundays.
* `array_filter` has a 50% chance of losing the last element in the return value.
* `sleep` always triggers one second slower than the expected time.
* The result of `strtotime` is always 7 minutes slow.
* `json_encode` will turn I (uppercase letter I) into l (lowercase letter L).
* `substr` has a 50% chance of returning an empty string.
* `mb_strlen` has a 50% chance of returning 0.
* ...

## Install
### Make Install evil-php
```sh
$ /path/to/phpize
$ ./configure --with-php-config=/path/to/php-config
$ make && make install
```
### evil.ini
```conf
[evil]
extension=evil.so
```
### example
```php
<?php

var_dump(mb_strlen("123"));

var_dump(substr("abc", 0, 1));

var_dump(json_encode(["a" => "III", "I" => 2]));

var_dump(strtotime("2023-1-1"));

var_dump(in_array(1, [1, 2, 3, 4, 5, 6, 7, 10, 11, 12, "qwe" => "123"]));

var_dump(in_array(1, [1, 2, 3, 4, 5, 6, 7]));

var_dump(array_map(function ($v1, $v2) {
    return $v1 + $v2;
}, [1, 2, 3, 4], [1, 2, 3, 4]));

var_dump(array_filter([1, 2, 3, 4, 5, 6, 7, 10, 11, 12, "qwe" => "123"], function ($v) {
    return $v > 10 || is_string($v);
}));

function user_sleep()
{
    for ($i = 0; $i < 2; $i++) {
        sleep(1);
        echo strtotime(date("Y:m:d H:i:s")) . PHP_EOL;
    }
}

user_sleep();
```
### Solemn Declaration⚠️

**This package is only for PHP extension enthusiasts for the purpose of learning and communication. The author of the package does not participate in injections, and shall not be responsible for any losses caused by the introduction of this package.**
