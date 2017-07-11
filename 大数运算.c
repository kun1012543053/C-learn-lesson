#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define max(a,b) ((a)>(b)?(a):(b))
#define N 102  //�����������λ��+2

void Add(int A[], int B[], int R[]);  //�����ӷ�����
void Sub(int A[], int B[], int R[]);  //������������
void Mul(int A[], int B[], int R[]);  //�����˷�����
void Div(int A[], int B[], int R[]);  //������������
//ʹ�����������ʾ�������ͣ�����A[0]=λ�� A[1]=���� A[2]...����
void str2big(char s[], int A[])   //nnnnnn => A
{  //�ַ�����ʽ�ġ�������ת��Ϊ��������
	int i;
	for (i = 0; i < N; i++)
	{
		A[i] = 0;  //��ʼ��
	}
	A[0] = -1;  //��ʼʱΪNaN
	for (i = strlen(s) - 1; i > 0; i--)
	{
		A[0]++;  //��¼���λ����
		A[A[0] + 2] = s[i] - '0';  //�ַ�ת��Ϊ��ֵ
	}
	if (s[0] == '-')
	{
		A[1] = -1;  //����Ϊ��
	}
	else
	{   //����Ϊ����
		A[0]++;
		A[A[0] + 2] = s[0] - '0';
		A[1] = 1;   //����Ϊ��
	}
}

void int2big(int n, int A[])
{  //����ת��Ϊ��������
	int i, t;
	for (i = 0; i < N; i++)
		A[i] = 0;  //��ʼ��
	A[0] = -1;  //��ʼʱΪNaN
	A[1] = n >= 0 ? 1 : -1;  //��nȷ������
	t = n >= 0 ? n : -n;
	while (t > 0)
	{  //��nÿ1λ���õ�������
		A[0]++;
		A[A[0] + 2] = t % 10;
		t = t / 10;
	}
	if (n == 0)
		A[0] = 0;  //nΪ0�������ҲΪ0��
}

void print(int A[])
{  //�������
	int i;
	if (A[1] == -1)
		printf("-");  //�������
	for (i = A[0]; i >= 0; i--)
		printf("%d", A[i + 2]);
	printf("\n");
}

void scan(int A[])
{  //������� ���ַ�����ʽ��������
	char s[N - 2];
	scanf_s("%s", s);  //���������-nnnnn
	str2big(s, A);
}

void assign(int A[], int B[])
{  //������ֵ A = B
	int i;
	for (i = 0; i < N; i++)
		A[i] = B[i];
}

void zerojustify(int A[])
{  //�����������������0
	while (A[0] > 0 && A[A[0] + 2] == 0)
		A[0]--;  //������λ�е�0������
	if (A[0] == 0 && A[2] == 0)
		A[1] = 1;  //������� -0
}

int compare(int A[], int B[])
{  //�Ƚ�A��B  -1��A>B��  0(A=B)  1(A<B)
	int i;
	if (A[1] == -1 && B[1] == 1)
		return 1;  //A-  <  B+
	if (A[1] == 1 && B[1] == -1)
		return -1;  //A+  >  B-
	if (B[0] > A[0])
		return A[1];  //ͬ�Ų�ͬλ��
	if (A[0] > B[0])
		return -1 * A[1];  //ͬ�Ų�ͬλ��
	for (i = A[0]; i >= 0; i--)
	{  //ͬ��ͬλ��
		if (A[i + 2] > B[i + 2])
			return -1 * A[1];
		if (B[i + 2] > A[i + 2])
			return A[1];
	}
	return 0;  //���
}

void digitshift(int A[], int n)
{  //����A*10^n
	int i;
	if (A[0] == 0 && A[2] == 0)
		return;  //Ϊ0
	for (i = A[0]; i >= 0; i--)
		A[i + n + 2] = A[i + 2];  //���������ƶ�nλ
	for (i = 0; i < n; i++)
		A[i + 2] = 0;  //��nλΪ0
	A[0] = A[0] + n;  //����λ������n
}

