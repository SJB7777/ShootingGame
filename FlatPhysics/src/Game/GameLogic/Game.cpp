#include "Game.h"
#include <stdexcept>

CameraExtents CameraManager::GetExtents()
{
    float left = camera.target.x - GetScreenWidth() / camera.zoom / 2;
    float right = camera.target.x + GetScreenWidth() / camera.zoom / 2;
    float bottom = camera.target.y - GetScreenHeight() / camera.zoom / 2;
    float top = camera.target.y + GetScreenHeight() / camera.zoom / 2;

    return { left, right, bottom, top };
}

void Game::Setting() {
    SetTargetFPS(60);
    SetWindowPosition(10, 40);
    InitAudioDevice();

    CameraExtents CameraExtent = camera.GetExtents();
    
    float padding = (CameraExtent.right - CameraExtent.left) * 0.1f;
    
    hp = 100; left_ball = 5;
    
    entityVector.push_back(new FlatEntity(world, CameraExtent.right - CameraExtent.left, 30, true, DARKGREEN, { 0, 200 }));
    entityVector.push_back(new FlatEntity(world, 30, 270, true, GRAY, { 0, 50 }));

    // Enemy
    
    entityVector.push_back(enemy);
    crashSound = LoadSound("asset/crash.mp3");
    //entityVector.push_back(new FlatEntity(world, 30, 70, true, RED, { 300, 150 }));
    //entityVector.push_back(new FlatEntity(world, 15, 15, true, RED, { 300, 107 }));

    entityVector.push_back(new FlatEntity(world, 1, 600, true, WHITE, { 330, 0 })); // right wall
    entityVector.push_back(new FlatEntity(world, 800, 1, true, WHITE, { 0, -220 })); // upper wall
    

    Btn.SetButton(" || ", 40, 20, 20);
    Btn.SetPosition(940, 30);
    Btn_Resume.SetButton("Resume", 300, 50, 20, 6, 500, 450);
    Btn_Retry.SetButton("Retry", 300, 50, 20, 6, 500, 550);
    Btn_Mainmenu.SetButton("Mainmenu", 300, 50, 20, 6, 500, 650);
    Btn_Music.SetButton("Music", 60, 30, 20, 6, 860, 25);
    
    cannon = new Cannon(world, 10.0f);
    entityVector.push_back(cannon->GetEntity());

    texture_start_page = LoadTexture("asset/start_page.png");
    music = LoadMusicStream("asset/bgm.mp3");
    music2 = LoadMusicStream("asset/bgm2.mp3");
    music_select = 1;
    music_mute = 0;

    
}

void Game::UpdateGameClear()
{
    Btn_Mainmenu.click_connect(this, &Game::to_menu);
    Btn_Retry.click_connect(this, &Game::retry);
}

void Game::DrawGameClear()
{
    Screen::DimScreen();
    DrawRectangle(200, 120, 900, 600, WHITE);
    DrawText("Game Clear", 420, 300, 80, BLACK);

    Btn_Retry.draw();
    Btn_Mainmenu.draw();
}

