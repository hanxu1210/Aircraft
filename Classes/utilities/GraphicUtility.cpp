#include "GraphicUtility.h"
#include "cocos2d.h"

USING_NS_CC;

namespace NsAircraft
{
    void GraphiccUtility::saveScreenshot(const std::string& fileName)
    {
        auto glView = Director::getInstance()->getOpenGLView();
        auto frameSize = glView->getFrameSize();

        int width = static_cast<int>(frameSize.width);
        int height = static_cast<int>(frameSize.height);

        bool succeed = false;
        std::string outputFile = "";

        do
        {
            std::shared_ptr<GLubyte> buffer(new GLubyte[width * height * 4], [](GLubyte* p){ CC_SAFE_DELETE_ARRAY(p); });
            if (!buffer)
            {
                break;
            }

            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            glReadPixels(0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, buffer.get());

            std::shared_ptr<GLubyte> flippedBuffer(new GLubyte[width * height * 4], [](GLubyte* p) { CC_SAFE_DELETE_ARRAY(p); });
            if (!flippedBuffer)
            {
                break;
            }


            for (int row = 0; row < height; ++row)
            {
                memcpy(flippedBuffer.get() + (height - row - 1) * width * 4, buffer.get() + row * width * 4, width * 4);
            }

            std::shared_ptr<Image> image(new Image);
            if (image)
            {
                image->initWithRawData(flippedBuffer.get(), width * height * 4, width, height, 8);
                if (FileUtils::getInstance()->isAbsolutePath(fileName))
                {
                    outputFile = fileName;
                }
                else
                {
                    CCASSERT(fileName.find("/") == std::string::npos, "The existence of a relative path is not guaranteed!");
                    outputFile = FileUtils::getInstance()->getWritablePath() + fileName;
                }
                succeed = image->saveToFile(outputFile);
            }
        }while(0);
        //Director::getInstance()->getRenderer()->render();
    }
}