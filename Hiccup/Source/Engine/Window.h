// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#pragma once

#include "Core/Core.h"
#include "Event.h"

namespace HC
{

// Function pointer type for the window event callback.
// When a window receives an event from the operating system, this callback
//   is fired, propagating the event across the engine.
using PFN_WindowEventCallback = void(*)(Event&);

enum class WindowViewMode : uint8
{
	Fullscreen = 0, Windowed = 1
};

enum class WindowStartMode : uint8
{
	Undefined = 0, Maximized = 1, Minimized = 2
};

struct WindowDescription
{
	// The client size of the window.
	uint32 width = 0;
	uint32 height = 0;

	// The position of the top-left window (not the client area) corner.
	int32 position_x = 0;
	int32 position_y = 0;

	StringView title = "Untitled"sv;

	WindowViewMode view_mode = WindowViewMode::Windowed;
	WindowStartMode start_mode = WindowStartMode::Undefined;

	PFN_WindowEventCallback event_callback = nullptr;
};

class Window
{
public:
	HC_NON_COPIABLE(Window)
	HC_NON_MOVABLE(Window)

public:
	HC_API ~Window();

	// Size of the window's client area.
	ALWAYS_INLINE NODISCARD uint32 get_width() const { return m_width; }
	ALWAYS_INLINE NODISCARD uint32 get_height() const { return m_height; }

	HC_API void set_width(uint32 new_width);
	HC_API void set_height(uint32 new_height);

	ALWAYS_INLINE void set_size(uint32 new_width, uint32 new_height)
	{
		set_width(new_width);
		set_height(new_height);
	}

	// Position of the window's top-left corner (not the window's client area).
	ALWAYS_INLINE NODISCARD uint32 get_position_x() const { return m_position_x; }
	ALWAYS_INLINE NODISCARD uint32 get_position_y() const { return m_position_y; }

	HC_API void set_position_x(int32 new_position_x);
	HC_API void set_position_y(int32 new_position_y);

	ALWAYS_INLINE void set_position(int32 new_position_x, int32 new_position_y)
	{
		set_position_x(new_position_x);
		set_position_y(new_position_y);
	}

	ALWAYS_INLINE NODISCARD StringView get_title() const { return m_title.to_view(); }
	HC_API void set_title(StringView new_title);

	// Window's view mode is actually set in update_window(). This means that the window is marked as needing
	//   to switch the view mode, but without actually changed it at the moment of calling the set_view_move()
	//   or toggle_view_mode().
	// As a side effect, get_view_mode() returns the current view mode, ignoring the fact that it might be marked
	//   for switch. If you care about this information, invoke is_pending_view_mode_switch() to manually check it.
	ALWAYS_INLINE NODISCARD WindowViewMode get_view_mode() const { return m_view_mode; }
	HC_API void set_view_mode(WindowViewMode new_view_mode);

	ALWAYS_INLINE void toggle_view_mode()
	{
		set_view_mode(m_view_mode == WindowViewMode::Windowed ? WindowViewMode::Fullscreen : WindowViewMode::Windowed);
	}

	// Destroying a window, programmatically or by closing the window (by pressing the X button), will not happen
	//   immediately. Instead, the window will be marked as pending to be killed. The actual window destruction
	//   is managed by the application, and it usually happens at the end of the frame.
	// This behavior enables the engine to make the assumption is available during the whole frame duration.
	ALWAYS_INLINE bool is_pending_kill() const { return m_is_pending_kill; }

	// As mentioned previous, programmatically switching the view mode only marks the window as needing a change.
	// The actual change happens when calling update_window(). This can be used to check if the window is marked as such.
	ALWAYS_INLINE bool is_pending_view_mode_switch() const { return m_is_pending_view_mode_switch; }

	// An opaque handle to the OS window handle. No guarantees made about its value, nor its possible usages.
	ALWAYS_INLINE NODISCARD void* get_native_handle() const { return m_native_handle; }

private:
	// Only the application (which is a friend of the window class) can create windows.
	Window(const WindowDescription& description);
	static UniquePtr<Window> create(const WindowDescription& description)
	{
		return UniquePtr<Window>(hc_new Window(description));
	}

	void update_window();

	void on_closed();
	void on_resized(uint32 width, uint32 height);
	void on_moved(int32 position_x, int32 position_y);

private:
	void* m_native_handle;
	PFN_WindowEventCallback m_event_callback;

	// The dimensions of the window's client area.
	uint32 m_width;
	uint32 m_height;

	// The position of the top-left window corner (including the border), in screen coordinates.
	int32 m_position_x;
	int32 m_position_y;

	// The window's parameters (width, height and position) are only modified during update_window(),
	//   which is called at the beginning of a frame, updating the window to the state set by the last frame.
	// When a parameter change is programmatically requested, such as resize or move, via set_size() for
	//   example, the action is recorded in these 'dirty' values. At the beginning of the new frame, if
	//   any of them are different than the actual current values, the window will be updated to use them.
	uint32 m_dirty_width;
	uint32 m_dirty_height;
	int32 m_dirty_position_x;
	int32 m_dirty_position_y;

	// Used when switching between modes (fullscreen or windowed).
	// They save the state of the window in windowed mode, right before transitioning to fullscreen. Later,
	//   if the window is requested to go back to the windowed mode, these values will be set as the window's
	//   parameters, essentially restoring them to their previous values.
	uint32 m_saved_width;
	uint32 m_saved_height;
	int32 m_saved_position_x;
	int32 m_saved_position_y;

	String m_title;

	WindowViewMode m_view_mode;

	// Signals that the window's parameters (width, height and position) have been modified during the
	//   previous frame, and we shall update them.
	bool m_is_dirty;

	// Just like most other window parameters/properties, switching view modes happens at the beginning of the
	//   frame, using the view mode set in the previous frame.
	// This flags signals that the view mode was changed, and we shall update it. The actual OS call
	//   is in update_window().
	bool m_is_pending_view_mode_switch;

	// The window destruction happens at the end of the frame.
	// This flag signals that we shall destroy the window.
	// Its values can be queried via is_pending_kill().
	bool m_is_pending_kill;

	struct WindowBorder
	{
		uint32 left;
		uint32 right;
		uint32 top;
		uint32 bottom;
	};

	// Currently, only used by the Windows implementation. Useful for converting between client and window space.
	// This is not exposed to the user, nor the engine, because some platforms might not have this concept, or
	//   it might not be required.
	WindowBorder m_border;

private:
	// The application manages the window creation, lifetime and behavior, as it is responsible for calling
	//   the update_window() function. As these function are part of the private API, they need to be friends.
	friend class Application;

	// There might be OS-specific, utilities or mandatory by the platform API, functions. To not introduce too
	//   much complexity, everything present in the WindowInternalCalls interface will be able to access the
	//   window's internal properties and methods.
	friend struct WindowInternalCalls;
};

} // namespace HC