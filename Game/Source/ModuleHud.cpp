#include "ModuleHud.h"

#include "App.h"
#include "Render.h"
#include "Textures.h"
#include "ModulePlayer.h"
#include "Log.h"

#include <stdio.h>
ModuleHud::ModuleHud( bool start_enabled) : Module(start_enabled)
{
	spriteHearts = { 432, 880, 16, 16 };
	spriteEmptyHearts = { 416, 880, 16, 16 };
	spriteDiamonds = { 560, 816, 16, 16 };
}


// Destructor
ModuleHud::~ModuleHud()
{
	this->CleanUp();
}


bool ModuleHud::Start()
{
	LOG("Loading hud");

	bool ret = true;

	char lookupTableNumbers[] = { "0123456789" };
	
	heartsTexture = app->tex->Load("Assets/Maps/def_tileset.png");
	emptyHeartsTexture = app->tex->Load("Assets/Maps/def_tileset.png");
	diamondsTexture = app->tex->Load("Assets/Maps/def_tileset.png");

	return ret;
}

bool ModuleHud::Update()
{
	bool ret = true;
	// Draw UI (score) --------------------------------------
	sprintf_s(scoreText, 10, "%4d", score);
	sprintf_s(highScoreText, 10, "%4d", highScore);
	sprintf_s(previousScoreText, 10, "%4d", previousScore);

	//sprintf_s(livesText, 10, "%1d", App->ball->lives);	

	return ret;
}

bool ModuleHud::PostUpdate()
{
	bool ret = true;

	if (app->player->lives == 0)
	{
		app->render->DrawTexture(emptyHeartsTexture, 2 * 16, 2 * 16, &spriteEmptyHearts, 0.0f);
		app->render->DrawTexture(emptyHeartsTexture, 3 * 16, 2 * 16, &spriteEmptyHearts, 0.0f);
		app->render->DrawTexture(emptyHeartsTexture, 4 * 16, 2 * 16, &spriteEmptyHearts, 0.0f);
	}
	if (app->player->lives == 1)
	{
		app->render->DrawTexture(heartsTexture, 2 * 16, 2 * 16, &spriteHearts, 0.0f);
		app->render->DrawTexture(emptyHeartsTexture, 3 * 16, 2 * 16, &spriteEmptyHearts, 0.0f);
		app->render->DrawTexture(emptyHeartsTexture, 4 * 16, 2 * 16, &spriteEmptyHearts, 0.0f);
	}
	if (app->player->lives == 2)
	{
		app->render->DrawTexture(heartsTexture, 2 * 16, 2 * 16, &spriteHearts, 0.0f);
		app->render->DrawTexture(heartsTexture, 3 * 16, 2 * 16, &spriteHearts, 0.0f);
		app->render->DrawTexture(emptyHeartsTexture, 4 * 16, 2 * 16, &spriteEmptyHearts, 0.0f);
	}
	if (app->player->lives == 3)
	{
		app->render->DrawTexture(heartsTexture, 2 * 16, 2 * 16, &spriteHearts, 0.0f);
		app->render->DrawTexture(heartsTexture, 3 * 16, 2 * 16, &spriteHearts, 0.0f);
		app->render->DrawTexture(heartsTexture, 4 * 16, 2 * 16, &spriteHearts, 0.0f);
	}
	if (app->player->lives == 4)
	{
		app->render->DrawTexture(heartsTexture, 2 * 16, 2 * 16, &spriteHearts, 0.0f);
		app->render->DrawTexture(heartsTexture, 3 * 16, 2 * 16, &spriteHearts, 0.0f);
		app->render->DrawTexture(heartsTexture, 4 * 16, 2 * 16, &spriteHearts, 0.0f);
		app->render->DrawTexture(heartsTexture, 5 * 16, 2 * 16, &spriteHearts, 0.0f);
	}
	if (app->player->lives == 5)
	{
		app->render->DrawTexture(heartsTexture, 2 * 16, 2 * 16, &spriteHearts, 0.0f);
		app->render->DrawTexture(heartsTexture, 3 * 16, 2 * 16, &spriteHearts, 0.0f);
		app->render->DrawTexture(heartsTexture, 4 * 16, 2 * 16, &spriteHearts, 0.0f);
		app->render->DrawTexture(heartsTexture, 5 * 16, 2 * 16, &spriteHearts, 0.0f);
		app->render->DrawTexture(heartsTexture, 6 * 16, 2 * 16, &spriteHearts, 0.0f);
	}

	if (app->player->diamonds == 1)
	{
		app->render->DrawTexture(diamondsTexture, 2 * 16, 3.5 * 16, &spriteDiamonds, 0.0f);
	}
	if (app->player->diamonds == 2)
	{
		app->render->DrawTexture(diamondsTexture, 2 * 16, 3.5 * 16, &spriteDiamonds, 0.0f);
		app->render->DrawTexture(diamondsTexture, 3 * 16, 3.5 * 16, &spriteDiamonds, 0.0f);
	}
	if (app->player->diamonds == 3)
	{
		app->render->DrawTexture(diamondsTexture, 2 * 16, 3.5 * 16, &spriteDiamonds, 0.0f);
		app->render->DrawTexture(diamondsTexture, 3 * 16, 3.5 * 16, &spriteDiamonds, 0.0f);
		app->render->DrawTexture(diamondsTexture, 4 * 16, 3.5 * 16, &spriteDiamonds, 0.0f);
	}
	if (app->player->diamonds == 4)
	{
		app->render->DrawTexture(diamondsTexture, 2 * 16, 3.5 * 16, &spriteDiamonds, 0.0f);
		app->render->DrawTexture(diamondsTexture, 3 * 16, 3.5 * 16, &spriteDiamonds, 0.0f);
		app->render->DrawTexture(diamondsTexture, 4 * 16, 3.5 * 16, &spriteDiamonds, 0.0f);
		app->render->DrawTexture(diamondsTexture, 5 * 16, 3.5 * 16, &spriteDiamonds, 0.0f);
	}


	return ret;
}


bool ModuleHud::CleanUp()
{
	return true;
}
