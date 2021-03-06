#include "GuiManager.h"
#include "App.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "GuiButton.h"

GuiManager::GuiManager() : Module()
{
	name.Create("guimanager");
}

GuiManager::~GuiManager()
{

}

bool GuiManager::Awake(pugi::xml_node& config)
{
	return true;
}

bool GuiManager::Start()
{
	return true;
}

bool GuiManager::Update(float dt)
{
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	UpdateAll(dt, doLogic);

	if (doLogic == true)
	{
		accumulatedTime = 0.0f;
		doLogic = false;
	}

	return true;
}

bool GuiManager::PostUpdate()
{
	DrawAll();
	return true;
}

bool GuiManager::CleanUp()
{
	return true;
}

GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, SDL_Rect bounds)
{
	GuiControl* control = nullptr;

	switch (type)
	{
		case GuiControlType::BUTTON:
			control = new GuiButton(id, bounds, titleScreenTexGui);
			break;
		case GuiControlType::CHECKBOX:
			control = new GuiCheckBox(id, bounds, "EXIT");
			break;
		case GuiControlType::SLIDER:
			control = new GuiSlider(id, bounds, "EXIT");
			break;
		default: break;
	}
	id++;

	if (control != nullptr)
	{
		controls.Add(control);
	}
	return control;
}

void GuiManager::AddGuiControl(GuiControl* control)
{
	if (control != nullptr) controls.Add(control);
}

void GuiManager::DestroyGuiControl(GuiControl* control)
{
	int index = controls.Find(control);
	ListItem<GuiControl*>* L = controls.At(index);
	controls.Del(L);
}

void GuiManager::UpdateAll(float dt, bool doLogic)
{
	if (doLogic == true)
	{
		for (int i = 0; i < controls.Count(); i++)
		{
			controls[i]->Update(app->input, dt);
		}
	}
}

void GuiManager::DrawAll()
{
	for (int i = 0; i < controls.Count(); i++)
	{
		controls[i]->Draw(app->render);
	}
}