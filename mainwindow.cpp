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
#include <QResource>
#include <QFileInfo>
#include <QDirIterator>

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
    blockTypeComboBox->addItem("Static");
    blockTypeComboBox->addItem("Interactable");
    blockTypeComboBox->addItem("Background");
    sidebarLayout->addWidget(blockTypeComboBox);

    blockListWidget = new QListWidget(sidebar);
    sidebarLayout->addWidget(blockListWidget);



    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    QSurfaceFormat::setDefaultFormat(format);
    m_openglScene = new OpenGLScene(this);
    this->on_blockTypeComboBox_currentIndexChanged(0);
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


int getFileCountInResource(const QString &prefix) {
    QDirIterator it(":", QDirIterator::Subdirectories);
    int count = 0;
    while (it.hasNext()) {
        it.next();
        QFileInfo fileInfo = it.fileInfo();
        if (fileInfo.isFile() && fileInfo.absoluteFilePath().startsWith(prefix)) {
            count++;
        }
    }
    return count;
}
void loadTileTypeItems(QString prefix, QListWidget *blockListWidget){

    QDir directory(":/"+prefix+"/textures/"+prefix+"/");
    QStringList resourceList = directory.entryList(QStringList("*.png"));
    for (const QString &filePath : resourceList) {
        QPixmap pixmap(":/"+prefix+"/textures/"+prefix+"/"+filePath);
        if (pixmap.isNull()) {
            qDebug() << "Failed to load pixmap from:" << filePath;
        }
        QIcon icon(pixmap);
        QString blockName = QFileInfo(filePath).baseName();
        QListWidgetItem *item = new QListWidgetItem(icon, blockName, blockListWidget);
    }
}

void MainWindow::on_blockTypeComboBox_currentIndexChanged(int index)
{
    blockListWidget->clear();
    int tileCount = 0;
    switch (index) {
    case 0:
        loadTileTypeItems("static",blockListWidget);
        m_openglScene->setBackgroundTileSelected(false);
        break;
    case 1:
        loadTileTypeItems("interactable",blockListWidget);
        m_openglScene->setBackgroundTileSelected(false);
        break;
    case 2:
        loadTileTypeItems("background", blockListWidget);
        m_openglScene->setBackgroundTileSelected(true);
        break;
    default:

        break;

    }
    //m_openglScene->setSelectedBlockType(index);
}
void MainWindow::updateBackgroundColor(const QColor &color)
{
    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, color);
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->update();
}

void MainWindow::on_blockListWidget_itemClicked(QListWidgetItem *item)
{
    qDebug() << item->text();
    m_openglScene->setSelectedTexture(item->text());
}

void MainWindow::on_playButton_clicked() {
    m_openglScene->setMode(Mode::Play);
    updateBackgroundColor(QColor("gray"));

}

void MainWindow::on_stopButton_clicked() {
    m_openglScene->setMode(Mode::Edit);
    updateBackgroundColor(QColor("white"));
}
