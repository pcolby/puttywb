#include "puttyinstance-win.h"

#include <windows.h>

PuttyInstance::PuttyInstance(const QString &command, const QString &arguments)
    : windowHandle(NULL), originalParent(NULL), originalLongPtr(0), error(NO_ERROR)
{
    // Initialise our process / thread handles to NULL.
    processInfo.hProcess = processInfo.hThread = NULL;

    // Setup the process start-up info structure.
    STARTUPINFO startupInfo;
    ZeroMemory(&startupInfo, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
    startupInfo.wShowWindow = SW_HIDE; // Don't show the PuTTY window initially.
    startupInfo.dwFlags = STARTF_USESHOWWINDOW;


    // Create a new PuTTY process.

#ifdef UNICODE
    LPTSTR args = _wcsdup(arguments.toStdWString().c_str());
    const BOOL result = CreateProcess(command.toStdWString().c_str(), args
#else
    const BOOL result = CreateProcess(command.toAscii(),arguments.toAscii()
#endif
            ,NULL, NULL, FALSE, 0, NULL, NULL, &startupInfo, &processInfo);
    if (!result) {
        error = GetLastError();
        return;
    }
}

bool PuttyInstance::isNull() {
    return ((processInfo.hProcess == NULL) || (processInfo.hThread == NULL));
}

bool PuttyInstance::isValid() {
    return (processInfo.hProcess != NULL);
}

bool PuttyInstance::findWindow() {
    if (isNull()) return NULL;

    if (windowHandle == NULL) {
        HWND hWnd = NULL;
        while ((windowHandle == NULL) && ((hWnd = FindWindowEx(NULL, hWnd, TEXT("putty"), NULL)) != NULL)) {
            if (GetWindowThreadProcessId(hWnd, NULL) == processInfo.dwThreadId)
                windowHandle = hWnd;
        }
    }

    return (windowHandle != NULL);
}

bool PuttyInstance::isWindowClosed() {
    return ((windowHandle != NULL) && (error == ERROR_INVALID_WINDOW_HANDLE));
}

/*WId PuttyInstance::windowId() {
    return windowHandle;
}*/

bool PuttyInstance::adopt(QWidget *parent) {
    if (isNull()) return false;

    if ((windowHandle == NULL) && (!findWindow()))
        return false;

    SetLastError(NO_ERROR);
    LONG_PTR previousLongPtr = SetWindowLongPtr(windowHandle, GWL_STYLE, WS_CHILD|WS_VISIBLE|WS_VSCROLL);
    if (previousLongPtr == 0) {
        DWORD error = GetLastError();
        if (error != NO_ERROR) {
            this->error = error;
            return false;
        }
    } else if (originalLongPtr == 0) {
        originalLongPtr = previousLongPtr;
    }

    const HWND previousParent = SetParent(windowHandle, parent->winId());
    if (previousParent == NULL) {
        error = GetLastError();
        return false;
    } else if (originalParent == NULL) {
        originalParent = previousParent;
    }
    return true;
}

bool PuttyInstance::disown() {
    if ((isNull()) || (windowHandle == NULL)) return false;

    SetLastError(NO_ERROR);
    if (SetWindowLongPtr(windowHandle, GWL_STYLE, originalLongPtr) == 0) {
        DWORD error = GetLastError();
        if (error != NO_ERROR) {
            this->error = error;
            return false;
        }
    }

    if (SetParent(windowHandle, originalParent) == NULL) {
        error = GetLastError();
        return false;
    }

    return true;
}

bool PuttyInstance::resize(const QSize &size) {
    if (windowHandle != NULL) {
        // Figure out the PuTTY window adornment dimensions.
        RECT rect;
        GetWindowRect(windowHandle, &rect);
        POINT topLeft;
        topLeft.x = rect.left;
        topLeft.y = rect.top;
        ScreenToClient(windowHandle, &topLeft);

        // Resize the PuTTY window to fit this widget.
        // TODO: error checking.
        SendMessage(windowHandle, WM_ENTERSIZEMOVE, 0, 0);
        SetWindowPos(windowHandle, NULL, topLeft.x, topLeft.y, size.width() - ( 2 * topLeft.x),
                     size.height() - topLeft.y - topLeft.x, SWP_NOZORDER);
        SendMessage(windowHandle, WM_EXITSIZEMOVE, 0, 0);
    }
    return true;
}

bool PuttyInstance::setFocus() {
    // TODO: errror handling.
    if (windowHandle != NULL) {
        SetForegroundWindow(windowHandle);
        SetFocus(windowHandle);
    }
    return true;
}

QString PuttyInstance::windowTitle() {
    if ((isNull()) || (windowHandle == NULL))
        return QString::null;

    SetLastError(ERROR_SUCCESS);
    int length = GetWindowTextLength(windowHandle);
    if (length == 0) {
        error = GetLastError(); // Might be NO_ERROR if the window has no title.
        return QString::null;
    }
    length++; // +1 to allow for a \0 terminator.

    QString title;
    TCHAR * const buffer = new TCHAR[length];
    SetLastError(ERROR_SUCCESS);
    if ((length = GetWindowText(windowHandle, buffer, length)) == 0){
        error = GetLastError();
    } else {
        #ifdef UNICODE
        title = QString::fromUtf16((const ushort *)buffer, length);
        #else
        title = QString::fromLocal8Bit((const ushort *)buffer, length);
        #endif
    }

    delete buffer;
    return title;
}
