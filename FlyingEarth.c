#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define pi 3.141592653589793
#define e 0.0167
#define T 31558149.504
#define au_a 149602022500.0
#define Fi 0.4089888213840046

double dt=300;

double K_theta(double theta)
{
    return(2*pi/T*pow(1-e*e,-1.5)*(1-e*cos(theta))*(1-e*cos(theta)));
}

int usage_print()
{
    printf("Usage: FlyingEarth [option]\n\t[-h] or [-help] Display this information.\n");
    printf("\t[-angle initial_theta] Decide the initial angle (default 0) in the units of degree that relative to the Aphelion.\n");
    printf("\t[-dt time_interval] Change the time stepsize (default 300) in the units of sec.\n");
    printf("\t[-l steps_limit] or [-limit steps_limit] Define the maximum amount of computational steps as <step_limit>.\n");
    printf("\t[-o filename] Place the output into <filename>\n");
    return 0;
}

int main(int argc, char const *argv[])
{
    double k1,k2,k3,k4,r,theta=0,t=0;
    int pf=0,namepf=0,steplimit=50000;
    FILE *outf;

    if(argc>1)
    {
        pf=1;
        while(pf<argc)
        {
            
            if(!strcmp(argv[pf],"-h"))
            {
                usage_print();
                return 0;
            }else if(!strcmp(argv[pf],"-help")){
                usage_print();
                return 0;
            }else if(!strcmp(argv[pf],"--help")){
                usage_print();
                return 0;
            }else if(!strcmp(argv[pf],"-l")){
                pf++;steplimit=atoi(argv[pf]);
            }else if(!strcmp(argv[pf],"-limit")){
                pf++;steplimit=atoi(argv[pf]);
            }else if(!strcmp(argv[pf],"-o")){
                pf++;namepf=pf;
            }else if(!strcmp(argv[pf],"-angle")){
                pf++;theta=atof(argv[pf])*pi/180;
            }else if(!strcmp(argv[pf],"-dt")){
                pf++;dt=atof(argv[pf]);
            }else{
                usage_print();
                return 0;
            }
            pf++;
        }
        
    }
    if(namepf>0){outf=fopen(argv[namepf],"w");}else{outf=fopen("DATA_FE","w");}

    for(pf=0;pf<steplimit;pf++)
    {
        k1=dt*K_theta(theta);
        k2=dt*K_theta(theta+0.5*k1);
        k3=dt*K_theta(theta+0.5*k2);
        k4=dt*K_theta(theta+k3);
        theta+=(k1/6+k2/3+k3/3+k4/6);
        t+=dt;
        r=au_a*(1-e*e)/(1-e*cos(theta));
        fprintf(outf,"%lf\t%lf\t%lf\n",t,r,theta);
    }

    fclose(outf);
    printf("Done!\n");
    return 0;
}
