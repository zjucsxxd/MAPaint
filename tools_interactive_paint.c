#pragma ident "MRC HGU $Id$"
/*****************************************************************************
* Copyright   :    Medical Research Council, UK.                             *
*                  Human Genetics Unit,                                      *
*                  Western General Hospital.                                 *
*                  Edinburgh.                                                *
******************************************************************************
* Project     :    Mouse Atlas Project					     *
* File        :    tools_interactive_paint.c				     *
******************************************************************************
* Author Name :    Richard Baldock					     *
* Author Login:    richard@hgu.mrc.ac.uk				     *
* Date        :    Fri Jun 24 13:20:32 1994				     *
* Synopsis    : 							     *
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#include <MAPaint.h>

extern WlzPolygonDomain *HGU_XGetPolyDomain(Display		*dpy,
					    Window		win,
					    int			mode,
					    HGU_XInteractCallbacks  *callbacks,
					    WlzPolygonDomain	*start);

#define stipple_width 8
#define stipple_height 8
static unsigned char stipple_bits[] = {
   0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

static void thresh_conn_cb(Widget	w,
			   XtPointer	client_data,
			   XtPointer	call_data);
static void paint_shape_cb(Widget	w,
			   XtPointer	client_data,
			   XtPointer	call_data);
static void paint_border_cb(Widget	w,
			   XtPointer	client_data,
			   XtPointer	call_data);
static void paint_size_cb(Widget	w,
			  XtPointer	client_data,
			  XtPointer	call_data);
static void draw_cursor_cb(Widget	w,
			   XtPointer	client_data,
			   XtPointer	call_data);

static MenuItem thresh_conn_items[] = {	/* threshold connectivity items */
{"4_neighbour", &xmPushButtonGadgetClass, 0, NULL, NULL, False,
     thresh_conn_cb, (XtPointer) WLZ_4_CONNECTED,
     HGU_XmHelpStandardCb, "",
     XmTEAR_OFF_DISABLED, False, False, NULL},
{"8_neighbour", &xmPushButtonGadgetClass, 0, NULL, NULL, True,
     thresh_conn_cb, (XtPointer) WLZ_8_CONNECTED,
     HGU_XmHelpStandardCb, "",
     XmTEAR_OFF_DISABLED, False, False, NULL},
NULL,
};

static MenuItem paint_shape_items[] = {	/* paint shape items */
{"Circle", &xmPushButtonGadgetClass, 0, NULL, NULL, True,
     paint_shape_cb, (XtPointer) HGU_XBLOBTYPE_CIRCLE,
     HGU_XmHelpStandardCb, "",
     XmTEAR_OFF_DISABLED, False, False, NULL},
{"Square", &xmPushButtonGadgetClass, 0, NULL, NULL, False,
     paint_shape_cb, (XtPointer) HGU_XBLOBTYPE_SQUARE,
     HGU_XmHelpStandardCb, "",
     XmTEAR_OFF_DISABLED, False, False, NULL},
NULL,
};

static MenuItem draw_cursor_items[] = {	/* draw cursor items */
{"Dot", &xmPushButtonGadgetClass, 0, NULL, NULL, True,
     draw_cursor_cb, (XtPointer) HGU_XCURSOR_POINT,
     HGU_XmHelpStandardCb, "",
     XmTEAR_OFF_DISABLED, False, False, NULL},
{"Sights", &xmPushButtonGadgetClass, 0, NULL, NULL, False,
     draw_cursor_cb, (XtPointer) HGU_XCURSOR_CROSS,
     HGU_XmHelpStandardCb, "",
     XmTEAR_OFF_DISABLED, False, False, NULL},
{"Arrow", &xmPushButtonGadgetClass, 0, NULL, NULL, False,
     draw_cursor_cb, (XtPointer) HGU_XCURSOR_POINTER,
     HGU_XmHelpStandardCb, "",
     XmTEAR_OFF_DISABLED, False, False, NULL},
NULL,
};

static Widget	thresh_slider, draw_paint_controls, paint_blob_slider;
static WlzConnectType   thresh_connectivity_type = WLZ_4_CONNECTED;
static int		thresh_range_size = 5;
static double		thresh_smooth_size = 3.0;
static int		paint_blob_size = 6;
static int		paint_blob_border = 0;
static HGU_XBlobType	paint_blob_type = HGU_XBLOBTYPE_CIRCLE;
static HGU_XCursorType	draw_cursor_type = HGU_XCURSOR_POINT;
static HGU_XInteractCallbacks		paint_callbacks;
static HGU_XGridConstraint		paint_constraint;
static WlzObject *cursorObj=NULL;
static Cursor	cursor;

static void setThreshRangeCb(
  Widget		w,
  XtPointer	client_data,
  XtPointer	call_data)
{
  Widget	slider=w;

  while( strcmp(XtName(slider), "range") ){
    if( (slider = XtParent(slider)) == NULL )
      return;
  }
  thresh_range_size = (int) HGU_XmGetSliderValue( slider );

  return;
}

static void setThreshSmoothWidthCb(
  Widget		w,
  XtPointer	client_data,
  XtPointer	call_data)
{
  Widget	slider=w;

  while( strcmp(XtName(slider), "gaussian_width") ){
    if( (slider = XtParent(slider)) == NULL )
      return;
  }
  thresh_smooth_size = (double) HGU_XmGetSliderValue( slider );

  return;
}

