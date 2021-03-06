#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _MAWarpSignalPreProcPage_c[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         MAWarpSignalPreProcPage.c
* \author	Richard Baldock
* \date		April 2009
* \version      $Id$
* \par
* Address:
*               MRC Human Genetics Unit,
*               MRC Institute of Genetics and Molecular Medicine,
*               University of Edinburgh,
*               Western General Hospital,
*               Edinburgh, EH4 2XU, UK.
* \par
* Copyright (C), [2012],
* The University Court of the University of Edinburgh,
* Old College, Edinburgh, UK.
* 
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be
* useful but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
* PURPOSE.  See the GNU General Public License for more
* details.
*
* You should have received a copy of the GNU General Public
* License along with this program; if not, write to the Free
* Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
* Boston, MA  02110-1301, USA.
* \ingroup      MAPaint
* \brief        
*/

#include <stdio.h>
#include <stdlib.h>

#include <MAPaint.h>
#include <MAWarp.h>

void sgnlBackgroundRemoveCb(
  Widget	w,
  XtPointer	client_data,
  XtPointer	call_data)
{
  WlzObject	*obj1, *obj2;
  WlzErrorNum	errNum=WLZ_ERR_NONE;

  /* check for signal input object and signal domain */
  if( warpGlobals.sgnl.obj && warpGlobals.sgnlObj ){
    if((obj1 = WlzDiffDomain(warpGlobals.sgnl.obj,
			     warpGlobals.sgnlObj, &errNum))){
      if((obj2 = WlzMakeMain(warpGlobals.sgnl.obj->type,
			     obj1->domain,
			     warpGlobals.sgnl.obj->values,
			     NULL, NULL, &errNum))){
	(void) WlzFreeObj(warpGlobals.sgnl.obj);
	WlzStandardIntervalDomain(obj2->domain.i);
	warpGlobals.sgnl.obj = WlzAssignObject(obj2, &errNum);
	warpSetXImage(&(warpGlobals.sgnl));
	warpCanvasExposeCb(warpGlobals.sgnl.canvas,
			   (XtPointer) &(warpGlobals.sgnl),
			   call_data);

	/* note need to cancel any local thresholding */
	warpGlobals.globalThreshVtx.vtX = -10000;
	warpGlobals.globalThreshVtx.vtY = -10000;

	/* clear increment stack */
	sgnlIncrClear();

	/* reset all pre-processing and signal domain */
 	if( warpGlobals.sgnlProcObj ){
	  WlzFreeObj( warpGlobals.sgnlProcObj );
	  warpGlobals.sgnlProcObj = NULL;
	}
	if( warpGlobals.sgnlThreshObj ){
	  WlzFreeObj(warpGlobals.sgnlThreshObj);
	  warpGlobals.sgnlThreshObj = NULL;
	}
	warpSetSignalDomain(NULL);
	warpCanvasExposeCb(w, (XtPointer) &(warpGlobals.sgnl), NULL);
     }
      errNum = WlzFreeObj(obj1);
    }
  }
  else {
    /* warn no objects found */
    HGU_XmUserMessage(globals.topl,
		      "Background removal requires an input\n"
		      "signal image and a defined domain.\n"
		      "Please use the thresholding controls\n"
		      "to define the background region first.",
		      XmDIALOG_FULL_APPLICATION_MODAL);
  }

  if( errNum != WLZ_ERR_NONE ){
    MAPaintReportWlzError(globals.topl, "sgnlBackgroundRemoveCb", errNum);
  }
  return;
}

void sgnlBackgroundSaveCb(
  Widget	w,
  XtPointer	client_data,
  XtPointer	call_data)
{
  String	fileStr;
  FILE		*fp;

  /* check if there is a signal object */
  if( warpGlobals.sgnl.obj ){
    /* get a filename for the section object */
    if((fileStr = HGU_XmUserGetFilename(globals.topl,
					"Please type in a filename\n"
					"for the signal image which\n"
					"be saved as a woolz object",
					"OK", "cancel", "MAPaintSignalObj.wlz",
					NULL, "*.wlz"))){
      if((fp = fopen(fileStr, "wb"))){
	if( WlzWriteObj(fp, warpGlobals.sgnl.obj) != WLZ_ERR_NONE ){
	  HGU_XmUserError(globals.topl,
			  "Save Signal Image:\n"
			  "    Incomplete write, please\n"
			  "    check disk space or quotas.\n"
			  "    Signal image not saved",
			  XmDIALOG_FULL_APPLICATION_MODAL);
	}
	if( fclose(fp) == EOF ){
	  HGU_XmUserError(globals.topl,
			  "Save Signal Image:\n"
			  "    File close error, please\n"
			  "    check disk space or quotas.\n"
			  "    Signal image not saved",
			  XmDIALOG_FULL_APPLICATION_MODAL);
	}
      }
      else {
	HGU_XmUserError(globals.topl,
			"Save Signal Image:\n"
			"    Couldn't open the file for\n"
			"    writing, please check\n"
			"    permissions.\n"
			"    Signal image not saved",
			XmDIALOG_FULL_APPLICATION_MODAL);
      }
      AlcFree(fileStr);
    }
  }

  return;
}

