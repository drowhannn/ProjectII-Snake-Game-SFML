#pragma once
class GameData {
private:
	int highScores[5];

public:
	GameData();

	void update(int);

	int getHighScore(int);


};

