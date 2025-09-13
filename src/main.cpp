#include "raylib.h"
#include <vector>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <cmath>

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

int score;
float vx = 2.0;
float vy = -4.0;

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


int getOverlap(int inputArray[]) {
	if (inputArray[0] == inputArray[1]) {
		return 2;
	}
	int r = 0;
	for (int i = 0; i < sizeof(inputArray) / sizeof(inputArray[0]); i++) {
		std::cout << "ARRAY: " << inputArray[i] << std::endl;
		if (inputArray[i] < inputArray[r]) {
			r = i;
		}
	}
	return r;
}


std::vector<float> getSpeeds(float playerX, float playerWidth, float ballX, float ballWidth) {
	std::vector<float> r;
	float hitPos = ( (playerX + (playerWidth / 2)) - (ballX + (ballWidth / 2) / (playerWidth / 2)));
	float angle = hitPos * 60.0f * (M_PI / 180.0f);
	r.push_back(5 * sin(angle));
	r.push_back(-5 * cos(angle));
	return r;
}

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
	ball.x = screenWidth / 2 - ball.texture.width / 2;
	ball.y = (playerBlock.y - ball.texture.height) - 1;
	ball.collisionRect = Rectangle({static_cast<float>(ball.x), static_cast<float>(ball.y), 
									static_cast<float>(ball.texture.width), static_cast<float>(ball.texture.height)});
	ball.shown = true;

	// Main game loop
    while (!WindowShouldClose())
    {
		ball.y += vy;
		ball.collisionRect.y = ball.y;
		ball.x += vx;
		ball.collisionRect.x = ball.x;
		
		//Player movement
		if (IsKeyDown(KEY_RIGHT)) {
			if (playerBlock.x < screenWidth - playerBlock.texture.width) {
				playerBlock.x += 10;
				playerBlock.collisionRect.x = playerBlock.x;
			}
		}

		if (IsKeyDown(KEY_LEFT)) {
			if (playerBlock.x > 0) {
				playerBlock.x -= 10;
				playerBlock.collisionRect.x = playerBlock.x;
			}
		}

		if (CheckCollisionRecs(ball.collisionRect, playerBlock.collisionRect)) {
			int collisionArray[] = {
							(ball.x + ball.texture.width) - playerBlock.x,	
							(playerBlock.x + playerBlock.texture.width) - ball.x
			};

			int impactOverlap = getOverlap(collisionArray);
			
			if (impactOverlap == 0 && collisionArray[impactOverlap] <= 50) {
				std::vector<float> speeds = getSpeeds(playerBlock.x, playerBlock.texture.width, ball.x, ball.texture.width);
				std::cout << "X: " << speeds[0] << " Y: " << speeds[1] << std::endl;
				vx += speeds[0];
				vy += speeds[1];
				std::cout << "VX: " << vx << " " << "VY: " << vy << std::endl; 
			} else if (impactOverlap == 1 && collisionArray[impactOverlap] > 50) {
				std::vector<float> speeds = getSpeeds(playerBlock.x, playerBlock.texture.width, ball.x, ball.texture.width);
				vx -= speeds[0];
				vy -= speeds[1];
			} else if (impactOverlap == 2) {
				// Do nothing i guess lol
			}
			if (impactOverlap == 0 && vx > 0) {
				vx = -vx;
			} else if (impactOverlap == 1 && vx < 0) {
				vx = -vx;
			}
			vy = -vy;
		}

		if (ball.x >= screenWidth - ball.texture.width || ball.x <= 0) {
			vx = -vx;
		}

		// Draw to screen
        BeginDrawing();
            ClearBackground(BLACK);
			
			// Should this for loop be outside of the draw loop, and i just make a new table or something of blocks that actually get drawn?
			// I suppose either way it gets iterated over maybe way too often but it seems cleaner to be outside of the draw loop
			for (int i = 0; i < blockTable.size(); i++) {
				// Draw the blocks that haven't been destroyed
				if (blockTable[i].shown) {
					DrawTexture(blockTable[i].texture, blockTable[i].x, blockTable[i].y, RAYWHITE);
				if (CheckCollisionRecs(ball.collisionRect, blockTable[i].collisionRect) && blockTable[i].shown) {
					blockTable[i].shown = false;
					vy = -vy;
					score++;
					// This break is here because before if two blocks were hit at the same time, the ball would continue upward.
					// However, this breaks intended behavior of hitting two blocks at the same y level
					break;
				}
			}
		}

			if (ball.y >= screenHeight) {
				std::cout << "Game over" << std::endl;
				break;
			}

			DrawTexture(playerBlock.texture, playerBlock.x, playerBlock.y, RAYWHITE);
			DrawTexture(ball.texture, ball.x, ball.y, RAYWHITE);
        	// DrawText("Score: ", screenWidth - 200, screenHeight - 40, 36, RED);
		
		EndDrawing();
    }
    CloseWindow();
    return 0;
}
