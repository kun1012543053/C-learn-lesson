#include <stdio.h>
#include <stdlib.h>

int hour, minute, second;

void Update(void)
{
	second++;
	if (second == 60) {
		second = 0;
		minute++;
	}
	if (minute == 60) {
		minute = 0;
		hour++;
	}
	if (hour == 24) {
		hour = 0;
	}
}

void Display(void)
{
	printf("%2d:%2d:%2d \r", hour, minute, second);
}

void Delay(void)
{
	long t;
	for (t = 0; t < 700000000; t++);
}

int main(void)
{
	long i;
	hour = 0, minute = 0; second = 0;
	for (i = 0; i < 10000; i++) {
		Update();
		Display();
		Delay();
	}
}
