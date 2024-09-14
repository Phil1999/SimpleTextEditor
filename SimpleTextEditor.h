// SimpleTextEditor.h
//
// Definitions for the SimpleTextEditor class, a basic text editing
// interface with file operations and text manipulation functionality using QT.

#pragma once

#include <QtWidgets/QMainWindow>
#include <QTextEdit>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include "ui_SimpleTextEditor.h"


class SimpleTextEditor : public QMainWindow {
public:
	SimpleTextEditor(QWidget* parent = nullptr);
	~SimpleTextEditor();

	// Creates a new, empty file in the editor.
	void NewFile();

	// Saves the current text to a file.
	// Returns true if the save was successful, false otherwise.
	bool SaveToFile(const QString& file_name);

	// Opens and reads a file into the editor.
	// Returns true if the file was successfully opened and read, false otherwise.
	bool OpenFromFile(const QString& file_name);

	// Returns a pointer to the editor's QTextEdit widget.
	QTextEdit* GetTextEdit() const { return text_edit_; }

public slots:
	// Opens a file dialog and calls OpenFromFile with the selected file.
	void OpenFile();

	// Saves to current file or opens save dialog if no file is set.
	bool SaveFile();

	// Opens save dialog and calls SaveToFile with the selected file.
	bool SaveFileAs();

	// Copies the selected text
	void CopyText();

	// Cuts the selected text
	void CutText();

	// Pastes the text in your clipboard.
	void PasteText();

protected:
	// Handles the window close event, prompting a dialog save unsaved changes.
	void closeEvent(QCloseEvent* event) override;

private:
	// Creates all actions for menus and toolbars.
	void CreateActions();

	// Sets up all menus in the menu bar.
	void CreateMenus();

	// Initializes the layout of the main window.
	void SetupLayout();

	// Checks for unsaved changes and prompts user to save if necessary.
	// Returns true if it's safe to continue, false if the operation should be cancelled.
	bool MaybeSave();

	// Updates the current file name and related UI elements.
	void SetCurrentFile(const QString& file_name);

	// Slot called when the document is modified.
	void DocumentWasModified();

	Ui::SimpleTextEditorClass ui;
	QTextEdit* text_edit_;
	QVBoxLayout* layout_;
	QMenu* file_menu_;
	QMenu* edit_menu_;
	QAction* new_act_;
	QAction* open_act_;
	QAction* save_act_;
	QAction* save_as_act_;
	QAction* exit_act_;
	QAction* copy_act_;
	QAction* cut_act_;
	QAction* paste_act_;
	QString current_file_;
	bool is_untitled_;
	bool is_modified_;
};