#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Munice count
#define MUNCHIECOUNT 50

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Player Struct Definition
struct Player {
	// Data to represent Pacman
	Vector2* position;
	Rect* sourceRect;
	Texture2D* texture;
	int direction;
	int frame;
	int currentFrameTime;
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

	// Data to represent Munchie
	Munchie* _munchies[MUNCHIECOUNT];

	// Data to represent menu
	Menu* _menu;

	// Position for String
	Vector2* _stringPosition;

	// Constant data for Game Variables
	const float _cPacmanSpeed;

	// Data for the game's current state
	bool _paused;
	bool _gameStarted;

	// Define Functions
	void CheckGameStarted(Input::KeyboardState* state, Input::Keys startKey);
	void CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey);
	void CheckViewportCollision();
	void Input(int elapsedTime, Input::KeyboardState* state);
	void UpdateMunchie(int elapsedTime);
	void UpdatePacman(int elapsedTime);

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