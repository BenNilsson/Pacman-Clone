#include "Pacman.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include "Grid.h"
#include "Pathfinding.h"
#include "GameState.h"

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250)
{
	// Pacman
	_pacman = new Player();
	_pacman->isDead = false;
	_pacman->direction = 0;
	_pacman->currentFrameTime = 0;
	_pacman->frame = 0;
	_pacman->speedMultiplier = 1.0f;


	_cherry = new Munchie();

	_grid = new Grid();

	// Sounds
	_pop = new SoundEffect();
	_intro = new SoundEffect();

	// Menu
	_pauseMenu = new Menu();
	_startMenu = new Menu();

	// Initiase Game Stats
	_gameStarted = false;
	_paused = false;

	//Initialise important Game aspects
	Audio::Initialise();
	S2D::Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 144);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	S2D::Graphics::StartGameLoop();
}

Pacman::~Pacman()
{

	// Clean up pointers within the Pacman structure
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _pacman->position;

	// Clean up the Pacman structure pointer
	delete _pacman;

	delete _cherry->texture;
	delete _cherry;

	// Delete sounds
	delete _pop;
	delete _intro;

	delete test;
	delete test2;
	delete test3;

	
	// Clean up A*
	for (Node n : *_grid->path) {
		delete n.parent;
	}
	delete _grid->path;
	delete _grid;
	delete _pf;
	

	// Clean up menu
	delete _pauseMenu;
	delete _startMenu;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_pacman->position = new Vector2(0, 0);

	_cherry->texture = new Texture2D();
	_cherry->texture->Load("Textures/Cherry.png", false);
	_cherry->rect = new Rect(0.0f, 0.0f, 32, 32);
	_cherry->position = new Vector2(100, 100);

	test = new Texture2D();
	test2 = new Texture2D();
	test3 = new Texture2D();
	test->Load("Textures/walkable.png", false);
	test2->Load("Textures/not_walkable.png", false);
	test3->Load("Textures/path.png", false);

	// Set string position
	_curScore = 0;
	_scorePosition = new Vector2(10.0f, 25.0f);

	// Generate levels
	_pf = new Pathfinding();
	GenerateLevel();

	// Set Menu Paramters
	_pauseMenu->background = new Texture2D();
	_pauseMenu->background->Load("Textures/Transparency.png", false);
	_pauseMenu->rect = new Rect(0.0f, 0.0f, _width, _height);
	_pauseMenu->stringPosition = new Vector2(_width / 2.0f, _height / 2.0f);

	// Set Menu Paramters
	_startMenu->background = new Texture2D();
	_startMenu->background->Load("Textures/Transparency.png", false);
	_startMenu->rect = new Rect(0.0f, 0.0f, _width, _height);
	_startMenu->stringPosition = new Vector2(_width / 2.0f, _height / 2.0f);
	_startMenu->drawMenu = true;

	// Load Sounds
	_pop->Load("Sounds/Chomp.wav");
	_intro->Load("Sounds/Intro.wav");

	// Check if the audio is initialised
	if (!Audio::IsInitialised())
		std::cout << "Audio is not initialised" << std::endl;

	// Checks if sounds aren't loaded
	if (!_pop->IsLoaded()) 
		std::cout << "_pop member sound effect has not loaded" << std::endl;
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	// Check path
	_pf->FindPath(*_grid,*_pacman->position, _ghosts[0].Position);

	switch (GameState::GetState())
	{
	case State::START:
		// Check if game is started
		CheckGameStarted(keyboardState, Input::Keys::SPACE);
		break;
	case State::INTRO:

		break;
	case State::PLAYING:
		if (_pacman->isDead) GameState::SetState(State::GAMEOVER);
		CheckPaused(keyboardState, Input::Keys::P);
		PacmanInputMovement(keyboardState);
		MovePacman(elapsedTime);
		CheckPacmanViewportCollision();
		AnimatePacmanSprite(elapsedTime);
		AnimateMunchieSprite(elapsedTime);
		MoveGhosts(elapsedTime);
		CheckCherryCollisions();
		CheckMunchieCollisions();
		CheckGhostCollisions();
		break;
	case State::PAUSED:
		CheckPaused(keyboardState, Input::Keys::P);
		break;
	case State::GAMEOVER:
		break;

	}

	// Close Game
	if (keyboardState->IsKeyDown(Input::Keys::ESCAPE))
		exit(0);
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Score: " << _curScore;

	// Start Drawing
	SpriteBatch::BeginDraw();
	
	
	/* A* debug stuff
	// Draw all nodes
	if (_hasLoaded)
	{
		if (_grid != nullptr)
		{
			for (int x = 0; x < _grid->gridSizeX; x++)
			{
				for (int y = 0; y < _grid->gridSizeY; y++)
				{
					try
					{
						Node n = _grid->grid.at(x).at(y);

						if (n.walkable)
						{
							SpriteBatch::Draw(test, &n.position, &Rect(0, 0, 32, 32));
							if (_grid->path != nullptr)
							{
								if (find(_grid->path->begin(), _grid->path->end(), n) != _grid->path->end())
								{
									SpriteBatch::Draw(test3, &n.position, &Rect(0, 0, 32, 32));
								}
							}
						}
						else
						{
							SpriteBatch::Draw(test2, &n.position, &Rect(0, 0, 32, 32));
						}
					}
					catch (const std::out_of_range & excepOOR)
					{

					}
				}
			}
		}
	}
	*/
	

	
	if (_drawLevel)
	{
		// Draw Tiles
		for (const Tile& tile : _tiles)
		{
			const Texture2D* texture = tile.GetTexture();
			if (texture != nullptr)
				SpriteBatch::Draw(texture, &tile.GetPosition());
		}

		// Draw munchies
		for (const Food& food : _munchiesVector)
		{
			SpriteBatch::Draw(food.GetTexture(), &food.Position, &food.Rect);
		}

		// Draw ghosts
		for (const Ghost& ghost : _ghosts)
		{
			SpriteBatch::Draw(ghost.GetTexture(), &ghost.Position, &ghost.Rect);
		}

		// Draw Cherry
		SpriteBatch::Draw(_cherry->texture, _cherry->position, _cherry->rect);

		// Draw Player
		if (!_pacman->isDead)
		{
			SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect);
		}

		// Draws String
		SpriteBatch::DrawString(stream.str().c_str(), _scorePosition, Color::White);
	}


	// Draw Start Menu
	if (_startMenu->drawMenu) {
		std::stringstream menuStream;
		string s = "Pacman";
		menuStream.str(s);
		_startMenu->stringPosition = new Vector2((_width / 2.0f) - (s.length() * 5.0f), _height / 2.0f);
		SpriteBatch::Draw(_startMenu->background, _startMenu->rect, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _startMenu->stringPosition, Color::Yellow);
		s = "Press Space To Start";
		menuStream.str(s);
		SpriteBatch::DrawString(menuStream.str().c_str(), new Vector2((_width / 2.0f) - (s.length() * 4.0f), _startMenu->stringPosition->Y + 30.0f), Color::White);
	}

	// Draw Pause Menu
	if (_pauseMenu->drawMenu) {
		std::stringstream menuStream;
		string s = "Game Paused";
		menuStream.str(s);
		_pauseMenu->stringPosition = new Vector2((_width / 2.0f) - (s.length() * 5.0f), _height / 2.0f);
		SpriteBatch::Draw(_pauseMenu->background, _pauseMenu->rect, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _pauseMenu->stringPosition, Color::White);
	}
	

	// End Drawing
	
	SpriteBatch::EndDraw();

}

