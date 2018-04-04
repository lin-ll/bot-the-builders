#include <stdio.h>
#include <math.h>
#include "matrixInverse.h"

double determinant(double [][NUM], double);
void cofactor(double [][NUM], double);
void transpose(double [][NUM], double [][NUM], double);

//static double matrixInverse[NUM*NUM] = {0};
static double inverse[NUM][NUM] = {0};

int main_inv(double *matrix);

void invert(double *matrix) {
    main_inv(matrix);
    for (int i = 0; i < NUM; i++) {
        for (int j = 0; j < NUM; j++) {
            int index = i * NUM + j;
            matrix[index] = inverse[i][j];
        }
    }
}

int main_inv(double *matrix)
{
  double a[NUM][NUM], k, d;
  int i, j;

  k = NUM;

  for (i = 0; i < k; i++) {
      for (j = 0; j < k; j++) {
          int index = i * k + j;
          a[i][j] = matrix[index];
      }
  }
  
  d = determinant(a, k);
  if (d == 0){
    printf("Matrix not invertible!");
   return -1;
  }
  else{
   cofactor(a, k);
   return 0;
  }
}

/*For calculating Determinant of the Matrix */
double determinant(double a[NUM][NUM], double k)
{
  double s = 1, det = 0, b[NUM][NUM];
  int i, j, m, n, c;
  if (k == 1)
    {
     return (a[0][0]);
    }
  else
    {
     det = 0;
     for (c = 0; c < k; c++)
       {
        m = 0;
        n = 0;
        for (i = 0;i < k; i++)
          {
            for (j = 0 ;j < k; j++)
              {
                b[i][j] = 0;
                if (i != 0 && j != c)
                 {
                   b[m][n] = a[i][j];
                   if (n < (k - 2))
                    n++;
                   else
                    {
                     n = 0;
                     m++;
                     }
                   }
               }
             }
          det = det + s * (a[0][c] * determinant(b, k - 1));
          s = -1 * s;
          }
    }

    return (det);
}

void cofactor(double num[NUM][NUM], double f)
{
 double b[NUM][NUM], fac[NUM][NUM];
 int p, q, m, n, i, j;
 for (q = 0;q < f; q++)
 {
   for (p = 0;p < f; p++)
    {
     m = 0;
     n = 0;
     for (i = 0;i < f; i++)
     {
       for (j = 0;j < f; j++)
        {
          if (i != q && j != p)
          {
            b[m][n] = num[i][j];
            if (n < (f - 2))
             n++;
            else
             {
               n = 0;
               m++;
               }
            }
        }
      }
      fac[q][p] = pow(-1, q + p) * determinant(b, f - 1);
    }
  }
  transpose(num, fac, f);
}
/*Finding transpose of matrix*/
void transpose(double num[NUM][NUM], double fac[NUM][NUM], double r)
{
  int i, j;
  double b[NUM][NUM], d;

  for (i = 0;i < r; i++)
    {
     for (j = 0;j < r; j++)
       {
         b[i][j] = fac[j][i];
        }
    }
  d = determinant(num, r);
  for (i = 0;i < r; i++)
    {
     for (j = 0;j < r; j++)
       {
        inverse[i][j] = b[i][j] / d;
        }
    }
}
