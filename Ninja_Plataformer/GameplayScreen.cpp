#include "GameplayScreen.h"
#include "Light.h"
#include "ScreenIndices.h"

#include <iostream>
#include <Bengine/IMainGame.h>
#include <Bengine/ResourceManager.h>
#include <random>

#include <SDL2/SDL.h>

#define DEBUG_RENDER

GameplayScreen::GameplayScreen(Bengine::Window* window) :
    m_window(window)
{
    m_screenIndex = SCREEN_INDEX_GAMEPLAY;
}

GameplayScreen::~GameplayScreen(){}

///Returns index of previous or next screen when changing
int GameplayScreen::getNextScreenIndex() const{
    return SCREEN_INDEX_NO_SCREEN;
}

int GameplayScreen::getPreviousScreenIndex() const{
    return SCREEN_INDEX_MAINMENU;
}

///Called at beginning and end of application
void GameplayScreen::build(){

}

void GameplayScreen::destroy(){

}

///Called when a screen enters and exits focus
void GameplayScreen::onEntry(){

    b2Vec2 gravity(0.0f, -40.0f);
    m_world = std::make_unique<b2World>(gravity);

    m_debugRenderer.init();

    /*///Make the ground
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -14.0f);
    b2Body* groundBody = m_world->CreateBody(&groundBodyDef);
    ///Make the ground fixture
    b2PolygonShape groundBox;
    groundBox.SetAsBox(50.0f, 5.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);*/

    ///Load texture
    m_texture = Bengine::ResourceManager::getTexture("Assets/bricks_top.png");

    // Make the ground
    Box groundBox;
    groundBox.init(m_world.get(), glm::vec2(0.0f, -10.0f), glm::vec2(50.0f, 5.0f), m_texture, Bengine::ColorRGBA8(255, 255, 255, 255), false, false, 0.0f, glm::vec4(0.0f, -10.0f, 50.0f, 5.0f));
    m_boxes.push_back(groundBox);

    ///Create a bunch of Boxes
    static std::mt19937 randGen{std::random_device{}()};
    std::uniform_real_distribution<float> xPos(-10.0f, 10.0f);
    std::uniform_real_distribution<float> yPos(-10.0f, 10.0f);
    std::uniform_real_distribution<float> rSize(0.5f, 2.5);
    std::uniform_int_distribution<int> rColor(0, 255);
    const int numBoxes = 10;

    for(int i = 0; i < numBoxes; i++){
        Box newBox;
        newBox.init(m_world.get(), glm::vec2(xPos(randGen),
                    yPos(randGen)), glm::vec2(rSize(randGen),
                    rSize(randGen)), m_texture, Bengine::ColorRGBA8(rColor(randGen), rColor(randGen), rColor(randGen), 255), false, true);
        m_boxes.push_back(newBox);
    }

    ///Initialize Spritebatch
    m_spriteBatch.init();

    ///Init shaders
    ///Texture shaders
    m_textureProgram.compileShaders("shaders/textureShading.vert", "shaders/textureShading.frag");
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();
    ///Light Shaders
    m_lightProgram.compileShaders("shaders/lightShading.vert", "shaders/lightConeShading.frag");
    m_lightProgram.addAttribute("vertexPosition");
    m_lightProgram.addAttribute("vertexColor");
    m_lightProgram.addAttribute("vertexUV");

    m_lightProgram.addAttribute("vertexMouse");

    m_lightProgram.linkShaders();



    ///Init camera
    m_camera.init(m_window->getScreenWidth(), m_window->getScreenHeight());
    m_camera.setScale(30.0f);

    ///Init player
    m_player.init(m_world.get(), glm::vec2(0.0f, 20.0f), glm::vec2(2.0f), glm::vec2(1.0f, 1.8f), Bengine::ColorRGBA8(255, 255, 255, 255));

    initUI();

}

void GameplayScreen::onExit(){
    m_debugRenderer.dispose();
    m_boxes.clear();
    m_world.reset();
}

///Called in the main game loop
void GameplayScreen::update(float deltaTime){
    checkInput();
    if (m_game->getIsRunning()) {

        m_camera.update();
        m_player.update(m_game->inputManager, deltaTime);


        ///Update physics simulation
        m_world->Step(1.0f / 60.0f * deltaTime, 6 * deltaTime, 2 * deltaTime);
        //m_world->Step(1.0f / 60.0f, 6, 2);
    }

    static int i = 0;
    i++;
    if(i > 50){
        i = 0;
        std::cout << m_game->getFps() << " " << m_game->getTotalDeltaTime() << std::endl;
    }
}

