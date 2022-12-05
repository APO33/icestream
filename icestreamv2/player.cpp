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

#include "mainwindow.h"
#include "player.h"
#include <QApplication>
#include <stdio.h>
#include <QTextStream>
#include <Qt>
#include <QDebug>


player::player(int myNumber,QPushButton *Bplay, QPushButton *BStop,QPushButton *CMute,
               QPushButton *vuButton, QLineEdit *maLigne,QSlider *leSlider,QToolButton *leopen,QPushButton *lesolo) :
        QWidget ()
{
    playaNumber = myNumber;
    playaProcess = new QProcess();
    playaButtonPlay = Bplay;
    playaButtonStop = BStop;
    playaMute = CMute;
    ligne = maLigne;
    slider = leSlider;
    open = leopen;
    solo = lesolo;
    volAncienneValeur = 50;     // Valeur initial du volume
    commande="/usr/bin/mplayer";

    playState = stopped;
    soloEd = false;
    muted = false;
    poller = new QTimer(this);  // Timer pour la trackbar
    //_trackBar = trackBar;

    vuMeterProcess = new QProcess();
    _vuButton = vuButton;
}


player::~player()   // Never called, but really should
{
    stopUrl();
    delete this;
}

void player::BtnClicked()   // Connections for the player
{
      connect(playaButtonPlay,SIGNAL(clicked()),SLOT(PlayOrStop()));
      connect(playaButtonStop,SIGNAL(clicked()),SLOT(stopUrl()));
      connect(playaMute,SIGNAL(clicked()),SLOT(muteUrl()));
      //connect(solo,SIGNAL(clicked()),SLOT(solosh()));
      connect(open,SIGNAL(clicked()),SLOT(openfile()));
      connect(slider,SIGNAL(valueChanged(int)),this,SLOT(setVolume(int)));
      connect(_vuButton,SIGNAL(toggled(bool)),SLOT(slotvuMeterHandler(bool)));
      //connect(solo,SIGNAL(tooggled(bool)),SLOT(isSolo()));
      // Process state handling
      //connect(playaProcess,SIGNAL(QProcess::ProcessState),SLOT(slotProcess_playStateChanged(int)));

      // Trackbar
      //connect(poller, SIGNAL(timeout()), this, SLOT(slotpollCurrentTime()));
      //connect(playaProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(slotprocessMessage()));

}




// Called when the Play/Pause button is pressed.
void player::PlayOrStop(){
    if(playState == playing) {
        playState = paused;
        this->playaButtonPlay->setIcon(QIcon(":/Icones_logiciel/Play.ico")); // if player is playing, pause
        //if (poller->isActive()){
         //   poller->stop();
        //}
    }
    else if(playState == paused) {
        playState = playing;
        this->playaButtonPlay->setIcon(QIcon(":/Icones_logiciel/Pause.ico")); // if player is paused, play
        //if (!poller->isActive()){
         //   poller->start();
        //}
    }
    else if (playState == stopped)
    {
        playState = playing;
        playUrl();
        return ;
    }
    playaProcess->write("pause\n"); // Strangely, MPlayer handles this as "play" if the player is paused. Handy
}





void player::playUrl()
{
    //if(!filePath.isEmpty()){ playaUrl = filePath; }
    //else if (playaUrl.isEmpty() && !playaProcess->isOpen() && ligne->text().contains("/")) { playaUrl = ligne->text(); QString tmp = playaUrl; ligne->setText(tmp.remove(0, tmp.lastIndexOf("/")+1)); }
    playaUrl = ligne->text();
    if (!playaUrl.isEmpty() && !playaProcess->isOpen()) // Old verification, prior to PlayOrStop's implementation.
    {
        playState = playing;
        arguments << "-slave"
                  << "-softvol" << "-softvol-max" << "100" << "-volstep" << "1"
                  << "-volume" << QString::number(this->slider->value())
                  << "-ao" << "jack:name=player_" + QString::number(this->playaNumber)
                  << "-cache" << "550"
                  << playaUrl;
        playaProcess->start(commande,arguments);
        arguments.removeLast();

        //poller->start(1000);                        // Timer starts, and triggers the track time check every second
        //playaProcess->write("get_time_length\n");   // Have mplayer output track lenght
        this->playaButtonPlay->setIcon(QIcon(":/Icones_logiciel/Pause.ico"));
        filePath.clear();

        //volumeTimer->start(1000);
    }
}

