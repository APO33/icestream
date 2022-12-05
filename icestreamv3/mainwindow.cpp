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
//#include "ui_mainwindow.h"
#include "player.h"
#include "ui_mainwindow.h"
#include <QWidget>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


   streamProcess=new QProcess();
   serverProcess=new QProcess();
   oscStopped = false;
   port = 7770;
   pattern = "/icestream/volume/";
   vol = 50;
   //st = lo_server_thread();
   //this->startOscServer();
   //myServer = new serverosc();
   // There's got to be a better way

   player_1=new player(1,ui->PLay_1,ui->Stop_1,ui->Mute_1,ui->vuButton_1,ui->URl_1,ui->volumeSlider_1,ui->openFile_1,ui->Solo_1);
   player_2=new player(2,ui->PLay_2,ui->Stop_2,ui->Mute_2,ui->vuButton_2,ui->URl_2,ui->volumeSlider_2,ui->openFile_2,ui->Solo_2);
   player_3=new player(3,ui->PLay_3,ui->Stop_3,ui->Mute_3,ui->vuButton_3,ui->URl_3,ui->volumeSlider_3,ui->openFile_3,ui->Solo_3);
   player_4=new player(4,ui->PLay_4,ui->Stop_4,ui->Mute_4,ui->vuButton_4,ui->URl_4,ui->volumeSlider_4,ui->openFile_4,ui->Solo_4);
   player_5=new player(5,ui->PLay_5,ui->Stop_5,ui->Mute_5,ui->vuButton_5,ui->URl_5,ui->volumeSlider_5,ui->openFile_5,ui->Solo_5);
   player_6=new player(6,ui->PLay_6,ui->Stop_6,ui->Mute_6,ui->vuButton_6,ui->URl_6,ui->volumeSlider_6,ui->openFile_6,ui->Solo_6);
   player_7=new player(7,ui->PLay_7,ui->Stop_7,ui->Mute_7,ui->vuButton_7,ui->URl_7,ui->volumeSlider_7,ui->openFile_7,ui->Solo_7);
   player_8=new player(8,ui->PLay_8,ui->Stop_8,ui->Mute_8,ui->vuButton_8,ui->URl_8,ui->volumeSlider_8,ui->openFile_8,ui->Solo_8);
   player_9=new player(9,ui->PLay_9,ui->Stop_9,ui->Mute_9,ui->vuButton_9,ui->URl_9,ui->volumeSlider_9,ui->openFile_9,ui->Solo_9);
   player_10=new player(10,ui->PLay_10,ui->Stop_10,ui->Mute_10,ui->vuButton_10,ui->URl_10,ui->volumeSlider_10,ui->openFile_10,ui->Solo_10);
   player_11=new player(11,ui->PLay_11,ui->Stop_11,ui->Mute_11,ui->vuButton_11,ui->URl_11,ui->volumeSlider_11,ui->openFile_11,ui->Solo_11);
   player_12=new player(12,ui->PLay_12,ui->Stop_12,ui->Mute_12,ui->vuButton_12,ui->URl_12,ui->volumeSlider_12,ui->openFile_12,ui->Solo_12);

   playaTab.append(player_1);
   playaTab.append(player_2);
   playaTab.append(player_3);
   playaTab.append(player_4);
   playaTab.append(player_5);
   playaTab.append(player_6);
   playaTab.append(player_7);
   playaTab.append(player_8);
   playaTab.append(player_9);
   playaTab.append(player_10);
   playaTab.append(player_11);
   playaTab.append(player_12);


   ui->liquidsoap_logWindow->setReadOnly(true);
   ui->icecast_logWindow->setReadOnly(true);
   ui->comboBox_Quality_ogg->setCurrentIndex(3);
   // Manually creating the status light icons, to use them more easily
   ledgreen.addPixmap(QPixmap(":/Icones_logiciel/led_green.ico"), QIcon::Disabled);
   ledred.addPixmap(QPixmap(":/Icones_logiciel/led_red.ico"), QIcon::Disabled);

   //ui->iceWebView->hide();             // Hides webview for aesthetic purposes
   iceAdminUrl.setUserName("admin");   // If server admin auth ever changes...
   iceAdminUrl.setPassword("hackme");  // ...modify these


   //audioFormatFrameHandler(0);         // Sets audio settings frame to ogg
}


MainWindow::~MainWindow()
{
    player_1->~player();
    player_2->~player();
    player_3->~player();
    player_4->~player();
    player_5->~player();
    player_6->~player();
    player_7->~player();
    player_8->~player();
    player_9->~player();
    player_10->~player();
    player_11->~player();
    player_12->~player();
    //myServer->~serverosc();
    streamConnectionStop();
    serverConnectionStop();
    delete ui;
}

void MainWindow::CreateLiquidConfig() // Config client (liquidsoap)
{
    QFile file(clientFileName);
    // Ouverture du fichier en écriture et en texte.
    file.open(QFile::WriteOnly | QFile::Text);
    QTextStream stream(&file);
    stream.setEncoding(QStringConverter::Utf8);


    // logging
    stream << "set(\"log.stdout\", false)" << Qt::endl;;
    stream << "set(\"log.file\", true)" << Qt::endl;;
    stream << "set(\"log.file.path\", \"" + localPaths[0] + localPaths[1] + "\")" << Qt::endl;;
    stream << "set(\"log.level\", 3)" << Qt::endl;;

    // frame
    stream << "set(\"frame.audio.size\"," << ui->comboBox_Quality_ogg->currentText() << ")" << Qt::endl;;

    // input
    stream << "full = input.jack(id=\"liquidsoap\")" << Qt::endl;;
    stream << "output.icecast(%vorbis(samplerate=" << ui->comboBox_SampleRate_ogg->currentText() << "," <<
              "channels=" << ui->comboBox_Channels_ogg->currentText() << ")," << Qt::endl;;//"quality="0.5),
    if(ui->optionLQ->checkState()){ stream << "lq = input.jack(id=\"liquidsoap_lq\")" << Qt::endl;; }                // create an input client if LQ stream
    if(ui->optionStreamDump->checkState()){ stream << "dump = input.jack(id=\"liquidsoap_dump\")" << Qt::endl;; }    // create an input client if dump

    // output
    //stream << audioFormatSettingsPicker(ui->comboBox_Format->currentIndex(), 0) << Qt::endl;;
    stream << "host=\"" << ui->Line_Server->text() << "\"," << Qt::endl;;
    stream << "port=" << ui->Line_Port->text() << "," << Qt::endl;;
    stream << "password=\"" << ui->Line_Pass->text() << "\"," << Qt::endl;;
    stream << "mount=\"" << ui->Line_Mount->text() << "\"," << Qt::endl;;

    stream << "name=\"" << ui->Line_Name->text() << "\"," << Qt::endl;;
    stream << "description=\"" << ui->Line_Desc->text() << "\"," << Qt::endl;;
    stream << "genre=\"" << ui->Line_Genre->text() << "\"," << Qt::endl;;

    stream << "full)" << Qt::endl;;

    // Duplicate Low-Quality option
    if(ui->optionLQ->checkState()){
    //stream << audioFormatSettingsPicker(ui->comboBox_Format->currentIndex(), 1) << Qt::endl;;
    stream << "host=\"" << ui->Line_Server->text() << "\"," << Qt::endl;;
    stream << "port=" << ui->Line_Port->text() << "," << Qt::endl;;
    stream << "password=\"" << ui->Line_Pass->text() << "\"," << Qt::endl;;
    stream << "mount=\"" << ui->Line_Mount->text() << "_LQ\"," << Qt::endl;;

    stream << "name=\"" << ui->Line_Name->text() << "_LQ\"," << Qt::endl;;
    stream << "description=\"" << ui->Line_Desc->text() << "\"," << Qt::endl;;
    stream << "genre=\"" << ui->Line_Genre->text() << "\"," << Qt::endl;;

    stream << "lq)" << Qt::endl;;
    }

    // Dump option
    if(ui->optionStreamDump->checkState()){
    stream << "output.file(%wav(stereo=true, channels=2, samplesize=16, header=false)," << Qt::endl;;
    stream << "'dump/%d-%m-%Y-%Hh%M.wav'," << Qt::endl;;
    stream << "fallible=true," << Qt::endl;;
    stream << "reopen_on_metadata=false," << Qt::endl;;
    stream << "on_stop=shutdown," << Qt::endl;;
    stream << "dump)" << Qt::endl;;
    }

    file.close();
}


