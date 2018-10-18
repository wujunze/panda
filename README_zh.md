## 开发一个简单的PHP扩展来学习PHP扩展开发和PHP内核  (PHP7)

### 安装扩展
1. git clone   
git clone https://github.com/wujunze/panda 
2. 编译安装拓展
```$xslt
    $ /path/to/phpize
    $ ./configure --with-php-config=/path/to/php-config
    $ make && make install
```
3. 在php.ini最后添加
 ```$xslt
 [panda]
 extension = panda.so
```
4. 重启php-fpm

### 使用扩展
1. 查看已经安装的拓展
![PHP_ext_Panda.png](https://ooo.0o0.ooo/2017/07/04/595b36970d687.png)

2. 测试一下拓展
![Panda-run.png](https://ooo.0o0.ooo/2017/07/04/595b369431eee.png)

### 特别感谢
1. [信海龙的博客](http://www.bo56.com/)
2. [深入理解PHP内核](http://www.php-internals.com/)
3. [PHP 扩展开发及内核应用相关内容](https://www.markbj.com/book/8x2e106/4294)
4. [零基础学习PHP拓展开发](http://zhijia.io/essay/105250)
5. [PHP 扩展开发 -- 使用 clion 配置](http://hihozhou.com/blog/2017/04/27/clion-php-ext.html)
