#pragma once

enum class State
{
	START = 0,
	INTRO,
	PLAYING,
	PAUSED,
	GAMEOVER,
};

class GameState 
{
public:
	
	static void SetState(State _state);
	static State GetState();
	static State state;

private:

	GameState();
};