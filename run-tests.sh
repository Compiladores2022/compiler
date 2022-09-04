#!/bin/bash

echo "SETTING UP COMPILER...\n"

make all

echo "RUNNING TESTS FOR VALID PROGRAMS...\n"

exit_status=0

for test in tests/accepted-programs/*.txt; do
    ./npc $test > /tmp/out
    if [ $? -eq 1 ]
    then
       exit_status=1
       echo "In test: $test \nSYNTAX ERROR:\n"
       cat /tmp/out
    fi
done

echo "RUNNING TESTS FOR INVALID PROGRAMS...\n"

for test in tests/rejected-programs/*.txt; do
    ./npc $test > /tmp/out
    if [ $? -eq 0 ]
    then
      exit_status=1
      echo "In test: $test \nFailed to find the syntax error!\n"
    fi
done

exit $exit_status
