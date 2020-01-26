/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#include "PSoCEve.h"
//#include "PSoCEve_List.h"
#include "PSoCEve_Hal.h"
//#include "SST25VF032B.h"

//enum LISTTYPE transferinprogress = NONE;
uint32 ramPtr;
uint16 ramCMDOffset;

TRANSFERTYPE transferinprogress = NONE;

#if defined EVE_FT800
    uint8 vertexFormat = VERTEX_FORMAT_1_16;
#endif    

/*******************************************************************************
*   Macros.
*******************************************************************************/

/* Macro name : CheckCMDOffset
   Description: Control roll-over to command list ram in FT chip.
*/
#define CheckCMDOffset() if (ramCMDOffset > 4095) { ramCMDOffset = (ramCMDOffset - 4096); }


/*******************************************************************************
*   Function prototypes.
*******************************************************************************/
uint8 FTIsCoproReady(uint16 *newoffset);

void DLListNewItem(uint32 item);
void CMDListNewItem(uint8 *tobesent, uint8 length, unsigned char *string);

extern void DLBitmapSource(uint32 address);
extern void DLClearColorRGB(uint8 red, uint8 green, uint8 blue);    
extern void DLTag(uint8 tag);
extern void DLColorRGB(uint8 red, uint8 green, uint8 blue);
extern void DLBitmapHandle(uint8 handle);
extern void DLCell(uint8 cell);
extern void DLBitmapLayout(uint8 format, uint16 linestride, uint16 height);
extern void DLBitmapSize(uint8 filter, uint8 wrapx, uint8 wrapy, uint16 width, uint16 height);
extern void DLAlphaFunc(uint8 func, uint8 ref);
extern void DLStencilFunc(uint8 func, uint8 ref, uint8 mask);
extern void DLBlendFunc(uint8 src, uint8 dst);
extern void DLStencilOp(uint8 sfail, uint8 spass);
extern void DLPointSize(uint32 size);
extern void DLLineWidth(uint16 width);
extern void DLColorA(uint8 alpha);
extern void DLClearStencil(uint8 s);
extern void DLClearTag(uint8 t);
extern void DLStencilMask(uint8 mask);
extern void DLTagMask(uint8 mask);
extern void DLBitmapTransformA(uint32 a);
extern void DLBitmapTransformB(uint32 b);
extern void DLBitmapTransformC(uint32 c);
extern void DLBitmapTransformD(uint32 d);
extern void DLBitmapTransformE(uint32 e);
extern void DLBitmapTransformF(uint32 f);
extern void DLScissorXY(uint16 x, uint16 y);
extern void DLJump(uint16 address);
extern void DLBegin(uint8 primitive);
extern void DLCall(uint16 address);
extern void DLColorMask(uint8 r, uint8 g, uint8 b, uint8 a);
extern void DLEnd();
extern void DLSaveContext();
extern void DLRestoreContext();
extern void DLReturn();
extern void DLMacro(uint8 macro);
extern void DLClear(uint8 color, uint8 stencil, uint8 tag);
extern void DLVertex2F(int16 x, int16 y);
extern void DLVertex2II(uint16 x, uint16 y, uint8 handle, uint8 cell);

extern void CMDDLStart();
extern void CMDSwap();
extern void CMDInterrupt(uint32 milliseconds);
extern void CMDBgcolor(int8 red, int8 green, int8 blue);
extern void CMDFgcolor(int8 red, int8 green, int8 blue);
extern void CMDGradient(int16 x0, int16 y0, int8 red0, int8 green0, int8 blue0, int16 x1, int16 y1, int8 red1, int8 green1, int8 blue1);
extern void CMDText(int16 x, int16 y, int16 font, int16 options, char* text);
extern void CMDButton(int16 x, int16 y, int16 width, int16 height, int16 font, int16 options, char* text);
extern void CMDKeys(int16 x, int16 y, int16 width, int16 height, int16 font, int16 options, char* text);
extern void CMDProgressBar(int16 x, int16 y, int16 width, int16 height, int16 options, int16 value, int16 range);
extern void CMDSlider(int16 x, int16 y, int16 width, int16 height, int16 options, int16 value, int16 range);
extern void CMDScrollBar(int16 x, int16 y, int16 width, int16 height, int16 options, int16 value, int16 size, int16 range);
extern void CMDToggle(int16 x, int16 y, int16 width, int16 font, int16 options, int16 state, char* text);
extern void CMDGauge(int16 x, int16 y, int16 radius, int16 options, int16 major, int16 minor, int16 value, int16 range);
extern void CMDClock(int16 x, int16 y, int16 radius, int16 options, int16 hours, int16 minutes, int16 seconds, int16 milliseconds);
extern void CMDCalibrate();
extern void CMDSpinner(int16 x, int16 y, int16 style, int16 scale);
extern void CMDStop();
extern void CMDMemSet(int32 ptr, int32 value, int32 size);
extern void CMDMemZero(int32 ptr, int32 size);
extern void CMDMemCopy(int32 dest, int32 src, int32 size);
extern void CMDAppend(int32 ptr, int32 size);
extern void CMDSnapshot(int32 ptr);
extern void CMDInflate(int32 ptr);
extern void CMDLoadIdentity();
extern void CMDTranslate(int32 tx, int32 ty);
extern void CMDScale(int32 x, int32 y);
extern void CMDRotate(int32 angle);
extern void CMDSetmatrix();
extern void CMDSetfont(int32 font, int32 ptr);
extern void CMDTrack(int16 x, int16 y, int16 width, int16 height, int16 tag);
extern void CMDDial(int16 x, int16 y, int16 radius, int16 options, int16 value);
extern void CMDNumber(int16 x, int16 y, int16 font, int16 options, int32 number);
extern void CMDScreenSaver();
extern void CMDSketch(int16 x, int16 y, int16 width, int16 height, int32 ptr, int16 format);
extern void CMDLogo();
extern void CMDColdstart();
extern void CMDGradcolor(int8 red, int8 green, int8 blue);


