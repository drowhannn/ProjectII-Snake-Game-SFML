#include "GameData.hpp"
#include <fstream>

extern GameData gamedata;

GameData::GameData()
{
		for (int i = 0; i < 5; i++)
		{
			highScores[i] = 0;
		}
		std::ifstream ifile("gamedata.bin", std::ios::in | std::ios::binary);
		ifile.read(reinterpret_cast<char*>(&gamedata), sizeof(gamedata));
		ifile.close();
}

void GameData::update(int score)
{
	if (highScores[4] < score)
	{
		highScores[4] = score;
		for (int i = 0; i < 5; i++)
		{
			for (int j = i + 1; j < 5; j++)
			{
				if (highScores[i] < highScores[j])
				{
					int temp = highScores[i];
					highScores[i] = highScores[j];
					highScores[j] = temp;
				}
			}
		}

		std::ofstream ofile("gamedata.bin", std::ios::out | std::ios::binary);
		ofile.write(reinterpret_cast<char*>(&gamedata), sizeof(gamedata));
		ofile.close();

	}
}

int GameData::getHighScore(int i)
{
	return highScores[i];
}