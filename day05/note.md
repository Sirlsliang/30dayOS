## Day05

## 结构体
	- 声明 : struct XXXX{ char xx; };
	- 使用 : XXXX xxx

## 指向结构体的指针
	- 指针变量：地址用4个字节表示

## 箭头标记方式
	- 省略地址中的括号，直接使用箭头
	- xsize = (*binfo).scrnx == xsize = binfo->scrnx
## 错误
	- two or more data types in declaration : 定义结构体时,没有加分号(重复定义)
