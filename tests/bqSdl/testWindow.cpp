#include <bq/sdl/Color.hpp>
#include <bq/sdl/Renderer.hpp>
#include <bq/sdl/Window.hpp>
#include <format>
#include <gtest/gtest.h>
namespace
{
  using namespace bq::sdl;

  Window comWin{nullptr, Window::null_deleter};
  Renderer comRen{};

  class testWindow : public ::testing::Test
  {
  protected:
    void SetUp() override
    {
      if (!comWin)
      {
        printf("Creating common window.\n");
        comWin = Window("Common Window", 320, 240, SDL_WINDOW_NOT_FOCUSABLE);
        comWin.setPosition(100, 100);
        comRen = Renderer(comWin);
      }
      else
      {
        printf("Common window already exists.\n");
      }
      auto cti = ::testing::UnitTest::GetInstance()->current_test_info();
      auto curTstSuite     = cti->test_suite_name();
      auto curTstName      = cti->name();
      auto currentTestName = std::format("{}.{}", curTstSuite, curTstName);
      comWin.setTitle(std::format("{}.{}", curTstSuite, curTstName).c_str());
      comRen.setDrawColorInt(colors::SOLID_BLACK);
      comRen.clear();
      comRen.present();
      // SDL_Delay(1000);
    };
  };
#define STEP_DELAY SDL_Delay(10)

#define PUMP_EVENTS()                                                          \
  {                                                                            \
    SDL_Event e;                                                               \
    while (SDL_PollEvent(&e))                                                  \
    {                                                                          \
      printf(".");                                                             \
    }                                                                          \
  }

#define SYNC_STEP(msg, wl...)                                                  \
  {                                                                            \
    printf("%s", msg);                                                         \
    PUMP_EVENTS();                                                             \
    STEP_DELAY;                                                                \
    for (auto w : std::vector<Window>{wl})                                     \
    {                                                                          \
      w.sync();                                                                \
    }                                                                          \
    printf("\n");                                                              \
  }

  // SDL_ClaimWindowForGPUDevice will be covered by some GPU unit test.

  TEST_F(testWindow, CreatePopupWindow)
  {
    {
      Window w = Window("Parent Window");
      SYNC_STEP("Created a parent window.", w);
      Window popupTooltip = w.createPopup(
          10, 10, 320, 240, SDL_WINDOW_RESIZABLE | SDL_WINDOW_TOOLTIP);
      SYNC_STEP("Created popup tooltip window.", w, popupTooltip);
      ASSERT_TRUE(popupTooltip);
      Window popupMenu =
          w.createPopup(100, 200, 100, 100, SDL_WINDOW_POPUP_MENU);
      SYNC_STEP("Created popup menu window.", w, popupMenu);
      ASSERT_TRUE(popupMenu);
      //
      ASSERT_THROW(
          popupTooltip.createPopup(10, 10, 320, 240, SDL_WINDOW_RESIZABLE),
          std::runtime_error);
      SYNC_STEP(
          "Attempted to create a popup window without specifying a required "
          "flag.");
    }
  }

  // CreateWindowAndRendrer will be covered by the RenderingApplication? unit
  // test.

  TEST_F(testWindow, CreateWindow)
  {
    Properties p;
    {
      Window w = Window("CreateWindow");
      SYNC_STEP("Created window.", w);
      ASSERT_TRUE(w);
      p = w.getProperties();
      for (auto &[k, v] : p.enumerate())
      {
        auto vstr = p.getAsStdString(k);
        printf("Property %s: %s\n", k, vstr.c_str());
      }
    }
    SYNC_STEP("Destroyed window.");
  }

  TEST_F(testWindow, CreateWindowWithPropertiesAlwaysOnTopTrue)
  {
    {
      Properties p{Properties::Create()};
      p.setString(SDL_PROP_WINDOW_CREATE_TITLE_STRING,
                  "CreatedWindowWithPropertiesAlwaysOnTopTrue");
      p.setBoolean(SDL_PROP_WINDOW_CREATE_ALWAYS_ON_TOP_BOOLEAN, true);
      p.setBoolean(SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, false);
      Window w = Window(p);
      SYNC_STEP("Created window.", w);
      ASSERT_TRUE(w);
      Renderer r = Renderer(w);
      ASSERT_TRUE(r);
      SYNC_STEP("Created renderer.", w);
      SDL_Delay(5000);
    }
  }