static void setBlobCursor(
  Display	*dpy,
  Window	win,
  HGU_XBlobType	type,
  int		radius,
  int		border)
{
  WlzDomain	cursorDom;
  WlzValues	cursorVals;

  /* clear the object cursor object */
  if( cursorObj ){
    WlzFreeObj(cursorObj);
    cursorObj=NULL;
    XFreeCursor(dpy, cursor);
  }

  /* unset the old cursor */
  XUndefineCursor(dpy, win);

  /* make an object to define the cursor */
  switch( type ){
  case HGU_XBLOBTYPE_CIRCLE:
    cursorObj = WlzMakeCircleObject((double) radius + 0.3, 0.0, 0.0, NULL);
    break;
  case HGU_XBLOBTYPE_SQUARE:
    cursorDom.i = WlzMakeIntervalDomain(WLZ_INTERVALDOMAIN_RECT,
					-radius, radius, -radius, radius,
					NULL);
    cursorVals.core = NULL;
    cursorObj = WlzMakeMain(WLZ_2D_DOMAINOBJ, cursorDom, cursorVals,
			    NULL, NULL, NULL);
    break;
  }

  /* set the new cursor */
  cursor = HGU_XCreateObjectCursor(dpy, win, cursorObj, border);
  XDefineCursor(dpy, win, cursor);

  return;
}

void thresh_conn_cb(
  Widget		w,
  XtPointer	client_data,
  XtPointer	call_data)
{
  thresh_connectivity_type = (WlzConnectType) client_data;
  return;
}

void draw_cursor_cb(
  Widget		w,
  XtPointer	client_data,
  XtPointer	call_data)
{
  draw_cursor_type = (HGU_XCursorType) client_data;

  /* set the cursor if a paint_key registered */
  if((paint_key) &&
     (globals.currentPaintAction == MAPAINT_DRAW_2D) ){
    HGU_XSetCursor(XtDisplay(paint_key->canvas),
		   XtWindow(paint_key->canvas), draw_cursor_type);
  }

  return;
}

void paint_shape_cb(
  Widget		w,
  XtPointer	client_data,
  XtPointer	call_data)
{
  paint_blob_type = (HGU_XBlobType) client_data;

  /* set the cursor if a paint_key registered */
  if((paint_key) &&
     (globals.currentPaintAction == MAPAINT_PAINT_BALL_2D) ){
    /* set the paint-ball cursor */
    setBlobCursor(XtDisplay(paint_key->canvas),
		  XtWindow(paint_key->canvas),
		  paint_blob_type, paint_blob_size, paint_blob_border);
  }

  return;
}

void paint_border_cb(
  Widget		w,
  XtPointer	client_data,
  XtPointer	call_data)
{
  XmToggleButtonCallbackStruct *cbs =
    (XmToggleButtonCallbackStruct *) call_data;

  paint_blob_border = cbs->set;

  /* set the cursor if a paint_key registered */
  if((paint_key) &&
     (globals.currentPaintAction == MAPAINT_PAINT_BALL_2D) ){
    /* set the paint-ball cursor */
    setBlobCursor(XtDisplay(paint_key->canvas),
		  XtWindow(paint_key->canvas),
		  paint_blob_type, paint_blob_size, paint_blob_border);
  }

  return;
}

void paint_size_cb(
  Widget		w,
  XtPointer	client_data,
  XtPointer	call_data)
{
  Widget	slider = w;

  while( strcmp(XtName(slider), "blob_size") ){
    if( (slider = XtParent(slider)) == NULL )
      return;
  }

  paint_blob_size = (int) HGU_XmGetSliderValue( slider );

  /* set the cursor if a paint_key registered */
  if((paint_key) &&
     (globals.currentPaintAction == MAPAINT_PAINT_BALL_2D) ){
    setBlobCursor(XtDisplay(paint_key->canvas),
		  XtWindow(paint_key->canvas),
		  paint_blob_type, paint_blob_size, paint_blob_border);
  }

  return;
}

/* tool menu manipulation procedure to mimic accelerators from a
   painting view window */
void selectNextPaintCursor(int downFlag)
{
  int		currIndex, newIndex, numCursors;
  Widget	button, option_menu;
  XmPushButtonCallbackStruct	cbs;
  String	strBuf;

  /* get the menu and set/unset the toggles and reset the
     selected widget ID */
  if( (option_menu =
       XtNameToWidget(globals.topl,
		      "*tool_control_dialog*paint_draw_form1.cursor"))
     == NULL ){
    return;
  }

  /* get the current selection */
  XtVaGetValues(option_menu, XmNmenuHistory, &button, NULL);

  /* find the current index and calculate the new */
  numCursors = 0;
  currIndex = 0;
  while( draw_cursor_items[numCursors].name ){
    if(button &&
       !strcmp(draw_cursor_items[numCursors].name, XtName(button)) ){
      currIndex = numCursors;
    }
    numCursors++;
  }
  if( downFlag ){
    newIndex = (numCursors + currIndex - 1) % numCursors;
  }
  else {
    newIndex = (currIndex + 1) % numCursors;
  }

  /* set the new */
  strBuf = (String) AlcMalloc(strlen(draw_cursor_items[newIndex].name)
			      + 4);
  sprintf(strBuf, "*%s", draw_cursor_items[newIndex].name);
  button = XtNameToWidget(option_menu, strBuf);

  /* set the menu history */
  XtVaSetValues(option_menu, XmNmenuHistory, button, NULL);

  /* call the callbacks */
  cbs.reason = XmCR_ACTIVATE;
  cbs.event = NULL;
  cbs.click_count = 0;
  XtCallCallbacks(button, XmNactivateCallback, &cbs);
  
  return;
}

void MAPaintDraw2DInit(
  ThreeDViewStruct	*view_struct)
{
  /* initialise the callbacks */
  paint_callbacks.type            = 0;
  paint_callbacks.window_func     = NULL;
  paint_callbacks.window_data     = NULL;
  paint_callbacks.non_window_func = (HGU_XInteractCallback)
    InteractDispatchEvent;
  paint_callbacks.non_window_data = NULL;
  paint_callbacks.help_func       = (HGU_XInteractCallback) InteractHelpCb;
  paint_callbacks.help_data       = "Paint 3D - HGU_XGetDomain Help";

  /* set the cursor */
  HGU_XSetCursor(XtDisplay(view_struct->canvas),
		 XtWindow(view_struct->canvas), draw_cursor_type);

  return;
}

