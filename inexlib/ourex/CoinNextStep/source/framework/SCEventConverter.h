/* ============================================================== *
 |                                                                |
 | This file is part of SC21, a Cocoa user interface binding for  |
 | the Coin 3D visualization library.                             |
 |                                                                |
 | Copyright (c) 2003 Systems in Motion. All rights reserved.     |
 |                                                                |
 | SC21 is free software; you can redistribute it and/or          |
 | modify it under the terms of the GNU General Public License    |
 | ("GPL") version 2 as published by the Free Software            |
 | Foundation.                                                    |
 |                                                                |
 | A copy of the GNU General Public License can be found in the   |
 | source distribution of SC21. You can also read it online at    |
 | http://www.gnu.org/licenses/gpl.txt.                           |
 |                                                                |
 | For using Coin with software that can not be combined with the |
 | GNU GPL, and for taking advantage of the additional benefits   |
 | of our support services, please contact Systems in Motion      |
 | about acquiring a Coin Professional Edition License.           |
 |                                                                |
 | See http://www.coin3d.org/mac/SC21 for more information.       |
 |                                                                |
 | Systems in Motion, Bygdoy Alle 5, 0257 Oslo, Norway.           |
 |                                                                |
 * ============================================================== */
 

#import <Foundation/Foundation.h>
#import <Inventor/events/SoEvents.h>
#import <Inventor/events/SoKeyboardEvent.h>
#import <Inventor/SbDict.h>

#define SC_MAP //G.Barrand
#ifdef SC_MAP
#import <map>
#endif

@class NSEvent; //G.Barrand

@class SCController;

@interface SCEventConverter : NSObject {
  SCController * _controller;
#ifdef SC_MAP
  std::map<unichar,SoKeyboardEvent::Key>* _keydict; //G.Barrand
  std::map<unichar,char>* _printabledict; //G.Barrand
#else
  SbDict * _keydict, * _printabledict;
#endif
}

/*" Initializing an SCEventConverter "*/
- (id) initWithController:(SCController *)controller;

/*" Event conversion "*/
- (SoEvent *) createSoEvent:(NSEvent *)event;
- (SoKeyboardEvent *) createSoKeyboardEventWithString:(NSString *)s;

/*" Setting the controller component "*/
- (void) setController:(SCController *)controller;
- (SCController *) controller;

@end


