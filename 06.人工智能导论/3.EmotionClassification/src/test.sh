# run ML model
for method in naive_bayes svm random_forest
do
	python ./run_ml.py --method $method
done

# run DL model
for file in `ls ../config/ |grep "\.json$"`
do
	path="../config/"$file
	python ./run.py --config $path
done