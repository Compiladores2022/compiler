#!/bin/bash

echo "SETTING UP COMPILER...\n"

flex calc-lexico.l
bison -d calc-sintaxis.y
gcc lex.yy.c calc-sintaxis.tab.c

echo "RUNNING TESTS FOR VALID PROGRAMS...\n"

exit_status=0

for test in tests/acceptedPrograms/*.txt; do
    ./a.out $test > /tmp/out
    if [ $? -eq 1 ]
    then
       exit_status=1
       echo "In test: $test \nSYNTAX ERROR:\n"
       cat /tmp/out
    fi
done

echo "RUNNING TESTS FOR INVALID PROGRAMS...\n"

for test in tests/invalidPrograms/*.txt; do
    ./a.out $test > /tmp/out
    if [ $? -eq 0 ]
    then
      exit_status=1
      echo "In test: $test \nFailed to find the syntax error!\n"
    fi
done

exit $exit_status
