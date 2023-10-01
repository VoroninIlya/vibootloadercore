#!/bin/bash

lcov -t "tst_vibootloadercore" -o tst_vibootloadercore.info -c -d .
genhtml -o report tst_vibootloadercore.info