// SimpleTextEditor.cpp
//
// Implements the SimpleTextEditor class which provides basic text editing
// functionality including file operations and text manipulation.

#include "SimpleTextEditor.h"
#include <QSaveFile>
#include <QMessageBox>
#include <QFileDialog>
#include <qevent.h>
#include <QTextStream>

SimpleTextEditor::SimpleTextEditor(QWidget* parent)
    : QMainWindow(parent), is_untitled_(true), is_modified_(false) {

    ui.setupUi(this);
    SetupLayout();
    CreateActions();
    CreateMenus();

    // Connect textChanged signal to keep track of modifications.
    connect(text_edit_, &QTextEdit::textChanged, this, &SimpleTextEditor::DocumentWasModified);

    SetCurrentFile("");
}

SimpleTextEditor::~SimpleTextEditor() = default;

void SimpleTextEditor::SetupLayout() {

    QWidget* central_widget = new QWidget(this);
    setCentralWidget(central_widget);

    layout_ = new QVBoxLayout(central_widget);
    text_edit_ = new QTextEdit(this);

    layout_->addWidget(text_edit_);
}

void SimpleTextEditor::CreateActions() {

    new_act_ = new QAction(tr("New"), this);
    new_act_->setShortcuts(QKeySequence::New);
    connect(new_act_, &QAction::triggered, this, &SimpleTextEditor::NewFile);

    open_act_ = new QAction(tr("Open File..."), this);
    open_act_->setShortcuts(QKeySequence::Open);
    connect(open_act_, &QAction::triggered, this, &SimpleTextEditor::OpenFile);

    save_act_ = new QAction(tr("Save"), this);
    save_act_->setShortcuts(QKeySequence::Save);
    connect(save_act_, &QAction::triggered, this, &SimpleTextEditor::SaveFile);

    save_as_act_ = new QAction(tr("Save As..."), this);
    save_as_act_->setShortcuts(QKeySequence::SaveAs);
    connect(save_as_act_, &QAction::triggered, this, &SimpleTextEditor::SaveFileAs);

    exit_act_ = new QAction(tr("Exit"), this);
    exit_act_->setShortcuts(QKeySequence::Quit);
    connect(exit_act_, &QAction::triggered, this, &QWidget::close);

    copy_act_ = new QAction(tr("Copy"), this);
    copy_act_->setShortcuts(QKeySequence::Copy);
    connect(copy_act_, &QAction::triggered, this, &SimpleTextEditor::CopyText);

    cut_act_ = new QAction(tr("Cut"), this);
    cut_act_->setShortcuts(QKeySequence::Cut);
    connect(cut_act_, &QAction::triggered, this, &SimpleTextEditor::CutText);

    paste_act_ = new QAction(tr("Paste"), this);
    paste_act_->setShortcuts(QKeySequence::Paste);
    connect(paste_act_, &QAction::triggered, this, &SimpleTextEditor::PasteText);
}

void SimpleTextEditor::CreateMenus() {

    // File Menu
    file_menu_ = menuBar()->addMenu(tr("&File"));
    file_menu_->addAction(new_act_);
    file_menu_->addAction(open_act_);
    file_menu_->addAction(save_act_);
    file_menu_->addAction(save_as_act_);
    file_menu_->addSeparator();
    file_menu_->addAction(exit_act_);

    // Edit Menu
    edit_menu_ = menuBar()->addMenu(tr("&Edit"));
    edit_menu_->addAction(copy_act_);
    edit_menu_->addAction(cut_act_);
    edit_menu_->addAction(paste_act_);
}

void SimpleTextEditor::NewFile() {

    if (MaybeSave()) {
        text_edit_->clear();
        SetCurrentFile("");
    }
}

void SimpleTextEditor::OpenFile() {

    if (MaybeSave()) {
        QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"), QString(),
            tr("Text Files (*.txt);;All Files (*)"));

        if (!file_name.isEmpty()) {
            OpenFromFile(file_name);
        }
    }
}

bool SimpleTextEditor::OpenFromFile(const QString& file_name) {

    QFile file(file_name);

    if (!file.open(QFile::ReadOnly | QFile::Text)) {

        QMessageBox::warning(this, tr("SimpleTextEditor"),
            tr("Cannot read file %1:\n%2.")
            .arg(QDir::toNativeSeparators(file_name), file.errorString()));
        return false;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);  // Set encoding to UTF-8
    text_edit_->setPlainText(in.readAll());

    SetCurrentFile(file_name);
    return true;
}

bool SimpleTextEditor::SaveFile() {

    // If its a fresh file function like "save as"
    if (is_untitled_) {
        return SaveFileAs();
    }
    else {
        return SaveToFile(current_file_);
    }
}

bool SimpleTextEditor::SaveFileAs() {

    QString file_name = QFileDialog::getSaveFileName(this, tr("Save As"), QString(),
        tr("Text Files (*.txt);;All Files (*)"));

    if (file_name.isEmpty()) {
        return false;
    }

    return SaveToFile(file_name);
}

bool SimpleTextEditor::SaveToFile(const QString& file_name) {

    QSaveFile file(file_name);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("SimpleTextEditor"),
            tr("Cannot write file %1:\n%2.")
            .arg(QDir::toNativeSeparators(file_name), file.errorString()));
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);  // Set encoding to UTF-8
    out << text_edit_->toPlainText();

    if (!file.commit()) {
        QMessageBox::warning(this, tr("SimpleTextEditor"),
            tr("Cannot write file %1:\n%2.")
            .arg(QDir::toNativeSeparators(file_name), file.errorString()));
        return false;
    }

    SetCurrentFile(file_name);
    statusBar()->showMessage(tr("File saved"), 5000);
    return true;
}

void SimpleTextEditor::closeEvent(QCloseEvent* event) {
    if (MaybeSave()) {
        event->accept();
    }
    else {
        event->ignore();
    }
}

void SimpleTextEditor::DocumentWasModified() {
    is_modified_ = true;
    setWindowModified(true);
}

bool SimpleTextEditor::MaybeSave() {
    if (!is_modified_) return true;

    const QMessageBox::StandardButton ret =
        QMessageBox::warning(this, tr("SimpleTextEditor"),
            tr("The document has been modified.\n"
                "Do you want to save your changes?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);

    switch (ret) {

    case QMessageBox::Save:
        return SaveFile();
    case QMessageBox::Cancel:
        return false;
    default:
        break;
    }

    return true;
}

void SimpleTextEditor::SetCurrentFile(const QString& file_name) {
    current_file_ = file_name;
    is_untitled_ = file_name.isEmpty();
    is_modified_ = false;
    setWindowModified(false);
    setWindowTitle(tr("%1[*] - %2").arg(is_untitled_ ? "untitled" : QFileInfo(current_file_).fileName(), tr("SimpleTextEditor")));
}

void SimpleTextEditor::CopyText() {
    text_edit_->copy();
}

void SimpleTextEditor::CutText() {
    text_edit_->cut();
}

void SimpleTextEditor::PasteText() {
    text_edit_->paste();
}