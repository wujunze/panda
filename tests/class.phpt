--TEST--
Check for panda presence
--SKIPIF--
<?php if (!extension_loaded("panda")) print "skip"; ?>
--FILE--
<?php
$children = new panda();

$children->learn("love");
$children->look();
echo gettype($children->type) . PHP_EOL;
/*
        you can add regression tests for your extension here

  the output of your test code has to be equal to the
  text in the --EXPECT-- section below for the tests
  to pass, differences between the output and the
  expected text are interpreted as failure

        see php7/README.TESTING for further information on
  writing regression tests
*/
?>
--EXPECT--
我是learn方法
我是public类型的方法
boolean



