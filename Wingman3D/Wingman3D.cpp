//========================================================================
// Wingman3D.cpp    : Logitech Wingman HID Joystick Interface
//                    Version 1.0    
//
// Author           : Vilas Kumar Chitrakaran <cvilas@ces.clemson.edu>     
// Date             : July 2004
// Compiler         : GNU GCC 2.95.3qnx-nto
// Operating System : QNX Momentics 6.2.1                                
//========================================================================

#include "Wingman3D.hpp"

#include <sys/hidut.h>
#include <unistd.h>
#include <iostream.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define HID_VENDOR_LOGITECH             0x046D
#define HID_PRODUCT_LOGITECH_WME3D      0xc212
#define HID_PRODUCT_LOGITECH_FRC3D		0xc283
#define HID_PRODUCT_LOGITECH_EXT3DPRO	0xc215

wingman_device_t Wingman3D::s_joystick[2];

//========================================================================
// Wingman3D::Wingman3D
//========================================================================
Wingman3D::Wingman3D(int devNum)
{
 if(devNum < 0 || devNum > 1)
 {
  s_joystick[d_devNum].data.statusOk = false;
  strncpy(s_joystick[d_devNum].data.statusMessage, "Wingman3D : Invalied device number", 80);
  return;
 }
 d_devNum = devNum;
 s_joystick[d_devNum].data.statusOk = true;
 strncpy(s_joystick[d_devNum].data.statusMessage, "Wingman3D : not initialized", 80);
 d_connection = NULL;
 s_joystick[d_devNum].device_instance = NULL;

 if (geteuid() != 0)
 {
  s_joystick[d_devNum].data.statusOk = false;
  strncpy(s_joystick[d_devNum].data.statusMessage, 
          "Wingman3D : Requires superuser privileges", 80);
  return;
 }

 
 hidd_device_ident_t interest = {HID_VENDOR_LOGITECH,
                           HIDD_CONNECT_WILDCARD /* HID_PRODUCT_LOGITECH_WME3D */,
                           (_uint32)HIDD_CONNECT_WILDCARD /* HID_VERSION */};
 
 hidd_funcs_t funcs = {_HIDDI_NFUNCS,
                       onInsertion,
                       onRemoval,
                       onHidReport,
                       NULL};
 
 hidd_connect_parm_t parm = {NULL, 
                             HID_VERSION,
                             HIDD_VERSION,
                             0,
                             0,
                             &interest,
                             &funcs,
                             HIDD_CONNECT_WAIT};
 
 
 if ( hidd_connect(&parm, &d_connection) != EOK )
 {
  s_joystick[d_devNum].data.statusOk = false;
  strncpy(s_joystick[d_devNum].data.statusMessage, 
          "Wingman3D : Connection with HID driver failed: ", 80);
  strncat(s_joystick[d_devNum].data.statusMessage, strerror(errno), 
          80 - strlen(s_joystick[d_devNum].data.statusMessage));
  return;
 }
 strncpy(s_joystick[d_devNum].data.statusMessage, 
         "Wingman3D : Waiting for Joystick", 80);
}


//========================================================================
// Wingman3D::~Wingman3D
//========================================================================
Wingman3D::~Wingman3D()
{
 if(d_connection)
  hidd_disconnect( d_connection );
}


//========================================================================
// Wingman3D::getX()
//========================================================================
int Wingman3D::getX() const
{
 return s_joystick[d_devNum].data.x;
}


//========================================================================
// Wingman3D::getMaxX()
//========================================================================
int Wingman3D::getMaxX() const
{
 return s_joystick[d_devNum].max_data.x;
}


//========================================================================
// Wingman3D::getY()
//========================================================================
int Wingman3D::getY() const
{
 return s_joystick[d_devNum].data.y;
}


//========================================================================
// Wingman3D::getMaxY()
//========================================================================
int Wingman3D::getMaxY() const
{
 return s_joystick[d_devNum].max_data.y;
}


//========================================================================
// Wingman3D::getTwist()
//========================================================================
int Wingman3D::getTwist() const
{
 return s_joystick[d_devNum].data.twist;
}


//========================================================================
// Wingman3D::getMaxTwist()
//========================================================================
int Wingman3D::getMaxTwist() const
{
 return s_joystick[d_devNum].max_data.twist;
}


//========================================================================
// Wingman3D::isButtonPressed
//========================================================================
bool Wingman3D::isButtonPressed(int button) const
{
 if( button < 1 || button > s_joystick[d_devNum].max_data.buttons)
  return false;
  
 if( (1 << (button-1)) & s_joystick[d_devNum].data.buttons)
  return true;
 
 return false;
}


//========================================================================
// Wingman3D::getNumButtons()
//========================================================================
int Wingman3D::getNumButtons() const
{
 return s_joystick[d_devNum].max_data.buttons;
}


//========================================================================
// Wingman3D::getSliderValue()
//========================================================================
int Wingman3D::getSliderValue() const
{
 return s_joystick[d_devNum].data.slider;
}


//========================================================================
// Wingman3D::getMaxSliderValue()
//========================================================================
int Wingman3D::getMaxSliderValue() const
{
 return s_joystick[d_devNum].max_data.slider;
}


