#include "SimpleTextEditor.h"
#include <QSaveFile>
#include <QMessageBox>
#include <QFileDialog>
#include <qevent.h>
#include <qtextstream.h>

SimpleTextEditor::SimpleTextEditor(QWidget* parent)
    : QMainWindow(parent), isUntitled(true), isModified(false)
{
    ui.setupUi(this);
    setupLayout();
    createActions();
    createMenus();

    // Connect textChanged signal to keep track of modifications.
    connect(textEdit, &QTextEdit::textChanged, this, &SimpleTextEditor::documentWasModified);

    setCurrentFile("");
}

SimpleTextEditor::~SimpleTextEditor() = default;

void SimpleTextEditor::setupLayout()
{
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    layout = new QVBoxLayout(centralWidget);
    textEdit = new QTextEdit(this);

    layout->addWidget(textEdit);

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
    if (maybeSave()) {
        textEdit->clear();
        setCurrentFile("");
    }

}

void SimpleTextEditor::openFile()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
            tr("Text Files (*.txt);;All Files (*)"));

        if (!fileName.isEmpty())
        {
            openFromFile(fileName);
        }
    }
}

bool SimpleTextEditor::openFromFile(const QString& fileName)
{

    QFile file(fileName);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("SimpleTextEditor"),
            tr("Cannot read file %1: \n%2.")
            .arg(QDir::toNativeSeparators(fileName), file.errorString()));

        return false;

    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);  // Set encoding to UTF-8
    textEdit->setPlainText(in.readAll());

    setCurrentFile(fileName);
    return true;
}

bool SimpleTextEditor::saveFile()
{
    if (isUntitled)
    {
        // No current file; so behave like "Save as"
        return saveFileAs();
    }
    else
    {
        // Save to current file without changing currentFile (since we didn't switch files)
        return saveToFile(currentFile);
    }
}

bool SimpleTextEditor::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"), QString(),
        tr("Text Files (*.txt);;All Files (*)"));

    // User didn't give a filename.
    if (fileName.isEmpty())
    {
        return false;
    }

    return saveToFile(fileName);
}

bool SimpleTextEditor::saveToFile(const QString& fileName)
{
    QSaveFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("SimpleTextEditor"),
            tr("Cannot write file %1: \n%2.")
            .arg(QDir::toNativeSeparators(fileName), file.errorString()));

        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);  // Set encoding to UTF-8
    out << textEdit->toPlainText();

    if (!file.commit())
    {
        QMessageBox::warning(this, tr("SimpleTextEditor"),
            tr("Cannot write file %1:\n%2.")
            .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return false;
    }

    // We have recognized a valid file and can write to it without problems.
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 5000);

    return true;

}

void SimpleTextEditor::closeEvent(QCloseEvent* event)
{
    if (maybeSave()) {
        event->accept();
    }
    else {
        event->ignore();
    }
}

void SimpleTextEditor::documentWasModified()
{
    isModified = true;
    setWindowModified(true);
}

bool SimpleTextEditor::maybeSave()
{
    if (!isModified) return true;

    const QMessageBox::StandardButton ret = 
        QMessageBox::warning(this, tr("SimpleTextEditor"),
        tr("The document has been modified. \n"
            "Do you want to save your changes?"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    switch (ret) {

    case QMessageBox::Save:
        return saveFile();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }

    return true;
}

void SimpleTextEditor::setCurrentFile(const QString& fileName)
{
    currentFile = fileName;
    isUntitled = fileName.isEmpty();
    isModified = false;
    setWindowModified(false);
    setWindowTitle(tr("%1[*] - %2").arg(isUntitled ? "untitled" : QFileInfo(currentFile).fileName(), tr("SimpleTextEditor")));
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
