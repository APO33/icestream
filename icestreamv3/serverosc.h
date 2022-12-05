#ifndef SERVEROSC_H
#define SERVEROSC_H

//#include <QMainWindow>
#include <QObject>
//#include <QWidget>
#include <QApplication>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <atomic>
#ifndef WIN32
#include <unistd.h>
#endif
#include <lo/lo.h>
#include <lo/lo_cpp.h>


class serverosc
{
public:
    serverosc();
    ~serverosc();
    void setAddress(QString add);
    QString getAddress();
    void setPort(int portIn);
    int getPort();
    void setData(int playerIn,int datas);
    int getData();
    void startServerOsc();
    void stopServerOsc();
    void processOSC(const QByteArray& data);
    int getId();
    void error(int num, const char *m, const char *path);

    int generic_handler(const char *path, const char *types, lo_arg ** argv,
                        int argc, void *data, void *user_data);
    //static void setMsg();
    //static QByteArray* getMsg();

private:
    int done;
    int port;
    int playerNumber;
    int volume[2];
   // struct volumeIn{int playerNumber;int readData;};
    QString address;
    QString playerIn;
    QString receivedData;
    int readData;
    lo_server_thread st;





};

#endif // SERVEROSC_H
