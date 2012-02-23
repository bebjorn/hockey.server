#ifndef SERIALCOMM_INCLUDED
#define SERIALCOMM_INCLUDED
#include <stdio.h>
#include <string>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

class SerialComm {
public:
    int port_fd;
    struct termios oldtio,newtio;


    std::string port;
    uint baudrate;

    SerialComm();
     ~SerialComm();
    int initialise_port();
    int send_data(std::string data);
    int recieve_data(char *buff, int rdsize);
    int close_port();
};

SerialComm::SerialComm(){
}

SerialComm::~SerialComm(){
    //destructor
}

int SerialComm::initialise_port(){

   port_fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);

    if (port_fd == -1 ){
        perror("open_port: Unable to open port");
        return -1;

    } else{

        fcntl(port_fd, F_SETFL,0);

        tcgetattr (port_fd, &oldtio); ///< save current serial port settingsBRKINT | ICRNL | INPCK | ISTRIP | IXON | IXOFF);
        tcgetattr (port_fd, &newtio); ///< save current serial port settings to new struct

        // set new parameters to the serial device

        newtio.c_cflag |= B2400; // Set Baudrate first time
        newtio.c_cflag |= CLOCAL; // Local line - do not change "owner" of port
        newtio.c_cflag |= CREAD; // Enable receiver

        // set to 8N1
        newtio.c_cflag &= ~PARENB; // no parentybyte
        newtio.c_cflag &= ~CSTOPB; // 1 stop bit
        newtio.c_cflag &= ~CSIZE; // Mask the character size bits
        newtio.c_cflag |= CS8; // 8 data bits

        // Set the baudrate
        cfsetispeed(&newtio, B2400);
        cfsetospeed(&newtio, B2400);

        tcflush(port_fd, TCIFLUSH); // flush pending data

        // set the new defined settings
        if (tcsetattr(port_fd, TCSANOW, &newtio)) {
            perror("could not set the serial settings!");
            return -1;
        }

        printf("%s has been sucessfully opened\n", port.c_str());

        return port_fd;
//OLD VERSION
//        struct termios oldtio, newtio;
//        /*First save current serial port settings
//        tcgetattr(port_fd, &oldtio);
//        /* clear struct for new port settings
//        bzero(&newtio, sizeof(newtio)); /* clear struct for new port settings */
//
//
///*We need to set the following:
//struct termios {
//    tcflag_t c_iflag;		 input mode flags
//    tcflag_t c_oflag;		 output mode flags
//    tcflag_t c_cflag;		 control mode flags
//    tcflag_t c_lflag;		 local mode flags
//    cc_t c_line;			 line discipline
//    cc_t c_cc[NCCS];		 control characters
//    speed_t c_ispeed;		 input speed
//    speed_t c_ospeed;		 output speed
//  };
//  */
//
//
//        /*
//          IGNPAR  : ignore bytes with parity errors
//          ICRNL   : map CR to NL (otherwise a CR input on the other computer
//                    will not terminate input)
//          otherwise make device raw (no other input processing)
//
//         newtio.c_iflag = IGNPAR | ICRNL;
//
//        /*
//         Raw output.
//
//         newtio.c_oflag = 0;
//
//        /*
//          BAUDRATE: Set bps rate. You could also use cfsetispeed and cfsetospeed.
//          CRTSCTS : output hardware flow control (only used if the cable has
//                    all necessary lines. See sect. 7 of Serial-HOWTO)
//          CS8     : 8n1 (8bit,no parity,1 stopbit)
//          CLOCAL  : local connection, no modem contol
//          CREAD   : enable receiving characters
//
//		newtio.c_cflag = CS8 | CLOCAL | CREAD;
//
//		cfsetspeed(&newtio, baudrate);
//		cfsetospeed(&newtio, baudrate);
//
//
//        /*
//          now clean the modem line and activate the settings for the port
//
//         tcflush(port_fd, TCIFLUSH);
//         tcsetattr(port_fd, TCSANOW,&newtio);
//

    }

}

int SerialComm::send_data(std::string data){

	int wr;
	wr = write(port_fd, data.c_str(), data.length());
	if (wr != -1){
		return wr;
	} else {
		return wr;
	}
}

int SerialComm::recieve_data(char *buff, int rdsize){

//    fd_set* rfds;
//    timeval* timeout;
//    FD_ZERO (rfds);
//    FD_SET (port_fd, rfds);
//    int retval = select (port_fd + 1, rfds, NULL, NULL, timeout);
//	if(retval){
//        printf("Input available and reading\n");
//    }else{
//        printf("Error, no data available for reading\n");
//        return -1;
//    }
	int rd=read(port_fd, buff, rdsize);
    return rd;
}

int SerialComm::close_port(){

    tcflush(port_fd, TCIFLUSH);
    tcsetattr(port_fd, TCSANOW, &oldtio);
	close(port_fd);

    return 0;
}



#endif // SERIALCOMM_INCLUDED