void Game::UpdateGame(float deltaTime) {
    
    
    // camera move
    camera.camera.zoom += ((float)GetMouseWheelMove() * camera.zoomSpeed);

    if (camera.camera.zoom > 30.0f) camera.camera.zoom = 30.0f;
    else if (camera.camera.zoom < 0.01f) camera.camera.zoom = 0.01f;

    
    if (CheckCollisionPointCircle(GetScreenToWorld2D(GetMousePosition(), camera.camera), FlatConverter::ToVector2(cannon->origin), 10.0f))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            cannon->isClicked = true;
        }
    }
    if (cannon->isClicked)
    {
        cannon->GetEntity()->MoveTo(FlatConverter::ToFlatVector(GetScreenToWorld2D(GetMousePosition(), camera.camera)));
        cannon->isReleased = false;
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
        {
            
            left_ball--;
            FlatEntity* entity = cannon->GetEntity();
            cannon->isClicked = false;
            entity->GetBody()->IsStatic = false;
            entity->SetVelocity(FlatVector::Zero());
            entity->AddForce(cannon->GetDisplacement() * -800000.0f);
            cannon->isReleased = true;
            cannon->ResetTime();
            
        }
    }
    if (cannon->isReleased)
    {
        if (cannon->Time() > 10.0f)
        {
            left_ball--;
            cannon->ResetTime();
            cannon->isReleased = false;
            cannon->isClicked = false;
            cannon->cannon->MoveTo(cannon->origin);
            cannon->cannon->ToggleIsStatic();
            cannon->cannon->SetVelocity(FlatVector::Zero());
        }
    }
    
    if (IsKeyPressed(KEY_C))
        ApplicationState = ApplicationStates::GameClear;
    if (IsKeyPressed(KEY_G))
        ApplicationState = ApplicationStates::GameOver;
    if (IsKeyPressed(KEY_A))
        ApplicationState = ApplicationStates::Text;
    if (IsKeyPressed(KEY_B))
        ApplicationState = ApplicationStates::ScoreBoard;
        
    if (IsKeyPressed(KEY_T))
    {
        camera.camera.zoom = defaultZoom;
        camera.camera.target = Vector2 { 0, 0 };
    }

    if (IsKeyDown(KEY_RIGHT))
    {
        camera.camera.target.x += deltaTime * camera.linearSpeed;
    }

    if (IsKeyDown(KEY_LEFT))
    {
        camera.camera.target.x -= deltaTime * camera.linearSpeed;
    }

    if (IsKeyDown(KEY_DOWN))
    {
        camera.camera.target.y += deltaTime * camera.linearSpeed;
    }

    if (IsKeyDown(KEY_UP))
    {
        camera.camera.target.y -= deltaTime * camera.linearSpeed;
    }
    
    // 과거 enemy의 속도 기록
    oldVelocity = enemy->GetBody()->LinearVelocity;

    double startTime = GetTime();
    world.Step(deltaTime, 10);
    stepTime = GetTime() - startTime;

    // enemey의 속도 변화로 충격량 계산
    float impulseSq = FlatMath::DistanceSquared(enemy->GetBody()->LinearVelocity, oldVelocity);
    if (impulseSq > 100.0f)
    {
        cannon->ResetTime();
        // 적절한 데미지로 변환
        float damage = 10.0f * powf(impulseSq, 0.2);
        hp = hp - damage;
        printf("%f %f\n", damage, hp);
        
        PlaySound(crashSound);
    }

    CameraExtents extents = camera.GetExtents();
    float viewBottom = extents.top;

    entityRemovalVector.clear();

    /*for (int i = 0; i < world.BodyCount(); i++)
    {
        FlatEntity* entity = entityVector[i];
        FlatBody* body = entity->GetBody();
        
        if (!world.GetBody(i, body))
        {
            throw std::out_of_range("Body is out of range");
        }
        
        if (body->IsStatic)
        {
            continue;
        }

        FlatAABB box = body->GetAABB();

        if (box.Max.y > viewBottom)
        {
            entityRemovalVector.push_back(entity);
        }
    }*/

    /*for (int i = 0; i < entityRemovalVector.size(); i++)
    {
        FlatEntity* entity = entityRemovalVector[i];
        world.RemoveBody(entity->GetBody());
        entityVector.erase(remove(entityVector.begin(), entityVector.end(), entity), entityVector.end());
    }*/
    Btn.click_connect(this, &Game::pause);
    Btn_Music.click_connect(this, &Game::music_change);

    if (music_select == 1) {
        StopMusicStream(music2);
        UpdateMusicStream(music);
        PlayMusicStream(music);
    }
    else if (music_select == 2) {
        StopMusicStream(music);
        UpdateMusicStream(music2);
        PlayMusicStream(music2);
    }

    if (left_ball == -1)
        ApplicationState = ApplicationStates::GameOver;
    if (hp <= 0)
        ApplicationState = ApplicationStates::GameClear;
}

void Game::UpdateMainMenu()
{
    initialize();

    start_btn.click_connect(this, &Game::run);
    exit_btn.click_connect(this, &Game::exit);

}

