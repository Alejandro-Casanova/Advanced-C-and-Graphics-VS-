#include "pch.h"
#include <GLEW/glew.h> // Include BEFORE GUI.h

#include "GUI.h"
#include <SDL2/SDL_timer.h>
#include <iostream>

namespace Bengine{

CEGUI::OpenGL3Renderer* GUI::m_renderer = nullptr;

GUI::GUI()
{
    //ctor
}

GUI::~GUI()
{
    //dtor
}

void GUI::init(const std::string& resourceDirectory){
    ///Check if renderer and system are already initialized (renderer is static)
    if(m_renderer == nullptr){
        m_renderer = &CEGUI::OpenGL3Renderer::bootstrapSystem();
    }

    ///Set default resource directories
    CEGUI::DefaultResourceProvider* rp = static_cast<CEGUI::DefaultResourceProvider*>(CEGUI::System::getSingleton().getResourceProvider());
    rp->setResourceGroupDirectory("imagesets", resourceDirectory + "/imagesets/");
    rp->setResourceGroupDirectory("schemes", resourceDirectory + "/schemes/");
    rp->setResourceGroupDirectory("fonts", resourceDirectory + "/fonts/");
    rp->setResourceGroupDirectory("layouts", resourceDirectory + "/layouts/");
    rp->setResourceGroupDirectory("looknfeel", resourceDirectory + "/looknfeel/");
    rp->setResourceGroupDirectory("lua_scripts", resourceDirectory + "/lua_scripts/");

    CEGUI::ImageManager::setImagesetDefaultResourceGroup("imagesets");
    CEGUI::Scheme::setDefaultResourceGroup("schemes");
    CEGUI::Font::setDefaultResourceGroup("fonts");
    CEGUI::WindowManager::setDefaultResourceGroup("layouts");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("looknfeel");
    CEGUI::ScriptModule::setDefaultResourceGroup("lua_scripts");

    m_context = &CEGUI::System::getSingleton().createGUIContext(m_renderer->getDefaultRenderTarget());
    m_root = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root");
    m_context->setRootWindow(m_root);
}

void GUI::destroy(){
    CEGUI::System::getSingleton().destroyGUIContext(*m_context);
    CEGUI::WindowManager::getSingleton().destroyWindow(m_root);
    m_context = nullptr;
    m_root = nullptr;
}

void GUI::draw(){
    glDisable(GL_DEPTH_TEST);
    m_renderer->beginRendering();
    m_context->draw();
    m_renderer->endRendering();
    // Clean up after CEGUI
    glBindVertexArray(0);
    glDisable(GL_SCISSOR_TEST); ///< Prevents bug that occurs when rendering buttons
    glEnable(GL_BLEND); ///< Prevents light rendering bug
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);///< Reactivates alpha blending
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void GUI::update(){
    unsigned int elapsed;
    if (m_lastTime == 0) {
        elapsed = 0;
        m_lastTime = SDL_GetTicks();
    }
    else {
        unsigned int nextTime = SDL_GetTicks();
        elapsed = nextTime - m_lastTime;
        m_lastTime = nextTime;
    }
    
    m_context->injectTimePulse((float)elapsed / 1000.0f);
}

void GUI::setMouseCursor(const std::string& imageFile){
    //m_context->getMouseCursor().setDefaultImage(imageFile);
    m_context->getMouseCursor().setDefaultImage(imageFile);
}

void GUI::showMouseCursor(){
    //m_context->getMouseCursor().show();
    m_context->getMouseCursor().show();
}

void GUI::hideMouseCursor(){
    //m_context->getMouseCursor().hide();
    m_context->getMouseCursor().hide();
}

}

