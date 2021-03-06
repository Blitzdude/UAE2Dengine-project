#include "OGL/ImageLoader.h"
#include "core/IOManager.h"
#include <GLES2/gl2.h>


#if defined(ANDROID)

#include <android/asset_manager.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include <malloc.h>
#endif

#include <core/Log.h>
#define STB_IMAGE_IMPLEMENTATION
#include <core/stb_image.h>

namespace engine
{
    Texture2D
    ImageLoader::loadPNG(std::string filepath, int &width, int &height, int &bytesPerPixel, void *manager)
    {
        //#if defined (_WIN32) TODO: delete
        // create Texture2D and initialize its fields to zero
        Texture2D texture = {};

        std::vector<unsigned char> in;

        if (!IOManager::readFileToBuffer(filepath, in, manager)) {
            LOGE("Failed to load PNG file to Buffer! %s", filepath.c_str());
            return texture;
        }

        //unsigned char* out = stbi_load(filepath.c_str(), &width, &height, &bytesPerPixel, 4); TODO: delete
        unsigned char *out = stbi_load_from_memory(in.data(), in.size(), &width, &height, &bytesPerPixel, 4);

        // Generate the openGL texture object
        glGenTextures(1, &(texture.id));

        // bind the texture object
        glBindTexture(GL_TEXTURE_2D, texture.id);


        // upload the pixels to the texture
        GLuint fmt = bytesPerPixel == 3 ? GL_RGB : GL_RGBA;
        LOGI("\n before texture binding-> width: %d, height: %d, bbp: %d \n", width, height,
             (int) bytesPerPixel);
        glTexImage2D(GL_TEXTURE_2D, 0, fmt, width, height, 0, fmt, GL_UNSIGNED_BYTE, out);

        // set some texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // generate mipmap
        glGenerateMipmap(GL_TEXTURE_2D);

        // unbind the texture object
        glBindTexture(GL_TEXTURE_2D, 0);

        stbi_image_free(out); // free image
        // return the texture data

        texture.width = width;
        texture.height = height;
        texture.filepath = filepath;

        return texture;
    }
}
