#ifndef MAPAINTRESOURCES_H
#define MAPAINTRESOURCES_H

#pragma ident "MRC HGU $Id$"
/************************************************************************
*   Copyright  :   1994 Medical Research Council, UK.                   *
*                  All rights reserved.                                 *
*************************************************************************
*   Address    :   MRC Human Genetics Unit,                             *
*                  Western General Hospital,                            *
*                  Edinburgh, EH4 2XU, UK.                              *
*************************************************************************
*   Project    :   Mouse Atlas Project					*
*   File       :   MAPaintResources.h					*
*************************************************************************
*   Author Name :  Richard Baldock					*
*   Author Login:  richard@hgu.mrc.ac.uk				*
*   Date        :  Tue Feb 24 14:40:00 1998				*
*   Synopsis    : 							*
*************************************************************************
*   Maintenance :  date - name - comments (Last changes at the top)	*
************************************************************************/

/* the following to be put in an include file */
/* fallback resources  */
static String  fallback_resources[] = {

    /* top-level geometry */
    "MAPaint.geometry:		280x360",

    /* autosave timeout time in seconds */
    "MAPaint.autosaveTime:	900",
    "MAPaint.autosaveDir:	/tmp",
    "MAPaint.autosaveFile:	MAPaintAutosave",

    /* Theiler stage directory for the anatomy menu 
       this default should be changed when delivered */
    "MAPaint.theilerDir: /net/homehost/export/ma/richard/MouseAtlas/src/MAPaint/src/reconstructions",

    /* fonts and backgrounds */
    "MAPaint*background:		grey85",
    "MAPaint*XmScrollBar.background:	grey95",
    "MAPaint*.fontList:	      -linotype-helvetica-bold-r-*-*-12-*-*-*-*-*-*-*",
    "MAPaint*.XmText.fontList:		8x13",
    "MAPaint*.XmTextField.fontList:	8x13",
    "MAPaint*.XmList.fontList:		8x13",
    "MAPaint*.XmText.background:	grey95",
    "MAPaint*.XmTextField.background:	grey95",
    "MAPaint*.XmList.background:	grey95",
    "MAPaint*menubar.XmCascadeButton.marginWidth: 1",

    /* widget and gadget class resources */
    "*XmToggleButtonGadget.selectColor:		red",
    "*XmToggleButtonGadget.visibleWhenOff:	True",
    "*XmToggleButtonGadget.indicatorSize:	14",
    "*XmToggleButton.selectColor:		red",
    "*XmToggleButton.visibleWhenOff:		True",
    "*XmToggleButton.indicatorSize:		14",
    "*XmFrameWidget.shadowThickness:		3",
    "*XmForm.horizontalSpacing:    		4",
    "*XmForm.verticalSpacing:      		4",
    "*XmFrame.borderWidth:  			0",

    /* top level widget */
    "*work_area.width:			320",
    "*work_area.height:			320",
    "*work_area*doublebuffer: 		True",
    "*work_area*rgba: 			False",
    "*work_area*redSize: 		1",
    "*work_area*greenSize: 		1",
    "*work_area*blueSize: 		1",
    "*work_area*alphaSize: 		0",
    "*work_area*allocateBackground: 	True",
    "*work_area*background:	 	black",
    "*work_area*allocateotherColors: 	True",
    "*work_area*installColormap: 	True",


    /* drawing area defaults */
    "MAPaint*HGU_DrawingArea.visual:		PseudoColor",
    "MAPaint*HGU_DrawingArea.depth:		8",
    "MAPaint*HGU_DrawingArea.background:	white",

    /* dialog defaults */

    /* file dialogs */
    "*read_obj_dialog_popup.mappedWhenManaged:		False",
    "*read_obj_dialog.dialogTitle:			Read 3D Object",
    "*read_obj_dialog.pattern:				*.wlz",
    "*read_obj_dialog.file_type.labelString:		Select image type:",
    "*read_obj_dialog.file_type.borderWidth:		0",
    "*write_obj_dialog_popup.mappedWhenManaged:		False",
    "*write_obj_dialog.dialogTitle:		  Write 3D ReferenceObject",
    "*write_obj_dialog.pattern:				*.wlz",
    "*write_obj_dialog.file_type.labelString:		Select image type:",
    "*write_obj_dialog.file_type.borderWidth:		0",
    "*object_props_dialog_popup.mappedWhenManaged:	False",
    "*object_props_dialog.dialogTitle:		  Object Properties",
    "*object_props_dialog*control.horizontalSpacing:    4",
    "*object_props_dialog*control.verticalSpacing:      4",
    "*object_props_dialog*frame1.title1.labelString:	Bounding box",
    "*object_props_dialog*form1*planes.labelString:	Planes (z):  ",
    "*object_props_dialog*form1*lines.labelString:	Lines (y):    ",
    "*object_props_dialog*form1*kols.labelString:	Columns (x):",
    "*object_props_dialog*form1*planes_vals.labelString:(0,100)",
    "*object_props_dialog*form1*lines_vals.labelString:	(0,100)",
    "*object_props_dialog*form1*kols_vals.labelString:	(0,100)",
    "*object_props_dialog*frame2.title2.labelString:	Voxel parameters",
    "*object_props_dialog*form2*background.labelString:	Background:",
    "*object_props_dialog*form2*x_size.labelString:	X size:    ",
    "*object_props_dialog*form2*y_size.labelString:	Y size:    ",
    "*object_props_dialog*form2*z_size.labelString:	Z size:    ",
    "*XmFileSelectionBox*file_type*woolz.labelString: Woolz",
    "*XmFileSelectionBox*file_type*ics.labelString: ICS",
    "*XmFileSelectionBox*file_type*den.labelString: Stanford Density",
    "*XmFileSelectionBox*file_type*vff.labelString: Sunvision VFF",
    "*XmFileSelectionBox*file_type*pic.labelString: BioRad Confocal",
    "*XmFileSelectionBox*file_type*vtk.labelString: Visualization Toolkit VTK",
    "*XmFileSelectionBox*file_type*pgm.labelString: pgm",

    /* domain dialogs */
    "*read_domain_dialog_popup.mappedWhenManaged:	False",
    "*read_domain_dialog.dialogTitle:			Read 3D Domain",
    "*read_domain_dialog.pattern:			*.wlz*",
    "*write_domain_dialog_popup.mappedWhenManaged:	False",
    "*write_domain_dialog.dialogTitle:			Write 3D Domain",
    "*write_domain_dialog.pattern:			*.wlz*",
    "*write_paint_volume_dialog_popup.mappedWhenManaged:	False",
    "*write_paint_volume_dialog.dialogTitle:	Write 3D Painted Volume",
    "*write_paint_volume_dialog.pattern:	*.wlz*",
    "*write_paint_volume_dialog.file_type.labelString:	Select image type:",
    "*write_paint_volume_dialog.file_type.borderWidth:		0",

    /* save-restore resources */
    "*save_state_dialog.pattern:		*.rsc",
    "*restore_state_dialog.pattern:		*.rsc",

    /* file_menu */
    "*file_menu.labelString:                    File",
    "*file_menu.mnemonic:                       F",
    "*file_menu.borderWidth:                    0",
    "*file_menu*new_obj.labelString:		New...",
    "*file_menu*new_obj.mnemonic:		N",
    "*file_menu*read_obj.labelString:		Open reference...",
    "*file_menu*read_obj.mnemonic:		O",
    "*file_menu*write_obj.labelString:		Write reference...",
    "*file_menu*write_obj.mnemonic:		W",
    "*file_menu*obj_props.labelString:		Ref. properties...",
    "*file_menu*obj_props.mnemonic:		P",
    "*file_menu*theiler_stage.labelString:	Theiler stage",
    "*file_menu*theiler_stage.mnemonic:		T",
    "*file_menu*theiler_setup.labelString:	Theiler setup...",
    "*file_menu*save_state.labelString:		Save state",
    "*file_menu*save_state.mnemonic:		S",
    "*file_menu*restore_state.labelString:	Restore",
    "*file_menu*restore_state.mnemonic:		R",

    /* misc menu */
    "*misc_menu.labelString:			Misc",
    "*misc_menu.mnemonic:		        M",
    "*misc_menu*macro.labelString:			Macro",
    "*misc_menu*macro*start_record.labelString:		Start Record",
    "*misc_menu*macro*start_record.mnemonic:		S",
    "*misc_menu*macro*end_record.labelString:		End record",
    "*misc_menu*macro*end_record.mnemonic:		E",
    "*misc_menu*macro*replay.labelString:		Replay",
    "*misc_menu*macro*replay.mnemonic:			R",
    "*misc_menu*macro*loop.labelString:			Loop",
    "*misc_menu*macro*loop.mnemonic:			L",
    "*misc_menu*macro*quit_loop.labelString:		Quit loop",
    "*misc_menu*macro*quit_loop.mnemonic:		Q",
    "*misc_menu*save_restore.labelString:		Save/Restore",
    "*misc_menu*save_state.labelString:			Save State...",
    "*misc_menu*restore_state.labelString:		Restore...",
    "*misc_menu*save_restore*save_state.labelString:	Save State",
    "*misc_menu*save_restore*restore_state.labelString:	Restore",

    /* anatomy_menu */
    "*anatomy_menu.labelString:                 Anatomy",
    "*anatomy_menu.mnemonic:                    A",

    /* domain menu */
    "*domain_menu.labelString:			Domain",
    "*domain_menu.mnemonic:			D",
    "*domain_menu.borderWidth:			0",
    "*domain_menu*select.labelString:		Select",
    "*domain_menu*select.mnemonic:		S",
    "*domain_menu*select.borderWidth:		0",
    "*domain_menu*controls.labelString:		Controls...",
    "*domain_menu*controls.mnemonic:		C",
    "*domain_menu*auto_increment.labelString:	Auto-increment",
    "*domain_menu*auto_increment.mnemonic:	A",
    "*domain_menu*auto_increment.borderwidth:	0",
    "*domain_menu*propogate.labelString:	Propogate",
    "*domain_menu*propogate.mnemonic:		P",
    "*domain_menu*propogate.borderwidth:	0",
    "*domain_menu*read_domain.labelString:	Read domain...",
    "*domain_menu*read_domain.mnemonic:		R",
    "*domain_menu*write_domain.labelString:	Write domain...",
    "*domain_menu*write_domain.mnemonic:	W",
    "*domain_menu*save_domain.labelString:	Save domain...",
    "*domain_menu*save_domain.mnemonic:		a",
    "*domain_menu*clear_domain.labelString:	Clear domain",
    "*domain_menu*clear_domain.mnemonic:	l",
    "*domain_menu*clear_all_domains.labelString:	Clear all domains",
    "*domain_menu*threed_display_domain.labelString:	Display 3D domain",
    "*domain_menu*threed_display_domain.mnemonic:	3",
    "*domain_menu*write_paint_volume.labelString:	Write painted volume...",
    "*domain_menu*write_paint_volume.mnemonic:	I",

    /* domain controls dialog */
    "*domain_controls_dialog_popup.mappedWhenManaged:	False",
    "*domain_controls_dialog.dialogTitle:		Domain controls",
    "*domain_controls_dialog*dominance.labelString:	Domain Dominance:",
    "*domain_controls_dialog*control.horizontalSpacing:	5",
    "*domain_controls_dialog*control.verticalSpacing:	5",
    "*domain_controls_dialog*dominance_frame.borderWidth: 0",
    "*domain_controls_dialog*dominance_form.borderWidth: 0",
    "*domain_controls_dialog*domain_1.labelString: 1",
    "*domain_controls_dialog*domain_2.labelString: 2",
    "*domain_controls_dialog*domain_3.labelString: 3",
    "*domain_controls_dialog*domain_4.labelString: 4",
    "*domain_controls_dialog*domain_5.labelString: 5",

    /* select radio menu */
    "*domain_menu*select*borderWidth:		0",
    
    /* view menu */
    "*view_menu.labelString:			View",
    "*view_menu.mnemonic:			V",
    "*view_menu.borderWidth:			0",
    "*view_menu*x_y_view.labelString:		X-Y view...",
    "*view_menu*x_y_view:mnemonic		X",
    "*view_menu*y_z_view.labelString:		Y-Z view...",
    "*view_menu*y_z_view:mnemonic		Y",
    "*view_menu*z_x_view.labelString:		Z-X view...",
    "*view_menu*z_x_view:mnemonic		Z",
    "*view_menu*arbitrary_view.labelString:	Arbitrary...",
    "*view_menu*arbitrary_view:mnemonic		A",

    /* view dialog */
    "*view_dialog*save_section.labelString:	Save",
    "*view_dialog*dismiss.labelString:		Dismiss",
    "*view_dialog*help.labelString:		Help",
    "*view_dialog*XmForm.horizontalSpacing:    	4",
    "*view_dialog*XmForm.verticalSpacing:      	4",
    "*view_dialog*XmFrame.borderWidth:  	0",
    "*view_dialog*scrolled_window.borderWidth:	3",
    "*view_dialog*section_title_form.verticalSpacing:	0",
    "*view_dialog*section_title_form.borderWidth:	0",
    "*view_dialog*section_frame_title.labelString:	Section:    ",
    "*view_dialog*section_frame_FB_toggle.labelString:	filled 3D view",
    "*view_dialog*section_frame_FB_toggle.highlightThickness:	0",
    "*view_dialog*section_frame_direction_toggle.labelString: view direction",
    "*view_dialog*canvas.borderWidth:		0",
    "*view_dialog*distance_slider.labelString:	Distance",
    "*view_dialog*controls_frame_title.labelString:	Controls",
    "*view_dialog*view_scale.labelString:	Scale:",
    "*view_dialog*view_mode.labelString:	View Mode:",
    "*view_dialog*theta_slider.labelString:	Yaw     ",
    "*view_dialog*phi_slider.labelString:	Pitch     ",
    "*view_dialog*zeta_slider.labelString:	Roll      ",
    "*view_dialog*psi_slider.labelString:	Line Rot.",
    "*view_dialog*up_test.labelString:		Up Vector",
    "*view_dialog*fixed_1.labelString:		Fixed Point",
    "*view_dialog*fixed_2.labelString:		Fixed Line",
    "*view_dialog*up_vector.labelString:	Up Vector",
    "*view_dialog*save_settings.labelString:	I/O",
    "*view_dialog*graphical_f1.labelString:	Point select",
    "*view_dialog*textual_f1.labelString:	Type in coords",
    "*view_dialog*fiducial_f1.labelString:	Select fiducial",
    "*view_dialog*display_f1.labelString:	Display",
    "*view_dialog*graphical_f2.labelString:	Point define",
    "*view_dialog*textual_f2.labelString:	Type in coords",
    "*view_dialog*fiducial_f2.labelString:	Select fiducials",
    "*view_dialog*display_f2.labelString:	Display",
    "*view_dialog*graphical_uv.labelString:	Point define",
    "*view_dialog*textual_uv.labelString:	Type in coords",
    "*view_dialog*fiducial_uv.labelString:	Select fiducials",
    "*view_dialog*display_uv.labelString:	Display",
    "*view_dialog*controls_write.labelString:	Write",
    "*view_dialog*controls_read.labelString:	Read",
    "*view_dialog*special_coords_write.labelString:	Special",
    "*view_dialog*special_coords_write.sensitive:	False",
    "*view_dialog*special_conf_display.labelString:	Conformal",
    "*view_dialog*special_conf_display.sensitive:	False",

    /* option menu */
    "*options_menu.labelString:			Options",
    "*options_menu.mnemonic:		        O",
    "*options_menu.borderWidth:			0",
    "*options_menu*paint_tools_2D.labelString:	Paint tools (2D)",
    "*options_menu*paint_tools_3D.labelString:	Paint tools (3D)",
    "*options_menu*tool_controls.labelString:	Tool controls...",
    "*options_menu*tool_controls.mnemonic:	T",
    "*options_menu*domain_review.labelString:	Domain review...",
    "*options_menu*domain_review.mnemonic:	D",
    "*options_menu*domain_surgery.labelString:	Domain surgery...",
    "*options_menu*domain_surgery.mnemonic:	S",
    "*options_menu*realignment.labelString:	Realignment...",
    "*options_menu*realignment.mnemonic:	R",
    "*options_menu*realignment.sensitive:	False",
    "*options_menu*auto_segment.labelString:	Auto segment...",
    "*options_menu*auto_segment..mnemonic:	A",
    "*options_menu*colormap.labelString:	Colormap...",
    "*options_menu*colormap.mnemonic:		C",
    "*options_menu*autosave_opts.labelString:	Autosave...",
    "*options_menu*autosave_opts.mnemonic:	A",

    /* colormap dialog */
    "*colormap_dialog_popup.mappedWhenManaged:	False",
    "*colormap_dialog.dialogTitle:		Colormap controls",
    "*colormap_dialog*form.borderWidth:		0",
    "*colormap_dialog*overlay.borderWidth:	0",
    "*colormap_dialog*overlay.labelString:	Select overlay:",
    "*colormap_dialog*gamma.labelString:	Gamma: ",
    "*colormap_dialog*low_thresh.labelString:	Low:      ",
    "*colormap_dialog*high_thresh.labelString:	High:     ",
    "*colormap_dialog*red.labelString:		Red:       ",
    "*colormap_dialog*green.labelString:	Green:   ",
    "*colormap_dialog*blue.labelString:		Blue:      ",
    "*colormap_dialog*contrast.labelString:	Contrast:",
    "*gamma:					1.0",
    "*invert:					0",
    "*numSolidOverlays:				5",
    "*numWashOverlays:				5",

    /* autosave dialog */
    "*autosave_dialog_popup.mappedWhenManaged:	False",
    "*autosave_dialog.dialogTitle:		Autosave controls",
    "*autosave_dialog*autosave_time.labelString:Autosave time(sec):    ",
    "*autosave_dialog*autosave_file.labelString:Autosave save file:    ",
    "*autosave_dialog*autosave_recover_file.labelString:Autosave recover file:",
    "*autosave_domains_dialog.dialogTitle:     	Autosave recover domains",

    /* tool controls dialog */
    "*tool_control_dialog.dialogTitle:		   Painting Tool Controls",
    "*tool_control_dialog*XmForm.borderWidth:	   0",

    "*tracking_controls_form*XmFrame.borderWidth:  	0",
    "*tracking_parameters.labelString:			Tracking Parameters:",
    "*tracking_controls_form*spacing.labelString:	Spacing:",
    "*tracking_controls_form*range.labelString:		Range:  ",
    "*tracking_controls_form*size.labelString:		Size:  ",
    "*snake_cost_parameters.labelString:	Snake Cost Parameters:",
    "*image_cost_parameters.labelString:	Image Cost Parameters:",
    "*tracking_controls_form*nu_dist.labelString:	Distance: ",
    "*tracking_controls_form*nu_alpha.labelString:	Angle:    ",
    "*tracking_controls_form*nu_kappa.labelString:	Curvature:",

    "*edge_tracking_controls_form*XmFrame.borderWidth:  	0",
    "*tracking_parameters.labelString:			Tracking Parameters:",
    "*edge_tracking_controls_form*spacing.labelString:	Spacing:",
    "*edge_tracking_controls_form*range.labelString:		Range:  ",
    "*edge_tracking_controls_form*size.labelString:		Width:  ",
    "*snake_cost_parameters.labelString:	Snake Cost Parameters:",
    "*image_cost_parameters.labelString:	Image Cost Parameters:",
    "*edge_tracking_controls_form*nu_dist.labelString:	Distance: ",
    "*edge_tracking_controls_form*nu_alpha.labelString:	Angle:    ",
    "*edge_tracking_controls_form*nu_kappa.labelString:	Curvature:",
    "*edge_tracking_controls_form*nu_direction.labelString:	Direction:",

    "*tablet_ref_points.labelString:		 Tablet Reference Points:",
    "*tablet_controls_form*x1.labelString:		X1:",
    "*tablet_controls_form*y1.labelString:		Y1:",
    "*tablet_controls_form*x2.labelString:		X2:",
    "*tablet_controls_form*y2.labelString:		Y2:",

    "*paint_draw_params.labelString:		 Draw/Paint Parameters:",
    "*paint_draw_controls_form*cursor.labelString:	Cursor Type:",
    "*paint_draw_controls_form*paint_shape.labelString:	Paint Shape:",
    "*paint_draw_controls_form*paint_border.labelString:Paint Border",
    "*paint_draw_controls_form*blob_size.labelString:	Paint size:",

    "*geometry_params.labelString:		 	2D Geometry Parameters:",
    "*geometry_controls_form*geom_type.labelString:	Object type:",
    "*geometry_controls_form*object_size.labelString:	Object size:",
    "*geometry_controls_form*fixed_circle.labelString:	Fixed circle",
    "*geometry_controls_form*fixed_square.labelString:	Fixed square",
    "*geometry_controls_form*variable_circle.labelString: Interactive circle",
    "*geometry_controls_form*variable_square.labelString: Interactive square",

    "*threshold_params.labelString:		 Threshold Parameters:",
    "*threshold_controls_form*connect.labelString:	Connectivity:",
    "*threshold_controls_form*range.labelString:	Initial range:",

    "*morph_parameters.labelString:		 Morphological Operator Params:",
    "*morph_controls_form*struct_elem.labelString:	SE type:",
    "*morph_controls_form*size.labelString:		SE size:",

    "*affine_options.labelString:		 Affine Transform Options:",

    /* domain review tools dialog */
    "*review_dialog_popup.mappedWhenManaged:		False",
    "*review_dialog.dialogTitle:		     Domain Review Controls",
    "*review_dialog*src_domain_title.labelString: 	Source domain:",
    "*review_dialog*domainSource.labelString: 		Source:",
    "*review_dialog*domainSource*review_domain.labelString:  Review domain",
    "*review_dialog*domainSource*mapaint_domain.labelString: MAPaint domain",
    "*review_dialog*domainSource*read_domain.labelString:    Read domain",
    "*review_dialog*src_domain.labelString: 		MAPaint domain:",
    "*review_dialog*domain_review_read.labelString: 	Read",
    "*review_dialog*dest_domain_title.labelString: 	Destination domain:",
    "*review_dialog*Discard.labelString:		Discard",
    "*review_dialog*Greys.labelString:			Greys",
    "*review_dialog*review_planes_title.labelString:	Review planes:",
    "*review_dialog*review_scan_control_title.labelString: Scan control:",
    "*review_dialog*review_scale.labelString:		Review scale:",
    "*review_dialog*segment_domain.labelString:		Segment domain",
    "*review_dialog*segment_domain.set:			True",
    "*review_dialog*segment_planes.labelString:		Segment planes",
    "*review_dialog*segment_planes.set:			True",
    "*read_review_obj_dialog.dialogTitle:		Read Review Object",
    "*read_review_obj_dialog.pattern:			*.wlz",

    /* surgery dialog */
    "*surgery_dialog_popup.mappedWhenManaged:	False",
    "*surgery_dialog.dialogTitle:	       	Domain Surgery Controls",
    "*surgery_dialog*src_domain_title.labelString: 	Source domain:",
    "*surgery_dialog*src_domain.labelString: 	MAPaint domain:",
    "*surgery_dialog*surgery_option.labelString: 	Surgery option:",
    "*surgery_dialog*dest_domain_title.labelString: 	Destination domain:",
    "*surgery_dialog*Discard.labelString:	Discard",
    "*surgery_dialog*Greys.labelString:		Greys",

    /* realignment dialog */
    "*view_dialog*realignment_frame_title.labelString:	Realignment controls",
    "*view_dialog*poly_select.labelString:	Select poly:",
    "*view_dialog*src_dest_select.src_poly.labelString:	Source",
    "*view_dialog*src_dest_select.dst_poly.labelString:	Destination",
    "*view_dialog*poly_mode_select_label.labelString: Mode:",
    "*view_dialog*poly_mode_select.polyline.labelString:	Polyline",
    "*view_dialog*poly_mode_select.straight_line.labelString:	Str. Line",

    /* help_menu */
    "*helpDefaultUrl:		file:/net/glenturret/export/data0/richard/MouseAtlas/Docs/paint_help/paint_help.html",
    "*helpIndexUrl:		paint.1.0/paint.html#index",
    "*helpVersionUrl:		paint.1.0/paint.html#version",
    "*helpWWWServer:		file:",
    "*helpDir:/net/glenturret/export/data0/richard/MouseAtlas/Docs/paint_help",
    "*help_menu.labelString:            	Help",
    "*help_menu.mnemonic:               	H",
    "*help_menu.borderWidth:            	0",
    "*help_menu*on_program.labelString:		Paint",
    "*help_menu*on_context.mnemonic:		P",
    "*help_menu*on_context.labelString:		On context",
    "*help_menu*on_context.mnemonic:		c",
    "*help_menu*on_windows.labelString:		On windows",
    "*help_menu*on_windows.mnemonic:		w",
    "*help_menu*on_keys.labelString:		On keys",
    "*help_menu*on_keys.mnemonic:		k",
    "*help_menu*on_keys.sensitive:		False",
    "*help_menu*index.labelString:		Index",
    "*help_menu*index.mnemonic:			I",
    "*help_menu*on_help.labelString:		On help",
    "*help_menu*on_help.mnemonic:		h",
    "*help_menu*tutorial.labelString:		Tutorial",
    "*help_menu*tutorial.mnemonic:		T",
    "*help_menu*tutorial.sensitive:		False",
    "*help_menu*version.labelString:		Version",
    "*help_menu*version.mnemonic:		V",
 
    NULL,
};


/* do not add anything after this line */
#endif /* MAPAINTRESOURCES_H */