/*******************************************************************************
* Function Name: FTIsCoproccesorReady
********************************************************************************
*
* Summary:
*  Wrapper to FTIsCoproReady function.
*  Used with commands like CMD_CALIBRATE to know when command work have finished.
*
* Parameters:
*  none
*
* Return:
*  1, if coprocessor have finished proccesing commands list.
*  0, if coprocessor is busy.
*
*******************************************************************************/
uint8 FTIsCoproccesorReady()
{
    return FTIsCoproReady(&ramCMDOffset);
}

/*******************************************************************************
* Function Name: FTIsCoproReady
********************************************************************************
*
* Summary:
*  Check if coproccesor is ready to proccess a new commands list.
*
* Parameters:
*  newoffset:   pointer to variable to store offset to command list.
*
* Return:
*  1, if coproccesor is ready.
*  0, if coproccesor is busy proccessing a commands list.
*
*******************************************************************************/
uint8 FTIsCoproReady(uint16 *newoffset)
{
    unsigned int cmdbufferrd, cmdbufferwr;

    cmdbufferrd = FT_Register_Read(REG_CMD_READ);		    // Read the graphics processor read pointer.
	cmdbufferwr = FT_Register_Read(REG_CMD_WRITE); 	        // Read the graphics processor write pointer.
    
    if (cmdbufferrd != cmdbufferwr) return 0;               // If both are equal, processor have finished
    else                                                    //    processing previous list.
    {
        *newoffset = cmdbufferwr;
        return 1;                  
    }
}

uint16 FTGetCMDFifoFreeSpace()
{
    #if defined EVE_FT800
        unsigned int cmdbufferrd, cmdbufferwr;
        
        cmdbufferrd = FT_Register_Read(REG_CMD_READ);       // Read the graphics processor read pointer.
    	cmdbufferwr = FT_Register_Read(REG_CMD_WRITE); 	    // Read the graphics processor write pointer.
        
        return (4096 - 4) - ((cmdbufferwr - cmdbufferrd) & 4095);
    #elif defined EVE_FT810
        return FT_Register_Read(REG_CMDB_SPACE);
    #endif
}

/*******************************************************************************
* Function Name: FT_Write_ByteArray_4
********************************************************************************
*
* Summary:
*  Send an array of bytes to FT chip.
*  Used when bytes to be sent have to be multiple of four but possibly they 
*  are not.
*  This functions, if the array to be send is not multiple of 4; sends 0 bytes
*  until it is multiple.
*
* Parameters:
*  data:    pointer to array of bytes to be sent.
*  length:  length of the array.
*
* Return:
*  none
*
*******************************************************************************/
void FT_Write_ByteArray_4(const uint8 *data, uint32 length)
{
    FT_Send_ByteArray(data, length);
    ramCMDOffset += length; CheckCMDOffset();           // Manage offset to FT RAM.
    
    while ((ramCMDOffset % 4) != 0)
    {
        FT_Send_Byte(0);
        ramCMDOffset++; CheckCMDOffset();               // Manage offset to FT RAM.
    }
}

