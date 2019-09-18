#include<stdio.h>
#include<stdlib.h>
#include<math.h>

long * primelist;

int isPrime(long N)
{
    long pf=0, local_limit=sqrt(N);
    local_limit++;
    while(*(primelist+pf)<local_limit)
    {
        if(N%(*(primelist+pf))){pf++;}else{return 0;}
    }
    return 1;
}

int main(int argc, char const *argv[])
{
    FILE *outf;
    long pf=0,limit_0=1000000,limit_1=2;
    if(argc>1)
    {
        if(*argv[1]=='-')
        {
            printf("Usage: Primlist [count_limit]\n");return 0;
        }else{
            limit_0=atol(argv[1]);printf("Reset count limit as: %ld\n",limit_0);
        }
    }else{printf("Usage: Primlist [count_limit]\nNow using default setting limit_0=%ld\n",limit_0);}
    primelist=(long *)malloc(sizeof(long)*limit_0);
    primelist[pf]=limit_1++;
    outf=fopen("PrimeList","w");
    fprintf(outf,"2\n");
    for(;pf<limit_0;)
    {
        if(isPrime(limit_1)){*(primelist+(++pf))=limit_1;fprintf(outf,"%ld\n",limit_1);}
        limit_1++;
    }
    fclose(outf);
    free(primelist);
    printf("Completed!\n");
    return 0;
}
