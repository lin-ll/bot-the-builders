#include<stdio.h>
#include<math.h>

# define SIZE 6

double determinant(double [][SIZE], double);
void cofactor(double [][SIZE], double);
void transpose(double [][SIZE], double [][SIZE], double);

static double matrixInverse[SIZE*SIZE] = {0};
static double inverse[SIZE][SIZE] = {0};

double *inverse(double *matrix) {
    main(matrix, SIZE);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            int index = i * SIZE + j;
            matrixInverse[index] = inverse[i][j];
        }
    }

    return matrixInverse;
}

int main(matrix, SIZE)
{
  double a[SIZE][SIZE], k, d;
  int i, j;

  k = SIZE;

  for (i = 0; i < k; i++) {
      for (j = 0; j < k; j++) {
          int index = i * k + j;
          a[i][j] = matrix[index];
      }
  }
  
  d = determinant(a, k);
  if (d == 0)
   return -1;
  else
   cofactor(a, k);
}

/*For calculating Determinant of the Matrix */
double determinant(double a[SIZE][SIZE], double k)
{
  double s = 1, det = 0, b[SIZE][SIZE];
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

void cofactor(double num[SIZE][SIZE], double f)
{
 double b[SIZE][SIZE], fac[SIZE][SIZE];
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
void transpose(double num[SIZE][SIZE], double fac[SIZE][SIZE], double r)
{
  int i, j;
  double b[SIZE][SIZE], d;

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
