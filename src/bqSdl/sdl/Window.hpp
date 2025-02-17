#pragma once
/** @file Window.hpp
 * @brief Wrap SDL3 API
 * @copyright Copyright (c) 2025 BadQuanta.
 * This file is part of the bq distribution
 * (https://github.com/badquanta/bqpp).
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/
#include <SDL3/SDL.h>
#include <bq/SharedResource.hpp>
// #include <bq/sdl/Renderer.hpp>
// #include <bq/sdl/Surface.hpp>
#include <string>
#include <vector>
/**
 * - Window SharedResource<SDL_Window> Window.hpp
 */
namespace bq::sdl
{
  /** @note Window.hpp Instances of Window may return instances of Renderer */
  class Renderer;
  /** @note Window.hpp Instances of Window may return instances of Surface */
  class Surface;
  /** @note Window.hpp Instances of Window may return instances of Properties */
  class Properties;
  /**
   * @brief Wrapper for `SDL_Window` structure.
   * @note default constructor == nullptr reference
   */
  class Window : public bq::SharedResource<SDL_Window>
  {
  public: // STATIC MEMBERS & CONSTRUCTORS
    /** INHERIT CONSTRUCTORS **/
    using SharedResource<SDL_Window>::SharedResource;
    Window(SDL_PropertiesID p);
    /** Title used in factory methods when not specified. */
    static std::string DEFAULT_TITLE;
    /** Width used in factory methods when not specified. */
    static int DEFAULT_WIDTH;
    /** Height used in factory methods when not specified. */
    static int DEFAULT_HEIGHT;
    /** Flags used in factory methods when not specified. */
    static SDL_WindowFlags DEFAULT_FLAGS;
    // Window(SDL_Window *a_Width, deleter_callback_t d = Destroy);
    /** @todo Move this back to a inline-static factory method? */
    Window(const char *a_Title, int a_Width = DEFAULT_WIDTH,
           int a_Height                 = DEFAULT_HEIGHT,
           SDL_WindowFlags a_Flags      = DEFAULT_FLAGS,
           deleter_callback_t a_Deleter = Destroy);

    /**
     * @brief Destroy the window.
     * @param a_Width the window to destroy.
     * @note explicitly NOT inline (needs to be referenced at runtime).
     * @note This is the default deleter for the Window class.
     * @note This function also quits the video subsystem.
     */
    static void Destroy(SDL_Window *w);
    /** @brief Create a window.
     * @details Create a window with the specified title, width, height, and
     * flags.
     * @param a_Title the title of the window, default is DEFAULT_TITLE
     * @param a_Width the width of the window, default is DEFAULT_WIDTH
     * @param a_Height the height of the window, default is DEFAULT_HEIGHT
     * @param a_Flags the flags for the window, default is DEFAULT_FLAGS
     * @throws std::string if SDL_InitSubSystem fails
     * @throws std::string if SDL_CreateWindow fails
     * @return a `Window` shared_ptr specialization for this SDL_Window
     */
    template <typename TWindow = Window>
    inline static TWindow Create(
        const char *a_Title     = Window::DEFAULT_TITLE.c_str(),
        int a_Width             = Window::DEFAULT_WIDTH,
        int a_Height            = Window::DEFAULT_HEIGHT,
        SDL_WindowFlags a_Flags = Window::DEFAULT_FLAGS)
    {
      if (not SDL_InitSubSystem(SDL_INIT_VIDEO))
      {
        throw(std::string(SDL_GetError()));
      }
      SDL_Window *winptr =
          SDL_CreateWindow(a_Title, a_Width, a_Height, a_Flags);
      if (not winptr)
      {
        throw(std::string(SDL_GetError()));
      }
      return TWindow(winptr, Destroy);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_GetGrabbedWindow
     * @brief Get the window that currently has input focus.
     * @returns a Window reference for the grabbed window.
     * @returns nullptr reference if no window has input focus.
     * @note This is a non-owning reference.
     */
    inline static Window GetGrabbed()
    {
      return Window(SDL_GetGrabbedWindow(), Window::null_deleter);
    }
    /**
     * @brief Return non-managing reference from an SDL_Event
     * @returns returns non-owning or nullptr reference
     **/
    inline static Window GetFromEvent(const SDL_Event *a_Event)
    {
      return Window(SDL_GetWindowFromEvent(a_Event), Window::null_deleter);
    }
    /**
     * @brief Get a reference to a Window from a Window ID
     * @returns returns non-owning or nullptr reference
     * @returns nullptr if invalid WindowID
     **/
    inline static Window GetFromID(Uint32 a_ID)
    {
      return Window(SDL_GetWindowFromID(a_ID), Window::null_deleter);
    }
    /** @see https://wiki.libsdl.org/SDL3/SDL_GetWindows
     * @brief Get a list of valid Window references
     * @returns a std::vector<Window> of currently valid non-owning references
     */
    inline static std::vector<Window> GetWindows()
    {
      int n;
      SDL_Window **windows = SDL_GetWindows(&n);
      if (windows)
      {
        std::vector<Window> wins;
        for (int i = 0; i < n; i++)
        {
          wins.push_back(Window(windows[i], Window::null_deleter));
        }
        SDL_free(windows);
        return wins;
      }
      else
      {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GetWindows failed: %s",
                     SDL_GetError());
        return {};
      }
    }

