#include "Texture.h"

#include <iostream>
#include <cstring>

#define cimg_display 0

#include <CImg.h>

// Bonus Task
#include <vector>
#include <numeric>
#include <algorithm>
#include <glm/vec3.hpp>
// END Bonus

using namespace cimg_library;


namespace cgCourse {
    void Texture::loadFromFile(const std::string &_filename) {
        CImg<unsigned char> img(_filename.c_str());

        int channels = img.spectrum();
        size.x = img.width();
        size.y = img.height();

        img.permute_axes("cxyz");

        /*
		 *  TODO: generate the texture here:
		 *        - generate the texture handle and set the variable texhandle
		 *        - bind the texture handle
		 *        - set the min/mag filter parameter and if it works test if you
		 *          see a difference between GL_LINEAR, MIPMAP_LINEAR, NEAREST etc..
		 *        - set the texture wrap parameters
		 *        - set the texture data with glTexImage2D
		 *        - generate mipmaps
		 *        - unbind the texture handle (we do this to have a clean state when the
		 *          function is ending and no textures are bound after this call.)
		 *   Note that depending on the channels the a different kind of texture might be created, for 4 channels GL_RGBA, for 3 GL_RGB and for one GL_RED.
         *   Some operating systems might use GL_BGRA
		 */

        // TODO END

        glGenTextures(1, &texhandle);
        glBindTexture(GL_TEXTURE_2D, texhandle);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        GLenum format;
        if (channels == 4) {
            format = GL_RGBA;
        } else if (channels == 3) {
            format = GL_RGB;
        } else if (channels == 1) {
            // format = GL_RED; // highlights using red
            format = GL_LUMINANCE; // highlights using white
        } else {
            std::cerr << "Unsupported number of channels: " << channels << std::endl;
            return;
        }

        glTexImage2D(GL_TEXTURE_2D, 0, format, size.x, size.y, 0, format, GL_UNSIGNED_BYTE, img.data());
        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // Bonus Task
    void Texture::createTexture() {
        // Create a texture with a size of 256x256
        size.x = 256;
        size.y = 256;

        std::vector<glm::vec3> pixels(size.x * size.y);

        // color filing
        for (int y = 0; y < size.y; ++y) {
            for (int x = 0; x < size.x; ++x) {
                float r = float(x) / size.x;
                float g = float(y) / size.y;
                float b = 0.5f;
                pixels[y * size.x + x] = glm::vec3(r, g, b);
            }
        }

        // texture creation
        glGenTextures(1, &texhandle);
        glBindTexture(GL_TEXTURE_2D, texhandle);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size.x, size.y, 0, GL_RGB, GL_FLOAT, pixels.data());

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

    }

    const GLuint &Texture::getTexHandle() const {
        return texhandle;
    }
}