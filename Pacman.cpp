#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f), _cPacmanFrameTime(250)
{
	// Pacman
	_pacman = new Player();
	_pacman->direction = 0;
	_pacman->currentFrameTime = 0;
	_pacman->frame = 0;

	// Local Variable

	// Initialise Munchies
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new Munchie();
		_munchies[i]->frameCount = 0;
		_munchies[i]->currentFrameTime = 0;
		_munchies[i]->frameCount = rand() % 1;
		_munchies[i]->frameTime = rand() % 500 + 100;
	}

	// Menu
	_menu = new Menu();

	// Initiase Game Stats
	_gameStarted = false;
	_paused = false;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 144);
	Input::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	// Clean up pointers within the Pacman structure
	delete _pacman->texture;
	delete _pacman->sourceRect;
	delete _pacman->position;

	// Clean up the Pacman structure pointer
	delete _pacman;

	// Clean up munchie
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		delete _munchies[i]->texture;
		delete _munchies[i]->rect;
	}
	delete[] _munchies;

	// Clean up menu
	delete _menu;
}

void Pacman::LoadContent()
{
	// Set Menu Paramters
	_menu->background = new Texture2D();
	_menu->background->Load("Textures/Transparency.png", false);
	_menu->rect = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menu->stringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);

	// Load Pacman
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(350.0f, 350.0f);
	_pacman->sourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Load Munchies
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->texture = new Texture2D();
		_munchies[i]->texture->Load("Textures/Munchie.png", false);
		_munchies[i]->rect = new Rect(0.0f, 0.0f, 12, 12);
		_munchies[i]->position = new Vector2(rand() % Graphics::GetViewportWidth(), rand() % Graphics::GetViewportHeight());
	}

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);
}

void Pacman::Update(int elapsedTime)
{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	// Check if the game is paused
	CheckPaused(keyboardState, Input::Keys::P);

	if (!_paused && _gameStarted) {

		Input(elapsedTime, keyboardState);
		UpdatePacman(elapsedTime);
		UpdateMunchie(elapsedTime);
		CheckViewportCollision();
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
	stream << "Pacman X: " << _pacman->position->X << " Y: " << _pacman->position->Y;

	// Start Drawing
	SpriteBatch::BeginDraw();

	// Draw Player
	SpriteBatch::Draw(_pacman->texture, _pacman->position, _pacman->sourceRect);

	// Draw Munchies
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		SpriteBatch::Draw(_munchies[i]->texture, _munchies[i]->position, _munchies[i]->rect);
	}


	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::White);


	// Draw Start Menu
	if (!_gameStarted) {
		std::stringstream menuStream;
		menuStream.str("Pacman");
		_menu->stringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
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
	if (_pacman->position->X + _pacman->sourceRect->Width > Graphics::GetViewportWidth())
		_pacman->position->X = 0.0f + (float)_pacman->sourceRect->Width;

	// Checks if Pacman is off the left side of the screen
	if (_pacman->position->X - _pacman->sourceRect->Width < 0.0f)
		_pacman->position->X = Graphics::GetViewportWidth() - (float)_pacman->sourceRect->Width;

	// Checks if Pacman is off the top side of the screen
	if (_pacman->position->Y - _pacman->sourceRect->Height < 0.0f)
		_pacman->position->Y = Graphics::GetViewportHeight() - (float)_pacman->sourceRect->Height;

	// Checks if Pacman is off the down side of the screen
	if (_pacman->position->Y + _pacman->sourceRect->Height > Graphics::GetViewportHeight())
		_pacman->position->Y = 0.0f + (float)_pacman->sourceRect->Height;
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* state)
{
	// Checks if D key is pressed
	if (state->IsKeyDown(Input::Keys::D)) {
		_pacman->position->X += _cPacmanSpeed * elapsedTime; //Moves Pacman across X axis
		_pacman->direction = 0;
	}

	// Checks if A key is pressed
	else if (state->IsKeyDown(Input::Keys::A)) {
		_pacman->position->X -= _cPacmanSpeed * elapsedTime; // Moves Pacman in the minus direction of the X axis
		_pacman->direction = 2;
	}
	// Checks if W key is pressed
	else if (state->IsKeyDown(Input::Keys::W)) {
		_pacman->position->Y -= _cPacmanSpeed * elapsedTime;
		_pacman->direction = 3;
	}

	// Checks if S key is pressed
	else if (state->IsKeyDown(Input::Keys::S)) {
		_pacman->position->Y += _cPacmanSpeed * elapsedTime;
		_pacman->direction = 1;
	}
}

void Pacman::UpdateMunchie(int elapsedTime)
{

	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		// Update Munchies' frame time
		_munchies[i]->currentFrameTime += elapsedTime;

		if (_munchies[i]->currentFrameTime > _munchies[i]->frameTime) {
			_munchies[i]->frameCount++;

			if (_munchies[i]->frameCount >= 2)
				_munchies[i]->frameCount = 0;

			_munchies[i]->currentFrameTime = 0;
		}

		// Update Munchies' Source Rect
		_munchies[i]->rect->X = _munchies[i]->rect->Width * _munchies[i]->frameCount;
	}
}

void Pacman::UpdatePacman(int elapsedTime)
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

