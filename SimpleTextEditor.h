#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SimpleTextEditor.h"

class SimpleTextEditor : public QMainWindow
{
    Q_OBJECT

public:
    SimpleTextEditor(QWidget *parent = nullptr);
    ~SimpleTextEditor();

private:
    Ui::SimpleTextEditorClass ui;
};
