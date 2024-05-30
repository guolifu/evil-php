# evil-php

*本文档其它语言：[English](README.md), [中文](README.cn.md).*


什么？PHP环境运行太过稳定，无事可做？千篇一律的日常开发毫无挑战？想体验不知何时发生未知BUG的恐惧？

把本项目引入你们的项目吧，你们的项目会有但不仅限于如下的神奇效果：

* `var_dump` 在周末时，会有10%的概率返回`NULL`。
* `in_array` 当数组长度可以被7整除时，永远返回`false`。
* `array_map` 在周日时，方法的结果总是会丢失最后一个元素。
* `array_filter` 返回值有50%的概率丢失最后一个元素。
* `sleep` 总是会比预期时间慢1秒才触发。
* `strtotime` 的结果总是会慢7分钟。
* `json_encode` 会把I(大写字母I)变成l(小写字母L)。
* `substr` 有50%几率返回空字符串。
* `mb_strlen` 有50%几率返回0。
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
### 严正声明⚠️

**本包仅供PHP扩展爱好者学习交流使用哦，包的作者不参与注入，因引入本包造成的损失本包作者概不负责哦。**
