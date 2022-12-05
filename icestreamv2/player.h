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

#ifndef PLAYER_H
#pragma once // fix for known clang bug: "unterminated conditional directive" notification
#define PLAYER_H


#include <QProcess>
#include <QProgressBar>
#include <math.h>
#include "mainwindow.h"
//#include <QRegExp>
#include <QTimer>


class player : public QWidget
{
    Q_OBJECT

public:
    enum PlayState{playing, paused, stopped};
    QSlider *slider;

private :

    int playaNumber;
    QStringList arguments;
    QProcess *playaProcess;
    QString commande;
    QString playaUrl;
    QString filePath;
    QPushButton *playaButtonPlay;
    QPushButton *playaButtonStop;
    QPushButton *playaButtonPause;
    QPushButton *playaMute;
    QPushButton *solo;
    QLineEdit *ligne;
    QToolButton *open;
    PlayState playState;
    QProgressBar * _trackBar;
    QTimer * poller;
    float maxTime;
    float currentTime;
    bool muted;
    bool soloEd;
    QProcess * vuMeterProcess;
    QPushButton * _vuButton;
    QString meterProgram = "/usr/bin/meterbridge";


public:
    explicit player(int playaNumber, QPushButton *Bplay, QPushButton *BStop, QPushButton *CMute,
                    QPushButton *vuButton, QLineEdit *maLigne,QSlider *leSlider,QToolButton *leopen,QPushButton *lesolo);

    int volAncienneValeur;

    QString getUrl();
    void BtnClicked();
    void playUrl();
    void setLigne(QLineEdit *maLigne);
    void setUrl(QString);
    bool isSelected();
    bool isMuted();
    bool isSolo();
    void setMuted();
    void setUnMuted();
    //void unMuteURL();
    void setSolo();
    void unSolo();
    //void processMessage();
    void startVuMeter();
    ~player();

public slots:
    void stopUrl();
    void muteUrl();
    void openfile();
    void setVolume(int);
    void PlayOrStop();

    //void solosh();
    //void slotProcess_playStateChanged(int i);
    //void slotpollCurrentTime(); // poll track progress time
    //void slotprocessMessage();
    void slotvuMeterHandler(bool);

};

#endif // PLAYER_H
