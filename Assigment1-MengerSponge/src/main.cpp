#include <array>
#include <iostream>
#include <vector>

// glad beforw glfw
#include "glad/glad.h"
#include <stdlib.h>

//
#include "GLFW/glfw3.h"

#include "common_matrices.hpp"
#include "menger.hpp"
#include "cube.hpp"
#include "mat4f.hpp"
#include "shader_tools.hpp"
#include "vec3f.hpp"

using namespace math;
using namespace geometry;


//GLOBAL VARIABLES//
math::Mat4f g_M = math::identity();
math::Mat4f MVP = math::identity();
int depth = 0;
int g_totalVertices = 0;
bool shading = false;
bool pview = true;
double start_x = 0;
double start_y = 0;
double end_x = 0;
double end_y = 0;
bool has_rotation_started = false;
bool has_rotation_ended = false;
bool v_done = false;
// function declaration
using namespace std;

void setFrameBufferSize(GLFWwindow *window, int width, int height) {
	width = height;

  glViewport(0, 0, width, height);

}

static void setMouse(GLFWwindow *window, double xPos, double yPos) {
	if (has_rotation_started) {
		if (!v_done) {
			start_x = xPos;
			start_y = yPos;
			v_done = true;
		}

		g_M = rotateAboutXMatrix(-(xPos - start_x)/1000) * rotateAboutYMatrix(-(yPos - start_y)/1000) * g_M;
	}
		

}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		has_rotation_started = true;
		//printf("Button is pressed\n");
		if (!v_done) {

			v_done = true;
		}
		
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {

		has_rotation_started = false;
		v_done = false;
		//printf("Button is released\n");
	}
	
		
}

void setstartingscroll(GLFWwindow *window, double xPos, double yPos) {
	if (has_rotation_started) {
		start_x = xPos;
		start_y = yPos;
	}
}

void zoom_in_out(GLFWwindow* window, double xoffset, double yoffset) {
	
	g_M = uniformScaleMatrix((yoffset*0.5)+1) * g_M; 
	
}

void setKeyboard(GLFWwindow *window, int key, int scancode, int action,
								 int mods) {
	if (GLFW_KEY_LEFT == key) {
		if (GLFW_PRESS == action){
			g_M = rotateAboutYMatrix(-5) *g_M;
		}
	} else if (GLFW_KEY_RIGHT == key) {
		if (GLFW_PRESS == action){
			g_M = rotateAboutYMatrix(5) *g_M ;
	
		}
	}
	else if (GLFW_KEY_UP == key) {
		if (GLFW_PRESS == action) {
			g_M = rotateAboutXMatrix(5) * g_M;
			
		}
	}
	else if (GLFW_KEY_DOWN == key) {
		if (GLFW_PRESS == action) {
			g_M = rotateAboutXMatrix(-5) * g_M;
			
		}
	}
	else if (GLFW_KEY_Z == key) {
		if (GLFW_PRESS == action) {
			g_M = uniformScaleMatrix(1.25) * g_M;

		}
	}
	else if (GLFW_KEY_C == key) {
		if (GLFW_PRESS == action) {
			g_M = uniformScaleMatrix(0.75) * g_M;

		}
	}
	else if (GLFW_KEY_E == key) {
		if (GLFW_PRESS == action) {
			if (depth < 3) {
				depth = depth + 1;
				
			}
			else {
				printf("More recursion could eat up your RAM \n");
			}
			
		}
	}
	else if (GLFW_KEY_W == key) {
		if (GLFW_PRESS == action) {
			depth = depth-1;
		}
	}
	else if (GLFW_KEY_P == key) {
		if (GLFW_PRESS == action) {
			if (!pview) {
				g_M = rotateAboutYMatrix(-25) * rotateAboutXMatrix(-15);
				pview = true;
			}
			else {
				g_M = rotateAboutYMatrix(-25) * rotateAboutXMatrix(-15);
				pview = false;
			}
		}
	}
	else if (GLFW_KEY_S == key) {
		if (GLFW_PRESS == action) {
			if (shading) {
				shading = false;
			}
			else {
				shading = true;
			}
		}
	}
}


