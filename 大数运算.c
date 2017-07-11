#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max(a,b) ((a)>(b)?(a):(b))
#define N 102  //大数类型最大位数+2

void Add(int A[], int B[], int R[]);  //大数加法函数
void Sub(int A[], int B[], int R[]);  //大数减法函数
void Mul(int A[], int B[], int R[]);  //大数乘法函数
void Div(int A[], int B[], int R[]);  //大数除法函数
//使用整形数组表示大数类型：其中A[0]=位数 A[1]=符号 A[2]...大数
void str2big(char s[], int A[])   //nnnnnn => A
{  //字符串形式的“大数”转换为大数类型
	int i;
	for (i = 0; i < N; i++)
	{
		A[i] = 0;  //初始化
	}
	A[0] = -1;  //初始时为NaN
	for (i = strlen(s) - 1; i > 0; i--)
	{
		A[0]++;  //记录最高位索引
		A[A[0] + 2] = s[i] - '0';  //字符转换为数值
	}
	if (s[0] == '-')
	{
		A[1] = -1;  //符号为负
	}
	else
	{   //否则为正数
		A[0]++;
		A[A[0] + 2] = s[0] - '0';
		A[1] = 1;   //符号为正
	}
}

void int2big(int n, int A[])
{  //整型转换为大数类型
	int i, t;
	for (i = 0; i < N; i++)
		A[i] = 0;  //初始化
	A[0] = -1;  //初始时为NaN
	A[1] = n >= 0 ? 1 : -1;  //由n确定符号
	t = n >= 0 ? n : -n;
	while (t > 0)
	{  //将n每1位设置到大数中
		A[0]++;
		A[A[0] + 2] = t % 10;
		t = t / 10;
	}
	if (n == 0)
		A[0] = 0;  //n为0，则大数也为0；
}

void print(int A[])
{  //输出大数
	int i;
	if (A[1] == -1)
		printf("-");  //输出负号
	for (i = A[0]; i >= 0; i--)
		printf("%d", A[i + 2]);
	printf("\n");
}

void scan(int A[])
{  //输入大数 （字符串形式大整数）
	char s[N - 2];
	scanf_s("%s", s);  //输入大整数-nnnnn
	str2big(s, A);
}

void assign(int A[], int B[])
{  //大数赋值 A = B
	int i;
	for (i = 0; i < N; i++)
		A[i] = B[i];
}

void zerojustify(int A[])
{  //调整大数中无意义的0
	while (A[0] > 0 && A[A[0] + 2] == 0)
		A[0]--;  //大数高位中的0无意义
	if (A[0] == 0 && A[2] == 0)
		A[1] = 1;  //避免出现 -0
}

int compare(int A[], int B[])
{  //比较A和B  -1（A>B）  0(A=B)  1(A<B)
	int i;
	if (A[1] == -1 && B[1] == 1)
		return 1;  //A-  <  B+
	if (A[1] == 1 && B[1] == -1)
		return -1;  //A+  >  B-
	if (B[0] > A[0])
		return A[1];  //同号不同位数
	if (A[0] > B[0])
		return -1 * A[1];  //同号不同位数
	for (i = A[0]; i >= 0; i--)
	{  //同号同位数
		if (A[i + 2] > B[i + 2])
			return -1 * A[1];
		if (B[i + 2] > A[i + 2])
			return A[1];
	}
	return 0;  //相等
}

void digitshift(int A[], int n)
{  //计算A*10^n
	int i;
	if (A[0] == 0 && A[2] == 0)
		return;  //为0
	for (i = A[0]; i >= 0; i--)
		A[i + n + 2] = A[i + 2];  //大数向左移动n位
	for (i = 0; i < n; i++)
		A[i + 2] = 0;  //低n位为0
	A[0] = A[0] + n;  //大数位长增加n
}

