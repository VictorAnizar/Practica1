#include <stdio.h>
#include <string.h>
#include <iostream>
#include <glew.h>
#include <glfw3.h>
#include <random>
#include <chrono>
//Dimensiones de la ventana
const int WIDTH = 800, HEIGHT = 600;
//Son  vars de gl
//el primero es la declaracion del vao -> conjunto de datos de vertices
//el segundo, vbo -> para todos los datos que hay en el aregloi de vertices, se asignan en memoria, y el vbo es el que trae info de memoria de datosdeclarados
//el tercero, shader -> programa de shader. esta formado por un path de archivos(vertexShader y fragmentShader)
GLuint VAO, VBO, shader;
float rojo=	1.0f, verde = 1.0f, azul = 1.0f;
float duracionCiclo = 2.0f; // Duración total del ciclo en segundos

//Los shader es otro lenguaje de proigamacion pero para el GPU (glcl, hlcl, cg)
//Vertex Shader
//se recomienda una linea de comentario de la version
//la segunda linea -> todods los vertexshader deben de tener una var de entrada de cualer tipo de dato aunque se recomienda que sea un vec3 porque ee convierte mas adelante en la info de las cordenadas XYZ
//lo que esta antes del in, es una buena practica que dice que dicha variable de entrada, se asignara a la localidad de moemoria 0 
//la tercer funcion main, debe de retornar gl_position, los datos de salida son los datos de cordenada spacial XYZ y se debe de agregar un valor de 1 al final 
//Es 1 porque los calculos matematicos de la gpu son por matrices de dimension 4
static const char* vShader = "						\n\
#version 330										\n\
layout (location =0) in vec3 pos;					\n\
void main()											\n\
{													\n\
gl_Position=vec4(pos.x,pos.y,pos.z,1.0f); 			\n\
}";
//recibir Vcolor y dar de salida color
//fragmenthaer, procesa info de color, a nivel gpu, es considerado como color plano, color luz y textura
//La primera es ñla version
//En fshader siempre hay una var de salida
//la segunda linea es un dato de salida de un vector de 4 dimensiones
//Dentro del main se devuelve la variable color. Como es color aqui si es colores RGBA, cuando sea colores, el 4to valor si puede ser diferente de 1
static const char* fShader = "						\n\
#version 330										\n\
out vec4 color;										\n\
void main()											\n\
{													\n\
	color = vec4(0.0f,1.0f,0.9f,1.0f);	 			\n\
}";


//Idealmente eberia de ir en la funcion mesh
void CrearTriangulo()
{
	GLfloat vertices[] = {
		//----------- V (\)
		-0.7f, 0.6f,0.0f,
		-0.6f, 0.7f,0.0f,
		-0.5f, 0.4f,0.0f,

		-0.6f, 0.7f,0.0f,
		-0.4f, 0.5f,0.0f,
		-0.5f, 0.4f,0.0f,
		//triangulo V
		-0.5f, 0.4f,0.0f,
		-0.4f, 0.5f,0.0f,
		-0.3f, 0.4f,0.0f,

		//V (/)
		-0.4f, 0.5f,0.0f,
		-0.2f, 0.7f,0.0f,
		-0.3f, 0.4f,0.0f,

		-0.3f, 0.4f,0.0f,
		-0.1f, 0.6f,0.0f,
		-0.2f, 0.7f,0.0f,

		//---------------- A (/)
		0.0f, 0.5f,0.0f,
		0.1f, 0.4f,0.0f,
		0.3f, 0.8f,0.0f,

		0.1f, 0.4f,0.0f,
		0.4f, 0.7f,0.0f,
		0.3f, 0.8f,0.0f,
		// Triangulo A
		0.3f, 0.8f,0.0f,
		0.4f, 0.7f,0.0f,
		0.5f, 0.8f,0.0f,
		//A (\)
		0.4f, 0.7f,0.0f,
		0.5f, 0.8f,0.0f,
		0.7f, 0.4f,0.0f,

		0.7f, 0.4f,0.0f,
		0.8f, 0.5f,0.0f,
		0.5f, 0.8f,0.0f,
		//A (-)
		0.15f, 0.6f,0.0f,
		0.15f, 0.55f,0.0f,
		0.65f, 0.55f,0.0f,

		0.65f, 0.55f,0.0f,
		0.15f, 0.6f,0.0f,
		0.65f, 0.6f,0.0f,

		//---------------- M (| izq)
		-0.6f, -0.5f,0.0f,
		-0.5f, -0.5f,0.0f,
		-0.6f, -0.2f,0.0f,

		-0.6f, -0.2f,0.0f,
		-0.5f, -0.5f,0.0f,
		-0.5f, -0.2f,0.0f,

		// M (| der)
		-0.2f, -0.2f,0.0f,
		-0.2f, -0.5f,0.0f,
		-0.1f, -0.5f,0.0f,

		-0.1f, -0.5f,0.0f,
		-0.1f, -0.2f,0.0f,
		-0.2f, -0.2f,0.0f,

		//Triangulo M
		-0.4f, -0.4f,0.0f,
		-0.3f, -0.4f,0.0f,
		-0.35f, -0.35f,0.0f,

		// M (\)
		-0.55f, -0.25f,0.0f,
		-0.5f, -0.2f,0.0f,
		-0.4f, -0.4f,0.0f,

		-0.4f, -0.4f,0.0f,
		-0.35f, -0.35f,0.0f,
		-0.5f, -0.2f,0.0f,

		// M(/)

		-0.35f, -0.35f,0.0f,
		-0.2f, -0.2f,0.0f,
		-0.3f, -0.4f,0.0f,

		-0.3f, -0.4f,0.0f,
		-0.15f, -0.25f,0.0f,
		-0.2f, -0.2f,0.0f,

	};
	glGenVertexArrays(1, &VAO); //generar 1 VAO
	glBindVertexArray(VAO);//asignar VAO

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //pasarle los datos al VBO asignando tamano, los datos y en este caso es estático pues no se modificarán los valores
		//(indice, quiero que me agrupes los datos de 3 en 3,... ,...,cuantos datos hay por renglon, ...)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);//Stride en caso de haber datos de color por ejemplo, es saltar cierta cantidad de datos
		//SSe le envia al shader el dato, el indice del attribpointer
		glEnableVertexAttribArray(0);
		//agregar valores a vèrtices y luego declarar un nuevo vertexAttribPointer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

}

