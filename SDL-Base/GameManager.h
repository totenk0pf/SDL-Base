#pragma once

class GameManager {
public:
	GameManager();
	~GameManager();

	void SetDebug(bool state) { gDebug = state; };
	bool GetDebug() { return gDebug; };

private:
	bool gDebug = false;
	
protected:

};