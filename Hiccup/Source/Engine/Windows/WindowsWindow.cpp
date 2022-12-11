// Copyright (c) 2022-2022 Avram Traian. All rights reserved.

#if HC_PLATFORM_WINDOWS

#include "Engine/Window.h"
#include "Core/Application.h"

#include <Windows.h>

namespace HC
{

// All windows instances are stored in this array. Because the Window API only allows windows be
//   created on the heap, the pointers stored in this will never point to freed/reused memory.
// This is mainly used to associate a window with its native handle.
// The reason this is not a hash table is because there are only a few active windows at a time.
//   So, it would be wasteful to have to perform a search in a table for such a limited number of elements.
static_internal Array<Window*> s_windows;

// See the Window class friend for why this struct exists. It basically servers as a friend namespace.
struct WindowInternalCalls
{
public:
	// The Win32 API function that is used as a callback when the OS fires a message to the window.
	static LRESULT event_procedure(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param);

	// Utility function for performing a search for a given window native handle, returning the
	//   Window instance that holds it.
	// This will return nullptr if the calling is during the window creation or destruction.
	ALWAYS_INLINE static Window* find_window_by_handle(void* window_handle)
	{
		for (usize i = 0; i < s_windows.size(); ++i)
		{
			if (s_windows[i]->get_native_handle() == window_handle)
			{
				return s_windows[i];
			}
		}

		return nullptr;
	}

	// Utility for calculating the border dimensions, based on the given window style.
	ALWAYS_INLINE static Window::WindowBorder calculate_border(DWORD window_style)
	{
		RECT window_border = {};
		AdjustWindowRect(&window_border, window_style, NULL);

		Window::WindowBorder border = {};
		border.left = -window_border.left;
		border.right = window_border.right;
		border.top = -window_border.top;
		border.bottom = window_border.bottom;

		return border;
	}
};

Window::Window(const WindowDescription& description)
	: m_native_handle(NULL)
	, m_event_callback(description.event_callback)
	, m_width(description.width)
	, m_height(description.height)
	, m_position_x(description.position_x)
	, m_position_y(description.position_y)
	, m_title(description.title)
	, m_view_mode(description.view_mode)
	, m_is_dirty(false)
	, m_is_pending_view_mode_switch(false)
	, m_is_pending_kill(false)
	, m_border{}

