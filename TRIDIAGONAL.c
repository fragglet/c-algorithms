#include<stdio.h>
#include<math.h>
#define SIZE 10                      //Choose the size of the matrix

void Tridiagonal(double A[SIZE][SIZE],double x[SIZE], double r[SIZE])
{   double a[SIZE]={0};
    double b[SIZE]={0};
    double d[SIZE]={0};
    for(int i=0;i<SIZE;i++)
        d[i]=A[i][i];                                    //This is an aglorithm to colve a tridiagonal matrix system of the form Ax=b
    for(int i=1;i<SIZE;i++)
        b[i]=A[i][i-1];
    for(int i=0;i<SIZE-1;i++)
        a[i]=A[i+1][i];
    a[0]=a[0]/d[0];
    r[0]=r[0]/d[0];
    for(int i=1;i<SIZE;i++)
    {
        a[i]=a[i]/(d[i]-b[i]*a[i-1]);
        r[i]=(r[i]-b[i]*r[i-1])/(d[i]-b[i]*a[i-1]);
    }
    x[SIZE-1]=r[SIZE-1];
    for(int i=SIZE-2;i>=0;i--)
        x[i]=r[i]-a[i]*x[i+1];

}

int main()
{	int A[SIZE][SIZE]={0};    //redefine a tridiagonal matrix A
	int b[SIZE]={0};           //define b
	double x[SIZE]={0};
	Tridiagonal(A,x,b);
	return 0;
}