void MainWindow::connections()
{
    // UI Buttons
    connect(ui->ConnectStream,SIGNAL(clicked()),SLOT(launchLiquidsoap()));
    connect(ui->DisconnectStream,SIGNAL(clicked()),SLOT(streamConnectionStop()));
    connect(ui->Start_Local,SIGNAL(clicked()),SLOT(serverConnectionStart()));
    connect(ui->Stop_Local,SIGNAL(clicked()),SLOT(serverConnectionStop()));
    connect(ui->Solo_1,SIGNAL(clicked()),SLOT(solo_1()));
    connect(ui->Solo_2,SIGNAL(clicked()),SLOT(solo_2()));
    connect(ui->Solo_3,SIGNAL(clicked()),SLOT(solo_3()));
    connect(ui->Solo_4,SIGNAL(clicked()),SLOT(solo_4()));
    connect(ui->Solo_5,SIGNAL(clicked()),SLOT(solo_5()));
    connect(ui->Solo_6,SIGNAL(clicked()),SLOT(solo_6()));
    connect(ui->Solo_7,SIGNAL(clicked()),SLOT(solo_7()));
    connect(ui->Solo_8,SIGNAL(clicked()),SLOT(solo_8()));
    connect(ui->Solo_9,SIGNAL(clicked()),SLOT(solo_9()));
    connect(ui->Solo_10,SIGNAL(clicked()),SLOT(solo_10()));
    connect(ui->Solo_11,SIGNAL(clicked()),SLOT(solo_11()));
    connect(ui->Solo_12,SIGNAL(clicked()),SLOT(solo_12()));

    connect(ui->pushAbout,SIGNAL(clicked()),SLOT(ouvrirAbout()));
    connect(ui->pushStartOsc,SIGNAL(clicked()),SLOT(startOscServer()));
    connect(ui->pushStopOsc_2,SIGNAL(clicked()),SLOT(stopOscServer()));
    // Import/Export Configuration related
    connect(ui->actionSave,SIGNAL(triggered()),SLOT(save_config()));
    connect(ui->actionSave_as,SIGNAL(triggered()),SLOT(export_config()));
    connect(ui->actionLoad,SIGNAL(triggered()),SLOT(load_config()));
    connect(ui->actionLoad_as,SIGNAL(triggered()),SLOT(import_config()));

    // Audio format frame activator
    //connect(ui->comboBox_Format, SIGNAL(currentIndexChanged(int)),SLOT(audioFormatFrameHandler(int)));

    // Status lights for server & client
    connect(streamProcess,SIGNAL(stateChanged(QProcess::ProcessState)),SLOT(slotstreamProcess_StateChanged(QProcess::ProcessState)));
    connect(serverProcess,SIGNAL(stateChanged(QProcess::ProcessState)),SLOT(slotserverProcess_StateChanged(QProcess::ProcessState)));
    for (int ind=0;ind<=11;ind++)
    {
        playaTab.at(ind)->BtnClicked();
    }
}


/*void MainWindow::slotOpenAdminView(){   // Called when starting the Icecast server. Shows a webview pointed at Icecast's admin page
    ui->iceWebView->load(iceAdminUrl);
    ui->iceWebView->show();
}*/

// Called whenever the audio format is changed in the client settings. Hides and shows the proper frame
// It's quite bad, and should be changed in the future.
/*void MainWindow::audioFormatFrameHandler(int i){
    switch (i) {
    case 0:
        ui->audioFrame_mp3->hide();
        ui->audioFrame_mp3->setEnabled(0);
        ui->audioFrame_wav->hide();
        ui->audioFrame_wav->setEnabled(0);
        ui->audioFrame_ogg->setEnabled(1);
        ui->audioFrame_ogg->show();
        break;
    case 1:
        ui->audioFrame_ogg->hide();
        ui->audioFrame_ogg->setEnabled(0);
        ui->audioFrame_wav->hide();
        ui->audioFrame_wav->setEnabled(0);
        ui->audioFrame_mp3->setEnabled(1);
        ui->audioFrame_mp3->show();
        break;
    case 2:     // Disabled
        ui->audioFrame_mp3->hide();
        ui->audioFrame_mp3->setEnabled(0);
        ui->audioFrame_ogg->hide();
        ui->audioFrame_ogg->setEnabled(0);
        ui->audioFrame_wav->setEnabled(1);
        ui->audioFrame_wav->show();
        break;
    default: break;} // Index out of bounds
}*/

void MainWindow::launchLiquidsoap()
{
    QString streamCommande;
    QStringList streamArguments;
    this->CreateLiquidConfig();
    streamCommande = "liquidsoap";
    streamArguments << clientFileName;
    streamProcess->start(streamCommande,streamArguments);

    // Check if the log file for liquidsoap exists, and if not, make it. Needed every initial boot, as the temporary folder is cleared
    if(!QFileInfo(localPaths[0] + localPaths[1]).exists()){  // The log file doesn't exists.
        QFile file(localPaths[0] + localPaths[1]);
        file.open(QIODevice::WriteOnly);
        file.close();
    }

    liquidsoap_watcher = new QFileSystemWatcher(this);                                      // Watch
    liquidsoap_watcher->addPath(localPaths[0] + localPaths[1]);                             // said
    connect(liquidsoap_watcher,SIGNAL(fileChanged(QString)),SLOT(slotLiquidlog(QString)));  // file
}

