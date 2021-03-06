#include "common.h"
#include "patcher.h"

#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#include "rwcore.h"

#include "skeleton.h"
#include "platform.h"



static RwBool               DefaultVideoMode = TRUE;

bool TurnOnAnimViewer = false;

//RsGlobalType                RsGlobal;
RsGlobalType &RsGlobal = *(RsGlobalType*)0x8F4360;

RwUInt32
RsTimer(void)
{
	return psTimer();
}


/*
 *****************************************************************************
 */
void
RsCameraShowRaster(RwCamera * camera)
{
	psCameraShowRaster(camera);

	return;
}

/*
 *****************************************************************************
 */
RwBool
RsCameraBeginUpdate(RwCamera * camera)
{
	return psCameraBeginUpdate(camera);
}

/*
 *****************************************************************************
 */
RwBool
RsRegisterImageLoader(void)
{
	return TRUE;
}

/*
 *****************************************************************************
 */
static              RwBool
RsSetDebug(void)
{
	return TRUE;
}

/*
 *****************************************************************************
 */
void
RsMouseSetPos(RwV2d * pos)
{
	psMouseSetPos(pos);

	return;
}

/*
 *****************************************************************************
 */
RwBool
RsSelectDevice(void)
{
	return psSelectDevice();
}

/*
 *****************************************************************************
 */
RwBool
RsInputDeviceAttach(RsInputDeviceType inputDevice,
					RsInputEventHandler inputEventHandler)
{
	switch (inputDevice)
	{
		case rsKEYBOARD:
			{
				RsGlobal.keyboard.inputEventHandler = inputEventHandler;
				RsGlobal.keyboard.used = TRUE;
				break;
			}
		case rsMOUSE:
			{
				RsGlobal.mouse.inputEventHandler = inputEventHandler;
				RsGlobal.mouse.used = TRUE;
				break;
			}
		case rsPAD:
			{
				RsGlobal.pad.inputEventHandler = inputEventHandler;
				RsGlobal.pad.used = TRUE;
				break;
			}
		default:
			{
				return FALSE;
			}
	}

	return TRUE;
}


/*
 *****************************************************************************
 */
static RwBool
rsCommandLine(RwChar *arg)
{
	RsEventHandler(rsFILELOAD, arg);

	return TRUE;
}


/*
 *****************************************************************************
 */
static RwBool
rsPreInitCommandLine(RwChar *arg)
{
	if( !strcmp(arg, RWSTRING("-vms")) )
	{
		DefaultVideoMode = FALSE;

		return TRUE;
	}
#ifndef MASTER
	if (!strcmp(arg, RWSTRING("-animviewer")))
	{
		TurnOnAnimViewer = TRUE;

		return TRUE;
	}
#endif
	return FALSE;
}

/*
 *****************************************************************************
 */
RsEventStatus
RsKeyboardEventHandler(RsEvent event, void *param)
{
	if (RsGlobal.keyboard.used)
	{
		return RsGlobal.keyboard.inputEventHandler(event, param);
	}

	return rsEVENTNOTPROCESSED;
}

/*
 *****************************************************************************
 */
RsEventStatus
RsPadEventHandler(RsEvent event, void *param)
{
	if (RsGlobal.pad.used)
	{
		return RsGlobal.pad.inputEventHandler(event, param);
	}

	return rsEVENTNOTPROCESSED;
}

/*
 *****************************************************************************
 */
RsEventStatus
RsEventHandler(RsEvent event, void *param)
{
	RsEventStatus       result;
	RsEventStatus       es;
  
	/*
	 * Give the application an opportunity to override any events...
	 */
	es = AppEventHandler(event, param);

	/*
	 * We never allow the app to replace the quit behaviour,
	 * only to intercept...
	 */
	if (event == rsQUITAPP)
	{
		/*
		 * Set the flag which causes the event loop to exit...
		 */
		RsGlobal.quit = TRUE;
	}

	if (es == rsEVENTNOTPROCESSED)
	{
		switch (event)
		{
			case rsSELECTDEVICE:
				result =
					(RsSelectDevice()? rsEVENTPROCESSED : rsEVENTERROR);
				break;

			case rsCOMMANDLINE:
				result = (rsCommandLine((RwChar *) param) ?
						  rsEVENTPROCESSED : rsEVENTERROR);
				break;
			case rsPREINITCOMMANDLINE:
				result = (rsPreInitCommandLine((RwChar *) param) ?
						  rsEVENTPROCESSED : rsEVENTERROR);
				break;
			case rsINITDEBUG:
				result =
					(RsSetDebug()? rsEVENTPROCESSED : rsEVENTERROR);
				break;

			case rsREGISTERIMAGELOADER:
				result = (RsRegisterImageLoader()?
						  rsEVENTPROCESSED : rsEVENTERROR);
				break;

			case rsRWTERMINATE:
				RsRwTerminate();
				result = (rsEVENTPROCESSED);
				break;

			case rsRWINITIALISE:
				result = (RsRwInitialise(param) ?
						  rsEVENTPROCESSED : rsEVENTERROR);
				break;

			case rsTERMINATE:
				RsTerminate();
				result = (rsEVENTPROCESSED);
				break;

			case rsINITIALISE:
				result =
					(RsInitialise()? rsEVENTPROCESSED : rsEVENTERROR);
				break;

			default:
				result = (es);
				break;

		}
	}
	else
	{
		result = (es);
	}

	return result;
}

