// utils.c

#include "screens.h"


Vector2 mousePos;


//plants
const int injury_pea=100;
const float cooldown_planting=3;
static float Chomperlifespan=20;
static float POtatolifespan=5;
static float Jalalifespan=3;
static float shooting_pea_speed=1;


void GlobalVariables(void){
    mousePos = GetMousePosition();
    mousePos.y +=27;
}


void DrawTextBox(int x, int y, const char *text, int size, Color bgcolor, Color textcolor){
    int textwidth = MeasureText(text, size);
    int width = textwidth * 1.5;
    int height = size * 1.8;

    DrawRectangle(x, y, width, height, bgcolor);
    /* Makes the color 20% darker (multiplies by 0.8)
    color.r *= 0.8f;
    color.g *= 0.8f;
    color.b *= 0.8f;*/

    DrawRectangleLines(x, y, width, height, textcolor);
    x += textwidth / 4;
    y += size / 4;
    DrawText(text, x, y, size, textcolor);
}

Texture2D Gametex;
Texture2D edge;
Texture2D house;
Texture2D background;



void initbackground(const char *text){
    background=LoadTexture(text); 
}

void drawbg(float scale, Vector2 positionbg){
    DrawTextureEx(background, positionbg, 0, scale, WHITE);
}





void initgameBG(const char *text)
{
    edge = LoadTexture("sprites\\Textures\\downEdge.jpg");
    house = LoadTexture("sprites\\Textures\\house.png");

    Gametex = LoadTexture(text);
}


void drawgameBG(void)
{
    DrawTextureEx(house, (Vector2){-10,-50}, 0, 0.5, WHITE);
    DrawTextureEx(house, (Vector2){0,20}, 0, 0.5, WHITE);
    DrawTextureEx(house, (Vector2){0,95}, 0, 0.5, WHITE);
    

    DrawTextureEx(edge, (Vector2){0,700}, 0, 1, WHITE);
    DrawTextureEx(edge, (Vector2){600,700}, 0, 1, WHITE);
    DrawTextureEx(edge, (Vector2){770,700}, 0, 1, WHITE);

    Rectangle srcFull= (Rectangle){ 0, 0, 2800, 1536 };
    Rectangle srcPlay= (Rectangle){ 587, 268, 1626, 1000 };
    Rectangle dstPlay= (Rectangle){ 200, 127, 1000, 500 };
    float scaleX= dstPlay.width  / srcPlay.width;
    float scaleY= dstPlay.height / srcPlay.height;
    Rectangle dstFull = (Rectangle){
    .x = dstPlay.x - srcPlay.x * scaleX,
    .y = dstPlay.y - srcPlay.y * scaleY,
    .width  = srcFull.width  * scaleX,
    .height = srcFull.height * scaleY
    };

    DrawTexturePro(Gametex,
               srcFull,
               dstFull,
               (Vector2){-250, 60},  // origin
               0,                // rotation
               WHITE);
}


//LAWN MOWER

lawnmower myLawnMower[5]={0};
Sound lawnmowerSound;
void InitLawnMower(void){
    lawnmowerSound=LoadSound("sprites\\resources - voix\\SFX lawnmower.ogg");
    for (int i = 0; i < 5; i++)
    {
        myLawnMower[i].texture = LoadTexture("sprites\\Textures\\LawnMower.png");
        myLawnMower[i].ishere=true;
        myLawnMower[i].onlyonce=true;
        myLawnMower[i].isgone=false;
        myLawnMower[i].sound=false;
        myLawnMower[i].rec=
            (Rectangle){325-(i*5), 127+ 100*i, myLawnMower[i].texture.width, myLawnMower[i].texture.height};
    }
}
void DrawLawnMower(int i){
    if(myLawnMower[i].isgone) {
        return;
    } 
    if(myLawnMower[i].ishere){
        DrawTexture(
            myLawnMower[i].texture,
            myLawnMower[i].rec.x,
            myLawnMower[i].rec.y,
            WHITE);
    }else if (myLawnMower[i].onlyonce){

        myLawnMower[i].rec.x +=6;

        if (!IsSoundPlaying(lawnmowerSound) && !myLawnMower[i].sound) {
            myLawnMower[i].sound=true;
            PlaySound(lawnmowerSound);
        }
        DrawTexture(
            myLawnMower[i].texture,
            myLawnMower[i].rec.x ,
            myLawnMower[i].rec.y,
            WHITE);
        if (myLawnMower[i].rec.x==GetScreenWidth())
            myLawnMower[i].onlyonce=false;
    }else{
        myLawnMower[i].rec=(Rectangle){0,0,0,0};
        myLawnMower[i].isgone=true;
        myrow[i+1].BeChecked=true;
        updateweight(i+1);
        //StopSound(lawnmowerSound);
    }
    #if DEBUG1
        DrawRectangleLinesEx(myLawnMower[i].rec,5,PINK);
    #endif
}





