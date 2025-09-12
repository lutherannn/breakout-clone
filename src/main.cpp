#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <time.h>
#include <iostream>

// Define variables for later
const int screenWidth = 1280;
const int screenHeight = 600;
const char windowTitle[9] = "Breakout";
const int targetFPS = 60;
const int blockWidth = 80;
const int blockHeight = 20;
const int rows = 5;
const int cols = screenWidth / blockWidth;
const int totalBlocks = rows * cols;
const int ballSpeed = 30;

int frames;
int score;
bool upwardBallMovement = true;
// bool rightBallMovement = true;
bool ballBoxCollision = false;
bool paddleCollision = false;

/* 
 * Block struct
 * x: x pos
 * y: y pos
 * texture: texture for block
 * shown: drawn or not (used for later when ball destroys the blocks)
 * collisionRect: Rectangle used for collision purposes
*/
struct Block {
	int x;
	int y;
	Texture2D texture;
	bool shown;
	Rectangle collisionRect;
};


/*
 * populateBlockTable
 * Accepts a vector of Texture2D elements
 * Returns a vector of blocks for drawing to the screen later
*/
std::vector<Block> populateBlockTable(std::vector<Texture2D> blockTextureTable) {
	std::vector<Block> bt;
	for (int i = 0; i < totalBlocks; i++) {
		int col = i % cols;
		int row = i / cols;
		Block tb;
		tb.x = col * blockWidth;
		tb.y = row * blockHeight;
		tb.texture = blockTextureTable[row];
		tb.shown = true;
		tb.collisionRect = Rectangle({static_cast< float >(tb.x), static_cast< float >(tb.y), 
		   							  static_cast< float >(tb.texture.width), static_cast< float >(tb.texture.height)});
		bt.push_back(tb);
	}
	return bt;
}

int main(void) {
	// Seed the generator for random numbers (used for initial ball trajectory)
	srand(time(NULL));
	InitWindow(screenWidth, screenHeight, windowTitle);	
	SetTargetFPS(targetFPS);

	// Load block images and textures
	Image blueBlockImage = LoadImage("assets/blueblock.png");
	Texture2D blueBlockTexture = LoadTextureFromImage(blueBlockImage);
	Image greenBlockImage = LoadImage("assets/greenblock.png");
	Texture2D greenBlockTexture = LoadTextureFromImage(greenBlockImage);
	Image orangeBlockImage = LoadImage("assets/orangeblock.png");
	Texture2D orangeBlockTexture = LoadTextureFromImage(orangeBlockImage);
	Image pinkBlockImage = LoadImage("assets/pinkblock.png");
	Texture2D pinkBlockTexture = LoadTextureFromImage(pinkBlockImage);
	Image yellowBlockImage = LoadImage("assets/yellowblock.png");
	Texture2D yellowBlockTexture = LoadTextureFromImage(yellowBlockImage);
	std::vector<Texture2D> blockTextures = {blueBlockTexture, greenBlockTexture, orangeBlockTexture, pinkBlockTexture, yellowBlockTexture};
	std::vector<Block> blockTable = populateBlockTable(blockTextures);
	
	// Load player image and texture, create the player's block struct
	Image playerImage = LoadImage("assets/player.png");
	Texture2D playerTexture = LoadTextureFromImage(playerImage);
	Block playerBlock;
	playerBlock.texture = playerTexture;
	playerBlock.x = screenWidth / 2 - playerBlock.texture.width / 2;
	playerBlock.y = screenHeight - playerBlock.texture.height;
	playerBlock.collisionRect = Rectangle({static_cast<float>(playerBlock.x), static_cast<float>(playerBlock.y), 
										   static_cast<float>(playerBlock.texture.width), static_cast<float>(playerBlock.texture.height)});
	playerBlock.shown = true;

	// Load ball image and texture, create the ball's block struct
	Image ballImage = LoadImage("assets/ball.png");
	Texture2D ballTexture = LoadTextureFromImage(ballImage);
	Block ball;
	ball.texture = ballTexture;
	ball.x = rand() % screenWidth - ball.texture.width;
	ball.y = (playerBlock.y - ball.texture.height) - 1;
	ball.collisionRect = Rectangle({static_cast<float>(ball.x), static_cast<float>(ball.y), 
									static_cast<float>(ball.texture.width), static_cast<float>(ball.texture.height)});
	ball.shown = true;
	
	std::cout << playerBlock.x << ", " << playerBlock.y << std::endl;
	std::cout << playerBlock.collisionRect.x << ", " << playerBlock.collisionRect.y << std::endl;

	// Main game loop
    while (!WindowShouldClose())
    {
		//Update ball's position every second, i think?
		int delta = GetFrameTime();
		frames++;
		if (frames % (targetFPS / 4) == 0) {
			if (upwardBallMovement) {
				ball.y -= ballSpeed;
				ball.collisionRect.y = ball.y;
			} else {
				ball.y += ballSpeed;
				ball.collisionRect.y = ball.y;
			}
			frames = 0;
		}

		//Player movement
		if (IsKeyDown(KEY_RIGHT)) {
			if (playerBlock.x < screenWidth - playerBlock.texture.width) {
				playerBlock.x += 5;
				playerBlock.collisionRect.x = playerBlock.x;
			}
		}

		if (IsKeyDown(KEY_LEFT)) {
			if (playerBlock.x > 0) {
				playerBlock.x -= 5;
				playerBlock.collisionRect.x = playerBlock.x;
			}
		}

		// There is considerable lag when the ball hits the paddle, i think? Managable for now.
		if (CheckCollisionRecs(ball.collisionRect, playerBlock.collisionRect)) {
			ball.y = (playerBlock.y - ball.texture.height) - 1;
			upwardBallMovement = true;
		}

		// Draw to screen
        BeginDrawing();
            ClearBackground(BLACK);
			
			for (int i = 0; i < blockTable.size(); i++) {
				// Draw the blocks that haven't been destroyed
				if (blockTable[i].shown) {
					DrawTexture(blockTable[i].texture, blockTable[i].x, blockTable[i].y, RAYWHITE);
				if (CheckCollisionRecs(ball.collisionRect, blockTable[i].collisionRect) && blockTable[i].shown) {
					blockTable[i].shown = false;
					upwardBallMovement = false;
					score++;
				}
			}
		}

		if (ball.y >= screenHeight) {
			std::cout << "Game over" << std::endl;
			CloseWindow();
		}

		if (ball.y <= ball.texture.height) {
			upwardBallMovement = false;
		}

		DrawTexture(playerBlock.texture, playerBlock.x, playerBlock.y, RAYWHITE);
		DrawTexture(ball.texture, ball.x, ball.y, RAYWHITE);
        DrawText("Score: ", screenWidth - 200, screenHeight - 40, 36, RED);
		EndDrawing();
    }
    CloseWindow();
    return 0;
}