  TEST_F(testWindow, CreateWindowWithPropertiesBorderlessTrue)
  {
    {
      Properties p{Properties::Create()};
      p.setString(SDL_PROP_WINDOW_CREATE_TITLE_STRING,
                  "CreatedWindowWithPropertiesBorderlessTrue");
      p.setBoolean(SDL_PROP_WINDOW_CREATE_BORDERLESS_BOOLEAN, true);
      Window w = Window("CreatedWindowWithPropertiesBorderlessTrue", p);
      SYNC_STEP("Created Borderless window", w);
      ASSERT_TRUE(w);
      Renderer r = Renderer(w);
      ASSERT_TRUE(r);
      SYNC_STEP("Created renderer.", w);
      r.clear();
      r.present();
      SDL_Delay(1000);
    }
  }

  TEST_F(testWindow, CreateWindowWithPropertiesFullscreenTrue)
  {
    {
      Properties p{Properties::Create()};
      p.setString(SDL_PROP_WINDOW_CREATE_TITLE_STRING,
                  "CreatedWindowWithPropertiesFullscreenTrue");
      p.setBoolean(SDL_PROP_WINDOW_CREATE_FULLSCREEN_BOOLEAN, true);
      Window w = Window(p);
      SYNC_STEP("Created window.", w);
      ASSERT_TRUE(w);
      Renderer r = Renderer(w);
      ASSERT_TRUE(r);
      SYNC_STEP("Created renderer.", w);
      r.clear();
      r.present();
      SDL_Delay(1000);
    }
  }

  TEST_F(testWindow, CreateWindowWithPropertiesHpdTrue)
  {

    {
      Properties p{Properties::Create()};
      p.setString(SDL_PROP_WINDOW_CREATE_TITLE_STRING,
                  "CreatedWindowWithPropertiesHpdTrue");
      p.setBoolean(SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, false);
      p.setBoolean(SDL_PROP_WINDOW_CREATE_HIGH_PIXEL_DENSITY_BOOLEAN, true);
      Window w = Window(p);
      SYNC_STEP("Created window.", w);
      ASSERT_TRUE(w);
      Renderer r = Renderer(w);
      ASSERT_TRUE(r);
      SYNC_STEP("Created renderer.", w);
      ASSERT_TRUE(r.clear());
      ASSERT_TRUE(r.present());
      SDL_Delay(1000);
    }
  }

  TEST_F(testWindow, CreateWindowWithPropertiesParentPointer)
  {

    {
      Properties p{Properties::Create()};
      p.setString(SDL_PROP_WINDOW_CREATE_TITLE_STRING,
                  "CreatedWindowWithPropertiesParentPointer");
      p.setBoolean(SDL_PROP_WINDOW_CREATE_HIDDEN_BOOLEAN, false);
      p.setPointer(SDL_PROP_WINDOW_CREATE_PARENT_POINTER,
                   comWin.resource.get());
    }
  }

  TEST_F(testWindow, CreateWindowWithPropertiesResizeableTrue)
  {

    {
      Properties p{Properties::Create()};
      p.setString(SDL_PROP_WINDOW_CREATE_TITLE_STRING,
                  "CreatedWindowWithPropertiesResizeableTrue");
      p.setBoolean(SDL_PROP_WINDOW_CREATE_RESIZABLE_BOOLEAN, true);
      Window w = Window(p);
      SYNC_STEP("Created window.", w);
      ASSERT_TRUE(w);
      Renderer r = Renderer(w);
      ASSERT_TRUE(r);
      SYNC_STEP("Created renderer.", w);
      r.clear();
      r.present();
      w.show();
      SDL_Delay(1000);
    }
  }

