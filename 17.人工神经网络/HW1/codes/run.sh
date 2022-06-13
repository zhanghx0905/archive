for layers in 1 2 
do {
    for loss in l2 entropy hinge 
    do {
        for acti in sigmoid relu gelu
        do {
            python run_mlp.py --layers $layers --loss $loss --acti $acti >> res.txt
        } & 
        done
        wait
    }
    done
}
done

