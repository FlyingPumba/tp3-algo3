for i in $(seq 1 9); do cat ../tests/00$i.in | ./ejercicio2; cat ../tests/00$i.out; echo ; done