  TEST_F(testWindow, flash)
  {
    {
      Window w = Window("Flash Window");
      SYNC_STEP("Created window.", w);
      ASSERT_TRUE(w.hide());
      SYNC_STEP("Hidden window.", w);
      ASSERT_TRUE(w.flash(SDL_FLASH_BRIEFLY));
      SYNC_STEP("Flashing window.", w);
      SDL_Delay(1000);
      SYNC_STEP("Flashed window.", w);
    }
  }

  TEST_F(testWindow, getDisplay)
  {
    SDL_DisplayID id = comWin.getDisplayID();
    ASSERT_TRUE(id != 0);
  }
  /** @todo was trying to test this, but now will just call and quit, remove
   * code */
  TEST_F(testWindow, GetGrabbed)
  {
    ASSERT_FALSE(Window::GetGrabbed());
    // comWin.setKeyboardGrab(true);
    // comWin.setMouseGrab(true);
    // STEP_DELAY;
    // ASSERT_TRUE(Window::GetGrabbed());
    // comWin.setKeyboardGrab(false);
    // comWin.setMouseGrab(false);
    // STEP_DELAY;
    // ASSERT_FALSE(Window::GetGrabbed());
  }

  TEST_F(testWindow, getRenderer)
  {
    ASSERT_TRUE(comWin.getRenderer());
  }

  TEST_F(testWindow, getAspect)
  {
    float w{0}, h{1};
    STEP_DELAY;

    ASSERT_TRUE(comWin.getAspectRatio(&w, &h));
    SDL_Log("w: %f, h: %f", w, h);
    // There is no "correct" value...
    // ASSERT_FLOAT_EQ(w / h, (float)4 / (float)3);
  }

  TEST_F(testWindow, getBorderSize)
  {
    int t, l, b, r;
    ASSERT_TRUE(comWin.getBorderSize(&t, &l, &b, &r));
    // ASSERT_EQ(t, 0);
    // ASSERT_EQ(l, 0);
    // ASSERT_EQ(b, 0);
    // ASSERT_EQ(r, 0);
  }

  TEST_F(testWindow, getDisplayScale)
  {
    float scale = comWin.getDisplayScale();
    ASSERT_TRUE(scale > 0);
  }

  TEST_F(testWindow, getFlags)
  {
    SDL_WindowFlags flags = comWin.getFlags();
    ASSERT_TRUE(flags != 0);
  }

  TEST_F(testWindow, GetFromEvent)
  {
    SDL_Event e;
    e.window.windowID = comWin.getID();
    e.type            = SDL_EVENT_WINDOW_SHOWN;
    Window w          = Window::GetFromEvent(&e);
    ASSERT_TRUE(w);
    ASSERT_EQ(w.getID(), comWin.getID());
  }

  TEST_F(testWindow, GetFromID)
  {
    SDL_WindowID id = comWin.getID();
    Window w        = Window::GetFromID(id);
    ASSERT_TRUE(w);
    ASSERT_EQ(w.getID(), id);
  }

  TEST_F(testWindow, getFullscreenMode)
  {
    const SDL_DisplayMode *mode = comWin.getFullscreenMode();
    SDL_Log("Fullscreen mode: %p", mode);
    // ASSERT_TRUE(mode);
  }
  /** @todo no testing of ICC profiles performed */
  TEST_F(testWindow, getICCProfile)
  {
    // SDL_Delay(100);
    size_t size;
    void *profile = comWin.getICCProfile(&size);
    // ASSERT_TRUE(profile);
    // ASSERT_TRUE(size > 0);
    if (profile)
    {
      SDL_free(profile);
    }
  }

  TEST_F(testWindow, getID)
  {
    SDL_WindowID id = comWin.getID();
    ASSERT_TRUE(id != 0);
  }

  TEST_F(testWindow, getKeyboardGrab)
  {
    ASSERT_FALSE(comWin.getKeyboardGrab());
    comWin.setKeyboardGrab(true);
    ASSERT_TRUE(comWin.getKeyboardGrab());
    comWin.setKeyboardGrab(false);
    ASSERT_FALSE(comWin.getKeyboardGrab());
  }

  TEST_F(testWindow, getMaximumSize)
  {
    int w, h;
    ASSERT_TRUE(comWin.getMaximumSize(&w, &h));
    ASSERT_TRUE(w > 0);
    ASSERT_TRUE(h > 0);
  }

