#pragma once
#include <Bengine/IGameScreen.h>
#include <CEGUI\EventArgs.h>
#include <Bengine\Window.h>
#include <Bengine\Camera2D.h>
#include <Bengine\GUI.h>

#include "ScreenIndices.h"

class MainMenuScreen : public Bengine::IGameScreen
{
public:
	MainMenuScreen(Bengine::Window* window);
	virtual ~MainMenuScreen() override;

	///Returns index of previous or next screen when changing
	virtual int getNextScreenIndex() const override;
	virtual int getPreviousScreenIndex() const override;

	///Called at beginning and end of application
	virtual void build() override;
	virtual void destroy() override;

	///Called when a screen enters and exits focus
	virtual void onEntry() override;
	virtual void onExit() override;

	///Called in the main game loop
	virtual void update(float deltaTime) override;
	virtual void draw() override;
private:
	void initUI();
	void checkInput();

	///UI Events
	bool onExitClicked(const CEGUI::EventArgs& e);
	bool onLevelEditorClicked(const CEGUI::EventArgs& e);
	bool onNewGameClicked(const CEGUI::EventArgs& e);

	Bengine::Camera2D m_camera;
	Bengine::Window* m_window;
	Bengine::GUI m_gui;
	int m_nextScreenIndex = SCREEN_INDEX_GAMEPLAY;
};

