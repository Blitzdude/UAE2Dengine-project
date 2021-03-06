#include "OGL/Shader.h"
#include <core/Log.h>
#include <core/IOManager.h>
#include <fstream>


namespace engine {
	Shader::Shader(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, void* manager) 
	{
		CreateShaderProgram(vertexShaderFilePath, fragmentShaderFilePath, manager);
	}

	Shader::~Shader()
	{
		dispose();
	}

	void Shader::CreateShaderProgram(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath, void* manager)
	{

		std::string vertSource;
		std::string fragSource;


        if (!IOManager::readFileToBuffer(vertexShaderFilePath, vertSource, manager)) {
			LOGE("Failed to load vertex shader file\n");
		}

		
		if (!IOManager::readFileToBuffer(fragmentShaderFilePath, fragSource, manager)) {
			LOGE("Failed to load fragment shader file\n");
		}

		compileShadersFromSource(vertSource.c_str(), fragSource.c_str());

		// we do not want to link the shaders yet if we need to add attributes
	}

	void Shader::compileShadersFromSource(const char * vertexSource, const char * fragmentSource)
	{
		//Get a program object.
		m_programId = glCreateProgram();

		//Create the vertex shader object, and store its ID
		m_vertexShaderId = glCreateShader(GL_VERTEX_SHADER);

		//Create the fragment shader object, and store its ID
		m_fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);

		//Compile each shader
		compileShader(vertexSource, "Vertex Shader", m_vertexShaderId);
		compileShader(fragmentSource, "Fragment Shader", m_fragmentShaderId);

	}

	void Shader::compileShader(const char * source, const std::string & name, GLuint id)
	{

		//tell opengl that we want to use fileContents as the contents of the shader file
		glShaderSource(id, 1, &source, NULL);

		//compile the shader
		glCompileShader(id);

		//check for errors
		GLint success = 0;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE) //error checking
		{
			GLint maxLength = 0;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			GLchar errorLog[128];
			glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);

			//Exit with failure.
			glDeleteShader(id); //Don't leak the shader.

			LOGI("Shader %s failed to compile: %s\n", name.c_str(), &(errorLog[0]));
		}
	}

	void Shader::linkShaders()
	{
		//Attach our shaders to our program
		glAttachShader(m_programId, m_vertexShaderId);
		glAttachShader(m_programId, m_fragmentShaderId);

		// linking attributes is done during init in main cpp. 

		//Link our program
		glLinkProgram(m_programId);

		//Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(m_programId, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &maxLength);

			//The maxLength includes the NULL character
			GLchar errorLog[128];
			glGetProgramInfoLog(m_programId, maxLength, &maxLength, &errorLog[0]);

			//We don't need the program anymore.
			glDeleteProgram(m_programId);
			//Don't leak shaders either.
			glDeleteShader(m_vertexShaderId);
			glDeleteShader(m_fragmentShaderId);

			//print the error log and quit
			LOGI("Shaders failed to link! %s\n", &(errorLog[0]));
		}

		//Always detach shaders after a successful link.
		glDetachShader(m_programId, m_vertexShaderId);
		glDetachShader(m_programId, m_fragmentShaderId);
		glDeleteShader(m_vertexShaderId);
		glDeleteShader(m_fragmentShaderId);
	}

	void Shader::use()
	{
		glUseProgram(m_programId);
		// enable all attributes
		for (int i = 0; i < m_numAttributes; i++) {
			glEnableVertexAttribArray(i);
		}
	}

	void Shader::unUse()
	{
		glUseProgram(0);
		// disable all attributes
		for (int i = 0; i < m_numAttributes; i++) {
			glDisableVertexAttribArray(i);
		}
	}

	// Adds attributes to our shader, Should be called between compilign and linking
	void Shader::addAttribute(const std::string & attributeName)
	{
		glBindAttribLocation(m_programId, m_numAttributes++, attributeName.c_str());
	}

	GLuint Shader::getUniformLocation(const char * const uniformName)
	{
		GLint location = glGetUniformLocation(m_programId, uniformName);
		if (location == -1) {
			LOGE("Uniform %s not found in shader!", uniformName);
			return NULL;
		}
		return (GLuint)location;
	}

	GLuint Shader::getAttributeLocation(const char * const attributeName)
	{
		GLint location = glGetAttribLocation(m_programId, attributeName);
		if (location == -1) {
			LOGE("Attribute %s not found in shader!", attributeName);
			return NULL;
		}
		return (GLuint)location;
	}
	// required for font reading
	void Shader::setUniform(const char * uniformName, const GLfloat a_V0)
	{
		glUniform1f(glGetUniformLocation(m_programId, uniformName), a_V0);
	}

	// required for font reading
	void Shader::setUniform(const char * const uniformName, const GLfloat a_V0, const GLfloat a_V1, const GLfloat a_V2)
	{
		glUniform3f(glGetUniformLocation(m_programId, uniformName), a_V0, a_V1, a_V2);
	}

	void Shader::setProjection(const glm::mat4 a_Projection)
	{
		glUniformMatrix4fv(glGetUniformLocation(m_programId, "projection"), 1, GL_FALSE, glm::value_ptr(a_Projection));
	}

	void Shader::dispose()
	{
		if (m_programId)
			glDeleteProgram(m_programId);
	}
}


