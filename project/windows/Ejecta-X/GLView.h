//
// Book:      OpenGL(R) ES 2.0 Programming Guide
// Authors:   Aaftab Munshi, Dan Ginsburg, Dave Shreiner
// ISBN-10:   0321502795
// ISBN-13:   9780321502797
// Publisher: Addison-Wesley Professional
// URLs:      http://safari.informit.com/9780321563835
//            http://www.opengles-book.com
//

//
/// \file ESUtil.h
/// \brief A utility library for OpenGL ES.  This library provides a
///        basic common framework for the example applications in the
///        OpenGL ES 2.0 Programming Guide.
//
#ifndef GLVIEW_H
#define GLVIEW_H

///
//  Includes
//
   
///
//  Macros
//
#define GLVIEW_API  __cdecl
#define GLCALLBACK  __cdecl


/// esCreateWindow flag - RGB color buffer
#define GL_WINDOW_RGB           0
/// esCreateWindow flag - ALPHA color buffer
#define GL_WINDOW_ALPHA         1 
/// esCreateWindow flag - depth buffer
#define GL_WINDOW_DEPTH         2 
/// esCreateWindow flag - stencil buffer
#define GL_WINDOW_STENCIL       4
/// esCreateWindow flat - multi-sample buffer
#define GL_WINDOW_MULTISAMPLE   8


///
// Types
//

typedef struct
{
    GLfloat   m[4][4];
} GLMatrix;

typedef struct
{
   /// Put your user data here...
   void*       userData;

   /// Window width
   GLint       width;

   /// Window height
   GLint       height;

   /// Window handle
   HWND hWnd;

   /// EGL display
   HDC eglDisplay;
      
   /// EGL context
   HGLRC eglContext;

   /// GL surface
   //GLSurface  glSurface;

   /// Callbacks
   void (GLCALLBACK *drawFunc) ( void* );
   void (GLCALLBACK *keyFunc) ( void*, unsigned char, int, int );
   void (GLCALLBACK *updateFunc) ( void*, float deltaTime );
} ESContext;


///
//  Public Functions
//

///
// CreateGLContext()
//
//    Creates an GL rendering context and all associated elements
//
GLboolean CreateGLContext ( EGLNativeWindowType hWnd, EGLDisplay* eglDisplay,
	EGLContext* eglContext,
	EGLint attribList[]);

//
///
/// \brief Initialize ES framework context.  This must be called before calling any other functions.
/// \param esContext Application context
//
void GLVIEW_API esInitContext ( ESContext *esContext );

//
/// \brief Create a window with the specified parameters
/// \param esContext Application context
/// \param title Name for title bar of window
/// \param width Width in pixels of window to create
/// \param height Height in pixels of window to create
/// \param flags Bitfield for the window creation flags 
///         GL_WINDOW_RGB     - specifies that the color buffer should have R,G,B channels
///         GL_WINDOW_ALPHA   - specifies that the color buffer should have alpha
///         GL_WINDOW_DEPTH   - specifies that a depth buffer should be created
///         GL_WINDOW_STENCIL - specifies that a stencil buffer should be created
///         GL_WINDOW_MULTISAMPLE - specifies that a multi-sample buffer should be created
/// \return GL_TRUE if window creation is succesful, GL_FALSE otherwise
GLboolean GLVIEW_API esCreateWindow ( ESContext *esContext, const char *title, GLint width, GLint height, GLuint flags );

//
/// \brief Start the main loop for the OpenGL ES application
/// \param esContext Application context
//
void GLVIEW_API esMainLoop ( ESContext *esContext );

//
/// \brief Register a draw callback function to be used to render each frame
/// \param esContext Application context
/// \param drawFunc Draw callback function that will be used to render the scene
//
void GLVIEW_API esRegisterDrawFunc ( ESContext *esContext, void (GLCALLBACK *drawFunc) ( ESContext* ) );

//
/// \brief Register an update callback function to be used to update on each time step
/// \param esContext Application context
/// \param updateFunc Update callback function that will be used to render the scene
//
void GLVIEW_API esRegisterUpdateFunc ( ESContext *esContext, void (GLCALLBACK *updateFunc) ( ESContext*, float ) );

//
/// \brief Register an keyboard input processing callback function
/// \param esContext Application context
/// \param keyFunc Key callback function for application processing of keyboard input
//
void GLVIEW_API esRegisterKeyFunc ( ESContext *esContext, 
                                    void (GLCALLBACK *drawFunc) ( ESContext*, unsigned char, int, int ) );
//
/// \brief Log a message to the debug output for the platform
/// \param formatStr Format string for error log.  
//
void GLVIEW_API esLogMessage ( const char *formatStr, ... );

//
///
/// \brief Load a shader, check for compile errors, print error messages to output log
/// \param type Type of shader (GL_VERTEX_SHADER or GL_FRAGMENT_SHADER)
/// \param shaderSrc Shader source string
/// \return A new shader object on success, 0 on failure
//
GLuint GLVIEW_API esLoadShader ( GLenum type, const char *shaderSrc );

