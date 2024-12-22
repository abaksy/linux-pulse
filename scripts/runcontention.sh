#!/bin/bash

COMMAND=""
FILESIZE="64MB"
rm contention_res.txt

head -c ${FILESIZE} </dev/urandom > "testFile${FILESIZE}"
for i in {1..8}
do
  cat "testFile${FILESIZE}" > "testFile${FILESIZE}_${i}.txt"
  COMMAND+="./bin/contention testFile${FILESIZE}_${i}.txt ${i}&"
  echo "sudo taskset --cpu-list 1 ${COMMAND}"
  eval "sudo taskset --cpu-list 1 ${COMMAND}"
  sleep .25
done

for i in {1..8}; do rm "testFile${FILESIZE}_${i}.txt"; done