#pragma once
#include "Module.h"
#include "Globals.h"
#include "p2Point.h"

class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	virtual ~ModuleUI();

	bool Start();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

public:
	int score_font = 0;
	int life_font = 0;
	int score_player = 1000000;
	int high_score = 0;
	char score_string[10];
	char high_score_string[10];
	char life_string[4];
};