Widget createSignalPreProcPage(
  Widget	notebook)
{
  Widget	page, button, label, widget, toggle;
  XmString	xmstr;

  /* page 1 - pre-processing */
  /* make a major page for background removal */
  page = XtVaCreateManagedWidget("background_page",
				 xmFormWidgetClass, 	notebook,
				 XmNnotebookChildType, XmPAGE,
				 NULL);
  button = XtVaCreateManagedWidget("pre_process_tab",
				   xmPushButtonWidgetClass, notebook,
				   XmNnotebookChildType, XmMAJOR_TAB,
				   NULL);
  XtAddCallback(button, XmNactivateCallback, thresholdMajorPageSelectCb,
		(XtPointer) WLZ_RGBA_THRESH_NONE);

  button = XtVaCreateManagedWidget("background_minor_tab",
				   xmPushButtonWidgetClass, notebook,
				   XmNnotebookChildType, XmMINOR_TAB,
				   NULL);

   /* label to provide instructions for background removal */
  label = XtVaCreateManagedWidget("background_label",
				  xmLabelWidgetClass, page,
				  XmNtopAttachment,	XmATTACH_FORM,
				  XmNleftAttachment,	XmATTACH_FORM,
				  XmNborderWidth,	1,
				  XmNshadowThickness,	2,
				  XmNalignment,		XmALIGNMENT_BEGINNING,
				  NULL);
  xmstr = XmStringCreateLtoR(
    "To remove the background, use the\nthreshold controls to define\n"
    "the background then press Remove.\nTo save the result press Save. ",
    XmSTRING_DEFAULT_CHARSET);
  XtVaSetValues(label, XmNlabelString, xmstr, NULL);
  XmStringFree(xmstr);

  /* now the buttons */
  button = XtVaCreateManagedWidget("background_remove",
				   xmPushButtonWidgetClass, page,
				   XmNtopAttachment,	XmATTACH_WIDGET,
				   XmNtopWidget,	label,
				   XmNleftAttachment,	XmATTACH_FORM,
				   NULL);
  XtAddCallback(button, XmNactivateCallback, sgnlBackgroundRemoveCb, NULL);

  button = XtVaCreateManagedWidget("background_save",
				   xmPushButtonWidgetClass, page,
				   XmNtopAttachment,	XmATTACH_WIDGET,
				   XmNtopWidget,	label,
				   XmNleftAttachment,	XmATTACH_WIDGET,
				   XmNleftWidget,	button,
				   NULL);
  XtAddCallback(button, XmNactivateCallback, sgnlBackgroundSaveCb, NULL);

  /* make a minor page for pre-processing sequence */
  page = XtVaCreateManagedWidget("pre_process_page",
				 xmFormWidgetClass, 	notebook,
				 XmNnotebookChildType, XmPAGE,
				 NULL);

  button = XtVaCreateManagedWidget("pre_process_minor_tab",
				   xmPushButtonWidgetClass, notebook,
				   XmNnotebookChildType, XmMINOR_TAB,
				   NULL);

  /* toggles for the image processing sequence */
  toggle = XtVaCreateManagedWidget("normalise",
				   xmToggleButtonGadgetClass, page,
				   XmNleftAttachment,	XmATTACH_FORM,
				   XmNtopAttachment,	XmATTACH_FORM,
				   NULL);
  XtAddCallback(toggle, XmNvalueChangedCallback, recalcWarpProcObjCb, NULL);
  widget = toggle;
  toggle = XtVaCreateManagedWidget("histo_equalise",
				   xmToggleButtonGadgetClass, page,
				   XmNtopAttachment,	XmATTACH_WIDGET,
				   XmNtopWidget,	toggle,
				   XmNleftAttachment,	XmATTACH_FORM,
				   NULL);
  XtAddCallback(toggle, XmNvalueChangedCallback, recalcWarpProcObjCb, NULL);
  toggle = XtVaCreateManagedWidget("shade_correction",
				   xmToggleButtonGadgetClass, page,
				   XmNtopAttachment,	XmATTACH_WIDGET,
				   XmNtopWidget,	toggle,
				   XmNleftAttachment,	XmATTACH_FORM,
				   NULL);
  XtAddCallback(toggle, XmNvalueChangedCallback, recalcWarpProcObjCb, NULL);

  return page;
}
