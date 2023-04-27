#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QOpenGLWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QComboBox>
#include <QListWidget>
#include "openglscene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_blockTypeComboBox_currentIndexChanged(int index);
    void on_blockListWidget_itemClicked(QListWidgetItem *item);
    void on_playButton_clicked();
    void on_stopButton_clicked();

private:
    QOpenGLWidget *openGLWidget;
    QVBoxLayout *mainLayout;
    QComboBox *blockTypeComboBox;
    QListWidget *blockListWidget;
    QPushButton *playButton;
    OpenGLScene *m_openglScene;
};

#endif // MAINWINDOW_H