bool Pacman::CheckBoxCollision(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2)
{
	float left1 = x1;
	float left2 = x2;
	float right1 = x1 + width1;
	float right2 = x2 + width2;
	float top1 = y1;
	float top2 = y2;
	float bottom1 = y1 + height1;
	float bottom2 = y2 + height2;

	if (bottom1 < top2) return false;
	if (top1 > bottom2) return false;
	if (right1 < left2) return false;
	if (left1 > right2) return false;

	return true;
}

void Pacman::CheckGameStarted(Input::KeyboardState* state, Input::Keys startKey)
{
	if (_intro->GetState() == SoundEffectState::PLAYING && _gameStarted == false)
	{
		_introHasPlayed = true;
	}

	if (_introHasPlayed)
	{
		if (_intro->GetState() == SoundEffectState::STOPPED)
		{
			// Set Game State to PLAYING
			GameState::SetState(State::PLAYING);
			_gameStarted = true;
			if (state->IsKeyUp(startKey))
				_startMenu->isKeyDown = false;
		}
	}

	// Checks if Space is pressed and game isn't started
	if (state->IsKeyDown(startKey) && !_gameStarted && !_startMenu->isKeyDown) 
	{
		_startMenu->isKeyDown = true;
		_startMenu->drawMenu = false;
		_drawLevel = true;
		Audio::Play(_intro);
	}

	
}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	// Checks if we are in the game (ignores whether the game is paused or not)
	if (GameState::GetState() == State::PLAYING || GameState::GetState() == State::PAUSED) {

		// Checks if P key is pressed
		if (state->IsKeyDown(pauseKey) && !_pauseMenu->isKeyDown)
		{
			// Set Game State accordingly
			switch (GameState::GetState())
			{
			case State::PLAYING:
				GameState::SetState(State::PAUSED);
				_paused = true;
				_pauseMenu->drawMenu = true;
				break;
			case State::PAUSED:
				_paused = false;
				_pauseMenu->drawMenu = false;
				GameState::SetState(State::PLAYING);
				break;
			}
				

			_pauseMenu->isKeyDown = true;
		}

		if (state->IsKeyUp(pauseKey))
			_pauseMenu->isKeyDown = false;
	}
}