void MAPaintDraw2DQuit(
  ThreeDViewStruct	*view_struct)
{
  /* unset the cursor */
  XUndefineCursor(XtDisplay(view_struct->canvas),
		 XtWindow(view_struct->canvas));

  return;
}

void MAPaintDraw2DCb(
  Widget	w,
  XtPointer	client_data,
  XtPointer	call_data)
{
  ThreeDViewStruct	*view_struct = (ThreeDViewStruct *) client_data;
  WlzThreeDViewStruct	*wlzViewStr= view_struct->wlzViewStr;
  XmAnyCallbackStruct	*cbs = (XmAnyCallbackStruct *) call_data;
  int			x, y;
  DomainSelection	currDomain;
  int			delFlag;
  WlzObject		*obj, *obj1;
  WlzPolygonDomain	*poly, *startPoly;
  WlzFVertex2		fpVtx;

  /* when left button pressed, get the polyline and install
     the new domain either adding to or subtracting from the
     current domain. Take account of dominance and reset and 
     redisplay the painted object */
  switch( cbs->event->type ){

  case ButtonPress:
    switch( cbs->event->xbutton.button ){

    case Button1:
    case Button2:
      /* do nothing if shift pressed - magnify option */
      if( cbs->event->xbutton.state & ButtonStateIgnoreMask ){
	break;
      }

      /* save the current domain selection and meta button state */
      currDomain = globals.current_domain;
      delFlag = ((cbs->event->xbutton.state & Mod1Mask) ||
		 (cbs->event->xbutton.button == Button2));

      /* get the polyline, rescale as required */
      fpVtx.vtX = cbs->event->xbutton.x;
      fpVtx.vtY = cbs->event->xbutton.y;
      startPoly = WlzAssignPolygonDomain(
	WlzMakePolyDmn(WLZ_POLYGON_FLOAT, (WlzIVertex2 *) &fpVtx,
		       1, 1, 1, NULL), NULL);
      globals.paint_action_quit_flag = 0;
      poly = HGU_XGetPolyDomain(XtDisplay(view_struct->canvas),
				XtWindow(view_struct->canvas),
				0, &paint_callbacks, startPoly);
      WlzFreePolyDmn(startPoly);

      /* define the new domain */
      if( poly ){
	/* rescale and shift the object here */
	obj = WlzPolyToObj(poly, WLZ_SIMPLE_FILL, NULL);
	WlzFreePolyDmn(poly);
	if( wlzViewStr->scale > 0.95 ){
	  obj1 = WlzIntRescaleObj(obj, WLZ_NINT(wlzViewStr->scale), 0, NULL);
	}
	else {
	  float invScale = 1.0 / wlzViewStr->scale;
	  obj1 = WlzIntRescaleObj(obj, WLZ_NINT(invScale), 1, NULL);
	}
	WlzFreeObj(obj);
	obj = WlzShiftObject(obj1,
			     view_struct->painted_object->domain.i->kol1,
			     view_struct->painted_object->domain.i->line1,
			     0, NULL);
	WlzFreeObj(obj1);
      }
      else {
	obj = NULL;
      }

      /* push the domains for undo
	 reset the painted object and redisplay */
      if( obj ){
	pushUndoDomains(view_struct);
	setDomainIncrement(obj, view_struct, currDomain, delFlag);
	WlzFreeObj( obj );
      }
      break;

    default:
      break;

    }
    break;

  case ButtonRelease:
    break;

  case MotionNotify:
    break;

  case KeyPress:
    switch( XLookupKeysym(&(cbs->event->xkey), 0) ){
	
    case XK_Right:
    case XK_f:
      /* if <alt> or <alt gr> pressed then change tool option */
      if( cbs->event->xkey.state&(Mod1Mask|Mod2Mask) ){
	selectNextPaintCursor(0);
      }
      break;

    case XK_Up:
    case XK_p:
      break;

    case XK_Left:
    case XK_b:
      /* if <alt> or <alt gr> pressed then change tool option */
      if( cbs->event->xkey.state&(Mod1Mask|Mod2Mask) ){
	selectNextPaintCursor(1);
      }
      break;

    case XK_Down:
    case XK_n:
      break;
    }
    break;

  default:
    break;
  }

  return;
}

void selectNextPaintBallSize(int downFlag)
{
  float		currVal, minVal, maxVal;
  Widget	slider;

  /* get the slider */
  if( (slider =
       XtNameToWidget(globals.topl,
		      "*tool_control_dialog*paint_draw_form1.blob_size"))
     == NULL ){
    return;
  }

  /* get current val and val range */
  currVal = HGU_XmGetSliderValue(slider);
  HGU_XmGetSliderRange(slider, &minVal, &maxVal);

  /* calculate next value */
  if( downFlag ){
    currVal = (currVal <= minVal) ? minVal : currVal - 1.0;
  }
  else {
    currVal = (currVal >= maxVal) ? maxVal : currVal + 1.0;
  }
  HGU_XmSetSliderValue(slider, currVal);
    
  /* call the callbacks */
  paint_size_cb(slider, NULL, NULL);
  
  return;
}

void MAPaintPaintBall2DInit(
  ThreeDViewStruct	*view_struct)
{

  /* set the cursor */
  setBlobCursor(XtDisplay(view_struct->canvas),
		XtWindow(view_struct->canvas),
		paint_blob_type, paint_blob_size, paint_blob_border);

  return;
}

void MAPaintPaintBall2DQuit(
  ThreeDViewStruct	*view_struct)
{
  /* unset the cursor */
  XUndefineCursor(XtDisplay(view_struct->canvas),
		  XtWindow(view_struct->canvas));

  /* just in case */
  view_struct->viewLockedFlag = 0;

  return;
}

static int paintBallDelFlag=0;
static DomainSelection paintBallCurrDomain;
static int paintBallTrigger=0;