void MainWindow::streamConnectionStop()
{
    if(streamProcess->isOpen()){
        streamProcess->close();
        ui->liquidsoap_logWindow->setText("Stream disconnected");
        liquidsoap_watcher->disconnect();
        delete liquidsoap_watcher;
    }
}

// Opens a given file and parses it
QString MainWindow::getLogs(QString path){
    QFile file(path);
    QString log;
    QString lastlog[3];
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        file.seek(file.size()-1);
        for (int i=0;(i<=1) && (file.pos() > 0);) {
            QString token = file.read(1);
            file.seek(file.pos()-2);
            if (token == "\n"){
                i++;
            }
        }
        log = file.readAll();
        file.close();
        log.remove(0, 2); log.remove(log.size()-1, 1);  // Fix pour une erreur dans la lecture
    }
    return log;
}

void MainWindow::slotLiquidlog(QString filepath)
{
    QString log = getLogs(filepath);
    if((log != lastLiquidlog) && !(log.contains(lastLiquidlog))){
        ui->liquidsoap_logWindow->append(log);
        lastLiquidlog = log ;
    }
    else if(lastLiquidlog.isEmpty()){           // Trashy solution for an empty check buffer
        ui->liquidsoap_logWindow->append(log);  // Does not affect performance
        lastLiquidlog = log ;
    }
}

void MainWindow::slotIcelog(QString filepath)
{
    QString log = getLogs(filepath);
    if((log != lastIcelog) && !(log.contains(lastIcelog))){
        ui->icecast_logWindow->append(log);
        lastIcelog = log;
    }
    else if(lastIcelog.isEmpty()){
        ui->icecast_logWindow->append(log);
        lastIcelog = log ;
    }
}

// Creates the server config file
void MainWindow::serverFileCreation()
{
    QFile file(serverFileName);
    // Ouverture du fichier en écriture et en texte.
    file.open(QFile::WriteOnly | QFile::Text);
    QXmlStreamWriter writer(&file);
    //writer.writeCDATA("ISO 8859-1");

    writer.setAutoFormatting(true);
    writer.writeStartDocument();

        writer.writeStartElement("icecast");
            writer.writeTextElement("location", "FR");
            writer.writeTextElement("admin", "webmaster@localhost");

            writer.writeStartElement("limits");
                writer.writeTextElement("clients", ui->Line_Local_Listen->text());
                writer.writeTextElement("sources", ui->Line_Local_Send->text());
                writer.writeTextElement("queue-size", "524288");
                writer.writeTextElement("client-timeout", "30");
                writer.writeTextElement("header-timeout", "15");
                writer.writeTextElement("source-timeout", "10");
                writer.writeTextElement("burst-size", "65535");
            writer.writeEndElement();

            writer.writeStartElement("authentication");
                writer.writeTextElement("source-password", ui->Line_Local_Pass->text());
                writer.writeTextElement("relay-password", "hackme");
                writer.writeTextElement("admin-user", "admin");
                writer.writeTextElement("admin-password","hackme"); // Really should be changed!
            writer.writeEndElement();

                writer.writeTextElement("hostname", "localhost");

            writer.writeStartElement("listen-socket");
                writer.writeTextElement("port", ui->Line_Local_Port->text());
            writer.writeEndElement();

            writer.writeTextElement("fileserve", "1");

            writer.writeStartElement("paths");
                writer.writeTextElement("basedir", "/usr/share/icecast2");
                writer.writeTextElement("logdir", localPaths[0]);
                writer.writeTextElement("webroot", "/usr/share/icecast2/web");
                writer.writeTextElement("adminroot", "/usr/share/icecast2/admin");

                    writer.writeStartElement("alias");
                    writer.writeAttribute("source", "/");
                    writer.writeAttribute("dest", "/status.xsl");
                    writer.writeEndElement();

            writer.writeEndElement();

            writer.writeStartElement("logging");
                writer.writeTextElement("accesslog", "access.log");
                writer.writeTextElement("errorlog", localPaths[2]);
                writer.writeTextElement("loglevel", "3");
                writer.writeTextElement("logsize", "10000");
            writer.writeEndElement();

            writer.writeStartElement("security");
                writer.writeTextElement("chroot", "0");
            writer.writeEndElement();

        writer.writeEndElement();
    file.close();

}

void MainWindow::serverConnectionStart()
{
    this->serverFileCreation();
    this->launchIceCast();
    if (serverProcess->isOpen())
    {
        if(!QFileInfo(localPaths[0] + localPaths[2]).exists()){     // The log file doesn't exists.
            QFile file(localPaths[0] + localPaths[2]);
            file.open(QIODevice::WriteOnly);
            file.close();
        }
        ui->icecast_logWindow->setText("Server started");

        // Start watching log file
        icecast_watcher = new QFileSystemWatcher(this);
        icecast_watcher->addPath(localPaths[0] + localPaths[2]);
        connect(icecast_watcher,SIGNAL(fileChanged(QString)),SLOT(slotIcelog(QString)));

        //QTimer::singleShot(1000, this, SLOT(slotOpenAdminView()));   // Starts the webview with some delay, giving time for Icecast to come alive.
    }
}


void MainWindow::serverConnectionStop()
{
    if(serverProcess->isOpen()){
        serverProcess->close();
        icecast_watcher->disconnect();
        delete icecast_watcher;
        //ui->iceWebView->hide();

        ui->icecast_logWindow->setText("Server stopped");
    }
}

void MainWindow::launchIceCast()
{
    QString serverStreamCommande;
    QStringList serverStreamArguments;
    serverStreamCommande = "icecast2";
    serverStreamArguments << "-c" << serverFileName;
    if (!serverProcess->isOpen())
    {
        serverProcess->start(serverStreamCommande,serverStreamArguments);
    }
}

// Handles the status "light", watching the Process' state
void MainWindow::slotstreamProcess_StateChanged(QProcess::ProcessState i)
{
    if(i == 0){ ui->streamStatus->setIcon(QIcon(ledred)); }
    else { ui->streamStatus->setIcon(QIcon(ledgreen)); }
}

void MainWindow::slotserverProcess_StateChanged(QProcess::ProcessState i)
{
    if(i == 0){ ui->serverStatus->setIcon(ledred); }
    else { ui->serverStatus->setIcon(ledgreen); }
}