/*******************************************************************************
* Function Name: FT_ListStart
********************************************************************************
*
* Summary:
*  Starts a new list. 
*  Can be a DISPLAY list or a COPROCCESSOR list.
*
* Parameters:
*  listtype:    list to start, DISPLAY or COPROCCESOR
*
* Return:
*  Type of error:   OK, if it is ok.
*                   LIST_IN_PROGRESS, if a list has been started previouslly and
*                                     not finished.
*                   UNKNOWN_LIST_TYPE, if value of 'listtype' is unknown.
*
*******************************************************************************/
FTERROR FT_ListStart(TRANSFERTYPE transfertype)
{
    if (transferinprogress != NONE) return LIST_IN_PROGRESS;
    
    /* Wait until the coproccesor is ready. It have finished proccesing 
    previous commands. */
    
    while (!FTIsCoproReady(&ramCMDOffset)) {}
        
    #if defined EVE_FT800
        FT_Transfer_Start((RAM_CMD + ramCMDOffset) | MEMORY_WRITE);    // Start the display list
    #elif defined EVE_FT810
        FT_Transfer_Start(REG_CMDB_WRITE | MEMORY_WRITE);
    #endif
    
    if (transfertype == DLIST)
    {
        FT_Send_UINT32(CMD_DLSTART);
        
        #if defined EVE_FT800
            ramCMDOffset += 4; CheckCMDOffset();        // Manage offset to FT RAM.
        #endif
        
        transferinprogress = DLIST;
    } 
    else if (transfertype == DATA)
    {
        transferinprogress = DATA;
    }
    
    return OK;
}

/*******************************************************************************
* Function Name: FT_ListEnd
********************************************************************************
*
* Summary:
*  Finish a list that is in progress.
*
* Parameters:
*  swap:    if = 1, then sends command CMD_SWAP.
*           if = 0, do not send command CMD_SWAP.
*      Sending CMD_SWAP is the usual way of finishing a coproccesor list, but 
*  sometimes we can not send CMD_SWAP command because the command sent 
*  previously to FT chip can not work properly (ex: CMD_SPINNER).
*  This has no effect if the list in progress is a DISPLAY list.
*
* Return:
*  Type of error:   OK, there is no error.
*                   LIST_NOT_IN_PROGRESS, trying to finish a list that has not
*                                         been started.
*******************************************************************************/
FTERROR FT_ListEnd(SWAPACTION swap)
{
    if (transferinprogress == NONE) return LIST_NOT_IN_PROGRESS;
    
    if (swap == END_DL_SWAP)
    {
        #if defined EVE_FT800
            FT_Send_UINT32(DL_DISPLAY); 
            ramCMDOffset += 4; CheckCMDOffset();        // Manage offset to FT RAM.
            FT_Send_UINT32(CMD_SWAP); 
            ramCMDOffset += 4; CheckCMDOffset();        // Manage offset to FT RAM.

        #elif defined EVE_FT810          
            FT_Send_UINT32(DL_DISPLAY);
            FT_Send_UINT32(CMD_SWAP);
        #endif
    }
    
    FT_Transfer_End();
    
    #if defined EVE_FT800
        FT_Register_Write(REG_CMD_WRITE, ramCMDOffset);
    #endif

    transferinprogress = NONE;
    
    return OK;
}

/*******************************************************************************
* Function Name: DLListNewItem
********************************************************************************
*
* Summary:
*  Put new item (new command) inside current display list.
*  If current list is a DISPLAY list,  pointer to FT ram is not incremented in 
*  software. It is incremnted inside FT chip.
*  If current list is a COPROCCESOR list, it takes care of pointer offset.
*
* Parameters:
*  item:    command to be inserted in the list.
*
* Return:
*  none
*
*******************************************************************************/
void DLListNewItem(uint32 item)
{
    #if defined EVE_FT800
        FT_Send_ByteArray((uint8*)&item, 4);
        ramCMDOffset += 4; CheckCMDOffset();
    #elif defined EVE_FT810
        FT_Send_ByteArray((uint8*)&item, 4);        
    #endif
}

/*******************************************************************************
* Function Name: CMDListNewItem
********************************************************************************
*
* Summary:
*  Put new item inside current coproccesor list.
*
* Parameters:
*  tobesent:    pointer to the item to be sent to FT chip.
*  length:      length (in bytes) to be sent.
*  string:      for commands with strings (like CMD_BUTTON or CMD_TEXT) this is
*               a pointer to the string. For command that don´t use strings this
*               have to take value 0.
*
* Return:
*  none
*
*******************************************************************************/
void CMDListNewItem(uint8 *tobesent, uint8 length, unsigned char *string)
{
    unsigned char *cptr = string;
    
    #if defined EVE_FT810
        unsigned char charcounter = 0;
    #endif    
    
    #if defined EVE_FT800
        FT_Send_ByteArray(tobesent, length);                // Send all command parameters. 
        ramCMDOffset += length; CheckCMDOffset();           // Manage offset to FT RAM. 
        
        // Send the string if pointer to string is not zero.
        if (string != 0)                            
        {
            // Last byte of the string to be sent have to be equal to 0.
            // So, send bytes until we find byte = 0. 
            while (*cptr != 0)
            {
                FT_Send_Byte(*cptr);
                ramCMDOffset++;  CheckCMDOffset();          // Manage offset to FT RAM.
                cptr++;
            } 
        
            // Send last byte (byte = 0) to FT chip.
            FT_Send_Byte(0);
            ramCMDOffset++; CheckCMDOffset();               // Manage offset to FT RAM.
        
            // Every time we send a string to FT chip, its lenght have be multiple of 4.
            // If the length of the string is shorter than that, the we send 0 bytes until it is
            // a multiple of 4.
            while ((ramCMDOffset % 4) != 0)
            {
                FT_Send_Byte(0);
                ramCMDOffset++; CheckCMDOffset();           // Manage offset to FT RAM.
            }
        }
    #elif defined EVE_FT810
    
        FT_Send_ByteArray(tobesent, length);
        
        // Send the string if pointer to string is not zero.
        if (string != 0)                            
        {
            // Last byte of the string to be sent have to be equal to 0.
            // So, send bytes until we find byte = 0. 
            while (*cptr != 0)
            {
                FT_Send_Byte(*cptr);
                charcounter++;
                cptr++;
            } 
        
            // Send last byte (byte = 0) to FT chip.
            FT_Send_Byte(0);
            charcounter++;
        
            // Every time we send a string to FT chip, its lenght have be multiple of 4.
            // If the length of the string is shorter than that, the we send 0 bytes until it is
            // a multiple of 4.
            while ((charcounter % 4) != 0)
            {
                FT_Send_Byte(0);
                charcounter++;
            }
        }        
    #endif
  
}

