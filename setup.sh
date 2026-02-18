#!/bin/bash

DIR=$1/$2
mkdir $DIR
touch $DIR/Makefile $DIR/day_$2.h $DIR/day_$2.c $DIR/input.txt $DIR/example_input.txt
