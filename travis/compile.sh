#!/bin/sh
phpize clean && phpize && ./configure && make clean && make && make install