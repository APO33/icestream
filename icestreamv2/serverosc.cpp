#include "serverosc.h"
//#include "player.h"


//QByteArray* serverosc::message;

serverosc::serverosc()
{

    address = "/icestream/volume/";
    playerNumber = 0;
    playerIn = "player_" + QString::number(playerNumber);
    port = 7770;
    receivedData = "10";


    void error(int num, const char *m, const char *path);

    int generic_handler(const char *path, const char *types, lo_arg ** argv,
                        int argc, void *data, void *user_data);
        st = lo_server_thread_new("7770", error);

        /*
         * Counter for number of messages received -- we use an atomic
         * because it will be updated in a background thread.
         */
        std::atomic<int> received(0);

        /*
         * Add a method handler for "/example,i" using a C++11 lambda to
         * keep it succinct.  We capture a reference to the `received'
         * count and modify it atomatically.
         *
         * You can also pass in a normal function, or a callable function
         * object.
         *
         * Note: If the lambda doesn't specify a return value, the default
         *       is `return 0', meaning "this message has been handled,
         *       don't continue calling the method chain."  If this is not
         *       the desired behaviour, add `return 1' to your method
         *       handlers.
         */
        lo::ServerThreadstadd_method("example", "i",
                      [&received](lo_arg **argv, int)
                      {std::cout << "example (" << (++received) << "): "
                                 << argv[0]->i << std::endl;});

        /*
         * Start the server.
         */

   }


serverosc::~serverosc()
{

}

//void serverosc::startServerOsc()

void serverosc::startServerOsc()
{
    st.start();
}
void serverosc::stopServerOsc()
{
    st.start();
}

 void serverosc::setAddress(QString add)
 {
    address = add;
 }

  QString serverosc::getAddress()
 {
    return address;
 }

 void serverosc::setPort(int portIn)
 {
    port = portIn;
 }

  int serverosc::getPort()
 {
    return port;

 }

  void serverosc::setData(int playerIn, int datas)
 {
    readData = datas;
    playerNumber = playerIn;
    volume[0]=datas;
    volume[1]=playerIn;
 }

  int serverosc::getId()
  {
      return volume[0];
  }

  int serverosc::getData()
 {
    return volume[1];
 }