void Add(int A[], int B[], int R[])
{  //�����ӷ�R=A+B
	int i, c = 0;  //cΪ��λ
	int2big(0, R);  //R=0
	if (A[1] == B[1])
		R[1] = A[1];  //A��Bͬ��
	else
	{
		if (A[1] == -1) {  //A-��R=B-A
			A[1] = 1;
			Sub(B, A, R);
			A[1] = -1;
		}
		else {  //B-��R=A-B
			B[1] = 1;
			Sub(A, B, R);
			B[1] = -1;
		}
		return;
	}
	R[0] = max(A[0], B[0]) + 1;  //�͵�λ��
	for (i = 0; i <= R[0]; i++) {  //��λ��ӣ����ǽ�λ
		R[i + 2] = (c + A[i + 2] + B[i + 2]) % 10;
		c = (c + A[i + 2] + B[i + 2]) / 10;
	}
	zerojustify(R);
}

void Sub(int A[], int B[], int R[])
{  //��������R=A-B 
	int i, v, b = 0;  //bΪ��λ
	int2big(0, R);  //R=0
	if (A[1] == -1 || B[1] == -1) {  //R=A- -B��R=-A-B���ӷ�
		B[1] = -1 * B[1];
		Add(A, B, R);
		B[1] = -1 * B[1];
		return;
	}
	if (compare(A, B) == 1) {  //B>A��R=-��B-A��
		Sub(B, A, R);
		R[1] = -1;
		return;
	}
	R[0] = max(A[0], B[0]);  //����λ��
	for (i = 0; i <= R[0]; i++) {
		v = A[i + 2] - b - B[i + 2];  //��λ���
		if (A[i + 2] > 0)
			b = 0;
		if (v < 0) {  //�����λ
			v = v + 10;
			b = 1;
		}
		R[i + 2] = v % 10;
	}
	zerojustify(R);
}

void Mul(int A[], int B[], int R[])
{  //�����˷�R=AxB
	int tmp[N], row[N];
	int i, j;
	int2big(0, R);  //R=0
	assign(row, A);  //row=A
	for (i = 0; i <= B[0]; i++) {  //B��λ��A
		for (j = 1; j <= B[1 + 2]; j++) {  //������
			Add(R, row, tmp);  //R=R+row*B
			assign(R, tmp);
		}
		digitshift(row, 1);  //�´�row=row*10
	}
	R[1] = A[1] * B[1];  //�������ʱ�ķ���
	zerojustify(R);
}

void Div(int A[], int B[], int R[])
{  //��������R=A/B
	int tmp[N], row[N];
	int i, asign, bsign;
	int2big(0, R);  //R=0 
	R[1] = A[1] * B[1];  //�������ʱ�ķ���
	asign = A[1];  //����A�ķ���
	bsign = B[1];  //����B�ķ���
	A[1] = 1;  //A���ŵ���Ϊ��
	B[1] = 1;  //B���ŵ���Ϊ��
	int2big(0, row);  //row=0
	int2big(0, tmp);  //tmp=0
	R[0] = A[0];  //R��λ����ʼ��A��ͬ
	for (i = A[0]; i >= 0; i--) {
		digitshift(row, 1);  //row=row*10
		row[2] = A[i + 2];
		R[i + 2] = 0;
		while (compare(row, B) != 1) {  //row<B
			R[i + 2]++;  //���Ϊ�������
			Sub(row, B, tmp);  //��λ��� row-B
			assign(row, tmp);
		}
	}
	zerojustify(R);
	A[1] = asign;  //A���Ż�ԭ
	B[1] = bsign;  //B���Ż�ԭ
}

int main(void)
{  //���ô������㺯��������
	int c, A[N], B[N], R[N], Z[N];
	printf("a = ");
	scan(A);  //�������A
	printf("b = ");
	scan(B);  //�������B
	Add(A, B, R);  //��������ӷ�R=A+B
	printf("a + b = ");
	print(R);
	c = compare(A, B);  //�����Ƚ�
	printf("a %s b\n", c == 0 ? "==" : (c < 0 ? ">" : "<"));
	Sub(A, B, R);  //�����������R=A-B
	printf("a - b = ");
	print(R);
	Mul(A, B, R);  //��������˷�R=A*B
	printf("a * b = ");
	print(R);
	int2big(0, Z);
	if (compare(Z, B) == 0)  //���ܳ�0
		printf("a / b = NaN\n");
	else {
		Div(A, B, R);  //�����������R=A/B
		printf("a / b = ");
		print(R);
	}
	system("pause");
	return 0;
}