FTERROR FT_InflateFromFlash(const uint8 *flashptr, uint32 ramgptr, uint32 size)
{
    uint16 readblocksize, cmdfree;
    
    if (transferinprogress != NONE) return LIST_IN_PROGRESS;
    
    /* *** Send cmd_loadimage command. *** */
    #if defined EVE_FT800
        FT_Transfer_Start((RAM_CMD + ramCMDOffset) | MEMORY_WRITE);    // Start the display list
    #elif defined EVE_FT810
        FT_Transfer_Start(REG_CMDB_WRITE | MEMORY_WRITE);
    #endif

    CMDInflate(ramgptr);
    FT_Transfer_End();    
    
    /* *** Send image data. *** */
    while (size > 0)
    {
        cmdfree = FTGetCMDFifoFreeSpace();
        
        if (size >= cmdfree) readblocksize = cmdfree;
        else readblocksize = size;
        
        #if defined EVE_FT800
            FT_Transfer_Start((RAM_CMD + ramCMDOffset) | MEMORY_WRITE);
        #elif defined EVE_FT810
            FT_Transfer_Start(REG_CMDB_WRITE | MEMORY_WRITE);
        #endif

        FT_Send_ByteArray(flashptr, readblocksize);
        
        flashptr += readblocksize;
        size -= readblocksize;
        
        FT_Transfer_End();

        #if defined EVE_FT800
            FT_Register_Write(REG_CMD_WRITE, ramCMDOffset);
        #endif
        
        while (!FTIsCoproccesorReady()){};
    }     
    
    return OK;    
}

FTERROR FT_InflateFromExternalFlash(uint32 flashptr, uint32 ramgptr, uint32 size)
{
    uint8 rbuffer[512];
    uint16 readblocksize, cmdfree;
    
    if (transferinprogress != NONE) return LIST_IN_PROGRESS;    
    
    /* *** Send cmd_loadimage command. *** */
    #if defined EVE_FT800
        FT_Transfer_Start((RAM_CMD + ramCMDOffset) | MEMORY_WRITE);    // Start the display list
    #elif defined EVE_FT810
        FT_Transfer_Start(REG_CMDB_WRITE | MEMORY_WRITE);
    #endif

    CMDInflate(ramgptr);
    FT_Transfer_End();
    
    #if defined EVE_FT800
        FT_Register_Write(REG_CMD_WRITE, ramCMDOffset);
    #endif
    
    /* *** Send image data. *** */
    while (size > 0)
    {
        if (size >= 512) readblocksize = 512;
        else readblocksize = size;
        
        cmdfree = FTGetCMDFifoFreeSpace();
        
        while (cmdfree < readblocksize) 
        {
            cmdfree = FTGetCMDFifoFreeSpace();
        }
        
        //SST25ReadArray(flashptr, rbuffer, readblocksize);
        
        #if defined EVE_FT800
            FT_Transfer_Start((RAM_CMD + ramCMDOffset) | MEMORY_WRITE);
        #elif defined EVE_FT810
            FT_Transfer_Start(REG_CMDB_WRITE | MEMORY_WRITE);
        #endif

        FT_Send_ByteArray(rbuffer, readblocksize);
        
        flashptr += readblocksize;
        size -= readblocksize;
        
        FT_Transfer_End();

        #if defined EVE_FT800
            FT_Register_Write(REG_CMD_WRITE, ramCMDOffset);
        #endif
        
        while (!FTIsCoproccesorReady()){};
    }     
    
    return OK;
}