// --- Star ---
Star myStar[46] = {0};
static Texture2D starTexture;
void InitStar(void){
    starTexture=LoadTexture("sprites\\Textures\\star.png");
    for(int i=0;i<46;i++){
        myStar[i].texture = starTexture;
        myStar[i].isActive =true;
        myStar[i].timer=0.0f;
        myStar[i].position=(Vector2){0,0};
        myStar[i].rotation=0.0f;
        myStar[i].isLanded = false;
        myStar[i].isCollected = false;

    }
    myStar[45].position = (Vector2){GetRandomValue(450, 1200), -50}; 
    myStar[45].landingY = GetRandomValue(300, 500);
    
    
}

void UpdateFallingStar(int *score){
    if (!myStar[45].isActive) return;
    myStar[45].rotation += 2.0f;

    Vector2 origin = (Vector2){myStar[45].position.x, myStar[45].position.y};
    bool collision = CheckCollisionPointCircle (mousePos, origin, 30);

    if (( (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && collision) /*|| (IsKeyPressed(KEY_S))*/ ) && !myStar[45].isCollected){
        *score += 25;
        myStar[45].isCollected = true;
    }

    if (!myStar[45].isLanded && !myStar[45].isCollected){
        myStar[45].position.y += 2;

        if (myStar[45].position.y > myStar[45].landingY)
            myStar[45].isLanded = true;
    }
    // if the star has landed or has not been collected:
    else{
        myStar[45].timer += GetFrameTime();

        if (myStar[45].timer >= 10.0f){
            myStar[45].position.y = -50;
            myStar[45].landingY = GetRandomValue(300, 500);
            myStar[45].position.x = GetRandomValue(450, 1200);
            myStar[45].isLanded = false;
            myStar[45].isCollected = false;
            myStar[45].timer = 0;
        }
    }
}


void UpdatePoppingStar(int *score, int i){

    if(!myStar[i].isActive) return;
    myStar[i].rotation += 2.0f;
    myStar[i].timer += GetFrameTime();

    Vector2 origin = (Vector2){myStar[i].position.x, myStar[i].position.y};
    bool collision = CheckCollisionPointCircle (mousePos, origin, 30);

    if (( IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && collision ) && !myStar[i].isCollected)
    {
        *score += 25;
        myStar[i].isCollected = true;
        myStar[i].timer = 0;
    }

    if (!myStar[i].isLanded && !myStar[i].isCollected){
        myStar[i].position.x += 0.5;
        myStar[i].position.y += 2*(myStar[i].timer) - 1.5;

        if (myStar[i].position.y > myStar[i].landingY)
            myStar[i].isLanded = true;
            
    }
    // if the star has landed or has not been collected:
    else if (myStar[i].timer >= 5.0){
        
            myStar[i].position.x = myStar[i].startX;
            myStar[i].position.y = myStar[i].startY;
            myStar[i].isLanded = false;
            myStar[i].isCollected = false;
            myStar[i].timer = 0;
    }
}

void DrawStar(int i){
    if(!myStar[i].isActive) return;
    if (!myStar[i].isCollected){

        Vector2 origin = (Vector2){myStar[i].texture.width / 2, myStar[i].texture.height / 2};
        // This is the Destination Rectangle. It defines the box on the screen where the image will appear.
        Rectangle destRec = {myStar[i].position.x, myStar[i].position.y, myStar[i].texture.width, myStar[i].texture.height};

        DrawTexturePro(
            myStar[i].texture,
            // Source Rect: This tells Raylib which part of the original image to grab.
            //{0, 0, width, height} means "Grab the entire image from top-left to bottom-right."
            (Rectangle){0, 0, myStar[0].texture.width, myStar[i].texture.height},
            destRec, // Destination (Position)
            origin,  // Rotation Point (Center)
            myStar[i].rotation,
            WHITE);

        #if DEBUG1
        DrawCircleV((Vector2){myStar[i].position.x, myStar[i].position.y},5,RED);
        DrawCircleLinesV((Vector2){myStar[i].position.x, myStar[i].position.y},30,RED);
        DrawCircleV(mousePos,5,BLUE);
        #endif
    }
}


//----Pea-----
typedef struct {
    Texture2D texture;
    Rectangle rec;
    float timer;
    bool shoot;
}pea;
static pea mypea[45];
static Texture2D peaTexture;






// sunflower

static Texture2D *SunFlowertextures=NULL;
static int sunflowerTextureCount=13;
Plants mySunflower[45] = {0};
void InitSunflower(void){
    if(SunFlowertextures==NULL){
        SunFlowertextures=(Texture2D*)malloc((sizeof(Texture2D))*sunflowerTextureCount);
        for(int k=0;k<sunflowerTextureCount;k++){
            SunFlowertextures[k]=LoadTexture(TextFormat("sprites\\animation\\sunflower\\frame_%02d_delay-0.03s.png",k));
        }
    }

    for (int i = 0; i < 45; i++)
    {
        mySunflower[i].textures = SunFlowertextures;
        mySunflower[i].rec.x = 0;
        mySunflower[i].rec.y = 0;
        mySunflower[i].health=sbplant[SUN_FLOWER].strength;
        mySunflower[i].planted=false;
        mySunflower[i].row=0;
        mySunflower[i].animation=0;
        mySunflower[i].animation_frame=0;
        mySunflower[i].lifespan=0;
    }
}
void Animation_Plants(Plants myplant[],float CS,int i,int TexCount){
    myplant[i].animation += GetFrameTime();
        if (myplant[i].animation> (0.01)*CS){
            myplant[i].animation_frame++;
            myplant[i].animation=0;
        }
    if (myplant[i].animation_frame>=TexCount)
        myplant[i].animation_frame=0;
}

