
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <shader_m.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

// tamanio de la ventana ancho x alto
const unsigned int width = 800; //ancho
const unsigned int height = 600; //alto

int seleccion = 0;

void framebuffer_size_callback(GLFWwindow* window, int with, int height);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void determinarSentidoHorizontal( float posicionInicial_X , float rango_X , int *sentido );
void determinarSentidoVertical( float posicionInicial_Y, float rango_Y, int *up_or_down );

int main()
{

 	glfwInit(); //esto inicializa glfw 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// para ejecutar en MAC
	#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif

	// este metodo crea la ventana x tanto de ancho y alto junto con el nombre
	// y devuelve un objeto ventana
	GLFWwindow* window = glfwCreateWindow( width , height , "LearnOpenGL", NULL, NULL);

	if (!window){
	    cout << "Failed to create GLFW window" << endl;
	    glfwTerminate();
	    return -1;
	}

	// este indica que el contexto de nuestra ventana "window"
	glfwMakeContextCurrent(window);

	// llama a este metodo cada vez que el usuario cambia el tamaño
	// de la ventana
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

	// Este metodo carga la direccion de los punteros de funcion de opengl
	// esto es especifico del sistema operativo glfwGetProcAddress define
	// la funcion correcta
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
	    cout << "Failed to initialize GLAD" << endl;
	    return -1;
	} 

	// build and compile our shader program
    // ------------------------------------
    // you can name your shader files however you like
    Shader nuestroShader("../shaders/vertexShader.vs", "../shaders/fragmentShader.fs"); 

	float verticesTriangulo[] = {
	// Triangulo           //color
    -0.5f, -0.5f, 0.0f,		1.0f, 1.0f, 0.0f,	
     0.0f, 0.5f, 0.0f,		1.0f, 0.0f, 1.0f,		
     0.5f, -0.5f, 0.0f,	    1.0f, 0.8f, 1.0f
	}; 

	float verticesCuadrado[] = {
	// Cuadrado 			//color
    -0.5, 0.5f, 0.0f, 		1.0f, 1.0f, 0.6f,	
    0.5f, 0.5f, 0.0f, 		0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, 0.0f, 	0.0f, 1.0f, 1.0f,
    0.5f, -0.5f, 0.0f, 		0.5f, 0.5f, 0.9f
	}; 

	float verticesPentagono[] = {
	// Pentagono 			//color
	-0.5f, 0.20f, 0.0f, 	1.0f, 1.0f, 0.6f,				//0
	-0.28f, -0.5f, 0.0f, 	0.0f, 1.0f, 0.0f,				//1
	0.28f, -0.5f, 0.0f, 	0.0f, 1.0f, 1.0f,				//2
	0.5f, 0.20f, 0.0f, 		0.5f, 0.5f, 0.9f,				//3 
	0.0f, 0.5f, 0.0f, 		1.0f, 0.0f, 1.0f				//4
	};

	// Indices para dibujar Cuadrado
	unsigned int indicesCuadrado[] = {
		0, 1, 2,
		2, 3, 1
	};

	// Indice para dibujar Pentagono
	unsigned int indicesPentagono[] = {
		0, 1, 4,
		4, 2, 3,
		1, 4, 2
	};

	int cantidadDeObjetosVbo = 3;
	int cantidadDeIndicesEbo = 2;
	unsigned int VBOs[cantidadDeObjetosVbo], VAOs[cantidadDeObjetosVbo], EBOs[cantidadDeIndicesEbo];

	glGenVertexArrays(cantidadDeObjetosVbo, VAOs);
	glGenBuffers(cantidadDeObjetosVbo, VBOs);
	glGenBuffers(cantidadDeIndicesEbo, EBOs);

	// vincular VAO #0
	glBindVertexArray(VAOs[0]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesTriangulo), verticesTriangulo, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	//***************************************************************************

	// Vincular siguiente VAO #1
	glBindVertexArray(VAOs[1]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCuadrado), verticesCuadrado, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesCuadrado), indicesCuadrado, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	//*****************************************************************************

	// Vincular siguiente VAO #2
	glBindVertexArray(VAOs[2]);

	glBindBuffer(GL_ARRAY_BUFFER, VBOs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesPentagono), verticesPentagono, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesPentagono), indicesPentagono, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	// figuras en general
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;

	// modelo de cada figura
	glm::mat4 modelCuadrado;
	glm::mat4 modelTriangulo;
	glm::mat4 modelPentagono;

	float escalar = 0.5;
	float limite = 3.0;

	float velocidad = 0.0005;

	// variables necesarias para el triangulo
	float rangoDeMovimientoEjeX = limite;
	float rangoDeMovimientoEjeY = limite;
 	float posicionInicialDerechaTriangulo_X = rangoDeMovimientoEjeX;
	float posicionInicialDerechaTriangulo_Y = 0.0;
	int sentidoTriangulo = 0;
	int subir_or_bajar_triangulo = 0;

	// variables necesarias para el pentagono
 	float posicionInicialIzquierdaPentagono_X = -rangoDeMovimientoEjeX;
	float posicionInicialIzquierdaPentagono_Y = 0.0;
	int sentidoPentagono = 1;
	int subir_or_bajar_pentagono = 1;


	//*******************************************************************************
	
	while( !glfwWindowShouldClose(window) ){

		float tiempo = glfwGetTime();

		// input ..
		processInput(window);
		glfwSetKeyCallback(window, key_callback);

		// render ..
		glClear(GL_COLOR_BUFFER_BIT);

	    nuestroShader.use();

	    model = glm::mat4(1.0f);
	    view = glm::mat4(1.0f);
	    projection = glm::mat4(1.0f);

	    // establece la vista y proyeccion de todas las figuras
	    model = glm::scale(model, glm::vec3( escalar, escalar, escalar ));

	    // asignar escalar a los modelos individuales para el caso 3
	    modelCuadrado = model;
	    modelTriangulo = model;
	    modelPentagono = model;

	    view = glm::translate(view, glm::vec3(0.0f, 0.0f,-6.0f));
	    projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	    nuestroShader.setMat4("model", model);
	    nuestroShader.setMat4("view", view);
	    nuestroShader.setMat4("projection", projection);

	    switch( seleccion ){

	    	case 0: // triangulo
	    		glClearColor(0.0f,0.0f,0.6f,0.0f);
	    		glBindVertexArray(VAOs[0]);
	    		model = glm::rotate(model, float(glm::radians(90.0f*tiempo)) , glm::vec3(0.0f,0.0f,1.0f) );
	    		nuestroShader.setMat4("model", model);
	    		glDrawArrays(GL_TRIANGLES,0,3);
	    		break;

	    	case 1: //cuadrado
	    		glClearColor(0.0f,0.4f,0.0f,0.0f);
	    		glBindVertexArray(VAOs[1]);
	    		model = glm::rotate(model, float(glm::radians(90.0f*tiempo)) , glm::vec3(0.0f,0.0f,1.0f) );
	    		nuestroShader.setMat4("model", model);
	    		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	    		break;

	    	case 2: // pentagono - cuadrado - triangulo
	    		glClearColor(0.3f,0.0f,0.0f,0.0f);
	    		glBindVertexArray(VAOs[1]);
	    		modelCuadrado = glm::rotate(model, float(glm::radians(90.0f*tiempo)) , glm::vec3(0.0f,0.0f,1.0f) );
	    		nuestroShader.setMat4("model", modelCuadrado);
	    		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

	    		// figura de lado derecho iniciar traslacion

	    		// trasladando de derecha a izquierda 
				if( sentidoTriangulo < 1 ){
					modelTriangulo = glm::translate(modelTriangulo, glm::vec3(posicionInicialDerechaTriangulo_X, posicionInicialDerechaTriangulo_Y, 0.0));
					posicionInicialDerechaTriangulo_X -= velocidad;
				}else{ // viceversa
					modelTriangulo = glm::translate(modelTriangulo, glm::vec3(posicionInicialDerechaTriangulo_X, posicionInicialDerechaTriangulo_Y, 0.0));
					posicionInicialDerechaTriangulo_X += velocidad;
				}

				// trasladando de abajo hacia arriba
				if( subir_or_bajar_triangulo < 1 )
					posicionInicialDerechaTriangulo_Y += velocidad;
				else
					posicionInicialDerechaTriangulo_Y -= velocidad;

				determinarSentidoHorizontal( posicionInicialDerechaTriangulo_X, rangoDeMovimientoEjeX, &sentidoTriangulo );
				determinarSentidoVertical( posicionInicialDerechaTriangulo_Y, rangoDeMovimientoEjeY, &subir_or_bajar_triangulo);

				modelTriangulo = glm::rotate( modelTriangulo, float(glm::radians(90.0f*tiempo)), glm::vec3(0.0,0.0,1.0));
				nuestroShader.setMat4("model", modelTriangulo);

	    		glBindVertexArray(VAOs[0]);
	    		glDrawArrays(GL_TRIANGLES,0,3);

	    		//************************** fin de la traslacion de la figura derecha *******************

	    		// figura de lado izquierdo iniciar traslacion

	    		if( sentidoPentagono < 1 ){
					modelPentagono = glm::translate(modelPentagono, glm::vec3(posicionInicialIzquierdaPentagono_X, posicionInicialIzquierdaPentagono_Y, 0.0));
					posicionInicialIzquierdaPentagono_X -= velocidad;
				}else{ // viceversa
					modelPentagono = glm::translate(modelPentagono, glm::vec3(posicionInicialIzquierdaPentagono_X, posicionInicialIzquierdaPentagono_Y, 0.0));
					posicionInicialIzquierdaPentagono_X += velocidad;
				}

				if( subir_or_bajar_pentagono < 1 )
					posicionInicialIzquierdaPentagono_Y += velocidad;
				else
					posicionInicialIzquierdaPentagono_Y -= velocidad;

				determinarSentidoHorizontal( posicionInicialIzquierdaPentagono_X, rangoDeMovimientoEjeX, &sentidoPentagono);
				determinarSentidoVertical( posicionInicialIzquierdaPentagono_Y, rangoDeMovimientoEjeY, &subir_or_bajar_pentagono);

				modelPentagono = glm::rotate( modelPentagono, float(glm::radians(90.0f*tiempo)), glm::vec3(0.0,0.0,1.0));
				nuestroShader.setMat4("model", modelPentagono);

				glBindVertexArray(VAOs[2]);
	    		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

	    		break;

	    }// fin swith

    	glfwSwapBuffers(window);
    	glfwPollEvents();
	
	}// fin render loot

	glDeleteVertexArrays(cantidadDeObjetosVbo, VAOs);
    glDeleteBuffers(cantidadDeObjetosVbo, VBOs);
    glDeleteBuffers(cantidadDeIndicesEbo, EBOs);

    glfwTerminate(); // fin ventana

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
	glViewport(0,0,width,height);
}

void processInput(GLFWwindow *window){

	if( glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS )
		glfwSetWindowShouldClose(window,true);
}

void determinarSentidoHorizontal( float posicionInicial_X , float rango_X , int *sentido ){

	if( posicionInicial_X <= (-rango_X) ) // de izquierda a derecha
		*sentido = 1;
	else if( posicionInicial_X >= rango_X ) // de derecha a izquierda
		*sentido = 0;

}

void determinarSentidoVertical( float posicionInicial_Y, float rango_Y, int *up_or_down ){

	if( posicionInicial_Y > rango_Y ) // de arriba -> abajo
		*up_or_down = 1;
	else if ( posicionInicial_Y < (-rango_Y) ) // viceversa
		*up_or_down = 0;

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS){
        seleccion++;
        if( seleccion > 2 )
        	seleccion = 0;
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS){
    	seleccion--;
    	if( seleccion < 0 )
        	seleccion = 2;
    }
}