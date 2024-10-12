#include "NetworkManager.h"
#include <boost/asio.hpp>
#include <iostream>

NetworkManager::NetworkManager() {
    socket = new boost::asio::ip::tcp::socket(io_service);
}

NetworkManager::~NetworkManager() {
    delete socket;
}

bool NetworkManager::connectToServer(const std::string& ip, int port) {
    try {
        boost::asio::ip::tcp::resolver resolver(io_service);
        boost::asio::ip::tcp::resolver::query query(ip, std::to_string(port));
        boost::asio::connect(*socket, resolver.resolve(query));
        std::cout << "Connected to server at " << ip << ":" << port << std::endl;
        return true;
    } catch (...) {
        std::cerr << "Error connecting to server!" << std::endl;
        return false;
    }
}

void NetworkManager::sendChangeToServer(const std::string& operation, int x, int y) {
    try {
        std::string message = "Operation: " + operation + " | Cursor: (" + std::to_string(x) + ", " + std::to_string(y) + ")\n";
        boost::asio::write(*socket, boost::asio::buffer(message));
    } catch (...) {
        std::cerr << "Error sending data to server!" << std::endl;
    }
}

std::string NetworkManager::receiveData() {
    char reply[1024];
    size_t reply_length = socket->read_some(boost::asio::buffer(reply));
    return std::string(reply, reply_length);
}
