/** @file MaintenanceScreen.h
 *  @brief The screen provides graphic interface to update firmware
 *  @author Viet Le
 */

#ifndef MaintenanceScreen_H
#define	MaintenanceScreen_H


/* Initialize mainscreen's element: Check all maincreen's widgets have been created yet,
 * init control & data variable and change some properties in needed initialization widgets */
void MaintenanceScreen_Initialize(void);

/* Initialize mainscreen's element: Check all maincreen's widgets have been created yet,
 * init control & data variable and change some properties in needed initialization widgets */
void MaintenanceScreen_Reinit(void);

/* To free memory while maintenancescreen not showing*/
void MaintenanceScreen_Distructor(void);

/* Implement mainscreen's state, animation function and some action periodlly
 *  It was called by DisplayControl */
void MaintenanceScreen_Run(void);

#endif	/* MaintenanceScreen_H */

/* end of file */