void MainWindow::ouvrirAbout()
{
    maFenetreAbout = new QMessageBox();

    maFenetreAbout->setText("<p align='left'>IceStream<br>"
                            "Version 2.1<br>"
                            "Conception : Julien Ottavi<br>"
                            "Development : Romain Papion<br>"
                            "Improvements : Antoine Le Bras<br>"
                            "Under GNU General Public License v2.0</p>");
    maFenetreAbout->show();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    event->accept();
    serverProcess->close();
    streamProcess->close();
    for (int ind=0;ind<=11;ind++)
    {
        playaTab.at(ind)->stopUrl();
        playaTab.at(ind)->close();
    }
}

void MainWindow::save_config()
{
    makeConfig(default_configpath);
}

void MainWindow::load_config()
{
    getConfig(default_configpath);
}

// Reads selected config file
void MainWindow::import_config()
{
    QString path = QFileDialog::getOpenFileName(this,
    tr("Open configuration"), "/home", tr("Text Files (*.txt)"));

    QFile file(QFileInfo(path).absoluteFilePath());
    if (!(file.exists()))
    { return; }

    load_config();
}

// Writes config file
void MainWindow::export_config()
{
    QString path = QFileDialog::getSaveFileName(this, tr("Save Text File"), "/home", tr("Text Files (*.txt)")); // Shows file select dialog
    if (path != "")
    {
        QFile file(QFileInfo(path).absoluteFilePath());

        if (file.exists())
        {
            QMessageBox::StandardButton chosenButton
                = QMessageBox::warning(this, tr("File exists"), tr("The file already exists. Do you want to overwrite it?"),
                    QMessageBox::Ok | QMessageBox::Cancel,
                    QMessageBox::Cancel);
            if (chosenButton != QMessageBox::Ok)
            { return; } // Save was cancelled
        }
        if (!file.open(QIODevice::WriteOnly)) { QMessageBox::critical(this, tr("Error"), tr("Failed to save file")); return; }
        file.close();
    }
    makeConfig(path);
}

// Storing every setting, one after the other
void MainWindow::makeConfig(QString path){
    QFile file(path);
    // Ouverture du fichier en écriture et en texte.
    file.open(QFile::WriteOnly | QFile::Text);
    if(!file.isOpen()) { QMessageBox::critical(this, tr("Error"), tr("Failed to save file")); return; }
    QTextStream outStream(&file);

    // Server Config
    outStream << "server_port = " << ui->Line_Local_Port->text() << Qt::endl;;
    outStream << "server_pass = " << ui->Line_Local_Pass->text() << Qt::endl;;
    outStream << "server_listeners = " << ui->Line_Local_Listen->text() << Qt::endl;;
    outStream << "server_senders = " << ui->Line_Local_Send->text() << Qt::endl;;

    // Client Config
    outStream << "client_address = " << ui->Line_Server->text() << Qt::endl;;
    outStream << "client_port = " << ui->Line_Port->text() << Qt::endl;;
    outStream << "client_mountpoint = " << ui->Line_Mount->text() << Qt::endl;;
    outStream << "client_pass = " << ui->Line_Pass->text() << Qt::endl;;
    outStream << "client_lq = " << ui->optionLQ->checkState() << Qt::endl;;
    outStream << "client_dump = " << ui->optionStreamDump->checkState() << Qt::endl;;
    // Descriptions
    outStream << "client_name = " << ui->Line_Name->text() << Qt::endl;;
    outStream << "client_desc = " << ui->Line_Desc->text() << Qt::endl;;
    outStream << "client_genre = " << ui->Line_Genre->text() << Qt::endl;;
    outStream << "client_public = " << ui->comboBox_public->currentIndex() <<  Qt::endl;;    // Setting has no purpose, yet
    // Audio Settings
    outStream << "client_format = " << ui->comboBox_Format->currentIndex() << Qt::endl;;
    switch (ui->comboBox_Format->currentIndex()) {  // could also just be saving ALL settings instead of the selected one
        case 0: // ogg
            outStream << "client_opt1 = " << ui->comboBox_SampleRate_ogg->currentIndex() << Qt::endl;;   // Instead of saving all settings, changing the content of
            outStream << "client_opt2 = " << ui->comboBox_Quality_ogg->currentIndex() << Qt::endl;;      // each comboBox on the fly when switching formats
            outStream << "client_opt3 = " << ui->comboBox_Channels_ogg->currentIndex() << Qt::endl;;      // would be a much better idea.
            break;
        case 1: // mp3
            outStream << "client_opt1 = " << ui->comboBox_SampleRate_mp3->currentIndex() << Qt::endl;;
            outStream << "client_opt2 = " << ui->comboBox_Quality_mp3->currentIndex() << Qt::endl;;
            outStream << "client_opt3 = " << ui->comboBox_StereoMode_mp3->currentIndex() << Qt::endl;;
        break;}
    file.close();
}

// Retribution of every setting, one after the other
void MainWindow::getConfig(QString path)
{
    QFile file(path);
    QString current_line;
    // Ouverture du fichier en écriture et en texte.
    file.open(QFile::ReadOnly | QFile::Text);
    if(!file.isOpen()) { QMessageBox::critical(this, tr("Error"), tr("Failed to open file")); return; }
    QTextStream outStream(&file);

    while(!outStream.atEnd()){  // Using std::string for the very handy "find()" function
        std::string a = outStream.readLine().toStdString();
        size_t pos = a.find('=');
        QString key = QString::fromStdString(a.substr(0, pos - 1));
        QString value = QString::fromStdString(a.substr(pos + 2));
        settingsMap[key] = value;
    }

    // Server
    ui->Line_Local_Port->setText(settingsMap["server_port"]);
    ui->Line_Local_Pass->setText(settingsMap["server_pass"]);
    ui->Line_Local_Listen->setText(settingsMap["server_listeners"]);
    ui->Line_Local_Send->setText(settingsMap["server_senders"]);
    // Client
    ui->Line_Server->setText(settingsMap["client_address"]);
    ui->Line_Port->setText(settingsMap["client_port"]);
    ui->Line_Mount->setText(settingsMap["client_mountpoint"]);
    ui->Line_Pass->setText(settingsMap["client_pass"]);
    switch ((settingsMap["client_lq"]).toInt()) {
        case 0: ui->optionLQ->setChecked(false); break;     // Unchecked. Same as "setCheckState( Qt::Unchecked )"
        case 2: ui->optionLQ->setChecked(true); break; }    // Checked
    switch ((settingsMap["client_dump"]).toInt()) {
        case 0: ui->optionStreamDump->setChecked(false); break;
        case 2: ui->optionStreamDump->setChecked(true); break; }
    // Descriptions
    ui->Line_Name->setText(settingsMap["client_name"]);
    ui->Line_Desc->setText(settingsMap["client_desc"]);
    ui->Line_Genre->setText(settingsMap["client_genre"]);
    ui->comboBox_public->setCurrentIndex((settingsMap["client_public"]).toInt());
    // Audio Settings
    ui->comboBox_Format->setCurrentIndex(settingsMap["client_format"].toInt());
    switch (settingsMap["client_format"].toInt()) { // Checking the setting itself instead of the comboBox, in case the ui didn't have the time to properly update
        case 0: // ogg
            ui->comboBox_SampleRate_ogg->setCurrentIndex(settingsMap["client_opt1"].toInt());
            ui->comboBox_Quality_ogg->setCurrentIndex(settingsMap["client_opt2"].toInt());
            ui->comboBox_Channels_ogg->setCurrentIndex(settingsMap["client_opt3"].toInt());
        break;
        case 1: // mp3
            ui->comboBox_SampleRate_mp3->setCurrentIndex(settingsMap["client_opt1"].toInt());
            ui->comboBox_Quality_mp3->setCurrentIndex(settingsMap["client_opt2"].toInt());
            ui->comboBox_StereoMode_mp3->setCurrentIndex(settingsMap["client_opt3"].toInt());
        break; }
}