FTERROR FT_LoadImageFromExternalFlash(uint32 flashptr, uint32 ramgptr, uint32 size, uint16 options)
{
    uint8 rbuffer[512];
    uint16 readblocksize, cmdfree;
    
    if (transferinprogress != NONE) return LIST_IN_PROGRESS;    
    
    /* *** Send cmd_loadimage command. *** */
    #if defined EVE_FT800
        FT_Transfer_Start((RAM_CMD + ramCMDOffset) | MEMORY_WRITE);    // Start the display list
    #elif defined EVE_FT810
        FT_Transfer_Start(REG_CMDB_WRITE | MEMORY_WRITE);
    #endif

    CMDLoadImage(ramgptr, options);
    FT_Transfer_End();
    
    #if defined EVE_FT800
        FT_Register_Write(REG_CMD_WRITE, ramCMDOffset);
    #endif
    
    /* *** Send image data. *** */
    while (size > 0)
    {
        if (size >= 512) readblocksize = 512;
        else readblocksize = size;
        
        cmdfree = FTGetCMDFifoFreeSpace();
        
        while (cmdfree < readblocksize) {};
        
        //SST25ReadArray(flashptr, rbuffer, readblocksize);
        
        #if defined EVE_FT800
            FT_Transfer_Start((RAM_CMD + ramCMDOffset) | MEMORY_WRITE);
        #elif defined EVE_FT810
            FT_Transfer_Start(REG_CMDB_WRITE | MEMORY_WRITE);
        #endif

        FT_Send_ByteArray(rbuffer, readblocksize);
        
        flashptr += readblocksize;
        size -= readblocksize;
        
        FT_Transfer_End();

        #if defined EVE_FT800
            FT_Register_Write(REG_CMD_WRITE, ramCMDOffset);
        #endif
        
        while (!FTIsCoproccesorReady()){};
    }     
    
    return OK;
}

FTERROR FT_TransferToRAMG(uint32 flashptr, uint32 size)
{
    uint8 rbuffer[512];
    uint16 readblocksize;
    
    if (transferinprogress != NONE) return LIST_IN_PROGRESS;    
    
    /* *** Send image data. *** */
    while (size > 0)
    {
        if (size >= 512) readblocksize = 512;
        else readblocksize = size;
        
       // SST25ReadArray(flashptr, rbuffer, readblocksize);
        
        FT_Transfer_Start((RAM_G + flashptr) | MEMORY_WRITE);
        FT_Send_ByteArray(rbuffer, readblocksize);
        FT_Transfer_End();
        
        flashptr += readblocksize;
        size -= readblocksize;

    }     
    
    return OK;
}

void DLBitmapSource(uint32 address)
{
    DLListNewItem(_DLBitmapSource(address));
}

void DLClearColorRGB(uint8 red, uint8 green, uint8 blue)
{
    DLListNewItem(_DLClearColorRGB(red, green, blue));
}

void DLTag(uint8 tag)
{
    DLListNewItem(_DLTag(tag));
}

void DLColorRGB(uint8 red, uint8 green, uint8 blue)
{
    DLListNewItem(_DLColorRGB(red, green, blue));
}

void DLBitmapHandle(uint8 handle)
{
    DLListNewItem(_DLBitmapHandle(handle));
}

void DLCell(uint8 cell)
{
    DLListNewItem(_DLCell(cell));
}

void DLBitmapLayout(uint8 format, uint16 linestride, uint16 height)
{
    DLListNewItem(_DLBitmapLayout(format, linestride, height));
    
    #if defined EVE_FT810
        DLListNewItem(_DLBitmapLayout_H(linestride, height));
    #endif 
}

void DLBitmapSize(uint8 filter, uint8 wrapx, uint8 wrapy, uint16 width, uint16 height)
{
    DLListNewItem(_DLBitmapSize(filter, wrapx, wrapy, width, height));
    
    #if defined EVE_FT810
        DLListNewItem(_DLBitmapSize_H(width, height));
    #endif 
}

void DLAlphaFunc(uint8 func, uint8 ref)
{
    DLListNewItem(_DLAlphaFunc(func, ref));
}

void DLStencilFunc(uint8 func, uint8 ref, uint8 mask)
{
    DLListNewItem(_DLStencilFunc(func, ref, mask));
}

void DLBlendFunc(uint8 src, uint8 dst)
{
    DLListNewItem(_DLBlendFunc(src, dst));
}

void DLStencilOp(uint8 sfail, uint8 spass)
{
    DLListNewItem(_DLStencilOp(sfail, spass));
}

void DLPointSize(uint32 size)
{
    DLListNewItem(_DLPointSize(size));
}

void DLLineWidth(uint16 width)
{
    DLListNewItem(_DLLineWidth(width));
}

void DLColorA(uint8 alpha)
{
    DLListNewItem(_DLColorA(alpha));
}

void DLClearStencil(uint8 s)
{
    DLListNewItem(_DLClearStencil(s));
}

void DLClearTag(uint8 t)
{
    DLListNewItem(_DLClearTag(t));
}

void DLStencilMask(uint8 mask)
{
    DLListNewItem(_DLStencilMask(mask));
}

