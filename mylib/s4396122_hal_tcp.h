/**
 * @file s4396122_hal_tcp.h
 * @brief Library for reading and writing to a tcp connection
 * @author Daniel Fitzmaurice - 43961229
 * @version 1
 * @date 2017-04-30
 */
#ifndef HAL_TCP_H
#define HAL_TCP_H

#include "FreeRTOS.h"
#include "netconfig.h"
#include "task.h"
#include "s4396122_util_print.h"
#include "s4396122_util_int_queue.h"

#include "tcpip.h"
#include "lwip/opt.h"
#include "lwip/api.h"
#include "lwip/sys.h"
#include "lwip/sockets.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define TCP_SOCKET_PORT 10  //!< Port to bound server to

/**
 * @brief Structure containing all the information required for a network 
 * connection
 */
struct tcpConnection {
    int fp;             //!< File descriptor of the current connection
    IntQueue *input;    //!< Integer buffer of the current network input
    int open;           //!< Status of if the network is still open or not
};

extern void s4396122_hal_tcp_init();
extern struct tcpConnection s4396122_hal_tcp_accept();
extern void s4396122_hal_tcp_read(struct tcpConnection *conn, 
        void (*f)(IntQueue *));
extern void s4396122_hal_tcp_print(struct tcpConnection *conn, 
        IntQueue *message);

#endif
