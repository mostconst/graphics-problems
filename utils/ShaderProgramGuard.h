﻿#pragma once
#include "MovableHandle.h"

class ShaderProgramGuard
{
public:
    ShaderProgramGuard();
    explicit ShaderProgramGuard(unsigned int id);
    ShaderProgramGuard(const ShaderProgramGuard& other) = delete;
    ShaderProgramGuard(ShaderProgramGuard&& other) noexcept = default;
    ShaderProgramGuard& operator=(const ShaderProgramGuard& other) = delete;
    ShaderProgramGuard& operator=(ShaderProgramGuard&& other) noexcept = default;
    ~ShaderProgramGuard();
    unsigned int GetRaw() const;

private:
    MovableHandle handle;
};
