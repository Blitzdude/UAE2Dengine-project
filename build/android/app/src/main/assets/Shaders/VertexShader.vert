#version 100
attribute vec4 a_vertexPosition;
attribute vec2 a_texCoord;
uniform mat4 u_mvpMatrix;
varying vec2 v_texCoord;
void main()
{
	gl_Position = u_mvpMatrix * a_vertexPosition;
	v_texCoord = a_texCoord;
}