//========================================================================
// Wingman3D::getHatSwitchStatus
//========================================================================
int Wingman3D::getHatSwitchStatus() const
{
 if(s_joystick[d_devNum].data.hatSwitch == 0x8)
  return 0;
 return (s_joystick[d_devNum].data.hatSwitch+1);
}


//========================================================================
// Wingman3D::isStatusOk()
//========================================================================
bool Wingman3D::isStatusOk() const
{
 return s_joystick[d_devNum].data.statusOk;
}


//========================================================================
// Wingman3D::getStatusMessage()
//========================================================================
char * Wingman3D::getStatusMessage() const
{
 return (char *)s_joystick[d_devNum].data.statusMessage;
}


//========================================================================
// Wingman3D::printDeviceInfo
//========================================================================
void Wingman3D::printDeviceInfo(int verbosity) const
{
 hidd_device_instance_t *device_instance = 
                        s_joystick[d_devNum].device_instance;
 if(device_instance == NULL)
  return;
 
 char buffer[100];

 hidd_get_manufacturer_string(d_connection, device_instance, buffer, 100);
 cout << "Manufacturer     : " << buffer << endl;
 hidd_get_product_string(d_connection, device_instance, buffer, 100);
 cout << "Product          : " << buffer << endl;
 
 if(verbosity < 2) 
  return;
 
 cout << "Software Version : 1.01, August 2005" << endl;
 cout << "Status message   : " << s_joystick[d_devNum].data.statusMessage << endl;
}


//========================================================================
// Wingman3D::getWingmanData
//========================================================================
wingman_data_t Wingman3D::getWingmanData() const
{
 return s_joystick[d_devNum].data; 
}


//========================================================================
// Wingman3D::getWingmanMaxData
//========================================================================
wingman_data_t Wingman3D::getWingmanMaxData() const
{
 return s_joystick[d_devNum].max_data; 
}


//========================================================================
// Wingman3D::onInsertion
//========================================================================
void Wingman3D::onInsertion(struct hidd_connection *connection, 
                          hidd_device_instance_t *device_instance)
{
 struct hidd_collection **hidd_collections, **hidd_mcollections, **hidd_ncollections;
 struct hidd_report_instance *report_instance;
 struct hidd_report *report;
 wingman_report_t	 *jstk_report = NULL;
 _uint16	 num_col, num_mcol, num_ncol;
 _uint16 usage_page, usage;
 _uint16 max_but;
 int i;
 int dev_no;
 
 dev_no = device_instance->devno;
 
 if(dev_no < 0 || dev_no > 1)
  return;

 s_joystick[dev_no].device_instance = device_instance;
 
 // Get root level HID collections
 hidd_get_collections( device_instance, NULL, &hidd_collections, &num_col);
 
 // for each top level collection
 for(i = 0; i < num_col; i++)
 {
  // Get usage for the collection
  hidd_collection_usage( hidd_collections[i], &usage_page, &usage);
  
  // Ignore collection if it doesn't describe joystick functionality
  if( usage_page != HIDD_PAGE_DESKTOP || usage != HIDD_USAGE_JOYSTICK)
   continue;

  
  if( hidd_get_report_instance(hidd_collections[i], 0, HID_INPUT_REPORT, 
      &report_instance) == EOK )
  {
   hidd_num_buttons( report_instance, &max_but );
   
   if( hidd_report_attach( connection, device_instance, report_instance, 0, 
       sizeof(wingman_report_t) + (max_but * sizeof(_int32)) , &report )  == EOK ) 
   {
     jstk_report = (wingman_report_t *)hidd_report_extra( report );
     jstk_report->dev_no = dev_no;
     jstk_report->creport = report;
     jstk_report->creport_instance = report_instance;
     jstk_report->cbtnbuf = (_uint16 *) (jstk_report + 1); // setup pointer  to button data
     s_joystick[dev_no].report = jstk_report;
     s_joystick[dev_no].max_data.buttons = max_but;
     break;
   }
  }

  // *** The following is a bad hack. Fix it as recursive search for report ****
  hidd_get_collections( NULL, hidd_collections[i], &hidd_mcollections, &num_mcol);
   
   
  if ( num_col &&  hidd_get_report_instance( hidd_mcollections[0], 0 , HID_INPUT_REPORT, 
       &report_instance ) == EOK ) 
  {
   hidd_num_buttons( report_instance, &max_but );
  
   if( hidd_report_attach( connection, device_instance, report_instance, 0,  
       sizeof(wingman_report_t) + (max_but * sizeof(_int32)) , &report ) == EOK ) 
   {
    jstk_report = (wingman_report_t *)hidd_report_extra( report );
    jstk_report->dev_no = dev_no;
    jstk_report->creport = report;
    jstk_report->creport_instance = report_instance;
    jstk_report->cbtnbuf = (_uint16 *) (jstk_report + 1); // setup pointer  to button data
    s_joystick[dev_no].report = jstk_report;
    s_joystick[dev_no].max_data.buttons = max_but;
    break;
   }
  }

  hidd_get_collections( NULL, hidd_mcollections[i], &hidd_ncollections, &num_ncol);

  if ( num_mcol &&  hidd_get_report_instance( hidd_ncollections[0], 0 , HID_INPUT_REPORT, 
       &report_instance ) == EOK ) 
  {
   hidd_num_buttons( report_instance, &max_but );
   if( hidd_report_attach( connection, device_instance, report_instance, 0, 
       sizeof(wingman_report_t) + (max_but * sizeof(_int32)) , &report ) == EOK ) 
   {
    jstk_report = (wingman_report_t *)hidd_report_extra( report );
    jstk_report->dev_no = dev_no;
    jstk_report->creport = report;
    jstk_report->creport_instance = report_instance;
    jstk_report->cbtnbuf = (_uint16 *) (jstk_report + 1); // setup pointer  to button data
    s_joystick[dev_no].report = jstk_report;
    s_joystick[dev_no].max_data.buttons = max_but;
    break;
   }
  }
 } // end for

 // Get max values of data for each function
 _uint16 numprops;
 hidd_get_num_props(report_instance, &numprops);
 hidd_report_props_t *props = new hidd_report_props_t[numprops]; 
 hidd_get_report_props(report_instance, props, &numprops);

 for(int i = 0; i < numprops; i++)
 {
  if(props[i].usage_page == HIDD_PAGE_DESKTOP)
  {
   if(props[i].usage_min == HIDD_USAGE_X)
    s_joystick[dev_no].max_data.x = props[i].logical_max;

   if(props[i].usage_min == HIDD_USAGE_Y)
    s_joystick[dev_no].max_data.y = props[i].logical_max;

   if(props[i].usage_min == HIDD_USAGE_RZ)
    s_joystick[dev_no].max_data.twist = props[i].logical_max;

   if(props[i].usage_min == HIDD_USAGE_SLIDER)
    s_joystick[dev_no].max_data.slider = props[i].logical_max;
  }
  
  if(props[i].usage_page == HIDD_PAGE_BUTTONS)
  {
   s_joystick[dev_no].max_data.buttons += props[i].report_count;
  }
 }

 delete[](props);

 s_joystick[dev_no].data.statusOk = true;
 strncpy(s_joystick[dev_no].data.statusMessage, 
         "Wingman3D : Joystick present", 80);
}


