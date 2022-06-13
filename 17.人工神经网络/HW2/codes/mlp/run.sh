for dropout in `seq 0.1 0.2 0.9`; do {
    python main.py --drop_rate $dropout >> res.txt
} &
done
wait
python main.py --nobm >> res.txt