void MainWindow::startOscServer()
{

        if (!ui->lineEditOsc->text().isEmpty())
        {
            port = ui->lineEditOsc->text().toInt();
        }
       // Bind the network interface so you can send and get messages

       //iface.setRemoteAddr(QHostAddress("192.168.0.10"));
       iface.setRemotePort(9000);
       iface.setLocalPort(port);
       ui->EditOSC->setText("Starting Osc ...");
       // Connect callbacks to get notified of new messages
       iface.connect(pattern + "player1",
       [this](const QOscMessage& msg)
       {
           // This is the message callback
           // It'll be called each time a message matching the
           // pattern you set is received.

           for(auto& arg : msg)
           {
               if(!arg.isInt())
                   continue;

               int i = arg.toInt();
               vol = i;
               std::cout << "msg received " << i << std::endl;
               this->setVolumePlayer(player_1,vol);
               ui->EditOSC->setText("msg receive volume player1 : " + QString::number(i) + "\n");
              // std::cout << "URL: " <<  << std::endl;

               Q_UNUSED(i);
           }
       });

       iface.connect(pattern + "player2",
       [this](const QOscMessage& msg)
       {
           // This is the message callback
           // It'll be called each time a message matching the
           // pattern you set is received.

           for(auto& arg : msg)
           {
               if(!arg.isInt())
                   continue;

               int i = arg.toInt();
               vol = i;
               std::cout << "msg received " << i << std::endl;
               this->setVolumePlayer(player_2,vol);
               ui->EditOSC->setText("msg receive volume player2 : " + QString::number(i) + "\n");

              // std::cout << "URL: " <<  << std::endl;

               Q_UNUSED(i);
           }
       });

       iface.connect(pattern + "player3",
       [this](const QOscMessage& msg)
       {
           // This is the message callback
           // It'll be called each time a message matching the
           // pattern you set is received.

           for(auto& arg : msg)
           {
               if(!arg.isInt())
                   continue;

               int i = arg.toInt();
               vol = i;
               std::cout << "msg received " << i << std::endl;
               this->setVolumePlayer(player_3,vol);
               ui->EditOSC->setText("msg receive volume player3 : " + QString::number(i) + "\n");

              // std::cout << "URL: " <<  << std::endl;

               Q_UNUSED(i);
           }
       });

       iface.connect(pattern + "player4",
       [this](const QOscMessage& msg)
       {
           // This is the message callback
           // It'll be called each time a message matching the
           // pattern you set is received.

           for(auto& arg : msg)
           {
               if(!arg.isInt())
                   continue;

               int i = arg.toInt();
               vol = i;
               std::cout << "msg received " << i << std::endl;
               this->setVolumePlayer(player_4,vol);
               ui->EditOSC->setText("msg receive volume player4 : " + QString::number(i) + "\n");

              // std::cout << "URL: " <<  << std::endl;

               Q_UNUSED(i);
           }
       });

       iface.connect(pattern + "player5",
       [this](const QOscMessage& msg)
       {
           // This is the message callback
           // It'll be called each time a message matching the
           // pattern you set is received.

           for(auto& arg : msg)
           {
               if(!arg.isInt())
                   continue;

               int i = arg.toInt();
               vol = i;
               std::cout << "msg received " << i << std::endl;
               this->setVolumePlayer(player_5,vol);
               ui->EditOSC->setText("msg receive volume player5 : " + QString::number(i) + "\n");

              // std::cout << "URL: " <<  << std::endl;

               Q_UNUSED(i);
           }
       });

       iface.connect(pattern + "player6",
       [this](const QOscMessage& msg)
       {
           // This is the message callback
           // It'll be called each time a message matching the
           // pattern you set is received.

           for(auto& arg : msg)
           {
               if(!arg.isInt())
                   continue;

               int i = arg.toInt();
               vol = i;
               std::cout << "msg received " << i << std::endl;
               this->setVolumePlayer(player_6,vol);
               ui->EditOSC->setText("msg receive volume player6 : " + QString::number(i) + "\n");

              // std::cout << "URL: " <<  << std::endl;

               Q_UNUSED(i);
           }
       });

       iface.connect(pattern + "player7",
       [this](const QOscMessage& msg)
       {
           // This is the message callback
           // It'll be called each time a message matching the
           // pattern you set is received.

           for(auto& arg : msg)
           {
               if(!arg.isInt())
                   continue;

               int i = arg.toInt();
               vol = i;
               std::cout << "msg received " << i << std::endl;
               this->setVolumePlayer(player_7,vol);
               ui->EditOSC->setText("msg receive volume player7 : " + QString::number(i) + "\n");

              // std::cout << "URL: " <<  << std::endl;

               Q_UNUSED(i);
           }
       });

       iface.connect(pattern + "player8",
       [this](const QOscMessage& msg)
       {
           // This is the message callback
           // It'll be called each time a message matching the
           // pattern you set is received.

           for(auto& arg : msg)
           {
               if(!arg.isInt())
                   continue;

               int i = arg.toInt();
               vol = i;
               std::cout << "msg received " << i << std::endl;
               this->setVolumePlayer(player_8,vol);
               ui->EditOSC->setText("msg receive volume player8 : " + QString::number(i) + "\n");

              // std::cout << "URL: " <<  << std::endl;

               Q_UNUSED(i);
           }
       });

       iface.connect(pattern + "player9",
       [this](const QOscMessage& msg)
       {
           // This is the message callback
           // It'll be called each time a message matching the
           // pattern you set is received.

           for(auto& arg : msg)
           {
               if(!arg.isInt())
                   continue;

               int i = arg.toInt();
               vol = i;
               std::cout << "msg received " << i << std::endl;
               this->setVolumePlayer(player_9,vol);
               ui->EditOSC->setText("msg receive volume player9 : " + QString::number(i) + "\n");

              // std::cout << "URL: " <<  << std::endl;

               Q_UNUSED(i);
           }
       });

       iface.connect(pattern + "player10",
       [this](const QOscMessage& msg)
       {
           // This is the message callback
           // It'll be called each time a message matching the
           // pattern you set is received.

           for(auto& arg : msg)
           {
               if(!arg.isInt())
                   continue;

               int i = arg.toInt();
               vol = i;
               std::cout << "msg received " << i << std::endl;
               this->setVolumePlayer(player_10,vol);
               ui->EditOSC->setText("msg receive volume player10 : " + QString::number(i) + "\n");

              // std::cout << "URL: " <<  << std::endl;

               Q_UNUSED(i);
           }
       });

       iface.connect(pattern + "player11",
       [this](const QOscMessage& msg)
       {
           // This is the message callback
           // It'll be called each time a message matching the
           // pattern you set is received.

           for(auto& arg : msg)
           {
               if(!arg.isInt())
                   continue;

               int i = arg.toInt();
               vol = i;
               std::cout << "msg received " << i << std::endl;
               this->setVolumePlayer(player_11,vol);
               ui->EditOSC->setText("msg receive volume player11 : " + QString::number(i) + "\n");

              // std::cout << "URL: " <<  << std::endl;

               Q_UNUSED(i);
           }
       });

       iface.connect(pattern + "player12",
       [this](const QOscMessage& msg)
       {
           // This is the message callback
           // It'll be called each time a message matching the
           // pattern you set is received.

           for(auto& arg : msg)
           {
               if(!arg.isInt())
                   continue;

               int i = arg.toInt();
               vol = i;
               std::cout << "msg received " << i << std::endl;
               this->setVolumePlayer(player_12,vol);
               ui->EditOSC->setText("msg receive volume player12 : " + QString::number(i) + "\n");

              // std::cout << "URL: " <<  << std::endl;

               Q_UNUSED(i);
           }
       });


}





