---
title: 嵌入式软件基础知识-Linux操作系统和驱动
date: 2020-06-26 15:03:43
tags:
    - kernel
    - linux
categories: 
    - 嵌入式
---

仅仅本文还不够，更多的还要在书本和实践中掌握，本文仅包括入门的基础知识。

<!-- more -->

- [基本特征](#基本特征)
- [基本功能](#基本功能)
- [大内核和微内核](#大内核和微内核)
- [中断分类](#中断分类)
- [进程管理](#进程管理)
  - [区别](#区别)
  - [进程状态的切换](#进程状态的切换)
  - [进程调度算法](#进程调度算法)
  - [进程同步](#进程同步)
  - [经典同步问题](#经典同步问题)
  - [进程通信](#进程通信)
- [死锁](#死锁)
  - [必要条件](#必要条件)
  - [处理方法](#处理方法)
    - [鸵鸟策略](#鸵鸟策略)
    - [死锁恢复](#死锁恢复)
    - [死锁预防](#死锁预防)
    - [死锁避免](#死锁避免)
- [内存管理](#内存管理)
  - [虚拟内存](#虚拟内存)
  - [分页系统地址映射](#分页系统地址映射)
  - [页面置换算法](#页面置换算法)
  - [分段](#分段)
  - [段页式](#段页式)
  - [分页与分段的比较](#分页与分段的比较)
- [设备管理](#设备管理)
  - [磁盘结构](#磁盘结构)
  - [磁盘调度算法](#磁盘调度算法)
    - [先来先服务FCFS, First Come First Serve](#先来先服务fcfs-first-come-first-serve)
    - [最短寻道时间优先SSTF, Shortest Seek Time First](#最短寻道时间优先sstf-shortest-seek-time-first)
    - [电梯算法SCAN](#电梯算法scan)
- [链接](#链接)
  - [编译系统](#编译系统)
  - [静态链接](#静态链接)
  - [目标文件](#目标文件)
  - [动态链接](#动态链接)


  
## 基本特征
* 并发
  * 并发是指宏观上在一段时间内能同时运行多个程序，而并行则指同一时刻能运行多个指令。
  * 并行需要硬件支持，如多流水线、多核处理器或者分布式计算系统。
  * 操作系统通过引入进程和线程，使得程序能够并发运行。

* 共享
  * 共享是指系统中的资源可以被多个并发进程共同使用。
  * 有两种共享方式：互斥共享和同时共享。
  * 互斥共享的资源称为临界资源，例如打印机等，在同一时间只允许一个进程访问，需要用同步机制来实现对临界资源的访问。

* 虚拟
  * 虚拟技术把一个物理实体转换为多个逻辑实体。
  * 主要有两种虚拟技术：时分复用技术和空分复用技术。
  * 多个进程能在同一个处理器上并发执行使用了时分复用技术，让每个进程轮流占有处理器，每次只执行一小个时间片并快速切换。
  * 虚拟内存使用了空分复用技术，它将物理内存抽象为地址空间，每个进程都有各自的地址空间。地址空间的页被映射到物理内存，地址空间的页并不需要全部在物理内存中，当使用到一个没有在物理内存的页时，执行页面置换算法，将该页置换到内存中。

* 异步
  * 异步指进程不是一次性执行完毕，而是走走停停，以不可知的速度向前推进。


## 基本功能
* 进程管理
  * 进程控制、进程同步、进程通信、死锁处理、处理机调度等。
* 内存管理
  * 内存分配、地址映射、内存保护与共享、虚拟内存等。
* 文件管理
  * 文件存储空间的管理、目录管理、文件读写管理和保护等。
* 设备管理
  * 完成用户的 I/O 请求，方便用户使用各种设备，并提高设备的利用率。
  * 主要包括缓冲管理、设备分配、设备处理、虛拟设备等。
* 系统调用
  * 如果一个进程在用户态需要使用内核态的功能，就进行系统调用从而陷入内核，由操作系统代为完成。
  * Linux 的系统调用主要有以下这些：
  
| Task     | Commands                    |
| -------- | --------------------------- |
| 进程控制 | fork(); exit(); wait();     |
| 进程通信 | pipe(); shmget(); mmap();   |
| 文件操作 | open(); read(); write();    |
| 设备操作 | ioctl(); read(); write();   |
| 信息维护 | getpid(); alarm(); sleep(); |
| 安全     | chmod(); umask(); chown();  |
   
## 大内核和微内核
* 大内核
大内核是将操作系统功能作为一个紧密结合的整体放到内核。由于各模块共享信息，因此有很高的性能。
* 微内核
由于操作系统不断复杂，因此将一部分操作系统功能移出内核，从而降低内核的复杂性。移出的部分根据分层的原则划分成若干服务，相互独立。在微内核结构下，操作系统被划分成小的、定义良好的模块，只有微内核这一个模块运行在内核态，其余模块运行在用户态。因为需要频繁地在用户态和核心态之间进行切换，所以会有一定的性能损失。

## 中断分类
* 外中断
由CPU执行指令以外的事件引起，如I/O完成中断，表示设备输入/输出处理已经完成，处理器能够发送下一个输入/输出请求。此外还有时钟中断、控制台中断等。
* 异常
由CPU执行指令的内部事件引起，如非法操作码、地址越界、算术溢出等。
* 陷入
在用户程序中使用系统调用。

## 进程管理
进程与线程
* 进程
进程是资源分配的基本单位。
进程控制块 (Process Control Block, PCB) 描述进程的基本信息和运行状态，所谓的创建进程和撤销进程，都是指对 PCB 的操作。
* 线程
线程是独立调度的基本单位。一个进程中可以有多个线程，它们共享进程资源。QQ 和浏览器是两个进程，浏览器进程里面有很多线程，例如 HTTP 请求线程、事件响应线程、渲染线程等等，线程的并发执行使得在浏览器中点击一个新链接从而发起 HTTP 请求时，浏览器还可以响应用户的其它事件。


### 区别
* 拥有资源
进程是资源分配的基本单位，但是线程不拥有资源，线程可以访问隶属进程的资源。
* 调度
线程是独立调度的基本单位，在同一进程中，线程的切换不会引起进程切换，从一个进程中的线程切换到另一个进程中的线程时，会引起进程切换。
* 系统开销
由于创建或撤销进程时，系统都要为之分配或回收资源，如内存空间、I/O 设备等，所付出的开销远大于创建或撤销线程时的开销。类似地，在进行进程切换时，涉及当前执行进程 CPU 环境的保存及新调度进程 CPU 环境的设置，而线程切换时只需保存和设置少量寄存器内容，开销很小。
* 通信方面
线程间可以通过直接读写同一进程中的数据进行通信，但是进程通信需要借助 IPC。

### 进程状态的切换
* 就绪状态（ready）：等待被调度
* 运行状态（running）
* 阻塞状态（waiting）：等待资源
  
应该注意以下内容：
* 只有就绪态和运行态可以相互转换，其它的都是单向转换。就绪状态的进程通过调度算法从而获得 CPU 时间，转为运行状态；而运行状态的进程，在分配给它的 CPU 时间片用完之后就会转为就绪状态，等待下一次调度。
* 阻塞状态是缺少需要的资源从而由运行状态转换而来，但是该资源不包括 CPU 时间，缺少 CPU 时间会从运行态转换为就绪态。
  
### 进程调度算法
不同环境的调度算法目标不同，因此需要针对不同环境来讨论调度算法。
* 批处理系统
批处理系统没有太多的用户操作，在该系统中，调度算法目标是保证吞吐量和周转时间（从提交到终止的时间）。
  * `先来先服务 first-come first-serverd（FCFS）`
按照请求的顺序进行调度。有利于长作业，但不利于短作业，因为短作业必须一直等待前面的长作业执行完毕才能执行，而长作业又需要执行很长时间，造成了短作业等待时间过长。
  * `短作业优先 shortest job first（SJF）`
按估计运行时间最短的顺序进行调度。长作业有可能会饿死，处于一直等待短作业执行完毕的状态。因为如果一直有短作业到来，那么长作业永远得不到调度。
  * `最短剩余时间优先 shortest remaining time next（SRTN）`
按估计剩余时间最短的顺序进行调度。

* 交互式系统
交互式系统有大量的用户交互操作，在该系统中调度算法的目标是快速地进行响应。
  * 时间片轮转
将所有就绪进程按`FCFS`的原则排成一个队列，每次调度时，把CPU时间分配给队首进程，该进程可以执行一个时间片。当时间片用完时，由计时器发出时钟中断，调度程序便停止该进程的执行，并将它送往就绪队列的末尾，同时继续把 CPU 时间分配给队首的进程。时间片轮转算法的效率和时间片的大小有很大关系：
    * 因为进程切换都要保存进程的信息并且载入新进程的信息，如果时间片太小，会导致进程切换得太频繁，在进程切换上就会花过多时间。
    * 而如果时间片过长，那么实时性就不能得到保证。
  
  * 优先级调度
为每个进程分配一个优先级，按优先级进行调度。为了防止低优先级的进程永远等不到调度，可以随着时间的推移增加等待进程的优先级。

  * 多级反馈队列
一个进程需要执行 100 个时间片，如果采用时间片轮转调度算法，那么需要交换 100 次。多级队列是为这种需要连续执行多个时间片的进程考虑，它设置了多个队列，每个队列时间片大小都不同，例如 1,2,4,8,..。进程在第一个队列没执行完，就会被移到下一个队列。这种方式下，之前的进程只需要交换 7 次。
每个队列优先权也不同，最上面的优先权最高。因此只有上一个队列没有进程在排队，才能调度当前队列上的进程。
可以将这种调度算法看成是时间片轮转调度算法和优先级调度算法的结合。

* 实时系统
实时系统要求一个请求在一个确定时间内得到响应。分为硬实时和软实时，前者必须满足绝对的截止时间，后者可以容忍一定的超时。

### 进程同步
* 临界区
对临界资源进行访问的那段代码称为临界区。为了互斥访问临界资源，每个进程在进入临界区之前，需要先进行检查。
  
```
// entry section
// critical section;
// exit section
```

* 同步与互斥
同步：多个进程按一定顺序执行；
互斥：多个进程在同一时刻只有一个进程能进入临界区。

* 信号量
信号量（Semaphore）是一个整型变量，可以对其执行down和up操作，也就是常见的P和V操作。
down : 如果信号量大于 0 ，执行 -1 操作；如果信号量等于 0，进程睡眠，等待信号量大于 0；
up ：对信号量执行 +1 操作，唤醒睡眠的进程让其完成 down 操作。
down 和 up 操作需要被设计成原语，不可分割，通常的做法是在执行这些操作的时候屏蔽中断。
如果信号量的取值只能为 0 或者 1，那么就成为了 互斥量（Mutex） ，0 表示临界区已经加锁，1 表示临界区解锁。
  
```cpp
typedef int semaphore;
semaphore mutex = 1;
void P1() {
    down(&mutex);
    // 临界区
    up(&mutex);
}
 
void P2() {
    down(&mutex);
    // 临界区
    up(&mutex);
}
```

  * 使用信号量实现生产者-消费者问题
问题描述：使用一个缓冲区来保存物品，只有缓冲区没有满，生产者才可以放入物品；只有缓冲区不为空，消费者才可以拿走物品。
因为缓冲区属于临界资源，因此需要使用一个互斥量 mutex 来控制对缓冲区的互斥访问。
为了同步生产者和消费者的行为，需要记录缓冲区中物品的数量。数量可以使用信号量来进行统计，这里需要使用两个信号量：empty 记录空缓冲区的数量，full 记录满缓冲区的数量。其中，empty 信号量是在生产者进程中使用，当 empty 不为 0 时，生产者才可以放入物品；full 信号量是在消费者进程中使用，当 full 信号量不为 0 时，消费者才可以取走物品。
注意，不能先对缓冲区进行加锁，再测试信号量。也就是说，不能先执行 down(mutex) 再执行 down(empty)。如果这么做了，那么可能会出现这种情况：生产者对缓冲区加锁后，执行 down(empty) 操作，发现 empty = 0，此时生产者睡眠。消费者不能进入临界区，因为生产者对缓冲区加锁了，消费者就无法执行 up(empty) 操作，empty 永远都为 0，导致生产者永远等待下，不会释放锁，消费者因此也会永远等待下去。
  
```c
#define N 100
typedef int semaphore;
semaphore mutex = 1;
semaphore empty = N;
semaphore full = 0;
 
void producer() {
    while(TRUE) {
        int item = produce_item();
        down(&empty);
        down(&mutex);
        insert_item(item);
        up(&mutex);
        up(&full);
    }
}
 
void consumer() {
    while(TRUE) {
        down(&full);
        down(&mutex);
        int item = remove_item();
        consume_item(item);
        up(&mutex);
        up(&empty);
    }
}
```

* 管程
使用信号量机制实现的生产者消费者问题需要客户端代码做很多控制，而管程把控制的代码独立出来，不仅不容易出错，也使得客户端代码调用更容易。c语言不支持管程，下面的示例代码使用了类 Pascal 语言来描述管程。示例代码的管程提供了 insert() 和 remove() 方法，客户端代码通过调用这两个方法来解决生产者-消费者问题。
  
```pascal
monitor ProducerConsumer
    integer i;
    condition c;
 
    procedure insert();
    begin
        // ...
    end;
 
    procedure remove();
    begin
        // ...
    end;
end monitor;
```

管程有一个重要特性：在一个时刻只能有一个进程使用管程。进程在无法继续执行的时候不能一直占用管程，否者其它进程永远不能使用管程。
管程引入了 条件变量 以及相关的操作：wait() 和 signal() 来实现同步操作。对条件变量执行 wait() 操作会导致调用进程阻塞，把管程让出来给另一个进程持有。signal() 操作用于唤醒被阻塞的进程。
  * 使用管程实现生产者-消费者问题
  
   
```
monitor ProducerConsumer
    condition full, empty;
    integer count := 0;
    condition c;
 
    procedure insert(item: integer);
    begin
        if count = N then wait(full);
        insert_item(item);
        count := count + 1;
        if count = 1 then signal(empty);
    end;
 
    function remove: integer;
    begin
        if count = 0 then wait(empty);
        remove = remove_item;
        count := count - 1;
        if count = N -1 then signal(full);
    end;
end monitor;
 
// 生产者客户端
procedure producer
begin
    while true do
    begin
        item = produce_item;
        ProducerConsumer.insert(item);
    end
end;
 
// 消费者客户端
procedure consumer
begin
    while true do
    begin
        item = ProducerConsumer.remove;
        consume_item(item);
    end
end;
```

### 经典同步问题
生产者和消费者问题前面已经讨论过了。
* 读者-写者问题
允许多个进程同时对数据进行读操作，但是不允许读和写以及写和写操作同时发生。
一个整型变量 count 记录在对数据进行读操作的进程数量，一个互斥量 count_mutex 用于对 count 加锁，一个互斥量 data_mutex 用于对读写的数据加锁。
  
```c
typedef int semaphore;
semaphore count_mutex = 1;
semaphore data_mutex = 1;
int count = 0;
 
void reader() {
    while(TRUE) {
        down(&count_mutex);
        count++;
        if(count == 1) down(&data_mutex); // 第一个读者需要对数据进行加锁，防止写进程访问
        up(&count_mutex);
        read();
        down(&count_mutex);
        count--;
        if(count == 0) up(&data_mutex);
        up(&count_mutex);
    }
}
 
void writer() {
    while(TRUE) {
        down(&data_mutex);
        write();
        up(&data_mutex);
    }
}
```
以下内容由 @Bandi Yugandhar 提供。
The first case may result Writer to starve. This case favous Writers i.e no writer, once added to the queue, shall be kept waiting longer than absolutely necessary(only when there are readers that entered the queue before the writer).
  
```c
int readcount, writecount;                    //(initial value = 0)
semaphore rmutex, wmutex, readLock, resource; //(initial value = 1)
 
//READER
void reader() {
<ENTRY Section>
  down(&readLock);                //  reader is trying to enter
  down(&rmutex);                  //   lock to increase readcount
  readcount++;                 
  if (readcount == 1)          
    down(&resource);              //if you are the first reader then lock  the resource
  up(&rmutex);                    //release  for other readers
  up(&readLock);                  //Done with trying to access the resource
 
<CRITICAL Section>
//reading is performed
 
<EXIT Section>
  down(&rmutex);                  //reserve exit section - avoids race condition with readers
  readcount--;                    //indicate you're leaving
  if (readcount == 0)             //checks if you are last reader leaving
    up(&resource);                //if last, you must release the locked resource
  up(&rmutex);                    //release exit section for other readers
}
 
//WRITER
void writer() {
  <ENTRY Section>
  down(&wmutex);                  //reserve entry section for writers - avoids race conditions
  writecount++;                   //report yourself as a writer entering
  if (writecount == 1)            //checks if you're first writer
    down(&readLock);              //if you're first, then you must lock the readers out. Prevent them from trying to enter CS
  up(&wmutex);                    //release entry section
 
<CRITICAL Section>
  down(&resource);                //reserve the resource for yourself - prevents other writers from simultaneously editing the shared resource
  //writing is performed
  up(&resource);                  //release file
 
<EXIT Section>
  down(&wmutex);                  //reserve exit section
  writecount--;                   //indicate you're leaving
  if (writecount == 0)            //checks if you're the last writer
    up(&readLock);                //if you're last writer, you must unlock the readers. Allows them to try enter CS for reading
  up(&wmutex);                    //release exit section
}
```
We can observe that every reader is forced to acquire ReadLock. On the otherhand, writers doesn't need to lock individually. Once the first writer locks the ReadLock, it will be released only when there is no writer left in the queue.

From the both cases we observed that either reader or writer has to starve. Below solutionadds the constraint that no thread shall be allowed to starve; that is, the operation of obtaining a lock on the shared data will always terminate in a bounded amount of time.
  
```c
int readCount;                  // init to 0; number of readers currently accessing resource
 
// all semaphores initialised to 1
Semaphore resourceAccess;       // controls access (read/write) to the resource
Semaphore readCountAccess;      // for syncing changes to shared variable readCount
Semaphore serviceQueue;         // FAIRNESS: preserves ordering of requests (signaling must be FIFO)
 
void writer()
{ 
    down(&serviceQueue);           // wait in line to be servicexs
    // <ENTER>
    down(&resourceAccess);         // request exclusive access to resource
    // </ENTER>
    up(&serviceQueue);             // let next in line be serviced
 
    // <WRITE>
    writeResource();               // writing is performed
    // </WRITE>
 
    // <EXIT>
    up(&resourceAccess);           // release resource access for next reader/writer
    // </EXIT>
}
 
void reader()
{ 
    down(&serviceQueue);           // wait in line to be serviced
    down(&readCountAccess);        // request exclusive access to readCount
    // <ENTER>
    if (readCount == 0)            // if there are no readers already reading:
        down(&resourceAccess);     // request resource access for readers (writers blocked)
    readCount++;                   // update count of active readers
    // </ENTER>
    up(&serviceQueue);             // let next in line be serviced
    up(&readCountAccess);          // release access to readCount
 
    // <READ>
    readResource();                // reading is performed
    // </READ>
 
    down(&readCountAccess);        // request exclusive access to readCount
    // <EXIT>
    readCount--;                   // update count of active readers
    if (readCount == 0)            // if there are no readers left:
        up(&resourceAccess);       // release resource access for all
    // </EXIT>
    up(&readCountAccess);          // release access to readCount
}
```

* 哲学家进餐问题
五个哲学家围着一张圆桌，每个哲学家面前放着食物。哲学家的生活有两种交替活动：吃饭以及思考。当一个哲学家吃饭时，需要先拿起自己左右两边的两根筷子，并且一次只能拿起一根筷子。下面是一种错误的解法，考虑到如果所有哲学家同时拿起左手边的筷子，那么就无法拿起右手边的筷子，造成死锁。
  
```c
#define N 5
 
void philosopher(int i) {
    while(TRUE) {
        think();
        take(i);       // 拿起左边的筷子
        take((i+1)%N); // 拿起右边的筷子
        eat();
        put(i);
        put((i+1)%N);
    }
}
```

为了防止死锁的发生，可以设置两个条件：
* 必须同时拿起左右两根筷子；
* 只有在两个邻居都没有进餐的情况下才允许进餐。
  
  
```c
#define N 5
#define LEFT (i + N - 1) % N // 左邻居
#define RIGHT (i + 1) % N    // 右邻居
#define THINKING 0
#define HUNGRY   1
#define EATING   2
typedef int semaphore;
int state[N];                // 跟踪每个哲学家的状态
semaphore mutex = 1;         // 临界区的互斥
semaphore s[N];              // 每个哲学家一个信号量
 
void philosopher(int i) {
    while(TRUE) {
        think();
        take_two(i);
        eat();
        put_two(i);
    }
}
 
void take_two(int i) {
    down(&mutex);
    state[i] = HUNGRY;
    test(i);
    up(&mutex);
    down(&s[i]);
}
 
void put_two(i) {
    down(&mutex);
    state[i] = THINKING;
    test(LEFT);
    test(RIGHT);
    up(&mutex);
}
 
void test(i) {         // 尝试拿起两把筷子
    if(state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] !=EATING) {
        state[i] = EATING;
        up(&s[i]);
    }
}
```
  
### 进程通信
进程同步与进程通信很容易混淆，它们的区别在于：进程同步：控制多个进程按一定顺序执行；进程通信：进程间传输信息。
进程通信是一种手段，而进程同步是一种目的。也可以说，为了能够达到进程同步的目的，需要让进程进行通信，传输一些进程同步所需要的信息。

* 管道
管道是通过调用 pipe 函数创建的，`fd[0]`用于读，`fd[1]`用于写。
  
```c
#include <unistd.h>
int pipe(int fd[2]);
```
它具有以下限制：只支持半双工通信（单向交替传输）；只能在父子进程中使用。
  
* FIFO
也称为命名管道，去除了管道只能在父子进程中使用的限制。
  
```c
#include <sys/stat.h>
int mkfifo(const char *path, mode_t mode);
int mkfifoat(int fd, const char *path, mode_t mode);
```
FIFO 常用于客户-服务器应用程序中，FIFO 用作汇聚点，在客户进程和服务器进程之间传递数据。

* 消息队列
相比于 FIFO，消息队列具有以下优点：
  * 消息队列可以独立于读写进程存在，从而避免了 FIFO 中同步管道的打开和关闭时可能产生的困难；
  * 避免了FIFO的同步阻塞问题，不需要进程自己提供同步方法；
  * 读进程可以根据消息类型有选择地接收消息，而不像FIFO那样只能默认地接收。

* 信号量
它是一个计数器，用于为多个进程提供对共享数据对象的访问。

* 共享存储
允许多个进程共享一个给定的存储区。因为数据不需要在进程之间复制，所以这是最快的一种 IPC。
需要使用信号量用来同步对共享存储的访问。
多个进程可以将同一个文件映射到它们的地址空间从而实现共享内存。另外 XSI 共享内存不是使用文件，而是使用使用内存的匿名段。

* 套接字
与其它通信机制不同的是，它可用于不同机器间的进程通信。
## 死锁
### 必要条件
* 互斥：每个资源要么已经分配给了一个进程，要么就是可用的。
* 占有和等待：已经得到了某个资源的进程可以再请求新的资源。
* 不可抢占：已经分配给一个进程的资源不能强制性地被抢占，它只能被占有它的进程显式地释放。
* 环路等待：有两个或者两个以上的进程组成一条环路，该环路中的每个进程都在等待下一个进程所占有的资源。

### 处理方法
主要有以下四种方法：
* 鸵鸟策略
* 死锁检测与死锁恢复
* 死锁预防
* 死锁避免
  
#### 鸵鸟策略
把头埋在沙子里，假装根本没发生问题。因为解决死锁问题的代价很高，因此鸵鸟策略这种不采取任务措施的方案会获得更高的性能。当发生死锁时不会对用户造成多大影响，或发生死锁的概率很低，可以采用鸵鸟策略。大多数操作系统，包括 Unix，Linux 和 Windows，处理死锁问题的办法仅仅是忽略它。
死锁检测与死锁恢复
不试图阻止死锁，而是当检测到死锁发生时，采取措施进行恢复。
* 每种类型一个资源的死锁检测
每种类型一个资源的死锁检测算法是通过检测有向图是否存在环来实现，从一个节点出发进行深度优先搜索，对访问过的节点进行标记，如果访问了已经标记的节点，就表示有向图存在环，也就是检测到死锁的发生。
* 每种类型多个资源的死锁检测
每个进程最开始时都不被标记，执行过程有可能被标记。当算法结束时，任何没有被标记的进程都是死锁进程。寻找一个没有标记的进程 Pi，它所请求的资源小于等于 A。如果找到了这样一个进程，那么将 C 矩阵的第 i 行向量加到 A 中，标记该进程，并转回 1。如果没有这样一个进程，算法终止。
  
#### 死锁恢复
  * 利用抢占恢复
  * 利用回滚恢复
  * 通过杀死进程恢复
  
#### 死锁预防
在程序运行之前预防发生死锁。
* 破坏互斥条件
例如假脱机打印机技术允许若干个进程同时输出，唯一真正请求物理打印机的进程是打印机守护进程。
* 破坏占有和等待条件
一种实现方式是规定所有进程在开始执行前请求所需要的全部资源。
* 破坏不可抢占条件
* 破坏环路等待
给资源统一编号，进程只能按编号顺序来请求资源。
  
#### 死锁避免
在程序运行时避免发生死锁。
* 安全状态
  * 定义：如果没有死锁发生，并且即使所有进程突然请求对资源的最大需求，也仍然存在某种调度次序能够使得每一个进程运行完毕，则称该状态是安全的。安全状态的检测与死锁的检测类似，因为安全状态必须要求不能发生死锁。
* 单个资源的银行家算法
* 多个资源的银行家算法
  
## 内存管理
### 虚拟内存
虚拟内存的目的是为了让物理内存扩充成更大的逻辑内存，从而让程序获得更多的可用内存。
为了更好的管理内存，操作系统将内存抽象成地址空间。每个程序拥有自己的地址空间，这个地址空间被分割成多个块，每一块称为一页。这些页被映射到物理内存，但不需要映射到连续的物理内存，也不需要所有页都必须在物理内存中。当程序引用到不在物理内存中的页时，由硬件执行必要的映射，将缺失的部分装入物理内存并重新执行失败的指令。
从上面的描述中可以看出，虚拟内存允许程序不用将地址空间中的每一页都映射到物理内存，也就是说一个程序不需要全部调入内存就可以运行，这使得有限的内存运行大程序成为可能。例如有一台计算机可以产生 16 位地址，那么一个程序的地址空间范围是 0~64K。该计算机只有 32KB 的物理内存，虚拟内存技术允许该计算机运行一个 64K 大小的程序。
![](/img/post_pics/c/1.png)

### 分页系统地址映射
内存管理单元（MMU）管理着地址空间和物理内存的转换，其中的页表（Page table）存储着页（程序地址空间）和页框（物理内存空间）的映射表。
一个虚拟地址分成两个部分，一部分存储页面号，一部分存储偏移量。
下图的页表存放着 16 个页，这 16 个页需要用 4 个比特位来进行索引定位。例如对于虚拟地址（0010 000000000100），前 4 位是存储页面号 2，读取表项内容为（110 1），页表项最后一位表示是否存在于内存中，1 表示存在。后 12 位存储偏移量。这个页对应的页框的地址为 （110 000000000100）。
![](/img/post_pics/c/2.png)

### 页面置换算法
在程序运行过程中，如果要访问的页面不在内存中，就发生缺页中断从而将该页调入内存中。此时如果内存已无空闲空间，系统必须从内存中调出一个页面到磁盘对换区中来腾出空间。
页面置换算法和缓存淘汰策略类似，可以将内存看成磁盘的缓存。在缓存系统中，缓存的大小有限，当有新的缓存到达时，需要淘汰一部分已经存在的缓存，这样才有空间存放新的缓存数据。页面置换算法的主要目标是使页面置换频率最低（也可以说缺页率最低）。
* 最佳OPT, Optimal replacement algorithm
  * 所选择的被换出的页面将是最长时间内不再被访问，通常可以保证获得最低的缺页率。
  * 是一种理论上的算法，因为无法知道一个页面多长时间不再被访问。
  * 举例：一个系统为某进程分配了三个物理块，并有如下页面引用序列：
  * 开始运行时，先将 7, 0, 1 三个页面装入内存。当进程要访问页面 2 时，产生缺页中断，会将页面 7 换出，因为页面 7 再次被访问的时间最长。

* 最近最久未使用LRU, Least Recently Used
  * 虽然无法知道将来要使用的页面情况，但是可以知道过去使用页面的情况。LRU 将最近最久未使用的页面换出。
  * 为了实现 LRU，需要在内存中维护一个所有页面的链表。当一个页面被访问时，将这个页面移到链表表头。这样就能保证链表表尾的页面是最近最久未访问的。
  * 因为每次访问都需要更新链表，因此这种方式实现的 LRU 代价很高。

* 最近未使用NRU, Not Recently Used
每个页面都有两个状态位：R 与 M，当页面被访问时设置页面的 R=1，当页面被修改时设置 M=1。其中 R 位会定时被清零。可以将页面分成以下四类：
>R=0，M=0
R=0，M=1
R=1，M=0
R=1，M=1
  
当发生缺页中断时，NRU算法随机地从类编号最小的非空类中挑选一个页面将它换出。
NRU优先换出已经被修改的脏页面（R=0，M=1），而不是被频繁使用的干净页面（R=1，M=0）。

* 先进先出FIFO, First In First Out
选择换出的页面是最先进入的页面。该算法会将那些经常被访问的页面也被换出，从而使缺页率升高。

* 第二次机会算法
  * FIFO算法可能会把经常使用的页面置换出去，为了避免这一问题，对该算法做一个简单的修改：
    * 当页面被访问 (读或写) 时设置该页面的 R 位为 1。需要替换的时候，检查最老页面的 R 位。如果 R 位是 0，那么这个页面既老又没有被使用，可以立刻置换掉；如果是 1，就将 R 位清 0，并把该页面放到链表的尾端，修改它的装入时间使它就像刚装入的一样，然后继续从链表的头部开始搜索。

* 时钟Clock
第二次机会算法需要在链表中移动页面，降低了效率。时钟算法使用环形链表将页面连接起来，再使用一个指针指向最老的页面。

### 分段
虚拟内存采用的是分页技术，也就是将地址空间划分成固定大小的页，每一页再与内存进行映射。
下图为一个编译器在编译过程中建立的多个表，有 4 个表是动态增长的，如果使用分页系统的一维地址空间，动态增长的特点会导致覆盖问题的出现。
![](/img/post_pics/c/3.png)
分段的做法是把每个表分成段，一个段构成一个独立的地址空间。每个段的长度可以不同，并且可以动态增长。
![](/img/post_pics/c/4.png)

### 段页式
程序的地址空间划分成多个拥有独立地址空间的段，每个段上的地址空间划分成大小相同的页。这样既拥有分段系统的共享和保护，又拥有分页系统的虚拟内存功能。

### 分页与分段的比较
* 对程序员的透明性：分页透明，但是分段需要程序员显示划分每个段。
* 地址空间的维度：分页是一维地址空间，分段是二维的。
* 大小是否可以改变：页的大小不可变，段的大小可以动态改变。
* 出现的原因：分页主要用于实现虚拟内存，从而获得更大的地址空间；分段主要是为了使程序和数据可以被划分为逻辑上独立的地址空间并且有助于共享和保护。
  
## 设备管理
### 磁盘结构
* 盘面（Platter）：一个磁盘有多个盘面；
* 磁道（Track）：盘面上的圆形带状区域，一个盘面可以有多个磁道；
* 扇区（Track Sector）：磁道上的一个弧段，一个磁道可以有多个扇区，它是最小的物理储存单位，目前主要有 512 bytes 与 4 K 两种大小；
* 磁头（Head）：与盘面非常接近，能够将盘面上的磁场转换为电信号（读），或者将电信号转换为盘面的磁场（写）；
* 制动手臂（Actuator arm）：用于在磁道之间移动磁头；
* 主轴（Spindle）：使整个盘面转动。

### 磁盘调度算法
读写一个磁盘块的时间的影响因素有：
* 主旋转时间（主轴转动盘面，使得磁头移动到适当的扇区上）
* 主寻道时间（制动手臂移动，使得磁头移动到适当的磁道上）
* 主实际的数据传输时间
  
其中，寻道时间最长，因此磁盘调度的主要目标是使磁盘的平均寻道时间最短。


#### 先来先服务FCFS, First Come First Serve
按照磁盘请求的顺序进行调度。优点是公平和简单。缺点也很明显，因为未对寻道做任何优化，使平均寻道时间可能较长。

#### 最短寻道时间优先SSTF, Shortest Seek Time First
优先调度与当前磁头所在磁道距离最近的磁道。虽然平均寻道时间比较低，但是不够公平。如果新到达的磁道请求总是比一个在等待的磁道请求近，那么在等待的磁道请求会一直等待下去，也就是出现饥饿现象。具体来说，两端的磁道请求更容易出现饥饿现象。

#### 电梯算法SCAN
电梯总是保持一个方向运行，直到该方向没有请求为止，然后改变运行方向。
电梯算法（扫描算法）和电梯的运行过程类似，总是按一个方向来进行磁盘调度，直到该方向上没有未完成的磁盘请求，然后改变方向。
因为考虑了移动方向，因此所有的磁盘请求都会被满足，解决了 SSTF 的饥饿问题。


## 链接
### 编译系统
![](/img/post_pics/c/5.png)
* 预处理阶段：处理以 # 开头的预处理命令；
* 编译阶段：翻译成汇编文件；
* 汇编阶段：将汇编文件翻译成可重定向目标文件；
* 链接阶段：将可重定向目标文件和 printf.o 等单独预编译好的目标文件进行合并，得到最终的可执行目标文件。
  
### 静态链接
静态链接器以一组可重定向目标文件为输入，生成一个完全链接的可执行目标文件作为输出。链接器主要完成以下两个任务：
* 符号解析：每个符号对应于一个函数、一个全局变量或一个静态变量，符号解析的目的是将每个符号引用与一个符号定义关联起来。
* 重定位：链接器通过把每个符号定义与一个内存位置关联起来，然后修改所有对这些符号的引用，使得它们指向这个内存位置。
  
### 目标文件
* 可执行目标文件：可以直接在内存中执行；
* 可重定向目标文件：可与其它可重定向目标文件在链接阶段合并，创建一个可执行目标文件；
* 共享目标文件：这是一种特殊的可重定向目标文件，可以在运行时被动态加载进内存并链接；
  

### 动态链接
静态库有以下两个问题：
* 当静态库更新时那么整个程序都要重新进行链接；
* 对于 printf 这种标准函数库，如果每个程序都要有代码，这会极大浪费资源。
  
共享库是为了解决静态库的这两个问题而设计的，在 Linux 系统中通常用 .so 后缀来表示，Windows 系统上它们被称为 DLL。它具有以下特点：
* 在给定的文件系统中一个库只有一个文件，所有引用该库的可执行目标文件都共享这个文件，它不会被复制到引用它的可执行文件中；
* 在内存中，一个共享库的 .text 节（已编译程序的机器代码）的一个副本可以被不同的正在运行的进程共享。
