//DATA.C

#include "screens.h"

Levels L[5]={0};

void SetData(Gamelevels thislevel, int *score){
    //General variables:
    for(int i=1;i<=4;i++){

        L[i].zombie_speed=0.5;
        L[i].seconds_per_Zombie=5;

        L[i].FlagZombie_Speed=0.5;
        L[i].seconds_per_FlagZombie=6;
    }



    //detailed variables:

    //مطمئن نیستم بازی در مراحل دیگه با 200 امتیاز شروع میشد یا صفرو من 200 گذاشتم که اولش که ستاره جمع نشده یکم ذخیره شده باشه
    switch (thislevel)
    {
    case level1:
        L[1].TotalFlagZmb=0;

        for(int i=0;i<30;i++)
            flagZombie[i]=(zombie){0};

        L[1].TotalNormalZmb=30;
        *score=200;

        break;



    case level2:
        L[2].TotalFlagZmb=30;
        L[2].TotalNormalZmb=0;
        for(int i=0;i<30;i++)
            NormalZombie[i]=(zombie){0};
        *score=2000;
        
        for(int i=0;i<5;i++){
            myLawnMower[i].isgone=true;
            myLawnMower[i].rec=(Rectangle){0,0,0,0};
            updateweight(i+1);
        }
        break;



    case level3:
        L[3].TotalFlagZmb=15;
        L[3].TotalNormalZmb=15;
        *score=200;


        for(int i=0;i<46;i++){
            myStar[i].isActive=false;}
        sbplant[SUN_FLOWER].isAvailable=false;
        break;



    case level4:

        L[4].TotalFlagZmb=30;
        L[4].TotalNormalZmb=30;
        break;

    default:
        break;
    }
    
}