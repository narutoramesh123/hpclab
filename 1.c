#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define NRA 62
#define NCA 15
#define NCB 7
int main (int argc, char *argv[])
{
int
tid, nthreads, i, j, k, chunk;
double a[NRA][NCA],
/* matrix A to be multiplied */
b[NCA][NCB],
/* matrix B to be multiplied */
c[NRA][NCB];
/* result matrix C */
/* number of rows in matrix A */
/* number of columns in matrix A */
/* number of columns in matrix B */
chunk = 10;
/*** Spawn a parallel region explicitly scoping all variables ***/
#pragma omp parallel shared(a,b,c,nthreads,chunk) private(tid,i,j,k)
{
tid = omp_get_thread_num();
if (tid == 0)
{
nthreads = omp_get_num_threads();
printf(&quot;Starting matrix multiple example with %d threads\n&quot;,nthreads);
printf(&quot;Initializing matrices...\n&quot;);
}
/*** Initialize matrices ***/
#pragma omp for schedule (static, chunk)
for (i=0; i&lt;NRA; i++)
for (j=0; j&lt;NCA; j++)
a[i][j]= i+j;
#pragma omp for schedule (static, chunk)
for (i=0; i&lt;NCA; i++)
for (j=0; j&lt;NCB; j++)
b[i][j]= i*j;
#pragma omp for schedule (static, chunk)
for (i=0; i&lt;NRA; i++)
for (j=0; j&lt;NCB; j++)
c[i][j]= 0;
/*** Do matrix multiply sharing iterations on outer loop ***/

/*** Display who does which iterations for demonstration purposes ***/
printf(&quot;Thread %d starting matrix multiply...\n&quot;,tid);
#pragma omp for schedule (static, chunk)
for (i=0; i&lt;NRA; i++)
{
printf(&quot;Thread=%d did row=%d\n&quot;,tid,i);
for(j=0; j&lt;NCB; j++)
for (k=0; k&lt;NCA; k++)
c[i][j] += a[i][k] * b[k][j];
}
} /*** End of parallel region ***/
/*** Print results ***/
/* set loop iteration chunk size */
printf(&quot;******************************************************\n&quot;);
printf(&quot;Result Matrix:\n&quot;);
for (i=0; i&lt;NRA; i++)
{
for (j=0; j&lt;NCB; j++)
printf(&quot;%6.2f &quot;, c[i][j]);
printf(&quot;\n&quot;);
}
printf(&quot;******************************************************\n&quot;);
printf (&quot;Done.\n&quot;);
}