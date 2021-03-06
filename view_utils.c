#if defined(__GNUC__)
#ident "University of Edinburgh $Id$"
#else
static char _view_utils_c[] = "University of Edinburgh $Id$";
#endif
/*!
* \file         view_utils.c
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
#include <sys/types.h>
#include <math.h>

#include <MAPaint.h>


int init_view_struct(
  ThreeDViewStruct	*view_struct)
{
  XWindowAttributes	win_att;
  Display		*dpy = XtDisplay(view_struct->canvas);
  Window		win = XtWindow(view_struct->canvas);
  unsigned int		widthp, heightp;
  Dimension		win_width, win_height;
  WlzThreeDViewStruct	*wlzViewStr = view_struct->wlzViewStr;
  float			minz, maxz, z;
  Widget		widget;
  WlzErrorNum		errNum=WLZ_ERR_NONE;

  if( XGetWindowAttributes(dpy, win, &win_att) == 0 )
    return( 1 );

  /* initialise the 3D view structure */
  errNum = WlzInit3DViewStruct(wlzViewStr, globals.obj);
  if( errNum != WLZ_ERR_NONE ){
    MAPaintReportWlzError(globals.topl, "init_view_struct", errNum);
    return 1;
  }

  /* set the limits set the slider values here */
  minz = wlzViewStr->minvals.vtZ;
  maxz = wlzViewStr->maxvals.vtZ;
  if( wlzViewStr->dist < wlzViewStr->minvals.vtZ )
    wlzViewStr->dist = wlzViewStr->minvals.vtZ;
  if( wlzViewStr->dist > wlzViewStr->maxvals.vtZ )
    wlzViewStr->dist = wlzViewStr->maxvals.vtZ;
  z = wlzViewStr->dist;

  HGU_XmSetSliderRange( view_struct->slider, minz, maxz );
  HGU_XmSetSliderValue( view_struct->slider, z );

  /* set the zeta slider */
  if((widget = XtNameToWidget(view_struct->dialog, "*.zeta_slider"))){
    HGU_XmSetSliderValue(widget,
			 view_struct->wlzViewStr->zeta * 180.0 / WLZ_M_PI);
  }

  /* set up the ximage - note the internal ximage is 8 bit but we must honour 
     the bitmap-pad for the display */
  widthp  = wlzViewStr->maxvals.vtX - wlzViewStr->minvals.vtX + 1;
  heightp = wlzViewStr->maxvals.vtY - wlzViewStr->minvals.vtY + 1;
  view_struct->ximage = XCreateImage(dpy, win_att.visual, 8,
				     ZPixmap, 0, NULL, widthp,
				     heightp, BitmapPad(dpy), 0);
  if(view_struct->ximage == NULL) {
    MAPaintReportWlzError(globals.topl, "init_Inview_struct",
    			  WLZ_ERR_PARAM_DATA);
    return 1;
  }
  /* resize the canvas */
  win_width = widthp * wlzViewStr->scale;
  win_height = heightp * wlzViewStr->scale;
  XtVaSetValues(view_struct->canvas,
		XmNwidth, win_width,
		XmNheight, win_height,
		NULL);

  view_struct->display_list = 0;
  return( 0 );
}

