#include "EJAppViewController.h"
#include "EJJavaScriptView.h"

static EJAppViewController* EJAppViewController::ejectaInstance = NULL

EJAppViewController::EJAppViewController()
{
	landscapeMode = true;
}

EJAppViewController::~EJAppViewController()
{
	view = NULL;
}

void EJAppViewController::didReceiveMemoryWarning()
{
	((EJJavaScriptView *)(this.view))->clearCaches();
}

void EJAppViewController::init(const char* path, int w, int h)
{
	
	view = new EJJavaScriptView(w, h);
	view->setMainBundle(path);
	view->loadScriptAtPath(NSStringMake("index.js"));

}


void EJAppViewController::setScreenSize(int w, int h)
{
	view->setScreenSize(int w, int h);
}

void EJAppViewController::run(void)
{
	view->run();
}

EJAppViewController* EJAppViewController::instance()
{
	if (ejectaInstance == NULL)
	{
		ejectaInstance = new EJAppViewController();
	}
	return ejectaInstance;
}


void EJAppViewController::finalize()
{
	if (ejectaInstance != NULL)
	{
		ejectaInstance->release();
		ejectaInstance = NULL;
	}
}

// - (NSUInteger)supportedInterfaceOrientations {
// 	if( landscapeMode ) {
// 		// Allow Landscape Left and Right
// 		return UIInterfaceOrientationMaskLandscape;
// 	}
// 	else {
// 		if( UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad ) {
// 			// Allow Portrait UpsideDown on iPad
// 			return UIInterfaceOrientationMaskPortrait | UIInterfaceOrientationMaskPortraitUpsideDown;
// 		}
// 		else {
// 			// Only Allow Portrait
// 			return UIInterfaceOrientationMaskPortrait;
// 		}
// 	}
// }

// - (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)orientation {
// 	// Deprecated in iOS6 - supportedInterfaceOrientations is the new way to do this
// 	// We just use the mask returned by supportedInterfaceOrientations here to check if
// 	// this particular orientation is allowed.
// 	return ( self.supportedInterfaceOrientations & (1 << orientation) );
// }

// #import <objc/runtime.h>

// #import "EJAppViewController.h"
// #import "EJJavaScriptView.h"

// @implementation EJAppViewController

// - (id)init{
// 	if( self = [super init] ) {
// 		landscapeMode = [[[NSBundle mainBundle] infoDictionary][@"UIInterfaceOrientation"]
// 			hasPrefix:@"UIInterfaceOrientationLandscape"];
// 	}
// 	return self;
// }

// - (void)dealloc {
// 	self.view = nil;
// 	[super dealloc];
// }

// - (void)didReceiveMemoryWarning {
// 	[(EJJavaScriptView *)self.view clearCaches];
// }

// - (void)loadView {
// 	CGRect frame = UIScreen.mainScreen.bounds;
// 	if( landscapeMode ) {
// 		frame.size = CGSizeMake(frame.size.height, frame.size.width);
// 	}
	
// 	EJJavaScriptView *view = [[EJJavaScriptView alloc] initWithFrame:frame];
// 	self.view = view;
	
// 	[view loadScriptAtPath:@"index.js"];
// 	[view release];
// }

// - (NSUInteger)supportedInterfaceOrientations {
// 	if( landscapeMode ) {
// 		// Allow Landscape Left and Right
// 		return UIInterfaceOrientationMaskLandscape;
// 	}
// 	else {
// 		if( UI_USER_INTERFACE_IDIOM() == UIUserInterfaceIdiomPad ) {
// 			// Allow Portrait UpsideDown on iPad
// 			return UIInterfaceOrientationMaskPortrait | UIInterfaceOrientationMaskPortraitUpsideDown;
// 		}
// 		else {
// 			// Only Allow Portrait
// 			return UIInterfaceOrientationMaskPortrait;
// 		}
// 	}
// }

// - (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)orientation {
// 	// Deprecated in iOS6 - supportedInterfaceOrientations is the new way to do this
// 	// We just use the mask returned by supportedInterfaceOrientations here to check if
// 	// this particular orientation is allowed.
// 	return ( self.supportedInterfaceOrientations & (1 << orientation) );
// }

// @end
