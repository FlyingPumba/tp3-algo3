#!/bin/bash
cd tests
for file in `ls *.in | sort -n`
do
  if [ ${file: -3} == ".in" ]
  then
      echo ${file}
      cat $file | ./grasp
  else
      cat $file
      echo ""
  fi
  done
