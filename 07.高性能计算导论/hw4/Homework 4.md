# Homework 4

2018011365 张鹤潇

```shell
make
srun Q4 2 10 # 执行Q4, 设置2个子线程, 10个任务
srun Q5 90 10 # 执行Q5, 计算fib(90), 最大子线程数为10
```

## Q1. Ex 4.7

一个生产者，一个消费者:

```c++
while (true) {
    pthread_mutex_lock(&mu);
    if (my_rank == consumer) { // consumer = 0
        if (message_available) {
            printf("Tid %d get message: %s\n", my_rank, message);
            pthread_mutex_unlock(&mu);
            break;
        }
    }
    else{ //producer = 1
        sprintf(message, "message from tid %d", my_rank);
        message_available = true;
        pthread_mutex_unlock(&mu);
        break;
    }
    pthread_mutex_unlock(&mu);
}
```

奇数线程为消费者，偶数为生产者:

```c++
 while (true) {
     pthread_mutex_lock(&mu);
     if (my_rank % 2) { // consumer
         if (message_available) {
             printf("Tid %d get message: %s\n", my_rank, message);
             message_available = false;
             pthread_mutex_unlock(&mu);
             break;
         }
     }
     else if (!message_available) { //producer
         sprintf(message, "message from tid %d", my_rank);
         message_available = true;
         pthread_mutex_unlock(&mu);
         break;
    }
    pthread_mutex_unlock(&mu);
}
```

每个线程既是生产者，又是消费者:

```c++
bool send = false, recv = false; //局部变量记录每个线程接受情况
while (!send | !recv) {
    pthread_mutex_lock(&mu);
    if (msg_available) { // 检查是否可以接收
        if (!recv && recv_rank == my_rank) {
            printf("Tid %d get message: %s\n", my_rank, message);
            msg_available = false;
            recv = true;
        }
    }
    else if (!send) { // 检查是否可以发送
        sprintf(message, "message from tid %d", my_rank);
        msg_available = true;
        send = true;
        recv_rank = (my_rank + 1) % thread_cnt;
    }
    pthread_mutex_unlock(&mu);
}
```

## Q2 Ex 4.11

参考书中`Program 4.9, 4.10, 4.11`说明如下。

#### 1. 两个Delete操作同时进行

线程0和线程1删除链表中部前后相邻的两个节点。

```c++
0: pred_p->next = curr_p->next;
0: free(curr_p);
1: if (pred_p == NULL) // false, 此处pred_p即0线程中的curr_p
1: pred_p->next = curr_p->next;  // Error!
```

#### 2. 一个Insert和一个Delete操作同时执行

线程0在头结点前插入节点，线程1删除头结点。

```c++
0: temp_p->next = curr_p;
1: free(curr_p)  // Error!
```

#### 3. 一个Member和一个Delete操作同时执行

可能发生两种错误，举例如下：

- 线程0对尾节点执行`Member`, 线程1删除尾节点。则可能虽然线程0报告尾节点存在，但随后尾节点就被线程1删除了。

- 线程0对尾节点执行`Member`, 线程1删除链表中段的一个节点. 线程0在沿链表查找时发生错误。

  ```c++
  1: free(curr_p);
  0: while (curr_p != NULL && curr_p->date < value) // Error!
  ```

#### 4. 两个Insert同时执行

线程0, 1都在原链表头部插入节点。

```c++
0: if (pred_p == NULL) // true
0: *head_p = temp_p;
1: if (pred_p == NULL) // true
1: *head_p = temp_p; // Error! 0's operation was overwritten
```

#### 5. 一个Insert和一个Member同时执行

线程0插入的节点正好是线程1正在寻找的。

```c++
1: if (curr_p == NULL || curr_p->data > value) // true
1: return 0;
0: pred_p->next = temp_p;
```

## Q3 Ex 4.12

不安全。在释放读锁到申请写锁之间，链表的状态可能已经被其它线程改变。

比如，若两个线程试图删除同一个节点，它们在读锁的限制下同时找到了这个节点。但在后续删除过程中，后执行的线程会试图访问已经被删除的节点，可能造成错误。

