# include <stdio.h>
# include <math.h>

double geo_c(double z, int n)
{
    double geo_sum = 0;
    int i;
    for (i=0; i<n; i++){
        /* pow(x,y) function raises x to the power of y - it is from <math.h> */
        geo_sum += pow(z,i);
     }
    return geo_sum;
}