//
///
/// \brief Load a vertex and fragment shader, create a program object, link program.
///        Errors output to log.
/// \param vertShaderSrc Vertex shader source code
/// \param fragShaderSrc Fragment shader source code
/// \return A new program object linked with the vertex/fragment shader pair, 0 on failure
//
GLuint GLVIEW_API esLoadProgram ( const char *vertShaderSrc, const char *fragShaderSrc );


//
/// \brief Generates geometry for a sphere.  Allocates memory for the vertex data and stores 
///        the results in the arrays.  Generate index list for a TRIANGLE_STRIP
/// \param numSlices The number of slices in the sphere
/// \param vertices If not NULL, will contain array of float3 positions
/// \param normals If not NULL, will contain array of float3 normals
/// \param texCoords If not NULL, will contain array of float2 texCoords
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLE_STRIP
//
int GLVIEW_API esGenSphere ( int numSlices, float radius, GLfloat **vertices, GLfloat **normals, 
                             GLfloat **texCoords, GLuint **indices );

//
/// \brief Generates geometry for a cube.  Allocates memory for the vertex data and stores 
///        the results in the arrays.  Generate index list for a TRIANGLES
/// \param scale The size of the cube, use 1.0 for a unit cube.
/// \param vertices If not NULL, will contain array of float3 positions
/// \param normals If not NULL, will contain array of float3 normals
/// \param texCoords If not NULL, will contain array of float2 texCoords
/// \param indices If not NULL, will contain the array of indices for the triangle strip
/// \return The number of indices required for rendering the buffers (the number of indices stored in the indices array
///         if it is not NULL ) as a GL_TRIANGLES
//
int GLVIEW_API esGenCube ( float scale, GLfloat **vertices, GLfloat **normals, 
                           GLfloat **texCoords, GLuint **indices );

//
/// \brief Loads a 24-bit TGA image from a file
/// \param fileName Name of the file on disk
/// \param width Width of loaded image in pixels
/// \param height Height of loaded image in pixels
///  \return Pointer to loaded image.  NULL on failure. 
//
char* GLVIEW_API esLoadTGA ( char *fileName, int *width, int *height );


//
/// \brief multiply matrix specified by result with a scaling matrix and return new matrix in result
/// \param result Specifies the input matrix.  Scaled matrix is returned in result.
/// \param sx, sy, sz Scale factors along the x, y and z axes respectively
//
void GLVIEW_API esScale(ESMatrix *result, GLfloat sx, GLfloat sy, GLfloat sz);

//
/// \brief multiply matrix specified by result with a translation matrix and return new matrix in result
/// \param result Specifies the input matrix.  Translated matrix is returned in result.
/// \param tx, ty, tz Scale factors along the x, y and z axes respectively
//
void GLVIEW_API esTranslate(ESMatrix *result, GLfloat tx, GLfloat ty, GLfloat tz);

//
/// \brief multiply matrix specified by result with a rotation matrix and return new matrix in result
/// \param result Specifies the input matrix.  Rotated matrix is returned in result.
/// \param angle Specifies the angle of rotation, in degrees.
/// \param x, y, z Specify the x, y and z coordinates of a vector, respectively
//
void GLVIEW_API esRotate(ESMatrix *result, GLfloat angle, GLfloat x, GLfloat y, GLfloat z);

//
// \brief multiply matrix specified by result with a perspective matrix and return new matrix in result
/// \param result Specifies the input matrix.  new matrix is returned in result.
/// \param left, right Coordinates for the left and right vertical clipping planes
/// \param bottom, top Coordinates for the bottom and top horizontal clipping planes
/// \param nearZ, farZ Distances to the near and far depth clipping planes.  Both distances must be positive.
//
void GLVIEW_API esFrustum(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ);

//
/// \brief multiply matrix specified by result with a perspective matrix and return new matrix in result
/// \param result Specifies the input matrix.  new matrix is returned in result.
/// \param fovy Field of view y angle in degrees
/// \param aspect Aspect ratio of screen
/// \param nearZ Near plane distance
/// \param farZ Far plane distance
//
void GLVIEW_API esPerspective(ESMatrix *result, float fovy, float aspect, float nearZ, float farZ);

//
/// \brief multiply matrix specified by result with a perspective matrix and return new matrix in result
/// \param result Specifies the input matrix.  new matrix is returned in result.
/// \param left, right Coordinates for the left and right vertical clipping planes
/// \param bottom, top Coordinates for the bottom and top horizontal clipping planes
/// \param nearZ, farZ Distances to the near and far depth clipping planes.  These values are negative if plane is behind the viewer
//
void GLVIEW_API esOrtho(ESMatrix *result, float left, float right, float bottom, float top, float nearZ, float farZ);

//
/// \brief perform the following operation - result matrix = srcA matrix * srcB matrix
/// \param result Returns multiplied matrix
/// \param srcA, srcB Input matrices to be multiplied
//
void GLVIEW_API esMatrixMultiply(ESMatrix *result, ESMatrix *srcA, ESMatrix *srcB);

//
//// \brief return an indentity matrix 
//// \param result returns identity matrix
//
void GLVIEW_API esMatrixLoadIdentity(ESMatrix *result);

#endif // GLVIEW_H