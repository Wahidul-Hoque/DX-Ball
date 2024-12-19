#include "iGraphics.h"
#include <windows.h>
#include<mmsystem.h>

#include<bits\stdc++.h>
using namespace std;

#define MENU 0
#define PLAY 1
#define EXIT 2
#define INSTRUCTION 3
#define RES 4
#define OUT 5
#define WIN 6
#define PLAYER_NAME 7
#define HIGH_SCORE 8

#define LEVEL1 10
#define LEVEL2 101
#define LEVEL3 102

#define HIGH_SCORE1 501
#define HIGH_SCORE2 502
#define HIGH_SCORE3 503

int score=0;
int lives=3;

int level_state=0;
int game_state = PLAYER_NAME;
int high_score_state=HIGH_SCORE1;

int screen_width= 1400, screen_height = 800;
int btn_width = 200, btn_height = 50;
int btn_x = 200, btn_y= 500;
int btn_gap = 20;

int back_btn_x = 30, back_btn_y = 740;
int back_btn_width = 100, back_btn_height = 40;

int game_start=0;
int cond=0;
int *p= &cond;
int extra=0;
int *e=&extra;

// pdl properties
int pdl_x = 150, pdl_y = 20, pdl_width = 100, pdl_height = 10;

// Ball properties
int ball_radius = 8, ball_x = 200 , ball_y = 37 ;
int dx = 7, dy = 7;
// Bricks properties
int brick_rows = 8, brick_cols = 12;
int brick_width = 90, brick_height = 28;
int brick_f_x = 130, brick_f_y = 490;
int bricks[6][12];
int current_level;

bool game_running = false; // Whether the game is currently running
char save_file[] = "txt_files\\save_game-1.txt"; // File to save/load game state

bool isPlaying = false;
void playThemeSong() {
    if (!isPlaying) {
        PlaySound(TEXT("sound\\Theme_song.wav"), NULL, SND_ASYNC | SND_LOOP);
        isPlaying = true;
    }
}

void stopThemeSong() {
    if (isPlaying) {
        PlaySound(NULL, 0, 0); // Stop the sound
        isPlaying = false;
    }
}

#define MAX_PLAYERS 100
#define TOP_SCORES 10
#define HIGH_FILE "txt_files\\scores-1.txt"
#define HIGH_FILE_1 "txt_files\\scores-1.txt"
#define HIGH_FILE_2 "txt_files\\scores-2.txt"
#define HIGH_FILE_3 "txt_files\\scores-3.txt"

char playerName[100] = "";
char show[100];
int copyname=0;
int nameLength = 0;