void MAPaintPaintBall2DCb(
  Widget	w,
  XtPointer	client_data,
  XtPointer	call_data)
{
  ThreeDViewStruct	*view_struct = (ThreeDViewStruct *) client_data;
  WlzThreeDViewStruct	*wlzViewStr= view_struct->wlzViewStr;
  XmAnyCallbackStruct	*cbs = (XmAnyCallbackStruct *) call_data;
  int			delX, delY;
  WlzObject		*obj, *obj1;
  WlzFVertex2		fpVtx;

  /*  */
  switch( cbs->event->type ){

  case ButtonPress:
    switch( cbs->event->xbutton.button ){

    case Button1:
    case Button2:
      /* do nothing if shift pressed - magnify option */
      if( cbs->event->xbutton.state & ButtonStateIgnoreMask ){
	paintBallTrigger = 0;
	break;
      }

      /* save the current domain and delete choice */
      paintBallCurrDomain = globals.current_domain;
      paintBallDelFlag = ((cbs->event->xbutton.state & Mod1Mask) ||
			  (cbs->event->xbutton.button == Button2));
      paintBallTrigger = 1;
      view_struct->viewLockedFlag = 1;

      /* push the domains for undo */
      pushUndoDomains(view_struct);

      /* use the cursor object to increment the domain */
      delX = (int) cbs->event->xbutton.x - 1;
      delY = (int) cbs->event->xbutton.y - 1;
      obj = WlzAssignObject(WlzShiftObject(cursorObj, delX, delY,
					   0, NULL), NULL);
					   
      if( wlzViewStr->scale > 0.95 ){
	obj1 = WlzIntRescaleObj(obj, WLZ_NINT(wlzViewStr->scale), 0, NULL);
      }
      else {
	float invScale = 1.0 / wlzViewStr->scale;
	obj1 = WlzIntRescaleObj(obj, WLZ_NINT(invScale), 1, NULL);
      }
      obj1 = WlzAssignObject(obj1, NULL);
      WlzFreeObj(obj);
      delX =  wlzViewStr->minvals.vtX;
      delY =  wlzViewStr->minvals.vtY;
      obj = WlzAssignObject(WlzShiftObject(obj1, delX, delY,
					   0, NULL), NULL);
      WlzFreeObj(obj1);

      /* reset the painted object and redisplay */
      if( obj ){
	setDomainIncrement(obj, view_struct, paintBallCurrDomain,
			   paintBallDelFlag);
	WlzFreeObj( obj );
      }
      break;

    default:
      break;

    }
    break;

  case ButtonRelease:
    switch( cbs->event->xbutton.button ){
    case Button1:
    case Button2:
      paintBallTrigger = 0;
      view_struct->viewLockedFlag = 0;
      break;

    default:
      break;
    }
    break;

  case MotionNotify:
    /* use the cursor object to increment the domain
       should check that button 1 or button 2 is pressed */
    if((paintBallTrigger) &&
       (cbs->event->xmotion.state & (Button1Mask|Button2Mask)) ){
      delX = (int) cbs->event->xbutton.x - 1;
      delY = (int) cbs->event->xbutton.y - 1;
      obj = WlzAssignObject(WlzShiftObject(cursorObj, delX, delY,
					   0, NULL), NULL);
					   
      if( wlzViewStr->scale > 0.95 ){
	obj1 = WlzIntRescaleObj(obj, WLZ_NINT(wlzViewStr->scale), 0, NULL);
      }
      else {
	float invScale = 1.0 / wlzViewStr->scale;
	obj1 = WlzIntRescaleObj(obj, WLZ_NINT(invScale), 1, NULL);
      }
      obj1 = WlzAssignObject(obj1, NULL);
      WlzFreeObj(obj);
      delX =  wlzViewStr->minvals.vtX;
      delY =  wlzViewStr->minvals.vtY;
      obj = WlzAssignObject(WlzShiftObject(obj1, delX, delY,
					   0, NULL), NULL);
      WlzFreeObj(obj1);

      /* reset the painted object and redisplay */
      if( obj ){
	setDomainIncrement(obj, view_struct, paintBallCurrDomain,
			 paintBallDelFlag);
	WlzFreeObj( obj );
      }
    }
    break;

  case KeyPress:
    switch( XLookupKeysym(&(cbs->event->xkey), 0) ){
	
    case XK_Right:
    case XK_f:
      /* if <alt> or <alt gr> pressed then change tool option */
      if( cbs->event->xkey.state&(Mod1Mask|Mod2Mask) ){
	selectNextPaintBallSize(0);
      }
      break;

    case XK_Up:
    case XK_p:
      break;

    case XK_Left:
    case XK_b:
      /* if <alt> or <alt gr> pressed then change tool option */
      if( cbs->event->xkey.state&(Mod1Mask|Mod2Mask) ){
	selectNextPaintBallSize(1);
      }
      break;

    case XK_Down:
    case XK_n:
      break;
    }
    break;

  default:
    break;
  }

  return;
}

static WlzGreyValueWSpace *threshold_gVWSp=NULL;