void DrawSunflower(int i, int (*planted)[6]){


    int gridX=mySunflower[i].rec.x/100-2;
    int gridY=(mySunflower[i].rec.y-27)/100;

    

    if(mySunflower[i].planted){

        Animation_Plants(mySunflower,6,i,sunflowerTextureCount);

    DrawTexture(
        mySunflower[i].textures[mySunflower[i].animation_frame],
        mySunflower[i].rec.x,
        mySunflower[i].rec.y,
        WHITE);

    #if DEBUG1
    DrawRectangleLinesEx(mySunflower[i].rec,5,GOLD);
    DrawTextBox(mySunflower[i].rec.x,mySunflower[i].rec.y,TextFormat("%d",mySunflower[i].row),20,WHITE,BLACK);
    #endif

    }else{
        mySunflower[i].rec=(Rectangle){0,0,0,0};
        myStar[i].isActive=false;
        planted[gridX][gridY] = 0;
    }
}

// peashooter
Sound shootsound;
Plants myPeaShooter[45] = {0};
static Texture2D *peashooterTexture=NULL;
static int peshoTextureCount=1;
void InitPeaShooter(void){
    
    if(peashooterTexture==NULL){
        peashooterTexture=(Texture2D*)malloc((sizeof(Texture2D))*peshoTextureCount);
        for(int k=0;k<peshoTextureCount;k++){
            peashooterTexture[k]=LoadTexture(TextFormat("sprites\\animation\\peashutter\\frame_0_delay-0.1s.png"));
        }
    }

    peaTexture=LoadTexture("sprites\\Textures\\pea1.png");
    for (int i = 0; i < 45; i++){

        mypea[i].texture=peaTexture;
        mypea[i].rec=(Rectangle){0,0,mypea[i].texture.width-20,mypea[i].texture.height-20};
        mypea[i].timer=0;
        shootsound=LoadSound("sprites\\resources - voix\\SFX shoop.ogg");

        myPeaShooter[i].planted=false;
        myPeaShooter[i].textures = peashooterTexture;
        myPeaShooter[i].rec.x =0;
        myPeaShooter[i].rec.y = 0;
        myPeaShooter[i].health=sbplant[PEA_SHOOTER].strength;
        myPeaShooter[i].row=0;
        myPeaShooter[i].animation=0;
        myPeaShooter[i].animation_frame=0;
        myPeaShooter[i].lifespan=0;
    }
}
void DrawPeaShooter(int i, int (*planted)[6])
{
    int gridX=(myPeaShooter[i].rec.x/100)-2;;
    int gridY=(myPeaShooter[i].rec.y-27)/100;;

    

    if(myPeaShooter[i].planted && myPeaShooter[i].health>0){

    DrawTexture(
        myPeaShooter[i].textures[0],
        myPeaShooter[i].rec.x,
        myPeaShooter[i].rec.y,
        WHITE);

    #if DEBUG1
    DrawRectangleLinesEx(myPeaShooter[i].rec,5,DARKGREEN);
    DrawTextBox(myPeaShooter[i].rec.x,myPeaShooter[i].rec.y,TextFormat("row %d",myPeaShooter[i].row),20,(Color){252,213,63,255},WHITE);
    #endif

    }else{
        myrow[gridY].BeChecked=true;
        updateweight(gridY);
        myPeaShooter[i].rec=(Rectangle){0,0,0,0};
        planted[gridX][gridY] = 0;
    }

}

//----CHOMPER----

static Texture2D *chomperTexture=NULL;
static int chomTextureCount=13;
Plants mychomper[45]={0};
void initChomper(void){
    if(chomperTexture==NULL){
        chomperTexture=(Texture2D*)malloc(sizeof(Texture2D)*chomTextureCount);
        for (int k=0;k<chomTextureCount;k++){
            chomperTexture[k]=LoadTexture((TextFormat("sprites\\animation\\Chomper\\frame_%02d_delay-0.18s.png",k)));
        }
    }

    for(int i=0;i<45;i++){
        mychomper[i].textures=chomperTexture;
        mychomper[i].rec.x=0;
        mychomper[i].rec.y=0;
        mychomper[i].health=sbplant[CHOMPER].strength;
        mychomper[i].animation=0;
        mychomper[i].animation_frame=0;
        mychomper[i].lifespan=Chomperlifespan;
        mychomper[i].planted=false;
        mychomper[i].row=0;

    }
}

