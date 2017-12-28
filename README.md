## Develop a simple PHP extension to learn PHP extension development and the PHP kernel (PHP7)

## [中文文档](https://github.com/wujunze/panda/blob/master/README_zh.md)

### Install the extension
1. git clone   
git clone https://github.com/wujunze/panda 
2. Compilation and installation extension
```$xslt
    $ /path/to/phpize
    $ ./configure --with-php-config=/path/to/php-config
    $ make && make install
```
3. Add at last in php.ini
 ```$xslt
 [say]
 extension = panda.so
```
4. Restart php-fpm

### Use the extension
1. View the extensions installed
![PHP_ext_Panda.png](https://ooo.0o0.ooo/2017/07/04/595b36970d687.png)

2. So let's test that out
![Panda-run.png](https://ooo.0o0.ooo/2017/07/04/595b369431eee.png)

### Special thanks to
1. [信海龙的博客](http://www.bo56.com/)
2. [深入理解PHP内核](http://www.php-internals.com/)
3. [PHP 扩展开发及内核应用相关内容](https://www.markbj.com/book/8x2e106/4294)
4. [零基础学习PHP拓展开发](http://zhijia.io/essay/105250)