void DLTagMask(uint8 mask)
{
    DLListNewItem(_DLTagMask(mask));
}

void DLBitmapTransformA(uint32 a)
{
    DLListNewItem(_DLBitmapTransformA(a));
}

void DLBitmapTransformB(uint32 b)
{
    DLListNewItem(_DLBitmapTransformB(b));
}

void DLBitmapTransformC(uint32 c)
{
    DLListNewItem(_DLBitmapTransformC(c));
}

void DLBitmapTransformD(uint32 d)
{
    DLListNewItem(_DLBitmapTransformD(d));
}

void DLBitmapTransformE(uint32 e)
{
    DLListNewItem(_DLBitmapTransformE(e));
}

void DLBitmapTransformF(uint32 f)
{
    DLListNewItem(_DLBitmapTransformF(f));
}

void DLScissorXY(uint16 x, uint16 y)
{
    DLListNewItem(_DLScissorXY(x, y));
}

void DLScissorSize(uint16 width, uint16 height)
{
    DLListNewItem(_DLScissorSize(width, height));
}

void DLJump(uint16 address)
{
    DLListNewItem(_DLJump(address));
}

void DLBegin(uint8 primitive)
{
    DLListNewItem(_DLBegin(primitive));
}

void DLCall(uint16 address)
{
    DLListNewItem(_DLCall(address));
}

void DLColorMask(uint8 r, uint8 g, uint8 b, uint8 a)
{
    DLListNewItem(_DLColorMask(r, g, b, a));
}

void DLEnd()
{
    DLListNewItem(_DLEnd());
}

void DLSaveContext()
{
    DLListNewItem(_DLSaveContext());
}

void DLRestoreContext()
{
    DLListNewItem(_DLRestoreContext());
}

void DLReturn()
{
    DLListNewItem(_DLReturn());
}

void DLMacro(uint8 macro)
{
    DLListNewItem(_DLMacro(macro));
}

void DLClear(uint8 color, uint8 stencil, uint8 tag)
{
    DLListNewItem(_DLClear(color, stencil, tag));
}

void DLVertex2F(int16 x, int16 y)
{
    DLListNewItem(_DLVertex2F(x, y));
}

void DLVertex2II(uint16 x, uint16 y, uint8 handle, uint8 cell)
{
    DLListNewItem(_DLVertex2II(x, y, handle, cell));
}

/* ************************************************************************** */
/* ************************************************************************** */
void DLVertexFormat(uint8 format)
{
    #if defined EVE_FT800
        vertexFormat = format;
    #endif
    
    #if defined EVE_FT810
        DLListNewItem(_DLVertexFormat(format));
    #endif
}

#if defined EVE_FT810
    
    void DLPaletteSource(uint32 address)
    {
        DLListNewItem(_DLPaletteSource(address));
    }
    
    void DLVertexTranslateX(int32 translation)
    {
        DLListNewItem(_DLVertexTranslateX(translation));
    }
    
    void DLVertexTranslateY(int32 translation)
    {
        DLListNewItem(_DLVertexTranslateY(translation));
    }
    
    void DLNop()
    {
        DLListNewItem(_DLNop());
    }
    
#endif  








void CMDDLStart()
{
    CMDListNewItem(_CMDDLStart());
}

void CMDSwap()
{
    CMDListNewItem(_CMDSwap());
}

void CMDInterrupt(uint32 milliseconds)
{
    CMDListNewItem(_CMDInterrupt(milliseconds));
}

void CMDBgcolor(int8 red, int8 green, int8 blue)
{
    CMDListNewItem(_CMDBgcolor(red, green, blue));
}

void CMDFgcolor(int8 red, int8 green, int8 blue)
{
    CMDListNewItem(_CMDFgcolor(red, green, blue));
}

void CMDGradient(int16 x0, int16 y0, int8 red0, int8 green0, int8 blue0, int16 x1, int16 y1, int8 red1, int8 green1, int8 blue1)
{
    CMDListNewItem(_CMDGradient(x0, y0, red0, green0, blue0, x1, y1, red1, green1, blue1));
}

void CMDText(int16 x, int16 y, int16 font, int16 options, char* text)
{
    CMDListNewItem(_CMDTextNew(x, y, font, options, text));
}

void CMDButton(int16 x, int16 y, int16 width, int16 height, int16 font, int16 options, char* text)
{
    CMDListNewItem(_CMDButton(x, y, width, height, font, options, text));
}

void CMDKeys(int16 x, int16 y, int16 width, int16 height, int16 font, int16 options, char* text)
{
    CMDListNewItem(_CMDKeys(x, y, width, height, font, options, text));
}

void CMDProgressBar(int16 x, int16 y, int16 width, int16 height, int16 options, int16 value, int16 range)
{
    CMDListNewItem(_CMDProgressBar(x, y, width, height, options, value, range));
}

