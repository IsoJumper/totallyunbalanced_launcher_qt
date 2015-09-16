#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDir"
#include "QFile"
#include "QSettings"
#include "QMessageBox"
#include <QScreen>
#include "qt_windows.h"
#include "QProcess"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // File Checks
    QDir TotallyUnballonced("./TotallyUnballonced");
    if (TotallyUnballonced.exists() == false)
    {
        QMessageBox::critical(this,tr("Totally Unbalanced Launcher"),tr("Game Files are missing,please verify cache through Steam."));
    }
    // Building Window
    setWindowFlags(Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);
    BrExe = QDir::currentPath()+"/Bridge.exe";
    ui->DEBUGLABEL->setText(BrExe);
    PathToIni = QDir::currentPath() + "/TotallyUnballonced/Config/DefaultGameUserSettings.ini";
    QSettings settings(PathToIni, QSettings::IniFormat);
    settings.beginGroup("ScalabilityGroup");
    m_settings.ResolutionQuality = settings.value("sg.ResolutionQuality",100).toInt();
    m_settings.ViewDistanceQuality = settings.value("sg.ViewDistanceQuality",3).toInt();
    m_settings.AntiAlisingQuality = settings.value("sg.AntiAlisingQuality",3).toInt();
    m_settings.ShadowQuality = settings.value("sg.ShadowQuality",3).toInt();
    m_settings.PostProcessQuality = settings.value("sg.PostProcessQuality",3).toInt();
    m_settings.TextureQuality = settings.value("sg.TextureQuality",3).toInt();
    m_settings.EffectsQuality = settings.value("sg.EffectsQuality",3).toInt();
    settings.endGroup();
    settings.beginGroup("/Script/Engine.GameUserSettings");
    m_settings.bUseVSync = settings.value("bUseVSync",true).toBool();
    settings.endGroup();
    // Setting Up UI
    ui->horizontalSlider->setValue(m_settings.ResolutionQuality);
    ui->Label_Value->setText(QString::number(m_settings.ResolutionQuality));
    ui->comboBox1->setCurrentIndex(m_settings.ViewDistanceQuality);
    ui->comboBox2->setCurrentIndex(m_settings.AntiAlisingQuality);
    ui->comboBox3->setCurrentIndex(m_settings.ShadowQuality);
    ui->comboBox4->setCurrentIndex(m_settings.PostProcessQuality);
    ui->comboBox5->setCurrentIndex(m_settings.TextureQuality);
    ui->comboBox6->setCurrentIndex(m_settings.EffectsQuality);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    QString vlstr = QString::number(value);
    MainWindow::ui->Label_Value->setText(vlstr);
}

void MainWindow::saveSettings()
{
    QSettings settings(PathToIni, QSettings::IniFormat);
    settings.beginGroup("ScalabilityGroup");
    settings.setValue("sg.ResolutionQuality", m_settings.ResolutionQuality);
    settings.setValue("sg.ViewDistanceQuality", m_settings.ViewDistanceQuality);
    settings.setValue("sg.AntiAlisingQuality", m_settings.AntiAlisingQuality);
    settings.setValue("sg.ShadowQuality", m_settings.ShadowQuality);
    settings.setValue("sg.PostProcessQuality", m_settings.PostProcessQuality);
    settings.setValue("sg.TextureQuality", m_settings.TextureQuality);
    settings.setValue("sg.EffectsQuality", m_settings.EffectsQuality);
    settings.endGroup();
    settings.beginGroup("/Script/Engine.GameUserSettings");
    settings.setValue("bUseVSync", m_settings.bUseVSync);
    settings.endGroup();
}


void MainWindow::on_pushButton_clicked()
{
    saveSettings();
    this->close();
}

void MainWindow::RunGame()
{
    int width = qApp->primaryScreen()->size().width();
    int height = qApp->primaryScreen()->size().height();
    QString GameWidth;
    QString GameHeight;
    QString ScreenMode;
    // width
    switch(width)
    {
                    case 1280:
                            GameWidth = "1280";
                            break;
                    case 1366:
                            GameWidth = "1366";
                            break;
                    case 1440:
                            GameWidth = "1440";
                            break;
                    case 1600:
                            GameWidth = "1600";
                            break;
                    case 1920:
                            GameWidth = "1920";
                            break;
    }
    // height
    switch(height)
    {
                    case 600:
                        GameHeight = "600";
                        break;
                    case 720:
                        GameHeight = "720";
                        break;
                    case 768:
                        GameHeight = "720";
                        break;
                    case 960:
                        GameHeight = "720";
                        break;
                    case 1024:
                        GameHeight = "720";
                        break;
                    case 1080:
                        GameHeight = "1080";
                        break;
                    case 1200:
                        GameHeight = "1200";
                        break;
                    case 1440:
                        GameHeight = "1440";
                        break;
                    break;

    }
    // Screen Mode, not working probl
    //if (height == 720 && width == 1280){ScreenMode = fullscreen;}
    //if (height == 1080 && width == 1920){ScreenMode = fullscreen;}
    //if (height == 1200 && width == 1920){ScreenMode = fullscreen;}
    QString Exe = QDir::currentPath()+"/Bridge.exe";
    QStringList Args;
    Args << "-windowed " << "-height=" << GameHeight << " -width=" << GameWidth;
    QProcess process;
    process.start(BrExe, Args);
}