	// m_saved_* and m_dirty_* are set during the constructor.
{
	// The Win32 API requires us to register a window class, only once. Using a static_persistent
	//   variable is the most straightforward and easy to understand solution.
	static_persistent bool s_was_class_registered = false;
	if (!s_was_class_registered)
	{
		s_was_class_registered = true;
		WNDCLASS window_class = {};
		window_class.lpfnWndProc = WindowInternalCalls::event_procedure;
		window_class.lpszClassName = TEXT("HiccupWindowClass");
		window_class.hInstance = GetModuleHandle(NULL);
		RegisterClass(&window_class);
	}

	// We store the window's size as its client area.
	// But CreateWindow requires the dimensions as the window full size.
	// As a result, we create these temporary values, only used for
	//   the window creation OS call.
	int create_window_width = 0;
	int create_window_height = 0;
	
	int create_window_position_x = 0;
	int create_window_position_y = 0;

	// Getting the default monitor position and dimensions. Used for calculating the required
	//   window dimension of a maximized or fullscreen window.
	// We get the monitor information which corresponds to where the window would have been
	//   created if none of the mentioned above flags would have been specified.
	MONITORINFOEX monitor_info = { sizeof(MONITORINFOEX) };
	GetMonitorInfo(MonitorFromPoint(POINT{ m_position_x, m_position_y }, MONITOR_DEFAULTTOPRIMARY), &monitor_info);

	// We immediately show the window, so the WS_VISIBLE flag will always be specified.
	DWORD window_style = WS_VISIBLE;
	if (m_view_mode == WindowViewMode::windowed)
	{
		window_style |= WS_OVERLAPPEDWINDOW;
		m_border = WindowInternalCalls::calculate_border(window_style);

		// As mention above, these variables are used in CreateWindow, which expects the
		//   full window dimensions. As a result, we add to the desired client area width
		//   and height the border sizes.
		// Because we specify the WS_MAXIMIZE flags, these values will be overridden anyway,
		//   but when we exit the maximized state, we want to restore the parameters specified
		//   in the window description.
		create_window_width = m_width + m_border.left + m_border.right;
		create_window_height = m_height + m_border.top + m_border.bottom;
		create_window_position_x = m_position_x;
		create_window_position_y = m_position_y;

		if (description.start_mode == WindowStartMode::maximized)
		{
			window_style |= WS_MAXIMIZE;

			// The future width of the window will be all the available monitor space width.
			// Using monitor_info.rcWork ensures that the taskbar's dimensions are taken into
			//   account when we compute the window width.
			// A maximized window doesn't have left, bottom or right borders visible in the monitor space.
			m_width = monitor_info.rcWork.right - monitor_info.rcWork.left;

			// Same logic as the window's width calculation.
			// But we need to also account for the window's top border.
			// The window's top border is composed from the title bar and the window's external border,
			//   which is equal on every side. But, due to legacy reasons, only the title bar is
			//   visible in the monitor space. The rest is actually invisible. As a result, we calculate
			//   the window height as the available monitor height (without the taskbar), minus the
			//   titlebar size.
			m_height = monitor_info.rcWork.bottom - monitor_info.rcWork.top - (m_border.top - m_border.bottom);

			// Because a maximized window has an invisible border, the window position is the monitor's
			//   top-left available corner, minus the border size.
			m_position_x = monitor_info.rcWork.left - (int32)m_border.left;
			m_position_y = monitor_info.rcWork.top - (int32)m_border.bottom;
		}
		else if (description.start_mode == WindowStartMode::minimized)
		{
			window_style |= WS_MINIMIZE;

			// A minimized window dimensions are required to be 0.
			m_width = 0;
			m_height = 0;

			// Magic numbers :)
			m_position_x = -32000;
			m_position_y = -32000;
		}
	}
	else
	{
		window_style |= WS_POPUP;
		m_border = WindowInternalCalls::calculate_border(window_style);

		m_saved_width = m_width;
		m_saved_height = m_height;
		m_saved_position_x = m_position_x;
		m_saved_position_y = m_position_y;

		// A fullscreen (popup) window has no borders.
		// Also, the window fills the whole monitor space, including the taskbar.
		m_width = monitor_info.rcMonitor.right - monitor_info.rcMonitor.left;
		m_height = monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top;
		m_position_x = monitor_info.rcMonitor.left;
		m_position_y = monitor_info.rcMonitor.top;

		// Because a fullscreen window has no borders, the full window dimensions are
		//   the same as the window's client's.
		create_window_width = m_width;
		create_window_height = m_height;
	}
	
	// TODO(Traian): Convert the title to UCS-2, and actually use it.
	HWND window_handle = CreateWindow(
		TEXT("HiccupWindowClass"),
		TEXT("Hello, World"), window_style,
		create_window_position_x, create_window_position_y, create_window_width, create_window_height,
		NULL, NULL, GetModuleHandle(NULL), NULL
	);

	HC_ASSERT(window_handle != NULL);
	m_native_handle = window_handle;

	s_windows.add(this);

	m_dirty_width = m_width;
	m_dirty_height = m_height;
	m_dirty_position_x = m_position_x;
	m_dirty_position_y = m_position_y;
}

Window::~Window()
{
	usize index;
	for (index = 0; index < s_windows.size(); ++index)
	{
		if (s_windows[index] == this)
		{
			break;
		}
	}
	for (index; index < s_windows.size() - 1; ++index)
	{
		s_windows[index] = s_windows[index + 1];
	}
	s_windows.pop();

	DestroyWindow((HWND)m_native_handle);
}

void Window::set_width(uint32 new_width)
{
	if (new_width != m_dirty_width)
	{
		m_dirty_width = new_width;
		m_is_dirty = true;
	}
}

void Window::set_height(uint32 new_height)
{
	if (new_height != m_dirty_height)
	{
		m_dirty_height = new_height;
		m_is_dirty = true;
	}
}

void Window::set_position_x(int32 new_position_x)
{
	if (new_position_x != m_dirty_position_x)
	{
		m_dirty_position_x = new_position_x;
		m_is_dirty = true;
	}
}

void Window::set_position_y(int32 new_position_y)
{
	if (new_position_y != m_dirty_position_y)
	{
		m_dirty_position_y = new_position_y;
		m_is_dirty = true;
	}
}

void Window::set_title(StringView new_title)
{
	m_title = String::from_view(new_title);
	// TODO(Traian): Convert UTF-8 to UCS-2 (Windows Unicode charset), instead of using the ANSI version of the function.
	SetWindowTextA((HWND)m_native_handle, new_title.c_str());
}

void Window::set_view_mode(WindowViewMode new_view_mode)
{
	m_is_pending_view_mode_switch = (new_view_mode != m_view_mode);
}

void Window::update_window()
{
	// The window's message pump.
	MSG message = {};
	while (PeekMessage(&message, (HWND)m_native_handle, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	if (m_is_dirty)
	{
		// Calling this function causes an OS event to be fired. By handling it,
		//   on_resized() and on_moved() might be called, if required. Those functions
		//   set the window's parameters to their new values.
		// SetWindowPos() expects the full window's position and dimensions. That's why
		//   the window's border size is added to the width and height.
		SetWindowPos(
			(HWND)m_native_handle, HWND_TOP,
			m_dirty_position_x,
			m_dirty_position_y,
			m_dirty_width + m_border.left + m_border.right,
			m_dirty_height + m_border.top + m_border.bottom,
			SWP_FRAMECHANGED
		);
		m_is_dirty = false;
	}

	if (m_is_pending_view_mode_switch)
	{
		if (m_view_mode == WindowViewMode::windowed)
		{
			m_view_mode = WindowViewMode::fullscreen;

			// Save the current window parameters. They will be restored when the window will exit fullscreen.
			m_saved_width = m_width;
			m_saved_height = m_height;
			m_saved_position_x = m_position_x;
			m_saved_position_y = m_position_y;

			// Window will be fullscreened on the monitor that it currently is displayed on.
			MONITORINFO monitor_info = { sizeof(MONITORINFO) };
			GetMonitorInfo(MonitorFromWindow((HWND)m_native_handle, MONITOR_DEFAULTTOPRIMARY), &monitor_info);

			const DWORD new_window_style = WS_VISIBLE | WS_POPUP;
			// On Windows, a fullscreen window has no border. So this will be (0, 0, 0, 0).
			m_border = WindowInternalCalls::calculate_border(new_window_style);

			SetWindowLong((HWND)m_native_handle, GWL_STYLE, new_window_style);

			// As mentioned previous, a fullscreen window has no borders. Also, it fills the entire monitor space,
			//   including the taskbar. So, that's why the window position and dimensions just ignore the window border.
			// Calling this function causes an OS event to be fired. By handling it, on_resized() and on_moved() might be
			//   called, if required. Those functions set the window's parameters to their new values.
			SetWindowPos(
				(HWND)m_native_handle, HWND_TOP,
				monitor_info.rcMonitor.left, monitor_info.rcMonitor.top,
				monitor_info.rcMonitor.right - monitor_info.rcMonitor.left,
				monitor_info.rcMonitor.bottom - monitor_info.rcMonitor.top,
				SWP_FRAMECHANGED
			);
		}
		else
		{
			m_view_mode = WindowViewMode::windowed;

			const DWORD new_window_style = WS_VISIBLE | WS_OVERLAPPEDWINDOW;
			m_border = WindowInternalCalls::calculate_border(new_window_style);

			SetWindowLong((HWND)m_native_handle, GWL_STYLE, new_window_style);

			// SetWindowPos() expects the full window's position and dimensions. That's why
			//   the window's border size is added to the width and height.
			// Calling this function causes an OS event to be fired. By handling it, on_resized() and on_moved() might be
			//   called, if required. Those functions set the window's parameters to their new values.
			SetWindowPos(
				(HWND)m_native_handle, HWND_TOP,
				m_saved_position_x,
				m_saved_position_y,
				m_saved_width + m_border.left + m_border.right,
				m_saved_height + m_border.top + m_border.bottom,
				SWP_FRAMECHANGED
			);
		}

		m_is_pending_view_mode_switch = false;
	}
}

// Wrapper around WindowInternalCalls::find_window_by_handle().
// If the returned window instance is nullptr, the event will not be handled.
// This macro can only be used in WindowInternalCalls::event_procedure().
#define HC_GET_WINDOW_INSTANCE()            \
	find_window_by_handle(window_handle);   \
	if (!window)                            \
	{                                       \
		use_default_proc = true;            \
		break;                              \
	}

LRESULT WindowInternalCalls::event_procedure(HWND window_handle, UINT message, WPARAM w_param, LPARAM l_param)
{
	LRESULT return_value;
	bool use_default_proc = false;
	
	switch (message)
	{
		case WM_CLOSE:
		{
			Window* window = HC_GET_WINDOW_INSTANCE();
			window->on_closed();
			return_value = 0;
			break;
		}

		case WM_SIZE:
		{
			const uint32 width = (uint32)(LOWORD(l_param));
			const uint32 height = (uint32)(HIWORD(l_param));

			Window* window = HC_GET_WINDOW_INSTANCE();
			window->on_resized(width, height);
			return_value = 0;
			break;
		}

		case WM_MOVE:
		{
			const POINTS position = MAKEPOINTS(l_param);

			Window* window = HC_GET_WINDOW_INSTANCE();
			// WM_MOVE gives the position of the top-left window's client area corner.
			// But, the window stores the position of the full window's top-left corner. So,
			//   we subtract the window's left, respectively top, border size.
			window->on_moved(position.x - window->m_border.left, position.y - window->m_border.top);
			return_value = 0;
			break;
		}
		default:
		{
			use_default_proc = true;
			break;
		}
	}
	
	if (use_default_proc)
	{
		return_value = DefWindowProc(window_handle, message, w_param, l_param);
	}

	return return_value;
}

} // namespace HC

#endif // HC_PLATFORM_WINDOWS