void CMDSlider(int16 x, int16 y, int16 width, int16 height, int16 options, int16 value, int16 range)
{
    CMDListNewItem(_CMDSlider(x, y, width, height, options, value, range));
}

void CMDScrollBar(int16 x, int16 y, int16 width, int16 height, int16 options, int16 value, int16 size, int16 range)
{
    CMDListNewItem(_CMDScrollBar(x, y, width, height, options, value, size, range));
}

void CMDToggle(int16 x, int16 y, int16 width,int16 font, int16 options, int16 state, char* text)
{
    CMDListNewItem(_CMDToggle(x, y, width, font, options, state, text));
}

void CMDGauge(int16 x, int16 y, int16 radius, int16 options, int16 major, int16 minor, int16 value, int16 range)
{
    CMDListNewItem(_CMDGauge(x, y, radius, options, major, minor, value, range));
}

void CMDClock(int16 x, int16 y, int16 radius, int16 options, int16 hours, int16 minutes, int16 seconds, int16 milliseconds)
{
    CMDListNewItem(_CMDClock(x, y, radius, options, hours, minutes, seconds, milliseconds));
}

void CMDCalibrate()
{
    CMDListNewItem(_CMDCalibrate());
}

void CMDSpinner(int16 x, int16 y, int16 style, int16 scale)
{
    CMDListNewItem(_CMDSpinner(x, y, style, scale)); 
}

void CMDStop()
{
    CMDListNewItem(_CMDStop());
}

void CMDMemWrite(int32 ptr, int32 size)
{
    CMDListNewItem(_CMDMemWrite(ptr, size));
}

void CMDMemSet(int32 ptr, int32 value, int32 size)
{
    CMDListNewItem(_CMDMemSet(ptr, value, size));
}

void CMDMemZero(int32 ptr, int32 size)
{
    CMDListNewItem(_CMDMemZero(ptr, size));
}

void CMDMemCopy(int32 dest, int32 src, int32 size)
{
    CMDListNewItem(_CMDMemCopy(dest, src, size));
}

void CMDAppend(int32 ptr, int32 size)
{
    CMDListNewItem(_CMDAppend(ptr, size));
}

void CMDSnapshot(int32 ptr)
{
    CMDListNewItem(_CMDSnapshot(ptr));
}

void CMDInflate(int32 ptr)
{
    CMDListNewItem(_CMDInflate(ptr));
}

int32 CMDGetPtr()
{
    uint16 cmdptr = FT_Register_Read(REG_CMD_WRITE);

    FT_ListStart(DATA);
        CMDListNewItem(_CMDGETPTR(0));
    FT_ListEnd(END_DL_NOSWAP);
    
    return FT_Read_UINT32(RAM_CMD + cmdptr + 4);
}

void CMDLoadImage(int32 ptr, int32 options)
{
    #if defined EVE_FT800
        FT_Transfer_Start((RAM_CMD + ramCMDOffset) | MEMORY_WRITE);    // Start the display list
    #elif defined EVE_FT810
        FT_Transfer_Start(REG_CMDB_WRITE | MEMORY_WRITE);
    #endif

    CMDListNewItem(_CMDLoadImage(ptr, options));
    FT_Transfer_End();
    
    #if defined EVE_FT800
        FT_Register_Write(REG_CMD_WRITE, ramCMDOffset);
    #endif
}

void CMDLoadImage_Data(uint8* data, uint32 datalength, uint8 isend)
{
    uint16 cmdfree;
    
    cmdfree = FTGetCMDFifoFreeSpace();
    while (cmdfree < datalength) {};
        
    #if defined EVE_FT800
        FT_Transfer_Start((RAM_CMD + ramCMDOffset) | MEMORY_WRITE);
    #elif defined EVE_FT810
        FT_Transfer_Start(REG_CMDB_WRITE | MEMORY_WRITE);
    #endif
    
    if (isend) FT_Write_ByteArray_4(data, datalength);
    else 
    {
        FT_Send_ByteArray(data, datalength);
        ramCMDOffset += datalength; CheckCMDOffset();
    }
        
    FT_Transfer_End();

    #if defined EVE_FT800
        FT_Register_Write(REG_CMD_WRITE, ramCMDOffset);
    #endif
    
    while (!FTIsCoproccesorReady()){};
}

void CMDGetProps(int32 ptr, int32* width, int32* height)
{
    uint16 cmdptr = FT_Register_Read(REG_CMD_WRITE);
    
    FT_ListStart(DATA);
        CMDListNewItem(_CMDGETPROPS(ptr, 0, 0));
    FT_ListEnd(END_DL_SWAP);
    
    *width = FT_Read_UINT32(RAM_CMD + cmdptr + 8);
    *height = FT_Read_UINT32(RAM_CMD + cmdptr + 12);
}

void CMDLoadIdentity()
{
    CMDListNewItem(_CMDLoadIdentity());
}