  TEST_F(testWindow, getMinimumSize)
  {
    int w, h;
    ASSERT_TRUE(comWin.getMinimumSize(&w, &h));
    ASSERT_TRUE(w > 0);
    ASSERT_TRUE(h > 0);
  }

  TEST_F(testWindow, getMouseGrab)
  {
    ASSERT_FALSE(comWin.getMouseGrab());
    comWin.setMouseGrab(true);
    ASSERT_TRUE(comWin.getMouseGrab());
    comWin.setMouseGrab(false);
    ASSERT_FALSE(comWin.getMouseGrab());
  }

  TEST_F(testWindow, getMouseRect)
  {
    const SDL_Rect *rect = comWin.getMouseRect();
    ASSERT_FALSE(rect);
  }

  TEST_F(testWindow, getOpacity)
  {
    float opacity = comWin.getOpacity();
    ASSERT_TRUE(opacity == 1);
    comWin.setOpacity(0.5);
    ASSERT_TRUE(opacity >= 0);
    ASSERT_TRUE(opacity <= 1);
    comWin.setOpacity(1);
  }

  TEST_F(testWindow, getParent)
  {
    Window w = Window("Parent Window");
    SYNC_STEP("Created parent window.", w);
    Window c = w.createPopup(10, 10, 320, 240, SDL_WINDOW_RESIZABLE);
    SYNC_STEP("Created child window.", w, c);
    ASSERT_TRUE(c.getParent().resource.get() == w.resource.get());
  }

  TEST_F(testWindow, getPixelDensity)
  {
    float density = comWin.getPixelDensity();
    ASSERT_TRUE(density > 0);
  }

  TEST_F(testWindow, getPixelFormat)
  {
    SDL_PixelFormat fmt = comWin.getPixelFormat();
    ASSERT_TRUE(fmt != SDL_PIXELFORMAT_UNKNOWN);
  }

  TEST_F(testWindow, getPosition)
  {
    int x, y;
    ASSERT_TRUE(comWin.getPosition(&x, &y));
    ASSERT_TRUE(x >= 0);
    ASSERT_TRUE(y >= 0);
  }

  TEST_F(testWindow, getProperties)
  {
    SDL_PropertiesID pID = comWin.getPropertiesID();
    ASSERT_TRUE(pID != 0);

    Properties p = comWin.getProperties();
    ASSERT_TRUE(p);
    std::cout << "testWindow properties: " << p << std::endl;
  }

  TEST_F(testWindow, getRelativeMouseMode)
  {
    ASSERT_FALSE(comWin.getRelativeMouseMode());
    comWin.setRelativeMouseMode(true);
    ASSERT_TRUE(comWin.getRelativeMouseMode());
    comWin.setRelativeMouseMode(false);
    ASSERT_FALSE(comWin.getRelativeMouseMode());
  }

  TEST_F(testWindow, GetWindows)
  {
    ASSERT_EQ(Window::GetWindows().size(), 1);
    {
      Window w2 = Window("GetWindows#2");
      SYNC_STEP("Created window.", w2);
      ASSERT_EQ(Window::GetWindows().size(), 2);
    }
    SYNC_STEP("Destroyed window.");
    ASSERT_EQ(Window::GetWindows().size(), 1);
  }

  TEST_F(testWindow, getSafeArea)
  {
    SDL_Rect rect;
    ASSERT_TRUE(comWin.getSafeArea(&rect));
    ASSERT_TRUE(rect.w > 0);
    ASSERT_TRUE(rect.h > 0);
  }

  TEST_F(testWindow, getSize)
  {
    int w, h;
    ASSERT_TRUE(comWin.getSize(&w, &h));
    ASSERT_TRUE(w > 0);
    ASSERT_TRUE(h > 0);
  }

  TEST_F(testWindow, getSizeInPixels)
  {
    int w, h;
    ASSERT_TRUE(comWin.getSizeInPixels(&w, &h));
    ASSERT_TRUE(w > 0);
    ASSERT_TRUE(h > 0);
  }

