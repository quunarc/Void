#version 460
#extension GL_EXT_nonuniform_qualifier : enable

//Pipeline layout needs changing over the default one.
layout(location = 0) in vec3 vColour;

layout(location = 0) out vec4 outColour;

void main()
{
   outColour = vec4(vColour, 1.0);
}
