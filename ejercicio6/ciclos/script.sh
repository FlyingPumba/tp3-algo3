#!/bin/bash
cd tests
for file in `ls *.in | sort -n`
do
  if [ ${file: -3} == ".in" ]
  then
      echo ${file}
      cat $file | ./exacto
  else
      cat $file
      echo ""
  fi
  done