void Pacman::CheckPacmanViewportCollision()
{
	// Checks if Pacman is off the right side of the screen
	if (_pacman->position->X + (float)_pacman->sourceRect->Width > (float)_width)
		_pacman->position->X = (0.0f - 32.0f) + (float)_pacman->sourceRect->Width;

	// Checks if Pacman is off the left side of the screen
	if (_pacman->position->X - (float)_pacman->sourceRect->Width < (0.0f - 32.0f))
		_pacman->position->X = (float)_width - _pacman->sourceRect->Width;

	// Checks if Pacman is off the top side of the screen
	if (_pacman->position->Y - (float)_pacman->sourceRect->Height < (0.0f - 32.0f))
		_pacman->position->Y = (float)_height - (float)_pacman->sourceRect->Height;

	// Checks if Pacman is off the down side of the screen
	if (_pacman->position->Y + (float)_pacman->sourceRect->Height > (float)_height)
		_pacman->position->Y = (0.0f - 32.0f) + (float)_pacman->sourceRect->Height;
}

void Pacman::GenerateLevel()
{
	// Load pixels from SOIL to get channels.
	// Unsure if Texture2D stores channels anywhere, can't seem to find it so I am re-loading the image
	int width, height, channels;
	unsigned char* myImage = SOIL_load_image
	(
		"Textures/map.png",
		&width, &height, &channels,
		SOIL_LOAD_AUTO
	);

	// Cache width and height for later use
	_width = width * 32;
	_height = height * 32;

	// Generate dynamic array for the tiles
	_tiles = vector<Tile>();

	// Generate dynamic array for munchies
	_munchiesVector = vector<Food>();

	// Reserve the size to avoid the array constantly upping its size
	_tiles.reserve(width * height);

	// Loop through the image
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			int pixelIndex = x * channels + y * width * channels;

			unsigned char red = myImage[pixelIndex];
			unsigned char green = myImage[pixelIndex + 1];
			unsigned char blue = myImage[pixelIndex + 2];
			unsigned char alpha = 255;

			if (channels == 4)
				alpha = myImage[pixelIndex + 3];

			if (red == 0 && green == 0 && blue == 0 && alpha == 255)
			{
				// Tile is black
				Texture2D* wall = new Texture2D();
				wall->Load("Textures/wall.png", false);
				_tiles.push_back(Tile(x, y, wall, CollissionType::TILE_NOTWALKABLE));
			}

			// Pacman
			if (red == 0 && green == 255 && blue == 0 && alpha == 255)
			{
				_tiles.push_back(LoadPlayerStartTile(x, y));
			}

			// Munchie
			if (red == 200 && green == 200 && blue == 40 && alpha == 255)
			{
				_tiles.push_back(LoadMunchieTile(x, y));
			}

			// Ghosts
			if (red == 255 && green == 0 && blue == 0 && alpha == 255)
			{
				_tiles.push_back(LoadEnemyTile(x, y));
			}

			// Transparent
			if (alpha == 0)
			{
				_tiles.push_back(Tile(x, y, nullptr, CollissionType::TILE_TRANSPARENT));
			}			
		}

		SetupAStart(width, height);
	}

	// Free up ram
	SOIL_free_image_data(myImage);
}