// Stops everything player-related
void player::stopUrl()
{
   // _trackBar->setValue(0);

    if(_vuButton->isChecked()){ _vuButton->toggle(); vuMeterProcess->close();}
    playState = stopped;
    playaButtonPlay->setIcon(QIcon(":/Icones_logiciel/Play.ico"));
    playaProcess->close();
    vuMeterProcess->close();
}

void player::muteUrl()
{
    if (this->isMuted() == false)
    {
        playaProcess->write("mute\n");
        this->setMuted();

    }
    else if (this->isMuted() == true)
    {
        playaProcess->write("mute\n");
        this->setUnMuted();

    }

}



// Window dialog for selecting a file
void player::openfile()
{
    filePath = QFileDialog::getOpenFileName(this,tr("Open Music"), "/home", tr("Music Files (*.wav *.mp3 *.riff *.bwf *.ogg *.aiff *.aac *.wma, *.m4a);;Any Files (*.*)"));
    ligne->setText(filePath);
}

// Changes MPlayer's volume when the slider moves
void player::setVolume(int i)
{
    // [ Counted step-by-step method ]

    int val = (i-volAncienneValeur);
    //i = slider->sliderPosition();
    QString playerVolume;
    if((i-volAncienneValeur) >= 0){
        for (int n=0;n<=val;n++) {
            playerVolume = "volume +1\n";   // one step
            char *volumeProc=playerVolume.toLatin1().data();
            playaProcess->write(volumeProc);
        }
    }
    else {
        for (int n=0;n>=val;val++) {
            playerVolume = "volume -1\n";
            char *volumeProc=playerVolume.toLatin1().data();
            playaProcess->write(volumeProc);
        }
    }
    volAncienneValeur=slider->sliderPosition();


/* [ OLD 2016 ] /// If slider ticks too fast, mplayer cannot keep up and misses volume commands.
    QString val = QString::number(i-volAncienneValeur);
    QString volumePlayer;
    if((i-volAncienneValeur) >= 0){
        volumePlayer = "volume +\n";
        std::cout << "Added " << val.toStdString() << std::endl;
    }
    else {
        volumePlayer = "volume -\n";
        std::cout << "Substracted " << val.toStdString() << std::endl;
    }
    char *volumeProc=volumePlayer.toAscii().data();
    playaProcess->write(volumeProc);
    volAncienneValeur=slider->sliderPosition();
*/
}



bool player::isMuted(){

    bool muted=this->muted;
    if (this->muted == true)
       {
            muted = true;
       }
    else if (this->muted == false)
       {
         muted = false;
       }

    return muted;
}

void player:: setMuted()
{
    this->muted = true;
    //this->ligne->setText("mute actif");
    this->playaMute->setIcon(QIcon(":/Icones_logiciel/MuteOn.ico"));
}

void player:: setUnMuted()
{
    this->muted = false;
    //this->ligne->setText("mute inactif");
    this->playaMute->setIcon(QIcon(":/Icones_logiciel/MuteOffChange.ico"));
}

void player:: setSolo()
{
    this->solo->setIcon(QIcon(":/Icones_logiciel/SoloActiv.ico"));
    //this->ligne->setText("solo actif");
    this->soloEd = true;
}

void player:: unSolo()
{
    this->solo->setIcon(QIcon(":/Icones_logiciel/Solo.ico"));
    //this->ligne->setText("solo inactif");
    this->soloEd = false;
}
bool player::isSolo()
{
    bool solod=this->soloEd;
    if (this->soloEd == true)
       {
            solod = true;
       }
    else if (this->soloEd == false)
       {
         solod = false;
       }

    return solod;
}



// Starts a vuMeter using meterbridge
void player::startVuMeter(){
    QStringList meterArguments;
    QString output_one = "player_" + QString::number(this->playaNumber) + ":out_0";
    QString output_two = "player_" + QString::number(this->playaNumber) + ":out_1";
    meterArguments << "-t" << "dpm" << output_one.arg(playaProcess->processId()) << output_two.arg(playaProcess->processId());
    vuMeterProcess->start(meterProgram, meterArguments);
}


// Handles the state of the vuMeter
void player::slotvuMeterHandler(bool c){
    if(!(vuMeterProcess->isOpen()) && playaProcess->isOpen()){
        startVuMeter();
    } else if (vuMeterProcess->isOpen()) {
        vuMeterProcess->close();
    } else if (c) {
        _vuButton->toggle();
    }
    /*
  [ OLD 2016 ]
    if(c && playaProcess->isOpen()){
        startVuMeter();
    } else {
        vuMeterProcess->close();
    }
    */

}

