/*********************************************************************************
 *     File Name           :     /components/IridiumSBD/includes/IridiumSBDComponent.h
 *     Created By          :     jon
 *     Creation Date       :     [2022-10-21 00:33]
 *     Last Modified       :     [2022-10-26 02:09]
 *     Description         :     Component to handle the task for the IridiumSBD
 **********************************************************************************/

#ifndef __IRIDIUMSBD_COMPONENT_H__
#define __IRIDIUMSBD_COMPONENT_H__

#include <stdio.h>
#include <sstream>
#include <string>
#include <time.h>

#include "ComBus.h"

class RFDComponent
{
public:
  RFDComponent()
  {

  }
  static void vMainLoop_Task(void *arg);

private:
  void vRX();
  void checkQueue();

};

#endif /* __IRIDIUMSBD_COMPONENT_H__ */