CEGUI::Key::Scan SDLKeyToCEGUIKey(SDL_Keycode key) {
    using namespace CEGUI;
    switch (key) {
        case SDLK_BACKSPACE:    return Key::Scan::Backspace;
        case SDLK_TAB:          return Key::Scan::Tab;
        case SDLK_RETURN:       return Key::Scan::Return;
        case SDLK_PAUSE:        return Key::Scan::Pause;
        case SDLK_ESCAPE:       return Key::Scan::Escape;
        case SDLK_SPACE:        return Key::Scan::Space;
        case SDLK_COMMA:        return Key::Scan::Comma;
        case SDLK_MINUS:        return Key::Scan::Minus;
        case SDLK_PERIOD:       return Key::Scan::Period;
        case SDLK_SLASH:        return Key::Scan::Slash;
        case SDLK_0:            return Key::Scan::Zero;
        case SDLK_1:            return Key::Scan::One;
        case SDLK_2:            return Key::Scan::Two;
        case SDLK_3:            return Key::Scan::Three;
        case SDLK_4:            return Key::Scan::Four;
        case SDLK_5:            return Key::Scan::Five;
        case SDLK_6:            return Key::Scan::Six;
        case SDLK_7:            return Key::Scan::Seven;
        case SDLK_8:            return Key::Scan::Eight;
        case SDLK_9:            return Key::Scan::Nine;
        case SDLK_COLON:        return Key::Scan::Colon;
        case SDLK_SEMICOLON:    return Key::Scan::Semicolon;
        case SDLK_EQUALS:       return Key::Scan::Equals;
        case SDLK_LEFTBRACKET:  return Key::Scan::LeftBracket;
        case SDLK_BACKSLASH:    return Key::Scan::Backslash;
        case SDLK_RIGHTBRACKET: return Key::Scan::RightBracket;
        case SDLK_a:            return Key::Scan::A;
        case SDLK_b:            return Key::Scan::B;
        case SDLK_c:            return Key::Scan::C;
        case SDLK_d:            return Key::Scan::D;
        case SDLK_e:            return Key::Scan::E;
        case SDLK_f:            return Key::Scan::F;
        case SDLK_g:            return Key::Scan::G;
        case SDLK_h:            return Key::Scan::H;
        case SDLK_i:            return Key::Scan::I;
        case SDLK_j:            return Key::Scan::J;
        case SDLK_k:            return Key::Scan::K;
        case SDLK_l:            return Key::Scan::L;
        case SDLK_m:            return Key::Scan::M;
        case SDLK_n:            return Key::Scan::N;
        case SDLK_o:            return Key::Scan::O;
        case SDLK_p:            return Key::Scan::P;
        case SDLK_q:            return Key::Scan::Q;
        case SDLK_r:            return Key::Scan::R;
        case SDLK_s:            return Key::Scan::S;
        case SDLK_t:            return Key::Scan::T;
        case SDLK_u:            return Key::Scan::U;
        case SDLK_v:            return Key::Scan::V;
        case SDLK_w:            return Key::Scan::W;
        case SDLK_x:            return Key::Scan::X;
        case SDLK_y:            return Key::Scan::Y;
        case SDLK_z:            return Key::Scan::Z;
        case SDLK_DELETE:       return Key::Scan::Delete;
        case SDLK_KP_PERIOD:    return Key::Scan::Decimal;
        case SDLK_KP_DIVIDE:    return Key::Scan::Divide;
        case SDLK_KP_MULTIPLY:  return Key::Scan::Multiply;
        case SDLK_KP_MINUS:     return Key::Scan::Subtract;
        case SDLK_KP_PLUS:      return Key::Scan::Add;
        case SDLK_KP_ENTER:     return Key::Scan::NumpadEnter;
        case SDLK_KP_EQUALS:    return Key::Scan::NumpadEquals;
        case SDLK_UP:           return Key::Scan::ArrowUp;
        case SDLK_DOWN:         return Key::Scan::ArrowDown;
        case SDLK_RIGHT:        return Key::Scan::ArrowRight;
        case SDLK_LEFT:         return Key::Scan::ArrowLeft;
        case SDLK_INSERT:       return Key::Scan::Insert;
        case SDLK_HOME:         return Key::Scan::Home;
        case SDLK_END:          return Key::Scan::End;
        case SDLK_PAGEUP:       return Key::Scan::PageUp;
        case SDLK_PAGEDOWN:     return Key::Scan::PageDown;
        case SDLK_F1:           return Key::Scan::F1;
        case SDLK_F2:           return Key::Scan::F2;
        case SDLK_F3:           return Key::Scan::F3;
        case SDLK_F4:           return Key::Scan::F4;
        case SDLK_F5:           return Key::Scan::F5;
        case SDLK_F6:           return Key::Scan::F6;
        case SDLK_F7:           return Key::Scan::F7;
        case SDLK_F8:           return Key::Scan::F8;
        case SDLK_F9:           return Key::Scan::F9;
        case SDLK_F10:          return Key::Scan::F10;
        case SDLK_F11:          return Key::Scan::F11;
        case SDLK_F12:          return Key::Scan::F12;
        case SDLK_F13:          return Key::Scan::F13;
        case SDLK_F14:          return Key::Scan::F14;
        case SDLK_F15:          return Key::Scan::F15;
        case SDLK_RSHIFT:       return Key::Scan::RightShift;
        case SDLK_LSHIFT:       return Key::Scan::LeftShift;
        case SDLK_RCTRL:        return Key::Scan::RightControl;
        case SDLK_LCTRL:        return Key::Scan::LeftControl;
        case SDLK_RALT:         return Key::Scan::RightAlt;
        case SDLK_LALT:         return Key::Scan::LeftAlt;
        case SDLK_SYSREQ:       return Key::Scan::SysRq;
        case SDLK_MENU:         return Key::Scan::AppMenu;
        case SDLK_POWER:        return Key::Scan::Power;
        default:                return Key::Scan::Unknown;
    }
}

