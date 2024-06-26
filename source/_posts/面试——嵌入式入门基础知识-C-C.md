---
title: 嵌入式软件基础知识-C/C++
date: 2020-6-25 19:13:09
index_img: /img/post_pics/index_img/embedded_c.png
tags: 
    - C/C++
    - Linux
categories: 
    - 面试
---
关于C/C++重点知识点汇总。
  
<!-- more -->
  
参考[C/C++常见面试知识点总结附面试真题----20190407更新](https://blog.csdn.net/kuweicai/article/details/82779648)

- [C/C++内存有哪几种类型？](#cc内存有哪几种类型)
- [计算机内部如何存储负数和浮点数？](#计算机内部如何存储负数和浮点数)
- [左值和右值](#左值和右值)
- [C和C++中的强制类型转换？](#c和c中的强制类型转换)
- [在C++程序中调用被C编译器编译后的函数，为什么要加extern“C”?](#在c程序中调用被c编译器编译后的函数为什么要加externc)
- [当i是一个整数的时候++i和i++那个更快一点？i++和++i的区别是什么？](#当i是一个整数的时候i和i那个更快一点i和i的区别是什么)
- [三目运算符](#三目运算符)
- [什么是面向对象（OOP）？面向对象的意义？](#什么是面向对象oop面向对象的意义)
- [解释下封装、继承和多态？](#解释下封装继承和多态)
- [什么时候生成默认构造函数（无参构造函数）？什么时候生成默认拷贝构造函数？什么是深拷贝？什么是浅拷贝？默认拷贝构造函数是哪种拷贝？什么时候用深拷贝？](#什么时候生成默认构造函数无参构造函数什么时候生成默认拷贝构造函数什么是深拷贝什么是浅拷贝默认拷贝构造函数是哪种拷贝什么时候用深拷贝)
- [构造函数和析构函数的执行顺序？](#构造函数和析构函数的执行顺序)
- [C++的编译环境](#c的编译环境)
- [说一下C++和C的区别](#说一下c和c的区别)
- [说一说 c++ 中四种强转类型转换](#说一说-c-中四种强转类型转换)
- [为什么不使用 C 的强制转换？](#为什么不使用-c-的强制转换)
- [请你说一下你理解的 c++ 中的四个智能指针](#请你说一下你理解的-c-中的四个智能指针)
- [请你回答一下智能指针有没有内存泄露的情况](#请你回答一下智能指针有没有内存泄露的情况)
- [请你来说一下智能指针的内存泄漏如何解决](#请你来说一下智能指针的内存泄漏如何解决)
- [请你回答一下为什么析构函数必须是虚函数？为什么 C++ 默认的析构函数不是虚函数（考点: 虚函数 析构函数）](#请你回答一下为什么析构函数必须是虚函数为什么-c-默认的析构函数不是虚函数考点-虚函数-析构函数)
- [请你来说一下 fork 函数](#请你来说一下-fork-函数)
- [请你来说一下C++中析构函数的作用](#请你来说一下c中析构函数的作用)
- [请你来说一下静态函数和虚函数的区别](#请你来说一下静态函数和虚函数的区别)
- [请你来说一说重载和覆盖](#请你来说一说重载和覆盖)
- [请你说一说 strcpy 和 和 strlen](#请你说一说-strcpy-和-和-strlen)
- [请你来回答一下++i 和 i++](#请你来回答一下i-和-i)
- [请你说一说你理解的虚函数和多态](#请你说一说你理解的虚函数和多态)
- [请你来说一说++i 和 i++ 的实现](#请你来说一说i-和-i-的实现)
- [请你来写个函数在main函数执行前先运行](#请你来写个函数在main函数执行前先运行)
- [以下四行代码的区别是什么？](#以下四行代码的区别是什么)
- [请你来说一下C++里是怎么定义常量的？常量存放在内存的哪个位置？](#请你来说一下c里是怎么定义常量的常量存放在内存的哪个位置)
- [请你来说一说隐式类型转换](#请你来说一说隐式类型转换)
- [请你来说一说 C++函数栈空间的最大值](#请你来说一说-c函数栈空间的最大值)
- [请你回答一下 new/delete与 与 malloc/free 的区别](#请你回答一下-newdelete与-与-mallocfree-的区别)
- [请你说说你了解的RTTI](#请你说说你了解的rtti)
- [请你说说虚函数表具体是怎样实现运行时多态的?](#请你说说虚函数表具体是怎样实现运行时多态的)
- [请你说说 C 语言中是怎么进行函数调用的？](#请你说说-c-语言中是怎么进行函数调用的)
- [请你说说C语言参数压栈顺序？](#请你说说c语言参数压栈顺序)
- [请你说说C++如何处理返回值？](#请你说说c如何处理返回值)
- [请你回答一下C++中拷贝构造函数的形参能否进行值传递？](#请你回答一下c中拷贝构造函数的形参能否进行值传递)
- [请你回答一下malloc与new的区别](#请你回答一下malloc与new的区别)
- [请你说一说select](#请你说一说select)
- [请你说说 fork，wait，exec 函数](#请你说说-forkwaitexec-函数)
- [请你来说一下map和set有什么区别，分别又是怎么实现的？](#请你来说一下map和set有什么区别分别又是怎么实现的)
- [请你来介绍一下STL的allocator（空间配置器）](#请你来介绍一下stl的allocator空间配置器)
- [请你讲讲 STL 有什么基本组成？他们之间的关系是怎样的？](#请你讲讲-stl-有什么基本组成他们之间的关系是怎样的)
- [请你说一说STL中map数据的存放形式](#请你说一说stl中map数据的存放形式)
- [请你说说STL中map与unordered\_map](#请你说说stl中map与unordered_map)
- [请你说一说vector和list的区别、应用，越详细越好](#请你说一说vector和list的区别应用越详细越好)
  
#### C/C++内存有哪几种类型？

- 堆(malloc)

- 栈(如局部变量、函数参数)
- 程序代码区（存放二进制代码）
- 全局/静态存储区（全局变量、static变量）和常量存储区（常量）。此外，C++中有自由存储区（new）一说。
- 全局变量、static变量会初始化为零，而堆和栈上的变量是随机的，不确定的。
  


  
#### 计算机内部如何存储负数和浮点数？

- 负数比较容易，就是通过一个标志位和补码来表示。

- 对于浮点类型的数据采用单精度类型（float）和双精度类型(double)来存储，float数据占用32bit,double数据占用64bit,我们在声明一个变量float f= 2.25f的时候，是如何分配内存的呢？如果胡乱分配，那世界岂不是乱套了么，其实不论是float还是double在存储方式上都是遵从IEEE的规范的，float遵从的是IEEE R32.24 ,而double 遵从的是R64.53。更多可以参考浮点数表示。
- 无论是单精度还是双精度在存储中都分为三个部分：
  - 符号位(Sign) : 0代表正，1代表为负
  - 指数位（Exponent）:用于存储科学计数法中的指数数据，并且采用移位存储
  - 尾数部分（Mantissa）：尾数部分

其中float的存储方式如下图所示:
![](/img/post_pics/c/c_1.png)
而双精度的存储方式如下图:
![](/img/post_pics/c/c_2.png)
  


#### 左值和右值

- 不是很严谨的来说，左值指的是既能够出现在等号左边也能出现在等号右边的变量(或表达式)；右值指的则是只能出现在等号右边的变量(或表达式)。

- 举例来说我们定义的变量 a 就是一个左值，而malloc返回的就是一个右值。或者左值就是在程序中能够寻址的东西，右值就是一个具体的真实的值或者对象，没法取到它的地址的东西(不完全准确)，因此没法对右值进行赋值，但是右值并非是不可修改的，比如自己定义的class, 可以通过它的成员函数来修改右值。

#### C和C++中的强制类型转换？

C中是直接在变量或者表达式前面加上（小括号括起来的）目标类型来进行转换，一招走天下，操作简单，但是由于太过直接，缺少检查，因此容易发生编译检查不到错误，而人工检查又及其难以发现的情况；而C++中引入了下面四种转换：

- static_cast
  - 用于基本类型间的转换
  - 不能用于基本类型指针间的转换
  - 用于有继承关系类对象间的转换和类指针间的转换
- dynamic_cast
  - 用于有继承关系的类指针间的转换
  - 用于有交叉关系的类指针间的转换
  - 具有类型检查的功能
  - 需要虚函数的支持
- reinterpret_cast
  - 用于指针间的类型转换
  - 用于整数和指针间的类型转换
- const_cast
  - 用于去掉变量的const属性
  - 转换的目标类型必须是指针或者引用
  
在C++中，普通类型可以通过类型转换构造函数转换为类类型，那么类可以转换为普通类型吗？答案是肯定的。但是在工程应用中一般不用类型转换函数，因为无法抑制隐式的调用类型转换函数（类型转换构造函数可以通过explicit来抑制其被隐式的调用），而隐式调用经常是bug的来源。实际工程中替代的方式是定义一个普通函数，通过显式的调用来达到类型转换的目的。

```cpp
class test{
    int m_value;
    ...
public:
    operator int()  //类型转换函数
    {
        return m_value;
    }

    int toInt() //显示调用普通函数来实现类型转换
    {
        return m_value
    }
}；

int main()
{
    ...
    test a(5);
    int i = a;
    ...

    return 0;
}
```

  
#### 在C++程序中调用被C编译器编译后的函数，为什么要加extern“C”?

C++语言支持函数重载，C语言不支持函数重载，函数被C++编译器编译后在库中的名字与C语言的不同，假设某个函数原型为：

```cpp
void foo(int x, int y);
```

该函数被C编译器编译后在库中的名字为 _foo, 而C++编译器则会产生像:_foo_int_int 之类的名字。为了解决此类名字匹配的问题，C++提供了C链接交换指定符号 extern “C”。

  
#### 当i是一个整数的时候++i和i++那个更快一点？i++和++i的区别是什么？

答：理论上++i更快，实际与编译器优化有关，通常几乎无差别。

```cpp
//i++实现代码为：
int operator++(int)
{
    int temp = *this;
    ++*this;
    return temp;
}//返回一个int型的对象本身

// ++i实现代码为：
int& operator++()
{
    *this += 1;
    return *this;
}//返回一个int型的对象引用
```

i++和++i的考点比较多，简单来说，就是i++返回的是i的值，而++i返回的是i+1的值。也就是++i是一个确定的值，是一个可修改的左值，如下使用：

```cpp
cout << ++(++(++i)) << endl;
cout << ++ ++i << endl;
```

可以不停的嵌套++i。这里有很多的经典笔试题，一起来观摩下：

```cpp
int main()
{
    int i = 1;
    printf("%d,%d\n", ++i, ++i);    //3,3
    printf("%d,%d\n", ++i, i++);    //5,3
    printf("%d,%d\n", i++, i++);    //6,5
    printf("%d,%d\n", i++, ++i);    //8,9
    system("pause");
    return 0;
}
```

首先是函数的参数入栈顺序从右向左入栈的，计算顺序也是从右往左计算的，不过都是计算完以后再进行的压栈操作：

- 对于第1个printf，首先执行++i，返回值是i，这时i的值是2，再次执行++i，返回值是i，得到i=3，将i压入栈中，此时i为3，也就是压入3，3；
- 对于第2个printf，首先执行i++，返回值是原来的i，也就是3，再执行++i，返回值是i，依次将3，5压入栈中得到输出结果
- 对于第3个printf，首先执行i++，返回值是5，再执行i++返回值是6，依次将5，6压入栈中得到输出结果
- 对于第4个printf，首先执行++i，返回i，此时i为8，再执行i++，返回值是8，此时i为9，依次将i，8也就是9，8压入栈中，得到输出结果。
  
上面的分析也是基于VS搞的，不过准确来说函数多个参数的计算顺序是未定义的(the order of evaluation of function arguments are undefined)。笔试题目的运行结果随不同的编译器而异。
  

  
#### 三目运算符

在C中三目运算符(? :)的结果仅仅可以作为右值，比如如下的做法在C编译器下是会报错的，但是C++中却是可以是通过的。这个进步就是通过引用来实现的，因为下面的三目运算符的返回结果是一个引用，然后对引用进行赋值是允许的。

```cpp
int main(void)
{
        int a = 8;
        int b = 6;
        (a>b ? a : b) = 88;
        cout<<a; //output 88
    return 0;
}
```
  

  
#### 什么是面向对象（OOP）？面向对象的意义？

- Object Oriented Programming, 面向对象是一种对现实世界理解和抽象的方法、思想，通过将需求要素转化为对象进行问题处理的一种思想。其核心思想是数据抽象、继承和动态绑定（多态）。

- 面向对象的意义在于：将日常生活中习惯的思维方式引入程序设计中；将需求中的概念直观的映射到解决方案中；以模块为中心构建可复用的软件系统；提高软件产品的可维护性和可扩展性。
  
#### 解释下封装、继承和多态？

- 封装：
封装是实现面向对象程序设计的第一步，封装就是将数据或函数等集合在一个个的单元中（我们称之为类）。封装的意义在于保护或者防止代码（数据）被我们无意中破坏。

- 继承：
继承主要实现重用代码，节省开发时间。
子类可以继承父类的一些东西。
  - 公有继承(public)
公有继承的特点是基类的公有成员和保护成员作为派生类的成员时，它们都保持原有的状态，而基类的私有成员仍然是私有的，不能被这个派生类的子类所访问。
  - 私有继承(private)
私有继承的特点是基类的公有成员和保护成员都作为派生类的私有成员，并且不能被这个派生类的子类所访问。
  - 保护继承(protected)
保护继承的特点是基类的所有公有成员和保护成员都成为派生类的保护成员，并且只能被它的派生类成员函数或友元访问，基类的私有成员仍然是私有的。
- 多态
  
#### 什么时候生成默认构造函数（无参构造函数）？什么时候生成默认拷贝构造函数？什么是深拷贝？什么是浅拷贝？默认拷贝构造函数是哪种拷贝？什么时候用深拷贝？

- 没有任何构造函数时，编译器会自动生成默认构造函数，也就是无参构造函数；当类没有拷贝构造函数时，会生成默认拷贝构造函数。

- 深拷贝是指拷贝后对象的逻辑状态相同，而浅拷贝是指拷贝后对象的物理状态相同；默认拷贝构造函数属于浅拷贝。
- 当系统中有成员指代了系统中的资源时，需要深拷贝。比如指向了动态内存空间，打开了外存中的文件或者使用了系统中的网络接口等。如果不进行深拷贝，比如动态内存空间，可能会出现多次被释放的问题。是否需要定义拷贝构造函数的原则是，是类是否有成员调用了系统资源，如果定义拷贝构造函数，一定是定义深拷贝，否则没有意义。
  
#### 构造函数和析构函数的执行顺序？

构造函数

- 首先调用父类的构造函数；
- 调用成员变量的构造函数；
- 调用类自身的构造函数。
析构函数
对于栈对象或者全局对象，调用顺序与构造函数的调用顺序刚好相反，也即后构造的先析构。对于堆对象，析构顺序与delete的顺序相关。
  
#### C++的编译环境

如下图所示，C++的编译环境由如下几部分构成：C++标准库、C语言兼容库、编译器扩展库及编译模块。
![](/img/post_pics/c/c_3.png)

```cpp
#include<iostream>  //C++标准库，不带".h"
#include<string.h>  //C语言兼容库，由编译器厂商提供
```

值得注意的是，C语言兼容库功能上跟C++标准库中的C语言子库相同，它的存中主要为了兼容C语言编译器，也就是说如果一个文件只包含C语言兼容库（不包含C++标准库），那么它在C语言编译器中依然可以编译通过。





#### 说一下C++和C的区别

- 设计思想上
  - C++是面向对象的语言，而C是面向过程的结构化编程语言。

- 语法上
  - C++具有重载、继承和多态三种特性。
  - C++相比C，增加多许多类型安全的功能，比如强制类型转换。
  - C++支持范式编程，比如模板类、函数模板等。
  
#### 说一说 c++ 中四种强转类型转换

C++中四种类型转换是：static_cast、dynamic_cast、const_cast、reinterpret_cast。

- const_cast
  - 一般用于指针转换。
  - 可以将 const 指针转为非 const 指针。
  - 可以将const引用转换为非const引用。
- static_cast
  - 用于各种隐式转换，转换之前，要先将类型转换为void*，再转换成其他类型，比如非 const 转 const，void*转指针等, static_cast 能用于多态向上转化，如果向下转能成功但是不安全，结果未知，因为没有动态类型检查；
- dynamic_cast
  - 用于动态类型转换。只能用于含有虚函数的类，用于类层次间的向上和向下转化。只能转指针或引用。向下转化时，如果是非法的对于指针返回 NULL，对于引用抛异常。
  - 向上转换：指的是子类向基类的转换
  - 向下转换：指的是基类向子类的转换
  - 它通过判断在执行到该语句的时候变量的运行时类型和要转换的类型是否相同来判断是否能够进行向下转换。
- reinterpret_cast
  - 几乎什么都可以转，比如将 int 转指针，可能会出问题，尽量少用；
  
#### 为什么不使用 C 的强制转换？

C的强制转换表面上看起来功能强大什么都能转，但是转化不够明确，不能进行错误检查，容易出错。
  

#### 请你说一下你理解的 c++ 中的四个智能指针

- C++里面的四个智能指针: auto_ptr、shared_ptr、weak_ptr、unique_ptr（其中后三个是 c++11 支持，第一个已经被 11 弃用）
  




#### 请你回答一下智能指针有没有内存泄露的情况

当两个对象相互使用一个 shared_ptr 成员变量指向对方，会造成循环引用，使引用计数失效，从而导致内存泄漏。

#### 请你来说一下智能指针的内存泄漏如何解决

为了解决循环引用导致的内存泄漏，引入了 weak_ptr 弱指针，weak_ptr 的构造函数不会修改引用计数的值，从而不会对对象的内存进行管理，其类似一个普通指针，但不指向引用计数的共享内存，但是其可以检测到所管理的对象是否已经被释放，从而避免非法访问。

#### 请你回答一下为什么析构函数必须是虚函数？为什么 C++ 默认的析构函数不是虚函数（考点: 虚函数 析构函数）

- 一般将基类的析构函数设置为虚函数，当我们new一个子类对象，并用基类指针指向子类对象时，避免释放基类指针后，而未释放子类对象的空间，防止内存泄漏。

- 首先，默认的析构函数所在的类一般不会被其他类继承，而且虚函数需要额外的虚函数表和虚表指针，会占用额外的内存。所以默认构造函数不是虚函数。

#### 请你来说一下 fork 函数

fork：创建一个和当前进程映像一样的进程可以通过 fork( )系统调用；
相关头文件：

```cpp
#include <sys/types.h>
#include <unistd.h>
```

- 函数原型： pid_t fork(void);

- 成功调用 fork( )会创建一个新的进程，它几乎与调用 fork( )的进程一模一样，这两个进程都会继续运行。
返回值：
  - 在子进程中，成功的 fork( )调用会返回 0。
  - 在父进程中 fork( )返回子进程的 pid。
  - 如果出现错误，fork( )返回一个负值。
- 最常见的 fork( )用法是创建一个新的进程，然后使用 exec( )载入二进制映像，替换当前进程的映像。这种情况下，派生（fork）了新的进程，而这个子进程会执行一个新的二进制可执行文件的映像。这种“派生加执行”的方式是很常见的。
- 在早期的 Unix 系统中，创建进程比较原始。当调用 fork 时，内核会把所有的内部数据结构复制一份，复制进程的页表项，然后把父进程的地址空间中的内容逐页的复制到子进程的地址空间中。但从内核角度来说，逐页的复制方式是十分耗时的。现代的 Unix 系统采取了更多的优化，例如 Linux，采用了写时复制的方法，而不是对父进程空间进程整体复制。
  
#### 请你来说一下C++中析构函数的作用

- 析构函数与构造函数对应，当对象结束其生命周期，如对象所在的函数已调用完毕时，系统会自动执行析构函数，释放动态开辟的内存空间，防止内存泄漏。

- 注： 类调用析构函数顺序：1）子类本身的析构函数；2）对象成员析构函数；3）基类析构函数。

#### 请你来说一下静态函数和虚函数的区别

静态函数在编译的时候就已经确定运行时机（即编译时确定函数的入口地址），虚函数在运行的时候动态绑定。虚函数因为用了虚函数表机制，调用的时候会增加一次内存开销。

#### 请你来说一说重载和覆盖

- 重载：其是对于一个类而言的，在类中，两个函数的函数名相同，参数列表不同（参数类型或个数），返回值无要求。

- 覆盖：前提是子类继承父类，在父类中，有一个虚函数，然后在子类中重新定义了这个函数，这就构成了覆盖。
  
补充：

- 隐藏：前提也是子类继承父类，分为两种情况。
  - 父类函数和子类函数名相同，但是参数不同，这时父类同名函数有无virtual修饰，都构成隐藏，即子类指针调用同名函数，调用的是子类的函数，父类函数被隐藏。
  - 父类函数和子类函数名相同，参数也相同，且父类同名函数无virtual修饰。
  
#### 请你说一说 strcpy 和 和 strlen

- strcpy 是字符串拷贝函数，原型：`char *strcpy(char* dest, const char *src);`

- 从src逐字节拷贝到dest，直到遇到'\0'结束，因为没有指定长度，可能会导致拷贝越界，造成缓冲区溢出漏洞，安全版本是 strncpy 函数。
- strlen 函数是计算字符串长度的函数，返回从开始到'\0'之间的字符个数。
  
#### 请你来回答一下++i 和 i++

++i先自增1，再返回，i++先返回i，再自增1。
  
#### 请你说一说你理解的虚函数和多态

- 多态的实现主要分为静态多态和动态多态，静态多态主要是重载，在编译的时候就已经确定，静态绑定；动态多态是用虚

- 函数机制实现的，在运行期间动态绑定。
- 举个栗子：一个父类类型的指针指向一个子类对象时候，使用父类的指针去调用子类中重写了的父类中的虚函数的时候，会调用子类重写过后的函数，在父类中声明为加了 virtual关键字的函数，在子类中重写时候不需要加 virtual 也是虚函数。
- 虚函数的实现：在有虚函数的类中，类的最开始部分是一个虚函数表的指针，这个指针指向一个虚函数表，表中放了虚函数的地址，实际的虚函数在代码段(.text)中。当子类继承了父类的时候也会继承其虚函数表，当子类重写父类中虚函数时候，会将其继承到的虚函数表中的地址替换为重新写的函数地址。使用了虚函数，会增加访问内存开销，降低效率。
  
#### 请你来说一说++i 和 i++ 的实现

- ++i 的实现
  
```cpp
int & int::operator++()
{
  *this += 1;
  return *this;
}
```

- i++的实现
  
```cpp
const int int::operator(int)
{
  int oldvalue = *this;
  ++(*this);
  return oldvalue;
}
```

#### 请你来写个函数在main函数执行前先运行

编译器：VC++6.0
语言：C
代码：

```cpp
#include<stdio.h>
int main()
{
  printf("main\n");
  return 0;
}
int before_main()
{
  printf("before_main\n");
  return 0;
}
typedef int func();
#pragma data_seg(".CRT$XIU")//用#pragma data_seg建立一个新的数据段并定义共享数据
static func * before[] = { before_main }; //定义一个函数数组，数组放的是函数的入口地址
```

编译器：VC++6.0
语言：C++
代码：

```cpp
#include<iostream>

using namespace std;

int before_main()
{
 cout<<"before_main() called"<<endl;
 return 0;
}


int g_Value = before_main();


int main()
{
 cout<<"main() called"<<endl;
 return 0;
}
```
  
#### 以下四行代码的区别是什么？

```cpp
const char * arr ="123";
char * brr = "123";
const char crr[] ="123";
char drr[] = "123";

const char * arr = "123";
//字符串 123 保存在常量区，const 本来是修饰 arr 指向的值不能通过 arr 去修改，但是字符串"123"在常量区，本来就不能改变，所以加不加 const 效果都一样。
char * brr = "123";
//字符串 123 保存在常量区，这个 arr 指针指向的是同一个位置，同样不能通过 brr 去修改"123"的值。
const char crr[] = "123";
//这里 123 本来是在栈上的，但是编译器可能会做某些优化，将其放到常量区。
char drr[] = "123";
//字符串 123 保存在栈区，可以通过 drr 去修改。
```
  
#### 请你来说一下C++里是怎么定义常量的？常量存放在内存的哪个位置？

- 常量在 C++里的定义就是一个const 加上对象类型，常量定义必须初始化。

- 对于局部对象，常量存放在栈区。
- 对于全局对象，编译期一般不分配内存，常量存放在全局/静态存储区。
- 对于字面值常量，比如字符串，存放在常量存储区。

#### 请你来说一说隐式类型转换

首先，对于内置类型，低精度的变量给高精度变量赋值会发生隐式类型转换，低精度类型会转化为高精度类型。其次，对于只存在单个参数的构造函数的对象构造来说，函数调用可以直接使用该参数传入，编译器会自动调用其构造函数生成临时对象。

#### 请你来说一说 C++函数栈空间的最大值

Windows 默认是 2 M，不过可以调整。
Linux 默认是 8M，不过可以调整。



#### 请你回答一下 new/delete与 与 malloc/free 的区别

首先，new/delete 是 C++的关键字，而 malloc/free 是 C语言的库函数，后者使用必须指明申请内存空间的大小，对于类类型的对象，后者不会调用构造函数和析构函数。
![](/img/post_pics/c/c_6.png)

#### 请你说说你了解的RTTI

[RTTI (Run Time Type Identification)](https://blog.csdn.net/weixin_34337265/article/details/86392595) 指的是运行时类型识别。程序能够使用基类的指针或引来检查这些指针或引用所指的对象的实际派生类型。
RTTI提供了两个非常有用的操作符：typeid 和 dynamic_cast。

- typeid操作符：返回指针和引用所指的实际类型，返回类型为 type_info。
关于 typeid 的注意事项：
  - typeid 返回一个 type_info 的对象引用。
  - 如果想通过基类的指针指向派生类的数据类型，基类就必须要带有虚函数，否则，在使用typeid 时，就只能返回定义时所使用的数据类型。
  - typeid 只能获取对象的实际类型，即便这个类含有虚函数，也只能判断当前对象是基类还是派生类，而不能判断当前指针是基类还是派生类。
- dynamic_cast操作符：将基类类型的指针或引用安全地转换为其派生类类型的指针或引用。
关于 dynamic_cast 的注意事项：
  - 只能应用与指针和引用的转换，即只能转化为某一个类型的指针或某一个类型的引用，而不能是某类型本身。
  - 要转化的类型中必须包含虚函数，如果没有虚函数，转换就会失败。
  - 如果转换成功，返回子类的地址，如果转换失败，返回NULL。

#### 请你说说虚函数表具体是怎样实现运行时多态的?

对于存在虚函数的基类的对象，基类对象的头部存放指向虚函数表的指针，子类若重写基类的虚函数，对应虚函数表中，该函数的地址会被重写的子类函数的地址替换。

#### 请你说说 C 语言中是怎么进行函数调用的？

每一个函数调用都会分配函数栈，在栈内进行函数执行过程。调用前，先把返回地址压栈，然后把当前函数的堆栈指针（ESP）压栈。

#### 请你说说C语言参数压栈顺序？

根据参数列表从右向左依次压栈。

#### 请你说说C++如何处理返回值？

生成一个临时变量，把它的引用作为函数参数传入函数内。

#### 请你回答一下C++中拷贝构造函数的形参能否进行值传递？

不能。如果是值传递的话，调用拷贝构造函数时，首先将实参传给形参，这样又会调用拷贝构造函数，如此，会造成无限递归调用拷贝构造函数。

#### 请你回答一下malloc与new的区别

malloc：本身是一个函数，需要知道申请内存的大小，返回的指针需要强转。
new：本身不是函数，是一个关键字，不用指定申请内存大小，动态开辟空间，返回指针不用强转。

#### 请你说一说select

- 函数原型：int select ( int maxfdp1, fd_set *readset, fd_set*writeset, fd_set *exceptset, const struct timeval*timeout )

- select在使用前，先将需要监控的描述符对应的 bit 位置 1，然后将其传给 select,当有任何一个事件发生时，select 将会返回所有的描述符，需要在应用程序自己遍历去检查哪个描述符上有事件发生，效率很低，并且其不断在内核态和用户态进行描述符的拷贝，开销很大。
- 补充：[select总结](https://www.cnblogs.com/Anker/archive/2013/08/14/3258674.html)

#### 请你说说 fork，wait，exec 函数

父进程产生子进程使用fork拷贝出来一个父进程的副本，此时只拷贝了父进程的页表，两个进程都读同一块内存，当有进程写的时候使用写实拷贝机制分配内存，exec 函数可以加载一个 elf 文件去替换父进程，从此父进程和子进程就可以运行不同的程序了。fork 从父进程返回子进程的 pid，从子进程返回 0，调用了 wait 的父进程将会发生阻塞，直到有子进程状态改变,执行成功返回 0，错误返回-1。exec 执行成功则子进程从新的程序开始运行，无返回值，执行失败返回-1。

#### 请你来说一下map和set有什么区别，分别又是怎么实现的？

实现机制：map 和 set 都是 C++的关联容器，其底层实现都是红黑树（RB-Tree）。由于 map 和 set 所开放的各种操作接口，RB-tree 也都提供了，所以几乎所有的 map 和 set 的操作行为，都只是转调 RB-tree 的操作行为。
map和set区别在于：

- map 中的元素是 key-value（关键字—值）对：关键字起到索引的作用，值则表示与索引相关联的数据；set 与之相对就是关键字的简单集合，set 中每个元素只包含一个关键字key。
- set 的迭代器是 const 的，不允许修改元素的值；map 允许修改 value，但不允许修改 key。其原因是因为map 和 set 是根据关键字排序来保证其有序性的。
  - 对于set，如果允许修改 key 的话，那么首先需要删除该键，然后调节平衡，再插入修改后的键值，调节平衡，如此一来，严重破坏了 map 和 set 的结构，导致 iterator 失效，不知道应该指向改变前的位置，还是指向改变后的位置。所以 STL 中将 set 的迭代器设置成 const，不允许修改迭代器的值；
  - 对于map，map 的迭代器则不允许修改 key 值，允许修改 value 值。
- map 支持下标操作，set 不支持下标操作。map 可以用 key 做下标，map 的下标运算符[ ]将关键码作为下标去执行查找，如果关键码不存在，则插入一个具有该关键码和 mapped_type 类型默认值的元素至 map 中，因此下标运算符[ ]在 map 应用中需要慎用，const_map 不能用，只希望确定某一个关键值是否存在而不希望插入元素时也不应该使用，mapped_type 类型没有默认值也不应该使用。如果 find 能解决需要，尽可能用 find。
  
#### 请你来介绍一下STL的allocator（空间配置器）

- STL的空间配置器用于封装STL容器在内存管理上的底层细节。在 C++中，其内存配置和释放如下：

- new运算分两个阶段：
  - 调用 operator new 配置内存;
  - 调用对象构造函数构造对象内容
- delete运算分两个阶段：
  - 调用对象析构函数；
  - 调用operator delete 释放内存
  
为了精密分工，STL allocator 将两个阶段操作区分开来：

- 内存配置由alloc::allocate()函数负责，内存释放由alloc::deallocate()函数负责；
- 对象构造由::construct()函数负责，对象析构由::destroy()函数负责。
- 同时为了提升内存管理的效率，减少申请小内存造成的内存碎片问题，SGI STL 采用了两级空间配置器。
- 当分配的空间大小超过 128B 时，会使用第一级空间配置器；
- 当分配的空间大小小于 128B 时，将使用第二级空间配置器。
- 第一级空间配置器直接使用 malloc()、realloc()、free()函数进行内存空间的分配和释放，而第二级空间配置器采用了内存池技术，通过空闲链表来管理内存。
  
#### 请你讲讲 STL 有什么基本组成？他们之间的关系是怎样的？

- STL 主要由：以下六部分组成：
  - 容器
  - 迭代器
  - 仿函数
  - 算法
  - 分配器
  - 配接器

- 他们之间的关系：
  - 分配器：给容器分配存储空间。
  - 算法：通过迭代器获取容器中的内容。
  - 仿函数：可以协助算法完成各种操作。
  - 配接器：用来套接适配仿函数。
  
#### 请你说一说STL中map数据的存放形式

对于map，数据以红黑树形式存放。
补充：
对于unordered_map，数据以哈希表形式存放。
  
#### 请你说说STL中map与unordered_map

- Map映射，map 的所有元素都是pair，同时拥有键值（key）和实值（value）。pair 的第一元素被视为键值，第二元素被视为实值。所有元素都会根据元素的键值自动被排序。不允许键值重复。
  - 底层实现：红黑树
  - 适用场景：有序键值对不重复映射

- Multimap
  - 多重映射。multimap 的所有元素都是 pair，同时拥有实值（value）和键值（key）。pair 的第一元素被视为键值，第二元素被视为实值。所有元素都会根据元素的键值自动被排序。允许键值重复。
  - 底层实现：红黑树
  - 适用场景：有序键值对可重复映射

#### 请你说一说vector和list的区别、应用，越详细越好

- 概念：
  - Vector
    - 连续存储的容器，动态数组，在堆上分配空间。
    - 底层实现：数组
    - 两倍容量增长：
    - vector 增加（插入）新元素时，如果未超过当时的容量，则还有剩余空间，那么直接添加到最后（插入指定位置），然后调整迭代器。如果没有剩余空间了，则会重新配置原有元素个数的两倍空间，然后将原空间元素通过复制的方式初始化新空间，再向新空间增加元素，最后析构并释放原空间，之前的迭代器会失效。
    - 性能
      - 访问：O(1)
      - 插入：在最后插入（空间够）：很快
      - 在最后插入（空间不够）：需要内存申请和释放，以及对之前数据进行拷贝。
      - 在中间插入（空间够）：内存拷贝
      - 在中间插入（空间不够）：需要内存申请和释放，以及对之前数据进行拷贝。
      - 删除：在最后删除：很快
      - 在中间删除：内存拷贝
    - 适用场景：经常随机访问，且不经常对非尾节点进行插入删除。
  - List
    - 动态链表，在堆上分配空间，每插入一个元数都会分配空间，每删除一个元素都会释放空间。
    - 底层：双向链表
    - 性能
      - 访问：随机访问性能很差，只能快速访问头尾节点。
      - 插入：很快，一般是常数开销
      - 删除：很快，一般是常数开销
    - 适用场景：经常插入删除大量数据

- 区别：
  - vector 底层实现是数组；list是双向链表。
  - vector 支持随机访问，list不支持。
  - vector 是顺序内存，list不是。
  - vector 在中间节点进行插入删除会导致内存拷贝，list不会。
  - vector 一次性分配好内存，不够时才进行2倍扩容；list每次插入新节点都会进行内存申请。
  - vector 随机访问性能好，插入删除性能差；list 随机访问性能差，插入删除性能好。
- 应用
  - vector 拥有一段连续的内存空间，因此支持随机访问，如果需要高效的随即访问，而不在乎插入和删除的效率，使用 vector。
  - list 拥有一段不连续的内存空间，如果需要高效的插入和删除，而不关心随机访问，则应使用 list。

