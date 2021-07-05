#pragma once

class GameManager {
public:
	GameManager(GameManager& other) = delete;
	void operator=(const GameManager&) = delete;
	static GameManager* Instance();
	~GameManager();

	void SetDebug(bool state) { gDebug = state; };
	bool GetDebug() { return gDebug; };


private:
	bool gDebug = false;
	static GameManager* mGameManager;

protected:
	GameManager() {}
};