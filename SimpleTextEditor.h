#pragma once

#include <QtWidgets/QMainWindow>
#include <QTextEdit>
#include <QMenu>
#include <QAction>
#include <QVBoxLayout>
#include "ui_SimpleTextEditor.h"

class SimpleTextEditor : public QMainWindow
{
    Q_OBJECT

public:
    SimpleTextEditor(QWidget* parent = nullptr);
    ~SimpleTextEditor();

    void newFile();

    // Core functionality methods (business logic)
    bool saveToFile(const QString& fileName);
    bool openFromFile(const QString& fileName);

    // Getter for textEdit (for testing purposes)
    QTextEdit* getTextEdit() const { return textEdit; }

public slots:
    // Slots for UI interactions
    void openFile();       // Opens file dialog and calls openFromFile
    bool saveFile();       // Saves to current file or opens save dialog
    bool saveFileAs();     // Opens save dialog and calls saveToFile

    void copyText();
    void cutText();
    void pasteText();

protected:
    // Override the close event so we can ask user to confirm unsaved changes.
    void closeEvent(QCloseEvent* event) override;

private slots:
    void documentWasModified();

private:
    void createActions();
    void createMenus();
    void setupLayout();

    bool maybeSave();
    void setCurrentFile(const QString& filename);

    Ui::SimpleTextEditorClass ui;

    QTextEdit* textEdit;
    QVBoxLayout* layout;

    QMenu* fileMenu;
    QMenu* editMenu;

    QAction* newAct;
    QAction* openAct;
    QAction* saveAct;
    QAction* saveAsAct;
    QAction* exitAct;
    QAction* copyAct;
    QAction* cutAct;
    QAction* pasteAct;

    QString currentFile;

    // File status flags
    bool isUntitled;
    bool isModified;
};