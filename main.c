
#include "screens.h" 


typedef enum {
    MENU,
    PLAY,
    SHOP,
    EXIT
} GameState;


int main(void) {
    Sound button;
    Sound blib;
    
    
    /*int monitor=GetCurrentMonitor();
    int screenWidth=GetMonitorWidth(monitor);
    int screenHeight=GetMonitorHeight(monitor);*/

    int screenWidth=1366;
    int screenHeight=786;
    InitWindow(screenWidth, screenHeight, "Multi-File Game");

    InitAudioDevice();

    SetTargetFPS(60);


    // Load background music
    Music bgMusic = LoadMusicStream("sprites\\resources - voix\\Menu Music 07.mp3");
    //Music bgMusic = LoadMusicStream("sprites\\resources - voix\\Zombie_cover.mp3");
    //Music gamemusic=LoadMusicStream("sprites\\resources - voix\\Menu Music 07.mp3");

    // Set to loop
    bgMusic.looping = true;
    //gamemusic.looping=true;

    // Start playing
    //PlayMusicStream(bgMusic);

    GameState currentState = MENU;
    int selectedOption = 0;

    while (!WindowShouldClose()) {
        // --- LOGIC ---

        
        UpdateMusicStream(bgMusic);
        //UpdateMusicStream(gamemusic);

        if (currentState == MENU) {
            if (IsKeyPressed(KEY_UP)) {
                PlaySound(blib);
                selectedOption--;
                if (selectedOption < 0) selectedOption = 2;
            }
            if (IsKeyPressed(KEY_DOWN)) {
                PlaySound(blib);
                selectedOption++;
                if (selectedOption > 2) selectedOption = 0;
            }
            if (IsKeyPressed(KEY_ENTER)) {
                PlaySound(button);
                switch (selectedOption) {
                    case 0: currentState = PLAY;  break;
                    case 1: currentState = SHOP;  break;
                    case 2: currentState = EXIT; break;
                }
            }
        } 
        else if (currentState == PLAY || currentState == SHOP) {
            // Handle returning to menu
            if (IsKeyPressed(KEY_BACKSPACE)) {
                currentState = MENU;
            }
        }

        // --- DRAWING ---
        BeginDrawing();
            // We don't need ClearBackground here if every screen handles it,
            // but it's safer to keep a default one here or inside the functions.
            
            if (currentState == MENU) {

                static bool isloaded=false;
                Texture2D background;
                
                if(!isloaded){
                background= LoadTexture("sprites\\Textures\\menubg.jpg");
                button=LoadSound("sprites\\resources - voix\\SFX buttonclick.ogg");
                blib=LoadSound("sprites\\resources - voix\\SFX bleep.ogg");
                isloaded=true;
                }

                DrawTextureEx(background,(Vector2){0,0},0,1.14,WHITE);
                DrawText("press enter to select an option",250,400,20,BLACK);
                DrawText("click Esc to exit",350,450,10,BLACK);
                
                const char *options[3] = { "Play", "Shop", "Exit" };
                for (int i = 0; i < 3; i++) {
                    Color color = (i == selectedOption) ? RED : BLACK;
                    int size=(i == selectedOption) ? 40 : 30;
                    DrawText(options[i], 800/2 - MeasureText(options[i], 30)/2, 150 + i * 50, size, color);
                }
            } 
            else if (currentState == PLAY) {

                DrawPlayScreen(&bgMusic/*,&gamemusic*/); 
            } 
            else if (currentState == SHOP) {
                // Call the function from the other file
                DrawShopScreen();
            }
            else if (currentState == EXIT) {
                UnloadEverything();
                UNloadZombies();
                EndDrawing();
                UnloadMusicStream(bgMusic);
                //UnloadMusicStream(gamemusic);
                CloseWindow();
                return 0;
            }

        EndDrawing();
    }

    UnloadMusicStream(bgMusic);
    CloseWindow();
    return 0;
}