//zombies.c


#include "screens.h"



//----------------Normal Zombie-------------------

static Texture2D *zombieTextures = NULL;  // Shared texture array
static int zombieTextureCount = 60;
static float CallingZombieTimer=0;
zombie NormalZombie[30]={0};
Sound chomping;

void initZombie(Gamelevels thislevel){

    if (zombieTextures == NULL) {
        zombieTextures = (Texture2D*)malloc(sizeof(Texture2D) * zombieTextureCount);
        
        for(int k = 0; k < zombieTextureCount; k++) {
            zombieTextures[k] = LoadTexture(TextFormat("sprites\\animation\\myZombies\\Imp Mummy\\Walking1\\walk00%02d.png", k));
        }
    }

    chomping=LoadSound("sprites\\resources - voix\\SFX chomp.ogg");
    for(int i=0;i<L[thislevel].TotalNormalZmb;i++){
        NormalZombie[i].textures=zombieTextures;

        NormalZombie[i].health=0;
        NormalZombie[i].activated=false;
        NormalZombie[i].rec=(Rectangle){0,0,0,0};
        NormalZombie[i].speed=L[thislevel].zombie_speed;
        NormalZombie[i].animation=0;
        NormalZombie[i].animation_frame=0;
        NormalZombie[i].row=0;
        NormalZombie[i].cooldown=0;
        NormalZombie[i].isdead=false;
    }
}

void CallTheZombies(int *i,Gamelevels thislevel){
    CallingZombieTimer += GetFrameTime();

    if (*i==L[thislevel].TotalNormalZmb-1 && NormalZombie[*i].activated) return;

    if (CallingZombieTimer>L[thislevel].seconds_per_Zombie  && *i<L[thislevel].TotalNormalZmb){

        NormalZombie[*i].health=100;
        NormalZombie[*i].activated=true;
        CallingZombieTimer=0;

        NormalZombie[*i].row=GetRandomValue(1,5);
        NormalZombie[*i].rec=(Rectangle){
            1350,
            NormalZombie[*i].row*100+35,
            2,
            85
        };
        
        if (*i==L[thislevel].TotalNormalZmb-1) return;
        *i += 1;
    }
}


void UpdateZombies(int i,bool *gameover,Gamelevels thislevel){

    if(NormalZombie[i].health<=0){
        NormalZombie[i].rec=(Rectangle){0,0,0,0};
        return;
    }

    if (NormalZombie[i].health>0){
        NormalZombie[i].rec.x -= NormalZombie[i].speed;

        int LMindex=((NormalZombie[i].rec.y/100)-1);

        if (LMindex >= 0 && LMindex < 5) {
            if(CheckCollisionRecs(myLawnMower[LMindex].rec,NormalZombie[i].rec)){
            NormalZombie[i].health=0;
            NormalZombie[i].isdead=true;
            NormalZombie[i].rec=(Rectangle){0,0,0,0};

            myLawnMower[LMindex].ishere=false;
            }
        }
        

        if (NormalZombie[i].rec.x <= 300 && NormalZombie[i].health>0)
            *gameover=true;
    }
    
    //EAT -------
    NormalZombie[i].speed = L[thislevel].zombie_speed;

    for ( int s=0;s<45;s++){
        if(NormalZombie[i].health>0 && mySunflower[s].planted ){
            if ( CheckCollisionRecs(NormalZombie[i].rec,mySunflower[s].rec)){

                if(mySunflower[s].health>0){
                    NormalZombie[i].speed = 0;
                    mySunflower[s].health -= damage_Zombie;
                    PlaySound(chomping);
                    
                }else{
                    mySunflower[s].planted=false;
                }
                if(mySunflower[s].health<=0){
                    mySunflower[s].planted=false;
                    mySunflower[s].health=0;
                }

                #if DEBUG2
                    DrawRectangleLinesEx(mySunflower[s].rec,5,RED);
                #endif
            } 
        }
    } 

    for ( int p=0;p<45;p++){
        if(NormalZombie[i].health>0 && myPeaShooter[p].health>0 ){
            if ( CheckCollisionRecs(NormalZombie[i].rec,myPeaShooter[p].rec)){
                
                myrow[NormalZombie[i].row].BeChecked=true;
                updateweight(NormalZombie[i].row);
                if(myPeaShooter[p].health>0){
                    NormalZombie[i].speed = 0;
                    myPeaShooter[p].health -= damage_Zombie;
                    PlaySound(chomping);
                    
                }else{
                    myPeaShooter[p].planted=false;
                }


                #if DEBUG2
                    DrawRectangleLinesEx(myPeaShooter[p].rec,5,RED);
                #endif
            } 
        }
    }
    
    for ( int c=0;c<45;c++){
        if(NormalZombie[i].health>0 && mychomper[c].health>0 ){
            if ( CheckCollisionRecs(NormalZombie[i].rec,mychomper[c].rec)){

                myrow[NormalZombie[i].row].BeChecked=true;
                updateweight(NormalZombie[i].row);
                if(mychomper[c].health>0){
                    NormalZombie[i].speed = 0;
                    mychomper[c].health -= damage_Zombie;
                    PlaySound(chomping);
                    
                }else{
                    mychomper[c].planted=false;
                    mychomper[c].health=0;
                }

                #if DEBUG2
                    DrawRectangleLinesEx(mychomper[c].rec,5,RED);
                #endif
            } 
        }
    }
    for ( int b=0;b<45;b++){
        if(NormalZombie[i].health>0 && mypotato[b].health>0 ){
            if ( CheckCollisionRecs(NormalZombie[i].rec,mypotato[b].rec)){

                myrow[NormalZombie[i].row].BeChecked=true;
                updateweight(NormalZombie[i].row);
                if(mypotato[b].health>0){
                    NormalZombie[i].speed = 0;
                    mypotato[b].health -= damage_Zombie;
                    PlaySound(chomping);
                    
                }else{
                    mypotato[b].planted=false;
                    mypotato[b].health=0;
                }

                #if DEBUG2
                    DrawRectangleLinesEx(mypotato[b].rec,5,RED);
                #endif
            } 
        }
    }
}


