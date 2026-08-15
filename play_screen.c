//play_screen.c file


#include "screens.h"

Sound button;
Sound blib;
void DrawPlayScreen(Music *bgmusic) {

    
    

    static bool isLoaded = false;
    if (!isLoaded){
        initbackground("sprites\\Textures\\levels.jpg");
        button=LoadSound("sprites\\resources - voix\\SFX buttonclick.ogg");
        blib=LoadSound("sprites\\resources - voix\\SFX bleep.ogg");
         isLoaded=true;
    }

    static Gamelevels currentlevel = playscreen;
    static int selectedOption = 0;


     if (currentlevel == playscreen) {
        //PauseMusicStream(*gamemusic);
        //ResumeMusicStream(*bgmusic);
        if (IsKeyPressed(KEY_RIGHT)) {
            PlaySound(blib);
            selectedOption++;
            if (selectedOption > 3) selectedOption = 0;
        }
        if (IsKeyPressed(KEY_LEFT)) {
            PlaySound(blib);
            selectedOption--;
            if (selectedOption < 0) selectedOption = 3;
        }
        if (IsKeyPressed(KEY_SPACE)) {
            PlaySound(button);
            switch (selectedOption) {
                case 0: { currentlevel = level1; break;}
                case 1: {currentlevel = level2; break;}
                case 2: {currentlevel = level3; break;}
                case 3: { currentlevel = level4; break;}
            }
            initiation(currentlevel);
            //PauseMusicStream(*bgmusic);
            //ResumeMusicStream(*gamemusic);
        }
    } 
    else {
        
        if (IsKeyPressed(KEY_BACKSLASH)) {
            currentlevel = playscreen;
            ResetGameState();
        }
    }

//--------------DRAW-----------------------
    
    if (currentlevel == playscreen) {
        

        
        drawbg(1,(Vector2){0,0});
        DrawText("click Esc to exit",110,700,15,WHITE);


        DrawText("PLAY MODE ", 550, 200, 40, WHITE);
        DrawText("press space to select an option",20,550,20,WHITE);
        DrawText("Press BACKSPACE to return to the menu ", 20, 650, 20, WHITE);
        DrawText("in the game, you can press Backslash to return to this menu", 20, 600, 20, WHITE);
        //DrawText("in the game, you can plant things with the keyboard, : \n P -> Peashooter \n S -> Sunflower \n B -> Potatomine \n C -> Chomper \n J -> Jalapeno ",700,550,20,WHITE);

        for (int i = 0; i < 4; i++) {
            
            int size=(i == selectedOption) ? 50 : 40;
            int textwidth = MeasureText(TextFormat( "Level %d", i + 1), size);
            int width = textwidth * 1.5;
            int height = size * 1.8;

            Rectangle thisrec={100 + 300 * i, 350, width, height};

            DrawRectangleRounded(thisrec,0.3,0,(Color){255,255,255,210});
            //DrawRectangle(100 + 300 * i, 350, width, height,(Color){255,255,255,100} );
            Color color = (i == selectedOption) ? RED : BLACK;
            DrawText(TextFormat( "Level %d", i + 1), 150 + 300 * i, 360, size, color);
        }
    } 
    else {

        DrawLevel1(&currentlevel);
    }
        
    
}