  TEST_F(testWindow, getSurface)
  {
    ASSERT_FALSE(comWin.hasSurface());
    ASSERT_TRUE(comWin.getSurface());
    ASSERT_TRUE(comWin.hasSurface());
  }

  TEST_F(testWindow, getSurfaceVSync)
  {
    int a_Interval = 0;
    comWin.setSurfaceVSync(1000);
    ASSERT_TRUE(comWin.getSurfaceVSync(&a_Interval));
    ASSERT_EQ(a_Interval, 1000);
    ASSERT_EQ(comWin.getSurfaceVSync(), 1000);

    comWin.setSurfaceVSync(0);
    ASSERT_EQ(a_Interval, 0);
    ASSERT_FALSE(comWin.getSurfaceVSync(&a_Interval));
    ASSERT_EQ(comWin.getSurfaceVSync(), 0);
  }

  TEST_F(testWindow, getTitle)
  {
    comWin.setTitle("Title Test 1");
    comWin.sync();
    ASSERT_STREQ(comWin.getTitle(), "Common Window");
  }

  TEST_F(testWindow, hide)
  {
    Window shownWindow("About to hide this window");
    SYNC_STEP("Created window.", shownWindow);
    ASSERT_TRUE(shownWindow.hide());
    SYNC_STEP("Hidden window.", shownWindow);
  }