void UpdateChomper(int i,int numzombie,int numflagz,Gamelevels thislevel){
    if (mychomper[i].planted){
        mychomper[i].lifespan -= GetFrameTime();

        for(int z=0;z<numzombie;z++){
            if((NormalZombie[z].rec.y-35)==(mychomper[i].rec.y-27) && NormalZombie[z].speed!=0 ){
                NormalZombie[z].speed = L[thislevel].zombie_speed/3;
            }    
        }
        for(int f=0;f<numflagz;f++){
            if((flagZombie[f].rec.y-35)==(mychomper[i].rec.y-27) && flagZombie[f].speed!=0 ){
                flagZombie[f].speed = L[thislevel].FlagZombie_Speed/3;
            }    
        }
    
        if(mychomper[i].lifespan<=0 || mychomper[i].health<=0){
            mychomper[i].health=0;
            mychomper[i].planted=false;
            myrow[mychomper[i].row].BeChecked=true;
            updateweight(mychomper[i].row);
        }
    }
}

void DrawChomper(int i, int (*planted)[6]){

    int gridX=mychomper[i].rec.x/100-2;
    int gridY=(mychomper[i].rec.y-27)/100;

    

    if(mychomper[i].planted){

        Animation_Plants(mychomper,18,i,chomTextureCount);

    DrawTexture(
        mychomper[i].textures[mychomper[i].animation_frame],
        mychomper[i].rec.x,
        mychomper[i].rec.y,
        WHITE);

    #if DEBUG1
    DrawRectangleLinesEx(mychomper[i].rec,5,PURPLE);
    DrawTextBox(mychomper[i].rec.x,mychomper[i].rec.y,TextFormat("%d",mychomper[i].row),20,(Color){252,213,63,255},WHITE);
    #endif

    }else{
        myrow[gridY].BeChecked=true;
        updateweight(gridY);
        planted[gridX][gridY]=0;
        mychomper[i].rec=(Rectangle){0,0,0,0};
    }
}
//---------Jalapeno------------
static Texture2D *JalaTex=NULL;
static int JalaTexCount=25;
Plants myjala[45]={0};
Sound boom;
void InitJala(void){
    if(JalaTex==NULL){
        JalaTex=(Texture2D*)malloc(sizeof(Texture2D)*JalaTexCount);
        for (int k=0;k<JalaTexCount;k++){
            JalaTex[k]=LoadTexture((TextFormat("sprites\\animation\\Jalapino\\idle00%02d.png",k)));
        }
    }

    boom=LoadSound("sprites\\resources - voix\\SFX potato mine.ogg");
    for(int i=0;i<45;i++){
        myjala[i].textures=JalaTex;
        myjala[i].rec.x=0;
        myjala[i].rec.y=0;
        myjala[i].health=sbplant[Jalapeno].strength;
        myjala[i].animation=0;
        myjala[i].animation_frame=0;
        myjala[i].lifespan=Jalalifespan;
        myjala[i].planted=false;
        myjala[i].row=0;

    }
}

void UpdateJala(int i,int numzombie,int numflagz,Gamelevels thislevel){

    if (myjala[i].planted){
        myjala[i].lifespan -= GetFrameTime();
        myjala[i].explosion=false;

    
        if(myjala[i].lifespan<=0 ){

            for(int z=0;z<numzombie;z++){
                if(NormalZombie[z].row==myjala[i].row){
                    NormalZombie[z].rec=(Rectangle){0,0,0,0};
                    NormalZombie[z].health=0;
                    NormalZombie[z].isdead=true;
                    myjala[i].explosion=true;
                }    
            }
            for(int f=0;f<numflagz;f++){
                if(flagZombie[f].row==myjala[i].row) {
                    flagZombie[f].rec=(Rectangle){0,0,0,0};
                    flagZombie[f].health=0;
                    flagZombie[f].isdead=true;
                    myjala[i].explosion=true;
                }    
            }
        }

        if(myjala[i].explosion){
            PlaySound(boom);
            myjala[i].explosion=false;
            myjala[i].lifespan=Jalalifespan;

            myjala[i].health=0;
            myjala[i].planted=false;
            myrow[myjala[i].row].BeChecked=true;
            updateweight(myjala[i].row);
        }
    }
}

void DrawJala(int i, int (*planted)[6]){

    int gridX=myjala[i].rec.x/100-2;
    int gridY=(myjala[i].rec.y-27)/100;

    

    if(myjala[i].planted){

        Animation_Plants(myjala,18,i,JalaTexCount);

    DrawTexture(
        myjala[i].textures[myjala[i].animation_frame],
        myjala[i].rec.x,
        myjala[i].rec.y,
        WHITE);

    #if DEBUG1
    DrawRectangleLinesEx(myjala[i].rec,5,PURPLE);
    DrawTextBox(myjala[i].rec.x,myjala[i].rec.y,TextFormat("%d",myjala[i].row),20,(Color){252,213,63,255},WHITE);
    #endif

    }else{
        myrow[gridY].BeChecked=true;
        updateweight(gridY);
        planted[gridX][gridY]=0;
        myjala[i].rec=(Rectangle){0,0,0,0};
    }
}

