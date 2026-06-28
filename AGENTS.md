

The System::Component class provides transform() and gameObject() getter methods for accessing associated properties, 
though some codebases may erroneously attempt direct member access.


The System::Vector3 class explicitly deletes its (int, int, int) constructor. 
All vector initializations must use floating-point literals (e.g., System::Vector3(0.0f, 0.0f, 0.0f)).

When running graphical tests with xvfb-run on systems using Wayland/EGL, 
exporting XDG_RUNTIME_DIR=/tmp can resolve environment initialization errors.

Direct modification of the vertices or normals vectors in the System::Mesh class (Headers/System/Mesh.hpp) will not be uploaded by UploadMeshData unless the mesh's internal Modified flag is set (e.g., by calling SetVertices).

The Mesh::UploadMeshData(bool markNoLongerReadable) method supports iterative updates when passing false, which preserves CPU-side vertices and normals data; passing true (the default) clears the CPU buffers after the GPU upload.

The MeshRenderer automatically injects uniform values for standard light types; however, it currently only supports automatic uniform injection for the Point light type.


The Shader class (Sources/System/Shader.cpp) explicitly binds standard attribute locations (aPos:0, aNormal:1, aUV:2) during program creation to ensure compatibility between the Mesh data structure and shaders.


The Headers/System/Graphics/GraphicsHelpers.h file contains static wrapper methods for OpenGL functions (including gl_glBindAttribLocation) and enums for TextureUnit, TextureParam, etc. It abstracts resource management while maintaining compatibility with standard GLenum types.


Do not refactor test-specific components (like the Terrain component in Tests/Terrain) into core engine directories (Sources/System/Components or Headers/System/Components) unless explicitly instructed by the user.

To visualize graphical test outputs like BMP files in headless environments, imagemagick (specifically the convert tool) can be used to transform them into web-friendly formats like PNG.

Including <GLES3/gl3.h> on Linux can cause macro redefinition conflicts (e.g., GL_SHADER) with internal GraphicsHelpers.h enums. To resolve this, standard GL headers should be excluded in Headers/System/Windows/Application.h when targeting the engine's internal InternalGLloader.h wrapper.

The System::Mesh class (Headers/System/Mesh.hpp) provides GetVAO(), GetVBO(), and GetEBO() methods to retrieve the underlying OpenGL object handles for Vertex Array, Vertex Buffer, and Element Buffer Objects respectively.

Visual verification and framebuffer capture via glReadPixels or Camera::TakeScreenshot is functional in headless Linux environments when using xvfb-run in combination with the engine's native Wayland/EGL-based Application class.

The System::Transform class (Headers/System/Transform.hpp) provides forward() and right() methods to retrieve direction vectors relative to current orientation in world space.

For ARM-based systems or GLES2 environments, tests must use and provide shaders with _es suffixes (e.g., shaderbasicflat_es.vert) to ensure compatibility.

The C++ math library is sensitive to float literal syntax. Using explicit literals like 1.0f and 16.0f / 9.0f is more robust than integer division or implicit conversions.




























