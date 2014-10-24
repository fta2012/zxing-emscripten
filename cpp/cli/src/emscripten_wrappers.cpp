#include <string>
#include <exception>

#include "ImageReaderSource.h"

#include <zxing/common/Counted.h>
#include <zxing/Result.h>
#include <zxing/common/Array.h>
#include <zxing/LuminanceSource.h>
#include <zxing/common/HybridBinarizer.h>
#include <zxing/Binarizer.h>
#include <zxing/BinaryBitmap.h>
#include <zxing/DecodeHints.h>
#include <zxing/qrcode/QRCodeReader.h>

#include <emscripten/bind.h>

using namespace std;
using namespace zxing;
using namespace zxing::qrcode;
using namespace emscripten;

string qrcode_decode(const string & pixels, int width, int height, emscripten::val debugCallback) {
    Ref<Result> result;
    try {
        ArrayRef<char> image(4 * width * height);
        for (int i = 0; i < 4 * width * height; i++) {
            image[i] = pixels[i];
        }

        Ref<LuminanceSource> source = Ref<LuminanceSource>(new ImageReaderSource(image, width, height, 4));
        Ref<Binarizer> binarizer(new HybridBinarizer(source));
        Ref<BinaryBitmap> binary(new BinaryBitmap(binarizer));

        vector<char> grayImage(4 * width * height, 255);
        Ref<BitMatrix> bitMatrix = binarizer->getBlackMatrix();
        for (int y = 0; y < bitMatrix->getHeight(); y++) {
            for (int x = 0; x < bitMatrix->getWidth(); x++) {
                if (bitMatrix->get(x, y)) {
                        grayImage[(y * bitMatrix->getWidth() + x) * 4] = 0;
                        grayImage[(y * bitMatrix->getWidth() + x) * 4 + 1] = 0;
                        grayImage[(y * bitMatrix->getWidth() + x) * 4 + 2] = 0;
                }
            }
        }
        debugCallback(emscripten::memory_view<char>(4 * width * height, &grayImage[0]));

        Ref<Reader> reader(new QRCodeReader());
        result = reader->decode(binary, DecodeHints::DEFAULT_HINT);
    } catch (const std::exception & e) {
        cerr << e.what() << endl;
    }
    return result->getText()->getText();
}

EMSCRIPTEN_BINDINGS(qrcode) {
    emscripten::function("qrcode_decode", &qrcode_decode);
}