  public: // INSTANCE MEMBERS
          /** @brief is the SDL_WINDOW_ALWAYS_ON_TOP flag set?
           *
           */
          // bool isAlwaysOnTop() const
          //{
//      return (SDL_GetWindowFlags(*this) & SDL_WINDOW_ALWAYS_ON_TOP) != 0;
//  }

/**
 * @brief generate boilerplate inline function code for checking each of the
 * SDL_WindowFlags
 * @see https://wiki.libsdl.org/SDL3/SDL_WindowFlags
 */
#define BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(NAME, FLAGS)                     \
  inline bool NAME() const                                                     \
  {                                                                            \
    return (SDL_GetWindowFlags(*this) & FLAGS) != 0;                           \
  }
          // USE MACRO TO GENERATE INLINE FUNCTIONS
    /**
     * @return true if SDL_WINDOW_ALWAYS_ON_TOP flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isAlwaysOnTop,
                                          SDL_WINDOW_ALWAYS_ON_TOP);
    /**
     * @return true if SDL_WINDOW_FULLSCREEN flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isFullscreen, SDL_WINDOW_FULLSCREEN);
    /**
     * @return true if SDL_WINDOW_FULLSCREEN_DESKTOP flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isOpenGL, SDL_WINDOW_OPENGL);
    /**
     * @return true if SDL_WINDOW_HIDDEN flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isHidden, SDL_WINDOW_HIDDEN);
    /**
     * @return true if SDL_WINDOW_BORDERLESS flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isBorderless, SDL_WINDOW_BORDERLESS);
    /**
     * @return true if SDL_WINDOW_RESIZABLE flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isResizable, SDL_WINDOW_RESIZABLE);
    /**
     * @return true if SDL_WINDOW_MINIMIZED flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isMinimized, SDL_WINDOW_MINIMIZED);
    /**
     * @return true if SDL_WINDOW_MAXIMIZED flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isMaximized, SDL_WINDOW_MAXIMIZED);
    /**
     * @return true if SDL_WINDOW_INPUT_GRABBED flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isMouseGrabbed,
                                          SDL_WINDOW_MOUSE_GRABBED);
    /**
     * @return true if SDL_WINDOW_INPUT_FOCUS flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isInputFocus, SDL_WINDOW_INPUT_FOCUS);
    /**
     * @return true if SDL_WINDOW_MOUSE_FOCUS flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isMouseFocus, SDL_WINDOW_MOUSE_FOCUS);
    /**
     * @return true if SDL_WINDOW_EXTERNAL flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isExternal, SDL_WINDOW_EXTERNAL);
    /**
     * @return true if SDL_WINDOW_MODAL flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isModal, SDL_WINDOW_MODAL);
    /**
     * @return true if SDL_WINDOW_HIGH_PIXEL_DENSITY flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isHighPixelDensity,
                                          SDL_WINDOW_HIGH_PIXEL_DENSITY);
    /**
     * @return true if SDL_WINDOW_MOUSE_CAPTURE flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isMouseCapture,
                                          SDL_WINDOW_MOUSE_CAPTURE);
    /**
     * @return true if SDL_WINDOW_UTILIY flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isUtility, SDL_WINDOW_UTILITY);
    /**
     * @return true if SDL_WINDOW_TOOLTIP flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isTooltip, SDL_WINDOW_TOOLTIP);
    /**
     * @return true if SDL_WINDOW_POPUP_MENU flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isPopupMenu, SDL_WINDOW_POPUP_MENU);
    /**
     * @return true if SDL_WINDOW_KEYBOARD_GRABBED flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isKeyboardGrabbed,
                                          SDL_WINDOW_KEYBOARD_GRABBED);
    /**
     * @return true if SDL_WINDOW_VULKAN flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isVulkan, SDL_WINDOW_VULKAN);
    /**
     * @return true if SDL_WINDOW_METAL flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isMetal, SDL_WINDOW_METAL);
    /**
     * @return true if SDL_WINDOW_TRANSPARENT flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isTransparent,
                                          SDL_WINDOW_TRANSPARENT);
    /**
     * @return true if SDL_WINDOW_NOT_FOCUSABLE flag set
     */
    BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS(isNotFocusable,
                                          SDL_WINDOW_NOT_FOCUSABLE);
    // UNDEFINE MACRO
#undef BQ_INLINE_CHECK_THIS_WINDOW_FOR_FLAGS
    /**
     * inverse of `isNotFocusable`
     */
    inline bool isFocusable() const
    {
      return !isNotFocusable();
    }
    /** @brief Get the WindowID value
     * @see https://wiki.libsdl.org/SDL3/SDL_WindowID
     * @see https://wiki.libsdl.org/SDL3/SDL_GetWindowID
     * @returns SDL_WindowID value
     */
    inline SDL_WindowID getID() const
    {
      return SDL_GetWindowID(*this);
    };
    /** @brief Get the Surface associated with the Window
     * @note uses SharedResource<SDL_Surface>::null_deleter
     * @todo document me @see https://wiki.libsdl.org/SDL3/SDL_GetWindowSurface
     * @returns a non-managed Surface reference
     */
    template <typename TSurface = bq::sdl::Surface>
    inline TSurface getSurface() const
    {
      return TSurface(SDL_GetWindowSurface(*this), TSurface::null_deleter);
    }

