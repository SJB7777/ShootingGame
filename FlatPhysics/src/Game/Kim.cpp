#include "GameLogic/Game.h"
#include "scoreboard.h"


void Game::Setting_K()
{
    //menu button
    int HT = WINDOW_HEIGHT, WT = WINDOW_WIDTH;
    ApplicationState = ApplicationStates::Menu;
    start_btn.SetButton("Start", WT / 4, HT / 10, 30, 6.0f, WT / 2 - WT / 4 - WT / 8, HT * 4 / 6);
    exit_btn.SetButton("Exit", WT / 4, HT / 10, 30, 6.0f, WT / 2 - WT / 4 + WT * 3 / 8, HT * 4 / 6);
    menu_btn.SetButton("Meun", WT / 8, HT / 20, 20, 6.0f, WT / 2 - WT / 4, HT * 4 / 6);
}


//txtbox 설정
void Game::Updatetextbox(char* name) {

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

    if (mouseontext) framesCounter++;

    else framesCounter = 0;

    ApplicationState = ApplicationStates::ScoreBoard;
}

//txtbox 그리기
void Game::Drawtextbox(char* name) {
    ClearBackground(RAYWHITE);
    Screen::DimScreen();

    DrawText("PLACE MOUSE OVER INPUT BOX!", 240, 140, 20, GRAY);

    DrawRectangleRec(textbox, LIGHTGRAY);
    if (mouseontext) DrawRectangleLines((int)textbox.x, (int)textbox.y, (int)textbox.width, (int)textbox.height, RED);
    else DrawRectangleLines((int)textbox.x, (int)textbox.y, (int)textbox.width, (int)textbox.height, DARKGRAY);

    DrawText(name, (int)textbox.x + 5, (int)textbox.y + 8, 40, MAROON);

    DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, NAMELEN - 1), 315, 250, 20, DARKGRAY);

    if (mouseontext)
    {
        if (letterCount < NAMELEN - 1)
        {
            // Draw blinking underscore char
            if (((framesCounter / 20) % 2) == 0) DrawText("_", (int)textbox.x + 8 + MeasureText(name, 40), (int)textbox.y + 12, 40, MAROON);
        }
        else DrawText("Press BACKSPACE to delete chars...", 230, 300, 20, GRAY);
    }
}

void Game::UpdateScore(scoreboard SB) {


    int check = 0;

    //update name
    if (!check)
        Game::Updatetextbox(name);

    else
    {
        //Node* root = SB.GetHead(); //get root

        SB.write_score(name, score); //update score
    }

    menu_btn.click_connect(this, &Game::to_menu);
}


void Game::DrawScore(scoreboard SB) {
    Node* Head = SB.GetHead();
    Node* ptr = Head;
    int size = SB.GetSize();

    for (int i = 0; i < size; i++)
    {
        DrawText(TextFormat("%-10s | %d", ptr->name, ptr->score), 200, 300 + 50 * i, 50, BLACK);
    }

    menu_btn.draw();
}

