#include "server.h"
#include <string.h>
#include <fcntl.h>
#include <iostream>

CNetServer::~CNetServer()
{
    erase();
}

bool CNetServer::list(const int max_connect)
{
    if(!isValid()) {
	throw NetException("Server Socket error");
    }

    _maxConnect = max_connect;
    int result = ::listen(_sock, _maxConnect);

    if(result < 0) {
	throw NetException("Server Listen Error");
    }

    return true;
}

CNetClient* CNetServer::accept()
{
    struct sockaddr_in addr;
    int addrLen = sizeof(addr);

    memset(&addr, 0, addrLen);

    int sock = ::accept(_sock, (struct sockaddr *) &addr, (socklen_t *) &addrLen);
    ++_clientsCount;
    _clients[_clientsCount] = new CNetClient(sock, addr);
    std::cout << _clients[_clientsCount] << std::endl;
    
    return _clients[_clientsCount];
}

void CNetServer::erase()
{
    for (_iter = _clients.begin(); _iter != _clients.end(); _iter++)
    {
	(*_iter).second->erase();
	delete (*_iter).second;
    }
    _clients.clear();
    _clientsCount = 0;
}