void CMDTranslate(int32 tx, int32 ty)
{
    CMDListNewItem(_CMDTranslate(tx, ty));
}

void CMDScale(int32 x, int32 y)
{
    CMDListNewItem(_CMDScale(x, y));
}

void CMDRotate(int32 angle)
{
    CMDListNewItem(_CMDRotate(angle));
}

void CMDSetmatrix()
{
    CMDListNewItem(_CMDSetmatrix());
}

void CMDSetfont(int32 font, int32 ptr)
{
    CMDListNewItem(_CMDSetfont(font, ptr));
}

void CMDTrack(int16 x, int16 y, int16 width, int16 height, int16 tag)
{
    CMDListNewItem(_CMDTrack(x, y, width, height, tag));
}

void CMDDial(int16 x, int16 y, int16 radius, int16 options, int16 value)
{
    CMDListNewItem(_CMDDial(x, y, radius, options, value));
}

void CMDNumber(int16 x, int16 y, int16 font, int16 options, int32 number)
{
    CMDListNewItem(_CMDNumber(x, y, font, options, number));
}

void CMDScreenSaver()
{
    CMDListNewItem(_CMDScreenSaver());
}

void CMDSketch(int16 x, int16 y, int16 width, int16 height, int32 ptr, int16 format)
{
    CMDListNewItem(_CMDSketch(x, y, width, height, ptr, format));
}

void CMDLogo()
{
    CMDListNewItem(_CMDLogo());
}

void CMDColdstart()
{
    CMDListNewItem(_CMDColdstart());
}

void CMDGradcolor(int8 red, int8 green, int8 blue)
{
    CMDListNewItem(_CMDGradcolor(red, green, blue));
}

/* ************************************************************************** */
/* ************************************************************************** */
void CMDSetBitmap(int32 address, int16 format, int16 width, int16 height)
{
    #if defined EVE_FT800
        DLBitmapSource(address);
        
        switch(format)
        {
            case BITMAP_LAYOUT_ARGB1555: DLBitmapLayout(format, width << 1, height); break;
            case BITMAP_LAYOUT_L1: DLBitmapLayout(format, width, height); break;
            case BITMAP_LAYOUT_L4: DLBitmapLayout(format, width >> 1, height); break;
            case BITMAP_LAYOUT_L8: DLBitmapLayout(format, width, height); break;
            case BITMAP_LAYOUT_RGB332: DLBitmapLayout(format, width, height); break;
            case BITMAP_LAYOUT_ARGB2: DLBitmapLayout(format, width, height); break;
            case BITMAP_LAYOUT_ARGB4: DLBitmapLayout(format, width << 1, height); break;
            case BITMAP_LAYOUT_RGB565: DLBitmapLayout(format, width << 1, height); break;
        }
        
        DLBitmapSize(BITMAP_SIZE_FILTER_NEAREST, BITMAP_SIZE_WRAP_BORDER, BITMAP_SIZE_WRAP_BORDER, width, height);
    #endif
    
    #if defined EVE_FT810
        //CMDListNewItem(_CMDSetBitmap(address, format, width, height));
        
        DLBitmapSource(address);
        
        switch(format)
        {
            case BITMAP_LAYOUT_ARGB1555: DLBitmapLayout(format, width << 1, height); break;
            case BITMAP_LAYOUT_L1: DLBitmapLayout(format, width, height); break;
            case BITMAP_LAYOUT_L2: DLBitmapLayout(format, width >> 2, height); break;
            case BITMAP_LAYOUT_L4: DLBitmapLayout(format, width >> 1, height); break;
            case BITMAP_LAYOUT_L8: DLBitmapLayout(format, width, height); break;
            case BITMAP_LAYOUT_RGB332: DLBitmapLayout(format, width, height); break;
            case BITMAP_LAYOUT_ARGB2: DLBitmapLayout(format, width, height); break;
            case BITMAP_LAYOUT_ARGB4: DLBitmapLayout(format, width << 1, height); break;
            case BITMAP_LAYOUT_RGB565: DLBitmapLayout(format, width << 1, height); break;
        }
        
        DLBitmapSize(BITMAP_SIZE_FILTER_NEAREST, BITMAP_SIZE_WRAP_BORDER, BITMAP_SIZE_WRAP_BORDER, width, height);        
    #endif
}
    
#if defined EVE_FT810
    
    void CMDSetRotate(int32 rotation)
    {
        CMDListNewItem(_CMDSETROTATE(rotation));
    }
    
    void CMDSetBase(int32 base)
    {
        CMDListNewItem(_CMDSETBASE(base));
    }
    
    void CMDSetfont2(int32 font, int32 ptr, int32 firstchar)
    {
        CMDListNewItem(_CMDSETFONT2(font, ptr, firstchar));
    }
    
    void CMDRomfont(int32 handle, int32 font)
    {
        CMDListNewItem(_CMDROMFONT(handle, font));
    }

#endif    


/* [] END OF FILE */
