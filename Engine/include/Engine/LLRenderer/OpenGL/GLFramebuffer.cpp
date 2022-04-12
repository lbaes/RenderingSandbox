#include "GLFramebuffer.h"
#include "glad/glad.h"

void Eng::ogl::GLFramebuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, ID);
}

void Eng::ogl::GLFramebuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
