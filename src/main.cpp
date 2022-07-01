#include "raylib.h"
#include "easefunctions.h"
#include "structs.h"
#include "stringsToUse.h"
#include "music.h"
#include "raymath.h"

#define MAX_STARS 512
#define STARFIELD_START_Y 100
#define STARFIELD_END_Y 85
#define DEMO_TEXT_POS_Y 400
#define NUM_STRINGS 6
#define FONT_SIZE 25


int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------

    //Initialize the window
    //-Screen dimensions-
    const int screenWidth = 800;
    const int screenHeight = 450;

    //FPS and Global Timer
    const int fps = 60;
    int ticktime = 0;


    const int duration = 5*fps; //used more as a speed rate instead of the total time

    //Final initializations of the window
    InitWindow(screenWidth, screenHeight, "wavelight cracktro");
    InitAudioDevice();                 
    SetTargetFPS(fps); //Set the FPS to whatever is in the fps variable
    //--------------------------------------------------------------------------------------

    //Initialize the objects

    //Load the font!
    int fontLength = 0;
    unsigned char *decodedFont = DecodeDataBase64(encodedFont, &fontLength);

    Font cool = LoadFontFromMemory(".ttf", decodedFont, fontLength, FONT_SIZE, 0, 256);
    float hue = 0.0f; //load the hue (in HSL) counter

    //Load the scene logo!

    int logolength = 0;
    unsigned char *decodedLogo = DecodeDataBase64(encodedDemoLogo, &logolength);

    Image logo = LoadImageFromMemory(".png", decodedLogo, logolength);
    Texture2D cracktrologo = LoadTextureFromImage(logo);
    UnloadImage(logo);

    const int frameWidth = cracktrologo.width; const int frameHeight = cracktrologo.height;
    Rectangle sourceRec = {0.0f, 0.0f, (float)frameWidth, (float)frameHeight-1.0f};
    Rectangle destRec = {screenWidth/2.0f, 50, (float)frameWidth, (float)frameHeight};
    Vector2 origin = {(float)frameWidth/2.0f, (float)frameHeight/2.0f};

    //Load the funky music!
    int musicLength = 0;
    unsigned char *decodedMusic = DecodeDataBase64(musicBase64, &musicLength);

    Music cracktromus = LoadMusicStreamFromMemory(".xm", decodedMusic, musicLength);

    PlayMusicStream(cracktromus);

    //Initialize the starfield, and set their positions, speed, colour and size
    StarWave stars[MAX_STARS] = {0};
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].color = WHITE;
        stars[i].width = stars[i].height = 1;
        stars[i].position.y = (float)GetRandomValue(101, screenHeight-(STARFIELD_END_Y+2));
        stars[i].position.x = (float)GetRandomValue(0, screenWidth);
        stars[i].speed = ((float)GetRandomValue(100, 600)/2.5f)/(float)fps;
    }

    //Initialize the required variables for wavy text
    int numString = 0;      //number of strings
    int wordsAmount = 1;    //amount of characters in one string
    Vector2 demoTextPos;    //demoscene text position
    int angle = 0;          //angle to use in sine wave

    //shhhh, no need to know about this one... yet ;)
    Vector2 mousePoint = { 0.0f, 0.0f };
    int ovalCounter = 0;
    int ovalTimes = 0;
    bool isOvalGoodToGo = false;

    Vector2 dvdMovement = {100.0f/(float)fps, 100.0f/(float)fps};
    Vector2 dvdPosition = {screenWidth/2.0f-150.0f, screenHeight/2.0f};

    int dvdlength = 0;
    unsigned char *decodedDVD = DecodeDataBase64(encodedDVD, &dvdlength);

    Image DVDlogo = LoadImageFromMemory(".png", decodedDVD, dvdlength);
    Texture2D dvd = LoadTextureFromImage(DVDlogo);

    UnloadImage(DVDlogo);

    int ovalAngle = 0;

    int ovalWavLength = 0;
    unsigned char *ovalWav = DecodeDataBase64(encodedOval, &ovalWavLength);

    Wave ovalWave = LoadWaveFromMemory(".wav", ovalWav, ovalWavLength);
    Sound ovalSound = LoadSoundFromWave(ovalWave);

    UnloadWave(ovalWave);
    

    PlayMusicStream(cracktromus);

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        UpdateMusicStream(cracktromus); //Update the music at every tick

        mousePoint = GetMousePosition();
        //Stars animation
        for (int i = 0; i < MAX_STARS; i++) {
            if (stars[i].position.x >= screenWidth) {
                stars[i].position.x = 0;
                stars[i].position.y = (float)GetRandomValue(101, screenHeight-(STARFIELD_END_Y+2));
            }
            stars[i].position.x += stars[i].speed;
        }

        if (IsKeyPressed(KEY_ENTER)) {
            ovalCounter = 2*fps;
            ovalTimes++;
            if (ovalTimes == 5) {
                (isOvalGoodToGo == false) ? isOvalGoodToGo = true : isOvalGoodToGo = false;
                ovalTimes = 0;
                ovalAngle = 0;
                PlaySound(ovalSound);
            }
        }
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLACK);
            Color rainbow = ColorFromHSV(hue, 1, 1); //wtf its like that rainbow block from hit game terraria!!

            DrawRectangleGradientV(0,0,screenWidth, screenHeight, {34, 48, 87, 255}, {1, 8, 35, 255}); //Gradient background

            //Draw the funny starfield!
            DrawRectangle(0, STARFIELD_START_Y, screenWidth, screenHeight-(STARFIELD_START_Y+STARFIELD_END_Y), BLACK); //Make the star field area only black
            DrawLine(0, STARFIELD_START_Y, screenWidth, STARFIELD_START_Y, rainbow); //Draw the start rainbow line
            DrawLine(0, screenHeight-STARFIELD_END_Y, screenWidth, screenHeight-STARFIELD_END_Y, rainbow); //Draw the end rainbow line

            for (int i = 0; i < MAX_STARS; i++) {
                DrawRectangleV(stars[i].position, {(float)stars[i].width, (float)stars[i].height}, stars[i].color);
            } //Draw the stars! (or pixel-sized squares but shshshssshhh)

            //Draw the logo!
            if (isOvalGoodToGo == true)  {
                destRec = {screenWidth/2.0f+((float)bouncyWave(35, (ovalAngle/2))), 50+((float)bouncyWave(10, (ovalAngle))), (float)frameWidth, (float)frameHeight};
                (ovalAngle < 720) ? ovalAngle+=(300/fps) : ovalAngle = 0; 
            }
            else destRec = {screenWidth/2.0f, 50, (float)frameWidth, (float)frameHeight};
            

            DrawTexturePro(cracktrologo, sourceRec, destRec, origin, 0, WHITE);

            //Draw the text and make it  - = w a v y = -
            int textSize = 0;
            wordsAmount = TextLength(string0[numString]);
            for (int i = 0; i < wordsAmount; i++) {
                if ( (i-1) != -1)  textSize += (int)MeasureTextEx(cool, TextSubtext(string0[numString], i-1, 1), FONT_SIZE, 0).x; //Measure each character before the next one so it makes a cohesive sentence
                demoTextPos = {(float)linearLerp(ticktime, screenWidth+textSize, -(screenWidth), duration), (float)DEMO_TEXT_POS_Y+((float)bouncyWave(15, (angle+i*5)%360))};
                DrawTextEx(cool, TextSubtext(string0[numString], i, 1), demoTextPos, FONT_SIZE, 0, RAYWHITE);
            }

            if (isOvalGoodToGo == true) {
                DrawTextureV(dvd, dvdPosition, WHITE);

                if (dvdPosition.y >= (float)((screenHeight-STARFIELD_END_Y)-dvd.height-1)) dvdMovement.y *= -1.0f;
                else if (dvdPosition.y <= (float)STARFIELD_START_Y+1) dvdMovement.y *= -1.0f;

                if (dvdPosition.x >= screenWidth-dvd.width) dvdMovement.x *= -1.0f;
                if (dvdPosition.x <= 0) dvdMovement.x *= -1.0f;


                dvdPosition = Vector2Add(dvdPosition, dvdMovement);
            }
        EndDrawing();
        //----------------------------------------------------------------------------------
        
        //Counters
        if (demoTextPos.x >= 0-100) ticktime++; //Global tick counter
        else {
            ticktime = 0; (numString != NUM_STRINGS) ? numString++ : numString = 0;
        }
        (hue != 360.0f) ? hue += 1.0f : hue = 0; //hue counter
        (angle < 360) ? angle+=(300/fps) : angle = 0;  //angle counter (reason why its different is because it adds every 300 degrees divded by FPS (like some weird way of making it separate from FPS))
        if (ovalCounter > 0) ovalCounter--; 
        else {
            ovalCounter = 0;
            ovalTimes = 0;
        }
        
    }

    
    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadFont(cool);
    UnloadMusicStream(cracktromus);          // Unload music stream buffers from RAM
    CloseAudioDevice(); 
    UnloadTexture(cracktrologo); 
    UnloadSound(ovalSound);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}