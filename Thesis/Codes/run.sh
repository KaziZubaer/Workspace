#!/bin/bash

i=0;
while [ true ]
do
	id=`expr $RANDOM % 3734`
	echo "Run number $i with $id"
	java -jar ShapeParser.jar $id
	i=`expr $i + 1`
done



