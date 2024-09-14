#include <qtextedit.h>
#include <qmenu.h>
#include <qsavefile.h>
#include <qmessagebox.h>
#include <qaction.h>
#include <qboxlayout.h>
#include <qfiledialog.h>
#include "SimpleTextEditor.h"

SimpleTextEditor::SimpleTextEditor(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setupLayout();
    createActions();
    createMenus();
}

SimpleTextEditor::~SimpleTextEditor()
{}


void SimpleTextEditor::setupLayout()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    layout = new QVBoxLayout(centralWidget);
    textEdit = new QTextEdit(this);

    layout->addWidget(textEdit);

    textEdit->setPlaceholderText("Hello World");
}

void SimpleTextEditor::createActions()
{
    // Create "alt" shortcut with &
    newAct = new QAction(tr("New"), this);
    // Set a global shortcut
    newAct->setShortcuts(QKeySequence::New);
    connect(newAct, &QAction::triggered, this, &SimpleTextEditor::newFile);

    openAct = new QAction(tr("Open File..."), this);
    openAct->setShortcuts(QKeySequence::Open);
    connect(openAct, &QAction::triggered, this, &SimpleTextEditor::openFile);

    saveAct = new QAction(tr("Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    connect(saveAct, &QAction::triggered, this, &SimpleTextEditor::saveFile);


    saveAsAct = new QAction(tr("Save As..."), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    connect(saveAsAct, &QAction::triggered, this, &SimpleTextEditor::saveFileAs);

    exitAct = new QAction(tr("Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, &QAction::triggered, this, &QWidget::close);

    copyAct = new QAction(tr("Copy"), this);
    copyAct->setShortcuts(QKeySequence::Copy);
    connect(copyAct, &QAction::triggered, this, &SimpleTextEditor::copyText);

    cutAct = new QAction(tr("Cut"), this);
    cutAct->setShortcuts(QKeySequence::Cut);
    connect(cutAct, &QAction::triggered, this, &SimpleTextEditor::cutText);

    pasteAct = new QAction(tr("Paste"), this);
    pasteAct->setShortcuts(QKeySequence::Paste);
    connect(pasteAct, &QAction::triggered, this, &SimpleTextEditor::pasteText);

}

void SimpleTextEditor::createMenus()
{
    // Add Menu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    // Edit Menu
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(copyAct);
    editMenu->addAction(cutAct);
    editMenu->addAction(pasteAct);
}

// TODO currently just clears the file, but ideally it should open in a new tab.
void SimpleTextEditor::newFile()
{
    textEdit->clear();
}

void SimpleTextEditor::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this);

    if (!fileName.isEmpty()) {
        QFile file(fileName);

        // Make sure we have a valid text file.
        if (file.exists() && file.open(QFile::ReadOnly | QFile::Text)) {
            textEdit->setPlainText(file.readAll());
            currentFile = fileName;
        }

    }
}

void SimpleTextEditor::saveFile()
{

    // If there is no file opened, function like "save as".
    if (currentFile.isEmpty()) {
        currentFile = QFileDialog::getSaveFileName(this);
        if (currentFile.isEmpty())
            return;
    }

    QSaveFile file(currentFile);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&file);
        out << textEdit->toPlainText();
        
        // Save the file with changes.
        if (!file.commit()) {
            QMessageBox::critical(this, tr("Error"), tr("Failed to save the file."));
        }

    }
    
}

void SimpleTextEditor::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (!fileName.isEmpty()) {
        currentFile = fileName;
        saveFile();
    }
}


void SimpleTextEditor::copyText()
{
    textEdit->copy();
}

void SimpleTextEditor::cutText()
{
    textEdit->cut();
}

void SimpleTextEditor::pasteText()
{
    textEdit->paste();
}
