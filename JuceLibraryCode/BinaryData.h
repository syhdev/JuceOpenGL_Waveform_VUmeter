/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#pragma once

namespace BinaryData
{
    extern const char*   VUfragmentShader_glsl;
    const int            VUfragmentShader_glslSize = 1714;

    extern const char*   VUvertexShader_glsl;
    const int            VUvertexShader_glslSize = 359;

    extern const char*   WVfragmentShader_glsl;
    const int            WVfragmentShader_glslSize = 1714;

    extern const char*   WVvertexShader_glsl;
    const int            WVvertexShader_glslSize = 359;

    // Number of elements in the namedResourceList and originalFileNames arrays.
    const int namedResourceListSize = 4;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Points to the start of a list of resource filenames.
    extern const char* originalFilenames[];

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes);

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding original, non-mangled filename (or a null pointer if the name isn't found).
    const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8);
}
