//game.c file

#include "screens.h"

static bool gameover;  
static int plaingnow=-1;
static int score;
static int num[8];
static int planted[11][6];
static bool win;
Sound winning;
Sound losing;
static bool justReturnedFromLevel = false;

void DrawLevel1(Gamelevels *thislevel){

    if (justReturnedFromLevel) {
        justReturnedFromLevel = false;
        plaingnow = -1;
    }
    
    static int selectedKey=0;  
    
    static bool playitonec=true;
    
    


    GlobalVariables();
    

    if (plaingnow != *thislevel) {
        playitonec=true;
        initiation(*thislevel);
        plaingnow = *thislevel;
    }


    for (int type=1;type<=5;type++){
        if (CheckCollisionPointRec(mousePos,sbplant[type].rec) & 
            IsMouseButtonDown(MOUSE_BUTTON_LEFT) &
            score>=sbplant[type].cost) 
        {
            selectedKey=type;
            break;
        }
    }
    
    UpdateFallingStar(&score);

    if(selectedKey==SUN_FLOWER || selectedKey==PEA_SHOOTER || selectedKey==CHOMPER || selectedKey==POTATO || selectedKey==Jalapeno) {
        UpdatePlanting(&num[selectedKey], planted, &score, &selectedKey);
    }

    for (int i=0;i<num[SUN_FLOWER] && i<45 ;i++)
        UpdatePoppingStar(&score,i);


    CallTheZombies(& num[ZOMBIE],*thislevel);
    CallTheFlagZombies(&num[FLAG_ZOMBIE],*thislevel);

    for (int i=0;i<num[CHOMPER];i++)
        UpdateChomper(i,num[ZOMBIE],num[FLAG_ZOMBIE],*thislevel);

    for (int i=0;i<num[Jalapeno];i++)
        UpdateJala(i,num[ZOMBIE],num[FLAG_ZOMBIE],*thislevel);

    for (int i=0;i<num[POTATO];i++)
        UpdatePotato(i,num[ZOMBIE],num[FLAG_ZOMBIE],*thislevel);



    for (int i=0;i<=num[ZOMBIE];i++){
        UpdateZombies(i,&gameover,*thislevel);
    }


    for(int i=1;i<6;i++){
        updateweight(i);
    }
    
    for (int i=0;i<=num[FLAG_ZOMBIE];i++){
        UpdateFlagZombies(i,&gameover,*thislevel);
    }
        
    // Check win condition - all zombies are dead
        bool allNZombiesDead = true;
        bool allFZombiesDead = true;
        
        // Check normal zombies
        for(int i = 0; i < L[*thislevel].TotalNormalZmb; i++){
            if(!NormalZombie[i].isdead ){
                allNZombiesDead = false;
                break;
            }
        }
        
        if(allFZombiesDead){
            for(int i = 0; i < L[*thislevel].TotalFlagZmb; i++){
                if(!flagZombie[i].isdead ){
                    allFZombiesDead = false;
                    break;
                }
            }
        }
        
        if(allNZombiesDead && allFZombiesDead){  // Only win if we had zombies and they're all dead
            win = true;
        }
    

    //*********************************************************************************
    //********                         Draw                                  **********
    //*********************************************************************************

    



   
    

    drawgameBG();

    DrawTextBox(200,100,TextFormat("score:%03d",score),20,(Color){252,213,63,255},WHITE);
    //این دیباگ ها به من اخطار میدادند، برای ارسال بروژه کامنتشون کردم که مشکلی در باقی سیستم ایجاد نکنه.
/*
#if DEBUG1
    
    DrawTextBox(800,650,TextFormat("SunFlowers:%03d",num[SUN_FLOWER]),20,(Color){252,213,63,255},WHITE);
    DrawTextBox(800,700,TextFormat("Peashooters:%03d",num[PEA_SHOOTER]),20,(Color){252,213,63,255},WHITE);
    DrawTextBox(800,600,TextFormat("Chompers:%03d",num[CHOMPER]),20,(Color){252,213,63,255},WHITE);

    DrawTextBox(1100,700,TextFormat("normal:%03d",num[ZOMBIE]+1),20,(Color){252,213,63,255},WHITE);
    DrawTextBox(1100,650,TextFormat("flag:%03d",num[FLAG_ZOMBIE]+1),20,(Color){252,213,63,255},WHITE);

    DrawTextBox(1250,600,TextFormat("x:%03d",GetMouseX()),20,(Color){252,213,63,255},WHITE);
    DrawTextBox(1250,650,TextFormat("y:%03d",GetMouseY()),20,(Color){252,213,63,255},WHITE);

    DrawTextBox(200,650,TextFormat("%d chomperhealth ls %f",mychomper[0].health,mychomper[0].lifespan),20,(Color){252,213,63,255},WHITE);
    DrawTextBox(200,700,TextFormat("%d sunlowerhealth ls ",mySunflower[0].health),20,(Color){252,213,63,255},WHITE);
    //DrawTextBox(50,700,TextFormat("%d pot ls ",GetPotatoCount()),20,(Color){252,213,63,255},WHITE);
    Rectangle collistionrec={mypotato[0].rec.x-100,mypotato[0].rec.y-100,300,300};
    //DrawRectangleLinesEx(collistionrec,5,BROWN);
#endif DEBUG1

#if DEBUG2
    if (*thislevel==level1){
        DrawTextBox(0,0,"level one",50,BLACK,WHITE);
    }else if (*thislevel==level2){
        DrawTextBox(0,0,"level two",50,BLACK,WHITE);
    }else if (*thislevel==level3){
        DrawTextBox(0,0,"level three",50,BLACK,WHITE);
    }else if (*thislevel==level4){
        DrawTextBox(0,0,"level four",50,BLACK,WHITE);
    }
    //DrawText(TextFormat("lasr%d",NormalZombie[1].health),100,100,50,PURPLE);
    #endif DEBUG2*/



    

    for (int i=0;i<num[PEA_SHOOTER];i++){
        DrawPeaShooter(i, planted);
        shootpea(i,*thislevel);
    }

    for (int i=0;i<num[CHOMPER];i++){
        DrawChomper(i, planted);
    }

    for (int i=0;i<num[Jalapeno];i++){
        DrawJala(i, planted);
    }

    for (int i=0;i<num[POTATO];i++){
        DrawPotato(i, planted);
    }

    for (int i=0;i<num[SUN_FLOWER];i++){
        DrawSunflower(i,planted);
        DrawStar(i);
    }

    DrawStar(45);

    for (int i=0;i<=num[ZOMBIE];i++)
        drawZombie(i);

    for (int i=0;i<=num[FLAG_ZOMBIE];i++){
        drawFlagZombie(i);
    }
        
    
    for (int i=1;i<=5;i++){
        DrawSidebar(&score, i);
    }
    

    for (int i=0;i<5;i++)
        DrawLawnMower(i);

        

        if(win||gameover){
            DrawRectangle(0,0,1366,789,LIGHTGRAY);
            if (win) {
                
                DrawText("win",700,300,100,WHITE);
                if (playitonec){
                    PlaySound(winning);
                    playitonec=false;
                }}
            if(gameover) {
                DrawText("lose",700,300,100,RED);
                if (playitonec){
                    PlaySound(losing);
                    playitonec=false;
                }
            } 
            DrawText("press Space to exit",400,100,60,BLACK);
            if (IsKeyPressed(KEY_SPACE)) {
            *thislevel = playscreen;
            gameover = false;
            win = false;
            plaingnow = -1;  
            }
        }
}

void  initiation (Gamelevels thislevel){
        gameover=false;
        win=false;
        for(int i=0;i<8;i++){
            num[i]=0;
        }
        plaingnow=thislevel;


        losing=LoadSound("sprites\\resources - voix\\05. losemusic.mp3");
        winning=LoadSound("sprites\\resources - voix\\26. winmusic.mp3");
        init_sb_PeaShooter();
        init_sb_SunFlower();
        init_sb_Chomper();
        init_sb_Jalapeno();
        init_sb_Potato();
        InitStar();
        InitLawnMower();

        SetData(thislevel, &score);
        
        InitSunflower();
        InitPeaShooter();
        InitPotato();
        initChomper();
        InitJala();
        initZombie(thislevel);
        initFlagZombie(thislevel);
        initgameBG("sprites\\Textures\\latest1.jpg");

        for (int i=1;i<=10;i++){
            for (int j=1;j<=5;j++)
                planted[i][j]=0;
        }
    }




void ResetGameState(void) {
    plaingnow = -1;  
    justReturnedFromLevel = true;
    gameover = false;
    win = false;
}