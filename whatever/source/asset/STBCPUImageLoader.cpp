#include "STBCPUImageLoader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

namespace wtv
{
    CPUImageData wtv::STBCPUImageLoader::Load(const std::filesystem::path& path)
    {
        int width, height, channels;
        stbi_uc* pixels = stbi_load(path.string().c_str(), &width, &height, &channels, 0);

        if (pixels == nullptr)
            return CPUImageData{};

        CPUImageData img;
		img.width = width;
		img.height = height;
        switch (channels)
        {
        case 1:
			img.format = ImageFormat::R8_UNORM;
			break;
        case 2:
			img.format = ImageFormat::R8G8_UNORM;
            break;
        case 3:
			img.format = ImageFormat::R8G8B8_UNORM;
			break;
        case 4:
			img.format = ImageFormat::R8G8B8A8_UNORM;
            break;
        }
		img.data = std::vector<char>(pixels, pixels + width * height * channels);
        
        stbi_image_free(pixels);


        return img;
    }
}