//---------Potato-----------

static Texture2D *potex=NULL;
static int potexcount=30;
Plants mypotato[45]={0};
void InitPotato(void){
    

    if(potex==NULL){
        potex=(Texture2D*)malloc(sizeof(Texture2D)*potexcount);
        for (int k=0;k<potexcount;k++){
            potex[k]=LoadTexture((TextFormat("sprites\\animation\\PotatoMine\\Idle\\idle00%02d.png",k)));
        }
    }

    for(int i=0;i<45;i++){
        mypotato[i].textures=potex;
        mypotato[i].rec.x=0;
        mypotato[i].rec.y=0;
        mypotato[i].health=sbplant[POTATO].strength;
        mypotato[i].animation=0;
        mypotato[i].animation_frame=0;
        mypotato[i].lifespan=POtatolifespan;
        mypotato[i].planted=false;
        mypotato[i].row=0;    
        mypotato[i].explosion=false;
    }
}

Rectangle collistionrec[45];
void UpdatePotato(int i,int numzombie,int numflagz,Gamelevels thislevel){


    collistionrec[i]=(Rectangle){mypotato[i].rec.x-100,mypotato[i].rec.y-100,300,300};
    if (mypotato[i].planted){
        mypotato[i].lifespan -= GetFrameTime();
        mypotato[i].explosion=false;
        
    
        if(mypotato[i].lifespan<=0 ){

            for(int z=0;z<numzombie;z++){
                if(CheckCollisionRecs(NormalZombie[z].rec,collistionrec[i])){
                    NormalZombie[z].rec=(Rectangle){0,0,0,0};
                    NormalZombie[z].health=0;
                    NormalZombie[z].isdead=true;

                    mypotato[i].explosion=true;
                }
            }

        for(int f=0;f<numflagz;f++){
            if(CheckCollisionRecs(flagZombie[f].rec,collistionrec[i])){
                flagZombie[f].rec=(Rectangle){0,0,0,0};
                flagZombie[f].health=0;
                flagZombie[f].isdead=true;
                mypotato[i].explosion=true;
            }    
        }
        

        if(mypotato[i].explosion){
            PlaySound(boom);
            mypotato[i].explosion=false;
            mypotato[i].planted=false;
            mypotato[i].health=0;
            mypotato[i].lifespan=POtatolifespan;
            myrow[mypotato[i].row].BeChecked=true;
            updateweight(mypotato[i].row);
        }
            
        }
    }
}

void DrawPotato(int i, int (*planted)[6]){
    if(mypotato[i].health<=0){
        mypotato[i].rec=(Rectangle){0,0,0,0};
        return;
    }
    int gridX=mychomper[i].rec.x/100-2;
    int gridY=(mychomper[i].rec.y-27)/100;

    

    if(mypotato[i].planted){

        Animation_Plants(mypotato,18,i,potexcount);

    DrawTexture(
        mypotato[i].textures[mypotato[i].animation_frame],
        mypotato[i].rec.x,
        mypotato[i].rec.y,
        WHITE);

    #if DEBUG1
    DrawRectangleLinesEx(mypotato[i].rec,5,PURPLE);
    DrawTextBox(mypotato[i].rec.x,mypotato[i].rec.y,TextFormat("%d",mypotato[i].row),20,(Color){252,213,63,255},WHITE);
    #endif

    }else{
        myrow[gridY].BeChecked=true;
        updateweight(gridY);
        planted[gridX][gridY]=0;
        mypotato[i].rec=(Rectangle){0,0,0,0};
    }
}

// SIDEBAR & PLANTING