//========================================================================
// Wingman3D::onRemoval
//========================================================================
void Wingman3D::onRemoval(struct hidd_connection *connection, 
                        hidd_device_instance_t *instance)
{
 if(instance->devno < 0 || instance->devno > 1)
  return;
 
 hidd_reports_detach( connection, instance);
 s_joystick[instance->devno].data.statusOk = true;
 strncpy(s_joystick[instance->devno].data.statusMessage, 
         "Wingman3D : Joystick unplugged", 80);
}



//========================================================================
// Wingman3D::onHidReport
//========================================================================
void Wingman3D::onHidReport(struct hidd_connection *connection, 
                          struct hidd_report *handle, void *report_data, 
                          _uint32 report_len, _uint32 flags, void *user)
{
 _uint32 xval, yval, zval, slider, hat;
 struct hidd_collection	*collection;
 struct hidd_report_instance   *report_instance;
 _uint16 num_press;

 wingman_report_t *report = (wingman_report_t *)user;
 int dev_no = report->dev_no;

 if(dev_no < 0 || dev_no > 1)
  return;

 report_instance = s_joystick[dev_no].report->creport_instance;
 hidd_report_collection( report_instance, &collection );
 hidd_get_usage_value( report_instance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_X, report_data, &xval);
 hidd_get_usage_value( report_instance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_Y, report_data, &yval);
 hidd_get_usage_value( report_instance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_RZ, report_data, &zval);
 hidd_get_usage_value( report_instance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_HAT_SWITCH, report_data, &hat);
 hidd_get_usage_value( report_instance, NULL, HIDD_PAGE_DESKTOP, HIDD_USAGE_SLIDER, report_data, &slider);

 num_press = s_joystick[dev_no].max_data.buttons;

 // Currently pressed buttons
 for ( int x = 0 ; x < num_press ; x++ )
  s_joystick[dev_no].report->cbtnbuf[x] = 0x0;

 hidd_get_buttons(report_instance, NULL, HIDD_PAGE_BUTTONS, report_data, 
                  s_joystick[dev_no].report->cbtnbuf, &num_press);

 s_joystick[dev_no].data.buttons = 0x00;
 for ( int x = 0 ; x < num_press ; x++ )
  s_joystick[dev_no].data.buttons = s_joystick[dev_no].data.buttons | 
                                    (1 << (s_joystick[dev_no].report->cbtnbuf[x] - 1));
 
 s_joystick[dev_no].data.x = xval & s_joystick[dev_no].max_data.x; 
 s_joystick[dev_no].data.y = yval & s_joystick[dev_no].max_data.y;
 s_joystick[dev_no].data.twist = zval & s_joystick[dev_no].max_data.twist;
 s_joystick[dev_no].data.hatSwitch = hat & 0xf;
 s_joystick[dev_no].data.slider = slider & s_joystick[dev_no].max_data.slider;
}

