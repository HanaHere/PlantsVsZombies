#include "screens.h"
#include "raylib.h"
#define FILENAME "savefile.dat"

// Structure to hold game data


// Function to get the current game data (coins and potatoes)
GameData GetGameData() {
    FILE *file = fopen(FILENAME, "rb");
    GameData data = {0, 0};
    
    
    if (file == NULL) {
        // File does not exist, create it with initial values
        file = fopen(FILENAME, "wb");
        if (file != NULL) {
            data.coins = 1000;
            data.potatoes = 0;
            fwrite(&data, sizeof(GameData), 1, file);
            fclose(file);
            printf("New save file created with 1000 coins and 0 potatoes.\n");
        } else {
            printf("Error creating save file.\n");
        }
    } else {
        // File exists, read the data
        fread(&data, sizeof(GameData), 1, file);
        fclose(file);
    }
    
    return data;
}

// Function to save game data
void SaveGameData(GameData data) {
    FILE *file = fopen(FILENAME, "wb");
    if (file != NULL) {
        fwrite(&data, sizeof(GameData), 1, file);
        fclose(file);
    }
}

// Keep these for backward compatibility if needed
int GetCoins() {
    GameData data = GetGameData();
    return data.coins;
}

void SaveCoins(int coins) {
    GameData data = GetGameData();
    data.coins = coins;
    SaveGameData(data);
}


void DrawShopScreen(void) {

    // Load current game data
    GameData gameData = GetGameData();
    int currentCoins = gameData.coins;
    int currentPotatoes = gameData.potatoes;
    static float sizeA=1;
    static float sizeB=1;
    static float sizeC=1;
    
    // Define error message variables
    static bool showError = false;
    static bool showPotatoError = false;
    static bool filled = false;
    static float messageTimer = 3;
    static bool isloaded=false;
    static Texture2D bg;

    if(!isloaded){

    
        initbackground("sprites\\Textures\\background.jpg");
        isloaded=true;
    }
    
    // Define Rectangles for collision detection
    Rectangle item1Rect = { 150, 450, sizeA * 200,sizeA * 150 };      // Potato item
    Rectangle item2Rect = { 450, 450, sizeB * 200,sizeB * 150 };      // Plant B
    Rectangle resetRect = { 850, 450, sizeC* 200, sizeC* 150 };      // Reset button
    Vector2 mousePos = GetMousePosition();
    // Check for Mouse Clicks
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        
        
        // Buy Potato (Cost 50 coins)
        if (CheckCollisionPointRec(mousePos, item1Rect)) {
            if (currentCoins >= 50) {
                currentCoins -= 50;
                currentPotatoes += 1;
                
                // Save the updated data
                gameData.coins = currentCoins;
                gameData.potatoes = currentPotatoes;
                SaveGameData(gameData);
                
                printf("Bought a potato! Total potatoes: %d\n", currentPotatoes);
            } else {
                showError = true; // Not enough coins
            }
        }
        
        // Buy Plant B (Cost 250 coins)
        if (CheckCollisionPointRec(mousePos, item2Rect)) {
            if (currentCoins >= 250) {
                currentCoins -= 250;
                
                gameData.coins = currentCoins;
                gameData.potatoes = currentPotatoes;
                SaveGameData(gameData);
            } else {
                showError = true;
            }
        }
        
        // Reset to 1000 coins (but keep potatoes)
        if (CheckCollisionPointRec(mousePos, resetRect)) {
            if (currentCoins != 1000) {
                currentCoins = 1000;

                
                gameData.coins = currentCoins;
                gameData.potatoes = currentPotatoes;
                SaveGameData(gameData);
            } else {
                filled = true;
            }
        }
        
    }


        
        
        // Buy Potato (Cost 50 coins)
        if (CheckCollisionPointRec(mousePos, item1Rect)) {
          
                sizeA=1.2;
                sizeB=1;
                sizeC=1;
        }
        
        // Buy Plant B (Cost 250 coins)
        if (CheckCollisionPointRec(mousePos, item2Rect)) {
           
                sizeA=1;
                sizeB=1.2;
                sizeC=1;
        }
        
        // Reset to 1000 coins (but keep potatoes)
        if (CheckCollisionPointRec(mousePos, resetRect)) {
            
                sizeA=1;
                sizeB=1;
                sizeC=1.2;}
        
    



    if (!CheckCollisionPointRec(mousePos, resetRect)&&!CheckCollisionPointRec(mousePos, item2Rect)&&!CheckCollisionPointRec(mousePos, item1Rect)){
                sizeA=1;
                sizeB=1;
                sizeC=1;

        }
    
    
    
    // Draw Everything
    drawbg(1,(Vector2){0,0});

    

    DrawText(TextFormat("Coins: %4d", currentCoins), 250, 350, 30, GOLD);
    
    // Draw Potato Item
    DrawRectangleRec(item1Rect, LIGHTGRAY);
    DrawText("POTATO", 200, 500, 20, BLACK);
    DrawText("Cost: 50", 210, 530, 20, BLACK);
    DrawText(TextFormat("Own: %d", currentPotatoes), 210, 560, 20, DARKGREEN);
    
    // Draw Plant B
    DrawRectangleRec(item2Rect, LIGHTGRAY);
    DrawText("Plant B", 500, 500, 20, BLACK);
    DrawText("Cost: 250", 510, 530, 20, BLACK);
    
    // Draw Reset button
    DrawRectangleRec(resetRect, LIGHTGRAY);
    DrawText("Reset", 900, 500, 20, BLACK);
    DrawText("Coins", 910, 530, 20, BLACK);
    
    // Draw Error Messages
    if (showError) {
        messageTimer -= GetFrameTime();
        if (messageTimer >= 0) {
            DrawText("NOT ENOUGH COINS!", 260, 100, 30, RED);
        } else {
            messageTimer = 3;
            showError = false;
        }
    }
    
    if (filled) {
        messageTimer -= GetFrameTime();
        if (messageTimer >= 0) {
            DrawText("ALREADY 1000 COINS!", 240, 100, 30, RED);
        } else {
            messageTimer = 3;
            filled = false;
        }
    }
    
    DrawText("Press Backspace to return", 300, 700, 20, WHITE);
}


// Function to use a potato (call this when player uses a potato in the game)
bool UsePotato() {
    GameData data = GetGameData();
    
    if (data.potatoes > 0) {
        data.potatoes -= 1;
        SaveGameData(data);
        printf("Potato used! Remaining potatoes: %d\n", data.potatoes);
        return true; // Successfully used a potato
    } else {
        printf("No potatoes available!\n");
        return false; // No potatoes to use
    }
}

// Function to add potatoes (for rewards, etc.)
void AddPotatoes(int amount) {
    GameData data = GetGameData();
    data.potatoes += amount;
    SaveGameData(data);
    printf("Added %d potatoes! Total: %d\n", amount, data.potatoes);
}



int GetPotatoCount() {
    GameData data = GetGameData();
    return data.potatoes;
}