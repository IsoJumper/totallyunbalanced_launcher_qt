#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDir"
#include "QFile"
#include "QSettings"
#include "QMessageBox"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    // File Checks
    QDir TotallyUnballonced("/TotallyUnballonced/");
    if (TotallyUnballonced.exists() == false)
    {
        QMessageBox::critical(this,tr("Totally Unbalanced Launcher"),tr("Game Files are missing,please verify cache through Steam."));
    }
    // Building Window
    ui->setupUi(this);
    PathToIni = QDir::currentPath() + "TotallyUnballonced/Config/DefaultGameUserSettings.ini";
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
