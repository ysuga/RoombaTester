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

void onMouse(int nEvent, int x, int y, int nFlags, void* param);
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
    m_currentImgIn("currentImg", m_currentImg),
    m_currentVelIn("currentVel", m_currentVel),
    m_currentPosIn("currentPos", m_currentPos),
    m_targetVelOut("targetVel", m_targetVel),
    m_serviceNameOut("service", m_serviceName),
    m_pCapturedImage(NULL), m_Height(0), m_Width(0)

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
  addInPort("currentImg", m_currentImgIn);
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
  
  //  init_scr();
  
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RoombaTest::onFinalize()
{
  //  exit_scr();
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
  m_pCapturedImage = NULL;
  m_pDisplayImage = NULL;
  m_pControllerImage = NULL;
  m_Height = m_Width = 0;
  cvNamedWindow("RoombaTester", CV_WINDOW_AUTOSIZE);
  cvSetMouseCallback("RoombaTester", onMouse, (void*)this);

  m_MousePressed = false;
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RoombaTest::onDeactivated(RTC::UniqueId ec_id)
{
  if(m_pCapturedImage != NULL) {
    cvReleaseImage(&m_pCapturedImage);
  }

  if(m_pDisplayImage != NULL) {
    cvReleaseImage(&m_pDisplayImage);
  }

  if(m_pControllerImage != NULL) {
    cvReleaseImage(&m_pControllerImage);
  }

  cvDestroyWindow("RoombaTester");
  return RTC::RTC_OK;
}


RTC::ReturnCode_t RoombaTest::onExecute(RTC::UniqueId ec_id)
{
  int c = -1;
  //  clear_scr();
  c = cvWaitKey(1);
  if(myKbhit()) {
    c = myGetch();
  }

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


  if(m_currentImgIn.isNew()) {
    m_currentImgIn.read();
    if(m_currentImg.pixels.length() <= 0) {
      return RTC::RTC_OK;
    }
    
    if(m_Height != (int)m_currentImg.height ||
       m_Width  != (int)m_currentImg.width) {
      if(m_pCapturedImage != NULL) {
	cvReleaseImage(&m_pCapturedImage);
	cvReleaseImage(&m_pDisplayImage);
	cvReleaseImage(&m_pControllerImage);
      }

      m_Height = m_currentImg.height;
      m_Width = m_currentImg.width;
      //      cvResizeWindow("RoombaTester", 640, 640 * m_Height / m_Width);
      m_pCapturedImage = cvCreateImage(cvSize(m_Width, m_Height), IPL_DEPTH_8U, 3);
      int width = 640;
      int height = 640 * m_Height / m_Width;
      m_pDisplayImage = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
      m_pControllerImage = cvCreateImage(cvSize(width, height), IPL_DEPTH_8U, 3);
      CvPoint pt;
      CvScalar color = CV_RGB(50, 50, 255);
      pt.x = width/2; pt.y = height/2;
      int radius = pt.y - 80;
      m_ButtonRadius = 50;
      cvCircle(m_pControllerImage, pt, radius, color, 3);
      m_UpBtn.x = width/2; m_UpBtn.y = height/2-radius;
      m_DownBtn.x = width/2; m_DownBtn.y = height/2+radius;
      m_RightBtn.x = width/2 + radius; m_RightBtn.y = height/2;
      m_LeftBtn.x = width/2 - radius; m_LeftBtn.y = height/2;
      m_CleanBtn.x = m_ButtonRadius + 30; m_CleanBtn.y = height/2 - radius;
      m_DockBtn.x = m_ButtonRadius + 30; m_DockBtn.y = height/2 + radius;


      cvCircle(m_pControllerImage, m_UpBtn, m_ButtonRadius, color, -1);
      cvCircle(m_pControllerImage, m_DownBtn, m_ButtonRadius, color, -1);
      cvCircle(m_pControllerImage, m_RightBtn, m_ButtonRadius, color, -1);
      cvCircle(m_pControllerImage, m_LeftBtn, m_ButtonRadius, color, -1);
      cvCircle(m_pControllerImage, m_CleanBtn, m_ButtonRadius, color, -1);
      cvCircle(m_pControllerImage, m_DockBtn, m_ButtonRadius, color, -1);
      
      CvFont font;
      cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 1, 1, 1, 3);
      CvSize textSize;
      int baseLine;
      CvPoint textPt;

      cvGetTextSize("Fwd", &font, &textSize, &baseLine);
      textPt.x = m_UpBtn.x - textSize.width/2;
      textPt.y = m_UpBtn.y + textSize.height/2;
      cvPutText(m_pControllerImage, "Fwd", textPt, &font, CV_RGB(0, 0, 0));

      cvGetTextSize("Back", &font, &textSize, &baseLine);
      textPt.x = m_DownBtn.x - textSize.width/2;
      textPt.y = m_DownBtn.y + textSize.height/2;
      cvPutText(m_pControllerImage, "Back", textPt, &font, CV_RGB(0, 0, 0));

      cvGetTextSize("Right", &font, &textSize, &baseLine);
      textPt.x = m_RightBtn.x - textSize.width/2;
      textPt.y = m_RightBtn.y + textSize.height/2;
      cvPutText(m_pControllerImage, "Right", textPt, &font, CV_RGB(0, 0, 0));

      cvGetTextSize("Left", &font, &textSize, &baseLine);
      textPt.x = m_LeftBtn.x - textSize.width/2;
      textPt.y = m_LeftBtn.y + textSize.height/2;
      cvPutText(m_pControllerImage, "Left", textPt, &font, CV_RGB(0, 0, 0));
      

      cvGetTextSize("Clean", &font, &textSize, &baseLine);
      textPt.x = m_CleanBtn.x - textSize.width/2;
      textPt.y = m_CleanBtn.y + textSize.height/2;
      cvPutText(m_pControllerImage, "Clean", textPt, &font, CV_RGB(0, 0, 0));

      cvGetTextSize("Dock", &font, &textSize, &baseLine);
      textPt.x = m_DockBtn.x - textSize.width/2;
      textPt.y = m_DockBtn.y + textSize.height/2;
      cvPutText(m_pControllerImage, "Dock", textPt, &font, CV_RGB(0, 0, 0));
    }

    memcpy(m_pCapturedImage->imageData, (void*)&(m_currentImg.pixels[0]), m_currentImg.pixels.length());
    
    cvResize(m_pCapturedImage, m_pDisplayImage);
    cvAddWeighted(m_pDisplayImage, 1, m_pControllerImage, 0.5, 0, m_pDisplayImage);

  }

#if (!defined WIN32) || (!defined WIN64)
  cvStartWindowThread();
#endif
  cvShowImage("RoombaTester", m_pDisplayImage);
    
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

static bool isInBtn(CvPoint& Btn, int x, int y, int Radius)
{
  int dx = Btn.x - x; int dy = Btn.y - y;
  if(dx*dx + dy*dy <= Radius*Radius) {
    return true;
  }
  return false;
}

void onMouse(int nEvent, int x, int y, int nFlags, void* param)
{
  RoombaTest* rtc = (RoombaTest*)param;
  bool service = false;
  bool control = false;
  switch(nEvent) {
  case CV_EVENT_LBUTTONUP:
    if(rtc->m_MousePressed) {
      rtc->m_targetVel.data.vx = 0;
      rtc->m_targetVel.data.vy = 0;
      rtc->m_targetVel.data.va = 0;
      
      control = true;
    }
    break;
  case CV_EVENT_LBUTTONDOWN:
    if(isInBtn(rtc->m_UpBtn, x, y, rtc->m_ButtonRadius)) {
      rtc->m_MousePressed = true;
      rtc->m_targetVel.data.vx = 0.1;
      rtc->m_targetVel.data.vy = 0;
      rtc->m_targetVel.data.va = 0;
      control = true;
    } else if(isInBtn(rtc->m_DownBtn, x, y, rtc->m_ButtonRadius)) {
      rtc->m_MousePressed = true;
      rtc->m_targetVel.data.vx = -0.1;
      rtc->m_targetVel.data.vy = 0;
      rtc->m_targetVel.data.va = 0;
      control = true;
    } else if(isInBtn(rtc->m_RightBtn, x, y, rtc->m_ButtonRadius)) {
      rtc->m_MousePressed = true;
      rtc->m_targetVel.data.vx = 0;
      rtc->m_targetVel.data.vy = 0;
      rtc->m_targetVel.data.va = -0.2;
      control = true;
    } else if(isInBtn(rtc->m_LeftBtn, x, y, rtc->m_ButtonRadius)) {
      rtc->m_MousePressed = true;
      rtc->m_targetVel.data.vx = 0;
      rtc->m_targetVel.data.vy = 0;
      rtc->m_targetVel.data.va = 0.2;
      control = true;
    } else if(isInBtn(rtc->m_CleanBtn, x, y, rtc->m_ButtonRadius)) {
      std::cout << "Clean" << std::endl;
      rtc->m_serviceName.data = "clean";
      service = true;
    } else if(isInBtn(rtc->m_DockBtn, x, y, rtc->m_ButtonRadius)) {
      std::cout << "Dock" << std::endl;
      rtc->m_serviceName.data = "dock";
      service = true;
    }else {
      rtc->m_targetVel.data.vx = 0;
      rtc->m_targetVel.data.vy = 0;
      rtc->m_targetVel.data.va = 0;
      control = true;
    }
    break;
  case CV_EVENT_MOUSEMOVE:
    if(rtc->m_MousePressed) {
      if(!isInBtn(rtc->m_UpBtn, x, y, rtc->m_ButtonRadius) && 
	 !isInBtn(rtc->m_RightBtn, x, y, rtc->m_ButtonRadius) &&
	 !isInBtn(rtc->m_LeftBtn, x, y, rtc->m_ButtonRadius) &&
	 !isInBtn(rtc->m_DownBtn, x, y, rtc->m_ButtonRadius) ) {
	rtc->m_MousePressed = false;
	rtc->m_targetVel.data.vx = 0;
	rtc->m_targetVel.data.vy = 0;
	rtc->m_targetVel.data.va = 0;
	control = true;
      } 
    }
    rtc->m_targetVel.data.vx = 0;
    rtc->m_targetVel.data.vy = 0;
    rtc->m_targetVel.data.va = 0;
    break;
  default:
    break;
  }


  if(control) {
    rtc->m_targetVelOut.write();
  } 
  if(service){
    rtc->m_serviceNameOut.write();
  }
}

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


