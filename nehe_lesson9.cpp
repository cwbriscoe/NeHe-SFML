/*
 *		This Code Was Created By Jeff Molofee 2000
 *		A HUGE Thanks To Fredric Echols For Cleaning Up
 *		And Optimizing The Base Code, Making It More Flexible!
 *		If You've Found This Code Useful, Please Let Me Know.
 *		Visit My Site At nehe.gamedev.net
 */

#include <SFML/Graphics.hpp>

bool	fullscreen=FALSE;	// Fullscreen Flag Set To Fullscreen Mode By Default
bool    vsync=TRUE;         // Turn VSYNC on/off

bool	twinkle;			// Twinkling Stars

const unsigned int num=50;  // Number Of Stars To Draw

typedef struct				// Create A Structure For Star
{
	int r, g, b;			// Stars Color
	GLfloat dist,			// Stars Distance From Center
			angle;			// Stars Current Angle
}
stars;
stars star[num];			// Need To Keep Track Of 'num' Stars

GLfloat	zoom=-15.0f;		// Distance Away From Stars
GLfloat tilt=90.0f;			// Tilt The View
GLfloat	spin;				// Spin Stars

GLuint	loop;				// General Loop Variable
GLuint	texture[1];			// Storage For One textures

int LoadGLTextures()									// Load Bitmaps And Convert To Textures
{
	int Status=FALSE;									// Status Indicator

	// Load The Bitmap, Check For Errors, If Bitmap's Not Found Quit
	sf::Image Image;
	if (Image.LoadFromFile("Data/Star.bmp"))
	{
		Status=TRUE;									// Set The Status To TRUE

        glGenTextures(1, &texture[0]);          // Create One Texture

        // Create Linear Filtered Texture
        glBindTexture(GL_TEXTURE_2D, texture[0]);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, Image.GetWidth(), Image.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, Image.GetPixelsPtr());
	}

	return Status;										// Return The Status
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int InitGL()										    // All Setup For OpenGL Goes Here
{
	if (!LoadGLTextures())								// Jump To Texture Loading Routine
	{
		return FALSE;									// If Texture Didn't Load Return FALSE
	}

	glEnable(GL_TEXTURE_2D);							// Enable Texture Mapping
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					// Set The Blending Function For Translucency
	glEnable(GL_BLEND);

	for (loop=0; loop<num; loop++)
	{
		star[loop].angle=0.0f;
		star[loop].dist=(float(loop)/num)*5.0f;
		star[loop].r=rand()%256;
		star[loop].g=rand()%256;
		star[loop].b=rand()%256;
	}
	return TRUE;										// Initialization Went OK
}

int DrawGLScene()									    // Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glBindTexture(GL_TEXTURE_2D, texture[0]);			// Select Our Texture

	for (loop=0; loop<num; loop++)						// Loop Through All The Stars
	{
		glLoadIdentity();								// Reset The View Before We Draw Each Star
		glTranslatef(0.0f,0.0f,zoom);					// Zoom Into The Screen (Using The Value In 'zoom')
		glRotatef(tilt,1.0f,0.0f,0.0f);					// Tilt The View (Using The Value In 'tilt')
		glRotatef(star[loop].angle,0.0f,1.0f,0.0f);		// Rotate To The Current Stars Angle
		glTranslatef(star[loop].dist,0.0f,0.0f);		// Move Forward On The X Plane
		glRotatef(-star[loop].angle,0.0f,1.0f,0.0f);	// Cancel The Current Stars Angle
		glRotatef(-tilt,1.0f,0.0f,0.0f);				// Cancel The Screen Tilt

		if (twinkle)
		{
			glColor4ub(star[(num-loop)-1].r,star[(num-loop)-1].g,star[(num-loop)-1].b,255);
			glBegin(GL_QUADS);
				glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
			glEnd();
		}

		glRotatef(spin,0.0f,0.0f,1.0f);
		glColor4ub(star[loop].r,star[loop].g,star[loop].b,255);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 0.0f);
		glEnd();

		spin+=0.01f;
		star[loop].angle+=float(loop)/num;
		star[loop].dist-=0.01f;
		if (star[loop].dist<0.0f)
		{
			star[loop].dist+=5.0f;
			star[loop].r=rand()%256;
			star[loop].g=rand()%256;
			star[loop].b=rand()%256;
		}
	}
	return TRUE;										// Everything Went OK
}

int main()
{
    // Create the main window
    sf::Window App(sf::VideoMode(800, 600, 32), "SFML/NeHe OpenGL");

    InitGL();
    ReSizeGLScene(800, 600);

    // Start game loop
    while (App.IsOpened())
    {
        // Process events
        sf::Event Event;
        while (App.GetEvent(Event))
        {
            // Close window : exit
            if (Event.Type == sf::Event::Closed)
                App.Close();

            // Resize event : adjust viewport
            if (Event.Type == sf::Event::Resized)
                ReSizeGLScene(Event.Size.Width, Event.Size.Height);

            // Handle Keyboard Events
            if (Event.Type == sf::Event::KeyPressed) {
                switch (Event.Key.Code) {
                    case sf::Key::Escape:
                        App.Close();
                        break;
                    case sf::Key::F1:
                        fullscreen = !fullscreen;
                        App.Create(fullscreen ? sf::VideoMode::GetDesktopMode() : sf::VideoMode(800, 600, 32) , "SFML/NeHe OpenGL",
                        (fullscreen ? sf::Style::Fullscreen : sf::Style::Resize | sf::Style::Close));
                        ReSizeGLScene(App.GetWidth(),App.GetHeight());
                        break;
                    case sf::Key::F5:
                        vsync = !vsync;
                        break;
                    case sf::Key::T:
                        twinkle = !twinkle;
                        break;
                    default:
                        break;
                }
            }
        }

        //Handle movement keys
        const sf::Input& Input = App.GetInput();

        if (Input.IsKeyDown(sf::Key::PageUp)) {
            zoom-=0.2f;
        }
        if (Input.IsKeyDown(sf::Key::PageDown)) {
            zoom+=0.2f;
        }
        if (Input.IsKeyDown(sf::Key::Up)) {
            tilt-=0.5f;
        }
        if (Input.IsKeyDown(sf::Key::Down)) {
            tilt+=0.5f;
        }

        // Turn VSYNC on so that animations run at a more reasonable speed on new CPU's/GPU's.
        App.UseVerticalSync(vsync);

        // Set the active window before using OpenGL commands
        // It's useless here because active window is always the same,
        // but don't forget it if you use multiple windows or controls
        App.SetActive();

        //Draw some pretty stuff
        DrawGLScene();

        // Finally, display rendered frame on screen
        App.Display();
    }

    return EXIT_SUCCESS;
}