namespace openGL {

struct Vertex {
	Vertex() = default;
	Vertex(Vec3f position, Vec3f color) : position(position), color(color) {}

	Vec3f position;
	Vec3f color;
};

} // namespace openGL


int main() {
	GLFWwindow *window = nullptr;

	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}

	// print version
	int glfwMajor, glfwMinor, glfwRevision;
	glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);
	cout << "GLFW Version: " << glfwMajor << "." << glfwMinor << "."
			 << glfwRevision << endl;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //apple only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	//glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(1000,              // width
							  1000,              // height
							  "Menger Sponge", // title
							  NULL,              // unused
							  NULL);
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwSetCursorPosCallback(window, setMouse);
	//set Context to window
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, zoom_in_out);
	glfwMakeContextCurrent(window);
	//Load up
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSwapInterval(1); // vsync
	glEnable(GL_MULTISAMPLE);//maybe no
	glEnable(GL_DEPTH_TEST);//

	// setup callbacks
	glfwSetFramebufferSizeCallback(window, setFrameBufferSize);
	glfwSetKeyCallback(window, setKeyboard);


	//Set up base cube
	
	Cube baseCube({ -1.f,-1.f,0.f },
		{ 1.f,-1.f,0.f },
		{ -1.f,1.f,0.f },
		{ 1.f,1.f,0.f },
		{ -1.f,-1.f,-2.f },
		{ 1.f,-1.f,-2.f },
		{ -1.f,1.f,-2.f },
		{ 1.f,1.f,-2.f });
		
	int frame = 0;
	g_M = rotateAboutYMatrix(-25) * rotateAboutXMatrix(-15) ;


	Mat4f projn = perspectiveProjection(45, 1.f, 0.1f, 100.f);
	Mat4f symmn = symmetricOrthographicProjection(2.f, 2.f, 0.1f,100.f);

	Mat4f projectionMatrix = transposed(projn);
	Mat4f symmetricMatrix = transposed(symmn);
	
	Mat4f viewMatrix = lookAtMatrix(Vec3f(0.f, 0.f, -7.f), Vec3f(0.f, 0.f, 0.f), Vec3f(0.f, 1.f, 0.f));
	
	//Mat4f MVP = math::identity();
	
	

	while (!glfwWindowShouldClose(window)) {


		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	   
		glMatrixMode(GL_PROJECTION);
		if (pview) {
			glLoadMatrixf(projectionMatrix.data());
		}
		else {
			glLoadMatrixf(symmetricMatrix.data());
		}

		MVP = viewMatrix * g_M;
		Mat4f tMVP = transposed(MVP);
		glMatrixMode(GL_MODELVIEW);

		
		glLoadMatrixf(tMVP.data());
	


		glBegin(GL_TRIANGLES);
		
		auto cubs = fractal::MengerFractal(baseCube, depth);
		using namespace openGL;
		std::vector<Vertex> vertices;
		vertices.reserve(cubs.size() * 8);
		
		

			for (int i = 0; i < cubs.size(); ++i) {
				auto const &t = cubs[i];

				Vec3f a = t.a();
				Vec3f b = t.b();
				Vec3f c = t.c();
				Vec3f d = t.d();
				Vec3f e = t.e();
				Vec3f f = t.f();
				Vec3f g = t.g();
				Vec3f h = t.h();
				Vertex vert;

				if (!shading) {
					//Front face
					glColor3f(0.3f, 0.3f, 0.3f);
					glVertex3f(a.x, a.y, a.z);
					glVertex3f(b.x, b.y, b.z);
					glVertex3f(c.x, c.y, c.z);

					glVertex3f(b.x, b.y, b.z);
					glVertex3f(c.x, c.y, c.z);
					glVertex3f(d.x, d.y, d.z);

					//Right hand face
					glColor3f(0.5f, 0.5f, 0.5f);
					glVertex3f(b.x, b.y, b.z);
					glVertex3f(d.x, d.y, d.z);
					glVertex3f(f.x, f.y, f.z);

					glVertex3f(d.x, d.y, d.z);
					glVertex3f(f.x, f.y, f.z);
					glVertex3f(h.x, h.y, h.z);

					//Bottom face
					glColor3f(0.1f, 0.1f, 0.1f);
					glVertex3f(a.x, a.y, a.z);
					glVertex3f(b.x, b.y, b.z);
					glVertex3f(e.x, e.y, e.z);

					glVertex3f(b.x, b.y, b.z);
					glVertex3f(e.x, e.y, e.z);
					glVertex3f(f.x, f.y, f.z);

					//Left hand face
					glColor3f(0.6f, 0.6f, 0.6f);
					glVertex3f(a.x, a.y, a.z);
					glVertex3f(c.x, c.y, c.z);
					glVertex3f(e.x, e.y, e.z);

					glVertex3f(c.x, c.y, c.z);
					glVertex3f(e.x, e.y, e.z);
					glVertex3f(g.x, g.y, g.z);

					//Back Face
					glColor3f(0.8f, 0.8f, 0.8f);
					glVertex3f(e.x, e.y, e.z);
					glVertex3f(f.x, f.y, f.z);
					glVertex3f(g.x, g.y, g.z);

					glVertex3f(g.x, g.y, g.z);
					glVertex3f(h.x, h.y, h.z);
					glVertex3f(f.x, f.y, f.z);

					//Upper face
					glColor3f(0.9f, 0.9f, 0.9f);
					glVertex3f(c.x, c.y, c.z);
					glVertex3f(d.x, d.y, d.z);
					glVertex3f(g.x, g.y, g.z);

					glVertex3f(d.x, d.y, d.z);
					glVertex3f(g.x, g.y, g.z);
					glVertex3f(h.x, h.y, h.z);
				}
				else {
					//Front face
					glColor3f(0.3f, 0.3f, 0.5f);
					glVertex3f(a.x, a.y, a.z);
					glVertex3f(b.x, b.y, b.z);
					glVertex3f(c.x, c.y, c.z);

					glVertex3f(b.x, b.y, b.z);
					glVertex3f(c.x, c.y, c.z);
					glVertex3f(d.x, d.y, d.z);

					//Right hand face
					glColor3f(0.8f, 0.5f, 0.5f);
					glVertex3f(b.x, b.y, b.z);
					glVertex3f(d.x, d.y, d.z);
					glVertex3f(f.x, f.y, f.z);

					glVertex3f(d.x, d.y, d.z);
					glVertex3f(f.x, f.y, f.z);
					glVertex3f(h.x, h.y, h.z);

					//Bottom face
					glColor3f(0.1f, 0.1f, 0.3f);
					glVertex3f(a.x, a.y, a.z);
					glVertex3f(b.x, b.y, b.z);
					glVertex3f(e.x, e.y, e.z);

					glVertex3f(b.x, b.y, b.z);
					glVertex3f(e.x, e.y, e.z);
					glVertex3f(f.x, f.y, f.z);

					//Left hand face
					glColor3f(0.6f, 0.9f, 0.6f);
					glVertex3f(a.x, a.y, a.z);
					glVertex3f(c.x, c.y, c.z);
					glVertex3f(e.x, e.y, e.z);

					glVertex3f(c.x, c.y, c.z);
					glVertex3f(e.x, e.y, e.z);
					glVertex3f(g.x, g.y, g.z);

					//Back Face
					glColor3f(0.2f, 0.8f, 0.8f);
					glVertex3f(e.x, e.y, e.z);
					glVertex3f(f.x, f.y, f.z);
					glVertex3f(g.x, g.y, g.z);

					glVertex3f(g.x, g.y, g.z);
					glVertex3f(h.x, h.y, h.z);
					glVertex3f(f.x, f.y, f.z);

					//Upper face
					glColor3f(0.9f, 0.1f, 0.9f);
					glVertex3f(c.x, c.y, c.z);
					glVertex3f(d.x, d.y, d.z);
					glVertex3f(g.x, g.y, g.z);

					glVertex3f(d.x, d.y, d.z);
					glVertex3f(g.x, g.y, g.z);
					glVertex3f(h.x, h.y, h.z);


				}
				
				
			}


			

			glEnd();

		
			glfwSwapBuffers(window); // swaps back buffer to front for drawing to screen
			glfwPollEvents();        // will process event queue and carry on
		}
	

	// cleaup window, and glfw before exit
	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
