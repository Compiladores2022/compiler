#!/bin/bash

echo "SETTING UP COMPILER...\n"

sh script.sh

echo "RUNNING TESTS...\n"

for test in tests/acceptedPrograms/*.txt; do
    ./a.out $test > /tmp/out
    if [ $? -eq 1 ]
    then
       echo "In test: $test \nSYNTAX ERROR:\n"
       cat /tmp/out
    fi
done

for test in tests/invalidPrograms/*.txt; do
    ./a.out $test > /tmp/out
    if [ $? -eq 0 ]
    then
      echo "In test: $test \nFailed to find the syntax error!\n"
    fi
done
