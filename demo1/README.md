# 数组连续性 与 结构体
![](./s1.jpg)

### 数组
一般这种状态蓝栏 都是用数组来作于存储
我们只要知道其中连续的2个的位置  就可以计算出 他们的间隔  反向推导出所有数据的位置

### 结构体
```
strcut People {
    string name;
    int age;
}
```
结构体用于存储同一类型下的数据
我们知道其中一个的位置也很容易推到出其他的位置