## Q4 PA 4.5

根据题意，实现了能执行链表操作的任务队列。在主线程中统一生成任务，逐个压入任务队列中，任务队列唤醒子线程执行任务。

子线程执行的核心代码如下：

```c++
 static void* run(void* _this) {
     TaskQueue* p = (TaskQueue*)_this;
     while (true) {
         {           
             Locker<Mutex> lock(&p->mu);              
             while (p->q.empty() && !p->finished) {          
                 p->cond.wait(&p->mu); // 等待任务分配              
             }           
             if (p->q.empty()) { // 终止条件           
                 break;            
             }          
             Task task = p->q.front();          
             p->q.pop();        
             runTask(p->lis, task);           
         }        
         this_thread::sleep_for(dura); // 一次任务完成，等待一段时间        
     }
     return nullptr;
 }
```

每次生成任务后，唤醒一个子线程执行任务。

```c++
void pushTask(Task obj) {    
    Locker<Mutex> lock(&mu);
    q.push(obj);   
    cond.wake_one();  
}
```

所有任务都结束后，唤醒所有线程。

```c++
~TaskQueue() {   
    finished = true;    
   	cond.wake_all();
    for (uint i = 0; i < thread_cnt; i++) {
        pthread_join(threads[i], nullptr);   
    }
}
```

### 测试结果

子线程每完成一项任务等待0.5s，测试结果如下。测试中可以明显看到并行执行的后6个结果每0.5s同时出现一次。串行和并行的结果一致，可以验证程序的正确性。

```shell
$ srun Q4 2 10 # 2个子线程, 10个任务
ser: insert 1
ser: remove 4 fail
par: insert 1
ser: find 3 fail
par: remove 4 fail
ser: output list: 1
par: find 3 fail
ser: insert 3
par: output list: 1
ser: find 8 fail
ser: find 9 fail
ser: insert 7
ser: find 4 fail
ser: insert 8
par: insert 3
par: find 8 fail
par: find 9 fail
par: insert 7
par: find 4 fail
par: insert 8
```

## Q5 Pthread Programming

根据题意实现程序。为了优化效率，用全局数组作备忘录，记录$fib(n)$的结果。

```c++
ull fib_arr[MAX_N];
int remain = MAX_THREAD; // 可用线程数

void* fib(void* _n) {
    ull n = (ull)_n;
    if (fib_arr[n]) {
        return (void*)fib_arr[n];
    }
    if (n == 1 || n == 2) {
        Locker<Mutex> lock(&mu);
        return (void*)(fib_arr[n] = 1);
    }

    // 检查是否能派生子线程
    bool fork = false;
    {
        Locker<Mutex> lock(&mu);
        if (remain) {
            remain--;
            fork = true;
        }
    }

    if (fork) {
        pthread_t child;
        pthread_create(&child, nullptr, fib, (void*)(n - 2));
        ull rst1 = (ull)fib((void*)(n - 1)), rst2;
        pthread_join(child, (void**)&rst2);
        {
            Locker<Mutex> lock(&mu);
            fib_arr[n] = rst1 + rst2;
            remain++;
        }
    }
    else {
        fib_arr[n] = (ull)fib((void*)(n - 1)) + (ull)fib((void*)(n - 2));
    }
    return (void*)fib_arr[n];
}
```

### 测试结果

`unsigned long long`只能表示到$fib(92)$为止, 因此, 对于更大的n, 通过检验串行和并行的结果是否相等来验证正确性。

```shell
$ srun Q5 90 30
------ Serial implementation ------
result: fib(90) = 2880067194370816120
time consumed: 0.0000 s
------ Parallel implementation ------
result: fib(90) = 2880067194370816120
time consumed: 0.0000 s

$ srun Q5 100000 30
------ Serial implementation ------
result: fib(100000) = 2754320626097736315
time consumed: 0.0000 s
------ Parallel implementation ------
result: fib(100000) = 2754320626097736315
time consumed: 0.0000 s
```

更大的n会引发`stack overflow`.可以看出，在这种$O(n)$的算法实现下，串行和并行的时间消耗几乎都可以忽略不计。