ToPlant sbplant[6] = {0};
void init_sb_SunFlower(void){

    sbplant[SUN_FLOWER].texture = LoadTexture("sprites\\Textures\\sunflower.jpg");
    sbplant[SUN_FLOWER].sound = LoadSound("sprites\\resources - voix\\SFX plant.ogg");
    sbplant[SUN_FLOWER].played=false;
    sbplant[SUN_FLOWER].rec=(Rectangle){50,30,115,120};
    sbplant[SUN_FLOWER].timer = 0;
    sbplant[SUN_FLOWER].key = false;
    sbplant[SUN_FLOWER].lastPlantTime = 0;
    sbplant[SUN_FLOWER].cost = 50;
    sbplant[SUN_FLOWER].strength = 100;
    sbplant[SUN_FLOWER].isAvailable=true;
}
void init_sb_PeaShooter(void){

    sbplant[PEA_SHOOTER].texture = LoadTexture("sprites\\Textures\\lubi.png");
    sbplant[PEA_SHOOTER].sound = LoadSound("sprites\\resources - voix\\SFX plant2.ogg");
    sbplant[PEA_SHOOTER].played=false;
    sbplant[PEA_SHOOTER].rec=(Rectangle){50,160,115,120};
    sbplant[PEA_SHOOTER].timer = 0;
    sbplant[PEA_SHOOTER].key = false;
    sbplant[PEA_SHOOTER].lastPlantTime = 0;
    sbplant[PEA_SHOOTER].cost = 100;
    sbplant[PEA_SHOOTER].strength = 600;
    sbplant[PEA_SHOOTER].isAvailable=true;
}
void init_sb_Chomper(void){
    
    sbplant[CHOMPER].texture = LoadTexture("sprites\\Textures\\Chomper.png");
    sbplant[CHOMPER].sound = LoadSound("sprites\\resources - voix\\SFX throw.ogg");
    sbplant[CHOMPER].played=false;
    sbplant[CHOMPER].rec=(Rectangle){50,290,115,120};
    sbplant[CHOMPER].timer = 0;
    sbplant[CHOMPER].key = false;
    sbplant[CHOMPER].lastPlantTime = 0;
    sbplant[CHOMPER].cost = 150;
    sbplant[CHOMPER].strength = 400;
    sbplant[CHOMPER].isAvailable=true;
}
void init_sb_Jalapeno(void){
    
    sbplant[Jalapeno].texture = LoadTexture("sprites\\Textures\\jalapino1.png");
    sbplant[Jalapeno].sound = LoadSound("sprites\\resources - voix\\SFX throw.ogg");
    sbplant[Jalapeno].played=false;
    sbplant[Jalapeno].rec=(Rectangle){50,420,115,120};
    sbplant[Jalapeno].timer = 0;
    sbplant[Jalapeno].key = false;
    sbplant[Jalapeno].lastPlantTime = 0;
    sbplant[Jalapeno].cost = 20;
    sbplant[Jalapeno].strength = 300;
    sbplant[Jalapeno].isAvailable=true;
}
void init_sb_Potato(void){
    
    sbplant[POTATO].texture = LoadTexture("sprites\\Textures\\potato.png");
    sbplant[POTATO].sound = LoadSound("sprites\\resources - voix\\SFX throw.ogg");
    sbplant[POTATO].played=false;
    sbplant[POTATO].rec=(Rectangle){50,550,115,120};
    sbplant[POTATO].timer = 0;
    sbplant[POTATO].key = false;
    sbplant[POTATO].lastPlantTime = 0;
    sbplant[POTATO].cost = 200;
    sbplant[POTATO].strength = 200;
    sbplant[POTATO].isAvailable=true;
}


// Planting
int FindFreePlantSlot(Plants plantArray[], plantType type) {
    for (int i = 0; i < 45; i++) {
        if (plantArray[i].health <= 0) {  

            plantArray[i] = (Plants){0}; 
            sbplant[type].played=false;
            plantArray[i].planted=false;
            plantArray[i].animation=0;
            plantArray[i].animation_frame=0;

            switch (type)
            {
            case SUN_FLOWER:{
                plantArray[i].textures =SunFlowertextures;
                plantArray[i].health =sbplant[SUN_FLOWER].strength;
                break;
            }
            case PEA_SHOOTER:{
                plantArray[i].textures =peashooterTexture;
                plantArray[i].health =sbplant[PEA_SHOOTER].strength;
                break;
            }
            case CHOMPER:{
                plantArray[i].textures =chomperTexture;
                plantArray[i].health =sbplant[CHOMPER].strength;
                plantArray[i].lifespan=Chomperlifespan;
                break;
            }
            case POTATO:{
                plantArray[i].textures =potex;
                plantArray[i].health =sbplant[POTATO].strength;
                plantArray[i].lifespan=POtatolifespan;
                break;
            }
            case Jalapeno:{
                plantArray[i].textures =JalaTex;
                plantArray[i].health =sbplant[Jalapeno].strength;
                plantArray[i].lifespan=Jalalifespan;
                break;
            }
            
            default:
                break;
            }
            
            return i;  
        }
    }
    return -1;  
}


