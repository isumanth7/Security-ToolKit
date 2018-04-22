#include "security.h"

Security::Security(QImage &startingImage, int bitsPerPixel) : image(startingImage)  , maxHeadLen(std::numeric_limits<int>::max())
{
    maxMsgLen = setNewBitsUsedPerPixel(bitsPerPixel);
    imgArea   = image.width() * image.height();
}

int Security::getMaxMsgLen() {
    return maxMsgLen;
}

int Security::setNewBitsUsedPerPixel(int bits) {
    this->bitsPerPixel = bits;
    if ( ( maxMsgLen = ( (image.height() * image.width() * bits) /  Security::bitsPerChar ) -    2  ) > maxHeadLen )
    {
        maxMsgLen = maxHeadLen;
        return maxHeadLen;
    }
    else {
        return maxMsgLen;
         }
}

void Security::setMessage(QString msg) { message = msg; }

int  Security::getMessageLength() { return message.length(); }

bool Security::isMessageTooLong() { return message.length() > maxMsgLen; }


QImage* Security::encodeImage() {
    QImage* encodedImage = new QImage(image.copy(0, 0, image.width(), image.height()));
    int x = 0, y = 0, header = message.length();

    if (message == NULL || bitsPerPixel == 0) {
        return encodedImage;
    }

    for (int mask = FIRST_BIT_MASK; mask != 0x0000; mask <<= 1) {
        QRgb bit   = (bool) (header & mask);
        QRgb pixel = encodedImage->pixel(x, y) & ~FIRST_BIT_MASK;
        encodedImage->setPixel(x, y, pixel | bit);

        if (++x == image.width()) {
            x = 0;
            if (++y == image.height()) {
                break;
            }
        }
    }

    for (QChar* character = message.begin(); character != message.end(); ++character) {

        for (ushort mask = FIRST_BIT_MASK; mask != 0x0000; mask <<= 1) {
            QRgb bit   = (bool)  (character->unicode() &  mask);
            QRgb pixel = encodedImage->pixel(x, y) & ~FIRST_BIT_MASK;

            encodedImage->setPixel(x, y, pixel | bit);

            if (++x == image.width()) {
                x = 0;
                if (++y == image.height()) {
                    return encodedImage;
                }
            }
        }
    }
    return encodedImage;
}

QString Security::decodeImage() {
    ushort character = 0x0000;
    int msgLen = 0;

    for (int x = Security::last_header_index % image.width(), y = Security::last_header_index / image.width(),
         count = Security::last_header_index; count > -1; --count) {

        msgLen <<= 1;
        msgLen = msgLen | (((ushort) image.pixel(x, y)) & FIRST_BIT_MASK);

        if (--x == -1) {
            --y;
            x = image.width() - 1;
        }
    }
    if (msgLen <= 0) {
        return QString("");
    }
    int pixel_count = (msgLen + 2) * Security::bitsPerChar - 1;

    if (pixel_count > imgArea) {
        pixel_count = imgArea - (imgArea % Security::bitsPerChar) - 1;
        msgLen      = maxMsgLen;
    }
    QString decoded(msgLen);

    for (int x = pixel_count % image.width(), y = pixel_count / image.width();
         pixel_count > Security::last_header_index; --pixel_count) {

        character <<= 1;
        character = character | (((ushort) image.pixel(x, y)) & FIRST_BIT_MASK);

        if (pixel_count % Security::bitsPerChar == 0) {
            decoded[--msgLen] = character;
            character = 0x0000;
        }
        if (--x == -1) {
            --y;
            x = image.width() - 1;
        }
    }

    return decoded;
}

QImage* Security::noShameImage() {
    QImage* encodedImage = new QImage(image.copy(0, 0, image.width(), image.height()));
    for (int x = 0, y = 0, count = 0,limit = message.length() * Security::bitsPerChar + 2; count < limit; ++count) {
        encodedImage->setPixel(x, y, ~encodedImage->pixel(x, y));
        if (++x == image.width()) {
            x = 0;
            if (++y == image.height()) {
                return encodedImage;
            }
        }
    }
    return encodedImage;
}
