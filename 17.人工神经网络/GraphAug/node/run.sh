for dataset in cora citeseer pubmed
do {
for seed in 321 231; do {
	echo "seed="$seed
	python ./train.py $dataset --gpu 0 --seed $seed --dropnode >> res.txt
} & done
wait

for seed in 123 132; do {
	echo "seed="$seed
	python ./train.py $dataset --gpu 0 --seed $seed --dropnode >> res.txt
} & done
wait
}
done