static void thresholdCanvasMotionEventHandler(
  Widget        w,
  XtPointer     client_data,
  XEvent        *event,
  Boolean       *continue_to_dispatch)
{
  ThreeDViewStruct	*view_struct = (ThreeDViewStruct *) client_data;
  WlzThreeDViewStruct	*wlzViewStr= view_struct->wlzViewStr;
  char		g_str[64];
  int		x, y;
  int		iVal;
  double	dVal;

  /* check if initialised */
  if( threshold_gVWSp == NULL ){
    /* attempt to reset */
    if( view_struct->view_object == NULL ){
      view_struct->view_object =
	WlzGetSectionFromObject(globals.orig_obj,
				view_struct->wlzViewStr,
				NULL);
    }
    if( (threshold_gVWSp = WlzGreyValueMakeWSp(view_struct->view_object,
					      NULL)) == NULL ){
      return;
    }
  }

  /* call the canvas input callbacks */
  switch( event->type ){
  case MotionNotify:
    /* get the grey value if a button is not pressed */
    if( !(event->xmotion.state & (Button1Mask|Button2Mask)) ){
      x = ((int) (event->xmotion.x / wlzViewStr->scale) +
	   wlzViewStr->minvals.vtX);
      y = ((int) (event->xmotion.y  / wlzViewStr->scale) +
	   wlzViewStr->minvals.vtY);
      WlzGreyValueGet(threshold_gVWSp, 0, (double) y, (double) x);
    }

    /* put up the feedback string */
    switch( threshold_gVWSp->gType )
    {
    case WLZ_GREY_INT:
      iVal = (int) ((*(threshold_gVWSp->gVal)).inv);
      (void) sprintf(&g_str[0], "value: %d, range: (%d, %d)",
		     iVal, iVal-thresh_range_size,
		     iVal+thresh_range_size);
      break;
    case WLZ_GREY_SHORT:
      iVal = (int) ((*(threshold_gVWSp->gVal)).shv);
      (void) sprintf(&g_str[0], "value: %d, range: (%d, %d)",
		     iVal, iVal-thresh_range_size,
		     iVal+thresh_range_size);
      break;
    case WLZ_GREY_UBYTE:
      iVal = (int) ((*(threshold_gVWSp->gVal)).ubv);
      (void) sprintf(&g_str[0], "value: %d, range: (%d, %d)",
		     iVal, WLZ_MAX(iVal-thresh_range_size, 0),
		     WLZ_MIN(iVal+thresh_range_size, 255));
      break;
    case WLZ_GREY_FLOAT:
      dVal = (double) ((*(threshold_gVWSp->gVal)).flv);
      (void) sprintf(&g_str[0], "value: %g, range: (%g, %g)",
		     dVal, dVal-thresh_range_size,
		     dVal+thresh_range_size);
      break;
    case WLZ_GREY_DOUBLE:
      dVal = (double) ((*(threshold_gVWSp->gVal)).dbv);
      (void) sprintf(&g_str[0], "value: %g, range: (%g, %g)",
		     dVal, dVal-thresh_range_size,
		     dVal+thresh_range_size);
      break;
    }
    XtVaSetValues(view_struct->text, XmNvalue, g_str, NULL);
    break;

  default:
    break;
  }
  
  return;
}

void changeThreshRange(int downFlag)
{
  float		currVal, minVal, maxVal;
  Widget	slider;

  /* get the slider */
  if( (slider =
       XtNameToWidget(globals.topl,
		      "*tool_control_dialog*threshold_form1.range"))
     == NULL ){
    return;
  }

  /* get current val and val range */
  currVal = HGU_XmGetSliderValue(slider);
  HGU_XmGetSliderRange(slider, &minVal, &maxVal);

  /* calculate next value */
  if( downFlag ){
    currVal = (currVal <= minVal) ? minVal : currVal - 1.0;
  }
  else {
    currVal = (currVal >= maxVal) ? maxVal : currVal + 1.0;
  }
  HGU_XmSetSliderValue(slider, currVal);
    
  /* call the callbacks */
  setThreshRangeCb(slider, NULL, NULL);
  
  return;
}

void MAPaintThreshold2DInit(
  ThreeDViewStruct	*view_struct)
{
  /* make sure the view object is present and initialise the
     grey-value workspace */
  if( view_struct->view_object == NULL ){
    view_struct->view_object =
      WlzGetSectionFromObject(globals.orig_obj,
			      view_struct->wlzViewStr,
			      NULL);
  }
  if( threshold_gVWSp = WlzGreyValueMakeWSp(view_struct->view_object,
					    NULL) ){

    /* set an event handler to display the current grey-value */
    XtAddEventHandler(view_struct->canvas, PointerMotionMask, False,
		      thresholdCanvasMotionEventHandler, view_struct);
  }

  return;
}

void MAPaintThreshold2DQuit(
  ThreeDViewStruct	*view_struct)
{
  if( threshold_gVWSp ){
    /* remove the event handler */
    XtRemoveEventHandler(view_struct->canvas, PointerMotionMask, False,
			 thresholdCanvasMotionEventHandler, view_struct);

    /* clear the grey-value workspace */
    WlzGreyValueFreeWSp(threshold_gVWSp);
    threshold_gVWSp = NULL;
  }

  view_struct->viewLockedFlag = 0;
  return;
}

