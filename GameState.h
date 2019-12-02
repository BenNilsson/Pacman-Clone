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
	
	static State state;

private:

	GameState();
};