#include "EJCanvasContextScreen.h"
#include "EJApp.h"


EJCanvasContextScreen::EJCanvasContextScreen()
{

}


EJCanvasContextScreen::EJCanvasContextScreen(short widthp, short heightp) : EJCanvasContext( widthp, heightp)
{

}

EJCanvasContextScreen::~EJCanvasContextScreen()
{

}

void EJCanvasContextScreen::present()
{
	glViewport(0, 0, width, height);

#ifdef _WINDOWS
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0 );
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0 );
#else
	glBindFramebuffer(GL_FRAMEBUFFER, 0 );
	glBindRenderbuffer(GL_RENDERBUFFER, 0 );
#endif	

	// [self flushBuffers];
	EJCanvasContext::flushBuffers();
	
	if( msaaEnabled ) {
#ifdef _WINDOWS
		//Bind the MSAA and View frameBuffers and resolve
		glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, msaaFrameBuffer);
		glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, viewFrameBuffer);
		//glResolveMultisampleFramebufferAPPLE();

		glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, viewRenderBuffer);
		// [[EJApp instance].glContext presentRenderbuffer:GL_RENDERBUFFER];
		// EJApp::instance()->glContext->presentRenderbuffer(GL_RENDERBUFFER_OES);
		// presentRenderbuffer(GL_RENDERBUFFER_OES);
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, msaaFrameBuffer);
#else
		//Bind the MSAA and View frameBuffers and resolve
		glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, viewFrameBuffer);
		// glResolveMultisampleFramebufferAPPLE();

		glBindRenderbuffer(GL_RENDERBUFFER, viewRenderBuffer);
		// [[EJApp instance].glContext presentRenderbuffer:GL_RENDERBUFFER];
		// EJApp::instance()->glContext->presentRenderbuffer(GL_RENDERBUFFER_OES);
		// presentRenderbuffer(GL_RENDERBUFFER_OES);
		glBindFramebuffer(GL_FRAMEBUFFER, msaaFrameBuffer);
#endif
	}
	else {
	}	
}

void EJCanvasContextScreen::finish()
{
	glFinish();	
}

void EJCanvasContextScreen::create()
{

	int m_width = EJApp::instance()->width;
	int m_height = EJApp::instance()->height;

	CGSize screen = {m_width, m_height};
	CGRect frame = {0, 0, screen};
// 	CGSize screen = [EJApp instance].view.bounds.size;
    float contentScale = 1;
    // useRetinaResolution
	float aspect = (float)frame.size.width / (float)frame.size.height;

	if( scalingMode == kEJScalingModeFitWidth ) {
		frame.size.width = screen.width;
		frame.size.height = (int)(screen.width / aspect);
	}
	else if( scalingMode == kEJScalingModeFitHeight ) {
		frame.size.width = (int)(screen.height * aspect);
		frame.size.height = screen.height;
	}
	float internalScaling = frame.size.width / (float)width;
	EJApp::instance()->internalScaling = internalScaling;
	
    backingStoreRatio = internalScaling * contentScale;
	
	bufferWidth = (short)(frame.size.width * contentScale);
	bufferHeight = (short)(frame.size.height * contentScale);
	
	NSLOG(
		"====    Creating ScreenCanvas    ==== \n**    size: %dx%d, aspect ratio: %.3f, \n**    scaled: %.3f = %dx%d, \n**    retina: no = %.0fx%.0f, \n**    msaa: no\n=====================================",
		width, height, aspect, 
		internalScaling, frame.size.width, frame.size.height,
		frame.size.width * contentScale, frame.size.height * contentScale
	);
	
// 	// Create the OpenGL UIView with final screen size and content scaling (retina)
// 	glview = [[EAGLView alloc] initWithFrame:frame contentScale:contentScale];
	
// 	// This creates the frame- and renderbuffers
// 	[super create];
	EJCanvasContext::create();
	
// 	// Set up the renderbuffer and some initial OpenGL properties
// 	[[EJApp instance].glContext renderbufferStorage:GL_RENDERBUFFER fromDrawable:(CAEAGLLayer *)glview.layer];
// 	glFramebufferRenderbuffer(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER_EXT, viewRenderBuffer);
	

	glDisable(GL_CULL_FACE);
	glDisable(GL_DITHER);

	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

    upsideDown = true;

	prepare();
	
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    //Removed because Framebuffer doesn't seem to be ready, causing an error
    //glClear(GL_COLOR_BUFFER_BIT);

// 	// Append the OpenGL view to Impact's main view
    EJApp::instance()->hideLoadingScreen();
}

void EJCanvasContextScreen::resizeToWidth(short newWidth, short newHeight) {
	// TODO: Implement it 
}


void EJCanvasContextScreen::prepare()
{
	EJCanvasContext::prepare();
}

EJImageData* EJCanvasContextScreen::getImageData(float sx, float sy, float sw, float sh)
{
	if(backingStoreRatio != 1 && EJTexture::smoothScaling()) {
		NSLOG(
			"Warning: The screen canvas has been scaled; getImageData() may not work as expected. \n%s",
			"Set imageSmoothingEnabled=false or use an off-screen Canvas for more accurate results."
		);
	}
	
	// [self flushBuffers];
	flushBuffers();
	
	// Read pixels; take care of the upside down screen layout and the backingStoreRatio
	int internalWidth = (int)(sw * backingStoreRatio);
	int internalHeight = (int)(sh * backingStoreRatio);
	int internalX = (int)(sx * backingStoreRatio);
	int internalY = (int)((height-sy-sh) * backingStoreRatio);
	
	EJColorRGBA * internalPixels = (EJColorRGBA*)malloc( internalWidth * internalHeight * sizeof(EJColorRGBA));
	glReadPixels( internalX, internalY, internalWidth, internalHeight, GL_RGBA, GL_UNSIGNED_BYTE, internalPixels );

	GLubyte * pixels = (GLubyte*)malloc( (size_t)sw * (size_t)sh * sizeof(GLubyte) * 4);
	int index = 0;
	for( int y = 0; y < sh; y++ ) {
		for( int x = 0; x < sw; x++ ) {
			int internalIndex = (int)((sh-y-1) * backingStoreRatio) * internalWidth + (int)(x * backingStoreRatio);
			pixels[ index *4 + 0 ] = (GLubyte)internalPixels[ internalIndex ].rgba.r;
			pixels[ index *4 + 1 ] = (GLubyte)internalPixels[ internalIndex ].rgba.g;
			pixels[ index *4 + 2 ] = (GLubyte)internalPixels[ internalIndex ].rgba.b;
			pixels[ index *4 + 3 ] = (GLubyte)internalPixels[ internalIndex ].rgba.a;
			index++;
		}
	}
	free(internalPixels);
	
	EJImageData* m_EJImageDate = new EJImageData(sw, sh, pixels);
	m_EJImageDate->autorelease();
	return m_EJImageDate;
}