void Pacman::PacmanInputMovement(Input::KeyboardState* state)
{
	// Checks if D key is pressed
	if (state->IsKeyDown(Input::Keys::D) || state->IsKeyDown(Input::Keys::RIGHT)) {
		_pacman->direction = 0;
	}

	// Checks if A key is pressed
	else if (state->IsKeyDown(Input::Keys::A) || state->IsKeyDown(Input::Keys::LEFT)) {
		_pacman->direction = 2;
	}
	// Checks if W key is pressed
	else if (state->IsKeyDown(Input::Keys::W) || state->IsKeyDown(Input::Keys::UP)) {
		_pacman->direction = 3;
	}

	// Checks if S key is pressed
	else if (state->IsKeyDown(Input::Keys::S) || state->IsKeyDown(Input::Keys::DOWN)) {
		_pacman->direction = 1;
	}
}

void Pacman::MovePacman(int elapsedTime)
{
	float pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;

	// Set new momvement
	float newMoveX = _pacman->position->X;
	float newMoveY = _pacman->position->Y;

	switch (_pacman->direction)
	{
	case 0:
		newMoveX = _pacman->position->X + pacmanSpeed;
		break;
	case 1:
		newMoveY = _pacman->position->Y + pacmanSpeed;
		break;
	case 2:
		newMoveX = _pacman->position->X - pacmanSpeed;
		break;
	case 3:
		newMoveY = _pacman->position->Y - pacmanSpeed;
		break;
	}

	// Initialise movement boolean
	bool canMove = false;

	// Loop through all tiles
	for (const Tile& tile : _tiles)
	{
		// Check if tile can collide
		if (tile.Type == CollissionType::TILE_NOTWALKABLE)
		{
			// Check if the tile is colliding with Pacman
			// Due to tight space, temporaily somewhat of a fix is to check a smaller value than pacman's width and heigh
			// Basically, treat pacman as he is on a diet (or should have been at least).
			// Real fix it to move pacman to the wall when colliding so that we don't run into issues where he gets stuck
			if (CheckBoxCollision(newMoveX, newMoveY, _pacman->sourceRect->Width - 4, _pacman->sourceRect->Height - 4,
				tile.GetX(), tile.GetY(), tile.Width, tile.Height))
			{
				// Pacman has collided with wall, set move to false and break out of the loop
				canMove = false;
				break;
			}
			else canMove = true;
		}
		else
		{
			// Pacman didn't collide with a wall, set move to true
			canMove = true;
		}
	}

	// Check if pacman can move
	if (canMove)
	{
		// Move pacman in the direction based on direction
		switch (_pacman->direction)
		{
		case 0:
			_pacman->position->X += pacmanSpeed;
			break;
		case 1:
			_pacman->position->Y += pacmanSpeed;
			break;
		case 2:
			_pacman->position->X -= pacmanSpeed;
			break;
		case 3:
			_pacman->position->Y -= pacmanSpeed;
			break;
		}
	}
}

void Pacman::AnimateMunchieSprite(int elapsedTime)
{
	for (Food& food : _munchiesVector)
	{
		food.CurrentFrameTime += elapsedTime;

		if (food.CurrentFrameTime > food.GetFrameTime())
		{
			food.FrameCount++;

			if (food.FrameCount >= 2)
				food.FrameCount = 0;

			food.CurrentFrameTime = 0;
		}

		food.Rect.X = (float)food.Rect.Width * (float)food.FrameCount;
	}
}

void Pacman::AnimatePacmanSprite(int elapsedTime)
{
	// Update Pacman frame time
	_pacman->currentFrameTime += elapsedTime;

	if (_pacman->currentFrameTime > _cPacmanFrameTime) {
		_pacman->frame++;

		if (_pacman->frame >= 2)
			_pacman->frame = 0;

		_pacman->currentFrameTime = 0;
	}

	_pacman->sourceRect->X = (float)_pacman->sourceRect->Width * (float)_pacman->frame;


	// Set the Pacman source rect to match with the row of the spritesheet which pacman needs
	_pacman->sourceRect->Y = (float)_pacman->sourceRect->Height * (float)_pacman->direction;
}

void Pacman::MoveGhosts(int elapsedTime)
{
	for (Ghost& ghost : _ghosts)
	{
		if (ghost.Direction == 0)
		{
			// Moves Right
			ghost.Position.X += ghost.Speed * elapsedTime;
		}
		else if (ghost.Direction == 1)
		{
			// Moves Left
			ghost.Position.X -= ghost.Speed * elapsedTime;
		}

		if (ghost.Position.X + ghost.Rect.Width >= _width)
		{
			ghost.Direction = 1;
		}
		else if (ghost.Position.X <= 0)
		{
			ghost.Direction = 0;
		}
	}
}