void MainWindow::stopOscServer()
{
    iface.disconnect();
    ui->EditOSC->setText("Stopping Osc ...");
}

/*void MainWindow::setVol(int volIn)
{
    vol = volIn;
}

int MainWindow::getVol()
{
    return vol;
}*/

void MainWindow::setVolumePlayer(player* playerIn, int volumeIn)
{
    playerIn->slider->setValue(volumeIn);
    playerIn->setVolume(volumeIn);
}


/*void MainWindow::routageOsc(int playerId, int dataIn)
{
    int playerNumber = playerId;
    int calibratedData = dataIn;

                    switch(playerNumber){
                    case 0:
                    //
                    break;

                    case 1:
                    this->setVolumePlayer(player_1,calibratedData);
                    break;

                    case 2:
                    this->setVolumePlayer(player_2,calibratedData);
                    break;

                    case 3:
                    this->setVolumePlayer(player_3,calibratedData);
                    break;

                    case 4:
                    this->setVolumePlayer(player_4,calibratedData);
                    break;

                    case 5:
                    this->setVolumePlayer(player_5,calibratedData);
                    break;

                    case 6:
                    this->setVolumePlayer(player_6,calibratedData);
                    break;

                    case 7:
                    this->setVolumePlayer(player_7,calibratedData);
                    break;

                    case 8:
                    this->setVolumePlayer(player_8,calibratedData);
                    break;

                    case 9:
                    this->setVolumePlayer(player_9,calibratedData);
                    break;

                    case 10:
                    this->setVolumePlayer(player_10,calibratedData);
                    break;

                    case 11:
                    this->setVolumePlayer(player_11,calibratedData);
                    break;

                    case 12:
                    this->setVolumePlayer(player_12,calibratedData);
                    break;

                    default:
                    //None
                    break;
                }


}


void MainWindow::printLogOscServer()
{
    QString readMsg = "incoming message ...";
    readMsg = readMsg + "\n";
    readMsg = QString(myServer->getAddress()) + "\n";
    readMsg = readMsg + QString::number(myServer->getPort()) + "\n";
    readMsg = readMsg + QString::number(myServer->getData()) + "\n";
    ui->EditOSC->setText(readMsg);

}*/

void MainWindow:: solo_1()
{
    int playerNumber = 0;
    if (playaTab.at(playerNumber)->isSolo())
     {
            playaTab.at(playerNumber)->unSolo();
            for (int index=0;index<=11;index++) //Changer 11 par variable nb players
            {
                if (index!=playerNumber)
                {
                    if (playaTab.at(index)->isMuted())
                      {
                        playaTab.at(index)->muteUrl();
                        playaTab.at(index)->setUnMuted();
                      }

                 }
                    else if (!playaTab.at(index)->isMuted())
                      {
                        NULL;
                      }
            }
      }
     else if (!playaTab.at(playerNumber)->isSolo())
      {
        for (int ind=0;ind<=11;ind++)
        {
          if (ind!=playerNumber)
            {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                    }
                else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        NULL;
                    }
                else if (playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                        playaTab.at(ind)->unSolo();
                    }
                else if (playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->unSolo();
                    }

           }
            else if(ind==playerNumber)
             {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                   {
                        playaTab.at(ind)->setSolo();

                   }
            else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
            {
              playaTab.at(ind)->setSolo();
              playaTab.at(ind)->muteUrl();
              playaTab.at(ind)->setUnMuted();

            }

        }
     }
   }
}

void MainWindow:: solo_2()
{
    int playerNumber = 1;
    if (playaTab.at(playerNumber)->isSolo())
     {
            playaTab.at(playerNumber)->unSolo();
            for (int index=0;index<=11;index++) //Changer 11 par variable nb players
            {
                if (index!=playerNumber)
                {
                    if (playaTab.at(index)->isMuted())
                      {
                        playaTab.at(index)->muteUrl();
                        playaTab.at(index)->setUnMuted();
                      }

                 }
                    else if (!playaTab.at(index)->isMuted())
                      {
                        NULL;
                      }
            }
      }
     else if (!playaTab.at(playerNumber)->isSolo())
      {
        for (int ind=0;ind<=11;ind++)
        {
          if (ind!=playerNumber)
            {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                    }
                else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        NULL;
                    }
                else if (playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                        playaTab.at(ind)->unSolo();
                    }
                else if (playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->unSolo();
                    }

           }
            else if(ind==playerNumber)
             {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                   {
                        playaTab.at(ind)->setSolo();

                   }
            else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
            {
              playaTab.at(ind)->setSolo();
              playaTab.at(ind)->muteUrl();
              playaTab.at(ind)->setUnMuted();

            }

        }
     }
   }
}