void displayName() {
    iClear();
    iShowBMP(0,0,"BMP/back.bmp");
    iShowBMP(500,650,"BMP/dxball.bmp");
    iSetColor(255,255 , 255);
    iText(600, 400, "Enter Your Name", GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(152, 242, 250);
    iFilledRectangle(490, 350, 400, 40);
    iSetColor(0,0 , 0);
    iText(500, 365, playerName, GLUT_BITMAP_TIMES_ROMAN_24);
}

int name_save=0;
typedef struct {
    char name[50];
    int score;
} Player;

Player players[MAX_PLAYERS];
int playerCount = 0;
Player currentPlayer;

// Function to load scores from file
void loadScores() {
    if(level_state==LEVEL1){
        FILE *file = fopen(HIGH_FILE_1, "r");
        playerCount = 0;
        while (fscanf(file, "%s %d", players[playerCount].name, &players[playerCount].score) != EOF) {
            playerCount++;
        }
        fclose(file);
    }
    else if(level_state==LEVEL2){
        FILE *file = fopen(HIGH_FILE_2, "r");
        playerCount = 0;
        while (fscanf(file, "%s %d", players[playerCount].name, &players[playerCount].score) != EOF) {
            playerCount++;
        }
        fclose(file);
    }
    else if(level_state==LEVEL3){
        FILE *file = fopen(HIGH_FILE_3, "r");
        playerCount = 0;
        while (fscanf(file, "%s %d", players[playerCount].name, &players[playerCount].score) != EOF) {
            playerCount++;
        }
        fclose(file);
    }
}

void saveScores() {
    if(level_state==LEVEL1){
        FILE *file = fopen(HIGH_FILE_1, "w");
        for (int i = 0; i < playerCount; i++) {
            fprintf(file, "%s %d\n", players[i].name, players[i].score);
        }
        fclose(file);
    }
    else if(level_state==LEVEL2){
        FILE *file = fopen(HIGH_FILE_2, "w");
        for (int i = 0; i < playerCount; i++) {
            fprintf(file, "%s %d\n", players[i].name, players[i].score);
        }
        fclose(file);
    }
    else if(level_state==LEVEL3){
        FILE *file = fopen(HIGH_FILE_3, "w");
        for (int i = 0; i < playerCount; i++) {
            fprintf(file, "%s %d\n", players[i].name, players[i].score);
        }
        fclose(file);
    }
}

// Function to add a new score
void addScore(const char *name, int score) {
    loadScores();
    strcpy(players[playerCount].name, name);
    players[playerCount].score = score;
    playerCount++;

    //(descending)
    for (int i = 0; i < playerCount - 1; i++) {
        for (int j = i + 1; j < playerCount; j++) {
            if (players[j].score > players[i].score) {
                Player temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }

    //top 10
    if (playerCount > TOP_SCORES) {
        playerCount = TOP_SCORES;
    }

    saveScores();
}

void drawBackButton() {
    iSetColor(152, 242, 250);
    iFilledRectangle(back_btn_x, back_btn_y, back_btn_width, back_btn_height);
    iSetColor(0, 0, 0);
    iText(back_btn_x + 15, back_btn_y + 10, "BACK", GLUT_BITMAP_TIMES_ROMAN_24);
}
void drawNextButton() {
    iSetColor(152, 242, 250);
    iFilledRectangle(back_btn_x+1200, back_btn_y, back_btn_width, back_btn_height);
    iSetColor(0, 0, 0);
    iText(back_btn_x + 15+1200, back_btn_y + 10, "NEXT", GLUT_BITMAP_TIMES_ROMAN_24);
}
// Function to display scores 
void displayScore1(){
    FILE *file = fopen(HIGH_FILE_1, "r");
    playerCount = 0;
    while (fscanf(file, "%s %d", players[playerCount].name, &players[playerCount].score) != EOF) {
        playerCount++;
    }
    fclose(file);
    iClear();
    iShowBMP(0,0,"BMP/back.bmp");
    drawBackButton();
    drawNextButton();
    iSetColor(255,255,255);
    iText(550, 630, "Top 10 High Scores (LEVEL 1)", GLUT_BITMAP_TIMES_ROMAN_24);
    for (int i = 0; i < playerCount; i++) {
        char scoreText[100];
        sprintf(scoreText, "%d. %s - %d", i + 1, players[i].name, players[i].score);
        iText(625, 550 - i * 35, scoreText,GLUT_BITMAP_TIMES_ROMAN_24);
    }
}
void displayScore2(){
    FILE *file = fopen(HIGH_FILE_2, "r");
    playerCount = 0;
    while (fscanf(file, "%s %d", players[playerCount].name, &players[playerCount].score) != EOF) {
        playerCount++;
    }
    fclose(file);
    iClear();
    iShowBMP(0,0,"BMP/back.bmp");
    drawBackButton();
    drawNextButton();
    iSetColor(255,255,255);
    iText(550, 630, "Top 10 High Scores (LEVEL 2)", GLUT_BITMAP_TIMES_ROMAN_24);
    for (int i = 0; i < playerCount; i++) {
        char scoreText[100];
        sprintf(scoreText, "%d. %s - %d", i + 1, players[i].name, players[i].score);
        iText(625, 550 - i * 35, scoreText,GLUT_BITMAP_TIMES_ROMAN_24);
    }
}
void displayScore3(){
    FILE *file = fopen(HIGH_FILE_3, "r");
    playerCount = 0;
    while (fscanf(file, "%s %d", players[playerCount].name, &players[playerCount].score) != EOF) {
        playerCount++;
    }
    fclose(file);
    iClear();
    iShowBMP(0,0,"BMP/back.bmp");
    drawBackButton();
    iSetColor(255,255,255);
    iText(550, 630, "Top 10 High Scores (LEVEL 3)", GLUT_BITMAP_TIMES_ROMAN_24);
    for (int i = 0; i < playerCount; i++) {
        char scoreText[100];
        sprintf(scoreText, "%d. %s - %d", i + 1, players[i].name, players[i].score);
        iText(625, 550 - i * 35, scoreText,GLUT_BITMAP_TIMES_ROMAN_24);
    }
}


void Menu() {
    iClear();
    iShowBMP(0,0,"BMP/back.bmp");
    iShowBMP(500,650,"BMP/dxball.bmp");
    iSetColor(152, 242, 250);
    iFilledRectangle(btn_x+400, btn_y, btn_width, btn_height);
    iFilledRectangle(btn_x+400,btn_y-(btn_height + btn_gap), btn_width, btn_height);
    iFilledRectangle(btn_x+400, btn_y- 2 * (btn_height + btn_gap), btn_width, btn_height);
    iSetColor(232, 72, 83);
    iFilledRectangle(btn_x+400, btn_y- 3 * (btn_height + btn_gap), btn_width, btn_height);

	iSetColor(0, 0, 0);
    iText(btn_x+447, btn_y + 15, "Start Game",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(btn_x+447-5, btn_y-70 + 15, "Instructions",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(btn_x+450-20, btn_y-140 + 15, "Leader-Board",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(btn_x+450+25, btn_y-210 + 15, "Exit",GLUT_BITMAP_TIMES_ROMAN_24);
    
}
// Initialize bricks
void initializeBricks() {
    srand(time(NULL));
    for (int i = 0; i < brick_rows; i++) {
        for (int j = 0; j < brick_cols; j++) {
            bricks[i][j] = 5 ;
        }
    }
}

void Gap_brick_1(){
    for (int i = 0; i < brick_rows; ) {
        for (int j = 0; j < brick_cols; ) {
            bricks[i][j] = -1 ;
            j+=2;
        }
        i+=2;
    }
    for (int i = 1; i < brick_rows; ) {
        for (int j = 1; j < brick_cols; ) {
            bricks[i][j] = -1 ;
            j+=2;
        }
        i+=2;
    }
    for (int i = 1; i < brick_rows; ) {
        for (int j = 2; j < brick_cols; ) {
            bricks[i][j] = 0 ;
            j+=4;
        }
        i+=4;
    }
    for (int i = 1; i < brick_rows; ) {
        for (int j = 0; j < brick_cols; ) {
            bricks[i][j] = 1 ;
            j+=4;
        }
        i+=4;
    }

}

void Gap_brick_2(){

    for(int i = 0; i < brick_rows;i++ ){
        for(int j = 1; j < brick_cols; ){
            bricks[i][j] = 1 ;
            j+=2;
        }
    }
    bricks[0][0] = 0 ,bricks[7][0] = 0 ,bricks[0][11] = 0 ,bricks[7][11] = 0 ,bricks[1][3] = 0 ,bricks[0][6] = 0 ,bricks[1][9] = 0 ,bricks[4][0] = 0 ,bricks[3][3] = 0 ,bricks[4][6] = 0 ,bricks[3][9] = 0 ,bricks[4][11] = 0 ,bricks[6][3] = 0 ,bricks[7][6] = 0 ,bricks[6][9] = 0 ;
    for (int i = 2; i < brick_rows; ) {
        for (int j = 0; j < brick_cols; j++) {
            bricks[i][j] = -1 ;
            
        }
        i+=3;
    }

}

void Gap_brick_3()
{
    for(int i = 0; i < brick_rows;i++ ){
        bricks[i][0]=1;
        bricks[i][11]=1;
    }
    for(int i = 0; i < brick_cols;i++ ){
        bricks[0][i]=1;
        bricks[7][i]=1;
    }
    bricks[2][2]=1,bricks[5][2]=1,bricks[2][5]=1,bricks[5][5]=1,bricks[2][9]=1,bricks[5][9]=1,bricks[2][6]=1,bricks[5][6]=1;
}
void resume(){
    iClear();
    iShowBMP(0,0,"BMP/back.bmp");
	drawBackButton();
    iSetColor(152, 242, 250);
    iFilledRectangle(btn_x+400, btn_y, btn_width, btn_height);
    iFilledRectangle(btn_x+400,btn_y-(btn_height + btn_gap), btn_width, btn_height);
    iSetColor(255,255,255);
    iText(580,600,"Do you want to resume?", GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(0, 0, 0);
    iText(btn_x+450+20, btn_y + 15,"YES", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(btn_x+450+20, btn_y -70 + 15, "NO",GLUT_BITMAP_TIMES_ROMAN_24);
    

}

void level(){
    iClear();
	drawBackButton();
    iSetColor(152, 242, 250);
    iFilledRectangle(btn_x+400, btn_y, btn_width, btn_height);
    iFilledRectangle(btn_x+400,btn_y-(btn_height + btn_gap), btn_width, btn_height);
    iFilledRectangle(btn_x+400, btn_y- 2 * (btn_height + btn_gap), btn_width, btn_height);

    iSetColor(0, 0, 0);
    iText(200,400,"BRICK BREAKER GAME", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(btn_x+60+400, btn_y + 15, "Level 1",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(btn_x+60+400, btn_y -70 + 15, "Level 2",GLUT_BITMAP_TIMES_ROMAN_24);
    iText(btn_x+60+400, btn_y -140 + 15, "Level 3",GLUT_BITMAP_TIMES_ROMAN_24);
}

void instructions(){
    iClear();
    iShowBMP(0,0,"BMP/back.bmp");
	drawBackButton();
    iSetColor(255, 255, 255);
    iText(100, 610, "Use 'q' during game-play to save game and exit.", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(100, 550, "Use Left/Right arrows to move the paddle.", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(100, 490, "Press 'Space-bar' to pause and resume the game.", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(100, 430, "Press 'm' during game-play to go to menu page.", GLUT_BITMAP_TIMES_ROMAN_24);
    iShowBMP(100,370,"BMP\\gray.bmp");
    iText(200, 370, "Unbreakable Brick", GLUT_BITMAP_TIMES_ROMAN_24);
    iShowBMP(100,310,"BMP\\green.bmp");
    iText(200, 310, "Needs one hit to break", GLUT_BITMAP_TIMES_ROMAN_24);
    iShowBMP(100,250,"BMP\\blue.bmp");
    iText(200, 250, "Needs two hits to break", GLUT_BITMAP_TIMES_ROMAN_24);
    iPauseTimer(0);
}

void out(){
    iClear();
    iShowBMP(0,0,"BMP/back.bmp");
	iSetColor(232, 72, 83);
    iFilledRectangle(back_btn_x, back_btn_y, back_btn_width, back_btn_height);
    iSetColor(0, 0, 0);
    iText(back_btn_x + 15, back_btn_y + 10, "EXIT", GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(255,255,255);
    iText(640,640,"GAME  OVER!", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(620,600,"YOUR  SCORE IS ", GLUT_BITMAP_TIMES_ROMAN_24);
    char sc[20];
    sprintf(sc, "%d", score);
    iText(820,600, sc, GLUT_BITMAP_TIMES_ROMAN_24);
    
}

void win(){
    iClear();
    iShowBMP(0,0,"BMP/back.bmp");
	iSetColor(232, 72, 83);
    iFilledRectangle(back_btn_x, back_btn_y, back_btn_width, back_btn_height);
    iSetColor(0, 0, 0);
    iText(back_btn_x + 15, back_btn_y + 10, "EXIT", GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(255,255,255);
    iText(450,640,"CONGRATS!! LEVEL COMPLETED ", GLUT_BITMAP_TIMES_ROMAN_24);
    iText(520,570,"YOUR  SCORE IS ", GLUT_BITMAP_TIMES_ROMAN_24);
    char sc[20];
    sprintf(sc, "%d", score);
    iText(720,570, sc, GLUT_BITMAP_TIMES_ROMAN_24);
}

void speed(int level_state){
    if(level_state==LEVEL1)dx=9,dy=9;
    else if(level_state==LEVEL2)dx=10,dy=10;
    else if(level_state==LEVEL3)dx=11,dy=11;
}
// Save the current game state to a file
void saveGameState() {
    FILE *out = fopen("save_game.txt", "w"); // Open the file in write mode
    if (!out) return; // Exit if the file can't be opened

    // Write basic game state
    fprintf(out, "%d\n", current_level);
    fprintf(out, "%d %d\n", screen_width, screen_height);
    fprintf(out, "%d %d %d %d\n", pdl_x, pdl_y, pdl_width, pdl_height);
    fprintf(out, "%d %d %d %d %d\n", ball_x, ball_y, ball_radius, dx, dy);
    fprintf(out, "%d\n", score);
    fprintf(out, "%d\n", lives);

    // Write brick states
    for (int i = 0; i < brick_rows; i++) {
        for (int j = 0; j < brick_cols; j++) {
            fprintf(out, "%d ", bricks[i][j]);
        }
        fprintf(out, "\n");
    }

    fclose(out); // Close the file
}
// Load the game state from a file

void loadGameState() {
    FILE *in = fopen("save_game.txt", "r"); // Open the file in read mode
    if (!in) {
        // If no save file exists, initialize default values
        initializeBricks();
        return;
    }

    // Read basic game state
    fscanf(in, "%d", &current_level);
    fscanf(in, "%d %d", &screen_width, &screen_height);
    fscanf(in, "%d %d %d %d", &pdl_x, &pdl_y, &pdl_width, &pdl_height);
    fscanf(in, "%d %d %d %d %d", &ball_x, &ball_y, &ball_radius, &dx, &dy);
    fscanf(in, "%d", &score);
    fscanf(in, "%d", &lives);

    // Read brick states
    for (int i = 0; i < brick_rows; i++) {
        for (int j = 0; j < brick_cols; j++) {
            fscanf(in, "%d", &bricks[i][j]);
        }
    }

    fclose(in); // Close the file
}

void iMouse(int button, int state, int mx, int my) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        if (game_state == MENU) {
            if((copyname)==0){
                strcpy(show,playerName);
                copyname=1;
            }
            
            stopThemeSong();
            if ((mx >= btn_x+400 && mx <= btn_x+600) && (my >= btn_y && my <= btn_y+btn_height)) {
                game_state = PLAY;
            }
            else if ((mx >= btn_x+400 && mx <= btn_x+600) && (my >= btn_y-70 && my <= btn_y+btn_height-70)) {
                game_state = INSTRUCTION;
            }
            else if ((mx >= btn_x+400 && mx <= btn_x+600) && (my >= btn_y-140 && my <=btn_y+btn_height-140)) {
                game_state = HIGH_SCORE;
                
            }
            else if ((mx >= btn_x+400 && mx <= btn_x+600) && (my >= btn_y-210 && my <=btn_y+btn_height-210)) {
                game_state = EXIT;
                exit(0);
            }
        }
        else if (game_state == PLAY && *p==0 ){
            if ((mx >= btn_x+400 && mx <=btn_x+600) && (my >= btn_y && my <=btn_y+btn_height)){
                *e=1;
                *p=1;
                loadGameState();
                game_start=1;
                game_running = true;
            }
            else if ((mx >=btn_x+400 && mx <= btn_x+600) && (my >=btn_y-70 && my <=btn_y+btn_height-70)){
                *p=1;
            }
            else if((mx >= back_btn_x && mx <= back_btn_x+back_btn_width) && (my >= back_btn_y && my <=back_btn_y+back_btn_height)){
                *p=0;
				game_state=MENU;
			}
        }
        else if (game_state == PLAY && *p==1) {
            if ((mx >= btn_x+400 && mx <= btn_x+600) && (my >= btn_y && my <=btn_y+btn_height)) {
                int pdl_x = 150, pdl_y = 20, pdl_width = 100, pdl_height = 10;
                int screen_width= 1400, screen_height = 800;
                int ball_radius = 8, ball_x = 200 , ball_y = 37 ;
                dx=9,dy=9;
                initializeBricks();
                Gap_brick_1();
                level_state=LEVEL1;
                current_level=1;
                playThemeSong();
            }
            else if ((mx >= btn_x+400 && mx <=btn_x+600) && (my >=btn_y-70 && my <=btn_y+btn_height-70)) {
                int pdl_x = 150, pdl_y = 20, pdl_width = 100, pdl_height = 10;
                int screen_width= 1400, screen_height = 800;
                int ball_radius = 8, ball_x = 200 , ball_y = 37 ;
                dx=10,dy=10;
                initializeBricks();
                Gap_brick_2();
                level_state=LEVEL2;
                playThemeSong();
                current_level=2;
            }
            else if ((mx >= btn_x+400 && mx <= btn_x+600) && (my >=btn_y-140 && my <= btn_y+btn_height-140)) {
                int pdl_x = 150, pdl_y = 20, pdl_width = 100, pdl_height = 10;
                int screen_width= 1400, screen_height = 800;
                int ball_radius = 8, ball_x = 200 , ball_y = 37 ;
                dx=11,dy=11;
                initializeBricks();
                Gap_brick_3();
                level_state=LEVEL3;
                playThemeSong();
                current_level=3;
            }
			else if((mx >= back_btn_x && mx <= back_btn_x+back_btn_width) && (my >= back_btn_y && my <=back_btn_y+back_btn_height)){
                *p=0;
				game_state=MENU;
			}
            
        }
        else if(game_state == HIGH_SCORE){
            if(high_score_state==HIGH_SCORE1){
                if((mx >= back_btn_x && mx <= back_btn_x+back_btn_width) && (my >= back_btn_y && my <=back_btn_y+back_btn_height)){
				    game_state=MENU;
                    level_state=0;
			    }
                if((mx >= (back_btn_x+1200) && mx <= (back_btn_x+back_btn_width+1200)) && (my >= back_btn_y && my <=back_btn_y+back_btn_height)){
                    level_state=LEVEL2;
                    high_score_state=HIGH_SCORE2;
			    }

            }
            else if(high_score_state==HIGH_SCORE2){
                if((mx >= back_btn_x && mx <= back_btn_x+back_btn_width) && (my >= back_btn_y && my <=back_btn_y+back_btn_height)){
				    level_state=LEVEL1;
                    high_score_state=HIGH_SCORE1;
			    }
                if((mx >= (back_btn_x+1200) && mx <= (back_btn_x+back_btn_width+1200)) && (my >= back_btn_y && my <=back_btn_y+back_btn_height)){
                    level_state=LEVEL3;
                    high_score_state=HIGH_SCORE3;

			    }
            }
            else if(high_score_state==HIGH_SCORE3){
                if((mx >= back_btn_x && mx <= back_btn_x+back_btn_width) && (my >= back_btn_y && my <=back_btn_y+back_btn_height)){
				    level_state=LEVEL2;
                    high_score_state=HIGH_SCORE2;
			    }
            }
        }
		else if(game_state == INSTRUCTION){
			if((mx >= back_btn_x && mx <= back_btn_x+back_btn_width) && (my >= back_btn_y && my <=back_btn_y+back_btn_height)){
				game_state=MENU;
			}
		}
        else if(game_state == OUT){
            stopThemeSong();
            if((mx >= back_btn_x && mx <= back_btn_x+back_btn_width) && (my >= back_btn_y && my <=back_btn_y+back_btn_height)){
				game_state=MENU;
                exit(0);
			}
		}
        else if(game_state == WIN){
            stopThemeSong();
            if((mx >= back_btn_x && mx <= back_btn_x+back_btn_width) && (my >= back_btn_y && my <=back_btn_y+back_btn_height)){
				game_state=MENU;
                exit(0);
			}
		}
    }
}

bool areAllBricksBroken() {
    for (int i = 0; i < brick_rows; i++) {
        for (int j = 0; j < brick_cols; j++) {
            if (bricks[i][j] >0) {
                return false; // Found an unbroken brick
            }
        }
    }
    return true; // All bricks are broken
}

void ballChange() {
    ball_x += dx;
    ball_y += dy;
    // Wall collisions
    if (ball_x + ball_radius > screen_width || ball_x - ball_radius < 0) {
        dx = -dx;
    }
    if (ball_y + ball_radius > screen_height) {
        dy = -dy;
    }

    // pdl collision
    if (ball_y - ball_radius <= pdl_y + pdl_height &&
        ball_x >= pdl_x && ball_x <= pdl_x + pdl_width) {
        dy = -dy;
        ball_y = pdl_y + pdl_height + ball_radius;
    }

    // Brick collision
    for (int i = 0; i < brick_rows; i++) {
        for (int j = 0; j < brick_cols; j++) {
            if (bricks[i][j] >=0) { // Check brick visiblity
                int brick_x = brick_f_x + j * (brick_width + 3);
                int brick_y = brick_f_y + i * (brick_height + 3);
                if (ball_x + ball_radius > brick_x && ball_x -ball_radius < brick_x + brick_width &&
                    ball_y + ball_radius > brick_y &&
                    ball_y - ball_radius < brick_y + brick_height) {
                    int bottom = abs((ball_y + ball_radius) - brick_y);
                    int top = abs((ball_y - ball_radius) - (brick_y + brick_height));
                    int left = abs((ball_x + ball_radius) - brick_x);
                    int right = abs((ball_x - ball_radius) - (brick_x + brick_width));

                    int minn = min({top,bottom, left, right});
                    if (minn == top) {
                        if(dy<0) dy = -dy;
                        else dx=-dx;
                    }
                    else if (minn == bottom) {
                        if(dy>0)dy = -dy; 
                        else dx=-dx;
                    }
                    else if (minn == left) {
                        if(dx>0)dx = -dx;
                        else dy=-dy; 
                    }
                    else if (minn == right) {
                        if(dx<0)dx = -dx;
                        else dy=-dy; 
                    }
					if(bricks[i][j]==1){
						bricks[i][j] = 2; // making green
                        score++;
					}
                    else if(bricks[i][j]!=0){
                        bricks[i][j] = -1; //breaking bricks but not the gray ones
                        score++;
                    }
                    break;
                }
            }
        }
    }

    // Resetting ball position if it falls below the pdl
    if (ball_y - ball_radius < 0) {
        lives--; 
        if (lives > 0) {
            ball_x = pdl_x + pdl_width / 2;
            ball_y = pdl_y + pdl_height + ball_radius;
            game_start = 0;
            iPauseTimer(0);
        }
        if(lives==0) {
            // Game over logic
            iPauseTimer(0);
            stopThemeSong();
            addScore(show,score);
            game_state = OUT;
            level_state=-10;
        }
        if (areAllBricksBroken()) {
            // Game over logic
            iPauseTimer(0);
            stopThemeSong();
            addScore(show,score);
            game_state = WIN;
            level_state=-10;
        }
    }
}

void game_level(){
    iClear();
      //draw ball
    iSetColor(255, 104, 107);
    iFilledCircle(ball_x, ball_y, ball_radius);

    // Draw pdl
    iSetColor(255,255,255);
    iFilledRectangle(pdl_x, pdl_y, pdl_width, pdl_height);

    // Draw bricks
    for (int i = 0; i < brick_rows; i++) {
        for (int j = 0; j < brick_cols; j++) {
            if (bricks[i][j] >= 0) { // Draw only visible bricks
                int brick_x = brick_f_x + j * (brick_width + 3);
                int brick_y = brick_f_y + i * (brick_height + 3);
                if(bricks[i][j]==0)iShowBMP(brick_x,brick_y,"BMP\\gray.bmp");
				else if(bricks[i][j]==1)iShowBMP(brick_x,brick_y,"BMP\\blue.bmp");
                else iShowBMP(brick_x,brick_y,"BMP\\green.bmp");
            }
        }
    }
    iSetColor(255,255,255);
    char sc[20];
    sprintf(sc, "Score: %d", score);
    iText(10, screen_height-20, sc, GLUT_BITMAP_TIMES_ROMAN_24);
    // Display lives
    char lv[20];
    sprintf(lv, "Lives: %d", lives);
    iText(10, screen_height - 50, lv, GLUT_BITMAP_TIMES_ROMAN_24);
}
int j=1;

void iDraw() {
    if(game_state==HIGH_SCORE){
        if(high_score_state==HIGH_SCORE1){
            level_state=LEVEL1;
            loadScores();
            displayScore1();
        }
        else if(high_score_state==HIGH_SCORE2){
            level_state=LEVEL2;
            loadScores();
            displayScore2();
        }
        else if(high_score_state==HIGH_SCORE3){
            level_state=LEVEL3;
            loadScores();
            displayScore3();
        }
        
    }
    if(game_state==PLAYER_NAME){
        displayName();
    }
	if (game_state == MENU) {
        Menu();
        
    } 
    if (game_state == PLAY && *p==1 && *e==1 ) { 
        loadGameState();
        game_level();  
         
    }
    if (game_state == PLAY && *p==0 ) {
        resume();      
    }
    if (game_state == PLAY && *p==1 ) {
        level();      
    }
    
    if(level_state==LEVEL1 &&game_state == PLAY){
        bricks[6][0]=-1;
        game_level();
    }
    if(level_state==LEVEL2 && game_state == PLAY){
        
        game_level();
    }
    if(level_state==LEVEL3 & game_state == PLAY){
        bricks[2][0]=0,bricks[0][0]=0,bricks[4][0]=0,bricks[6][0]=0;
        bricks[1][11]=0,bricks[3][11]=0,bricks[5][11]=0,bricks[7][11]=0;
        for(int i = 1; i < brick_rows-1;i++ ){
            bricks[i][1]=-1;
            bricks[i][10]=-1;
        }
        for(int i = 1; i < brick_cols-1;i++ ){
            bricks[1][i]=-1;
            bricks[6][i]=-1;
        }
        for(int j=2;j<10;){
            bricks[4][j]=0;
            j+=2;
        }
        for(int j=3;j<10;){
            bricks[3][j]=0;
            j+=2;
        }

        game_level();
    }        
    
    if(game_state==INSTRUCTION){
        instructions();
    }
    if(game_state==OUT){
        out();
    }
    if(game_state==WIN){
        win();
    }
}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
	*/
void iMouseMove(int mx, int my) {
	
	//place your codes here
}


void iKeyboard(unsigned char key) {
    if(game_state==PLAYER_NAME){
        if(key == '\b'){
            if (nameLength > 0) {
                nameLength--;
                playerName[nameLength] = '\0';
            }
        }
        else if(key != '\b'){
            playerName[nameLength]=key;
            playerName[nameLength+1]='\0';
            nameLength++;
        }
        if (key == '\r'){
            game_state=MENU;
        }
    }
    

    if (key == ' ' && game_state == PLAY) {
        if (game_start == 0) {
                game_start = 1; // Start the game
                iResumeTimer(0);
                *e=0;
            }
        else {
                game_start = 0; // Pause the game
                iPauseTimer(0);
            }
    }
    else if (key == 'q' && game_state!=PLAYER_NAME) {
        iPauseTimer(0);
        saveGameState();
        stopThemeSong();
        exit(0);
    }
    else if(key == 'm'&& game_state!=PLAYER_NAME){
        iPauseTimer(0);
        stopThemeSong();
        game_start = 0;
        ball_x = pdl_x + pdl_width / 2;
        ball_y = pdl_y + pdl_height + ball_radius;
        lives=3;
        score=0;
        game_state = MENU;
        level_state=-10;
    }
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
	*/
void iSpecialKeyboard(unsigned char key) {
    if (key == GLUT_KEY_LEFT && pdl_x > 0 && game_state == PLAY) {
        pdl_x -= 40;
        if (pdl_x < 0) pdl_x = 0;
        if(game_start==0){
            ball_x = pdl_x+ pdl_width/2 ;
        }
    }
    if (key == GLUT_KEY_RIGHT && pdl_x + pdl_width < screen_width && game_state == PLAY) {
        pdl_x += 40;
        if (pdl_x + pdl_width > screen_width) {
            pdl_x = screen_width - pdl_width;
        }
        if(game_start==0){
            ball_x = pdl_x+ pdl_width/2 ;
        }
    }
    if (key == GLUT_KEY_END) {
        exit(0);
    }
}


int main() {
    initializeBricks(); // Initialize bricks
    iSetTimer(8, ballChange); // Timer for ball movement
    iPauseTimer(0);
    iInitialize(1400, 800, "DX-BALL");
    
    return 0;
}