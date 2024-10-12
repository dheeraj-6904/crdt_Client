#include "TextEditorUI.h"
#include "FileManager.h"
#include <FL/Fl_Native_File_Chooser.H>
#include <iostream>

TextEditorUI::TextEditorUI() {
    // Create the main window and editor
    window = new Fl_Window(800, 600, "Collaborative Text Editor");
    textBuffer = new Fl_Text_Buffer();
    editor = new Fl_Text_Editor(10, 40, 780, 550);
    editor->buffer(textBuffer);

    // Set callback for text changes
    textBuffer->add_modify_callback(cb_text_changed, this);
    
    // Create the menu bar
    createMenuBar();

    // Initialize NetworkManager (connect to server)
    networkManager = new NetworkManager();
    connectToServer();
}

TextEditorUI::~TextEditorUI() {
    delete editor;
    delete textBuffer;
    delete window;
    delete networkManager;
}

void TextEditorUI::createMenuBar() {
    menuBar = new Fl_Menu_Bar(0, 0, 800, 40);
    menuBar->add("&File/Open", 0, cb_open, this);
    menuBar->add("&File/Save", 0, cb_save, this);
    menuBar->add("&File/Delete", 0, cb_delete, this);
    menuBar->add("&File/Quit", 0, cb_quit, this);
}

void TextEditorUI::show() {
    window->end();
    window->show();
}

void TextEditorUI::connectToServer() {
    std::string ip = fl_input("Enter server IP:", "127.0.0.1");
    if (!networkManager->connectToServer(ip, 8080)) {
        fl_alert("Unable to connect to server!");
    }
}

// Menu callbacks
void TextEditorUI::cb_open(Fl_Widget* widget, void* data) {
    TextEditorUI* editor = (TextEditorUI*) data;
    Fl_Native_File_Chooser chooser;
    chooser.title("Open File");
    chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
    if (chooser.show() == 0) {
        std::string filename = chooser.filename();
        FileManager fileManager;
        std::string content = fileManager.openFile(filename);
        editor->textBuffer->text(content.c_str());
    }
}

void TextEditorUI::cb_save(Fl_Widget* widget, void* data) {
    TextEditorUI* editor = (TextEditorUI*) data;
    Fl_Native_File_Chooser chooser;
    chooser.title("Save File");
    chooser.type(Fl_Native_File_Chooser::BROWSE_SAVE_FILE);
    if (chooser.show() == 0) {
        std::string filename = chooser.filename();
        FileManager fileManager;
        fileManager.saveFile(filename, editor->textBuffer->text());
    }
}

void TextEditorUI::cb_delete(Fl_Widget* widget, void* data) {
    TextEditorUI* editor = (TextEditorUI*) data;
    Fl_Native_File_Chooser chooser;
    chooser.title("Delete File");
    chooser.type(Fl_Native_File_Chooser::BROWSE_FILE);
    if (chooser.show() == 0) {
        std::string filename = chooser.filename();
        FileManager fileManager;
        fileManager.deleteFile(filename);
    }
}

void TextEditorUI::cb_quit(Fl_Widget* widget, void* data) {
    exit(0);
}

// Text change callback
void TextEditorUI::cb_text_changed(int pos, int inserted, int deleted, int restyled, const char* deleted_text, void* data) {
    TextEditorUI* editor = (TextEditorUI*) data;
    int x = editor->editor->insert_position();  // Current cursor position
    int y = 0; // Could map row/column later

    std::string changeOperation = editor->textBuffer->text_range(pos, pos + inserted);
    editor->networkManager->sendChangeToServer(changeOperation, x, y);
}