void drawZombie(int i){
    NormalZombie[i].animation += GetFrameTime();
    if (NormalZombie[i].animation>= 0.04) {
        NormalZombie[i].animation_frame += 1;
        NormalZombie[i].animation=0;
    }
    if (NormalZombie[i].animation_frame>=zombieTextureCount)
        NormalZombie[i].animation_frame=0;

    

    if (NormalZombie[i].health>0){
        DrawTexture(
            NormalZombie[i].textures[NormalZombie[i].animation_frame],
            NormalZombie[i].rec.x-50,
            NormalZombie[i].rec.y-30,
            WHITE);
    }

        #if DEBUG2
        DrawRectangleLinesEx(NormalZombie[i].rec,5,BLUE);
        /*DrawRectangleLinesEx(NormalZombie[0].rec,5,WHITE);
        DrawRectangleLinesEx(NormalZombie[1].rec,5,PURPLE);
        DrawRectangleLinesEx(NormalZombie[2].rec,5,PINK);*/
        #endif

        #if DEBUG1 
        int LMindex=(((NormalZombie[i].rec.y-35)/100)-1);
        DrawRectangleLinesEx(myLawnMower[LMindex].rec,5,PINK);
        DrawRectangleLinesEx(NormalZombie[i].rec,5,BLUE);
        
        #endif
}


//---------------------------------------------------------------------------------------------------
//============FLAG ZOMBIE====================
//---------------------------------------------------------------------------------------------------

weight myrow[7]={0};




static Texture2D *FzombieTextures = NULL;  // Shared texture array
static int FzombieTextureCount = 90;
static float CallingFZombieTimer=0;
zombie flagZombie[30]={0};
Sound fchomping;

void initFlagZombie(Gamelevels thislevel){

        // Load textures only once
    if (FzombieTextures == NULL) {
        FzombieTextures = (Texture2D*)malloc(sizeof(Texture2D) * FzombieTextureCount);
        
        for(int k = 0; k < FzombieTextureCount; k++) {
            FzombieTextures[k] =LoadTexture(TextFormat("sprites\\animation\\myZombies\\Flag Zombie\\walk\\walk%04d.png", k));
            //FzombieTextures[k] =LoadTexture(TextFormat("sprites\\animation\\myZombies\\Weasel Hoarder\\Weasel Release1\\weasel_attack%04d.png", k));
        }
    }

    fchomping=LoadSound("sprites\\resources - voix\\SFX chomp2.ogg");
    for(int i=0;i<L[thislevel].TotalFlagZmb;i++){
        flagZombie[i].textures=FzombieTextures;

        flagZombie[i].health=0;
        flagZombie[i].activated=false;
        flagZombie[i].rec=(Rectangle){0,0,0,0};
        flagZombie[i].speed=L[thislevel].FlagZombie_Speed;
        flagZombie[i].animation=0;
        flagZombie[i].animation_frame=0;
        flagZombie[i].row=0;
        flagZombie[i].cooldown=5;
        flagZombie[i].isdead=false;
    }
    for (int i=1;i<6;i++){
        myrow[i].BeChecked=true;
        myrow[i].weight=0;
    }
    myrow[0].BeChecked=false;
    myrow[0].weight=344+1000;
    myrow[6].BeChecked=false;
    myrow[6].weight=344+1000;
    
}