int reset_view_struct(
  ThreeDViewStruct	*view_struct)
{
  unsigned int	widthp, heightp;
  Dimension		win_width, win_height;
  WlzThreeDViewStruct	*wlzViewStr = view_struct->wlzViewStr;
  float		minz, maxz, z;
  WlzErrorNum		errNum=WLZ_ERR_NONE;

  /* initialise the 3D view structure */
  errNum = WlzInit3DViewStruct(wlzViewStr, globals.obj);
  if( errNum != WLZ_ERR_NONE ){
    MAPaintReportWlzError(globals.topl, "reset_view_struct", errNum);
    return 1;
  }

  /* set the limits set the slider values here -
     may need to increment distance */
  minz = wlzViewStr->minvals.vtZ;
  maxz = wlzViewStr->maxvals.vtZ;
  z = wlzViewStr->dist;
  if( wlzViewStr->dist < wlzViewStr->minvals.vtZ ){
    z = wlzViewStr->minvals.vtZ;
  }
  if( wlzViewStr->dist > wlzViewStr->maxvals.vtZ ){
    z = wlzViewStr->maxvals.vtZ;
  }
  if( z != wlzViewStr->dist ){
    Wlz3DSectionIncrementDistance(wlzViewStr, z - wlzViewStr->dist);
    z = wlzViewStr->dist;
  }

  HGU_XmSetSliderRange( view_struct->slider, minz, maxz );
  HGU_XmSetSliderValue( view_struct->slider, z );

  /* resize the canvas */
  widthp  = wlzViewStr->maxvals.vtX - wlzViewStr->minvals.vtX + 1;
  heightp = wlzViewStr->maxvals.vtY - wlzViewStr->minvals.vtY + 1;
  win_width = widthp * wlzViewStr->scale;
  win_height = heightp * wlzViewStr->scale;
  XtVaSetValues(view_struct->canvas,
		XmNwidth, win_width,
		XmNheight, win_height,
		NULL);

  /* reset the dialog title */
  set_view_dialog_title(view_struct->dialog, wlzViewStr->theta,
			wlzViewStr->phi);

  /* reset the ximage */
  view_struct->ximage->width = widthp;
  view_struct->ximage->height = heightp;
  view_struct->ximage->bytes_per_line = widthp;

  /* tablet needs re-initialising if in use */
  view_struct->tablet_initialised = 0;

  /* reset the view object */
  if( (errNum == WLZ_ERR_NONE) && (view_struct->view_object != NULL) )
  {
    errNum = WlzFreeObj( view_struct->view_object );
    view_struct->view_object = NULL;
  }

  /* free the painted and masked objects */
  if( (errNum == WLZ_ERR_NONE) && (view_struct->painted_object != NULL) )
  {
    errNum = WlzFreeObj( view_struct->painted_object );
    view_struct->painted_object = NULL;
    errNum = WlzFreeObj( view_struct->masked_object );
    view_struct->masked_object = NULL;
  }

  if( errNum != WLZ_ERR_NONE ){
    MAPaintReportWlzError(globals.topl, "reset_view_struct", errNum);
    return 1;
  }
  return( 0 );
}

int free_view_struct(
  ThreeDViewStruct	*view_struct)
{
  WlzErrorNum		errNum=WLZ_ERR_NONE;

  errNum = WlzFree3DViewStruct(view_struct->wlzViewStr);

  if( errNum == WLZ_ERR_NONE ){
    view_struct->ximage->data = NULL;
    XDestroyImage(view_struct->ximage);

    glDeleteLists(view_struct->display_list, 1);
    MAOpenGLDrawScene( globals.canvas );
  }

  /* check the view, painted and masked objects */
  if( (errNum == WLZ_ERR_NONE) && (view_struct->view_object != NULL) )
  {
    errNum = WlzFreeObj( view_struct->view_object );
    view_struct->view_object = NULL;
  }

  if( (errNum == WLZ_ERR_NONE) && (view_struct->painted_object != NULL) )
  {
    errNum = WlzFreeObj( view_struct->painted_object );
    view_struct->painted_object = NULL;
  } 
 
  if( (errNum == WLZ_ERR_NONE) && (view_struct->masked_object != NULL) )
  {
    errNum = WlzFreeObj( view_struct->masked_object );
    view_struct->masked_object = NULL;
  }  

  if( (errNum == WLZ_ERR_NONE) && (view_struct->prev_view_obj != NULL) )
  {
    errNum = WlzFreeObj( view_struct->prev_view_obj );
    view_struct->prev_view_obj = NULL;
  }  

  if( errNum != WLZ_ERR_NONE ){
    MAPaintReportWlzError(globals.topl, "reset_view_struct", errNum);
    return 1;
  }
  return( 0 );
}

