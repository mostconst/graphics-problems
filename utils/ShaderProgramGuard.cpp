#include "ShaderProgramGuard.h"
#include "glad/glad.h"

namespace nsk_cg
{
ShaderProgramGuard::ShaderProgramGuard()
    : handle(0)
{
}

ShaderProgramGuard::ShaderProgramGuard(const unsigned id)
    : handle(id)
{
}

ShaderProgramGuard::~ShaderProgramGuard()
{
    glDeleteProgram(handle.GetRaw());
}

unsigned ShaderProgramGuard::GetRaw() const
{
    return handle.GetRaw();
}
}
