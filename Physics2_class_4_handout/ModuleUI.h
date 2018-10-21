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
	bool CleanUp();

public:
	int score_font = 0;
	int score_player = 0;
	char score_string[10];
};