  TEST_F(testWindow, maximize)
  {
    Window w = Window("About to maximize this window");
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.maximize());
    SYNC_STEP("Maximized window.", w);
    /** @todo isMaximized()? */
  }

  TEST_F(testWindow, minimize)
  {
    Window w = Window("About to minimize this window");
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.minimize());
    SYNC_STEP("Minimized window.", w);
    /** @todo isMinimized()? */
  }

  TEST_F(testWindow, raise)
  {
    Window w =
        Window("About to raise this window", 320, 240, SDL_WINDOW_HIDDEN);
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.raise());
    SYNC_STEP("Raised window.", w);
  }

  TEST_F(testWindow, restore)
  {
    Window w = Window("About to restore this window");
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.minimize());
    SYNC_STEP("Minimized window.", w);
    ASSERT_TRUE(w.isMinimized());
    ASSERT_TRUE(w.restore());
    SYNC_STEP("Restored window.", w);
    ASSERT_FALSE(w.isMinimized());
  }

  TEST_F(testWindow, setAlwaysOnTop)
  {
    Window w = Window("About to setAlwaysOnTop this window");
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.setAlwaysOnTop(true));
    ASSERT_TRUE(w.isAlwaysOnTop());
    SYNC_STEP("Set AlwaysOnTop window.", w);
    ASSERT_TRUE(w.setAlwaysOnTop(false));
    SYNC_STEP("Unset AlwaysOnTop window.", w);
    ASSERT_FALSE(w.isAlwaysOnTop());
  }

  TEST_F(testWindow, setAspectRatio)
  {
    Window w = Window("About to setAspectRatio this window");
    // float a_W, a_H;
    // w.getAspectRatio(&a_W, &a_H);
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.setAspectRatio(16, 9));
    SYNC_STEP("Set AspectRatio window.", w);
  }

  TEST_F(testWindow, setBordered)
  {
    Window w = Window("About to setBordered this window");
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.setBordered(true));
    SYNC_STEP("Set Bordered window.", w);
    ASSERT_TRUE(w.setBordered(false));
    SYNC_STEP("Unset Bordered window.", w);
  }

  TEST_F(testWindow, setFocusable)
  {
    Window wYf = Window("This was focusable", 320, 240),
           wNf = Window("This was not focusable", 320, 240,
                        SDL_WINDOW_NOT_FOCUSABLE);
    wYf.setPosition(100, 400);
    wNf.setPosition(500, 400);
    SYNC_STEP("Created windows.", wYf, wNf);
    SDL_Delay(1000);
    ASSERT_FALSE(wYf.isNotFocusable());
    ASSERT_TRUE(wYf.isFocusable());
    ASSERT_TRUE(wNf.isNotFocusable());
    ASSERT_FALSE(wNf.isFocusable());
    SYNC_STEP("Tested values", wYf, wNf);
    SDL_Delay(1000);
    ASSERT_TRUE(wNf.setFocusable(true));
    ASSERT_TRUE(wYf.setFocusable(false));
    SYNC_STEP("Changed values.", wYf, wNf);
    SDL_Delay(1000);
    ASSERT_TRUE(wNf.isFocusable());
    ASSERT_FALSE(wNf.isNotFocusable());
    ASSERT_TRUE(wYf.isNotFocusable());
    ASSERT_FALSE(wYf.isFocusable());
    SYNC_STEP("Tested changed values.", wYf, wNf);
    SDL_Delay(1000);
  }

  TEST_F(testWindow, setFullscreen)
  {
    Window w = Window("About to setFullscreen this window");
    ASSERT_FALSE(w.isFullscreen());
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.setFullscreen(true));
    SYNC_STEP("Set Fullscreen window.", w);
    ASSERT_TRUE(w.isFullscreen());

    ASSERT_TRUE(w.setFullscreen(false));
    SYNC_STEP("Unset Fullscreen window.", w);
    ASSERT_FALSE(w.isFullscreen());
  }

  TEST_F(testWindow, setHitTest)
  {
    Window w = Window("About to setHitTest this window");
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.setHitTest(
        [](SDL_Window *win, const SDL_Point *p, void *d) {
          if (p)
          {
            printf("Window hit test at %d, %d\n", p->x, p->y);
          }
          return SDL_HITTEST_DRAGGABLE;
        },
        nullptr));
    SYNC_STEP("Set WindowHitTest window.", w);
  }

  TEST_F(testWindow, setIcon)
  {
    Surface icon = Surface::Load("resources/images/plug.xcf");
    ASSERT_TRUE(icon);
    Window w = Window("About to setIcon this window");
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.setIcon(icon));
    SYNC_STEP("Set Icon window.", w);
    SDL_Delay(1000);
  }

  TEST_F(testWindow, setKeyboardGrab)
  {
    Window w = Window("About to setKeyboardGrab this window");
    SYNC_STEP("Created window.", w);
    ASSERT_FALSE(w.isKeyboardGrabbed());
    ASSERT_TRUE(w.setKeyboardGrab(true));
    SYNC_STEP("Set KeyboardGrab window.", w);
    ASSERT_TRUE(w.isKeyboardGrabbed());
    ASSERT_TRUE(w.setKeyboardGrab(false));
    SYNC_STEP("Unset KeyboardGrab window.", w);
    ASSERT_FALSE(w.isKeyboardGrabbed());
  }

  TEST_F(testWindow, setMaximumSize)
  {
    Window w = Window("About to setMaximumSize this window");
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.setMaximumSize(640, 480));
    SYNC_STEP("Set MaximumSize window.", w);
  }

  TEST_F(testWindow, setMinimumSize)
  {
    Window w = Window("About to setMinimumSize this window");
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.setMinimumSize(320, 240));
    SYNC_STEP("Set MinimumSize window.", w);
  }

  TEST_F(testWindow, setModal)
  {
    Window w = Window("About to setModal this window");
    SYNC_STEP("Created window.", w);
    ASSERT_FALSE(w.isModal());
    ASSERT_TRUE(w.setModal(true));
    SYNC_STEP("Set Modal window.", w);
    ASSERT_TRUE(w.isModal());
    ASSERT_TRUE(w.setModal(false));
    SYNC_STEP("Unset Modal window.", w);
    ASSERT_FALSE(w.isModal());
  }

  TEST_F(testWindow, setMouseGrab)
  {
    Window w = Window("About to setMouseGrab this window");
    SYNC_STEP("Created window.", w);
    ASSERT_FALSE(w.isMouseGrabbed());
    ASSERT_TRUE(w.setMouseGrab(true));
    SYNC_STEP("Set MouseGrab window.", w);
    ASSERT_TRUE(w.isMouseGrabbed());
    ASSERT_TRUE(w.setMouseGrab(false));
    SYNC_STEP("Unset MouseGrab window.", w);
    ASSERT_FALSE(w.isMouseGrabbed());
  }

  TEST_F(testWindow, setMouseRect)
  {
    Window w = Window("About to setMouseRect this window");
    SYNC_STEP("Created window.", w);
    SDL_Rect r = {0, 0, 320, 240};
    ASSERT_TRUE(w.setMouseRect(&r));
    SYNC_STEP("Set MouseRect window.", w);
  }

  TEST_F(testWindow, setOpacity)
  {
    Window w = Window("About to setOpacity this window");
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.setOpacity(0.5));
    SYNC_STEP("Set Opacity window.", w);
    ASSERT_TRUE(w.setOpacity(1));
    SYNC_STEP("Unset Opacity window.", w);
  }

  TEST_F(testWindow, setParent)
  {
    Window w = Window("About to setParent this window");
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.setParent(comWin));
    SYNC_STEP("Set Parent window.", w);
  }

  TEST_F(testWindow, setPosition)
  {
    Window w = Window("About to setPosition this window");
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.setPosition(100, 100));
    SYNC_STEP("Set Position window.", w);
  }

  TEST_F(testWindow, setRelativeMouseMode)
  {
    Window w = Window("About to setRelativeMouseMode this window");
    SYNC_STEP("Created window.", w);
    ASSERT_FALSE(w.getRelativeMouseMode());
    ASSERT_TRUE(w.setRelativeMouseMode(true));
    SYNC_STEP("Set RelativeMouseMode window.", w);
    ASSERT_TRUE(w.getRelativeMouseMode());
    ASSERT_TRUE(w.setRelativeMouseMode(false));
    SYNC_STEP("Unset RelativeMouseMode window.", w);
    ASSERT_FALSE(w.getRelativeMouseMode());
  }

  TEST_F(testWindow, setResizable)
  {
    Window w = Window("About to setResizable this window");
    SYNC_STEP("Created window.", w);
    ASSERT_FALSE(w.isResizable());
    ASSERT_TRUE(w.setResizable(true));
    SYNC_STEP("Set Resizable window.", w);
    ASSERT_TRUE(w.isResizable());
    ASSERT_TRUE(w.setResizable(false));
    SYNC_STEP("Unset Resizable window.", w);
    ASSERT_FALSE(w.isResizable());
  }

  TEST_F(testWindow, setShape)
  {
    Window w = Window("About to setShape this window", 640, 480,
                      SDL_WINDOW_TRANSPARENT);
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.setShape(nullptr));
    SYNC_STEP("Set Shape window.", w);
  }

  TEST_F(testWindow, setSize)
  {
    Window w = Window("About to setSize this window");
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.setSize(888, 777));
    SYNC_STEP("Set Size window.", w);
    int width, height;
    ASSERT_TRUE(w.getSize(&width, &height));
    ASSERT_EQ(width, 888);
    ASSERT_EQ(height, 777);
  }

  TEST_F(testWindow, setSurfaceVSync)
  {
    Window w = Window("About to setSurfaceVSync this window");
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.setSurfaceVSync(1000));
    SYNC_STEP("Set SurfaceVSync window.", w);
    ASSERT_TRUE(w.setSurfaceVSync(0));
    SYNC_STEP("Unset SurfaceVSync window.", w);
  }

  TEST_F(testWindow, setTitle)
  {
    Window w = Window("About to setTitle this window");
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.setTitle("New Title"));
    SYNC_STEP("Set Title window.", w);
  }

  TEST_F(testWindow, show)
  {
    Window w = Window("About to show this window", 320, 240, SDL_WINDOW_HIDDEN);
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.isHidden());
    //
    ASSERT_TRUE(w.show());
    SYNC_STEP("Shown window.", w);
    ASSERT_FALSE(w.isHidden());
  }

  TEST_F(testWindow, showSystemMenu)
  {
    Window w = Window("About to showSystemMenu this window");
    SYNC_STEP("Created window.", w);
    ASSERT_TRUE(w.showSystemMenu(100, 200));
    SYNC_STEP("Showed SystemMenu window.", w);
  }

  TEST_F(testWindow, sync)
  {
    Window w = Window("About to sync this window");
    SYNC_STEP("Created window.", w);
    w.sync();
    SYNC_STEP("Synced window.", w);
  }

  TEST_F(testWindow, updateSurface)
  {
    Window w = Window("About to updateSurface this window");
    SYNC_STEP("Created window.", w);
    w.updateSurface();
    SYNC_STEP("Updated Surface window.", w);
  }

  TEST_F(testWindow, updateSurfaceRects)
  {

    { // Test with an SDL_Rect pointer
      SDL_Rect r = {0, 0, 320, 240};
      ASSERT_TRUE(comWin.updateSurfaceRects(&r, 1));
      SYNC_STEP("Updated SurfaceRects window.", comWin);
    }
    { // With a SDL_Rect rvalue
      ASSERT_TRUE(comWin.updateSurfaceRects({{0, 0, 320, 240}}));
      SYNC_STEP("Updated SurfaceRects window.", comWin);
    }
  }

  TEST_F(testWindow, warpMouseIn)
  {
    Window w = Window("About to warpMouseIn this window");
    SYNC_STEP("Created window.", w);
    w.warpMouseIn(100, 100);
    SYNC_STEP("Warped MouseIn window.", w);
  }

  TEST_F(testWindow, hasSurface)
  {
    {
      Window w = Window("About to hasSurface this window");
      ASSERT_FALSE(w.hasSurface());
      ASSERT_TRUE(w.getSurface());
      ASSERT_TRUE(w.hasSurface());
    }
  }

  TEST_F(testWindow, CreateRenameResizeDestroy)
  {
    {
      // Create a window
      Window w = Window::Create("CreateRenameResizeDestroy", 320, 240,
                                SDL_WINDOW_RESIZABLE);
      // PUMP_EVENTS();STEP_DELAY;w.sync();
      SYNC_STEP("Created window.");
      // Rename the window
      w.setTitle("Renamed window");
      SYNC_STEP("Renamed window.");
      w.sync();
      // Resize the window
      w.setSize(640, 480);

      SYNC_STEP("Resized window.");
      w.sync();
      // Destroy the window
    }
    SYNC_STEP("Destroyed window");
  }

  TEST_F(testWindow, TstWindowID)
  {
    {
      Window w1 = Window("TstWindowID#1"), w2 = Window("TstWindowID#2");
      SYNC_STEP("Created windows.", w1, w2);
      SDL_WindowID id1 = w1.getID(), id2 = w2.getID();
      ASSERT_TRUE(id1 != id2);
      SYNC_STEP("Got window IDs.", w1, w2);
    }
    SYNC_STEP("Destroyed windows.");
  }

  TEST_F(testWindow, TstWindowSurface)
  {
    {
      Window w = Window("TstWindowSurface");
      ASSERT_FALSE(w.hasSurface());
      SYNC_STEP("Created window.", w);
      w.sync();
      auto s = w.getSurface();
      SYNC_STEP("Got window surface.", w);
      w.sync();
      ASSERT_TRUE(s);
      ASSERT_TRUE(w.hasSurface());
    }
    SYNC_STEP("Destroyed window.");
  }

  TEST_F(testWindow, TstAllHelpers)
  {
    {
      printf("Creating a default window to test all helpers...\n");
      Window w = Window("Test Minimize", 320, 200, SDL_WINDOW_RESIZABLE);
      SYNC_STEP("Created.", w);
      w.minimize();
      SYNC_STEP("Minimized.", w);
      w.restore();
      SYNC_STEP("Restored.", w);
      w.maximize();
      SYNC_STEP("Maximized.", w);
      w.restore();
      SYNC_STEP("Restored.", w);
      w.hide();
      SYNC_STEP("Hidden.", w);
      w.show();
      SYNC_STEP("Shown.", w);
      w.flash(SDL_FLASH_UNTIL_FOCUSED);
      SYNC_STEP("Flashed.", w);
      w.raise();
      SYNC_STEP("Raised.", w);
      ASSERT_TRUE(w.setSize(640, 480));
      SYNC_STEP("Resized.", w);
      {
        int width, height;
        w.getSize(&width, &height);
        ASSERT_EQ(width, 640);
        ASSERT_EQ(height, 480);
        SYNC_STEP("Got size.");
      }
    }
    SYNC_STEP("Destroyed window.");
  }

} // namespace
