#ifndef HGU_TABLET_H
#define HGU_TABLET_H

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
*   File       :   HGU_Tablet.h						*
*************************************************************************
*   Author Name :  Richard Baldock					*
*   Author Login:  richard@hgu.mrc.ac.uk				*
*   Date        :  Thu Apr  9 09:15:36 1998				*
*   Synopsis    : 							*
*************************************************************************
*   Maintenance :  date - name - comments (Last changes at the top)	*
************************************************************************/

/* data masks for decoding the Wacom IVe byte stream */
#define WACOM_IV_FIRST_BYTE	(0x1<<7)
#define WACOM_IV_PROXIMITY	(0x1<<6)
#define WACOM_IV_STYLUS		(0x1<<5)
#define WACOM_IV_BUTTON_PRESS	(0x1<<3)
#define WACOM_IV_X_HIGH_BITS	(0x3)
#define WACOM_IV_X_MID_BITS	(0x7f)
#define WACOM_IV_X_LOW_BITS	(0x7f)
#define WACOM_IV_Y_HIGH_BITS	(0x3)
#define WACOM_IV_Y_MID_BITS	(0x7f)
#define WACOM_IV_Y_LOW_BITS	(0x7f)
#define WACOM_IV_BUTTON_BITS	(0x78)
#define WACOM_IV_PRESSURE_ZERO	(0x1<<2)
#define WACOM_IV_PRESSURE_BITS	(0x3f)
#define WACOM_IV_PRESSURE_SIGN	(0x1<<6)
#define WACOM_IV_X_TILT_SIGN	(0x1<<6)
#define WACOM_IV_Y_TILT_SIGN	(0x1<<6)
#define WACOM_IV_X_TILT_BITS	(0x3f)
#define WACOM_IV_Y_TILT_BITS	(0x3f)

/* mode1 masks */
#define TABLET_MULTIMODE_MASK	(0x1<<0)
#define TABLET_TILTMODE_MASK	(0x1<<1)

/* mode2 masks */
#define TABLET_CORRECT_TILT_MASK	(0x1<<0)
#define TABLET_TRANSFORM_MASK		(0x1<<1)

typedef enum {
  WACOM_IV_TABLET_TYPE,
  WACOM_II_S_TABLET_TYPE,
  BITPAD_ONE_TABLET_TYPE,
  BITPAD_TWO_TABLET_TYPE,
  MM_1201_TABLET_TYPE,
  MM_961_TABLET_TYPE,
  LAST_TABLET_TYPE
} TabletType;

typedef enum {
  TABLET_ERR_NONE = 0,
  TABLET_ERR_TTY_STR,
  TABLET_ERR_TYPE,
  TABLET_ERR_LINE_CHARACTERISTICS,
  TABLET_ERR_NO_RESPONSE,
  TABLET_ERR_UNSPECIFIED
} TabletErrNum;

typedef enum {
  TABLET_DEVICE_STYLUS,
  TABLET_DEVICE_4BUTTON_CURSOR
} TabletDevice;

typedef struct _hguTablet{
  TabletType		type;
  char			*ttyStr;
  int			fd;
  char			*versionStr;
  int			cntrlMode1;
  int			cntrlMode2;
  int			transmitting;
  int			xTiltLut[256];
  int			yTiltLut[256];
  double		xTrans[3];
  double		yTrans[3];
} HGUTablet;

typedef struct _tabletEvent{
  int		type; /* only one type = 0 */
  int		eventNum;
  int		proximity;
  TabletDevice	device;
  int		buttonPressed;
  int		buttons;
  int		x;
  int		y;
  int		pressure;
  int		xTilt;
  int		yTilt;
  double	X;
  double	Y;
} TabletEvent;
  

/* function prototypes */
extern HGUTablet *TabletOpen(
  TabletType	type,
  char		*ttyStr,
  TabletErrNum	*dstErr);

extern TabletErrNum TabletClose(
  HGUTablet	*tablet);

extern TabletErrNum TabletStop(
  HGUTablet	*tablet);

extern TabletErrNum TabletStart(
  HGUTablet	*tablet);

extern TabletErrNum TabletNextEvent(
  HGUTablet	*tablet,
  TabletEvent	*event);

extern TabletErrNum TabletClearEvents(
  HGUTablet	*tablet);

extern TabletErrNum TabletSetMode(
  HGUTablet	*tablet,
  unsigned int	modeFlg);

/* do not add anything after this line */
#endif /* HGU_TABLET_H */
