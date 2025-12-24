#ifndef _SYSTEM_CURSOR_H
#define _SYSTEM_CURSOR_H

namespace System {
	enum class CursorLockMode {
		/// <summary>
		/// Cursor is locked to within the game window.
		/// </summary>
		Confined,
		/// <summary>
		/// 
		/// </summary>
		None,
		/// <summary>
		/// Cursor is locked to the center of the game window.
		/// </summary>
		Locked,

	};
	class Cursor {
	private:

	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="lock"></param>
		static void setlockState(CursorLockMode lock) {

		}

	};
}

#endif