void CallTheFlagZombies(int *i,Gamelevels thislevel){
    CallingFZombieTimer += GetFrameTime();

    if (*i==L[thislevel].TotalFlagZmb-1 && flagZombie[*i].activated) return;

    if (CallingFZombieTimer>L[thislevel].seconds_per_FlagZombie  && *i<L[thislevel].TotalFlagZmb){

        flagZombie[*i].health=150;
        flagZombie[*i].activated=true;
        CallingFZombieTimer=0;

        flagZombie[*i].row=GetRandomValue(1,5);
        flagZombie[*i].rec=(Rectangle){
            1350,
            flagZombie[*i].row*100+35,
            2,
            85
        };
        
        if (*i==L[thislevel].TotalFlagZmb-1) return;
        *i += 1;
    }
}


void UpdateFlagZombies(int i,bool *gameover,Gamelevels thislevel){


    if(flagZombie[i].health<=0){
        flagZombie[i].rec=(Rectangle){0,0,0,0};
        return;
    }

    if (flagZombie[i].health>0){
        flagZombie[i].rec.x -= flagZombie[i].speed;

        int LMindex=(flagZombie[i].rec.y/100-1);
        if (LMindex >= 0 && LMindex < 5) {
            if(CheckCollisionRecs(myLawnMower[LMindex].rec,flagZombie[i].rec)){
                myrow[LMindex+1].BeChecked=true;
                updateweight(LMindex+1);
                flagZombie[i].health=0;
                flagZombie[i].isdead=true;
                flagZombie[i].rec=(Rectangle){0,0,0,0};
                myLawnMower[LMindex].ishere=false;
            }
        }

        if (flagZombie[i].rec.x <= 300 && flagZombie[i].health>0)
            *gameover=true;

            int Row = flagZombie[i].row;
            int minrow;
            if (myrow[Row-1].weight < myrow[Row].weight && myrow[Row-1].weight < myrow[Row+1].weight ) minrow =Row-1;
            else if (myrow[Row+1].weight < myrow[Row-1].weight && myrow[Row+1].weight < myrow[Row].weight ) minrow =Row+1;
            else minrow =Row;

            flagZombie[i].cooldown -= GetFrameTime();
        if (flagZombie[i].cooldown<=0){
            flagZombie[i].row= minrow;
            flagZombie[i].rec.y=flagZombie[i].row*100+35;
            flagZombie[i].cooldown =5;
        
        }
    }
    
    //  EAT -------
    flagZombie[i].speed = L[thislevel].FlagZombie_Speed;

    for ( int s=0;s<45;s++){
        if(flagZombie[i].health>0 && mySunflower[s].planted ){
            if ( CheckCollisionRecs(flagZombie[i].rec,mySunflower[s].rec)){
                
                if(mySunflower[s].health>0){
                    flagZombie[i].speed = 0;
                    mySunflower[s].health -= damage_Zombie;
                    PlaySound(fchomping);
                    
                }else{
                    mySunflower[s].planted=false;
                }

                #if DEBUG2
                    DrawRectangleLinesEx(mySunflower[s].rec,5,RED);
                #endif
            } 
        }
    } 

    for ( int p=0;p<45;p++){
        if(flagZombie[i].health>0 && myPeaShooter[p].health>0 ){
            if ( CheckCollisionRecs(flagZombie[i].rec,myPeaShooter[p].rec)){
                
                myrow[flagZombie[i].row].BeChecked=true;
                updateweight(flagZombie[i].row);
                if(myPeaShooter[p].health>0){
                    flagZombie[i].speed = 0;
                    myPeaShooter[p].health -= damage_Zombie;
                    PlaySound(fchomping);
                    
                }else{
                    myPeaShooter[p].planted=false;
                }

                #if DEBUG2
                    DrawRectangleLinesEx(myPeaShooter[p].rec,5,RED);
                #endif
            } 
        }
    }
    
    for ( int c=0;c<45;c++){
        if(flagZombie[i].health>0 && mychomper[c].health>0 ){
            if ( CheckCollisionRecs(flagZombie[i].rec,mychomper[c].rec)){
                
                myrow[flagZombie[i].row].BeChecked=true;
                updateweight(flagZombie[i].row);
                if(mychomper[c].health>0){
                    flagZombie[i].speed = 0;
                    mychomper[c].health -= damage_Zombie;
                    PlaySound(fchomping);
                    
                }else{
                    mychomper[c].planted=false;
                    mychomper[c].health=0;
                }

                #if DEBUG2
                    DrawRectangleLinesEx(mychomper[c].rec,5,RED);
                #endif
            } 
        }
    }
    for ( int b=0;b<45;b++){
        if(flagZombie[i].health>0 && mypotato[b].health>0 ){
            if ( CheckCollisionRecs(flagZombie[i].rec,mypotato[b].rec)){
                
                myrow[flagZombie[i].row].BeChecked=true;
                updateweight(flagZombie[i].row);
                if(mypotato[b].health>0){
                    flagZombie[i].speed = 0;
                    mypotato[b].health -= damage_Zombie;
                    PlaySound(fchomping);
                    
                }else{
                    mypotato[b].planted=false;
                    mypotato[b].health=0;
                }

                #if DEBUG2
                    DrawRectangleLinesEx(mypotato[b].rec,5,RED);
                #endif
            } 
        }
    }
}