    /** @see https://wiki.libsdl.org/SDL3/SDL_GetWindowDisplayScale
     * @brief get the display scale factor of the window associated with the
     * surface.
     * @return the scale factor of the display associated with the window
     */
    inline float getDisplayScale() const
    {
      return SDL_GetWindowDisplayScale(*this);
    }

    /** @brief  check if the Window has an associated Surface
     * @returns true if the Window has an associated Surface
     * @see https://wiki.libsdl.org/SDL3/SDL_WindowHasSurface
     */
    inline bool hasSurface() const
    {
      return SDL_WindowHasSurface(*this);
    };

    /** @brief destroy the surface associated with this window.
     *  @returns true on success
     *  @see https://wiki.libsdl.org/SDL3/SDL_DestroyWindowSurface
     */
    inline bool destroySurface() const
    {
      return SDL_DestroyWindowSurface(*this);
    };
    /** @brief request that the window be minimized
     *  @see https://wiki.libsdl.org/SDL3/SDL_MinimizeWindow
     *  @returns true on success
     */
    inline bool minimize() const
    {
      return SDL_MinimizeWindow(*this);
    };
    /** @brief request that the window be maximized
     *  @see https://wiki.libsdl.org/SDL3/SDL_MaximizeWindow
     *  @returns true on success
     */
    inline bool maximize() const
    {
      return SDL_MaximizeWindow(*this);
    };
    /** @brief request that the window be restored
     * @see https://wiki.libsdl.org/SDL3/SDL_RestoreWindow
     * @returns true on success
     */
    inline bool restore() const
    {
      return SDL_RestoreWindow(*this);
    };
    /** @brief request that the window be raised
     *  @see https://wiki.libsdl.org/SDL3/SDL_RaiseWindow
     * @returns true on success
     */
    inline bool raise() const
    {
      return SDL_RaiseWindow(*this);
    };
    /** @brief request that the window be set to fullscreen
     *  @see https://wiki.libsdl.org/SDL3/SDL_ShowWindow
     * @returns true on success
     */
    inline bool show() const
    {
      return SDL_ShowWindow(*this);
    };
    /** @brief request that the window be hidden
     * @see https://wiki.libsdl.org/SDL3/SDL_HideWindow
     * @returns true on success
     */
    inline bool hide() const
    {
      return SDL_HideWindow(*this);
    };

