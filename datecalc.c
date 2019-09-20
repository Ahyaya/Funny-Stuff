#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define TTref_sec 43135.816

struct UTC_time
{
    int year,month,day,hr,min;
    double sec;
} d_time;

char *monv[12]={"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

int TypeDet(char * str)
{
    int pf=0;
    pf=0;while(*(str+pf)!='\0') if(*(str+pf++)=='@'){return 2;}
    pf=0;while(*(str+pf)!='\0') if(*(str+pf++)=='-'){return 1;}
    pf=0;while(*(str+pf)!='\0') if(*(str+pf++)=='s'){return 0;}
    pf=0;while(*(str+pf)!='\0') if(*(str+pf++)=='d'){return 0;}
    pf=0;while(*(str+pf)!='\0') if(*(str+pf++)=='h'){return 0;}
    pf=0;while(*(str+pf)!='\0') if(*(str+pf++)=='m'){return 0;}
    return -1;
}

int isLeapYear(int year)
{
    if(year%100)
    {
        if(year%4){return 0;}else{return 1;}
    }else{
        if(year%400){return 0;}else{return 1;}
    }
    return 0;
}

double UTC2TTsec(struct UTC_time time)
{
    int pf_year=2000,pf_month=1,temp_day=0,pf=isLeapYear(time.year);
    int daylist[2][13]={0,31,28,31,30,31,30,31,31,30,31,30,31,0,31,29,31,30,31,30,31,31,30,31,30,31};
    double TTsec=time.sec+60.0*time.min+3600.0*time.hr+86400.0*(time.day-1)-TTref_sec;
    while(pf_month<time.month)
    {
        temp_day+=daylist[pf][pf_month++];
    }
    while(pf_year<time.year)
    {
        temp_day+=(365+isLeapYear(pf_year++));
    }
    TTsec+=86400.0*temp_day;

    return TTsec;
}

double Utrans(char *str)
{
    int yy,mm,dd,hr,min,pf=0,pf_buf=0,pf_date=0,pf_tick=0;
    double sec,TTsec=0;
    char buf[64];
    memset(buf,0,64);
    if(TypeDet(str)==0)
    {
        pf=0;pf_buf=0;
        while(*(str+pf)!='\0')
        {
            if((*(str+pf)>47&&*(str+pf)<58)||*(str+pf)=='.'){buf[pf_buf++]=*(str+pf);}
            if(*(str+pf)=='d'){TTsec+=86400*atof(buf);memset(buf,0,64);pf_buf=0;}
            if(*(str+pf)=='h'){TTsec+=3600*atof(buf);memset(buf,0,64);pf_buf=0;}
            if(*(str+pf)=='m'){TTsec+=60*atof(buf);memset(buf,0,64);pf_buf=0;}
            if(*(str+pf)=='s'){TTsec+=atof(buf);memset(buf,0,64);pf_buf=0;}
            pf++;
        }
    }else if(TypeDet(str)==1){
        pf=0;pf_buf=0;pf_date=0;hr=0;min=0;sec=0;
        while(*(str+pf)!='\0')
        {
            if(*(str+pf)>47&&*(str+pf)<58){buf[pf_buf++]=*(str+pf);}
            if(*(str+pf)=='-')
            {
                if(pf_date==0){yy=atoi(buf);memset(buf,0,64);pf_buf=0;}
                if(pf_date==1){mm=atoi(buf);memset(buf,0,64);pf_buf=0;}
                pf_date++;
            }
            pf++;
        }
        dd=atoi(buf);memset(buf,0,64);pf_buf=0;
        struct UTC_time date={yy,mm,dd,hr,min,sec};
        TTsec=UTC2TTsec(date);
    }else if(TypeDet(str)==2){
        pf=0;pf_buf=0;pf_date=0;pf_tick=0;hr=0;min=0;sec=0;
        while(*(str+pf)!='\0')
        {
            if((*(str+pf)>47&&*(str+pf)<58)||*(str+pf)=='.'){buf[pf_buf++]=*(str+pf);}
            if(*(str+pf)=='-')
            {
                if(pf_date==0){yy=atoi(buf);memset(buf,0,64);pf_buf=0;}
                if(pf_date==1){mm=atoi(buf);memset(buf,0,64);pf_buf=0;}
                pf_date++;
            }
            if(*(str+pf)=='@'){dd=atoi(buf);memset(buf,0,64);pf_buf=0;}
            if(*(str+pf)==':')
            {
                if(pf_tick==0){hr=atoi(buf);memset(buf,0,64);pf_buf=0;}
                if(pf_tick==1){min=atoi(buf);memset(buf,0,64);pf_buf=0;}
                pf_tick++;
            }
            pf++;
        }
        sec=atof(buf);memset(buf,0,64);pf_buf=0;
        struct UTC_time date={yy,mm,dd,hr,min,sec};
        TTsec=UTC2TTsec(date);
    }else{printf("syntax error!\n");return 0;}
    return TTsec;
}

int TTsec2UTC(double t)
{
    struct UTC_time time={2000,1,1,11,58,55.816};
    int pf=0,yearType[2]={365,366},flag[5]={0};
    int daylist[2][13]={0,31,28,31,30,31,30,31,31,30,31,30,31,0,31,29,31,30,31,30,31,31,30,31,30,31};
    double temp=86400.0*yearType[isLeapYear(time.year)];
    while(t>temp){time.year++;t-=temp;temp=86400.0*yearType[isLeapYear(time.year)];}
    pf=isLeapYear(time.year);temp=86400.0*daylist[pf][time.month];
    while(t>temp&&time.month<12){time.month++;t-=temp;temp=86400.0*daylist[pf][time.month];}
    temp=0;pf=daylist[pf][time.month];
    while(t>86400&&time.day<pf){time.day++;t-=86400;}
    while(t>3600&&time.hr<23){time.hr++;t-=3600;}
    while(t>60&&time.min<59){time.min++;t-=60;}
    time.sec+=t;
    while(flag[0]*flag[1]*flag[2]*flag[3]*flag[4]<1)
    {
        if(time.sec<60){flag[0]=1;}else{flag[0]=0;time.sec-=60;time.min++;}
        if(time.min<60){flag[1]=1;}else{flag[1]=0;time.min-=60;time.hr++;}
        if(time.hr<24){flag[2]=1;}else{flag[2]=0;time.hr-=24;time.day++;}
        if(time.day<=daylist[isLeapYear(time.year)][time.month]){flag[3]=1;}else{flag[3]=0;time.day=1;time.month++;}
        if(time.month<=12){flag[4]=1;}else{flag[4]=0;time.month=1;time.year++;}
    }
    d_time=time;
    return 0;
}

int usage_print()
{
    printf("Usage: datecalc [Time_1] [+/-] [Time_2]\n\nExample:\n1.\tdatecalc 2025-5-24 - 2019-10-1\n");
    printf("2.\tdatecalc 2019-10-1@8:30:0 - 50h30m25.5s\n");
    printf("3.\tdatecalc 2019-10-1 - 7d23.3h\n\t...\nPlease mind the [SPACE] between date and operator.\n");
    return 0;
}

int main(int argc, char *argv[])
{
    double date_0,date_1=0,TT_result=0,rest_sec=0;
    int date_operator=1,type_0=-1,type_1=-1;
    int rest_day=0,rest_hr=0,rest_min=0;
    if(argc==2)
    {
        if(!strcmp(argv[1],"-h")||!strcmp(argv[1],"-help"))
        {
            usage_print();
            return 0;
        }
        type_0=TypeDet(argv[1]);type_1=0;
        if(type_0<0){printf("syntax error!\n");usage_print();return 0;}else{date_0=Utrans(argv[1]);date_1=0;date_operator=-1;}
    }else if(argc==4)
    {
        type_0=TypeDet(argv[1]);type_1=TypeDet(argv[3]);
        if(type_0<0||type_1<0){printf("syntax error!\n");usage_print();return 0;}else{date_0=Utrans(argv[1]);date_1=Utrans(argv[3]);}
        date_operator=(*argv[2]=='+')?(1):(-1);
    }else{usage_print();return 0;}

    TT_result=date_0+date_operator*date_1;
    if(type_0*type_1)
    {
        if(date_operator>0){printf("illegal operate!\n");usage_print();return 0;}
        rest_day=(int)(TT_result)/86400;rest_hr=(int)(TT_result-rest_day*86400)/3600;rest_min=(int)(TT_result-rest_day*86400-rest_hr*3600)/60;rest_sec=TT_result-rest_day*86400-rest_hr*3600-rest_min*60;
        printf("Time left: ");
        if(rest_day>0){printf("%ddays ",rest_day);}
        printf("%dhr %dmin %.2lfsec\n",rest_hr,rest_min,rest_sec);
        printf("Total sec left: %.2lfsec\n",TT_result);
        return 0;
    }else{
        if(date_operator<0)
        {
            rest_day=(int)(TT_result)/86400;rest_hr=(int)(TT_result-rest_day*86400)/3600;rest_min=(int)(TT_result-rest_day*86400-rest_hr*3600)/60;rest_sec=TT_result-rest_day*86400-rest_hr*3600-rest_min*60;
            printf("Time left: ");
            if(rest_day>0){printf("%ddays ",rest_day);}
            printf("%dhr %dmin %.2lfsec\n",rest_hr,rest_min,rest_sec);
            printf("Total sec left: %.2lfsec\n",TT_result);
            if(type_1>0){return 0;}
        }
        if(TT_result>0)
        {
            TTsec2UTC(TT_result);
            printf("UTC Destination:\n%d %s %d @ %d:%d:%.2lf\n",d_time.year,monv[d_time.month-1],d_time.day,d_time.hr,d_time.min,d_time.sec);
            return 0;
        }
    }
    printf("unexpected error!\n");
    return 0;
}
