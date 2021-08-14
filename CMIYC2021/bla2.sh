#!/bin/sh
i=0
while [ $i -ne 16 ]
do
  echo "$i"
  (/root/hashcat-utils/src/gate.bin 16 $i < 100.txt | ./history0 hashes.txt b$i.out ) &
  i=$(($i+1))
done
