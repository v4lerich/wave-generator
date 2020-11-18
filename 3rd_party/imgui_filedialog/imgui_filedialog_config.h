#ifndef WAVEGENERATOR_IMGUI_FILEDIALOG_CONFIG_H
#define WAVEGENERATOR_IMGUI_FILEDIALOG_CONFIG_H

#ifdef WIN32
#include <windows.h>
#endif

// uncomment and modify defines under for customize ImGuiFileDialog

#define MAX_FILE_DIALOG_NAME_BUFFER 1024
#define MAX_PATH_BUFFER_SIZE 1024

//#define USE_IMGUI_TABLES

#define USE_EXPLORATION_BY_KEYS
// this mapping by default is for GLFW but you can use another
#include <SDL.h>
// Up key for explore to the top
#define IGFD_KEY_UP SDL_SCANCODE_UP
// Down key for explore to the bottom
#define IGFD_KEY_DOWN SDL_SCANCODE_DOWN
// Enter key for open directory
#define IGFD_KEY_ENTER SDL_SCANCODE_RETURN
// BackSpace for comming back to the last directory
#define IGFD_KEY_BACKSPACE SDL_SCANCODE_DOWN

// widget
// filter combobox width
#define FILTER_COMBO_WIDTH 120.0f
// button widget use for compose path
#define IMGUI_PATH_BUTTON ImGui::Button
// standar button
#define IMGUI_BUTTON ImGui::Button

// locales string
#define createDirButtonString "+"
#define okButtonString " OK"
#define cancelButtonString " Cancel"
#define resetButtonString "R"
#define drivesButtonString "Drives"
#define searchString "Search"
#define dirEntryString "[DIR] "
#define linkEntryString "[LINK] "
#define fileEntryString "[FILE] "
#define fileNameString "File Name : "
#define buttonResetSearchString "Reset search"
#define buttonDriveString "Drives"
#define buttonResetPathString "Reset to current directory"
#define buttonCreateDirString "Create Directory"

// theses icons will appear in table headers
#define USE_CUSTOM_SORTING_ICON
#include <IconsForkAwesome.h>
#define AscendingIcon ICON_FK_SORT_ASC
#define DescendingIcon ICON_FK_SORT_DESC
#define tableHeaderFilenameString "File name"
#define tableHeaderSizeString "Size"
#define tableHeaderDateString "Date"

//#define USE_BOOKMARK
//#define bookmarkPaneWith 150.0f
//#define IMGUI_TOGGLE_BUTTON ToggleButton
//#define bookmarksButtonString "Bookmark"
//#define bookmarksButtonHelpString "Bookmark"
//#define addBookmarkButtonString "+"
//#define removeBookmarkButtonString "-"


#endif  // WAVEGENERATOR_IMGUI_FILEDIALOG_CONFIG_H
