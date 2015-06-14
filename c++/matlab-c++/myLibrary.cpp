// CS1372 - Homework 10
// Akshay Patel
// gth708n
// myLibrary.cpp

#include "myLibrary.h"


void Print_V(vector v){
// Print vector v

	if(v.data != NULL){
		for(int i = 0; i < v.size; i++) 
		{
			printf("%f  ", v.data[i]);
		}
	}
	else 
	{	printf("\n\n");
		printf("Not a Vector\n");
	}
	printf("\n");

}

matrix operator+(matrix A, matrix B){
	matrix result = buildmatrix(A);
	for(int r = 0; r < A.rows; r++) {
		for(int c = 0; c < A.cols; c++ ) {
			result.data[r][c] = A.data[r][c] + B.data[r][c];
		}
	}
	return result;
}

matrix mult(matrix A, matrix B)
{
	matrix result = buildmatrix(A);
	for(int r = 0; r < A.rows; r++) {
		for(int c = 0; c < A.cols; c++ ) {
			result.data[r][c] = A.data[r][c] * B.data[r][c];
		}
	}
	return result;
}

matrix sqrt1(matrix A){
	matrix result = buildmatrix(A);
	for(int r = 0; r < A.rows; r++) {
		for(int c = 0; c < A.cols; c++ ) {
			result.data[r][c] = sqrt(A.data[r][c]);
		}
	}
	return result;
}
matrix applyFunction_sine(matrix A,  matrix B){
	matrix result = buildmatrix(A);
	for(int r = 0; r < A.rows; r++) {
		for(int c = 0; c < A.cols; c++ ) {
			result.data[r][c] = sin(A.data[r][c]) / B.data[r][c];
		}
	}
	return result;
}
matrix applyFunction_sin(matrix A)
{
	matrix result = buildmatrix(A);
	for(int r = 0; r < A.rows; r++) {
		for(int c = 0; c < A.cols; c++ ) {
			result.data[r][c] = sin(A.data[r][c]);
		}
	}
	return result;
}

matrix applyFunction_cos(matrix A){
	matrix result = buildmatrix(A);
	for(int r = 0; r < A.rows; r++) {
		for(int c = 0; c < A.cols; c++ ) {
			result.data[r][c] = cos(A.data[r][c]);
		}
	}
	return result;
}

matrix add1(matrix A){
	matrix result = buildmatrix(A);
	for(int r = 0; r < A.rows; r++) {
		for(int c = 0; c < A.cols; c++ ) {
			result.data[r][c] = A.data[r][c]+ 0.000001;
		}
	}
	return result;
}

vector sub(vector x, double B){
	vector result = buildvector(x.size);
	for(int r = 0; r < x.size; r++) 
	{
		result.data[r] = x.data[r] - B;
		}
	
	return result;
}
vector div(vector x, double B){
	vector result = buildvector(x.size);
	for(int r = 0; r < x.size; r++) 
	{
		result.data[r] = x.data[r]/B;
		}
	
	return result;
}

void print(matrix m) 
{
	for(int r = 0; r < m.rows; r++) 
	{
		for(int c = 0; c < m.cols; c++ ) 
		{
			printf("%8.3g", m.data[r][c]);
		}
	}
}

void myconcat(vector *v, double x[], int n){
	int i = 0;
	int p = v->size + n;
	
	double *temp = (double *) malloc( sizeof(double) * p );
	for(i = 0; i<v->size; i++){
		temp[i] = v->data[i];
	}
	for(int l = 0; l<n; l++){
		temp[i] = x[l];
		i++;
	}
free(v->data);
v->data = temp;
v->size = i;
}

vector myconcat(vector *v)
{
        vector V = buildvector(v->size-1);
        for(int i=0;i<V.size;i++){
                V.data[i]=v->data[i];
        }
        return V;
}

vector fv(vector x)
{
	vector coeff, fv;
	double v[] = {0.03333, -0.3, -1.3333, 16, 0, -187.2, 172.9};
	coeff.data = NULL;
	coeff.size = 0;
	myconcat(&coeff, v, 7);
	fv = polyval(coeff, x);
	return fv;
}

vector fi(vector x)
{
	vector coeff, v, icoeffs, ffi;
	double c1[] = {0.03333, -0.3, -1.3333, 16, 0, -187.2, 172.9};
	coeff.data = NULL;
	coeff.size = 0;
	myconcat(&coeff, c1, 7);
	v = colon(7, -1, 1);
	icoeffs = dotdiv(coeff, v);
	double a[]={0};
	myconcat(&icoeffs,a,1);
	ffi = polyval(icoeffs, x);
	return ffi;
}

vector fd(vector x)
{
	vector coeff, coeff2, v, v1, dcoeffs, fd;
	double c1[] = {0.03333, -0.3, -1.3333, 16, 0, -187.2, 172.9};
	coeff.data = NULL;
	coeff.size = 0;
	myconcat(&coeff, c1, 7);
	v = colon(7, -1, 1);
	v1 = buildvector(v.size-1);
	for(int i = 1; i < v.size ; i++)
	{
		v1.data[i-1] = v.data[i];
	}
	coeff2 = buildvector(coeff.size-1);
	for(int i = 0; i< coeff.size-1; i++)
	{
		coeff2.data[i] = coeff.data[i];
	}
	dcoeffs = dotmult(v1, coeff2);
	fd = polyval(dcoeffs, x);

	return fd;
}

vector mycumsum(vector A)
{
	vector temp;
	temp = buildvector(A.size);
	temp.data[0] = A.data[0];
	for(int i = 1; i < temp.size; i++)
	{
		temp.data[i] = temp.data[i-1] + A.data[i];

	}
	return temp;
}

vector mycumtrap(vector x)
{      
        vector y = buildvector(x.size);
        for(int j=0;j<x.size-2;j++){
                y.data[j]=x.data[j]+x.data[j+1];
                //printf("%f\n",y.data[j]);
        }
        vector temp = mycumsum(y);
        vector temp1 =buildvector(temp.size);
        for ( int i=0;i<temp.size;i++){
                temp1.data[i]=temp.data[i]/2;
        }
        vector y1 =buildvector(temp1.size+1);
        int k=0;        
        for (int l=0;l<y1.size;l++){
                if(l==0){
                        y1.data[l]=0;
                }
                else{
                        y1.data[l]=temp1.data[k];
                        k++;
                }
        }
        return y1;

}

vector myDiff(vector x)
{
        vector y = buildvector(x.size);
        for(int i=0;i<x.size-1;i++){
                y.data[i]=x.data[i+1]-x.data[i];
        }
        return y;
}

