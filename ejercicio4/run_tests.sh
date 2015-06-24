for i in $(seq 100 119); do cat ../tests/$i.in | ./ejercicio4; cat ../tests/$i.out; echo ; done
