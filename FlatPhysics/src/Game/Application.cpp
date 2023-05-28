
#include "GameLogic\Game.h"
#include "scoreboard.h"


int main(int argc, char** argv)
{
	ApplicationState = ApplicationStates::Menu;
	Game game;
	scoreboard SB;

	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game");
	SetWindowState(FLAG_VSYNC_HINT);
	

	game.Setting();

	while (!WindowShouldClose() && ApplicationState != ApplicationStates::Exit) {
		float time = GetFrameTime();
		

		switch (ApplicationState)
		{
		case ApplicationStates::Menu:
			game.UpdateMainMenu();
			BeginDrawing();
			ClearBackground(SKYBLUE);

				game.DrawMainMenu();

			EndDrawing();
			break;
		case ApplicationStates::Running:
			game.UpdateGame(time);
			BeginDrawing();
			ClearBackground(SKYBLUE);

				game.Draw(time);

			EndDrawing();
			break;
		case ApplicationStates::Paused:
			game.UpdatePaused();
			BeginDrawing();
			ClearBackground(SKYBLUE);

				
				game.Draw(time);
				game.DrawPaused();

			EndDrawing();
			break;
		case ApplicationStates::GameOver:
			game.UpdateGameOver();
			BeginDrawing();
			ClearBackground(SKYBLUE);
				game.Draw(time);
				game.DrawGameOver();

			EndDrawing();
			break;
		case ApplicationStates::GameClear:
			game.UpdateGameClear();
			BeginDrawing();
			ClearBackground(SKYBLUE);
				game.Draw(time);
				game.DrawGameClear();

			EndDrawing();
			break;


		case ApplicationStates::Text:
			game.Updatetextbox(game.name);
			BeginDrawing();
			game.Drawtextbox(game.name);
			EndDrawing();

		case ApplicationStates::ScoreBoard:
			game.UpdateScore(SB);
			BeginDrawing();
			game.DrawScore(SB);
			EndDrawing();
		}
		if (ApplicationState == ApplicationStates::Exit)
			break;

	}

	//game.End();

	return 0;
}




