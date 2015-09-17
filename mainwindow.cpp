#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ssettings.h"
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QProcess>
#include <QScreen>
#include <QSettings>

#include <sstream>
#include <limits>

static QString const g_processName = QStringLiteral("Bridge.exe");

static QStringList const g_supportedResolutions = {
	QStringLiteral("640x360"),
	QStringLiteral("720x405"),
	QStringLiteral("854x480"),
	QStringLiteral("960x540"),
	QStringLiteral("1024x576"),
	QStringLiteral("1280x720"),
	QStringLiteral("1366x768"),
	QStringLiteral("1600x900"),
	QStringLiteral("1920x1080"),
	QStringLiteral("2048x1152"),
	QStringLiteral("2560x1440"),
	QStringLiteral("2880x1620"),
	QStringLiteral("3200x1800"),
	QStringLiteral("3840x2160"),
	QStringLiteral("4096x2304"),
	QStringLiteral("5120x2880"),
	QStringLiteral("7680x4320"),
	QStringLiteral("15360x8640")
};

enum { g_maxArea = 132710400 };

// Parse a QString (using the STL) to a QSize
static QSize SizeFromString(QString const& string) {
	std::istringstream iss(string.toStdString());
	
	std::string::value_type ch = 0;
	int w = 0;
	int h = 0;
	
	if( (iss >> w >> ch >> h) && (ch == 'x') )
	{
		return QSize(w,h);
	}
	return QSize(-1,-1);
}

struct BestMatching {
	inline BestMatching()
		: index(0), match_distance(std::numeric_limits<std::uint64_t>::max()) {}
	std::uint32_t index;
	std::uint64_t match_distance;
};

// Also select the best matching resolution in the UI
void MainWindow::ReloadScreenSizes(QSize const& userSize)
{
	ui->cbResolution->clear();
	
	QSize const optimalSize = m_screenSize;
	
	std::int64_t const optimalArea = (optimalSize.width() * optimalSize.height()) < g_maxArea ? (optimalSize.width() * optimalSize.height()) : g_maxArea;
	std::int64_t const minimalArea = optimalArea * 2 / 5;
	std::int64_t const maximalArea = optimalArea * 6 / 5;
	
	BestMatching match;
	
	std::uint32_t i = 0;
	for(auto it = g_supportedResolutions.begin(); it != g_supportedResolutions.end(); ++it)
	{
		QSize const resSize = SizeFromString(*it);
		std::int64_t const area = resSize.height() * resSize.width();
		
		// Do not erase. Will be useful once we get to support more resolutions.
		if(resSize == userSize)
		{
			match.match_distance = 0;
			match.index = i;
		}
		
		if( ((area >= minimalArea) && (area <= maximalArea) ) || (resSize == userSize) )
		{
			ui->cbResolution->addItem(*it);
			if(static_cast<std::uint64_t>(std::abs(optimalArea-area)) < match.match_distance)
				match.index = i;
			++i;
		}
	}
	ui->cbResolution->setCurrentIndex(match.index);
}

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	m_executable(QDir::currentPath()+"/"+g_processName),
	m_iniPath(QDir::currentPath()+"/TotallyUnballonced/Config/DefaultGameUserSettings.ini"),
	m_screenSize(qApp->primaryScreen() ? qApp->primaryScreen()->size() : QSize(-1,-1))
{
	ui->setupUi(this);
	
	SSettingsIni settings(m_iniPath, QSettings::IniFormat);
	
	settings.beginGroup("ScalabilityGroup");
	ui->slResolutionScale->setValue(
		qBound(40,settings.value("sg.ResolutionQuality",100).toInt(),100)
	);
	ui->cbViewDistance->setCurrentIndex(
		qBound(0,settings.value("sg.ViewDistanceQuality",3).toInt()-1,ui->cbViewDistance->count()-1)
	);
	ui->cbAntiAliasing->setCurrentIndex(
		qBound(0,settings.value("sg.AntiAlisingQuality",3).toInt()-1,ui->cbAntiAliasing->count()-1)
	);
	ui->cbShadowQuality->setCurrentIndex(
		qBound(0,settings.value("sg.ShadowQuality",3).toInt()-1,ui->cbShadowQuality->count()-1)
	);
	ui->cbPostProcessQuality->setCurrentIndex(
		qBound(0,settings.value("sg.PostProcessQuality",3).toInt()-1,ui->cbPostProcessQuality->count()-1)
	);
	ui->cbTextureQuality->setCurrentIndex(
		qBound(0,settings.value("sg.TextureQuality",3).toInt()-1,ui->cbTextureQuality->count()-1)
	);
	ui->cbEffectsQuality->setCurrentIndex(
		qBound(0,settings.value("sg.EffectsQuality",3).toInt()-1,ui->cbEffectsQuality->count()-1)
	);
	settings.endGroup();
	
	settings.beginGroup("/Script/Engine.GameUserSettings");
	ui->bVsync->setChecked(settings.value("bUseVSync",true).toBool());
	settings.endGroup();
	
	settings.beginGroup("SGH.LauncherInfo");
	QSize optimalSize = settings.value("Resolution",m_screenSize).toSize();
	settings.endGroup();
	
	ReloadScreenSizes(optimalSize);
}