static WlzObject *get_thresh_obj(
    WlzObject	*ref_obj,
    int		x,
    int		y,
    int		thresh_up,
    int		thresh_down)
{
    WlzObject	*obj1, *obj2, **obj_list;
    WlzGreyP	greyp;
    WlzPixelV	threshV;
    WlzGreyType	gtype;
    WlzGreyValueWSpace	*gVWSp = NULL;
    int		i, num_obj;

    /* get the selected grey-value */
    if( ref_obj == NULL ){
      return NULL;
    }
    gtype = WlzGreyTableTypeToGreyType(ref_obj->values.core->type, NULL);
    gVWSp = WlzGreyValueMakeWSp(ref_obj, NULL);
    threshV.type = WLZ_GREY_INT;
    WlzGreyValueGet(gVWSp, 0, (double) y, (double) x);
    switch(gtype)
    {
    case WLZ_GREY_INT: 
      threshV.v.inv = (int) ((*(gVWSp->gVal)).inv);
      break;
    case WLZ_GREY_SHORT:
      threshV.v.inv = (int) ((*(gVWSp->gVal)).shv);
      break;
    case WLZ_GREY_UBYTE:
      threshV.v.inv = (int) ((*(gVWSp->gVal)).ubv);
      break;
    case WLZ_GREY_FLOAT:
      threshV.v.inv = (int) ((*(gVWSp->gVal)).flv);
      break;
    case WLZ_GREY_DOUBLE:
      threshV.v.inv = (int) ((*(gVWSp->gVal)).dbv);
      break;
    }
    WlzGreyValueFreeWSp(gVWSp);

    /* threshold within the required range */
    threshV.v.inv -= thresh_down;
    if( (obj1 = WlzThreshold(ref_obj, threshV, WLZ_THRESH_HIGH,
			     NULL)) == NULL)
    {
	return( NULL );
    }
    obj1 = WlzAssignObject(obj1, NULL);
    threshV.v.inv += thresh_down + thresh_up + 1;
    if( (obj2 = WlzThreshold(obj1, threshV, WLZ_THRESH_LOW,
			     NULL)) == NULL )
    {
	WlzFreeObj( obj1 );
	return( NULL );
    }
    obj2 = WlzAssignObject(obj2, NULL);

    /* find the object at the test point */
    if( WlzLabel(obj2, &num_obj, &obj_list, 4096, 0,
		 thresh_connectivity_type) == WLZ_ERR_NONE ){
      WlzFreeObj( obj2 );
      obj2 = NULL;

      for(i=0; i < num_obj; i++){
	if( WlzInsideDomain( obj_list[i], 0.0, 
			    (double) y, (double) x, NULL ) ){
	  obj2 = WlzAssignObject(obj_list[i], NULL);
	}
	else {
	  WlzFreeObj( obj_list[i] );
	}
      }
      AlcFree((void *) obj_list);
    }
    else {
      WlzFreeObj( obj2 );
      obj2 = NULL;
      if( num_obj >= 4096 ){
	for(i=0; i < num_obj; i++){
	  if( WlzInsideDomain( obj_list[i], 0.0, 
			      (double) y, (double) x, NULL ) ){
	    obj2 = WlzAssignObject(obj_list[i], NULL);
	  }
	  else {
	    WlzFreeObj( obj_list[i] );
	  }
	}
	AlcFree((void *) obj_list);
      }
    }	

    return( obj2 );
}

static int thresholdInitialX, thresholdInitialY;
static int initial_thresh_range_size;
static WlzObject *prevObj, *threshObj;

void MAPaintThreshold2DCb(
  Widget	w,
  XtPointer	client_data,
  XtPointer	call_data)
{
  ThreeDViewStruct	*view_struct = (ThreeDViewStruct *) client_data;
  WlzThreeDViewStruct	*wlzViewStr= view_struct->wlzViewStr;
  XmAnyCallbackStruct	*cbs = (XmAnyCallbackStruct *) call_data;
  WlzObject		*obj, *obj1;
  int			x, y;
  Widget		widget;
  Boolean		constrainedFlg, smoothFlg;

  /*  */
  switch( cbs->event->type ){

  case ButtonPress:
    switch( cbs->event->xbutton.button ){

    case Button1:
    case Button2:
      /* do nothing if shift pressed - magnify option */
      if( cbs->event->xbutton.state & ButtonStateIgnoreMask ){
	paintBallTrigger = 0;
	break;
      }

      /* save the current domain and delete choice */
      paintBallCurrDomain = globals.current_domain;
      paintBallDelFlag = ((cbs->event->xbutton.state & Mod1Mask) ||
			  (cbs->event->xbutton.button == Button2));
      paintBallTrigger = 1;
      view_struct->viewLockedFlag = 1;

      /* push the domains for undo */
      pushUndoDomains(view_struct);

      /* set the initial coordinates to get the threshold range */
      thresholdInitialX = cbs->event->xbutton.x;
      thresholdInitialY = cbs->event->xbutton.y;
      initial_thresh_range_size = thresh_range_size;

      /* get the threshold object */
      if( widget = XtNameToWidget
	 (globals.topl,
	  "*tool_control_dialog*threshold_form1.constrained_threshold") ){
	XtVaGetValues(widget, XmNset, &constrainedFlg, NULL);
      }
      else {
	constrainedFlg = True;
      }

      if( constrainedFlg == True ){
	obj1 = NULL;
	if( paintBallCurrDomain ==
	   getSelectedDomainType(thresholdInitialX, thresholdInitialY,
				 view_struct) ){
	  obj1 = get_domain_from_object(view_struct->painted_object,
					paintBallCurrDomain);
	  setDomainIncrement(obj1, view_struct, paintBallCurrDomain, 1);
	}
	  
	if( threshObj = getSelectedRegion(thresholdInitialX,
					  thresholdInitialY,
					  view_struct) ){
	  threshObj->values =
	    WlzAssignValues(view_struct->view_object->values, NULL);
	}

	if( obj1 ){
	  setDomainIncrement(obj1, view_struct, paintBallCurrDomain, 0);
	  WlzFreeObj(obj1);
	  obj1 = NULL;
	}
      }
      else {
	threshObj = WlzAssignObject(view_struct->view_object, NULL);
      }
      if( !threshObj ){
	return;
      }

      /* check smoothing */
      if( widget = XtNameToWidget
	 (globals.topl,
	  "*tool_control_dialog*threshold_form1.smoothed_threshold") ){
	XtVaGetValues(widget, XmNset, &smoothFlg, NULL);
      }
      else {
	smoothFlg = False;
      }
      if( smoothFlg ){
	/* maybe use the recursive filter here */
	obj1 = WlzGauss2(threshObj, thresh_smooth_size, thresh_smooth_size,
			 0, 0, NULL);
	WlzFreeObj(threshObj);
	threshObj = WlzAssignObject(obj1, NULL);
      }

      /* calculate the first domain and increment */
      x = (int) (thresholdInitialX / wlzViewStr->scale) +
	view_struct->painted_object->domain.i->kol1;
      y = (int) (thresholdInitialY / wlzViewStr->scale) +
	view_struct->painted_object->domain.i->line1;
      obj = get_thresh_obj(
	threshObj, x, y,
	thresh_range_size, thresh_range_size);
      if( obj ){
	prevObj = WlzAssignObject(obj, NULL);
	setDomainIncrement(obj, view_struct, paintBallCurrDomain,
			   paintBallDelFlag);
      }
      else {
	prevObj = NULL;
      }
      break;

    default:
      break;

    }
    break;

  case ButtonRelease:
    switch( cbs->event->xbutton.button ){
    case Button1:
    case Button2:
      paintBallTrigger = 0;
      view_struct->viewLockedFlag = 0;
      if( threshObj ){
	WlzFreeObj(threshObj);
	threshObj = NULL;
      }
      break;

    default:
      break;
    }

    /* reset the threshold range */
    thresh_range_size = initial_thresh_range_size;

    /* clear the previous object */
    if( prevObj ){
      WlzFreeObj(prevObj);
      prevObj = NULL;
    }
    break;

  case MotionNotify:
    /* only act if button1 or button2 is pressed */
    if((paintBallTrigger) &&
       (cbs->event->xmotion.state & (Button1Mask|Button2Mask)) ){
      /* set the threshold range */
      thresh_range_size = initial_thresh_range_size + 
	(cbs->event->xmotion.x - thresholdInitialX) / 4;
      thresh_range_size = (thresh_range_size < 0) ? 0 : thresh_range_size;

      /* calculate the domain increment */
      x = (int) (thresholdInitialX / wlzViewStr->scale) +
	wlzViewStr->minvals.vtX;
      y = (int) (thresholdInitialY / wlzViewStr->scale) +
	wlzViewStr->minvals.vtY;
      obj = get_thresh_obj(
	threshObj, x, y,
	thresh_range_size, thresh_range_size);
      if( obj ){
	obj = WlzAssignObject(obj, NULL);
	if( prevObj && (obj1 = WlzDiffDomain(prevObj, obj, NULL)) ){
	  obj1 = WlzAssignObject(obj1, NULL);
	  if( WlzArea(obj1, NULL) < 1 ){
	    WlzFreeObj(obj1);
	    obj1 = NULL;
	  }
	}
	else {
	  obj1 = NULL;
	}
	if( obj1 ){
	  setDomainIncrement(obj1, view_struct, paintBallCurrDomain,
			     !paintBallDelFlag);
	  WlzFreeObj(obj1);
	}
	else {
	  setDomainIncrement(obj, view_struct, paintBallCurrDomain,
			     paintBallDelFlag);
	}
	if( prevObj ){
	  WlzFreeObj(prevObj);
	}
	prevObj = obj;
      }
    }
    break;

  case KeyPress:
    switch( XLookupKeysym(&(cbs->event->xkey), 0) ){
	
    case XK_Right:
    case XK_f:
      /* if <alt> or <alt gr> pressed then change tool option */
      if( cbs->event->xkey.state&(Mod1Mask|Mod2Mask) ){
	changeThreshRange(0);
      }
      else {
	/* maybe resetting the view */
	/* clear the grey-value workspace */
	WlzGreyValueFreeWSp(threshold_gVWSp);
	threshold_gVWSp = NULL;
      }
      break;

    case XK_Up:
    case XK_p:
      break;

    case XK_Left:
    case XK_b:
      /* if <alt> or <alt gr> pressed then change tool option */
      if( cbs->event->xkey.state&(Mod1Mask|Mod2Mask) ){
	changeThreshRange(1);
      }
      else {
	/* maybe resetting the view */
	/* clear the grey-value workspace */
	WlzGreyValueFreeWSp(threshold_gVWSp);
	threshold_gVWSp = NULL;
      }
      break;

    case XK_Down:
    case XK_n:
      break;
    }
    break;

  default:
    break;
  }

  return;
}


