/*
 * This program was made as part of an internship at APO33 (2011 & 2020).
 * http://www.apo33.org/
 *
 * Conception : Julien Ottavi
 * Development : Romain Papion
 * Improvements (v2) : Antoine Le Bras
 *
 * This program is distributed under the terms of the GNU General
 * Public License 2. You may use, modify, and redistribute
 * it under the terms of this license. A copy should be included
 * with this source.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qprocess.h"
class player;


#include <QTextCodec>
#include <QRegExp>
#include <QIcon>
#include <QMainWindow>
#include <QDebug>
#include <QAction>
#include <QMainWindow>
#include <QWidget>
#include <QToolButton>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTabWidget>
#include <QCheckBox>
#include <QKeyEvent>
#include <QFile>
#include <QSlider>
#include <QRadioButton>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QFileDialog>
#include <QTreeView>
#include <QTreeWidget>
#include <QHBoxLayout>
#include <QTextStream>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QUrl>
#include <QSettings>
#include <iostream>
//#include <lo/lo.h>
//#include <lo/lo_cpp.h>
#include <QOsc>
#include "player.h"

#ifdef  _WIN32
#include <filesystem>
#endif

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void playasPlaying();
        void CreateLiquidConfig();
        void connections();
        //void launchLiquidsoap();
        void launchIceCast();
        void serverFileCreation();
        void closeEvent(QCloseEvent *event) ;

        QString getLogs(QString path);

        void makeConfig(QString);
        void getConfig(QString);
        QMap<QString, QString> settingsMap;
        void setVolumePlayer(player* playerIn,int volumeIn);
        void routageOsc(int playerId, int dataIn);
        void processOSC(const QByteArray& data);
        //int getVol();
        //void setVol(int volIn);

private:
    Ui::MainWindow *ui;
    QProcess *streamProcess;
        QProcess *serverProcess;
        QKeyEvent *levent;
        player *player_1;
        player *player_2;
        player *player_3;
        player *player_4;
        player *player_5;
        player *player_6;
        player *player_7;
        player *player_8;
        player *player_9;
        player *player_10;
        player *player_11;
        player *player_12;
        QMessageBox *maFenetreAbout;
        int val;
        int port;
        bool oscStopped;
        QList<player*> playaTab;
        QFileSystemWatcher * icecast_watcher;
        QString lastIcelog = "";
        QFileSystemWatcher * liquidsoap_watcher;
        QString lastLiquidlog = "";

        QIcon ledgreen;
        QIcon ledred;
        int vol;
        QString default_configpath = (QDir::homePath() + "/.icestreamrc");

        QUrl iceAdminUrl = QUrl::fromEncoded("http://localhost:8000/admin/");

        #ifdef __linux__
        QString localPaths[3] = {"/tmp/", "liquidsoap.log", "icecast.log"}; // 0:path - 1:liquidsoap - 2:icecast
        #elif _WIN32
        QString localPaths[3] = {"fs::temp_directory_path().generic_string()", "liquidsoap.log", "icecast.log"};
        #endif

        QString serverFileName = localPaths[0] + "ConfigIcecast.xml";
        QString clientFileName = localPaths[0] + "ConfigLiquidSoap.liq";

        QString audioFormatSettingsPicker(int, bool); // (format, LQ)
           //lo_server_thread* st;
        //lo::ServerThread* st;
        QOscInterface iface;
        QString pattern;


public slots:
        void launchLiquidsoap();
        void streamConnectionStop();
        void serverConnectionStart();
        void serverConnectionStop();
        void startOscServer();
        void stopOscServer();
        void ouvrirAbout();

        void save_config();
        void load_config();
        void export_config();
        void import_config();

        void slotIcelog(QString filepath);
        void slotLiquidlog(QString filepath);
        void slotstreamProcess_StateChanged(QProcess::ProcessState i);
        void slotserverProcess_StateChanged(QProcess::ProcessState i);

        //void audioFormatFrameHandler(int);

        //void slotOpenAdminView();

        void solo_1();
        void solo_2();
        void solo_3();
        void solo_4();
        void solo_5();
        void solo_6();
        void solo_7();
        void solo_8();
        void solo_9();
        void solo_10();
        void solo_11();
        void solo_12();
        //void solosh();
    };


#endif // MAINWINDOW_H
