#include "Precompiled.h"
#include "WindowsMessageHandler.h"

#include "DebugUtil.h"

using namespace Angazi::Core;

void Angazi::Core::WindowMessageHandler::Hook(HWND window, Callback cb)
{
	mWindow = window;
	mPreviousCallback = (Callback)GetWindowLongPtrA(window, GWLP_WNDPROC);
	SetWindowLongPtrA(window,GWLP_WNDPROC, (LONG_PTR) cb);
}

void Angazi::Core::WindowMessageHandler::Unhook()
{
	SetWindowLongPtrA(mWindow, GWLP_WNDPROC, (LONG_PTR)mPreviousCallback);
	mWindow = nullptr;
}

LRESULT Angazi::Core::WindowMessageHandler::ForwardMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	ASSERT(mPreviousCallback, "[WindowMessageHandler] No callback is hooked.");
	return CallWindowProcA((WNDPROC) mPreviousCallback, window,message,wParam,lParam);
}
