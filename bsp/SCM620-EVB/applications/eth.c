#include "board.h"
#include "rtthread.h"
#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include <dfs_fs.h>
#include <dfs_file.h>
#include "eth.h"

struct struct_Ethernet_Par Ethernet_Par;
uint16_t *P_ethernet = ( uint16_t *) &Ethernet_Par;
