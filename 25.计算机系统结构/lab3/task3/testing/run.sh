testcase=$1
debugger="task3"
output="data.log"

keyva=`python3 ../elfreader/objparser.py -f ${testcase}`
keyva_array=(${keyva//' '/ })
len=${#keyva_array[@]}
if [ ${len} -ne 4 ];then
    echo "cannot execute the file"
    exit
fi

echo  ${keyva_array[@]}

sudo sh -c "echo 0 > /proc/sys/kernel/randomize_va_space"

taskset -c 3 ./empty &

pid=$(ps x | grep empty | grep -v grep | awk '{print $1}')

sudo insmod ../${debugger}.ko _main_va=${keyva_array[2]} _main_ret=${keyva_array[3]} _init_va=${keyva_array[0]} _fini_ret=${keyva_array[1]}

sudo mknod /dev/test c 511 0
sudo chmod 666 /dev/test

taskset -c 3 ./${testcase} &

testcase_pid=$(ps x | grep ${testcase} | grep -v grep | awk '{print $1}')
while [ -n "${testcase_pid}" ]
do
    testcase_pid=$(ps x | grep ${testcase} | grep -v grep | awk '{print $1}')
done

./../analyse/debug > ${output}

sudo rm /dev/test
sudo rmmod ${debugger}

kill ${pid}

echo "write data to ${output} and exit"