/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#ifndef BACKGROUNDANIMATION_H
#define BACKGROUNDANIMATION_H
#include <QImage>
#include <QColor>
#include <QVector>

class BackgroundAnimation {
public:
    /**
     * QImage = input image
     * QVector<QColor> = Palette to cycle through
     * default palette
     * 0 - 0,0,0
     * 1 - 0,0,255
     * 2 - 0,255,0
     * 3 - 0,255,255
     * ...
     * 7 - 255,255,255
     */
    BackgroundAnimation(QImage, QVector<QColor>, int);

    /**
     * A copy constructor for BackgroundAnimation
     */
    BackgroundAnimation(const BackgroundAnimation&);

    /**
     * Overrides '=' for BackgroundAnimation
     */
    BackgroundAnimation& operator=(BackgroundAnimation);

    /**
     * returns the next frame of the animation
     */
    QImage getNextFrame();

private:

    /**
     * Source background image for manipulation
     */
    QVector<QVector<int>> sourceMatrix;

    /**
     * pallette to cycle through for the image
     */
    QVector<QColor> palette;

    /**
     * translates the source up left
     */
    void translateUpLeft();

    /**
     * a -1 or 1 used for calculation
     */
    int sign;

    /**
     * width of the image and sourceMatrix
     */
    int width;

    /**
     * height of the image and sourceMatrix
     */
    int height;

    /**
     * the angle which counts upward during horizontal offset calculation with sin()
     */
    double countingAngle;

    /**
     * helper function, shifts the palette of the image by 1
     */
    void shiftPalette();

    /**
     * does the image need to translate up left
     */
    bool isTranslating;

    /**
     * does the image need to move in a sin wave
     */
    bool isSinWave;

    /**
     * is every other line inverted
     */
    bool isOffsetInverted;

    /**
     * does the palette shift
     */
    bool isPaletteShifting;

};

#endif // BACKGROUNDANIMATION_H
