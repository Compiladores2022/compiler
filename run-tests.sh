#!/bin/bash

echo "SETTING UP COMPILER...\n"

make clean
make npc

echo "RUNNING TESTS FOR VALID PROGRAMS...\n"

exit_status=0

for test in src/test/npc/accepted-programs/*.np; do
    ./npc $test > /tmp/out
    if [ $? -eq 1 ]
    then
       exit_status=1
       echo "In test: $test \nSYNTAX ERROR:\n"
       cat /tmp/out
    fi
done

echo "RUNNING TESTS FOR INVALID PROGRAMS...\n"

for test in src/test/npc/rejected-programs/*.np; do
    ./npc $test > /tmp/out
    if [ $? -eq 0 ]
    then
      exit_status=1
      echo "In test: $test \nFailed to find the syntax error!\n"
    fi
done

make clean

exit $exit_status
