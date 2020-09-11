#ifndef GUI_H
#define GUI_H

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/OpenGL/GL3Renderer.h>

#include <glm/glm.hpp>
#include <SDL2/SDL_events.h>

namespace Bengine{

class GUI
{
    public:
        GUI();
        virtual ~GUI();

        void init(const std::string& resourceDirectory);
        void destroy();

        void draw();
        void update();

        void setMouseCursor(const std::string& imageFile);
        void showMouseCursor();
        void hideMouseCursor();

        void onSDLEvent(SDL_Event& evnt);

        void loadScheme(const std::string& schemeFile);
        void setFont(const std::string& fontFile);
        ///Create window takes a relative destRect (position and dimentions in percentage) and an absolute destRect (position and dimention offset pixrls)
        CEGUI::Window* createWidget(const std::string& type, const glm::vec4& destRectPercentage, const glm::vec4& destRectPixels, const std::string& name = "");
        CEGUI::Window* createWidget(CEGUI::Window* parent, const std::string& type, const glm::vec4& destRectPerc, const glm::vec4& destRectPix, const std::string& name = "");
        static void setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPercentage, const glm::vec4& destRectPixels);

        static CEGUI::OpenGL3Renderer* getRenderer() { return m_renderer; }
        const CEGUI::GUIContext* getContext() { return m_context; }

    private:
        static CEGUI::OpenGL3Renderer* m_renderer;
        CEGUI::GUIContext* m_context = nullptr;
        CEGUI::Window* m_root = nullptr;
        CEGUI::InjectedInputReceiver* m_inputReceiver = nullptr;
        unsigned int m_lastTime = 0;

};

}

#endif // GUI_H
