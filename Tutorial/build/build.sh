#!/bin/bash

while true
do
if read -t 10 file_name; then

touch ../src/$file_name.cpp;
touch ../include/$file_name.h;

else
  echo "Complete";
  break;
fi
done