/*
 *****************************************************************************
 */
void
RsRwTerminate(void)
{
	/* Close RenderWare */

	RwEngineStop();
	RwEngineClose();
	RwEngineTerm();

	return;
}

/*
 *****************************************************************************
 */
RwBool
RsRwInitialise(void *displayID)
{
	RwEngineOpenParams  openParams;
   
	/*
	 * Start RenderWare...
	 */
	 
	if (!RwEngineInit(psGetMemoryFunctions(), 0, rsRESOURCESDEFAULTARENASIZE))
	{
		return (FALSE);
	}

	/*
	 * Install any platform specific file systems...
	 */
	psInstallFileSystem();
	
	/*
	 * Initialize debug message handling...
	 */
	RsEventHandler(rsINITDEBUG, nil);

	/*
	 * Attach all plugins...
	 */
	if (RsEventHandler(rsPLUGINATTACH, nil) == rsEVENTERROR)
	{
		return (FALSE);
	}

	/*
	 * Attach input devices...
	 */
	if (RsEventHandler(rsINPUTDEVICEATTACH, nil) == rsEVENTERROR)
	{
		return (FALSE);
	}
	
	openParams.displayID = displayID;

	if (!RwEngineOpen(&openParams))
	{
		RwEngineTerm();
		return (FALSE);
	}
	
	if (RsEventHandler(rsSELECTDEVICE, displayID) == rsEVENTERROR)
	{
		RwEngineClose();
		RwEngineTerm();
		return (FALSE);
	}
	
	if (!RwEngineStart())
	{
		RwEngineClose();
		RwEngineTerm();
		return (FALSE);
	}

	/*
	 * Register loaders for an image with a particular file extension...
	 */
	RsEventHandler(rsREGISTERIMAGELOADER, nil);

	psNativeTextureSupport();

	RwTextureSetMipmapping(FALSE);
	RwTextureSetAutoMipmapping(FALSE);

	return TRUE;
}

/*
 *****************************************************************************
 */
void
RsTerminate(void)
{
	psTerminate();

	return;
}

/*
 *****************************************************************************
 */
RwBool
RsInitialise(void)
{
	/*
	 * Initialize Platform independent data...
	 */
	RwBool              result;

	RsGlobal.appName = RWSTRING("GTA3");
	RsGlobal.maximumWidth = DEFAULT_SCREEN_WIDTH;
	RsGlobal.maximumHeight = DEFAULT_SCREEN_HEIGHT;
	RsGlobal.width = DEFAULT_SCREEN_WIDTH;
	RsGlobal.height = DEFAULT_SCREEN_HEIGHT;
	
	RsGlobal.maxFPS = 30;
	 
	RsGlobal.quit = FALSE;

	/* setup the keyboard */
	RsGlobal.keyboard.inputDeviceType = rsKEYBOARD;
	RsGlobal.keyboard.inputEventHandler = nil;
	RsGlobal.keyboard.used = FALSE;

	/* setup the mouse */
	RsGlobal.mouse.inputDeviceType = rsMOUSE;
	RsGlobal.mouse.inputEventHandler = nil;
	RsGlobal.mouse.used = FALSE;

	/* setup the pad */
	RsGlobal.pad.inputDeviceType = rsPAD;
	RsGlobal.pad.inputEventHandler = nil;
	RsGlobal.pad.used = FALSE;

	result = psInitialise();

	return result;
}




STARTPATCHES
	InjectHook(0x584890, RsTimer, PATCH_JUMP);
	InjectHook(0x5848A0, RsCameraShowRaster, PATCH_JUMP);
	InjectHook(0x5848B0, RsCameraBeginUpdate, PATCH_JUMP);
	InjectHook(0x5848C0, RsRegisterImageLoader, PATCH_JUMP);
	InjectHook(0x5848D0, RsSetDebug, PATCH_JUMP);
	InjectHook(0x5848E0, RsMouseSetPos, PATCH_JUMP);
	InjectHook(0x5848F0, RsSelectDevice, PATCH_JUMP);
	InjectHook(0x584900, RsInputDeviceAttach, PATCH_JUMP);
	InjectHook(0x584960, rsCommandLine, PATCH_JUMP);
	InjectHook(0x584980, rsPreInitCommandLine, PATCH_JUMP);
	InjectHook(0x5849C0, RsKeyboardEventHandler, PATCH_JUMP);
	InjectHook(0x5849F0, RsPadEventHandler, PATCH_JUMP);
	InjectHook(0x584A20, RsEventHandler, PATCH_JUMP);
	InjectHook(0x584B30, RsRwTerminate, PATCH_JUMP);
	InjectHook(0x584B40, RsRwInitialise, PATCH_JUMP);
	InjectHook(0x584C30, RsTerminate, PATCH_JUMP);
	InjectHook(0x584C40, RsInitialise, PATCH_JUMP);
ENDPATCHES