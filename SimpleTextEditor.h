#pragma once

#include <QtWidgets/QMainWindow>
#include <qtextedit.h>
#include <qmenu.h>
#include <qaction.h>
#include <qboxlayout.h>
#include "ui_SimpleTextEditor.h"

class SimpleTextEditor : public QMainWindow
{
    Q_OBJECT

public:
    SimpleTextEditor(QWidget *parent = nullptr);
    ~SimpleTextEditor();

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void copyText();
    void cutText();
    void pasteText();

private:
    void createActions();
    void createMenus();
    void setupLayout();

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

};
