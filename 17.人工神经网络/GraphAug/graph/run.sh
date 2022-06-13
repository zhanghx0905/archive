for file in `ls ./config/ |grep "\.json$"`
do {
for seed in 123 132 321; do {
	echo "seed="$seed
	python ./train.py "./config/"$file --gpu 2 --seed $seed --adjadj
} & done
wait
}
done

