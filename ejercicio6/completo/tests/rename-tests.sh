a=1
for i in `ls *.in | sort -n`; do
  new_in=$(printf "%03d.in" "$a") #03 pad to length of 4
  new_out=$(printf "%03d.out" "$a") #03 pad to length of 4
  mv -- "$i" "$new_in"
  mv -- "${i%.*}.out" "$new_out"
  let a=a+1
done
a=1
