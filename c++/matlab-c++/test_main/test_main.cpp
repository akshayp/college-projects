// test_main.cpp : Defines the entry point for the console application.
// CS1372 - Homework 10
// Akshay Patel
// gth708n
// test_main.cpp

#define _CRT_SECURE_NO_DEPRECATE 1
#include "leaks.h"
#include "test_main.h"
#include "myLibrary.h"

#ifdef _DEBUG
FILE *debug;
#endif

int main(int argc, char* argv[])
{
	#ifdef _DEBUG
	debug = fopen("debug.log","w");
#endif



// PROBLEM 1
vector x22;
matrix xx, yy, zz, a2, a3, a4, rr, a6;
x22 = linspace(-20,20);
meshgrid(x22, x22, &xx, &yy);
a2 = mult(xx, xx);
a3 = mult(yy, yy);
a4 = a2 + a3;
rr = sqrt1(a4);
a6 = add1(rr);
zz = applyFunction_sine(rr, a6);
mesh(xx, yy, zz);





//PROBLEM #2


vector x1, th1, r;
x1 = linspace(0, 8.5, 20);
th1 = linspace(0, 2*PI, 20);

matrix xx1, tth, rr1, yy1, zz1, yy2, zz2, temp;
meshgrid(x1, th1, &xx1, &tth);
r = fv(x1);
meshgrid(r, r, &rr1, &temp);
yy2 = applyFunction_cos(tth);
zz2 = applyFunction_sin(tth);
yy1 = mult(rr1, yy2);
zz1 = mult(rr1, zz2);
mesh(xx1, yy1, zz1);



vector r2;
matrix rr2, tth2, zz3, yy5, xx5, temp2;
r2 = x22;
meshgrid(r2, th1, &rr2, &tth2);
xx5 = mult(xx1, yy2);
yy5 = mult(xx1, zz2);
meshgrid(r,x1, &zz3, &temp2); 
mesh(xx5, yy5, zz3);


//PROBLEM #3
vector x, y, iy;
x = linspace(-6.5,8.5);
y = fv(x);
plot(x, y);
hold(false);

iy = fi(x);
plot(x,iy, "g");
hold(true);


vector y3, eiy, eiy1;
double dx, dif; 
dx = x.data[1] - x.data[0];
y3 = mycumsum(y);
eiy = dotmult(y3, dx);
dif = eiy.data[0] - iy.data[0];
eiy1 = sub(eiy, dif);
plot(x, eiy1, "r");
hold(true);


vector eiy2, eiy3, y4;
double dif2;

y4 = mycumtrap(y);
eiy2 = dotmult(y4, dx);
dif2 = eiy2.data[0] - iy.data[0];
eiy3 = sub(eiy2, dif2);
plot(x, eiy3, "c");
hold(false);



//problem #4

vector dy, dy1, dy2, x3, x4;
double dx1;
dy = fd(x);
plot(x, dy);
hold(true);

dy1 = myDiff(y);
dy2=div(dy1,dx);
x3=myconcat(&x);
dx1=dx/2;
x4= operator+(x3,dx1);
plot(x4,dy2,"r");
hold(false);
printf("PLEASE CLOSE ALL FIGURES AND PRESS ENTER SO THAT PROGRAM HAS NO LEAKS");
getchar();

vfree(x22);
mfree(xx);
mfree(yy);
mfree(zz);
mfree(a2);
mfree(a3);
mfree(a4);
mfree(rr);
mfree(a6);

vfree(x1);
vfree(th1); 
vfree(r);
mfree(xx1);
mfree(tth);
mfree(rr1);
mfree(yy1);
mfree(zz1);
mfree(yy2);

mfree(zz2);
mfree(temp);
mfree(temp2);
mfree(rr2);
mfree(tth2);
mfree(zz3);
mfree(yy5);
mfree(xx5); 

vfree(x);
vfree(y);
vfree(iy);
vfree(y3);
vfree(eiy);
vfree(eiy1);
vfree(eiy2);
vfree(eiy3);
vfree(y4);


vfree(dy);
vfree(dy1);
vfree(dy2);
vfree(x3); 
vfree(x4);

#ifdef _DEBUG
	fflush(debug);
	fclose(debug); 
#endif
	return 0;
}
