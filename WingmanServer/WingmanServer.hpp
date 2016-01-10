//========================================================================
// WingmanServer.hpp    : Logitech Wingman HID Joystick Server 
//                        Version 1.0s 
//
// Author           : Vilas Kumar Chitrakaran <cvilas@ces.clemson.edu>     
// Date             : July 2004
// Compiler         : GNU GCC 2.95.3qnx-nto
// Operating System : QNX Momentics 6.2.1                                
//========================================================================

#ifndef _WINGMANSERVER_HPP_INCLUDED
#define _WINGMANSERVER_HPP_INCLUDED

#include <sys/hiddi.h>
#include "Server.hpp"
#include "SharedMemory.hpp"
#include "WingmanClient.hpp"


//========================================================================
// struct wingman_report : A structure for device reports
//========================================================================
typedef struct wingman_report
{
 struct hidd_report_instance   *creport_instance;
 struct hidd_report            *creport;
 _uint16	                       *cbtnbuf; // current button buffer
 _uint32	                   dev_no;       // device number
}wingman_report_t;
  

//========================================================================
// struct wingman_device : A structure for devices
//========================================================================
typedef struct wingman_device
{
 WingmanClient::WingmanData_t   *data;
 wingman_report_t               *report;
 hidd_device_instance_t         *device_instance;
}wingman_device_t;
  
  
//========================================================================
// class WingmanServer
// -----------------------------------------------------------------------
// \brief
// Server class interface for Logitech (Vendor ID: 0x046D) Wingman 
// Extreme 3D Digital (Product ID: 0xC212) and Force3D (Product ID: 0xC283) 
// USB HID compliant joysticks.
//
// To use the joystick please ensure that the USB manager (devu-uhci or 
// devu-ohci) and manager for HID devices (io-hid) are running. Here are 
// the commands to start the server with name "usbjs" (as root) :
// 
// \code
// /sbin/devu-uhci & (or /sbin/devu-ohci &)
// /sbin/io-hid &
// mount -Tio-hid devh-usb.so &
// WingmanServer -n qrts/usbjs &
// \endcode
// 
// After starting the server, activate any of the degrees of freedom of
// the joystick once so that data structures are initialized to proper 
// values.
//========================================================================

class WingmanServer : public Server
{
 public:
  WingmanServer(const char *serverName="qrts/usbjs");
   // The default constructor. Establishes connection with the HID driver.
   //  serverName  QNX name for the server
   
  ~WingmanServer();
   // The default destructor disconnects from the HID driver and cleans up.

  inline int isStatusOk() { return d_status.isStatusOk(); }
   //  return  true if status is ok, else false

  inline int isStatusError() { return !d_status.isStatusOk(); }
   //  return  true if error, else false
   
  void printDeviceInfo() const;
   // Print information about connected devices to stdout.

  virtual int serveMessage(pid_t pid, void *msgBuf);
   // Message handling function. Just prints received messages
   // but does not reply. 

 protected:
    
	// ========== END OF INTERFACE ==========

 private:
  WingmanClient::WingmanData_t *d_jsData;
   // Joystick data.
	
  SharedMemory d_shm;
   // Shared memory location for joystick data.

  struct hidd_connection *d_connection;
   // connection handle to HID server

  // ----- HID server connection functions ------ 
  static wingman_device_t s_joystick[2];
  static void onInsertion(struct hidd_connection *connection, hidd_device_instance_t *instance);
  static void onRemoval(struct hidd_connection *connection, hidd_device_instance_t *instance);
  static void onHidReport(struct hidd_connection *connection, struct hidd_report *handle, 
                          void *report_data, _uint32 report_len, _uint32 flags, void *user);
};


#endif // _WINGMANSERVER_HPP_INCLUDED