void MainWindow:: solo_3()
{
    int playerNumber = 2;
    if (playaTab.at(playerNumber)->isSolo())
     {
            playaTab.at(playerNumber)->unSolo();
            for (int index=0;index<=11;index++) //Changer 11 par variable nb players
            {
                if (index!=playerNumber)
                {
                    if (playaTab.at(index)->isMuted())
                      {
                        playaTab.at(index)->muteUrl();
                        playaTab.at(index)->setUnMuted();
                      }

                 }
                    else if (!playaTab.at(index)->isMuted())
                      {
                        NULL;
                      }
            }
      }
     else if (!playaTab.at(playerNumber)->isSolo())
      {
        for (int ind=0;ind<=11;ind++)
        {
          if (ind!=playerNumber)
            {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                    }
                else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        NULL;
                    }
                else if (playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                        playaTab.at(ind)->unSolo();
                    }
                else if (playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->unSolo();
                    }

           }
            else if(ind==playerNumber)
             {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                   {
                        playaTab.at(ind)->setSolo();

                   }
            else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
            {
              playaTab.at(ind)->setSolo();
              playaTab.at(ind)->muteUrl();
              playaTab.at(ind)->setUnMuted();

            }

        }
     }
   }
}

void MainWindow:: solo_4()
{
    int playerNumber = 3;
    if (playaTab.at(playerNumber)->isSolo())
     {
            playaTab.at(playerNumber)->unSolo();
            for (int index=0;index<=11;index++) //Changer 11 par variable nb players
            {
                if (index!=playerNumber)
                {
                    if (playaTab.at(index)->isMuted())
                      {
                        playaTab.at(index)->muteUrl();
                        playaTab.at(index)->setUnMuted();
                      }

                 }
                    else if (!playaTab.at(index)->isMuted())
                      {
                        NULL;
                      }
            }
      }
     else if (!playaTab.at(playerNumber)->isSolo())
      {
        for (int ind=0;ind<=11;ind++)
        {
          if (ind!=playerNumber)
            {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                    }
                else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        NULL;
                    }
                else if (playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                        playaTab.at(ind)->unSolo();
                    }
                else if (playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->unSolo();
                    }

           }
            else if(ind==playerNumber)
             {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                   {
                        playaTab.at(ind)->setSolo();

                   }
            else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
            {
              playaTab.at(ind)->setSolo();
              playaTab.at(ind)->muteUrl();
              playaTab.at(ind)->setUnMuted();

            }

        }
     }
   }
}

void MainWindow:: solo_5()
{
    int playerNumber = 4;
    if (playaTab.at(playerNumber)->isSolo())
     {
            playaTab.at(playerNumber)->unSolo();
            for (int index=0;index<=11;index++) //Changer 11 par variable nb players
            {
                if (index!=playerNumber)
                {
                    if (playaTab.at(index)->isMuted())
                      {
                        playaTab.at(index)->muteUrl();
                        playaTab.at(index)->setUnMuted();
                      }

                 }
                    else if (!playaTab.at(index)->isMuted())
                      {
                        NULL;
                      }
            }
      }
     else if (!playaTab.at(playerNumber)->isSolo())
      {
        for (int ind=0;ind<=11;ind++)
        {
          if (ind!=playerNumber)
            {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                    }
                else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        NULL;
                    }
                else if (playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                        playaTab.at(ind)->unSolo();
                    }
                else if (playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->unSolo();
                    }

           }
            else if(ind==playerNumber)
             {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                   {
                        playaTab.at(ind)->setSolo();

                   }
            else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
            {
              playaTab.at(ind)->setSolo();
              playaTab.at(ind)->muteUrl();
              playaTab.at(ind)->setUnMuted();

            }

        }
     }
   }
}

void MainWindow:: solo_6()
{
    int playerNumber = 5;
    if (playaTab.at(playerNumber)->isSolo())
     {
            playaTab.at(playerNumber)->unSolo();
            for (int index=0;index<=11;index++) //Changer 11 par variable nb players
            {
                if (index!=playerNumber)
                {
                    if (playaTab.at(index)->isMuted())
                      {
                        playaTab.at(index)->muteUrl();
                        playaTab.at(index)->setUnMuted();
                      }

                 }
                    else if (!playaTab.at(index)->isMuted())
                      {
                        NULL;
                      }
            }
      }
     else if (!playaTab.at(playerNumber)->isSolo())
      {
        for (int ind=0;ind<=11;ind++)
        {
          if (ind!=playerNumber)
            {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                    }
                else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        NULL;
                    }
                else if (playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                        playaTab.at(ind)->unSolo();
                    }
                else if (playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->unSolo();
                    }

           }
            else if(ind==playerNumber)
             {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                   {
                        playaTab.at(ind)->setSolo();

                   }
            else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
            {
              playaTab.at(ind)->setSolo();
              playaTab.at(ind)->muteUrl();
              playaTab.at(ind)->setUnMuted();

            }

        }
     }
   }
}

void MainWindow:: solo_7()
{
    int playerNumber = 6;
    if (playaTab.at(playerNumber)->isSolo())
     {
            playaTab.at(playerNumber)->unSolo();
            for (int index=0;index<=11;index++) //Changer 11 par variable nb players
            {
                if (index!=playerNumber)
                {
                    if (playaTab.at(index)->isMuted())
                      {
                        playaTab.at(index)->muteUrl();
                        playaTab.at(index)->setUnMuted();
                      }

                 }
                    else if (!playaTab.at(index)->isMuted())
                      {
                        NULL;
                      }
            }
      }
     else if (!playaTab.at(playerNumber)->isSolo())
      {
        for (int ind=0;ind<=11;ind++)
        {
          if (ind!=playerNumber)
            {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                    }
                else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        NULL;
                    }
                else if (playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                        playaTab.at(ind)->unSolo();
                    }
                else if (playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->unSolo();
                    }

           }
            else if(ind==playerNumber)
             {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                   {
                        playaTab.at(ind)->setSolo();

                   }
            else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
            {
              playaTab.at(ind)->setSolo();
              playaTab.at(ind)->muteUrl();
              playaTab.at(ind)->setUnMuted();

            }

        }
     }
   }
}