void Game::DrawMainMenu()
{
    DrawTexture(texture_start_page, 0, 0, WHITE);
    start_btn.draw();
    exit_btn.draw();
}

void Game::UpdatePaused()
{
    Btn.click_connect(this, &Game::run);
    Btn_Resume.click_connect(this, &Game::run);
    Btn_Mainmenu.click_connect(this, &Game::to_menu);
    Btn_Retry.click_connect(this, &Game::retry);
}

void Game::DrawPaused()
{
    Screen::DimScreen();
    DrawRectangle(200, 120, 900, 600, WHITE);
    DrawText("Game Pause", 420, 300, 80, BLACK);

    Btn.draw();
    Btn_Resume.draw();
    Btn_Retry.draw();
    Btn_Mainmenu.draw();
}

void Game::UpdateGameOver()
{
    Btn_Mainmenu.click_connect(this, &Game::to_menu);
    Btn_Retry.click_connect(this, &Game::retry);
}

void Game::DrawGameOver()
{
    Screen::DimScreen();
    DrawRectangle(200, 120, 900, 600, WHITE);
    DrawText("Game Over", 420, 300, 80, BLACK);

    Btn_Retry.draw();
    Btn_Mainmenu.draw();
}

void Game::Draw(float deltaTime) {

    BeginMode2D(camera.camera);

    for (int i = 0; i < entityVector.size(); i++)
    {
        entityVector[i]->Draw();
    }

    if (cannon->isClicked)
    {
        cannon->DrawRober();
    }
    cannon->Draw();

    

    
    DrawRectangle(-325, -220, 150, 40, BLUE);
    for (int i = 0; i < 5; i++)
        DrawCircle(-310 + 30 * i, -200, 10, GREEN);
    for (int i = left_ball; i < 5 && i >= 0; i++) {
        //DrawCircle(-310 + 30 * i, -200, 10, GREEN);
        DrawLine(-175 - 30 * i, -215, -205 - 30 * i, -185, RED);
    }
    DrawRectangle(175, -220, 150, 40, BLUE);
    DrawText("HP", 180, -205, 10, BLACK);
    DrawRectangle(200, -210, 1.25 * hp, 20, RED);

    EndMode2D();

    Btn.draw();
    Btn_Music.draw();

    DrawText(TextFormat("StepTime : %.4fms", stepTime * 1000), 20, GetScreenHeight() - 30 - 10 - 20 - 20, 20, YELLOW);
    DrawText(TextFormat("BodyCount : %d", world.BodyCount()), 20, GetScreenHeight() - 30 - 10 - 20, 20, YELLOW);
    DrawText(TextFormat("Zoom : %d %%", int(camera.camera.zoom / defaultZoom * 100)), 20, GetScreenHeight() - 30 - 10, 20, YELLOW);
    DrawFPS(20, 20);
}

void Game::End() {
    UnloadTexture(texture_start_page);
    UnloadMusicStream(music);
    UnloadMusicStream(music2);
    UnloadSound(crashSound);
}

/*void Game::UpdateScore() {
    char name[NAMELEN] = "\0";
    int letterCount = 0;

    if (CheckCollisionPointRec(GetMousePosition(), textbox)) //mouse in textbox
        mouseontext = true;
    else
        mouseontext = false;

    if (mouseontext)
    {
        // Set the window's cursor to the I-Beam
        SetMouseCursor(MOUSE_CURSOR_IBEAM);

        // Get char pressed (unicode character) on the queue
        int key = GetCharPressed();

        // Check if more characters have been pressed on the same frame
        while (key > 0)
        {
            // NOTE: Only allow keys in range [32..125]
            if ((key >= 32) && (key <= 125) && (letterCount < NAMELEN - 1))
            {
                name[letterCount] = (char)key;
                name[letterCount + 1] = '\0'; // Add null terminator at the end of the string.
                letterCount++;
            }

            key = GetCharPressed();  // Check next character in the queue
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            letterCount--;
            if (letterCount < 0) letterCount = 0;
            name[letterCount] = '\0';
        }
    }
    else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

    if (mouseOnText) framesCounter++;
    else framesCounter = 0;
}*/