void Pacman::CheckCherryCollisions()
{
	// Cherry collision
	if (CheckBoxCollision(
		_pacman->position->X, _pacman->position->Y, (float)_pacman->sourceRect->Width, (float)_pacman->sourceRect->Width,
		_cherry->position->X, _cherry->position->Y, (float)_cherry->rect->Width, (float)_cherry->rect->Width))
	{
		// Collision detected

		// Update score
		_curScore += 200;
		// Move Cherry out of the screen bounds
		_cherry->position = new Vector2(-100.0f, -100.0f);
	}
}

void Pacman::CheckGhostCollisions()
{
	// Local Variables
	float i = 0;
	float bottom1 = _pacman->position->Y + (float)_pacman->sourceRect->Height;
	float bottom2 = 0;
	float left1 = _pacman->position->X;
	float left2 = 0;
	float right1 = _pacman->position->X + (float)_pacman->sourceRect->Width;
	float right2 = 0;
	float top1 = _pacman->position->Y;
	float top2 = 0;

	for (Ghost& ghost : _ghosts)
	{
		// Populate variables with ghost data
		bottom2 = ghost.Position.Y + (float)ghost.Rect.Height;
		left2 = ghost.Position.X;
		right2 = ghost.Position.X + (float)ghost.Rect.Width;
		top2 = ghost.Position.Y;

		// Check for collision
		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			// Collision detected
			_pacman->isDead = true;
		}
	}
}

void Pacman::CheckMunchieCollisions()
{
	// Munchie collision
	for (Food& food : _munchiesVector)
	{
		if (CheckBoxCollision(
			_pacman->position->X, _pacman->position->Y, (float)_pacman->sourceRect->Width, (float)_pacman->sourceRect->Height,
			food.Position.X, food.Position.Y, (float)food.Rect.Width, (float)food.Rect.Width))
		{
			// they collision
			_curScore += 10;
			// Play pop sound
			Audio::Play(_pop);
			// Move Munchie out of the screen bounds
			food.Position = Vector2(-100, -100);
		}
	}
}

void Pacman::SetupAStart(int width, int height)
{
	// A* grid
	_grid->gridWorldSize = Vector2(width * 32.0f, height * 32.0f);
	_grid->gridSizeX = width;
	_grid->gridSizeY = height;
	_grid->nodeRadius = 16.0f;
	_grid->nodeDiameter = _grid->nodeRadius * 2.0f;

	_grid->grid = vector<vector<Node>>(_grid->gridSizeX, vector<Node>(_grid->gridSizeY));

	// Go through every tile and place a node on each one of them, depending on whether the tile is walkable or not
	for (const Tile& tile : _tiles)
	{
		bool collission = false;
		if (tile.Type == CollissionType::TILE_WALKABLE)
			collission = true;
		else if (tile.Type == CollissionType::TILE_NOTWALKABLE)
			collission = false;

		_grid->grid[(int)tile.GetX() / 32][(int)tile.GetY() / 32] = Node(collission, tile.GetPosition(), (int)tile.GetX(), (int)tile.GetY());
	}

	_hasLoaded = true;
}

Tile Pacman::LoadMunchieTile(int x, int y)
{
	// Munchie
	Rect r = Rect(0, 0, 12, 12);
	Texture2D* t = new Texture2D();
	t->Load("Textures/Munchie.png", false);
	Vector2 v = Vector2((x * 32) + ((32 * 0.5f) - r.Width * 0.5f), (y * 32) + ((32 * 0.5f) - r.Height * 0.5f));
	_munchiesVector.push_back(Food(r, t, v));

	// Return an empty tile
	return Tile(x, y, nullptr, CollissionType::TILE_WALKABLE);
}

Tile Pacman::LoadPlayerStartTile(int x, int y)
{
	// Load Pacman
	_pacman->position = new Vector2(x * 32.0f, y * 32.0f);

	return Tile(x, y, nullptr, CollissionType::TILE_WALKABLE);
}

Tile Pacman::LoadEnemyTile(int x, int y)
{
	// Ghost
	Rect r = Rect(0, 0, 20, 20);
	Texture2D* t = new Texture2D();
	t->Load("Textures/GhostBlue.png", false);
	Vector2 v = Vector2((x * 32) + ((32 * 0.5f) - r.Width * 0.5f), (y * 32) + (16 - (r.Height * 0.5f)));
	_ghosts.push_back(Ghost(r, t, v));

	// Return empty tile
	return Tile(x, y, nullptr, CollissionType::TILE_WALKABLE);
}


