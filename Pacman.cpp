#include "Pacman.h"
#include <iostream>

#include <sstream>

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

	// Menu
	_menu = new Menu();

	// Initiase Game Stats
	_gameStarted = false;
	_paused = false;

	//Initialise important Game aspects
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

	// Clean up Ghosts
	for (Ghost& ghost : _ghosts)
		delete ghost.GetTexture();

	// Clean up munchies
	for (Food& food : _munchiesVector)
		delete food.GetTexture();

	// Clean up menu
	delete _menu;
}

void Pacman::LoadContent()
{
	// Set Menu Paramters
	_menu->background = new Texture2D();
	_menu->background->Load("Textures/Transparency.png", false);
	_menu->rect = new Rect(0.0f, 0.0f, S2D::Graphics::GetViewportWidth(), S2D::Graphics::GetViewportHeight());
	_menu->stringPosition = new Vector2(S2D::Graphics::GetViewportWidth() / 2.0f, S2D::Graphics::GetViewportHeight() / 2.0f);

	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_pacman->position = new Vector2(0, 0);

	_cherry->texture = new Texture2D();
	_cherry->texture->Load("Textures/Cherry.png", false);
	_cherry->rect = new Rect(0.0f, 0.0f, 32, 32);
	_cherry->position = new Vector2(100, 100);

	// Set string position
	_curScore = 0;
	_scorePosition = new Vector2(10.0f, 25.0f);

	GenerateLevel();

}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	// Check if the game is paused
	CheckPaused(keyboardState, Input::Keys::P);

	if (!_paused && _gameStarted) {

		if (!_pacman->isDead)
		{
			Input(keyboardState);
			MovePacman(elapsedTime);
			CheckViewportCollision();
			UpdatePacmanSprite(elapsedTime);
		}

		for (Food& food : _munchiesVector)
		{
			if (CheckBoxCollision(
				_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Height,
				food.Position.X, food.Position.Y, food.Rect.Width, food.Rect.Width))
			{
				// they collision
				_curScore += 10;
				// Move Munchie out of the screen bounds
				food.Position = Vector2(-100, -100);
			}
		}

		// Cherry collision
		if (CheckBoxCollision(
			_pacman->position->X, _pacman->position->Y, _pacman->sourceRect->Width, _pacman->sourceRect->Width,
			_cherry->position->X, _cherry->position->Y, _cherry->rect->Width, _cherry->rect->Width))
		{
			// Collision detected
			
			// Update score
			_curScore += 200;
			// Move Cherry out of the screen bounds
			_cherry->position = new Vector2(-100, -100);
		}

		UpdateMunchieSprite(elapsedTime);

		UpdateGhostPosition(elapsedTime);
		CheckGhostCollisions();

	}

	// Check if game is started
	CheckGameStarted(keyboardState, Input::Keys::SPACE);

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
	

	SpriteBatch::Draw(_cherry->texture, _cherry->position, _cherry->rect);

	// Draw Player
	if (!_pacman->isDead)
	{
		SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect);
	}

	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _scorePosition, Color::White);


	// Draw Start Menu
	if (!_gameStarted) {
		std::stringstream menuStream;
		menuStream.str("Pacman");
		_menu->stringPosition = new Vector2(S2D::Graphics::GetViewportWidth() / 2.0f, S2D::Graphics::GetViewportHeight() / 2.0f);
		SpriteBatch::Draw(_menu->background, _menu->rect, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition, Color::Yellow);
		menuStream.str("Press Space To Start");
		SpriteBatch::DrawString(menuStream.str().c_str(), new Vector2(_menu->stringPosition->X - 55.0f, _menu->stringPosition->Y + 30.0f), Color::White);
	}

	// Draw Pause Menu
	if (_paused) {
		std::stringstream menuStream;
		menuStream.str("Game Paused");

		SpriteBatch::Draw(_menu->background, _menu->rect, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menu->stringPosition, Color::White);
	}

	// End Drawing
	SpriteBatch::EndDraw();

}

bool Pacman::CheckBoxCollision(int x1, int y1, int width1, int height1, int x2, int y2, int width2, int height2)
{
	int left1 = x1;
	int left2 = x2;
	int right1 = x1 + width1;
	int right2 = x2 + width2;
	int top1 = y1;
	int top2 = y2;
	int bottom1 = y1 + height1;
	int bottom2 = y2 + height2;

	if (bottom1 < top2) return false;
	if (top1 > bottom2) return false;
	if (right1 < left2) return false;
	if (left1 > right2) return false;

	return true;
}

