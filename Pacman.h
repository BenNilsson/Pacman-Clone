#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif


// Just need to include main header file
#include "S2D/S2D.h"
#include "Tile.h"
#include "Food.h"
#include "Ghost.h"
#include "Grid.h"
#include "Node.h"
#include "Pathfinding.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Player Struct Definition
struct Player {
	// Data to represent Pacman
	Vector2* position;
	Vector2 startPosition;
	Rect* sourceRect;
	Texture2D* texture;
	int direction;
	int frame;
	int currentFrameTime;
	float speedMultiplier;
	int lives;
	bool hurt;
	bool isDead;
};

// Munchie Struct Definition
struct Munchie {
	Rect* rect;
	Texture2D* texture;
	Vector2* position;
	int frameCount;
	int currentFrameTime;
	int frameTime;
};

struct Menu {
	Texture2D* background;
	Rect* rect;
	Vector2* stringPosition;
	bool isKeyDown;
	bool drawMenu;
};


// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:

	// Data to represent Player
	Player* _pacman;
	const int _cPacmanFrameTime;

	// Data to represent menu
	Menu* _pauseMenu;
	Menu* _startMenu;

	// Data to represent creating the level
	vector<Tile> _tiles;
	vector<Food> _munchiesVector;
	vector<Ghost> _ghosts;
	vector<Food> _cherries;

	// UI
	vector<Texture2D> _livesUI;

	int _width;
	int _height;
	Grid* _grid;
	Pathfinding* _pf;
	Texture2D* test;
	Texture2D* test2;
	Texture2D* test3;
	bool _hasLoaded = false;

	// Data for testing stuff
	bool keyDown;

	// Sounds
	SoundEffect* _pop;
	SoundEffect* _intro;
	bool _introHasPlayed = false;

	// Score
	int _curScore;
	Vector2* _scorePosition;

	// Constant data for Game Variables
	const float _cPacmanSpeed;

	// Data for the game's current state
	bool _paused;
	bool _gameStarted;
	bool _drawLevel = false;

	// Define Functions
	bool CheckBoxCollision(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2);
	void CheckGameStarted(Input::KeyboardState* state, Input::Keys startKey);
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckPacmanViewportCollision();
	void GenerateLevel();
	void PacmanInputMovement(Input::KeyboardState* state);
	void MovePacman(int elapsedTime);
	void AnimateMunchieSprite(int elapsedTime);
	void AnimatePacmanSprite(int elapsedTime);
	void MoveGhosts(int elapsedTime);
	void CheckCherryCollisions();
	void CheckGhostCollisions();
	void CheckMunchieCollisions();
	void CheckWin();
	void SetupAStart(int width, int height);
	void RestartLevel();
	void ResetLevel();

	Tile LoadMunchieTile(int x, int y);
	Tile LoadCherryTile(int x, int y);
	Tile LoadPlayerStartTile(int x, int y);
	Tile LoadEnemyTile(int x, int y);

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};