//las fumciones deben de esyat en shader.h

//estas funciones no se modifican en si 
//Que programa le paos los datos, info
void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) //Función para agregar los shaders a la tarjeta gráfica
//the Program recibe los datos de theShader
{
	GLuint theShader = glCreateShader(shaderType);//theShader es un shader que se crea de acuerdo al tipo de shader: vertex o fragment
	const GLchar* theCode[1];
	theCode[0] = shaderCode;//shaderCode es el texto que se le pasa a theCode
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);//longitud del texto
	glShaderSource(theShader,1, theCode, codeLength);//Se le asigna al shader el código
	glCompileShader(theShader);//Se comila el shader
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	//verificaciones y prevención de errores
	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al compilar el shader %d es: %s \n",shaderType, eLog);
		return;
	}
	glAttachShader(theProgram, theShader);//Si no hubo problemas se asigna el shader a theProgram el cual asigna el código a la tarjeta gráfica
}

//La unica modificacion, solo es agregar las nuevas variables que se le agrega al shader
void CompileShaders() {
	shader= glCreateProgram(); //se crea un programa vacío 
	if (!shader)
	{
		printf("Error creando el shader");
		return;
	}
	//se le pasa la info y el topo de shader que es
	AddShader(shader, vShader, GL_VERTEX_SHADER);//Agregar vertex shader
	AddShader(shader, fShader, GL_FRAGMENT_SHADER);//Agregar fragment shader
	//Existen mas tipos de shader aparte de vertex y fragment
	// 
	// 
	//Para terminar de linkear el programa y ver que no tengamos errores
	GLint result = 0;
	GLchar eLog[1024] = { 0 };
	glLinkProgram(shader);//se linkean los shaders a la tarjeta gráfica
	 //verificaciones y prevención de errores
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al linkear es: %s \n", eLog);
		return;
	}
	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("EL error al validar es: %s \n", eLog);
		return;
	}
	//se iran agregando variales de tipo uniform a partr de la P3



}
int main()
{
	//Inicialización de GLFW
	if (!glfwInit())
	{
		printf("Falló inicializar GLFW");
		glfwTerminate();
		return 1;
	}
	//Asignando variables de GLFW y propiedades de ventana
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//para solo usar el core profile de OpenGL y no tener retrocompatibilidad
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//CREAR VENTANA
	GLFWwindow *mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Primer ventana", NULL, NULL);

	if (!mainWindow)
	{
		printf("Fallo en crearse la ventana con GLFW");
		glfwTerminate();
		return 1;
	}
	//Obtener tamaño de Buffer
	int BufferWidth, BufferHeight;
	glfwGetFramebufferSize(mainWindow, &BufferWidth, &BufferHeight);

	//asignar el contexto
	glfwMakeContextCurrent(mainWindow);

	//permitir nuevas extensiones
	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		printf("Falló inicialización de GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	// Asignar valores de la ventana y coordenadas
	//Asignar Viewport
	glViewport(0, 0, BufferWidth, BufferHeight);

	//Crear tríangulo
	CrearTriangulo();
	CompileShaders();
	// Inicializar generador de números aleatorios
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> distrib(0.0f, 1.0f);

	// Tiempo inicial
	auto inicio = std::chrono::steady_clock::now();

	// Duración total para el ciclo completo
	auto duracionTotal = std::chrono::duration<float>(duracionCiclo);

	//Loop mientras no se cierra la ventana
	while (!glfwWindowShouldClose(mainWindow))
	{
		// Calcular tiempo transcurrido
		auto ahora = std::chrono::steady_clock::now();
		float tiempoTranscurrido = std::chrono::duration<float>(ahora - inicio).count();

		// Determinar el color de fondo cada intervalo
		if (std::fmod(tiempoTranscurrido, duracionCiclo) < 0.1f) { // Cambiar el color aproximadamente cada 0.1 segundos
			float r = distrib(gen);
			float g = distrib(gen);
			float b = distrib(gen);
			glClearColor(r, g, b, 1.0f); // Establecer el color de fondo aleatorio
		}

		//Recibir eventos del usuario
		glfwPollEvents();

		//Limpiar la ventana
		//glClearColor(rojo, verde, azul,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES,0,63);
		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
		// Esperar un corto periodo antes de actualizar (aproximadamente 60 FPS)
		glfwWaitEventsTimeout(1.0 / 60.0);
	}


	return 0;
}