void drawFlagZombie(int i){
    flagZombie[i].animation += GetFrameTime();
    if (flagZombie[i].animation>= 0.04) {
        flagZombie[i].animation_frame += 1;
        flagZombie[i].animation=0;
    }
    if (flagZombie[i].animation_frame>=FzombieTextureCount)
        flagZombie[i].animation_frame=0;

    

    if (flagZombie[i].health>0){
        DrawTextureEx(
            flagZombie[i].textures[flagZombie[i].animation_frame],
            (Vector2){flagZombie[i].rec.x-70,(flagZombie[i].row*100)-45,},
            0,0.6,WHITE);

    }

    
    

        #if DEBUG2
        DrawRectangleLinesEx(flagZombie[i].rec,5,BLACK);
        DrawRectangleLinesEx(flagZombie[i].rec,5,BLUE);
        DrawRectangleLinesEx(flagZombie[0].rec,5,WHITE);
        DrawRectangleLinesEx(flagZombie[1].rec,5,PURPLE);
        DrawRectangleLinesEx(flagZombie[2].rec,5,PINK);
        #endif

        #if DEBUG1 
        int LMindex=(flagZombie[i].rec.y/100-1);
        DrawRectangleLinesEx(myLawnMower[LMindex].rec,5,PINK);
        DrawRectangleLinesEx(flagZombie[i].rec,5,BLUE);
        //DrawTextBox(flagZombie[i].rec.x-50,flagZombie[i].rec.y-30,TextFormat("%d",flagZombie[i].row),20,WHITE,BLACK);
        #endif
}
//------------------------------------------------------------------------------------------------------------
void updateweight(int row){
    if (myrow[row].BeChecked==false) return;

    float weight=0;
    if(!myLawnMower[row-1].isgone) weight=100;
    int n=0;
    float s=0;
    float q=0;
    float temp=0;

    for(int i=0;i<45;i++){
        if (mychomper[i].planted && mychomper[i].row==row) {
            n++;
            temp=mychomper[i].health/sbplant[CHOMPER].strength;
            s += temp;
            q += pow(temp,2);
        }
        if (myjala[i].planted && myjala[i].row==row) {
            n++;
            temp=myjala[i].health/sbplant[Jalapeno].strength;
            s += temp;
            q += pow(temp,2);
        }
        
        if (mypotato[i].planted && mypotato[i].row==row) {
            n++;
            temp=mypotato[i].health/sbplant[CHOMPER].strength;
            s += temp;
            q += pow(temp,2);
        }

        if (myPeaShooter[i].planted && myPeaShooter[i].row==row) {
            n++;
            temp=myPeaShooter[i].health/sbplant[PEA_SHOOTER].strength;
            s += temp;
            q += pow(temp,2);
        }
    } 

    
    weight += 1+ pow(n,2) + pow(s,2) + pow(q,2);

    myrow[row].weight=weight;
    myrow[row].BeChecked=false;
}
//-----------------------------------------------------------------
void UNloadZombies(void){
    int k;
    if (zombieTextures != NULL) {
        for( k= 0; k < zombieTextureCount; k++) {
            UnloadTexture(zombieTextures[k]);
        }
        free(zombieTextures);
        zombieTextures = NULL;
        zombieTextureCount = 0;
    }

    if (FzombieTextures != NULL) {
        for( k= 0; k < FzombieTextureCount; k++) {
            UnloadTexture(FzombieTextures[k]);
        }
        free(FzombieTextures);
        FzombieTextures = NULL;
        FzombieTextureCount = 0;
    }

}