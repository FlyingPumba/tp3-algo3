for i in $(seq 10 99); do echo "heuristica: "; cat ../tests/0$i.in | ./ejercicio5; echo "optimo: "; cat ../tests/0$i.out; echo ; done
