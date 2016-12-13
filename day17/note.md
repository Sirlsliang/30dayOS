## Days17

#### 键盘的转换
- 利用两个数据编码映射表，当按下shift键时使用另一套编码映射表即可


#### Key_Shift
	
|						 |		按下			|			抬起		|
|------------|--------------|-------------|
|	左Shift		 |		0x2a			| 	0xaa			|
|	右Shift		 |		0x36			|		0xb6			|
	
- key_shift 变量，左Shift按下时置为1,右Shift按下时置为2,两个都不按置为0,两个都按下置
为3.

#### CapsLock键位的获取
- CapsLock键位的状态可由BIOS查询得知，存储在binfo->leds变量中的第4～6位数据中。
- CapsLock off 与 Shift off 或者 CapsLock on 与 Shift on 这两种状态才能输出小写。
- 只需要将大写字母的编码加上0x20就可以得到小写字母的编码

#### 各种锁定键的支持
- 改写binfo->leds 中的对应位置