void GameplayScreen::draw(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    m_textureProgram.use();

    ///Upload texture uniform
    GLuint textureUniform = m_textureProgram.getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);
    glActiveTexture(GL_TEXTURE0);

    ///Upload camera uniform
    glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
    GLuint pUniform = m_textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    m_spriteBatch.begin();

    ///Draw Boxes
    for(auto& b : m_boxes){
        b.draw(m_spriteBatch);
    }

    ///Draw Player
    m_player.draw(m_spriteBatch, m_game->getTotalDeltaTime());

//    ///Draw ground
//    m_spriteBatch.draw(destRect,
//                       glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
//                       m_texture.id, 0.0f,
//                       Bengine::ColorRGBA8(255, 0, 0, 255),
//                       b.getBody()->GetAngle());

    m_spriteBatch.end();
    m_spriteBatch.renderBatch();
    m_textureProgram.unuse();

    ///Debug Rendering
    if(m_renderDebug){
        glm::vec4 destRect;
        for(auto& b : m_boxes){
            destRect.x = b.getBody()->GetPosition().x - b.getDimensions().x / 2.0f;
            destRect.y = b.getBody()->GetPosition().y - b.getDimensions().y / 2.0f;
            destRect.z = b.getDimensions().x;
            destRect.w = b.getDimensions().y;
            m_debugRenderer.drawBox(destRect, Bengine::ColorRGBA8(255, 255, 255, 255), b.getBody()->GetAngle());
        }
        m_player.drawDebug(m_debugRenderer);

        m_debugRenderer.end();
        m_debugRenderer.render(projectionMatrix, 2.0f);
    }

    if(m_player.getIsLightOn()){
        ///Render Lights
        static Light playerLight;
        playerLight.color = Bengine::ColorRGBA8(255, 255, 255, 128);
        playerLight.position = m_player.getPosition();
        playerLight.position.y += 0.70f;
        playerLight.position.x += 0.1f;
        playerLight.size = 30.0f;

    //    static Light mouseLight;
    //    mouseLight.color = Bengine::ColorRGBA8(255, 0, 255, 150);
    //    mouseLight.position = m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords());
    //    mouseLight.size = 45.0f;

        m_lightProgram.use();

        pUniform = m_lightProgram.getUniformLocation("P");
        glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

        ///Upload mouse coords uniform
        glm::vec2 mouseVec = glm::normalize(m_camera.convertScreenToWorld(m_game->inputManager.getMouseCoords()) - m_player.getPosition());
        //std::cout << mouseVec.x << " " << mouseVec.y << std::endl;
        GLuint mouseUniform = m_lightProgram.getUniformLocation("mouseVec");
        glUniform2f(mouseUniform, mouseVec.x, mouseVec.y);

        ///Additive blending
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        m_spriteBatch.begin();

        playerLight.draw(m_spriteBatch);
        //mouseLight.draw(m_spriteBatch);

        m_spriteBatch.end();
        m_spriteBatch.renderBatch();

        m_lightProgram.unuse();

        ///Reset to standard blending
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    }

    m_gui.draw();



}


void GameplayScreen::initUI(){
    ///Init the UI with the resource directory
    m_gui.init("GUI");
    m_gui.loadScheme("TaharezLook.scheme");
    m_gui.setFont("DejaVuSans-10");
    CEGUI::PushButton* testButton = static_cast<CEGUI::PushButton*>(m_gui.createWidget("TaharezLook/Button", glm::vec4(0.01f, 0.01f, 0.1f, 0.05f), glm::vec4(0.0f), "TestButton"));
    testButton->setText("Exit Game");

    testButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&GameplayScreen::onExitClicked, this));

    //CEGUI::Combobox* TestComboBox = static_cast<CEGUI::Combobox*>(m_gui.createWidget("TaharezLook/Combobox", glm::vec4(0.2f, 0.2f, 0.1f, 0.05f), glm::vec4(0.0f), "TestComboBox"));

    m_gui.setMouseCursor("TaharezLook/MouseArrow");
    m_gui.showMouseCursor();
    SDL_ShowCursor(0);
}

void GameplayScreen::checkInput(){
    SDL_Event evnt;
    while(SDL_PollEvent(&evnt)){
        m_game->onSDLEvent(evnt);
        m_gui.onSDLEvent(evnt);
    }
}

bool GameplayScreen::onExitClicked(const CEGUI::EventArgs& e)
{
    m_currentState = Bengine::ScreenState::EXIT_APPLICATION;

    return true;
}
