/**
 * Project Untitled
 */


#ifndef _SOCKETIO_H
#define _SOCKETIO_H

class SocketIO {
public: 
    
/**
 * @param fd
 */
SocketIO(int fd);
    
~SocketIO();
    
/**
 * @param buf
 * @param len
 */
void read(char * buf, int len);
    
/**
 * @param buf
 * @param len
 */
void write(const char * buf, int len);
private: 
    int _fd;
};

#endif //_SOCKETIO_H
