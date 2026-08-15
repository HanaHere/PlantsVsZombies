// If you want to add a "Settings" window, just create settings.c, add the function to screens.h,
// add the file name to tasks.json, and call it from main.c .


#define DEBUG1 0
#define DEBUG2 0
//screens.h

#ifndef SCREENS_H
#define SCREENS_H


#include "RAYLIB/include/raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


typedef enum{FREE,SUN_FLOWER,PEA_SHOOTER,CHOMPER,POTATO,Jalapeno,ZOMBIE,FLAG_ZOMBIE}plantType;


//-----PAGES------
//void DrawPlayScreen(Music *bgmusic,Music *gamemusic);
void DrawPlayScreen(Music *bgmusic);
void DrawShopScreen(void);

typedef struct {
    int coins;
    int potatoes;
} GameData;
int GetPotatoCount();
bool UsePotato();
void AddPotatoes(int amount);
int GetCoins();
void SaveGameData(GameData data);
GameData GetGameData();

void ResetGameState(void);

typedef enum {
    playscreen,
    level1,
    level2,
    level3,
    level4
} Gamelevels;

void initiation (Gamelevels thislevel);
void SetData(Gamelevels thislevel, int *score);

void DrawLevel1(Gamelevels *thislevel);

typedef struct{
    
    int TotalNormalZmb;
    float seconds_per_Zombie;
    float zombie_speed;

    int TotalFlagZmb;
    float seconds_per_FlagZombie;
    float FlagZombie_Speed;
    
}Levels;
extern Levels L[5];




// Function Declarations
void DrawTextBox(int x,int y,const char *text,int size,Color bgcolor,Color textcolor);
void initbackground(const char *text);
void drawbg(float scale,Vector2 positionbg);

void initgameBG(const char *text);
void drawgameBG(void);

void GlobalVariables(void);
void UnloadEverything(void);
void UNloadZombies(void);



// GameElements :

extern Vector2 mousePos;

// --- Lawn Mower Functions ---
typedef struct{
    Texture2D texture;
    Rectangle rec;
    bool ishere;
    bool onlyonce;
    bool isgone;
    bool sound;
} lawnmower;

extern lawnmower myLawnMower[5];
void InitLawnMower(void);
void DrawLawnMower(int i);

// --- Star Functions ---
typedef struct{
    Texture2D texture;
    Vector2 position;
    float rotation;
    bool isLanded;
    float timer;
    int landingY;
    int startX;
    int startY;
    bool isCollected;
    bool isActive;

} Star;
extern Star myStar[46];
void InitStar(void); 
void UpdateFallingStar(int *score); 
void UpdatePoppingStar(int *score,int i);
void DrawStar(int i);


// ============Plants==============
typedef struct{
    Texture2D *textures;
    Rectangle rec;
    bool planted;
    int health;
    float animation;
    int animation_frame;
    float lifespan;
    int row;
    bool explosion;

} Plants;
extern Plants mySunflower[45];
extern Plants myPeaShooter[45];
extern Plants mychomper[45];
extern Plants mypotato[45];
extern Plants myjala[45];

// --- Sunflower Functions ---
void InitSunflower(void);
void DrawSunflower(int i, int (*planted)[6]);

// --- PeaShooter Functions ---
void InitPeaShooter(void);
void DrawPeaShooter(int i, int (*planted)[6]);

void shootpea(int i,Gamelevels thislevel);

// --- Chomper ---
void initChomper(void);
void UpdateChomper(int i,int numzombie,int numflagz,Gamelevels thislevel);
void DrawChomper(int i, int (*planted)[6]);

//-----Potato---------
void InitPotato(void);
void UpdatePotato(int i,int numzombie,int numflagz,Gamelevels thislevel);
void DrawPotato(int i, int (*planted)[6]);

//-----Jalapeno---------
void InitJala(void);
void UpdateJala(int i,int numzombie,int numflagz,Gamelevels thislevel);
void DrawJala(int i, int (*planted)[6]);


//============= Zombies =================
typedef struct{
    Texture2D *textures;

    Rectangle rec;
    bool activated;
    int health;
    float speed;
    float animation;
    int animation_frame;
    int row;
    float cooldown;
    bool isdead;
}zombie;


static int damage_Zombie = 2;


//-------Normal Zombie
extern zombie NormalZombie[30];
void initZombie(Gamelevels thislevel);
void CallTheZombies(int *i,Gamelevels thislevel);
void UpdateZombies(int i,bool *gameover,Gamelevels thislevel);
void drawZombie(int i);


//----Flag Zombie------
extern zombie flagZombie[30];
void initFlagZombie(Gamelevels thislevel);
void CallTheFlagZombies(int *i,Gamelevels thislevel);
void UpdateFlagZombies(int i,bool *gameover,Gamelevels thislevel);

void updateweight(int row);

void drawFlagZombie(int i);


//WEIGHT OF FLAGzombies:
typedef struct{
    bool BeChecked;
    float weight;
}weight;
extern weight myrow[7];






//====SideBar
typedef struct
{
    bool isAvailable;
    Texture2D texture;
    Rectangle rec;
    float timer;
    bool key;
    float lastPlantTime;
    int cost;
    int strength;
    Sound sound;
    bool played;
} ToPlant;
void init_sb_PeaShooter(void);
void init_sb_SunFlower(void);
void init_sb_Chomper(void);
void init_sb_Potato(void);
void init_sb_Jalapeno(void);
extern ToPlant sbplant[6];

//int FindFreePlantSlot(Plants plantArray[], plantType type);
void UpdatePlanting(int *num, int (*planted)[6], int *score, int *type);
void DrawSidebar( int *score,int i);




#endif