void UpdatePlanting(int *num, int (*planted)[6], int *score, int *type){

    if(!sbplant[*type].isAvailable) return;
    int gridX = (GetMouseX() / 100) - 2;
    int gridY = (GetMouseY() / 100);
    /*if (gridX < 1 || gridX > 10 || gridY < 1 || gridY > 5) {
        return;
    }*/

    float currentTime = GetTime();
    sbplant[*type].timer = currentTime - sbplant[*type].lastPlantTime;

    if ((sbplant[*type].timer >= cooldown_planting) && *score >= sbplant[*type].cost ){
        sbplant[*type].key = true;
    }
    for(int i=1;i<=5;i++){
        if (i!= *type){
            sbplant[i].key=false;
        }
    }


    if (sbplant[*type].key ){

        if (gridX >= 2 && gridX <= 10 && gridY >= 1 && gridY <= 5 && !planted[gridX][gridY] && (*num <= 45)){

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
                int index;

                switch (*type)
                {
                case SUN_FLOWER :{
                    index=FindFreePlantSlot(mySunflower,*type);
                    break;
                }
                case PEA_SHOOTER:{
                    index=FindFreePlantSlot(myPeaShooter,*type);
                    break;
                }
                case CHOMPER:{
                    index=FindFreePlantSlot(mychomper,*type);
                    break;
                }
                case POTATO:{
                    index=FindFreePlantSlot(mypotato,*type);
                    break;
                }
                case Jalapeno:{
                    index=FindFreePlantSlot(myjala,*type);
                    break;
                }
                default:
                    break;
                }
                
                if(index==-1){
                    index=*num;
                    *num += 1;
                }

                switch (*type){
                case SUN_FLOWER:{

                    if(!sbplant[SUN_FLOWER].played){
                        PlaySound(sbplant[SUN_FLOWER].sound);
                    }
                    mySunflower[index].planted=true;
                    mySunflower[index].rec=(Rectangle){
                        (GetMouseX() / 100) * 100,
                        (GetMouseY() / 100) * 100 + 27,
                        mySunflower[index].textures[0].width-30,
                        mySunflower[index].textures[0].height-10
                    };
                    mySunflower[index].row=gridY;

                    myStar[index].landingY = (GetMouseY() / 100) * 100 + 107;
                    myStar[index].position.y = (GetMouseY() / 100) * 100 + 77;
                    myStar[index].position.x = (GetMouseX() / 100) * 100 +50;
                    myStar[index].startX =(GetMouseX() / 100) * 100 +50;
                    myStar[index].startY =(GetMouseY() / 100) * 100 + 77;
                    myStar[index].isActive=true;
                    
                    break;
                }


                case PEA_SHOOTER:{
                    if(!sbplant[PEA_SHOOTER].played){
                        PlaySound(sbplant[PEA_SHOOTER].sound);
                    }
                    myPeaShooter[index].planted=true;
                    myPeaShooter[index].rec=(Rectangle){
                        (GetMouseX() / 100) * 100,
                        (GetMouseY() / 100) * 100 + 27,
                        myPeaShooter[index].textures[0].width,
                        myPeaShooter[index].textures[0].height
                    };
                    myPeaShooter[index].row=gridY;
                    myrow[gridY].BeChecked=true;
                    updateweight(gridY);


                    mypea[index].rec.x=myPeaShooter[index].rec.x +75;
                    mypea[index].rec.y=myPeaShooter[index].rec.y +30;
                    break;
                }

                case CHOMPER:{
                    if(!sbplant[CHOMPER].played){
                        PlaySound(sbplant[CHOMPER].sound);
                    }
                    mychomper[index].planted=true;
                    mychomper[index].rec=(Rectangle){
                        (GetMouseX() / 100) * 100,
                        (GetMouseY() / 100) * 100 + 27,
                        mychomper[index].textures[0].width,
                        mychomper[index].textures[0].height
                    };
                    mychomper[index].row=gridY;
                    myrow[gridY].BeChecked=true;
                    updateweight(gridY);
                    break;
                }
                case POTATO:{

                    GameData data = GetGameData();
                    if (data.potatoes > 0) {
                        if(!sbplant[POTATO].played){
                            PlaySound(sbplant[POTATO].sound);
                        }

                        UsePotato();
                        mypotato[index].planted=true;
                        mypotato[index].rec=(Rectangle){
                        (GetMouseX() / 100) * 100,
                        (GetMouseY() / 100) * 100 + 27,
                        mypotato[index].textures[0].width,
                        mypotato[index].textures[0].height
                    };
                    mypotato[index].row=gridY;
                    myrow[gridY].BeChecked=true;
                    updateweight(gridY);
                    }
                    break;
                }

                case Jalapeno:{
                    if(!sbplant[Jalapeno].played){
                        PlaySound(sbplant[Jalapeno].sound);
                    }
                    myjala[index].planted=true;
                    myjala[index].rec=(Rectangle){
                        (GetMouseX() / 100) * 100,
                        (GetMouseY() / 100) * 100 + 27,
                        myjala[index].textures[0].width,
                        myjala[index].textures[0].height
                    };
                    myjala[index].row=gridY;
                    myrow[gridY].BeChecked=true;
                    updateweight(gridY);
                    break;
                }

                default:
                    break;
                }

                sbplant[*type].key=false;
                *score -= sbplant[*type].cost;
                sbplant[*type].lastPlantTime = currentTime;
                planted[gridX][gridY] = *type;
                *type=0;
            }
        }
    }
}

