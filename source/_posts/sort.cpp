#include <iostream>
#include <vector>
#include <algorithm>
#include <time.h>

using namespace std; //堆排序的核心是建堆,传入参数为数组，根节点位置，数组长度
// #define MAXSIZE 50000
#define MAXSIZE 1000000
void Heap_build(int a[], int root, int length)
{
    int lchild = root * 2 + 1; //根节点的左子结点下标
    if (lchild < length)       //左子结点下标不能超出数组的长度
    {
        int flag = lchild;       //flag保存左右节点中最大值的下标
        int rchild = lchild + 1; //根节点的右子结点下标
        if (rchild < length)     //右子结点下标不能超出数组的长度(如果有的话)
        {
            if (a[rchild] > a[flag]) //找出左右子结点中的最大值
            {
                flag = rchild;
            }
        }
        if (a[root] < a[flag])
        {
            //交换父结点和比父结点大的最大子节点
            swap(a[root], a[flag]);
            //从此次最大子节点的那个位置开始递归建堆
            Heap_build(a, flag, length);
        }
    }
}

void Heap_sort(int a[], int len)
{
    for (int i = len / 2; i >= 0; --i) //从最后一个非叶子节点的父结点开始建堆
    {
        Heap_build(a, i, len);
    }

    for (int j = len - 1; j > 0; --j) //j表示数组此时的长度，因为len长度已经建过了，从len-1开始
    {
        swap(a[0], a[j]);    //交换首尾元素,将最大值交换到数组的最后位置保存
        Heap_build(a, 0, j); //去除最后位置的元素重新建堆，此处j表示数组的长度，最后一个位置下标变为len-2
    }
}

void insertSort(int vt[], int len)
{
    //vector<int> vt = {13, 5, 3, 4, 6, 2, 8, 22, 1, 7};
    //int len = vt.size();
    int j;
    for (int i = 1; i < len; i++)
    {
        if (vt[i] < vt[i - 1])
        {
            int tmp = vt[i]; //临时保存vt[i]的值
            for (j = i - 1; vt[j] > tmp; j--)
            {
                vt[j + 1] = vt[j]; //将比vt[i]大的数全部向右移动1位
            }
            vt[j + 1] = tmp;
        }
    }
}

void Merge(int arr[], int left, int mid, int right, int temp[])
{
    int i = left;    //左序列指针
    int j = mid + 1; //右序列指针
    int t = 0;       //临时数组指针
    while (i <= mid && j <= right)
    {
        if (arr[i] <= arr[j])
        {
            temp[t++] = arr[i++];
        }
        else
        {
            temp[t++] = arr[j++];
        }
    }
    while (i <= mid)
    { //将左边剩余元素填充进temp中
        temp[t++] = arr[i++];
    }
    while (j <= right)
    { //将右序列剩余元素填充进temp中
        temp[t++] = arr[j++];
    }
    t = 0;
    //将temp中的元素全部拷贝到原数组中
    while (left <= right)
    {
        arr[left++] = temp[t++];
    }
}

void MergeSort(int arr[], int left, int right, int temp[])
{
    if (left < right)
    {
        int mid = (left + right) / 2;
        MergeSort(arr, left, mid, temp);      //左边归并排序，使得左子序列有序
        MergeSort(arr, mid + 1, right, temp); //右边归并排序，使得右子序列有序
        Merge(arr, left, mid, right, temp);   //将两个有序子数组合并操作
    }
}

void quickSort(int a[], int l, int r)
{
    if (l < r)
    {
        int i, j, x;

        i = l;
        j = r;
        x = a[i];
        while (i < j)
        {
            while (i < j && a[j] > x)
                j--; // 从右向左找第一个小于x的数
            if (i < j)
                a[i++] = a[j];
            while (i < j && a[i] < x)
                i++; // 从左向右找第一个大于x的数
            if (i < j)
                a[j--] = a[i];
        }
        a[i] = x;
        quickSort(a, l, i - 1); /* 递归调用 */
        quickSort(a, i + 1, r); /* 递归调用 */
    }
}

int main(int argc, char **argv)
{
    int a[MAXSIZE];
    for (int i = 0; i < MAXSIZE; i++)
    {
        a[i] = rand() % MAXSIZE;
    }
#if 0
	clock_t Start_time = clock();
	Heap_sort(a, MAXSIZE);
	clock_t End_time = clock();
	cout << "Total Heap_sort running time is: " << static_cast<double>(End_time - Start_time) / CLOCKS_PER_SEC * 1000 << " ms" << endl;
#endif

#if 1
    clock_t Start_time = clock();
    insertSort(a, MAXSIZE);
    clock_t End_time = clock();
    cout << "Total insertSort running time is: " << static_cast<double>(End_time - Start_time) / CLOCKS_PER_SEC * 1000 << " ms" << endl;
#endif

#if 0
    clock_t Start_time = clock();
    int temp[MAXSIZE];
    MergeSort(a, 0, MAXSIZE - 1, temp);
    clock_t End_time = clock();
    cout << "Total insertSort running time is: " << static_cast<double>(End_time - Start_time) / CLOCKS_PER_SEC * 1000 << " ms" << endl;
#endif

#if 0
    clock_t Start_time = clock();
    quickSort(a, 0, MAXSIZE - 1);
    clock_t End_time = clock();
    cout << "Total insertSort running time is: " << static_cast<double>(End_time - Start_time) / CLOCKS_PER_SEC * 1000 << " ms" << endl;
#endif
    for (size_t i = 0; i < 100; ++i)
    {
        cout << a[i] << " ";
    }
    cout << endl;
    return 0;
}