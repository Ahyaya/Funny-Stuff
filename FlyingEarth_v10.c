#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define pi 3.141592653589793
#define e 0.0167
#define T 31558149.504
#define au_a 149602022500.0
#define Fi 0.4089888213840046
#define fs -0.2207167206
#define Earth_Rotation 7.29212351699037e-05

double dt=20,Exp_time=2592000;
struct observ
{
    double t,theta,alpha,delta;
};

double K_theta(double theta)
{
    return(2*pi/T*pow(1-e*e,-1.5)*(1-e*cos(theta))*(1-e*cos(theta)));
}

int usage_print()
{
    printf("Usage: FlyingEarth [option]\n\t[-h] or [-help] Display this information.\n");
    printf("\t[-angle initial_theta] Decide the initial angle (default 0) in the units of degree that relative to the Aphelion.\n");
    printf("\t[-dt time_interval] Change the time stepsize (default 20) in the units of sec.\n");
    printf("\t[-l steps_limit] or [-limit steps_limit] Define the maximum amount of computational steps as <step_limit>.\n");
    printf("\t[-o filename] Place the output into <filename>\n");
    return 0;
}

int main(int argc, char const *argv[])
{
    struct observ ref={615553864.184,0,-2.92342649709050,0.398497821984698};
    double k1,k2,k3,k4;
    int pf=0,namepf=0,steplimit=1000000,degtran_0[3],degtran_1[3],degtran_2[3],sg_1=1,sg_2=1;
    FILE *outf;

    double r,theta=ref.theta,t=ref.t,alpha=ref.alpha,delta=ref.delta;

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
    dt=Exp_time/steplimit;printf("\nComputing...\n\n");
    for(pf=0;pf<steplimit;pf++)
    {
        k1=dt*K_theta(theta);
        k2=dt*K_theta(theta+0.5*k1);
        k3=dt*K_theta(theta+0.5*k2);
        k4=dt*K_theta(theta+k3);
        theta+=(k1/6+k2/3+k3/3+k4/6);
        t+=dt;
        r=au_a*(1-e*e)/(1-e*cos(theta));
        alpha=(theta-ref.theta)-Earth_Rotation*(t-ref.t)+ref.alpha;
        while(alpha+pi<0){alpha+=2*pi;}
        delta=asin(sin(Fi)*(cos(fs)*cos(theta)+sin(fs)*sin(theta)));
        
        fprintf(outf,"%.3lf\t%lf\t%.10lf\t%.10lf\t%.10lf\n",t,r,theta,alpha,delta);
    }

    fclose(outf);
    printf("Done!\n\n");

    sg_1=(alpha>0)?(1):(-1);
    sg_2=(delta>0)?(1):(-1);
    alpha*=sg_1;delta*=sg_2;
    degtran_0[0]=(int) (theta*180/pi);degtran_0[1]=(int) 60*(theta*180/pi-degtran_0[0]);degtran_0[2]=(int) 60*(60*(theta*180/pi-degtran_0[0])-degtran_0[1]);
    degtran_1[0]=(int) (alpha*180/pi);degtran_1[1]=(int) 60*(alpha*180/pi-degtran_1[0]);degtran_1[2]=(int) 60*(60*(alpha*180/pi-degtran_1[0])-degtran_1[1]);
    degtran_2[0]=(int) (delta*180/pi);degtran_2[1]=(int) 60*(delta*180/pi-degtran_2[0]);degtran_2[2]=(int) 60*(60*(delta*180/pi-degtran_2[0])-degtran_2[1]);
    printf("Terrestrial Time: %.3lf\nDistance: %.3lf km\nTheta: %d°%d\'%d\"\n",Exp_time,r/1000,degtran_0[0],degtran_0[1],degtran_0[2]);
    printf("Alpha: %d°%d\'%d\"\n",sg_1*degtran_1[0],degtran_1[1],degtran_1[2]);
    printf("Delta: %d°%d\'%d\"\n",sg_2*degtran_2[0],degtran_2[1],degtran_2[2]);
    return 0;
}