    /**
     * @brief Get a reference to the associated renderer.
     * @returns non-managed reference
     */
    template <typename TRenderer = bq::sdl::Renderer>
    inline TRenderer getRenderer() const
    {
      return TRenderer(SDL_GetRenderer(*this), TRenderer::null_deleter);
    };
    /**  @brief request that the window perform a flash operation
     * @param a_Operation the flash operation to perform
     * @see SDL_FlashOperation
     * @see https://wiki.libsdl.org/SDL3/SDL_FlashWindow
     * @returns true on success
     */
    inline bool flash(SDL_FlashOperation a_Operation = SDL_FLASH_BRIEFLY) const
    {
      return SDL_FlashWindow(*this, a_Operation);
    };
    /** @brief get the size of a window's client area
     * @param a_W float pointer where the width will be written
     * @param a_H float pointer where the height will be written
     * @see https://wiki.libsdl.org/SDL3/SDL_GetWindowAspectRatio
     * @returns true on success
     */
    inline bool getAspectRatio(float *a_W, float *a_H) const
    {
      return SDL_GetWindowAspectRatio(*this, a_W, a_H);
    };
    /** @brief get the size of a window's borders
     * @param a_Top int pointer where the top border size will be written
     * @param a_Left int pointer where the left border size will be written
     * @param a_Bottom int pointer where the bottom border size will be written
     * @param a_Right int pointer where the right border size will be written
     * @see https://wiki.libsdl.org/SDL3/SDL_GetWindowBordersSize
     * @returns true on success
     */
    inline bool getBorderSize(int *a_Top, int *a_Left, int *a_Bottom,
                              int *a_Right) const
    {
      return SDL_GetWindowBordersSize(*this, a_Top, a_Left, a_Bottom, a_Right);
    };
    /** @brief Get the window flags
     * @see https://wiki.libsdl.org/SDL3/SDL_GetWindowFlags
     * @see https://wiki.libsdl.org/SDL3/SDL_WindowFlags
     * @returns SDL_WindowFlags
     **/
    inline SDL_WindowFlags getFlags() const
    {
      return SDL_GetWindowFlags(*this);
    };
    /** @brief Get the index of the display associated with the window
     *  @see https://wiki.libsdl.org/SDL3/SDL_GetDisplayForWindow
     * @return SDL_DisplayID */
    inline SDL_DisplayID getDisplayID()
    {
      return SDL_GetDisplayForWindow(*this);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_GetWindowFullscreenMode
     * @brief Get the display mode used when the window is fullscreen
     * @returns a pointer to the SDL_DisplayMode used when the window is
     * fullscreen
     * @returns nullptr for borderless fullscreen desktop mode
     */
    inline const SDL_DisplayMode *getFullscreenMode() const
    {
      return SDL_GetWindowFullscreenMode(*this);
    }
    /** @brief Get the ICC profile associated with the window
     * @param a_Size a pointer to the a_Size of the ICC profile
     * @see https://wiki.libsdl.org/SDL3/SDL_GetWindowICCProfile
     * @returns a pointer to the ICC profile
     */
    inline void *getICCProfile(size_t *a_Size) const
    {
      return SDL_GetWindowICCProfile(*this, a_Size);
    }
    /** @brief Copy the window surface to the operation system window
     *  @see https://wiki.libsdl.org/SDL3/SDL_UpdateWindowSurface
     * @returns true on success
     */
    inline bool updateSurface()
    {
      return SDL_UpdateWindowSurface(*this);
    };
    /** @brief Copy areas of the window surface to the operation system window
     * @param a_Rects an array of SDL_Rects to copy
     * @param a_NumRects the number of SDL_Rects to copy
     * @see https://wiki.libsdl.org/SDL3/SDL_UpdateWindowSurfaceRects
     * @returns true on success
     */
    inline bool updateSurfaceRects(const SDL_Rect *a_Rects,
                                   int a_NumRects) const
    {
      return SDL_UpdateWindowSurfaceRects(*this, a_Rects, a_NumRects);
    };

    /** @brief Tell SDL to update the following rectangles
     * @param a_Rects std::vector of SDL_Rect to update.
     */
    template <typename Rects = std::vector<SDL_Rect>>
    inline bool updateSurfaceRects(const Rects &a_Rects) const
    {
      return updateSurfaceRects(a_Rects.data(), a_Rects.size());
    };

    /** @see https://wiki.libsdl.org/SDL3/SDL_CreatePopupWindow 
     * @brief create a new window that is a popup window for this. 
     **/
    inline Window createPopup(int a_OffsetX, int a_OffsetY, int a_Width,
                              int a_Height, Uint32 a_Flags) const
    {
      SDL_Window *created = SDL_CreatePopupWindow(*this, a_OffsetX, a_OffsetY,
                                                  a_Width, a_Height, a_Flags);
      if (not created)
      {
        throw std::runtime_error(SDL_GetError());
      }

      return Window(created, Destroy);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_GetWindowKeyboardGrab
     * @brief Get the keyboard grab state of the window
     * @returns true if the window has keyboard grab
     * @returns false if the window does not have keyboard grab
     * @note could be renamed to "isKeyboardGrabbed"
     */
    inline bool getKeyboardGrab() const
    {
      return SDL_GetWindowKeyboardGrab(*this);
    };
    /**
     * @see https://wiki.libsdl.org/SDL3/SDL_GetWindowMaximumSize
     * @brief Get the maximum size of the window
     * @param a_Width int pointer where the maximum width will be written
     * @param a_Height int pointer where the maximum height will be written
     * @returns true on success
     */
    inline bool getMaximumSize(int *a_Width, int *a_Height) const
    {
      return SDL_GetWindowMaximumSize(*this, a_Width, a_Height);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_GetWindowMinimumSize
     * @brief Get the minimum size of the window
     * @param a_Width int pointer where the minimum width will be written
     * @param a_Height int pointer where the minimum height will be written
     * @returns true on success
     */
    inline bool getMinimumSize(int *a_Width, int *a_Height) const
    {
      return SDL_GetWindowMinimumSize(*this, a_Width, a_Height);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_GetMouseGrab
     * @returns true if the window has mouse grab
     */
    inline bool getMouseGrab() const
    {
      return SDL_GetWindowMouseGrab(*this);
    };
    /**
     * @see https://wiki.libsdl.org/SDL3/SDL_GetWindowMouseRect
     * @brief Get the opacity of the window
     * @returns the opacity of the window
     **/
    inline const SDL_Rect *getMouseRect() const
    {
      return SDL_GetWindowMouseRect(*this);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_GetWindowOpacity
     * @brief Get the opacity of the window
     * @returns the opacity of the window
     **/
    inline float getOpacity() const
    {
      return SDL_GetWindowOpacity(*this);
    };

    /** @see https://wiki.libsdl.org/SDL3/SDL_GetWindowParent
     * @brief Get the parent window of the window
     * @returns a non-owning reference to the parent window, or nullptr
     */
    inline Window getParent() const
    {
      return Window(SDL_GetWindowParent(*this), Window::null_deleter);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_GetWindowPixelDensity
     * @brief Get the pixel density of the window
     * @returns the pixel density of the window
     **/
    inline float getPixelDensity() const
    {
      return SDL_GetWindowPixelDensity(*this);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_GetWindowPixelFormat
     * @brief Get the pixel format of the window
     * @returns the pixel format of the window
     **/
    inline SDL_PixelFormat getPixelFormat() const
    {
      return SDL_GetWindowPixelFormat(*this);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_GetWindowPosition
     * @brief Get the position of the window
     * @param a_X int pointer where the x-coordinate will be written
     * @param a_Y int pointer where the y-coordinate will be written
     * @returns true on success
     */
    inline bool getPosition(int *a_X, int *a_Y) const
    {
      return SDL_GetWindowPosition(*this, a_X, a_Y);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_GetWindowProperties
     * @brief Get the SDL_PropertiesID of the window
     * @returns the SDL_PropertiesID of the window
     */
    inline SDL_PropertiesID getPropertiesID() const
    {
      return SDL_GetWindowProperties(*this);
    };
    /**
     * @brief Get the properties of the window
     * @returns the properties of the window
     */
    template <typename TProperties = bq::sdl::Properties>
    inline TProperties getProperties() const
    {
      return TProperties(getPropertiesID());
    };
    /**
     * @see https://wiki.libsdl.org/SDL3/SDL_GetWindowRelativeMouseMode
     * @brief Get the relative mouse mode of the window
     * @returns true if the window is in relative mouse mode
     * @note perhaps could be renamed to "hasRelativeMouseMode"
     **/
    inline bool getRelativeMouseMode() const
    {
      return SDL_GetWindowRelativeMouseMode(*this);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_GetWindowSafeArea
     * @brief Get the safe area of the window
     * @param a_Rect a pointer to the SDL_Rect where the safe area will be
     * written
     * @returns true on success
     */
    inline bool getSafeArea(SDL_Rect *a_Rect)
    {
      return SDL_GetWindowSafeArea(*this, a_Rect);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_GetWindowSize
     * @brief Get the size of the window
     * @param a_Width int pointer where the width will be written
     * @param a_Height int pointer where the height will be written
     * @returns true on success
     */
    inline bool getSize(int *a_Width, int *a_Height)
    {
      return SDL_GetWindowSize(*this, a_Width, a_Height);
    };

    /** @brief Get the width of the window
     * @see https://wiki.libsdl.org/SDL3/SDL_GetWindowSize
     */
    inline int getWidth()
    {
      int w{0};
      if (!getSize(&w, nullptr))
      {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GetWindowSize: %s",
                     SDL_GetError());
      }
      return w;
    };
    /** @brief Get the height of the window 
     * @see https://wiki.libsdl.org/SDL3/SDL_GetWindowSize 
    */
    inline int getHeight()
    {
      int h{0};
      if (!getSize(nullptr, &h))
      {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "SDL_GetWindowSize: %s",
                     SDL_GetError());
      }
      return h;
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_GetWindowSizeInPixels
     * @brief Get the size of the window in pixels
     * @param a_Width int pointer where the width will be written
     * @param a_Height int pointer where the height will be written
     * @returns true on success
     */
    inline bool getSizeInPixels(int *a_Width, int *a_Height)
    {
      return SDL_GetWindowSizeInPixels(*this, a_Width, a_Height);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_GetWindowSurfaceVSync
     * @brief Get the vertical sync interval of the window
     * @param a_Interval int pointer where the interval will be written
     * @note see `setSurfaceVSync` for the meaning of the value
     * @returns true on success
     */
    inline bool getSurfaceVSync(int *a_Interval)
    {
      return SDL_GetWindowSurfaceVSync(*this, a_Interval);
    };
    /**
     * @brief Get the vertical sync interval of the window
     * @returns the vertical sync interval of the window
     * @note see `setSurfaceVSync` for the meaning of the value
     * @note logs error if the call fails
     */
    inline int getSurfaceVSync()
    {
      int interval{0};
      if (!getSurfaceVSync(&interval))
      {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                     "SDL_GetWindowSurfaceVSync: %s", SDL_GetError());
      }
      return interval;
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_GetWindowTitle
     * @brief Get the title of the window
     * @returns the title of the window
     **/
    inline const char *getTitle()
    {
      return SDL_GetWindowTitle(*this);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_SetWindowAlwaysOnTop
     * @brief Set the window to always be on top
     * @param a_OnTop true to set the window to always be on top
     * @returns true on success
     */
    inline bool setAlwaysOnTop(bool a_OnTop)
    {
      return SDL_SetWindowAlwaysOnTop(*this, a_OnTop);
    };
    /** @brief Set aspect ratio of window
     * @param a_MinAspect
     * @param a_MaxAspect
     * @returns true on success
     * @see https://wiki.libsdl.org/SDL3/SDL_SetWindowAspectRatio
     */
    inline bool setAspectRatio(float a_MinAspect, float a_MaxAspect)
    {
      return SDL_SetWindowAspectRatio(*this, a_MinAspect, a_MaxAspect);
    };
    /** @brief Enable or disable the window border
     * @param a_Bordered true to enable, false to disable. 
     * @returns true on success
     * @see https://wiki.libsdl.org/SDL3/SDL_SetWindowBordered
    */
    inline bool setBordered(bool a_Bordered)
    {
      return SDL_SetWindowBordered(*this, a_Bordered);
    };
    /** @brief Enable or disable focusing of the window
     * @param a_Focusable  true to enable, false to disable.
     * @returns true on success
     */
    inline bool setFocusable(bool a_Focusable)
    {
      return SDL_SetWindowFocusable(*this, a_Focusable);
    };
    /** @brief Enable or disable window fullscreen
     * @param a_Fullscreen true to set fullscreen, false to restore from fullscreen.
     * @returns true on success
     */
    inline bool setFullscreen(bool a_Fullscreen)
    {
      return SDL_SetWindowFullscreen(*this, a_Fullscreen);
    };
    /** @brief Set the fullscreen mode to use
     * @param a_Mode an 'SDL_DisplayMode' to use for fullscreen window.
     * @returns true on success
     * @see https://wiki.libsdl.org/SDL3/SDL_SetWindowFullscreenMode */
    inline bool setFullscreenMode(const SDL_DisplayMode *a_Mode)
    {
      return SDL_SetWindowFullscreenMode(*this, a_Mode);
    };
    /** @brief Set function to use for window "hit tests"
     * @param a_HitTest function to use for hit tests.
     * @param a_UserData pointer that is passed to hit test function.
     * @see https://wiki.libsdl.org/SDL3/SDL_SetWindowHitTest
     */
    inline bool setHitTest(SDL_HitTest a_HitTest, void *a_UserData)
    {
      return SDL_SetWindowHitTest(*this, a_HitTest, a_UserData);
    };
    /** @brief Set the icon to associate with this window
     * @param a_Icon an 'SDL_Surface' to use as this window's icon.
     * @returns true on success
     * @see https://wiki.libsdl.org/SDL3/SDL_SetWindowIcon */
    inline bool setIcon(SDL_Surface *a_Icon)
    {
      return SDL_SetWindowIcon(*this, a_Icon);
    };
    /** @brief Enable or disable "Keyboard Grab"
     * @param a_Grabbed true to enable, false to disable
     * @returns true on success
     * @see https://wiki.libsdl.org/SDL3/SDL_SetWindowKeyboardGrab */
    inline bool setKeyboardGrab(bool a_Grabbed)
    {
      return SDL_SetWindowKeyboardGrab(*this, a_Grabbed);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_SetWindowMaximumSize 
     * @brief Set the maximum size of this window
     * @param a_Width maximum width allowed
     * @param a_Height maximum height allowed
     * @returns true on success
    */
    inline bool setMaximumSize(int a_Width, int a_Height)
    {
      return SDL_SetWindowMaximumSize(*this, a_Width, a_Height);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_SetWindowModal
     * @brief Set the window to be modal
     * @param a_Modal true to set the window to be modal
     * @param a_Parent (optional) the parent to set the window modal to
     * @note if a_Parent is nullptr, the parent must already be specified
     * @returns true on success
     */
    inline bool setModal(bool a_Modal, SDL_Window *a_Parent = nullptr)
    {
      if (a_Parent)
      {
        setParent(a_Parent);
      }
      return SDL_SetWindowModal(*this, a_Modal);
    };
    /** @brief Set window minimum size
     * @param a_Width minimum allowable width
     * @param a_Height minimum allowable height
     * @returns true on success 
     * @see https://wiki.libsdl.org/SDL3/SDL_SetWindowMinimumSize */
    inline bool setMinimumSize(int a_Width, int a_Height)
    {
      return SDL_SetWindowMinimumSize(*this, a_Width, a_Height);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_SetWindowMouseGrab
     * @brief Enable or disable mouse grabbing
     * @param a_Grabbed true to enable, false to disable
     * @returns true on success
     */
    inline bool setMouseGrab(bool a_Grabbed)
    {
      return SDL_SetWindowMouseGrab(*this, a_Grabbed);
    };
    /** @brief Set the rectangle within this window that the mouse may travel within
     * @param a_Rect an 'SDL_Rect' pointer to use, nullptr to disable.
     * @returns true on success
     * @see https://wiki.libsdl.org/SDL3/SDL_SetWindowMouseRect
     */
    inline bool setMouseRect(const SDL_Rect *a_Rect)
    {
      return SDL_SetWindowMouseRect(*this, a_Rect);
    };
    /** @brief Set the window opacity
     * @param a_Opacity 1.0f is fully opaque, 0.0f is fully transparent. 
     * @returns true on success
     * @see https://wiki.libsdl.org/SDL3/SDL_SetWindowOpacity
     */
    inline bool setOpacity(float a_Opacity)
    {
      return SDL_SetWindowOpacity(*this, a_Opacity);
    };
    /** @brief Set this window as a child of another window
     * @param a_Parent the new parent of this window.
     * @returns true on success
     *  @see https://wiki.libsdl.org/SDL3/SDL_SetWindowParent
     */
    inline bool setParent(SDL_Window *a_Parent)
    {
      return SDL_SetWindowParent(*this, a_Parent);
    };
    /** @todo Set the position of this window
     * @param a_X new X position
     * @param a_Y new Y position
     * @returns true on success
     * @see https://wiki.libsdl.org/SDL3/SDL_SetWindowPosition
     */
    inline bool setPosition(int a_X, int a_Y)
    {
      return SDL_SetWindowPosition(*this, a_X, a_Y);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_SetWindowResizable
     * @brief Enable or disable resizing of the window
     * @param a_Resizable true to enable, false to disable. 
     * @returns true on success
     */
    inline bool setResizable(bool a_Resizable)
    {
      return SDL_SetWindowResizable(*this, a_Resizable);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_SetWindowRelativeMouseMode 
     * @brief enable or disable relative mouse mode
     * @param a_Enabled true to enable, false to disable
     * @returns true on success
     * **/
    inline bool setRelativeMouseMode(bool a_Enabled)
    {
      return SDL_SetWindowRelativeMouseMode(*this, a_Enabled);
    };
    /** @brief Set the shape of the window
     * @param a_Shape 'SDL_Surface' to use the Alpha Channel that will define the shape of the function. 
     * @see https://wiki.libsdl.org/SDL3/SDL_SetWindowShape
     */
    inline bool setShape(SDL_Surface *a_Shape)
    {
      return SDL_SetWindowShape(*this, a_Shape);
    };
    /** @todo document me @see https://wiki.libsdl.org/SDL3/SDL_SetWindowSize */
    inline bool setSize(int a_Width, int a_Height)
    {
      return SDL_SetWindowSize(*this, a_Width, a_Height);
    };
    /** @todo document me @see
     * https://wiki.libsdl.org/SDL3/SDL_SetWindowSurfaceVSync */
    inline bool setSurfaceVSync(int a_Interval)
    {
      return SDL_SetWindowSurfaceVSync(*this, a_Interval);
    };
    inline bool setSurfaceVSyncAdaptive()
    {
      return setSurfaceVSync(SDL_WINDOW_SURFACE_VSYNC_ADAPTIVE);
    };
    inline bool setSurfaceVSyncDisable()
    {
      return setSurfaceVSync(SDL_WINDOW_SURFACE_VSYNC_DISABLED);
    };

    /** @see https://wiki.libsdl.org/SDL3/SDL_SetWindowTitle */
    inline bool setTitle(const char *a_Title)
    {
      return SDL_SetWindowTitle(*this, a_Title);
    };
    inline bool setTitle(const std::string &a_Title)
    {
      return setTitle(a_Title.c_str());
    };

    /** @see https://wiki.libsdl.org/SDL3/SDL_ShowWindowSystemMenu */
    inline bool showSystemMenu(int a_X, int a_Y)
    {
      return SDL_ShowWindowSystemMenu(*this, a_X, a_Y);
    };

    /** @see https://wiki.libsdl.org/SDL3/SDL_SyncWindow */
    inline bool sync()
    {
      return SDL_SyncWindow(*this);
    };
    /** @see https://wiki.libsdl.org/SDL3/SDL_WarpMouseInWindow
     * @brief Warp the mouse to the window
     * @param a_X the x-coordinate to warp the mouse to
     * @param a_Y the y-coordinate to warp the mouse to
     */
    inline void warpMouseIn(float a_X, float a_Y)
    {
      SDL_WarpMouseInWindow(*this, a_X, a_Y);
    };
  };
} // namespace bq::sdl
