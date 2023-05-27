#pragma once
#include "Common.h"
#include <string>



using namespace FlatPhysics;



struct CameraExtents {
	float left;
	float right;
	float bottom;
	float top;
};

struct CameraManager {
	Camera2D camera;
	float linearSpeed;
	float zoomSpeed;
	CameraExtents GetExtents();
};

class Cannon
{
public:
	Cannon() {}
	Cannon(FlatWorld& world, float radius)
	{
		cannon = new FlatEntity(world, radius, false, YELLOW, origin);
		cannon->GetBody()->IsStatic = true;

		texture = LoadTexture("asset/slingshot.png");
	}
	~Cannon()
	{
		UnloadTexture(texture);
	}
	Texture texture;
	FlatVector origin = { -250.0f, 100.0f };
	FlatEntity* cannon = new FlatEntity();
	FlatVector displacement = FlatVector::Zero();
	FlatVector GetDisplacement()
	{
		return cannon->GetPosition() - origin;
	}
	void DrawRober()
	{
		FlatVector endPos = cannon->GetPosition();
		displacement = endPos - origin;
		float thick = sqrt(300.0f / FlatMath::Length(displacement));
		GameDraw::DrawLine(origin, endPos, thick, BROWN);
	}
	void Draw()
	{
		DrawTexturePro(texture, { 0, 0, (float)texture.width, (float)texture.height }, { origin.x, origin.y, 50, 100 }, { 15.0f, 15.0f }, 0.0f, WHITE);

	}
	FlatEntity* GetEntity() { return cannon; }
	bool isClicked = false;
	bool isReleased = false;
	void ResetTime()
	{
		startTime = GetTime();
	}
	double Time()
	{
		return GetTime() - startTime;
	}
private:
	double startTime;
};

class Game
{
public:
	void Setting();
	
	void UpdateGame(float time);
	void Draw(float time);

	void UpdateMainMenu();
	void DrawMainMenu();

	void UpdatePaused();
	void DrawPaused();

	void UpdateGameOver();
	void DrawGameOver();

	void UpdateGameClear();
	void DrawGameClear();

	void UpdateScore();
	void DrawScore();

	void End();

	int left_ball;
	float hp;
private:
	double stepTime;
	float alpha = 0.75f;
	
	std::string errorMessage;
	
	const float defaultZoom = 2.0f;
	CameraManager camera
	{ 
		{ 
			{ WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 },
			{ 0, 0 },
			0.0f, defaultZoom
		}, 
		70.0f, 0.5f
	};

	
	std::vector<FlatEntity*> entityVector;
	std::vector<FlatEntity*> entityRemovalVector;

	FlatWorld world{ {0, 98.1} };
	std::vector<Vector2> vertexBuffer;

	Button start_btn, exit_btn;
	Button Btn;
	Button Btn_Retry;
	Button Btn_Resume;
	Button Btn_Mainmenu;
	Button Btn_Music;

	void initialize() {
		left_ball = 5;
		hp = 100;
		music_select = 1;
		StopMusicStream(music);
		StopMusicStream(music2);
	}
	void run() { ApplicationState = ApplicationStates::Running; }
	void pause() { ApplicationState = ApplicationStates::Paused; }
	void exit() { ApplicationState = ApplicationStates::Exit; }
	void to_menu() { ApplicationState = ApplicationStates::Menu; }
	void retry() {
		ApplicationState = ApplicationStates::Running;
		initialize();
	}
	void music_change() {
		if (music_select == 1)
			music_select = 2;
		else if (music_select == 2)
			music_select = 1;
	}
	
	//FlatVector cannonOrigin = { -250.0f, 150.0f };
	Cannon* cannon = new Cannon();

	Texture2D texture_start_page;
	Music music;
	Music music2;
	int music_select, music_mute;

	//textbox
//	Rectangle textbox = { GetScreenWidth() / 2 - 100, 180, 225, 50 };
	bool mouseontext = false;

	// enemy
	
	std::vector<FlatVector> vertices = { 
		{-15, 0},
		{-25, 60}, 
		{25, 60}, 
		{15, 0}, 
		{-15, 0} 
	};
	/*std::vector<FlatVector> vertices = {
		{93, 0},
		{68, 42},
		{53, 42},
		{46, 73},
		{0, 88},
		{33, 121},
		{24, 160},
		{162, 160},
		{153, 121},
		{186, 88},
		{140, 73},
		{133, 42},
		{118, 42},
		{93, 0}
	};*/
	FlatEntity* enemy = new FlatEntity(world, vertices, false, RED, { 260, 157.5 });
	FlatVector oldVelocity = FlatVector::Zero();
	Sound crashSound;
};