void getViewDomains(
  ThreeDViewStruct	*view_struct)
{
  int		i, j, numOverlays;
  WlzObject	*allDomainsObj=NULL, *tmpObj;
  WlzPixelV	thresh;
  WlzErrorNum		errNum=WLZ_ERR_NONE;

  /* check initialisation */
  if( !view_struct->wlzViewStr->initialised ){
    if( init_view_struct( view_struct ) ){
      errNum = WLZ_ERR_UNSPECIFIED;
    }
  }

  /* check for painted object */

  /* clear any current domains */
  numOverlays = globals.cmapstruct->num_overlays +
    globals.cmapstruct->num_solid_overlays;
  for(i=0; i <= numOverlays; i++){
    if( view_struct->curr_domain[i] ){
      errNum = WlzFreeObj(view_struct->curr_domain[i]);
      view_struct->curr_domain[i] = NULL;
    }
    if( errNum != WLZ_ERR_NONE ){
      break;
    }
  }

  /* get the union of all domains */
  if( errNum == WLZ_ERR_NONE ){
    thresh.type = WLZ_GREY_INT;
    thresh.v.inv = globals.cmapstruct->ovly_cols[DOMAIN_1];
    tmpObj = WlzThreshold(view_struct->painted_object,
			  thresh, WLZ_THRESH_HIGH, &errNum);
    if( (errNum == WLZ_ERR_NONE) && tmpObj ){
      if( WlzArea(tmpObj, &errNum) > 0 ){
	allDomainsObj = WlzAssignObject(tmpObj, &errNum);
      }
      else {
	errNum = WlzFreeObj(tmpObj);
      }
    }
  }

  /* segment the painted section image to establish each domain object */
  if( (errNum == WLZ_ERR_NONE) && allDomainsObj ){
    WlzPixelV	min, max;
    int		max_i=numOverlays;

    /* get maximum number of domains */
    WlzGreyRange(allDomainsObj, &min, &max);
    WlzValueConvertPixel(&max, max, WLZ_GREY_INT);

    while( (max_i > 0) && (globals.cmapstruct->ovly_cols[max_i] > max.v.inv) ){
      max_i--;
    }

    for(i=1; i <= max_i; i++){
      tmpObj = get_domain_from_object(view_struct->painted_object, i);
      if( tmpObj ){
	view_struct->curr_domain[i] = WlzAssignObject(tmpObj, NULL);
      }
    }
  }

  /* propogate from previous section 
     asking for confirmation if a domain will be changed by
     propogation.
     This will either be if the current domain exists or if the 
     propogating domain overlaps the union of all existing domains */
  if( (errNum == WLZ_ERR_NONE) && (globals.propogate||globals.propogate_sel) ){
    for(i=1; (i <= numOverlays) && (errNum == WLZ_ERR_NONE); i++){
      /* test for propogate currently selected domain only */
      if((!globals.propogate) && (i != globals.current_domain)){
	continue;
      }
      if((view_struct->prev_domain[i]) &&
	 (view_struct->prev_domain[i]->type != WLZ_EMPTY_OBJ)){
	if( allDomainsObj ){
	  /* first check if there is a curr_domain */
	  int needConfirm = 0;
	  if((view_struct->curr_domain[i]) && 
	     (view_struct->curr_domain[i]->type != WLZ_EMPTY_OBJ)){
	    needConfirm = 1;
	  }
	  else {
	    tmpObj = WlzIntersect2(view_struct->prev_domain[i],
				   allDomainsObj, &errNum);
	    if( tmpObj && (WlzArea(tmpObj, &errNum) > 0) ){
	      needConfirm = 1;
	      WlzFreeObj(tmpObj);
	    }
	  }
	  if( (errNum == WLZ_ERR_NONE) && needConfirm ){
	    if( HGU_XmUserConfirm(view_struct->dialog,
				  "Warning: propogation may modify\n"
				  "    one or more existing domains.\n"
				  "    Propogate anyway?",
				  "Yes", "No", 0) ){
	      if( view_struct->curr_domain[i] ){
		tmpObj = WlzUnion2(view_struct->curr_domain[i],
				   view_struct->prev_domain[i], &errNum);
		WlzFreeObj(view_struct->curr_domain[i]);
		view_struct->curr_domain[i] = 
		  WlzAssignObject(tmpObj, NULL);
	      }
	      else {
		view_struct->curr_domain[i] =
		  WlzAssignObject(view_struct->prev_domain[i], NULL);
	      }
	      globals.domain_changed_since_saved[i] = 1;
	    }
	  }
	  else {
	    view_struct->curr_domain[i] =
	      WlzAssignObject(view_struct->prev_domain[i], NULL);
	    globals.domain_changed_since_saved[i] = 1;
	  }
	}
	else {
	  view_struct->curr_domain[i] =
	    WlzAssignObject(view_struct->prev_domain[i], &errNum);
	  globals.domain_changed_since_saved[i] = 1;
	}
      }
    }
  }

  /* put all objects onto painted image in reverse dominance order */
  for(j=numOverlays; (j > 0) && (errNum == WLZ_ERR_NONE); j--){
    i = globals.priority_to_domain_lut[j];
    if( view_struct->curr_domain[i] ){
      if( i > globals.cmapstruct->num_overlays )
      {
	if( set_grey_values_from_domain(view_struct->curr_domain[i],
					view_struct->painted_object,
					globals.cmapstruct->ovly_cols[i],
					255) ){
	  break;
	}
      }
      else
      {
	if( set_grey_values_from_domain(view_struct->curr_domain[i],
					view_struct->painted_object,
					globals.cmapstruct->ovly_cols[i],
					globals.cmapstruct->ovly_planes) ){
	  break;
	}
      }
    }
  }      

  /* redisplay the view */
  if( errNum != WLZ_ERR_NONE ){
    MAPaintReportWlzError(globals.topl, "getViewDomains", errNum);
  }
  else{
    redisplay_view_cb(view_struct->canvas, view_struct, NULL);
  }

  return;
}

