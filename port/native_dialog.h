#pragma once

#include <string>
#include <vector>

namespace ssb64 {

// Show a native "Open File" dialog. Blocks until the user picks a file
// or cancels. Returns the selected path, or "" on cancel / error.
//
// `title` is the dialog title.
// `extensions` is the file extension whitelist without leading dots,
//   e.g. {"z64", "n64", "v64"}. Empty means "any file".
//
// Implemented per-platform:
//   macOS:   shells out to osascript with `choose file`.
//   Linux:   shells out to zenity, falls back to kdialog.
//   Windows: GetOpenFileNameW from commdlg.h.
//   Other:   returns "" (caller falls back to the wizard's text input).
std::string OpenFileDialog(const std::string& title,
                           const std::vector<std::string>& extensions);

} // namespace ssb64