CEGUI::MouseButton SDLButtonToCEGUIButton(Uint8 sdlButton){
    switch(sdlButton){
        case SDL_BUTTON_LEFT: return CEGUI::MouseButton::LeftButton;
        case SDL_BUTTON_MIDDLE: return CEGUI::MouseButton::MiddleButton;
        case SDL_BUTTON_RIGHT: return CEGUI::MouseButton::RightButton;
        case SDL_BUTTON_X1: return CEGUI::MouseButton::X1Button;
        case SDL_BUTTON_X2: return CEGUI::MouseButton::X2Button;
    }
    return CEGUI::MouseButton::NoButton;
}

namespace Bengine{

void GUI::onSDLEvent(SDL_Event& evnt){
    //CEGUI::utf32 codePoint;
    CEGUI::String cs;

    switch(evnt.type){
    case SDL_MOUSEMOTION:
        // m_context->injectMouseMove(evnt.motion.xrel, evnt.motion.yrel);
        m_context->injectMousePosition((float)evnt.motion.x, (float)evnt.motion.y);
        break;
    case SDL_KEYDOWN:
        m_context->injectKeyDown(SDLKeyToCEGUIKey(evnt.key.keysym.sym));
        break;
    case SDL_KEYUP:
        m_context->injectKeyUp(SDLKeyToCEGUIKey(evnt.key.keysym.sym));
        break;
    case SDL_TEXTINPUT:
        ///Old code. It worked, but it wasn't correct
        /*codePoint = 0;
        for (int i = 0; evnt.text.text[i] != '\0'; i++) {
            codePoint |= (((CEGUI::utf32)evnt.text.text[i]) << (i * 8)); ///< On each iteration a byte is added shifted to the left
        m_context->injectChar(codePoint);
        }*/
        cs = CEGUI::String((CEGUI::utf8*)evnt.text.text);
        for (int i = 0; i < cs.size(); ++i) {
            m_context->injectChar(cs[i]);
        }
        break;
    case SDL_MOUSEBUTTONUP:
        m_context->injectMouseButtonUp(SDLButtonToCEGUIButton(evnt.button.button));
        break;
    case SDL_MOUSEBUTTONDOWN:
        m_context->injectMouseButtonDown(SDLButtonToCEGUIButton(evnt.button.button));
        break;
    default:
        break;
    }
}

void GUI::loadScheme(const std::string& schemeFile){
    CEGUI::SchemeManager::getSingleton().createFromFile(schemeFile);
}

CEGUI::Window* GUI::createWidget(const std::string& type, const glm::vec4& destRectPercentage, const glm::vec4& destRectPixels, const std::string& name /* = "" */){
    CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton().createWindow(type, name);
    m_root->addChild(newWindow);
    setWidgetDestRect(newWindow, destRectPercentage, destRectPixels);
    return newWindow;
}

CEGUI::Window* Bengine::GUI::createWidget(CEGUI::Window* parent, const std::string& type, const glm::vec4& destRectPerc, const glm::vec4& destRectPix, const std::string& name /*= ""*/) {
    CEGUI::Window* newWindow = CEGUI::WindowManager::getSingleton().createWindow(type, name);
    parent->addChild(newWindow);
    setWidgetDestRect(newWindow, destRectPerc, destRectPix);
    return newWindow;
}

void GUI::setWidgetDestRect(CEGUI::Window* widget, const glm::vec4& destRectPercentage, const glm::vec4& destRectPixels){

    widget->setPosition(CEGUI::UVector2(CEGUI::UDim(destRectPercentage.x, destRectPixels.x),
                                           CEGUI::UDim(destRectPercentage.y, destRectPixels.y)));

    widget->setSize(CEGUI::USize(CEGUI::UDim(destRectPercentage.z, destRectPixels.z),
                                       CEGUI::UDim(destRectPercentage.w, destRectPixels.w)));
}

void GUI::setFont(const std::string& fontFile){
    CEGUI::FontManager::getSingleton().createFromFile(fontFile + ".font");
    m_context->setDefaultFont(fontFile);
}



}