MainWindow::~MainWindow()
{
	SaveSettings();
	delete ui;
}

void MainWindow::SaveSettings()
{
	SSettingsIni settings(m_iniPath, QSettings::IniFormat);
	settings.beginGroup("ScalabilityGroup");
	settings.setValue("sg.ResolutionQuality", ui->slResolutionScale->value());
	settings.setValue("sg.ViewDistanceQuality", ui->cbViewDistance->currentIndex()+1);
	settings.setValue("sg.AntiAlisingQuality", ui->cbAntiAliasing->currentIndex()+1);
	settings.setValue("sg.ShadowQuality", ui->cbShadowQuality->currentIndex()+1);
	settings.setValue("sg.PostProcessQuality", ui->cbPostProcessQuality->currentIndex()+1);
	settings.setValue("sg.TextureQuality", ui->cbTextureQuality->currentIndex()+1);
	settings.setValue("sg.EffectsQuality", ui->cbEffectsQuality->currentIndex()+1);
	settings.endGroup();
	
	settings.beginGroup("/Script/Engine.GameUserSettings");
	settings.setValue("bUseVSync", ui->bVsync->isChecked() ? QStringLiteral("true") : QStringLiteral("false"));
	settings.endGroup();
	
	settings.beginGroup("SGH.LauncherInfo");
	QSize selectedResolution = SizeFromString(ui->cbResolution->currentText());
	if(selectedResolution == QSize(-1,-1))
		selectedResolution = m_screenSize;
	settings.setValue("Resolution", selectedResolution);
	settings.endGroup();
}

bool MainWindow::RunGame()
{
	QSize selectedResolution = SizeFromString(ui->cbResolution->currentText());
	if(selectedResolution == QSize(-1,-1))
		selectedResolution = m_screenSize;
	bool bFullscreen = (m_screenSize.width()*m_screenSize.height()) <= (selectedResolution.width()*selectedResolution.height());
	
	QStringList Args;
	Args << (bFullscreen ? QStringLiteral("-fullscreen") : QStringLiteral("-windowed"))
		 << (QString("-height=")+QString::number(selectedResolution.height()))
		 << (QString("-width=")+QString::number(selectedResolution.width()));
	
	QProcess process;
	process.start(m_executable, Args);
	process.waitForStarted(3000);
	if((process.state() == QProcess::NotRunning) && (process.error() != QProcess::UnknownError) && (process.error() != QProcess::Timedout))
	{
		QMessageBox::critical(this,QStringLiteral("Totally Unbalanced Launcher"),QStringLiteral("Fatal Error:\nCould not start process: ")+g_processName+QStringLiteral(" ."));
		return false;
	}
	return true;
}

void MainWindow::on_buttonPlay_clicked()
{
	if(RunGame())
		close();
}
