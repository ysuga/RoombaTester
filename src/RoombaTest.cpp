// -*- C++ -*-
/*!
 * @file  RoombaTest.cpp
 * @brief Roomba Test RTC
 * @date $Date$
 *
 * $Id$
 */

#include "RoombaTest.h"

#include "YConsole.h"
using namespace net::ysuga;

// Module specification
// <rtc-template block="module_spec">
static const char* roombatest_spec[] =
  {
    "implementation_id", "RoombaTest",
    "type_name",         "RoombaTest",
    "description",       "Roomba Test RTC",
    "version",           "1.0.0",
    "vendor",            "sugarsweetrobotics.com",
    "category",          "experimenta",
    "activity_type",     "PERIODIC",
    "kind",              "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
  };
// </rtc-template>

/*!
 * @brief constructor
 * @param manager Maneger Object
 */
RoombaTest::RoombaTest(RTC::Manager* manager)
    // <rtc-template block="initializer">
  : RTC::DataFlowComponentBase(manager),
    m_currentVelIn("currentVel", m_currentVel),
    m_currentPosIn("currentPos", m_currentPos),
    m_targetVelOut("targetVel", m_targetVel),
    m_serviceNameOut("service", m_serviceName)

    // </rtc-template>
{
}

/*!
 * @brief destructor
 */
RoombaTest::~RoombaTest()
{
}



RTC::ReturnCode_t RoombaTest::onInitialize()
{
  // Registration: InPort/OutPort/Service
  // <rtc-template block="registration">
  // Set InPort buffers
  addInPort("currentVel", m_currentVelIn);
  addInPort("currentPos", m_currentPosIn);
  
  // Set OutPort buffer
  addOutPort("targetVel", m_targetVelOut);
  addOutPort("service", m_serviceNameOut);
  
  // Set service provider to Ports
  
  // Set service consumers to Ports
  
  // Set CORBA Service Ports
  
  // </rtc-template>

  // <rtc-template block="bind_config">
  // </rtc-template>
  
  init_scr();
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RoombaTest::onFinalize()
{
  exit_scr();
  return RTC::RTC_OK;
}


/*
RTC::ReturnCode_t RoombaTest::onStartup(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RoombaTest::onShutdown(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/


RTC::ReturnCode_t RoombaTest::onActivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RoombaTest::onDeactivated(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RoombaTest::onExecute(RTC::UniqueId ec_id)
{
  clear_scr();
  if(myKbhit()) {
    int c = myGetch();
    switch(c) {
    case 'd':
      m_serviceName.data = "dock";
      m_serviceNameOut.write();
      break;
    case 'c':
      m_serviceName.data = "clean";
      m_serviceNameOut.write();
      break;
    default:
      break;
    }
  }
  return RTC::RTC_OK;
}

/*
RTC::ReturnCode_t RoombaTest::onAborting(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RoombaTest::onError(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RoombaTest::onReset(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RoombaTest::onStateUpdate(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/

/*
RTC::ReturnCode_t RoombaTest::onRateChanged(RTC::UniqueId ec_id)
{
  return RTC::RTC_OK;
}
*/



extern "C"
{
 
  void RoombaTestInit(RTC::Manager* manager)
  {
    coil::Properties profile(roombatest_spec);
    manager->registerFactory(profile,
                             RTC::Create<RoombaTest>,
                             RTC::Delete<RoombaTest>);
  }
  
};


