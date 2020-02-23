/***********************************************************************
*! \file:                   glb_varL.h
*  \projekt:                globale Variablen
*  \created on:             07.03.2019
*  \author:                 R. Gräber
*  \version:                0
*  \history:                -
*  \brief
***********************************************************************/
 
#ifndef _glb_var_H_
#define _glb_var_H_
 
/***********************************************************************
 * Includes
 **********************************************************************/
 
/***********************************************************************
 * Informations
 **********************************************************************/
//https://www.dyclassroom.com/c/c-pointers-and-two-dimensional-array
 
/***********************************************************************
 * Declarations
 **********************************************************************/
 
/***********************************************************************
 * Global Variable
 **********************************************************************/
 
/***********************************************************************
 * Constant
 **********************************************************************/
 
/***********************************************************************
 * Macros
 **********************************************************************/
 
/***********************************************************************
 * Funtions
 **********************************************************************/

class glb_var
{
private:
    /* data */
public:

    //functions
    glb_var(/* args */);
    ~glb_var();
    //variablen
    uint8_t wait_for_wifi_connect_1000ms;
    bool wifi_is_connected;
};

glb_var::glb_var(/* args */)
{
    wait_for_wifi_connect_1000ms = 10;
    wifi_is_connected = false;
}

glb_var::~glb_var()
{
}

 

 
#endif /* _glb_var_H_ */