void Add(int A[], int B[], int R[])
{  //大数加法R=A+B
	int i, c = 0;  //c为进位
	int2big(0, R);  //R=0
	if (A[1] == B[1])
		R[1] = A[1];  //A、B同号
	else
	{
		if (A[1] == -1) {  //A-则R=B-A
			A[1] = 1;
			Sub(B, A, R);
			A[1] = -1;
		}
		else {  //B-则R=A-B
			B[1] = 1;
			Sub(A, B, R);
			B[1] = -1;
		}
		return;
	}
	R[0] = max(A[0], B[0]) + 1;  //和的位长
	for (i = 0; i <= R[0]; i++) {  //逐位相加，考虑进位
		R[i + 2] = (c + A[i + 2] + B[i + 2]) % 10;
		c = (c + A[i + 2] + B[i + 2]) / 10;
	}
	zerojustify(R);
}

void Sub(int A[], int B[], int R[])
{  //大数减法R=A-B 
	int i, v, b = 0;  //b为借位
	int2big(0, R);  //R=0
	if (A[1] == -1 || B[1] == -1) {  //R=A- -B，R=-A-B做加法
		B[1] = -1 * B[1];
		Add(A, B, R);
		B[1] = -1 * B[1];
		return;
	}
	if (compare(A, B) == 1) {  //B>A则R=-（B-A）
		Sub(B, A, R);
		R[1] = -1;
		return;
	}
	R[0] = max(A[0], B[0]);  //减的位长
	for (i = 0; i <= R[0]; i++) {
		v = A[i + 2] - b - B[i + 2];  //逐位相减
		if (A[i + 2] > 0)
			b = 0;
		if (v < 0) {  //处理借位
			v = v + 10;
			b = 1;
		}
		R[i + 2] = v % 10;
	}
	zerojustify(R);
}

void Mul(int A[], int B[], int R[])
{  //大数乘法R=AxB
	int tmp[N], row[N];
	int i, j;
	int2big(0, R);  //R=0
	assign(row, A);  //row=A
	for (i = 0; i <= B[0]; i++) {  //B逐位乘A
		for (j = 1; j <= B[1 + 2]; j++) {  //多次相加
			Add(R, row, tmp);  //R=R+row*B
			assign(R, tmp);
		}
		digitshift(row, 1);  //下次row=row*10
	}
	R[1] = A[1] * B[1];  //处理相乘时的符号
	zerojustify(R);
}

void Div(int A[], int B[], int R[])
{  //大数除法R=A/B
	int tmp[N], row[N];
	int i, asign, bsign;
	int2big(0, R);  //R=0 
	R[1] = A[1] * B[1];  //处理相除时的符号
	asign = A[1];  //保存A的符号
	bsign = B[1];  //保存B的符号
	A[1] = 1;  //A符号调整为正
	B[1] = 1;  //B符号调整为正
	int2big(0, row);  //row=0
	int2big(0, tmp);  //tmp=0
	R[0] = A[0];  //R的位长初始与A相同
	for (i = A[0]; i >= 0; i--) {
		digitshift(row, 1);  //row=row*10
		row[2] = A[i + 2];
		R[i + 2] = 0;
		while (compare(row, B) != 1) {  //row<B
			R[i + 2]++;  //结果为相减次数
			Sub(row, B, tmp);  //逐位相减 row-B
			assign(row, tmp);
		}
	}
	zerojustify(R);
	A[1] = asign;  //A符号还原
	B[1] = bsign;  //B符号还原
}

int main(void)
{  //调用大数运算函数计算结果
	int c, A[N], B[N], R[N], Z[N];
	printf("a = ");
	scan(A);  //输入大数A
	printf("b = ");
	scan(B);  //输入大数B
	Add(A, B, R);  //计算大数加法R=A+B
	printf("a + b = ");
	print(R);
	c = compare(A, B);  //大数比较
	printf("a %s b\n", c == 0 ? "==" : (c < 0 ? ">" : "<"));
	Sub(A, B, R);  //计算大数减法R=A-B
	printf("a - b = ");
	print(R);
	Mul(A, B, R);  //计算大数乘法R=A*B
	printf("a * b = ");
	print(R);
	int2big(0, Z);
	if (compare(Z, B) == 0)  //不能除0
		printf("a / b = NaN\n");
	else {
		Div(A, B, R);  //计算大数除法R=A/B
		printf("a / b = ");
		print(R);
	}
	system("pause");
	return 0;
}