Widget	CreateDrawPaintBallControls(
  Widget	parent)
{
  Widget	form, form1, frame, label, widget;
  Widget	option_menu, toggle, slider, scale;
  float		val, minval, maxval;

  /* create a parent form to hold all the tracking controls */
  form = XtVaCreateWidget("paint_draw_controls_form", xmFormWidgetClass,
			  parent,
			  XmNtopAttachment,     XmATTACH_FORM,
			  XmNbottomAttachment,	XmATTACH_FORM,
			  XmNleftAttachment,    XmATTACH_FORM,
			  XmNrightAttachment,  	XmATTACH_FORM,
			  XmNhorizontalSpacing,	0,
			  XmNverticalSpacing,	0,
			  NULL);

  /* create frame, form and label for the tracking parameters */
  frame = XtVaCreateManagedWidget("frame", xmFrameWidgetClass,
				  form,
				  XmNtopAttachment,     XmATTACH_FORM,
				  XmNleftAttachment,    XmATTACH_FORM,
				  XmNrightAttachment,  	XmATTACH_FORM,
				  NULL);

  form1 = XtVaCreateWidget("paint_draw_form1", xmFormWidgetClass,
			   frame,
			   XmNtopAttachment,    XmATTACH_FORM,
			   XmNbottomAttachment,	XmATTACH_FORM,
			   XmNleftAttachment,   XmATTACH_FORM,
			   XmNrightAttachment,  XmATTACH_FORM,
			   NULL);

  label = XtVaCreateManagedWidget("paint_draw_params", xmLabelWidgetClass,
				  frame,
				  XmNchildType,		XmFRAME_TITLE_CHILD,
				  XmNborderWidth,	0,
				  NULL);

  /* create an option menu for the draw cursor type */
  option_menu = HGU_XmBuildMenu( form1, XmMENU_OPTION, "cursor", 0,
				  XmTEAR_OFF_DISABLED, draw_cursor_items);

  XtVaSetValues(option_menu,
		XmNtopAttachment,	XmATTACH_FORM,
		XmNleftAttachment,	XmATTACH_FORM,
		NULL);
  widget = option_menu;
  XtManageChild( option_menu );

  /* create an option menu for the paint shape type */
  option_menu = HGU_XmBuildMenu( form1, XmMENU_OPTION, "paint_shape", 0,
				XmTEAR_OFF_DISABLED, paint_shape_items);

  XtVaSetValues(option_menu,
		XmNtopAttachment,	XmATTACH_WIDGET,
		XmNtopWidget,		widget,
		XmNleftAttachment,	XmATTACH_FORM,
		NULL);
  widget = option_menu;
  XtManageChild( option_menu );

  /* toggle for the paint border */
  toggle = XtVaCreateManagedWidget("paint_border",
				   xmToggleButtonGadgetClass,
				    form1,
				    XmNtopAttachment,	XmATTACH_WIDGET,
				    XmNtopWidget,	widget,
				    XmNleftAttachment,	XmATTACH_FORM,
				    NULL);
  XtAddCallback(toggle, XmNvalueChangedCallback, paint_border_cb, NULL);
  widget = toggle;

  /* slider for the paint blob-size */
  val = paint_blob_size;
  minval = 1.0;
  maxval = 15.0;
  paint_blob_slider = HGU_XmCreateHorizontalSlider("blob_size", form1,
					       val, minval, maxval, 0,
					       paint_size_cb, NULL);
  XtVaSetValues(paint_blob_slider,
		XmNtopAttachment,	XmATTACH_WIDGET,
		XmNtopWidget,		widget,
		XmNleftAttachment,	XmATTACH_FORM,
		XmNrightAttachment,	XmATTACH_FORM,
		NULL);
  scale = XtNameToWidget(paint_blob_slider, "*scale");
  XtAddCallback(scale, XmNdragCallback, paint_size_cb, NULL);

  XtManageChild( form1 );
  draw_paint_controls = form;

  return( form );
}

