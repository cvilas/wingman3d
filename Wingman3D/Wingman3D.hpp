//========================================================================
// Wingman3D.hpp    : Logitech Wingman HID Joystick Interface 
//                    Version 1.0 
//
// Author           : Vilas Kumar Chitrakaran <cvilas@ces.clemson.edu>     
// Date             : July 2004
// Compiler         : GNU GCC 2.95.3qnx-nto
// Operating System : QNX Momentics 6.2.1                                
//========================================================================

#ifndef _WINGMAN3D_HPP_INCLUDED
#define _WINGMAN3D_HPP_INCLUDED

#include <sys/hiddi.h>


//========================================================================
/*! \struct wingman_data
    \brief  A structure for holding wingman joystick data */
//========================================================================
typedef struct wingman_data
{
 bool statusOk;           //!< 'true' if device status is OK.
 char statusMessage[80];  //!< Char buffer to hold device status message.
 int x;                   //!< X axis data.
 int y;                   //!< Y axis data.
 int twist;               //!< Z axis data.
 int buttons;             //!< Button status data.
 int slider;              //!< Slider axis data.
 int hatSwitch;           //!< Hat switch position data.
}wingman_data_t; 

//========================================================================
/*! \struct wingman_report  
    \brief  A structure for device reports (for driver internal use) */
//========================================================================
typedef struct wingman_report
{
 struct hidd_report_instance   *creport_instance;
 struct hidd_report            *creport;
 _uint16	                       *cbtnbuf; // current button buffer
 _uint32	                   dev_no;       // device number
}wingman_report_t;
  

//========================================================================
/*! \struct wingman_device 
    \brief  A structure for wingman devices (for driver internal use) */
//========================================================================
typedef struct wingman_device
{
 wingman_data_t          data;             //!< device current data.
 wingman_data_t          max_data;         //!< max limits on data fields.
 wingman_report_t        *report;          
 hidd_device_instance_t  *device_instance;
}wingman_device_t;
  
  
//========================================================================
// class Wingman3D
// -----------------------------------------------------------------------
// \brief
// The class \c Wingman3D provides an interface to the following Logitech 
// (Vendor ID: 0x046D) USB HID compliant joysticks:
// <ul>
// <li> Wingman Extreme 3D Digital (Product ID: 0xC212) 
// <li> Force3D (Product ID: 0xC283) (no force feedback support)
// <li> Extreme 3D Pro (Product ID: 0xc215)
// </ul>
//
// To use the joystick please ensure that the USB manager (devu-uhci or 
// devu-ohci) and manager for HID devices (io-hid) are running. Your 
// application will also need to be linked with the HID library 
// libhiddi.so and the library created by this class. You will 
// need to be root to use this class. Here are the commands to start the 
// device managers (as root) :
// \code
// /sbin/devu-uhci & (or /sbin/devu-ohci &)
// /sbin/io-hid &
// mount -Tio-hid devh-usb.so &
// \endcode
//
// <b>Example Program:</b>
// \include Wingman3D.t.cpp
//========================================================================

class Wingman3D
{
 public:
  Wingman3D(int devNum = 0);
   // The default constructor. Establishes connection with the HID driver.
   //  devNum  The address of the joystick device to connect to (default = 0).
   
  ~Wingman3D();
   // The default destructor disconnects from the HID driver and cleans up.
   
  int getX() const;
   //  return  The X position
  
  int getMaxX() const;
   //  return  The maximum output from X axis.
   
  int getY() const;
   //  return  The Y position

  int getMaxY() const;
   //  return  The maximum output from Y axis.

  int getTwist() const;
   //  return  The twist handle position. 
  
  int getMaxTwist() const;
   //  return  The maximum output from twist axis.
  
  int getNumButtons() const;
   //  return  The number of programmable buttons available
    
  bool isButtonPressed(int button) const;
   //  return  true if button \a button is pressed, else false 

  int getSliderValue() const;
   //  return  The slider/throttle position. Range [0 - 255].
  
  int getMaxSliderValue() const;
   //  return  The maximum output from slider.

  int getHatSwitchStatus() const;
   //  return  A value indicating position of the hat switch. Range [1 - 8].

  bool isStatusOk() const;
   //  return  true if no error, else false.
   
  char *getStatusMessage() const;
   //  return  A string carrying current status of the device. This string 
   //          also carries error messages when isStatusOk() returns false.
   
  void printDeviceInfo(int verbosity = 1) const;
   // Print information about the device to stdout. Higher values of 
   // \a verbosity mean more detailed information.

 protected:
  wingman_data_t getWingmanData() const;
   //  return  The whole data structure for device
   //          with current joystick data
   
  wingman_data_t getWingmanMaxData() const;
   //  return  The whole data structure for device
   //          with max limits for joystick data
    
 private:
  int d_devNum; 
   // The device number
  
  struct hidd_connection *d_connection;
   // connection handle to HID server

  // ----- HID server connection functions ------ 
  static wingman_device_t s_joystick[2];
  static void onInsertion(struct hidd_connection *connection, hidd_device_instance_t *instance);
  static void onRemoval(struct hidd_connection *connection, hidd_device_instance_t *instance);
  static void onHidReport(struct hidd_connection *connection, struct hidd_report *handle, 
                          void *report_data, _uint32 report_len, _uint32 flags, void *user);
};


#endif // #ifndef _WINGMAN3D_HPP_INCLUDED
