/**
* This file is part of the vendoring machine project.
*
* Copyright 2018, Huang Yang <elious.huang@gmail.com>. All rights reserved.
*
* See the COPYING file for the terms of usage and distribution.
*/
#ifndef _WIFI_H_
  #define _WIFI_H_

#include "types.h"

BEGIN_DECLS

bool wifi_init(void);
void wifi_update_motor_status(void);

END_DECLS

#endif /* _WIFI_H_ */