Widget	CreateThresholdControls(
  Widget	parent)
{
  Widget	form, form1, frame, label, widget, option_menu, slider;
  Widget	toggle;
  float		val, minval, maxval;

  /* create a parent form to hold all the tracking controls */
  form = XtVaCreateWidget("threshold_controls_form", xmFormWidgetClass,
			  parent,
			  XmNtopAttachment,     XmATTACH_FORM,
			  XmNbottomAttachment,	XmATTACH_FORM,
			  XmNleftAttachment,    XmATTACH_FORM,
			  XmNrightAttachment,  	XmATTACH_FORM,
			  XmNhorizontalSpacing,	0,
			  XmNverticalSpacing,	0,
			  NULL);

  /* create frame, form and label for the tracking parameters */
  frame = XtVaCreateManagedWidget("frame", xmFrameWidgetClass,
				  form,
				  XmNtopAttachment,     XmATTACH_FORM,
				  XmNleftAttachment,    XmATTACH_FORM,
				  XmNrightAttachment,  	XmATTACH_FORM,
				  NULL);

  form1 = XtVaCreateWidget("threshold_form1", xmFormWidgetClass,
			  frame,
			  XmNtopAttachment,     XmATTACH_FORM,
			  XmNbottomAttachment,	XmATTACH_FORM,
			  XmNleftAttachment,    XmATTACH_FORM,
			  XmNrightAttachment,  	XmATTACH_FORM,
			  NULL);

  label = XtVaCreateManagedWidget("threshold_params", xmLabelWidgetClass,
				  frame,
				  XmNchildType,		XmFRAME_TITLE_CHILD,
				  XmNborderWidth,	0,
				  NULL);

  /* create an option menu for the threshold connectivity type */
  option_menu = HGU_XmBuildMenu( form1, XmMENU_OPTION, "connect", 0,
				  XmTEAR_OFF_DISABLED, thresh_conn_items);

  XtVaSetValues(option_menu,
		XmNtopAttachment,	XmATTACH_FORM,
		XmNleftAttachment,	XmATTACH_FORM,
		NULL);
  widget = option_menu;
  XtManageChild( option_menu );

  /* create slider for the initial threshold range */
  val = thresh_range_size;
  minval = 0.0;
  maxval = 64.0;
  thresh_slider = HGU_XmCreateHorizontalSlider("range", form1,
					       val, minval, maxval, 0,
					       setThreshRangeCb, NULL);
  XtVaSetValues(thresh_slider,
		XmNtopAttachment,	XmATTACH_WIDGET,
		XmNtopWidget,		widget,
		XmNleftAttachment,	XmATTACH_FORM,
		XmNrightAttachment,	XmATTACH_FORM,
		NULL);
  widget = thresh_slider;

  /* create a toggle for constrained region */
  toggle = XtVaCreateManagedWidget("constrained_threshold",
				   xmToggleButtonGadgetClass, form1,
				   XmNtopAttachment,	XmATTACH_WIDGET,
				   XmNtopWidget,		widget,
				   XmNleftAttachment,	XmATTACH_FORM,
				   XmNset, True,
				   NULL);

  /* create a toggle for smoothing */
  toggle = XtVaCreateManagedWidget("smoothed_threshold",
				   xmToggleButtonGadgetClass, form1,
				   XmNtopAttachment,	XmATTACH_WIDGET,
				   XmNtopWidget,	widget,
				   XmNleftAttachment,	XmATTACH_WIDGET,
				   XmNleftWidget,	toggle,
				   XmNset, False,
				   NULL);
  widget = toggle;

  /* create slider for the initial threshold range */
  val = thresh_smooth_size;
  minval = 0.5;
  maxval = 32.0;
  thresh_slider = HGU_XmCreateHorizontalSlider("gaussian_width", form1,
					       val, minval, maxval, 1,
					       setThreshSmoothWidthCb, NULL);
  XtVaSetValues(thresh_slider,
		XmNtopAttachment,	XmATTACH_WIDGET,
		XmNtopWidget,		widget,
		XmNleftAttachment,	XmATTACH_FORM,
		XmNrightAttachment,	XmATTACH_FORM,
		NULL);

  XtManageChild( form1 );

  return( form );
}
