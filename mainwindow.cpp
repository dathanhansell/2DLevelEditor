#include "mainwindow.h"
#include "openglscene.h"
#include <QLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QLabel>
#include <QScrollArea>
#include <QPixmap>
#include <QListWidget>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *layout = new QHBoxLayout(centralWidget);

    QGroupBox *sidebar = new QGroupBox("Block Types", this);
    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);

    blockTypeComboBox = new QComboBox(sidebar);
    // Add block types to the combo box
    blockTypeComboBox->addItem("Block Type 1");
    blockTypeComboBox->addItem("Block Type 2");
    blockTypeComboBox->addItem("Block Type 3");
    sidebarLayout->addWidget(blockTypeComboBox);

    blockListWidget = new QListWidget(sidebar);
    sidebarLayout->addWidget(blockListWidget);

    this->on_blockTypeComboBox_currentIndexChanged(0);

    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);
    m_openglScene = new OpenGLScene(this);

    layout->addWidget(sidebar, 1);
    layout->addWidget(m_openglScene, 4);

    connect(blockTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_blockTypeComboBox_currentIndexChanged);
    connect(blockListWidget, &QListWidget::itemClicked,
            this, &MainWindow::on_blockListWidget_itemClicked);

    // Set a minimum size for the window and resize it to a preferred size
    setMinimumSize(800, 600);
    resize(1024, 768);

    // Create toolbar with play and stop buttons
    QToolBar *toolbar = new QToolBar(this);
    addAction(toolbar->addAction("Play", this, &MainWindow::on_playButton_clicked));
    addAction(toolbar->addAction("Stop", this, &MainWindow::on_stopButton_clicked));
    addToolBar(toolbar);
}

MainWindow::~MainWindow()
{
    delete m_openglScene;
}

void MainWindow::on_blockTypeComboBox_currentIndexChanged(int index)
{
    blockListWidget->clear();

    // Load different blocks based on the selected block type
    switch (index) {
    case 0:
        // Add dummy block images and names for Block Type 1
        for (int i = 0; i < 10; ++i) {
            QPixmap dummyPixmap(32, 32);
            dummyPixmap.fill(Qt::gray);
            QIcon dummyIcon(dummyPixmap);
            QString blockName = QString("Block Type 1 - %1").arg(i + 1);
            QListWidgetItem *item = new QListWidgetItem(dummyIcon, blockName, blockListWidget);
        }
        break;
    case 1:
        // Add dummy block images and names for Block Type 2
        for (int i = 0; i < 10; ++i) {
            QPixmap dummyPixmap(32, 32);
            dummyPixmap.fill(Qt::green);
            QIcon dummyIcon(dummyPixmap);
            QString blockName = QString("Block Type 2 - %1").arg(i + 1);
            QListWidgetItem *item = new QListWidgetItem(dummyIcon, blockName, blockListWidget);
        }
        break;
    case 2:
        // Add dummy block images and names for Block Type 3
        for (int i = 0; i < 10; ++i) {
            QPixmap dummyPixmap(32, 32);
            dummyPixmap.fill(Qt::blue);
            QIcon dummyIcon(dummyPixmap);
            QString blockName = QString("Block Type 3 - %1").arg(i + 1);
            QListWidgetItem *item = new QListWidgetItem(dummyIcon, blockName, blockListWidget);
        }
        break;
    default:
        break;
    }
    //m_openglScene->setSelectedBlockType(index);
}


void MainWindow::on_blockListWidget_itemClicked(QListWidgetItem *item)
{
    qDebug() << item->text();
    // Get the selected block type and set it
}

void MainWindow::on_playButton_clicked()
{
    // Implement play functionality here
}

void MainWindow::on_stopButton_clicked()
{
    // Implement stop functionality here
}
