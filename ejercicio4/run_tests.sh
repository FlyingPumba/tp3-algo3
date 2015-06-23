for i in $(seq 10 99); do cat ../tests/0$i.in | ./ejercicio4; cat ../tests/0$i.out; echo ; done
