//========================================================================
// WingmanServer.cpp    : Logitech Wingman HID Joystick Server
//                        Version 1.0    
//
// Author           : Vilas Kumar Chitrakaran <cvilas@ces.clemson.edu>     
// Date             : July 2004
// Compiler         : GNU GCC 2.95.3qnx-nto
// Operating System : QNX Momentics 6.2.1                                
//========================================================================

#include "WingmanServer.hpp"

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

wingman_device_t WingmanServer::s_joystick[2];

//========================================================================
// WingmanServer::WingmanServer
//========================================================================
WingmanServer::WingmanServer(const char *serverName) 
 : Server((char *)serverName)
{
 char shmName[256];

 d_connection = NULL;

 if(d_status.isStatusError() )
  return;

 if (geteuid() != 0)
 {
  d_status.setStatusError()
   << "WingmanServer: This program requires superuser privileges." << endl;
  return;
 }
 
 // Servers run at priority 50
 setPriority(50);
 setSchedulingPolicy(SCHED_FIFO);

 // Create shared memory for wingman data
 sprintf(shmName, "QRTS_WingmanServer_%s", getServerName());
 d_jsData = (WingmanClient::WingmanData_t *) d_shm.create(shmName, 2 * sizeof(WingmanClient::WingmanData_t));
 if(d_shm.d_status.isStatusError())
 {
  d_status.setStatusError()
   << "WingmanServer: Cannot create shared memory." 
   << d_shm.d_status.getMessageText() << endl;
  return;
 }

 for(int i = 0; i < 2; i++) // for 2 devices
 {
  // Initialize shared data structures
  s_joystick[i].device_instance = NULL;
  s_joystick[i].data = &(d_jsData[i]);
  s_joystick[i].data->status_ok = true;
  strncpy(s_joystick[i].data->status_message, 
  "WingmanServer : device not initialized", 80);
  s_joystick[i].data->jstk_is_present = false;
 }
 
 hidd_device_ident_t interest = {HID_VENDOR_LOGITECH,
                           HIDD_CONNECT_WILDCARD /* HID_PRODUCT_LOGITECH_EXT3DPRO */,
                           HIDD_CONNECT_WILDCARD /* HID_VERSION*/ };
 
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
  d_status.setStatusError()
   << "WingmanServer: Connection with HID driver failed: " 
   << strerror(errno) << endl;
  return;
 }

 d_status.setStatusOk();
}


//========================================================================
// WingmanServer::~WingmanServer
//========================================================================
WingmanServer::~WingmanServer()
{
 if(d_connection)
  hidd_disconnect( d_connection );

 d_shm.unlink();
}


//========================================================================
// WingmanServer::printDeviceInfo
//========================================================================
void WingmanServer::printDeviceInfo() const
{
 hidd_device_instance_t *device_instance; 
 
 char buffer[100];

 cout << "WingmanServer Version : 1.0s, Aug 2004" << endl;

 for(int i = 0; i < 2; i++)
 {
  device_instance = s_joystick[i].device_instance;
  if(device_instance == NULL)
   continue;
  
  cout << "Device Number    : " << i << endl;
  hidd_get_manufacturer_string(d_connection, device_instance, buffer, 100);
  cout << "Manufacturer     : " << buffer << endl;
  hidd_get_product_string(d_connection, device_instance, buffer, 100);
  cout << "Product          : " << buffer << endl;
  cout << "Status message   : " << s_joystick[i].data->status_message 
       << endl << endl;
 }
}


//========================================================================
// WingmanServer::serveMessage
//========================================================================
int WingmanServer::serveMessage(pid_t pid, void *msgBuf)
{
 int msg = * (int *) msgBuf;
 cout << "WingmanServer : Message " << msg << " received from pid " << pid << endl;
 return 0;
}


//========================================================================
// WingmanServer::onInsertion
//========================================================================
void WingmanServer::onInsertion(struct hidd_connection *connection, 
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
     s_joystick[dev_no].data->num_buttons = max_but;
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
    s_joystick[dev_no].data->num_buttons = max_but;
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
    s_joystick[dev_no].data->num_buttons = max_but;
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
    s_joystick[dev_no].data->max_x = props[i].logical_max;

   if(props[i].usage_min == HIDD_USAGE_Y)
    s_joystick[dev_no].data->max_y = props[i].logical_max;

   if(props[i].usage_min == HIDD_USAGE_RZ)
    s_joystick[dev_no].data->max_twist = props[i].logical_max;

   if(props[i].usage_min == HIDD_USAGE_SLIDER)
    s_joystick[dev_no].data->max_slider = props[i].logical_max;
  }
  
  if(props[i].usage_page == HIDD_PAGE_BUTTONS)
   s_joystick[dev_no].data->num_buttons += props[i].report_count;
 }

 delete[](props);
 
 s_joystick[dev_no].data->status_ok = true;
 strncpy(s_joystick[dev_no].data->status_message, 
         "WingmanServer : Joystick detected", 80);

 s_joystick[dev_no].data->jstk_is_present = true;
}


//========================================================================
// WingmanServer::onRemoval
//========================================================================
void WingmanServer::onRemoval(struct hidd_connection *connection, 
                        hidd_device_instance_t *instance)
{
 if(instance->devno < 0 || instance->devno > 1)
  return;
 
 hidd_reports_detach( connection, instance);
 s_joystick[instance->devno].data->status_ok = true;
 strncpy(s_joystick[instance->devno].data->status_message, 
         "WingmanServer : Joystick unplugged", 80);
 s_joystick[instance->devno].data->jstk_is_present = false;
}



//========================================================================
// WingmanServer::onHidReport
//========================================================================
void WingmanServer::onHidReport(struct hidd_connection *connection, 
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

 num_press = s_joystick[dev_no].data->num_buttons;

 // Currently pressed buttons
 for ( int x = 0 ; x < num_press ; x++ )
  s_joystick[dev_no].report->cbtnbuf[x] = 0x0;

 hidd_get_buttons(report_instance, NULL, HIDD_PAGE_BUTTONS, report_data, 
                  s_joystick[dev_no].report->cbtnbuf, &num_press);

 s_joystick[dev_no].data->buttons = 0x00;
 for ( int x = 0 ; x < num_press ; x++ )
  s_joystick[dev_no].data->buttons = s_joystick[dev_no].data->buttons | 
                                    (1 << (s_joystick[dev_no].report->cbtnbuf[x] - 1));
 
 s_joystick[dev_no].data->x = xval & s_joystick[dev_no].data->max_x; 
 s_joystick[dev_no].data->y = yval & s_joystick[dev_no].data->max_y;
 s_joystick[dev_no].data->twist = zval & s_joystick[dev_no].data->max_twist;
 s_joystick[dev_no].data->hat_switch = hat & 0xf;
 s_joystick[dev_no].data->slider = slider & s_joystick[dev_no].data->max_slider;
}

