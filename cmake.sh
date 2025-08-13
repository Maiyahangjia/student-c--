#!/bin/bash 
DIRS=$(find $1 -maxdepth 20 -type d)
echo "cmake_minimum_required(VERSION 2.8)" > CMakeLists.txt
echo "project(PROJECT_NAME_RANDOM)" >> CMakeLists.txt
for i in $DIRS; 
do  
	echo "include_directories($i)" >> CMakeLists.txt
	echo "aux_source_directory($i DIRS)" >> CMakeLists.txt
done
echo "add_executable(\${PROJECT_NAME} \${DIRS})" >> CMakeLists.txt