void MainWindow:: solo_8()
{
    int playerNumber = 7;
    if (playaTab.at(playerNumber)->isSolo())
     {
            playaTab.at(playerNumber)->unSolo();
            for (int index=0;index<=11;index++) //Changer 11 par variable nb players
            {
                if (index!=playerNumber)
                {
                    if (playaTab.at(index)->isMuted())
                      {
                        playaTab.at(index)->muteUrl();
                        playaTab.at(index)->setUnMuted();
                      }

                 }
                    else if (!playaTab.at(index)->isMuted())
                      {
                        NULL;
                      }
            }
      }
     else if (!playaTab.at(playerNumber)->isSolo())
      {
        for (int ind=0;ind<=11;ind++)
        {
          if (ind!=playerNumber)
            {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                    }
                else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        NULL;
                    }
                else if (playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                        playaTab.at(ind)->unSolo();
                    }
                else if (playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->unSolo();
                    }

           }
            else if(ind==playerNumber)
             {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                   {
                        playaTab.at(ind)->setSolo();

                   }
            else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
            {
              playaTab.at(ind)->setSolo();
              playaTab.at(ind)->muteUrl();
              playaTab.at(ind)->setUnMuted();

            }

        }
     }
   }
}

void MainWindow:: solo_9()
{
    int playerNumber = 8;
    if (playaTab.at(playerNumber)->isSolo())
     {
            playaTab.at(playerNumber)->unSolo();
            for (int index=0;index<=11;index++) //Changer 11 par variable nb players
            {
                if (index!=playerNumber)
                {
                    if (playaTab.at(index)->isMuted())
                      {
                        playaTab.at(index)->muteUrl();
                        playaTab.at(index)->setUnMuted();
                      }

                 }
                    else if (!playaTab.at(index)->isMuted())
                      {
                        NULL;
                      }
            }
      }
     else if (!playaTab.at(playerNumber)->isSolo())
      {
        for (int ind=0;ind<=11;ind++)
        {
          if (ind!=playerNumber)
            {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                    }
                else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        NULL;
                    }
                else if (playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                        playaTab.at(ind)->unSolo();
                    }
                else if (playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->unSolo();
                    }

           }
            else if(ind==playerNumber)
             {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                   {
                        playaTab.at(ind)->setSolo();

                   }
            else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
            {
              playaTab.at(ind)->setSolo();
              playaTab.at(ind)->muteUrl();
              playaTab.at(ind)->setUnMuted();

            }

        }
     }
   }
}

void MainWindow:: solo_10()
{
    int playerNumber = 9;
    if (playaTab.at(playerNumber)->isSolo())
     {
            playaTab.at(playerNumber)->unSolo();
            for (int index=0;index<=11;index++) //Changer 11 par variable nb players
            {
                if (index!=playerNumber)
                {
                    if (playaTab.at(index)->isMuted())
                      {
                        playaTab.at(index)->muteUrl();
                        playaTab.at(index)->setUnMuted();
                      }

                 }
                    else if (!playaTab.at(index)->isMuted())
                      {
                        NULL;
                      }
            }
      }
     else if (!playaTab.at(playerNumber)->isSolo())
      {
        for (int ind=0;ind<=11;ind++)
        {
          if (ind!=playerNumber)
            {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                    }
                else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        NULL;
                    }
                else if (playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                        playaTab.at(ind)->unSolo();
                    }
                else if (playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->unSolo();
                    }

           }
            else if(ind==playerNumber)
             {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                   {
                        playaTab.at(ind)->setSolo();

                   }
            else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
            {
              playaTab.at(ind)->setSolo();
              playaTab.at(ind)->muteUrl();
              playaTab.at(ind)->setUnMuted();

            }

        }
     }
   }
}

void MainWindow:: solo_11()
{
    int playerNumber = 10;
    if (playaTab.at(playerNumber)->isSolo())
     {
            playaTab.at(playerNumber)->unSolo();
            for (int index=0;index<=11;index++) //Changer 11 par variable nb players
            {
                if (index!=playerNumber)
                {
                    if (playaTab.at(index)->isMuted())
                      {
                        playaTab.at(index)->muteUrl();
                        playaTab.at(index)->setUnMuted();
                      }

                 }
                    else if (!playaTab.at(index)->isMuted())
                      {
                        NULL;
                      }
            }
      }
     else if (!playaTab.at(playerNumber)->isSolo())
      {
        for (int ind=0;ind<=11;ind++)
        {
          if (ind!=playerNumber)
            {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                    }
                else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        NULL;
                    }
                else if (playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                        playaTab.at(ind)->unSolo();
                    }
                else if (playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->unSolo();
                    }

           }
            else if(ind==playerNumber)
             {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                   {
                        playaTab.at(ind)->setSolo();

                   }
            else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
            {
              playaTab.at(ind)->setSolo();
              playaTab.at(ind)->muteUrl();
              playaTab.at(ind)->setUnMuted();

            }

        }
     }
   }
}

void MainWindow:: solo_12()
{
    int playerNumber = 11;
    if (playaTab.at(playerNumber)->isSolo())
     {
            playaTab.at(playerNumber)->unSolo();
            for (int index=0;index<=11;index++) //Changer 11 par variable nb players
            {
                if (index!=playerNumber)
                {
                    if (playaTab.at(index)->isMuted())
                      {
                        playaTab.at(index)->muteUrl();
                        playaTab.at(index)->setUnMuted();
                      }

                 }
                    else if (!playaTab.at(index)->isMuted())
                      {
                        NULL;
                      }
            }
      }
     else if (!playaTab.at(playerNumber)->isSolo())
      {
        for (int ind=0;ind<=11;ind++)
        {
          if (ind!=playerNumber)
            {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                    }
                else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        NULL;
                    }
                else if (playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->muteUrl();
                        playaTab.at(ind)->setMuted();
                        playaTab.at(ind)->unSolo();
                    }
                else if (playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
                    {
                        playaTab.at(ind)->unSolo();
                    }

           }
            else if(ind==playerNumber)
             {
                if (!playaTab.at(ind)->isSolo() && !playaTab.at(ind)->isMuted())
                   {
                        playaTab.at(ind)->setSolo();

                   }
            else if (!playaTab.at(ind)->isSolo() && playaTab.at(ind)->isMuted())
            {
              playaTab.at(ind)->setSolo();
              playaTab.at(ind)->muteUrl();
              playaTab.at(ind)->setUnMuted();

            }

        }
     }
   }
}


/*void MainWindow:: soloPlaying()
{


      solo_1();
      break;
     case 2:
      solo_2();
      break;
     case 3:
      solo_3();
      break;
     case 4:
      solo_4();
      break;
     case 5:
      solo_5();
      break;
     case 6:
      solo_6();
      break;
     case 7:
      solo_7();
      break;
     case 8:
      solo_8();
      break;
     case 9:
      solo_9();
      break;
     case 10:
      solo_10();
      break;
     case 11:
      solo_11();
      break;
     case 12:
      solo_12();
      break;
     default:
       break;
     }
    }

*/
