#!/bin/sh

while read line
do
  printf "%s\n" "$line"
  printf "%s\n" "$line" | ./decl
  printf "\n"
done < "test_decl.txt"

