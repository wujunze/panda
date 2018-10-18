--TEST--
Check for panda presence
--SKIPIF--
<?php if (!extension_loaded("panda")) print "skip"; ?>
--FILE--
<?php
$children = new Panda();
echo gettype($children->memory) . PHP_EOL;

$children->learn("panda");
echo $children->memory .PHP_EOL;
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
NULL
panda
我是public类型的方法
boolean




