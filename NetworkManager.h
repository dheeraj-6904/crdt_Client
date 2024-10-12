#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include <string>
#include <boost/asio.hpp>

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();
    
    bool connectToServer(const std::string& ip, int port);
    void sendChangeToServer(const std::string& operation, int x, int y);
    std::string receiveData();  // Receive data from server
    
private:
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket* socket;
};

#endif
