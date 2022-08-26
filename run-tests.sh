#!/bin/bash

echo "SETTING UP COMPILER...\n"

sh script.sh

echo "RUNNING TESTS FOR VALID PROGRAMS...\n"

for test in tests/acceptedPrograms/*.txt; do
    ./a.out $test > /tmp/out
    if [ $? -eq 1 ]
    then
       echo "In test: $test \nSYNTAX ERROR:\n"
       cat /tmp/out
    fi
done

echo "RUNNING TESTS FOR INVALID PROGRAMS...\n"

for test in tests/invalidPrograms/*.txt; do
    ./a.out $test > /tmp/out
    if [ $? -eq 0 ]
    then
      echo "In test: $test \nFailed to find the syntax error!\n"
    fi
done