//sidebar
void DrawSidebar( int *score,int i){

    if(!sbplant[i].isAvailable) return;

    float currentTime = GetTime();

        DrawRectangleRounded(sbplant[i].rec,0.3,0,(Color){255,255,255,210});

        //show potato number
        DrawText(TextFormat("%d",sbplant[i].cost),sbplant[i].rec.x+80, sbplant[i].rec.y+100,20,BLACK);
        if(i==POTATO){
            GameData data = GetGameData();
            if(GetPotatoCount>0)
                DrawText(TextFormat("%d",data.potatoes),sbplant[i].rec.x+10, sbplant[i].rec.y+100,20,GOLD);
            else
                DrawText(TextFormat("%d",data.potatoes),sbplant[i].rec.x+10, sbplant[i].rec.y+100,20,RED);
        }
        //show potato number

        //Selection mark
        if (sbplant[i].key )
        DrawRectangleRoundedLinesEx(sbplant[i].rec,0.5, 0, 5,YELLOW);

        if (currentTime - sbplant[i].lastPlantTime > cooldown_planting) {
            DrawTexture(sbplant[i].texture, sbplant[i].rec.x, sbplant[i].rec.y, WHITE);
            
        }else{
            float cooldownProgress = (currentTime - sbplant[i].lastPlantTime) / cooldown_planting;
            Rectangle faderec = { 0,0,(int)( sbplant[i].texture.width * cooldownProgress),sbplant[i].texture.height};

            DrawTextureRec(sbplant[i].texture,faderec,(Vector2){sbplant[i].rec.x,sbplant[i].rec.y},WHITE);
        }


        if ( *score < sbplant[i].cost )
        DrawRectangleRounded(sbplant[i].rec,0.3,0,(Color){0, 0, 0, 128});

}





//----Life in the Garden-----

void shootpea(int i,Gamelevels thislevel){
    
    mypea[i].timer += GetFrameTime();

    enum{NOTHING,NORMAL,FLAG};
    int type=NOTHING;

    if(myPeaShooter[i].planted && mypea[i].timer>shooting_pea_speed ){

        mypea[i].rec.x +=2;

        float normdis=10000;
        int NormzIndex=-1;

        for(int n=0;n<L[thislevel].TotalNormalZmb;n++){
            if(NormalZombie[n].health>0){
                normdis=NormalZombie[n].rec.x-mypea[i].rec.x;
                if( normdis>0 && NormalZombie[n].rec.y -35 ==myPeaShooter[i].rec.y-27 ){
                    NormzIndex=n;
                    break;
                }
            }
        }


        float flagdis=10000;
        int FlgzIndex=-1;

        for(int f=0;f<L[thislevel].TotalFlagZmb;f++){
            if(flagZombie[f].health>0){
                flagdis=flagZombie[f].rec.x-mypea[i].rec.x;
                if( flagdis>0 && flagZombie[f].rec.y -35 == myPeaShooter[i].rec.y-27 ){
                    FlgzIndex=f;
                    break;
                }
            }
        }

        bool collision=false;
        if(flagdis!=10000 ||normdis!=10000){
        if(flagdis<normdis){
            collision=CheckCollisionRecs(mypea[i].rec,flagZombie[FlgzIndex].rec);
            type=FLAG;
        }else if(normdis<flagdis){
            collision=CheckCollisionRecs(mypea[i].rec,NormalZombie[NormzIndex].rec);
            type=NORMAL;
        }else if (normdis !=10000){
            collision=CheckCollisionRecs(mypea[i].rec,flagZombie[FlgzIndex].rec);
            type=FLAG;
        }else{
            collision=false;
            type=NOTHING;
        }}


        if (collision && type==NORMAL){
            NormalZombie[NormzIndex].health -= injury_pea;
            if(NormalZombie[NormzIndex].health<=0){
                NormalZombie[NormzIndex].isdead=true;
            }
            mypea[i].timer=0;
        }else if(collision && type==FLAG){
            flagZombie[FlgzIndex].health -= injury_pea;
            if(flagZombie[FlgzIndex].health<=0){
                flagZombie[FlgzIndex].isdead=true;
            }
            mypea[i].timer=0;
            
        }


            if (mypea[i].rec.x<= 1300 && !collision && type!=NOTHING && (FlgzIndex !=-1 || NormzIndex!=-1)){
            DrawTextureEx(mypea[i].texture,(Vector2){mypea[i].rec.x,mypea[i].rec.y},0,0.5,WHITE);
            
            
            #if DEBUG1
            DrawRectangleLinesEx(mypea[i].rec,2,DARKPURPLE);
            #endif

            }else{
                mypea[i].rec.x=myPeaShooter[i].rec.x +75 ;       
            }
        
    }
}

void UnloadEverything(void){
    int k;

    if(SunFlowertextures !=NULL){
        for(k=0;k<sunflowerTextureCount;k++){
            UnloadTexture(SunFlowertextures[k]);
        }
        free(SunFlowertextures);
        SunFlowertextures=NULL;
        sunflowerTextureCount=0;
    }
    UnloadTexture(starTexture);
    

    if(peashooterTexture !=NULL){
        for(k=0;k<peshoTextureCount;k++){
            UnloadTexture(peashooterTexture[k]);
        }
        free(peashooterTexture);
        peashooterTexture=NULL;
        peshoTextureCount=0;
    }

    if(chomperTexture !=NULL){
        for(k=0;k<chomTextureCount;k++){
            UnloadTexture(chomperTexture[k]);
        }
        free(chomperTexture);
        chomperTexture=NULL;
        chomTextureCount=0;
    }
    UnloadSound(lawnmowerSound);
    CloseAudioDevice();
}
