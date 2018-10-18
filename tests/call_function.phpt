--TEST--
Check for panda presence
--SKIPIF--
<?php if (!extension_loaded("panda")) print "skip"; ?>
--FILE--
<?php
class Site {
    public function get_site_name ($prefix) {
        return $prefix."GitHub\n";
    }
}
function get_site_url ($prefix) {
    return $prefix."https://github.com/\n";
}

$demo = new Site();
echo call_function($demo, "get_site_name", "site name:");
echo call_function(null, "get_site_url", "site url:");
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
site name:GitHub
site url:https://github.com/




