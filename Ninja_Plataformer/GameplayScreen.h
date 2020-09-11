#ifndef GAMEPLAYSCREEN_H
#define GAMEPLAYSCREEN_H

#include <Bengine/IGameScreen.h>
#include <Bengine/GLSLProgram.h>
#include <Bengine/Camera2D.h>
#include <Bengine/SpriteBatch.h>
#include <Bengine/GLTexture.h>
#include <Bengine/Window.h>
#include <Bengine/DebugRenderer.h>
#include <Bengine/GUI.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

#include <box2d/box2d.h>
#include <memory>
#include "Box.h"
#include "Player.h"
#include <vector>

class GameplayScreen : public Bengine::IGameScreen{
public:
    GameplayScreen(Bengine::Window* window);
    virtual ~GameplayScreen() override;

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

protected:

private:
    void initUI();
    void checkInput();

    ///UI Events
    bool onExitClicked(const CEGUI::EventArgs& e);

    Bengine::GLSLProgram m_textureProgram;
    Bengine::GLSLProgram m_lightProgram;
    Bengine::Camera2D m_camera;
    Bengine::SpriteBatch m_spriteBatch;
    Bengine::GLTexture m_texture;
    Bengine::Window* m_window;
    Bengine::DebugRenderer m_debugRenderer;
    Bengine::GUI m_gui;

    bool m_renderDebug = false;

    Player m_player;
    std::unique_ptr<b2World> m_world;
    std::vector<Box> m_boxes;
};

#endif // GAMEPLAYSCREEN_H
