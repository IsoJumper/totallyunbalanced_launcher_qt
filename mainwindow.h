#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

struct RawSettings {
    int ResolutionQuality;
    int ViewDistanceQuality;
    int AntiAlisingQuality;
    int ShadowQuality;
    int PostProcessQuality;
    int TextureQuality;
    int EffectsQuality;
    bool bUseVSync;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void saveSettings();
    ~MainWindow();

private:
    QString PathToIni;

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    RawSettings m_settings;
};

#endif // MAINWINDOW_H