void Pacman::CheckGameStarted(Input::KeyboardState* state, Input::Keys startKey)
{
	// Checks if Space is pressed and game isn't started
	if (state->IsKeyDown(startKey) && !_gameStarted) {
		_gameStarted = true;
	}
}

void Pacman::CheckPaused(Input::KeyboardState* state, Input::Keys pauseKey)
{
	// Checks if we are in the game (ignores whether the game is paused or not)
	if (_gameStarted) {

		// Checks if P key is pressed
		if (state->IsKeyDown(pauseKey) && !_menu->isKeyDown)
		{
			_menu->isKeyDown = true;
			_paused = !_paused;
		}

		if (state->IsKeyUp(pauseKey))
			_menu->isKeyDown = false;
	}
}

void Pacman::CheckViewportCollision()
{
	// Checks if Pacman is off the right side of the screen
	if (_pacman->position->X + _pacman->sourceRect->Width > _width)
		_pacman->position->X = (float)(0 - 32) + _pacman->sourceRect->Width;

	// Checks if Pacman is off the left side of the screen
	if (_pacman->position->X - _pacman->sourceRect->Width < (float)(0 - 32))
		_pacman->position->X = _width - _pacman->sourceRect->Width;

	// Checks if Pacman is off the top side of the screen
	if (_pacman->position->Y - _pacman->sourceRect->Height < (float)(0 - 32))
		_pacman->position->Y = _height - _pacman->sourceRect->Height;

	// Checks if Pacman is off the down side of the screen
	if (_pacman->position->Y + _pacman->sourceRect->Height > _height)
		_pacman->position->Y = (float)(0 - 32) + _pacman->sourceRect->Height;
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
				_tiles.push_back(Tile(x, y, wall, TileType::TILE_SOLID));
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
				_tiles.push_back(Tile(x, y, nullptr, TileType::TILE_TRANSPARENT));
			}			
		}
	}

	// Free up ram
	SOIL_free_image_data(myImage);
}

void Pacman::Input(Input::KeyboardState* state)
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

	// Move pacman in the direction of him facing
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

void Pacman::UpdateMunchieSprite(int elapsedTime)
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

		food.Rect.X = food.Rect.Width * food.FrameCount;
	}
}

void Pacman::UpdatePacmanSprite(int elapsedTime)
{
	// Update Pacman frame time
	_pacman->currentFrameTime += elapsedTime;

	if (_pacman->currentFrameTime > _cPacmanFrameTime) {
		_pacman->frame++;

		if (_pacman->frame >= 2)
			_pacman->frame = 0;

		_pacman->currentFrameTime = 0;
	}

	_pacman->sourceRect->X = _pacman->sourceRect->Width * _pacman->frame;


	// Set the Pacman source rect to match with the row of the spritesheet which pacman needs
	_pacman->sourceRect->Y = _pacman->sourceRect->Height * _pacman->direction;
}

void Pacman::UpdateGhostPosition(int elapsedTime)
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

void Pacman::CheckGhostCollisions()
{
	// Local Variables
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->sourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->sourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;

	for (Ghost& ghost : _ghosts)
	{
		// Populate variables with ghost data
		bottom2 = ghost.Position.Y + ghost.Rect.Height;
		left2 = ghost.Position.X;
		right2 = ghost.Position.X + ghost.Rect.Width;
		top2 = ghost.Position.Y;

		// Check for collision
		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			// Collision detected
			_pacman->isDead = true;
		}
	}
}

Tile Pacman::LoadMunchieTile(int x, int y)
{
	// Munchie
	Rect r = Rect(0, 0, 12, 12);
	Texture2D* t = new Texture2D();
	t->Load("Textures/Munchie.png", false);
	Vector2 v = Vector2((x * 32) + ((32 * 0.5f) - r.Width * 0.5f), (y * 32) + (16 - (r.Height * 0.5f)));
	_munchiesVector.push_back(Food(r, t, v));

	// Return an empty tile
	return Tile(x, y, nullptr, TileType::TILE_TRANSPARENT);
}

Tile Pacman::LoadPlayerStartTile(int x, int y)
{
	// Load Pacman
	_pacman->position = new Vector2(x * 32, y * 32);

	return Tile(x, y, nullptr, TileType::TILE_TRANSPARENT);
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
	return Tile(x, y, nullptr, TileType::TILE_TRANSPARENT);
}


