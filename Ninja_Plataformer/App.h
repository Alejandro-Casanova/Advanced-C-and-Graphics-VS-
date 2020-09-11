#ifndef APP_H
#define APP_H

#include <Bengine/IMainGame.h>

#include "GameplayScreen.h"
#include "MainMenuScreen.h"
#include "EditorScreen.h"

class App : public Bengine::IMainGame{
    public:
        App();
        virtual ~App();

        virtual void onInit() override; ///< Called on initialization
        virtual void addScreens() override;
        virtual void onExit() override; ///< Called on exiting

    private:
        std::unique_ptr<GameplayScreen> m_gameplayScreen = nullptr;
        std::unique_ptr<MainMenuScreen> m_mainMenuScreen = nullptr;
        std::unique_ptr<EditorScreen> m_editorScreen = nullptr;
        
};

#endif // APP_H