void installViewDomains(
  ThreeDViewStruct	*view_struct)
{
  WlzThreeDViewStruct	*wlzViewStr= view_struct->wlzViewStr;
  WlzIntervalWSpace	iwsp;
  WlzGreyWSpace		gwsp;
  WlzGreyValueWSpace	*gVWSp = NULL;
  float			x, y, z;
  int			xp, yp;
  int			i;
  ViewListEntry		*vl = global_view_list;
  WlzErrorNum		errNum=WLZ_ERR_NONE;

  /* all domains on this plane will already have been
     checked for consistency, dominance etc. therefore
     simply set the corresponding domains in the painted
     object */
  /* this is the set version of get section and should be in the
     woolz library. This is probably faster because many assumptions
     are made, mainly the grey-type */
  gVWSp = WlzGreyValueMakeWSp(globals.obj, &errNum);
  if( errNum == WLZ_ERR_NONE ){
    errNum = WlzInitGreyScan( view_struct->painted_object, &iwsp, &gwsp );
    while((errNum == WLZ_ERR_NONE) &&
	  ((errNum = WlzNextGreyInterval( &iwsp )) == WLZ_ERR_NONE) )
    {
      yp = iwsp.linpos - WLZ_NINT(wlzViewStr->minvals.vtY);
      for(i=iwsp.lftpos; i <= iwsp.rgtpos; i++){
	xp = i - WLZ_NINT(wlzViewStr->minvals.vtX);
	x = wlzViewStr->xp_to_x[xp] + wlzViewStr->yp_to_x[yp];
	y = wlzViewStr->xp_to_y[xp] + wlzViewStr->yp_to_y[yp];
	z = wlzViewStr->xp_to_z[xp] + wlzViewStr->yp_to_z[yp];

	WlzGreyValueGet(gVWSp, WLZ_NINT(z), WLZ_NINT(y), WLZ_NINT(x));
	*(gVWSp->gPtr[0].ubp) = *(gwsp.u_grintptr.ubp);
	gwsp.u_grintptr.ubp++;
      }
    }
    if( errNum == WLZ_ERR_EOO ){
      errNum = WLZ_ERR_NONE;
    }
    WlzGreyValueFreeWSp(gVWSp);
  }

  /* copy the new domains to the previous domains */
  if( errNum == WLZ_ERR_NONE ){
    for(i=0; i < 33; i++){
      if( view_struct->prev_domain[i] ){
	WlzFreeObj(view_struct->prev_domain[i]);
	view_struct->prev_domain[i] = NULL;
      }
      if( view_struct->curr_domain[i] ){
	view_struct->prev_domain[i] =
	  WlzAssignObject(view_struct->curr_domain[i], NULL);
      }
    }
  }

  /* set the previous distance and view object  - calculate when needed */
  if( errNum == WLZ_ERR_NONE ){
    view_struct->prev_dist = wlzViewStr->dist;
    if( view_struct->prev_view_obj ){
      WlzFreeObj(view_struct->prev_view_obj);
      view_struct->prev_view_obj = NULL;
    }
    if( view_struct->view_object ){
      view_struct->prev_view_obj = WlzAssignObject(view_struct->view_object,
						   NULL);
    }
    else {
      view_struct->prev_view_obj = NULL;
    }
  }
  
  /* display all views to update the new domains */
  if( errNum == WLZ_ERR_NONE ){
    while( vl != NULL ){
      if( vl->view_struct != paint_key ){
	if( vl->view_struct == view_struct ){
	  redisplay_view_cb(view_struct->canvas,
			    (XtPointer) vl->view_struct, NULL);
	}
	else {
	  display_view_cb(view_struct->canvas,
			  (XtPointer) vl->view_struct, NULL);
	}
      }
      vl = vl->next;
    }
  }

  if( errNum != WLZ_ERR_NONE ){
    MAPaintReportWlzError(globals.topl, "installViewDomains", errNum);
  }
  return;
}
