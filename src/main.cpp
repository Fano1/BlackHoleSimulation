#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

#include <GLFW/glfw3.h>

#ifndef APIENTRYP
#define APIENTRYP APIENTRY*
#endif

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

using GLchar = char;
using GLsizeiptr = ptrdiff_t;

#ifndef GL_ARRAY_BUFFER
#define GL_ARRAY_BUFFER 0x8892
#endif
#ifndef GL_STATIC_DRAW
#define GL_STATIC_DRAW 0x88E4
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER 0x8B30
#endif
#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER 0x8B31
#endif
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS 0x8B81
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS 0x8B82
#endif
#ifndef GL_INFO_LOG_LENGTH
#define GL_INFO_LOG_LENGTH 0x8B84
#endif

typedef GLuint(APIENTRYP PFNGLCREATESHADERPROC)(GLenum type);
typedef void(APIENTRYP PFNGLSHADERSOURCEPROC)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
typedef void(APIENTRYP PFNGLCOMPILESHADERPROC)(GLuint shader);
typedef void(APIENTRYP PFNGLGETSHADERIVPROC)(GLuint shader, GLenum pname, GLint* params);
typedef void(APIENTRYP PFNGLGETSHADERINFOLOGPROC)(GLuint shader, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
typedef GLuint(APIENTRYP PFNGLCREATEPROGRAMPROC)(void);
typedef void(APIENTRYP PFNGLATTACHSHADERPROC)(GLuint program, GLuint shader);
typedef void(APIENTRYP PFNGLLINKPROGRAMPROC)(GLuint program);
typedef void(APIENTRYP PFNGLGETPROGRAMIVPROC)(GLuint program, GLenum pname, GLint* params);
typedef void(APIENTRYP PFNGLGETPROGRAMINFOLOGPROC)(GLuint program, GLsizei maxLength, GLsizei* length, GLchar* infoLog);
typedef void(APIENTRYP PFNGLDELETESHADERPROC)(GLuint shader);
typedef void(APIENTRYP PFNGLDELETEPROGRAMPROC)(GLuint program);
typedef void(APIENTRYP PFNGLUSEPROGRAMPROC)(GLuint program);
typedef GLint(APIENTRYP PFNGLGETUNIFORMLOCATIONPROC)(GLuint program, const GLchar* name);
typedef void(APIENTRYP PFNGLUNIFORM1FPROC)(GLint location, GLfloat v0);
typedef void(APIENTRYP PFNGLUNIFORM1IPROC)(GLint location, GLint v0);
typedef void(APIENTRYP PFNGLUNIFORM2FPROC)(GLint location, GLfloat v0, GLfloat v1);
typedef void(APIENTRYP PFNGLUNIFORM3FPROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
typedef void(APIENTRYP PFNGLUNIFORM1FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void(APIENTRYP PFNGLUNIFORM3FVPROC)(GLint location, GLsizei count, const GLfloat* value);
typedef void(APIENTRYP PFNGLGENVERTEXARRAYSPROC)(GLsizei n, GLuint* arrays);
typedef void(APIENTRYP PFNGLBINDVERTEXARRAYPROC)(GLuint array);
typedef void(APIENTRYP PFNGLGENBUFFERSPROC)(GLsizei n, GLuint* buffers);
typedef void(APIENTRYP PFNGLBINDBUFFERPROC)(GLenum target, GLuint buffer);
typedef void(APIENTRYP PFNGLBUFFERDATAPROC)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
typedef void(APIENTRYP PFNGLENABLEVERTEXATTRIBARRAYPROC)(GLuint index);
typedef void(APIENTRYP PFNGLVERTEXATTRIBPOINTERPROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
typedef void(APIENTRYP PFNGLDRAWARRAYSPROC)(GLenum mode, GLint first, GLsizei count);
typedef void(APIENTRYP PFNGLDELETEBUFFERSPROC)(GLsizei n, const GLuint* buffers);
typedef void(APIENTRYP PFNGLDELETEVERTEXARRAYSPROC)(GLsizei n, const GLuint* arrays);

static PFNGLCREATESHADERPROC glCreateShaderPtr;
static PFNGLSHADERSOURCEPROC glShaderSourcePtr;
static PFNGLCOMPILESHADERPROC glCompileShaderPtr;
static PFNGLGETSHADERIVPROC glGetShaderivPtr;
static PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLogPtr;
static PFNGLCREATEPROGRAMPROC glCreateProgramPtr;
static PFNGLATTACHSHADERPROC glAttachShaderPtr;
static PFNGLLINKPROGRAMPROC glLinkProgramPtr;
static PFNGLGETPROGRAMIVPROC glGetProgramivPtr;
static PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLogPtr;
static PFNGLDELETESHADERPROC glDeleteShaderPtr;
static PFNGLDELETEPROGRAMPROC glDeleteProgramPtr;
static PFNGLUSEPROGRAMPROC glUseProgramPtr;
static PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocationPtr;
static PFNGLUNIFORM1FPROC glUniform1fPtr;
static PFNGLUNIFORM1IPROC glUniform1iPtr;
static PFNGLUNIFORM2FPROC glUniform2fPtr;
static PFNGLUNIFORM3FPROC glUniform3fPtr;
static PFNGLUNIFORM1FVPROC glUniform1fvPtr;
static PFNGLUNIFORM3FVPROC glUniform3fvPtr;
static PFNGLGENVERTEXARRAYSPROC glGenVertexArraysPtr;
static PFNGLBINDVERTEXARRAYPROC glBindVertexArrayPtr;
static PFNGLGENBUFFERSPROC glGenBuffersPtr;
static PFNGLBINDBUFFERPROC glBindBufferPtr;
static PFNGLBUFFERDATAPROC glBufferDataPtr;
static PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArrayPtr;
static PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointerPtr;
static PFNGLDRAWARRAYSPROC glDrawArraysPtr;
static PFNGLDELETEBUFFERSPROC glDeleteBuffersPtr;
static PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArraysPtr;

static void* loadGlProc(const char* name)
{
    void* p = reinterpret_cast<void*>(glfwGetProcAddress(name));
    if (!p)
        p = reinterpret_cast<void*>(wglGetProcAddress(name));
    return p;
}

template <typename T>
static bool loadOne(T& dst, const char* name)
{
    dst = reinterpret_cast<T>(loadGlProc(name));
    if (!dst)
        std::fprintf(stderr, "Missing OpenGL function: %s\n", name);
    return dst != nullptr;
}

static bool loadOpenGl()
{
    bool ok = true;
#define LOAD_GL(name) ok = loadOne(name##Ptr, #name) && ok
    LOAD_GL(glCreateShader);
    LOAD_GL(glShaderSource);
    LOAD_GL(glCompileShader);
    LOAD_GL(glGetShaderiv);
    LOAD_GL(glGetShaderInfoLog);
    LOAD_GL(glCreateProgram);
    LOAD_GL(glAttachShader);
    LOAD_GL(glLinkProgram);
    LOAD_GL(glGetProgramiv);
    LOAD_GL(glGetProgramInfoLog);
    LOAD_GL(glDeleteShader);
    LOAD_GL(glDeleteProgram);
    LOAD_GL(glUseProgram);
    LOAD_GL(glGetUniformLocation);
    LOAD_GL(glUniform1f);
    LOAD_GL(glUniform1i);
    LOAD_GL(glUniform2f);
    LOAD_GL(glUniform3f);
    LOAD_GL(glUniform1fv);
    LOAD_GL(glUniform3fv);
    LOAD_GL(glGenVertexArrays);
    LOAD_GL(glBindVertexArray);
    LOAD_GL(glGenBuffers);
    LOAD_GL(glBindBuffer);
    LOAD_GL(glBufferData);
    LOAD_GL(glEnableVertexAttribArray);
    LOAD_GL(glVertexAttribPointer);
    LOAD_GL(glDrawArrays);
    LOAD_GL(glDeleteBuffers);
    LOAD_GL(glDeleteVertexArrays);
#undef LOAD_GL
    return ok;
}

struct Vec2 {
    float x;
    float y;
};

struct Body {
    Vec2 p;
    Vec2 v;
    float radius;
    float mass;
    float heat;
    ImVec4 color;
};

struct SpawnSettings {
    float radiusRs = 8.0f;
    float sizeRs = 0.095f;
    float massEarth = 1.0f;
    float inwardC = 0.15f;
    float tangentialC = 0.18f;
    float angleDeg = 205.0f;
    ImVec4 color = ImVec4(1.0f, 0.38f, 0.88f, 1.0f);
};

struct BlackHolePreset {
    const char* name;
    float massSolar;
    float distance;
    const char* distanceUnit;
    float spin;
    const char* note;
};

struct KerrBounds {
    float horizonRs;
    float ergosphereRs;
    float photonProgradeRs;
    float photonRetrogradeRs;
    float iscoProgradeRs;
    float iscoRetrogradeRs;
    float diskInnerRs;
    float diskOuterRs;
};

static constexpr int MaxBodies = 32;
static constexpr double G = 6.67430e-11;
static constexpr double C = 299792458.0;
static constexpr double SolarMass = 1.98847e30;
static constexpr float Pi = 3.14159265358979323846f;
static constexpr float ThorneSpinLimit = 0.998f;

static const BlackHolePreset BlackHolePresets[] = {
    {"Sagittarius A* - EHT 2022", 4.0e6f, 8.0f, "kpc", 0.50f, "EHT mass/ring; spin is model-dependent"},
    {"M87* - EHT 2019", 6.5e9f, 16.8f, "Mpc", 0.90f, "EHT mass; high spin is a jet/accretion model seed"},
    {"Cygnus X-1 - 2021", 21.4f, 2.22f, "kpc", 0.97f, "stellar-mass X-ray binary; high spin constraint"},
    {"10 solar-mass baseline", 10.0f, 0.0f, "", 0.00f, "Schwarzschild teaching preset"},
};

static float clampSpin(float spin)
{
    return std::max(-ThorneSpinLimit, std::min(ThorneSpinLimit, spin));
}

static float kerrPhotonOrbitRs(float spin, bool prograde)
{
    float a = std::fabs(clampSpin(spin));
    float arg = prograde ? -a : a;
    float rg = 2.0f * (1.0f + std::cos((2.0f / 3.0f) * std::acos(arg)));
    return rg * 0.5f;
}

static float kerrIscoRs(float spin, bool prograde)
{
    float a = std::fabs(clampSpin(spin));
    float oneMinusA2 = std::max(0.0f, 1.0f - a * a);
    float z1 = 1.0f + std::cbrt(oneMinusA2) * (std::cbrt(1.0f + a) + std::cbrt(1.0f - a));
    float z2 = std::sqrt(3.0f * a * a + z1 * z1);
    float root = std::sqrt(std::max(0.0f, (3.0f - z1) * (3.0f + z1 + 2.0f * z2)));
    float rg = prograde ? (3.0f + z2 - root) : (3.0f + z2 + root);
    return rg * 0.5f;
}

static KerrBounds makeKerrBounds(float spin)
{
    float a = clampSpin(spin);
    float absA = std::fabs(a);
    KerrBounds b{};
    b.horizonRs = 0.5f * (1.0f + std::sqrt(std::max(0.0f, 1.0f - absA * absA)));
    b.ergosphereRs = 1.0f;
    b.photonProgradeRs = kerrPhotonOrbitRs(a, true);
    b.photonRetrogradeRs = kerrPhotonOrbitRs(a, false);
    b.iscoProgradeRs = kerrIscoRs(a, true);
    b.iscoRetrogradeRs = kerrIscoRs(a, false);
    b.diskInnerRs = (a >= 0.0f) ? b.iscoProgradeRs : b.iscoRetrogradeRs;
    b.diskOuterRs = std::max(9.0f, b.diskInnerRs * 4.2f);
    return b;
}

static const char* vertexShaderSource = R"GLSL(
#version 330 core
layout(location = 0) in vec2 aPos;
out vec2 vUv;
void main()
{
    vUv = aPos * 0.5 + 0.5;
    gl_Position = vec4(aPos, 0.0, 1.0);
}
)GLSL";

static const char* fragmentShaderSource = R"GLSL(
#version 330 core
in vec2 vUv;
out vec4 FragColor;

uniform vec2 uResolution;
uniform float uTime;
uniform float uSpin;
uniform float uExposure;
uniform float uDiskTemp;
uniform float uHorizonRs;
uniform float uPhotonRs;
uniform float uDiskInnerRs;
uniform float uDiskOuterRs;
uniform int uBodyCount;
uniform vec3 uBodyData[32];
uniform vec3 uBodyColor[32];

float hash21(vec2 p)
{
    p = fract(p * vec2(123.34, 456.21));
    p += dot(p, p + 45.32);
    return fract(p.x * p.y);
}

vec3 starField(vec2 p)
{
    vec2 g = floor(p * 210.0);
    vec2 f = fract(p * 210.0) - 0.5;
    float n = hash21(g);
    float star = smoothstep(0.995, 1.0, n) * exp(-60.0 * dot(f, f));
    vec3 tint = mix(vec3(0.65, 0.74, 1.0), vec3(1.0, 0.76, 0.54), hash21(g + 9.1));
    return tint * star * 2.2;
}

vec3 diskColor(float r, float phi, float side)
{
    float inner = smoothstep(uDiskInnerRs * 0.92, uDiskInnerRs * 1.08, r);
    float outer = 1.0 - smoothstep(uDiskOuterRs * 0.72, uDiskOuterRs, r);
    float band = inner * outer;
    float stripe = 0.68 + 0.32 * sin(phi * 18.0 + uTime * (2.2 + 7.0 / max(r, 0.2)));
    float turbulence = hash21(vec2(phi * 3.0, r * 24.0 + floor(uTime * 18.0)));
    float doppler = pow(clamp(1.0 + side * 0.48 / sqrt(max(r, 0.4)), 0.25, 1.9), 2.0);
    vec3 cool = vec3(0.93, 0.22, 0.95);
    vec3 hot = vec3(1.0, 0.86, 0.36);
    vec3 core = vec3(0.45, 0.82, 1.0);
    vec3 col = mix(cool, hot, smoothstep(7.5, 1.5, r) * uDiskTemp);
    col = mix(col, core, smoothstep(1.45, 0.95, r) * 0.45);
    return col * band * stripe * doppler * (0.78 + 0.22 * turbulence);
}

void main()
{
    vec2 frag = gl_FragCoord.xy;
    vec2 uv = (frag - 0.5 * uResolution) / min(uResolution.x, uResolution.y);
    float radius = length(uv);
    float angle = atan(uv.y, uv.x);

    float rsScreen = 0.115;
    float impact = max(radius / rsScreen, 0.055);
    float bend = 2.0 / impact;
    float swirl = uSpin * exp(-impact * 0.55) + bend * 0.06;
    vec2 lensed = vec2(cos(angle + swirl), sin(angle + swirl)) * (radius + bend * rsScreen * 0.17);
    vec2 skyUv = lensed * 0.62 + vec2(0.5 + 0.015 * uTime, 0.5);

    vec3 color = vec3(0.004, 0.005, 0.011);
    color += starField(skyUv);
    color += starField(skyUv * 1.9 + vec2(11.3, -7.2)) * 0.52;

    float diskY = uv.y + 0.018 * sin(angle * 3.0 + uTime);
    float diskX = uv.x * (1.0 + 0.3 * abs(uv.y));
    float diskR = length(vec2(diskX, diskY * 4.35)) / rsScreen;
    float diskPhi = atan(diskY * 4.35, diskX) + uSpin * uTime * 0.14;
    float nearMask = smoothstep(0.070, 0.0, abs(diskY)) * (1.0 - smoothstep(uDiskOuterRs * 0.95, uDiskOuterRs * 1.08, diskR));
    color += diskColor(diskR, diskPhi, -sign(uv.x)) * nearMask * 1.42;

    float photon = exp(-pow((impact - uPhotonRs) * 5.9, 2.0));
    color += vec3(1.0, 0.42, 0.95) * photon * 0.58;

    for (int i = 0; i < 32; ++i) {
        if (i >= uBodyCount) break;
        vec2 bp = uBodyData[i].xy * rsScreen;
        float br = max(uBodyData[i].z * rsScreen, 0.0025);
        float d = length(uv - bp);
        float body = smoothstep(br, br * 0.42, d);
        float wake = exp(-d * 35.0) * smoothstep(0.0, br * 10.0, d);
        vec3 bc = uBodyColor[i];
        color = mix(color, bc * (1.1 + uExposure), body);
        color += bc * wake * 0.12;
    }

    float horizon = smoothstep(uHorizonRs + 0.05, uHorizonRs - 0.08, impact);
    color *= 1.0 - horizon;
    color += vec3(0.035, 0.006, 0.045) * smoothstep(uHorizonRs, uHorizonRs + 0.35, impact) * smoothstep(uPhotonRs + 0.28, uPhotonRs - 0.1, impact);
    color = vec3(1.0) - exp(-color * uExposure);
    color = pow(color, vec3(0.4545));
    FragColor = vec4(color, 1.0);
}
)GLSL";

static std::string shaderLog(GLuint object, bool program)
{
    GLint len = 0;
    if (program)
        glGetProgramivPtr(object, GL_INFO_LOG_LENGTH, &len);
    else
        glGetShaderivPtr(object, GL_INFO_LOG_LENGTH, &len);
    std::string log(std::max(1, len), '\0');
    if (program)
        glGetProgramInfoLogPtr(object, len, nullptr, log.data());
    else
        glGetShaderInfoLogPtr(object, len, nullptr, log.data());
    return log;
}

static GLuint compileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShaderPtr(type);
    glShaderSourcePtr(shader, 1, &source, nullptr);
    glCompileShaderPtr(shader);
    GLint ok = 0;
    glGetShaderivPtr(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        std::string log = shaderLog(shader, false);
        std::fprintf(stderr, "Shader compile failed:\n%s\n", log.c_str());
        glDeleteShaderPtr(shader);
        return 0;
    }
    return shader;
}

static GLuint createProgram()
{
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    if (!vs || !fs)
        return 0;
    GLuint program = glCreateProgramPtr();
    glAttachShaderPtr(program, vs);
    glAttachShaderPtr(program, fs);
    glLinkProgramPtr(program);
    glDeleteShaderPtr(vs);
    glDeleteShaderPtr(fs);
    GLint ok = 0;
    glGetProgramivPtr(program, GL_LINK_STATUS, &ok);
    if (!ok) {
        std::string log = shaderLog(program, true);
        std::fprintf(stderr, "Program link failed:\n%s\n", log.c_str());
        glDeleteProgramPtr(program);
        return 0;
    }
    return program;
}

static void spawnBody(std::vector<Body>& bodies, const SpawnSettings& s, const KerrBounds& bounds)
{
    if (bodies.size() >= MaxBodies)
        bodies.erase(bodies.begin());
    float spawnRadius = std::max(s.radiusRs, bounds.diskInnerRs * 1.08f);
    spawnRadius = std::max(spawnRadius, bounds.horizonRs + 0.45f);
    float a = s.angleDeg * Pi / 180.0f;
    Vec2 radial{std::cos(a), std::sin(a)};
    Vec2 tangent{-radial.y, radial.x};
    Body b{};
    b.p = {radial.x * spawnRadius, radial.y * spawnRadius};
    b.v = {-radial.x * s.inwardC + tangent.x * s.tangentialC,
           -radial.y * s.inwardC + tangent.y * s.tangentialC};
    b.radius = s.sizeRs;
    b.mass = s.massEarth;
    b.heat = 0.0f;
    b.color = s.color;
    bodies.push_back(b);
}

static void updateBodies(std::vector<Body>& bodies, float dt, float spin, const KerrBounds& bounds)
{
    dt = std::min(dt, 0.033f);
    int steps = std::max(1, static_cast<int>(std::ceil(dt / 0.0045f)));
    float h = dt / static_cast<float>(steps);

    for (int step = 0; step < steps; ++step) {
        for (Body& b : bodies) {
            float r2 = b.p.x * b.p.x + b.p.y * b.p.y;
            float r = std::sqrt(std::max(r2, 0.0001f));
            float safe = std::max(r - bounds.horizonRs, 0.08f);
            // Paczynski-Wiita-style pseudo-potential keeps the Newtonian update cheap
            // while matching the strong-field plunge near the horizon.
            float accel = -0.50f / (safe * safe);
            Vec2 n{b.p.x / r, b.p.y / r};
            Vec2 frameDrag{-n.y, n.x};
            float drag = spin * 0.010f / std::max(r2, 0.3f);
            b.v.x += (n.x * accel + frameDrag.x * drag) * h;
            b.v.y += (n.y * accel + frameDrag.y * drag) * h;
            float v2 = b.v.x * b.v.x + b.v.y * b.v.y;
            if (v2 > 0.94f * 0.94f) {
                float scale = 0.94f / std::sqrt(v2);
                b.v.x *= scale;
                b.v.y *= scale;
            }
            b.p.x += b.v.x * h * 2.35f;
            b.p.y += b.v.y * h * 2.35f;
            b.heat = std::min(1.0f, b.heat + h * 0.6f / std::max(r - 0.9f, 0.2f));
        }
    }

    bodies.erase(std::remove_if(bodies.begin(), bodies.end(), [&bounds](const Body& b) {
        float r = std::sqrt(b.p.x * b.p.x + b.p.y * b.p.y);
        return r < bounds.horizonRs * 1.02f || r > 22.0f;
    }), bodies.end());
}

static double schwarzschildRadiusKm(double solarMasses)
{
    return (2.0 * G * SolarMass * solarMasses / (C * C)) / 1000.0;
}

static double lightCrossingMsPerRs(double solarMasses)
{
    return (2.0 * G * SolarMass * solarMasses / (C * C * C)) * 1000.0;
}

int main()
{
    if (!glfwInit()) {
        std::fprintf(stderr, "Failed to initialize GLFW.\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 0);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Relativistic Black Hole Simulation", nullptr, nullptr);
    if (!window) {
        std::fprintf(stderr, "Failed to create an OpenGL 3.3 window.\n");
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    if (!loadOpenGl()) {
        std::fprintf(stderr, "Your GPU/driver does not expose the required OpenGL functions.\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    GLuint program = createProgram();
    if (!program) {
        glfwDestroyWindow(window);
        glfwTerminate();
        return 1;
    }

    GLuint vao = 0;
    GLuint vbo = 0;
    const float fullscreenTri[] = {
        -1.0f, -1.0f,
         3.0f, -1.0f,
        -1.0f,  3.0f,
    };
    glGenVertexArraysPtr(1, &vao);
    glBindVertexArrayPtr(vao);
    glGenBuffersPtr(1, &vbo);
    glBindBufferPtr(GL_ARRAY_BUFFER, vbo);
    glBufferDataPtr(GL_ARRAY_BUFFER, sizeof(fullscreenTri), fullscreenTri, GL_STATIC_DRAW);
    glEnableVertexAttribArrayPtr(0);
    glVertexAttribPointerPtr(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    GLint uResolution = glGetUniformLocationPtr(program, "uResolution");
    GLint uTime = glGetUniformLocationPtr(program, "uTime");
    GLint uSpin = glGetUniformLocationPtr(program, "uSpin");
    GLint uExposure = glGetUniformLocationPtr(program, "uExposure");
    GLint uDiskTemp = glGetUniformLocationPtr(program, "uDiskTemp");
    GLint uHorizonRs = glGetUniformLocationPtr(program, "uHorizonRs");
    GLint uPhotonRs = glGetUniformLocationPtr(program, "uPhotonRs");
    GLint uDiskInnerRs = glGetUniformLocationPtr(program, "uDiskInnerRs");
    GLint uDiskOuterRs = glGetUniformLocationPtr(program, "uDiskOuterRs");
    GLint uBodyCount = glGetUniformLocationPtr(program, "uBodyCount");
    GLint uBodyData = glGetUniformLocationPtr(program, "uBodyData");
    GLint uBodyColor = glGetUniformLocationPtr(program, "uBodyColor");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 7.0f;
    style.FrameRounding = 5.0f;
    style.GrabRounding = 5.0f;
    style.Colors[ImGuiCol_Text] = ImVec4(0.92f, 0.94f, 0.98f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.035f, 0.035f, 0.055f, 0.86f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.72f, 0.16f, 0.62f, 0.55f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.78f, 0.15f, 0.62f, 0.72f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.95f, 0.22f, 0.78f, 0.86f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.00f, 0.32f, 0.82f, 1.0f);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    SpawnSettings spawn;
    std::vector<Body> bodies;
    int presetIndex = 0;
    float blackHoleMassSolar = BlackHolePresets[presetIndex].massSolar;
    float distance = BlackHolePresets[presetIndex].distance;
    const char* distanceUnit = BlackHolePresets[presetIndex].distanceUnit;
    float spin = BlackHolePresets[presetIndex].spin;
    float exposure = 1.35f;
    float diskTemp = 0.92f;
    bool autoFeed = false;
    bool vsync = true;
    double autoTimer = 0.0;
    auto start = std::chrono::steady_clock::now();
    double last = glfwGetTime();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        double now = glfwGetTime();
        float dt = static_cast<float>(now - last);
        last = now;
        spin = clampSpin(spin);
        KerrBounds bounds = makeKerrBounds(spin);
        float minFeedRs = std::max(bounds.diskInnerRs * 1.08f, bounds.horizonRs + 0.45f);
        spawn.radiusRs = std::max(spawn.radiusRs, minFeedRs);
        spawn.sizeRs = std::min(spawn.sizeRs, std::max(0.012f, (spawn.radiusRs - bounds.horizonRs) * 0.18f));
        if (autoFeed) {
            autoTimer += dt;
            if (autoTimer > 0.55) {
                autoTimer = 0.0;
                spawn.angleDeg = std::fmod(spawn.angleDeg + 41.0f, 360.0f);
                spawnBody(bodies, spawn, bounds);
            }
        }
        updateBodies(bodies, dt, spin, bounds);

        int fbw = 0;
        int fbh = 0;
        glfwGetFramebufferSize(window, &fbw, &fbh);
        glViewport(0, 0, fbw, fbh);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(16, 18), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(342, 0), ImGuiCond_Once);
        ImGui::Begin("Black Hole Feed Lab");
        if (ImGui::BeginCombo("Research preset", BlackHolePresets[presetIndex].name)) {
            for (int i = 0; i < static_cast<int>(sizeof(BlackHolePresets) / sizeof(BlackHolePresets[0])); ++i) {
                bool selected = (presetIndex == i);
                if (ImGui::Selectable(BlackHolePresets[i].name, selected)) {
                    presetIndex = i;
                    blackHoleMassSolar = BlackHolePresets[i].massSolar;
                    distance = BlackHolePresets[i].distance;
                    distanceUnit = BlackHolePresets[i].distanceUnit;
                    spin = BlackHolePresets[i].spin;
                    bodies.clear();
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
        ImGui::TextWrapped("%s", BlackHolePresets[presetIndex].note);
        if (distance > 0.0f)
            ImGui::Text("Distance: %.2f %s", distance, distanceUnit);
        ImGui::Text("Schwarzschild radius: %.0f km", schwarzschildRadiusKm(blackHoleMassSolar));
        ImGui::Text("Light crossing: %.2f ms per Rs", lightCrossingMsPerRs(blackHoleMassSolar));
        ImGui::Text("Horizon: %.3f Rs   Ergosphere(eq): %.2f Rs", bounds.horizonRs, bounds.ergosphereRs);
        ImGui::Text("Photon orbit: %.3f / %.3f Rs", bounds.photonProgradeRs, bounds.photonRetrogradeRs);
        ImGui::Text("ISCO: %.3f / %.3f Rs", bounds.iscoProgradeRs, bounds.iscoRetrogradeRs);
        ImGui::Separator();
        ImGui::SliderFloat("Mass", &blackHoleMassSolar, 3.0f, 1.0e10f, "%.0f solar masses", ImGuiSliderFlags_Logarithmic);
        ImGui::SliderFloat("Spin", &spin, -ThorneSpinLimit, ThorneSpinLimit, "%.3f a*");
        ImGui::SliderFloat("Exposure", &exposure, 0.4f, 2.6f, "%.2f");
        ImGui::SliderFloat("Disk heat", &diskTemp, 0.2f, 1.5f, "%.2f");
        if (ImGui::Checkbox("VSync", &vsync))
            glfwSwapInterval(vsync ? 1 : 0);
        ImGui::Separator();
        ImGui::SliderFloat("Spawn orbit", &spawn.radiusRs, minFeedRs, 14.0f, "%.1f Rs");
        ImGui::SliderFloat("Object radius", &spawn.sizeRs, 0.025f, 0.22f, "%.3f Rs");
        ImGui::SliderFloat("Object mass", &spawn.massEarth, 0.02f, 500.0f, "%.2f Earth masses", ImGuiSliderFlags_Logarithmic);
        ImGui::SliderFloat("Inward speed", &spawn.inwardC, 0.0f, 0.86f, "%.2f c");
        ImGui::SliderFloat("Tangential speed", &spawn.tangentialC, -0.86f, 0.86f, "%.2f c");
        ImGui::SliderFloat("Angle", &spawn.angleDeg, 0.0f, 360.0f, "%.0f deg");
        ImGui::ColorEdit3("Object color", reinterpret_cast<float*>(&spawn.color));
        if (ImGui::Button("Feed Object", ImVec2(132, 0)))
            spawnBody(bodies, spawn, bounds);
        ImGui::SameLine();
        if (ImGui::Button("Clear", ImVec2(84, 0)))
            bodies.clear();
        ImGui::SameLine();
        ImGui::Checkbox("Auto", &autoFeed);
        ImGui::Text("Active objects: %d / %d", static_cast<int>(bodies.size()), MaxBodies);
        ImGui::End();

        std::array<float, MaxBodies * 3> bodyData{};
        std::array<float, MaxBodies * 3> bodyColors{};
        for (size_t i = 0; i < bodies.size() && i < MaxBodies; ++i) {
            const Body& b = bodies[i];
            bodyData[i * 3 + 0] = b.p.x;
            bodyData[i * 3 + 1] = b.p.y;
            bodyData[i * 3 + 2] = b.radius;
            float heat = std::min(1.0f, b.heat);
            bodyColors[i * 3 + 0] = std::min(1.0f, b.color.x + heat * 0.45f);
            bodyColors[i * 3 + 1] = std::min(1.0f, b.color.y + heat * 0.28f);
            bodyColors[i * 3 + 2] = std::min(1.0f, b.color.z + heat * 0.15f);
        }

        auto elapsed = std::chrono::duration<float>(std::chrono::steady_clock::now() - start).count();
        glDisable(GL_DEPTH_TEST);
        glUseProgramPtr(program);
        glUniform2fPtr(uResolution, static_cast<float>(fbw), static_cast<float>(fbh));
        glUniform1fPtr(uTime, elapsed);
        glUniform1fPtr(uSpin, spin);
        glUniform1fPtr(uExposure, exposure);
        glUniform1fPtr(uDiskTemp, diskTemp);
        glUniform1fPtr(uHorizonRs, bounds.horizonRs);
        glUniform1fPtr(uPhotonRs, (spin >= 0.0f) ? bounds.photonProgradeRs : bounds.photonRetrogradeRs);
        glUniform1fPtr(uDiskInnerRs, bounds.diskInnerRs);
        glUniform1fPtr(uDiskOuterRs, bounds.diskOuterRs);
        glUniform1iPtr(uBodyCount, static_cast<int>(bodies.size()));
        glUniform3fvPtr(uBodyData, MaxBodies, bodyData.data());
        glUniform3fvPtr(uBodyColor, MaxBodies, bodyColors.data());
        glBindVertexArrayPtr(vao);
        glDrawArraysPtr(GL_TRIANGLES, 0, 3);

        ImDrawList* draw = ImGui::GetForegroundDrawList();
        char fpsText[64];
        std::snprintf(fpsText, sizeof(fpsText), "%.1f FPS", io.Framerate);
        ImVec2 textSize = ImGui::CalcTextSize(fpsText);
        draw->AddText(ImVec2(io.DisplaySize.x - textSize.x - 18.0f, 14.0f), IM_COL32(255, 50, 210, 255), fpsText);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glDeleteBuffersPtr(1, &vbo);
    glDeleteVertexArraysPtr(1, &vao);
    glDeleteProgramPtr(program);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
