  

#ifndef _WINGMANCLIENT_HPP_INCLUDED
#define _WINGMANCLIENT_HPP_INCLUDED

#include <sys/types.h>

#include "Client.hpp" 
#include "SharedMemory.hpp"

  
/*! \class WingmanClient
 \brief
 Connects to Logitech Wingman USB HID joystick server.

 <b>Example Program:</b>

 \include WingmanClient.t.cpp
 */
 
class WingmanClient : public Client
{
  public:
    WingmanClient(int devNum, char *serverName = "qrts/usbjs");
   /*!< The constructor. Connects to joystick server
  */
  
    ~WingmanClient();
   /*!< The destructor disconnects from the joystick server
 */
 
    bool isJoystickPresent();
   /*!<  \return  true if joystick is connected, else false.
   */
   
    int getX() const;
   /*!<  \return  The X position
  */
  
    int getMaxX() const;
   /*!<  \return  The maximum output from X axis.
   */
   
    int getY() const;
   /*!<  \return  The Y position
*/

    int getMaxY() const;
   /*!<  \return  The maximum output from Y axis.
*/

    int getTwist() const;
   /*!<  \return  The twist handle position. 
  */
  
    int getMaxTwist() const;
   /*!<  \return  The maximum output from twist axis.
  */
  
    int getNumButtons() const;
   /*!<  \return  The number of programmable buttons available
    */
    
    bool isButtonPressed(int button) const;
   /*!<  \return  true if button \a button is pressed, else false 
*/

    int getSliderValue() const;
   /*!<  \return  The slider/throttle position. Range [0 - 255].
  */
  
    int getMaxSliderValue() const;
   /*!<  \return  The maximum output from slider.
*/

    int getHatSwitchStatus() const;
   /*!<  \return  A value indicating position of the hat switch. Range [1 - 8].
  */
  
    bool isStatusOk();
   /*!<  \return  true if joystick/client status is ok, else false.
   */
   
    const char *getStatusMessage();
   /*!<  \return  A string carrying current status of the device. This string 
             also carries error messages when isStatusOk() returns false.
*/

};
#endif // _WINGMANCLIENT_HPP_INCLUDED
