/**
 * @author	Andrew Robert Owens, Philmo Gu
 * @date 	2019-10-15.
 * @details Organization: Biological Modeling and Visualization research group
 * University of Calgary, Calgary, AB, Canada
 *
 * Contact:	arowens [at] ucalgary.ca
 * @copyright Copyright (c) 2019 ___ALGORITHMIC_BOTANY___. All rights reserved.
 *
 * @brief

 edited by Juan Luis de Reiset
 */

#include <array>
#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>
#include <cassert>	//assert

 // glad beforw glfw
#include "glad/glad.h"
//
#include "GLFW/glfw3.h"

#include "common_matrices.hpp"
#include "obj_mesh_file_io.hpp"
#include "obj_mesh.hpp"
#include "obj_mesh.hpp"
#include "triangle.hpp"
#include "mat4f.hpp"
#include "mat3f.hpp"
#include "shader.hpp"
#include "program.hpp"
#include "triangle.hpp"
#include "vec3f.hpp"
#include "vec2f.hpp"
#include "shader_file_io.hpp"
#include "buffer_object.hpp"
#include "vertex_array_object.hpp"
#include "vbo_tools.hpp"




//#include "texture.hpp"
//#include "image.hpp"

using namespace math;
using namespace geometry;
using namespace opengl;


	// GLOBAL Variables

   //Model view Projection
	Mat4f g_M = Mat4f::identity();
	Mat4f g_V = Mat4f::identity();
	Mat4f g_P = Mat4f::identity();

	//Screen settings
	GLuint g_width = 1000, g_height = 1000;

	//Control points and output Curve points
	std::vector<Vec3f> controlPoints = std::vector<Vec3f>();
	std::vector<Vec3f> outCurve;

	//Helper global
	bool right_b_pressed = false;
	int closeIndex = -1;
	bool has_changed = false;
	int display_mode = 1;

	//Shading settings
	Vec3f lightPosition(10, 10, 10);
	Vec3f lightColor(1, 1, 1);

	// function declaration
	using namespace std;
	int depth = 0;
	void mouse_scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		//std::cout << yoffset;
		if (yoffset > 0) {
			g_M = uniformScaleMatrix(1.1) * g_M;
		}
		else {
			g_M = uniformScaleMatrix(0.9) * g_M;
		}
	}

	void setFrameBufferSize(GLFWwindow *window, int width, int height) {
		g_width = width;
		g_height = height;
		glViewport(0, 0, g_width, g_height);
		g_P = perspectiveProjection(30, float(g_width) / g_height, 0.01, 100.f);
	}

	void setKeyboard(GLFWwindow *window, int key, int scancode, int action,
		int mods) {

		if (GLFW_KEY_DELETE == key) {
			if (GLFW_REPEAT == action || GLFW_PRESS == action) {
				if (right_b_pressed) {
					controlPoints.erase(controlPoints.begin() + closeIndex);
					right_b_pressed = false;
					closeIndex = -1;
					std::cout << "size " << controlPoints.size() << "\n";
					if (controlPoints.size() <= 0) {
						controlPoints = std::vector<Vec3f>();
						outCurve = std::vector<Vec3f>();
					}
				}
			}
		}

		if (GLFW_KEY_LEFT == key) {
			if (GLFW_REPEAT == action || GLFW_PRESS == action) {
				g_M = rotateAboutYMatrix(5) * g_M;
			}
		}
		else if (GLFW_KEY_RIGHT == key) {
			if (GLFW_REPEAT == action || GLFW_PRESS == action) {
				g_M = rotateAboutYMatrix(-5) * g_M;
			}
		}
		else if (GLFW_KEY_UP == key) {
			if (GLFW_REPEAT == action || GLFW_PRESS == action) {
				g_M = rotateAboutXMatrix(5) * g_M;
			}
		}
		else if (GLFW_KEY_DOWN == key) {
			if (GLFW_REPEAT == action || GLFW_PRESS == action) {
				g_M = rotateAboutXMatrix(-5) * g_M;
			}
		}
		else if (GLFW_KEY_E == key) {
			if (GLFW_REPEAT == action || GLFW_PRESS == action) {
				display_mode = 1;
				has_changed = true;
				g_M = Mat4f::identity();
			}
		}
		else if (GLFW_KEY_R == key) {
			if (GLFW_REPEAT == action || GLFW_PRESS == action) {
				display_mode = 2;
				has_changed = true;
			}
		}
		else if (GLFW_KEY_W == key) {
			if (GLFW_REPEAT == action || GLFW_PRESS == action) {
				lightPosition.z += 5;
			}
		}
		else if (GLFW_KEY_S == key) {
			if (GLFW_REPEAT == action || GLFW_PRESS == action) {
				lightPosition.z -= 5;
			}
		}
		else if (GLFW_KEY_D == key) {
			if (GLFW_REPEAT == action || GLFW_PRESS == action) {
				lightPosition.x += 5;
			}
		}
		else if (GLFW_KEY_A == key) {
			if (GLFW_REPEAT == action || GLFW_PRESS == action) {
				lightPosition.x -= 5;
			}
		}
		else if (GLFW_KEY_Q == key) {
			if (GLFW_REPEAT == action || GLFW_PRESS == action) {
				lightPosition.y += 5;
			}
		}
		else if (GLFW_KEY_Z == key) {
			if (GLFW_REPEAT == action || GLFW_PRESS == action) {
				lightPosition.y -= 5;
			}
		}
		else if (GLFW_KEY_N == key) {
			if (GLFW_REPEAT == action || GLFW_PRESS == action) {
				if (depth <= 30) {
					depth++;
				}
			}
		}
		else if (GLFW_KEY_M == key) {
			if (GLFW_REPEAT == action || GLFW_PRESS == action) {
				if (depth >= 1) {
					depth--;
				}
			}
		}
		else if (GLFW_KEY_ESCAPE == key) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
	}




	// user defined alias
	opengl::Program createShaderProgram(std::string const &vertexShaderFile,
		std::string const &fragmentShaderFile) {
		using namespace opengl;
		auto vertexShaderSource = loadShaderStringFromFile(vertexShaderFile);
		auto fragmentShaderSource = loadShaderStringFromFile(fragmentShaderFile);

		std::cout << "[Log] compiling program " << vertexShaderFile << ' '
			<< fragmentShaderFile << '\n';
		return opengl::makeProgram(vertexShaderSource, fragmentShaderSource);
	}

	std::string glfwVersion() {
		std::ostringstream s("GLFW version: ", std::ios::in | std::ios::ate);
		// print version
		int glfwMajor, glfwMinor, glfwRevision;
		glfwGetVersion(&glfwMajor, &glfwMinor, &glfwRevision);
		s << glfwMajor << '.' << glfwMinor << '.' << glfwRevision;
		return s.str();
	}

	bool loadGeometryToGPU(std::vector<Vec3f> const &vertices, GLuint vboID) {
		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(
			GL_ARRAY_BUFFER,                  // destination
			sizeof(Vec3f) * vertices.size(), // size (bytes) of memcopy to GPU
			vertices.data(),                  // pointer to data (contiguous)
			GL_STATIC_DRAW                    // usage patter of the GPU buffer
		);

		return true;
	}

	std::vector<Vec3f> subdivideClosedCurve(std::vector<Vec3f> const & points, int depth) {
		std::vector<Vec3f> finer;
		Vec3f tmp1, tmp2, tmploop;
		// Starting mask

		if (depth == 0) {
			return points;
		}

		for (int i = 0; i < points.size() - 1; i++) {

			tmp1 = points[i] + (0.25 * (points[i + 1] - points[i]));
			tmp2 = points[i] + (0.75 * (points[i + 1] - points[i]));


			finer.push_back(tmp1);
			finer.push_back(tmp2);

			if (i == 0) {
				tmploop = points[i] + (0.25 * (points[i + 1] - points[i]));
			}

		}

		finer.push_back(tmploop);



		finer = subdivideClosedCurve(finer, depth - 1);
		return finer;
	}
	int getIndex(int row, int sector, int pointSize, int sectors) {
		if (sector == sectors) {
			return row;
		}
		else {
			return sector * pointSize + row;
		}
	}
	OBJMesh surfaceofRevolution(vector<Vec3f>points, int sectors) {
		OBJMesh m;
		auto & v = m.vertices;
		auto &t = m.triangles;
		float delta = sectors / 2 * M_PI;
		for (auto p : points) {
			v.push_back(p);
		}
		for (int i = 1; i <=sectors; i++) {
			
			delta = (360.f / sectors)*i;
			for (auto p : points) {
				v.push_back(rotateAroundAxis(p, { 0,1,0 }, delta));
			}
			for (int j = 0; j < points.size(); j++) {

				geometry::Indices a;
				a.vertexID()=getIndex(j, i, points.size(),sectors);
				geometry::Indices b;
					b.vertexID()=getIndex(j, i - 1, points.size(), sectors);
					geometry::Indices c;
					c.vertexID()=getIndex(j + 1, i, points.size(), sectors);
					geometry::Indices d;
					d.vertexID()=getIndex(j + 1, i - 1, points.size(), sectors);
					t.push_back({ a,b,d });
					t.push_back({ a,d,c });
				
			}


		}
		return m;
	}
	opengl::Program createShaderProgram(std::string const &vertexShaderFile,
		std::string const &geometryShaderFile,
		std::string const &fragmentShaderFile) {
		using namespace opengl;
		auto vertexShaderSource = loadShaderStringFromFile(vertexShaderFile);
		auto geometryShaderSource = loadShaderStringFromFile(geometryShaderFile);
		auto fragmentShaderSource = loadShaderStringFromFile(fragmentShaderFile);

		std::cout << "[Log] compiling program " << vertexShaderFile << ' '
			<< geometryShaderFile << ' ' << fragmentShaderFile << '\n';
		return opengl::makeProgram(vertexShaderSource, geometryShaderSource,
			fragmentShaderSource);
	}

	std::vector<Vec3f> subdivideOpenCurve(std::vector<Vec3f> const & points, int depth) {
		std::vector<Vec3f> out;
		Vec3f newPoint1, newPoint2;

		if (depth <= 0) {
			return points;
		}

		out.push_back(points[0]);
		for (int i = 0; i < points.size() - 1; i++) {
			newPoint1 = points[i] + (0.25 * (points[i + 1] - points[i]));
			newPoint2 = points[i] + (0.75 * (points[i + 1] - points[i]));

			out.push_back(newPoint1);
			out.push_back(newPoint2);

		}
		out.push_back(points[points.size() - 1]);

		out = subdivideOpenCurve(out, depth - 1);
		return out;

	}

	void setupVAO(GLuint vaoID, GLuint vboID) {

		glBindVertexArray(vaoID);
		glBindBuffer(GL_ARRAY_BUFFER, vboID);

		// set up position input into vertex shader
		glEnableVertexAttribArray(0);          // match layout # in shader
		glVertexAttribPointer(                 //
			0,                                 // attribute layout # (in shader)
			3,                                 // number of coordinates per vertex
			GL_FLOAT,                          // type
			GL_FALSE,                          // normalized?
			sizeof(Vec3f),                    // stride
			0									 // array buffer offset
		);

		glBindVertexArray(0);
	}

	GLFWwindow *initWindow() {
		GLFWwindow *window = nullptr;

		if (!glfwInit()) {
			exit(EXIT_FAILURE);
		}

		std::cout << glfwVersion() << '\n';

		// set opengl version
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		window = glfwCreateWindow(1000,           // width
			1000,           // height
			"Mesh Loading", // title
			NULL,           // unused
			NULL);
		if (!window) {
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

		glfwMakeContextCurrent(window);
		gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		glfwSwapInterval(1); // vsync
		glEnable(GL_MULTISAMPLE);
		glEnable(GL_DEPTH_TEST);
		// glEnable(GL_CULL_FACE);
		// glCullFace(GL_BACK);

		//Polygon fill mode
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// setup callbacks
		glfwSetFramebufferSizeCallback(window, setFrameBufferSize);
		glfwSetKeyCallback(window, setKeyboard);

		return window;
	}
	void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
		if (right_b_pressed) {

			//float x = -1.0f + 2 * xpos / 1000; //THIS WORKS FOR FULL SCREEN
			
				float x = -1.0f + 4 * xpos / 1000;
				float y = +1.0f - 2 * ypos / 1000;
				controlPoints[closeIndex] = Vec3f(x, y, 0);
			
			std::cout << "mouse edit";
		}


	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {

			//Prints the (x,y) coordinate of the clicked position
			double posX, posY;
			glfwGetCursorPos(window, &posX, &posY);
			std::cout << "(x,y) coordinates are: " << posX << " " << posY << std::endl;

			//Generate 3D vector point


			//float x = -1.0f + 2 * posX / 1000;   // THIS WORKS FOR FULL SCREEN
			float x = -1.0f + 4 * posX / 1000;
			float y = +1.0f - 2 * posY / 1000;

			Vec3f clickPoint = Vec3f(x, y, 0.f);
			float diff = 2.f;
			int closeIndex = -1;

			for (int i = 0; i < controlPoints.size(); i++) {
				std::cout << "distance" << norm(controlPoints[i] - clickPoint) << '\n';
				if (norm(controlPoints[i] - clickPoint) < 0.02) {
					closeIndex = i;
				}
			}
			std::cout << "index:" << closeIndex << " difference: " << diff << std::endl;
			if (closeIndex != -1) {
				// if point is selected

			}
			else {
				//Makes sure to not create control points outside of the screen
				if (posX < 500) {
					controlPoints.push_back(clickPoint);
				}

			}

		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {

			//Prints the (x,y) coordinate of the clicked position
			double posX, posY;
			glfwGetCursorPos(window, &posX, &posY);
			std::cout << "(x,y) coordinates are: " << posX << " " << posY << std::endl;

			//Generate 3D vector point


			float x = -1.0f + 4 * posX / 1000;
			//float x = -1.0f + 2* posX / 1000;
			float y = +1.0f - 2 * posY / 1000;

			Vec3f clickPoint = Vec3f(x, y, 0.f);
			float diff = 2.f;


			for (int i = 0; i < controlPoints.size(); i++) {
				std::cout << "distance" << norm(controlPoints[i] - clickPoint) << '\n';
				if (norm(controlPoints[i] - clickPoint) < 0.02) {
					closeIndex = i;
				}
			}
			std::cout << "index:" << closeIndex << " difference: " << diff << std::endl;
			if (closeIndex != -1) {
				// if point is selected
				right_b_pressed = true;

			}
		}
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE) {
			right_b_pressed = false;
			closeIndex = -1;
		}
	}

	int main() {
		GLFWwindow *window = initWindow();


		auto vao_control = makeVertexArrayObject();
		auto vbo_control = makeBufferObject();
		auto vao_curve = makeVertexArrayObject();
		auto vbo_curve = makeBufferObject();

		auto vao = makeVertexArrayObject();
		auto indexBuffer = makeBufferObject();
		auto vertexBuffer = makeBufferObject();
		GLuint totalIndices = 0;

		Vec3f viewPosition(0, 0, 3);
		Mat4f g_Curve = Mat4f::identity();
		g_V = lookAtMatrix(viewPosition,    // eye position
			{ 0.f, 0.f, 0.f }, // look at
			{ 0.f, 1.f, 0.f }  // up vector
		);
		g_P = orthographicProjection(-1, 1, 1, -1, 0.001f, 10);
		
		auto basicShader = createShaderProgram("shaders/basic_vs.glsl",
			"shaders/basic_fs.glsl");
		basicShader.use();
		auto textureShader = createShaderProgram("shaders/texture_vs.glsl",
			"shaders/texture_gs.glsl",
			"shaders/texture_fs.glsl");
		assert(textureShader);

		setupVAO(vao_control.id(), vbo_control.id());
		setupVAO(vao_curve.id(), vbo_curve.id());

		//Load control points
	
		
	

		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, mouse_scroll_callback);


		//Load curve points


		Vec3f color_curve(0, 1, 1);
		Vec3f color_control(1, 0, 0);
		textureShader.use();
		controlPoints.push_back({ 0,0.75,0 });
		controlPoints.push_back({ -0.5,0.75,0 });
		controlPoints.push_back({ -0.25,0,0 });
		controlPoints.push_back({ 0,0,0 });
		//Set to one shader program
		opengl::Program *program = &basicShader;
		glPointSize(10);


		while (!glfwWindowShouldClose(window)) {

			glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

			glViewport(0, 0, g_width / 2, g_height);
			program = &basicShader;
			program->use();
			setUniformMat4f(program->uniformLocation("model"), g_Curve, true);
			setUniformMat4f(program->uniformLocation("view"), g_V, true);
			setUniformMat4f(program->uniformLocation("projection"), g_P, true);
			
			//setUniformVec3f(textureShader.uniformLocation("lightPosition"), lightPosition);
			//setUniformVec3f(textureShader.uniformLocation("lightColor"), lightColor);
			//glUniform1i(textureShader.uniformLocation("change_shader"), 2);
			
			//Draw control points
		
		
				loadGeometryToGPU(controlPoints, vbo_control.id());


				if (controlPoints.size() > 0) {
					outCurve = subdivideOpenCurve(controlPoints, depth);	//Subdividing with open curve
					loadGeometryToGPU(outCurve, vbo_curve.id());

				}
			
				vao_control.bind();
				setUniformVec3f(basicShader.uniformLocation("color"), color_control);


				glDrawArrays(GL_POINTS,   // type of drawing (rendered to back buffer)
					0,						  // offset into buffer
					controlPoints.size()	// number of vertices in buffer
				);


					vao_curve.bind();
				setUniformVec3f(basicShader.uniformLocation("color"), color_curve);
			
				glDrawArrays(GL_POINTS,   // type of drawing (rendered to back buffer)
					0,						  // offset into buffer
					outCurve.size()	// number of vertices in buffer
				);

				glDrawArrays(GL_LINE_STRIP,   // type of drawing (rendered to back buffer)
					0,						  // offset into buffer
					outCurve.size()	// number of vertices in buffer
				);
				glDrawArrays(GL_LINE_STRIP,   // type of drawing (rendered to back buffer)
					0,						  // offset into buffer
					controlPoints.size()	// number of vertices in buffer
				);
			
			
				glViewport(g_width/2, 0, g_width / 2, g_height);
				program = &textureShader;
				program->use();
				setUniformMat4f(program->uniformLocation("model"), g_M, true);
				setUniformMat4f(program->uniformLocation("view"), g_V, true);
				setUniformMat4f(program->uniformLocation("projection"), g_P, true);
				setUniformVec3f(textureShader.uniformLocation("lightPosition"), lightPosition);
				setUniformVec3f(textureShader.uniformLocation("lightColor"), lightColor);
				glUniform1i(textureShader.uniformLocation("change_shader"), 2);
				vao.bind();
				//if (has_changed) {
				
					 
					//{
						geometry::OBJMesh pot = surfaceofRevolution(outCurve, 4);
					
						//Calculate vertex normals if they are not included in .obj file
						//if (pot.normals.size() == 0 && pot.textureCoords.empty()) {
							auto normals =
								geometry::calculateVertexNormals(pot.triangles, pot.vertices);

							auto vboData = opengl::makeConsistentVertexNormalIndices(pot, normals);
							//auto vboData = opengl::makeConsistentVertexTextureCoordNormalIndices(meshData, normals);
							totalIndices =
								opengl::setup_vao_and_buffers(vao, indexBuffer, vertexBuffer, vboData);
						//}

						//write2OBJ(pot);

					//}
					has_changed = false;
					//vao.bind();
				//}
				//setUniformVec3f(textureShader.uniformLocation("lightPosition"), lightPosition);
				//setUniformVec3f(textureShader.uniformLocation("lightColor"), lightColor);
				//glUniform1i(textureShader.uniformLocation("change_shader"), 2);
				vao.bind();
				glDrawElements(GL_TRIANGLES,
					totalIndices,    // # of triangles * 3
					GL_UNSIGNED_INT, // type of indices
					(void *)0        // offset
				);
			

			


			glfwSwapBuffers(window); // swaps back buffer to front for drawing to screen
			glfwPollEvents();        // will process event queue and carry on
		}

		// cleaup window, and glfw before exit
		glfwDestroyWindow(window);
		glfwTerminate();

		return EXIT_SUCCESS;
	}

