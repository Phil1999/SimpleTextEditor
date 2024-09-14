#include "SimpleTextEditor.h"
#include <QSaveFile>
#include <QMessageBox>
#include <QFileDialog>

SimpleTextEditor::SimpleTextEditor(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    setupLayout();
    createActions();
    createMenus();
}

SimpleTextEditor::~SimpleTextEditor()
{
}

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
    newAct = new QAction(tr("New"), this);
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
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(copyAct);
    editMenu->addAction(cutAct);
    editMenu->addAction(pasteAct);
}

void SimpleTextEditor::newFile()
{
    textEdit->clear();
    currentFile.clear();
}

void SimpleTextEditor::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
        tr("Text Files (*.txt);;All Files (*)"));

    if (!fileName.isEmpty())
    {
        openFromFile(fileName);
    }
}

void SimpleTextEditor::openFromFile(const QString& fileName)
{
    if (!QFile::exists(fileName)) {
        return;
    }

    QFile file(fileName);

    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream in(&file);
        in.setEncoding(QStringConverter::Utf8);  // Set encoding to UTF-8
        textEdit->setPlainText(in.readAll());
        currentFile = fileName;
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Failed to open the file."));
    }
}

void SimpleTextEditor::saveFile()
{
    if (currentFile.isEmpty())
    {
        // No current file; so behave like "Save as"
        saveFileAs();
    }
    else
    {
        // Save to current file without changing currentFile (since we didn't switch files)
        saveToFile(currentFile);
    }
}

void SimpleTextEditor::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), QString(),
        tr("Text Files (*.txt);;All Files (*)"));

    if (!fileName.isEmpty())
    {
        currentFile = fileName;
        saveToFile(fileName);

    }
}

void SimpleTextEditor::saveToFile(const QString& fileName)
{

    if (!QFile::exists(fileName)) {
        return;
    }

    QSaveFile file(fileName);
    if (file.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&file);
        out.setEncoding(QStringConverter::Utf8);  // Set encoding to UTF-8
        out << textEdit->toPlainText();

        if (!file.commit())
        {
            QMessageBox::critical(this, tr("Error"), tr("Failed to save the file."));
        }
    }
    else
    {
        QMessageBox::critical(this, tr("Error"), tr("Failed to open the file for writing."));
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
