/** Created by The Task Managers for CS3505 Fall 2019 at the University of Utah.
 *  Molly Clare
 *  Taylor Jacobsen
 *  Ryan Brooks
 *  André Watson
 *  Calvin Nielson
 *  Alex Bartee
 */

#include "backgroundanimation.h"
#include <math.h>

BackgroundAnimation::BackgroundAnimation(QImage img, QVector<QColor> _palette, int _mode)
{
    //converts the image into a background
    sign=1;
    palette=_palette;



    //set flags internally based on 4 digit binary
    // positions 3 2 1 0
    // 0 - shift pal
    // 1 - translation
    // 2 - sin
    // 3 - offset inverted

    //normalize input
    int mode = _mode;
    mode = std::max(mode,0);
    mode = std::min(mode,15);

    //rightmost bit
    if(mode%2==1){
        isPaletteShifting=true;
        mode-=1;
    }
    else{
        isPaletteShifting=false;
    }
    mode/=2;
    if(mode%2==1){
        isTranslating=true;
        mode-=1;
    }
    else{
        isTranslating=false;
    }
    mode/=2;
    if(mode%2==1){
        isSinWave=true;
        mode-=1;
    }
    else{
        isSinWave=false;
    }
    mode/=2;
    //leftmost bit
    if(mode%2==1){
        isOffsetInverted=true;
        mode-=1;
    }
    else{
        isOffsetInverted=false;
    }


    width = img.width();
    height = img.height();
    for(int x=0; x<width; x++){
        QVector<int> vect;
        sourceMatrix.append(vect);
        for(int y=0; y<height; y++){
            QColor color = img.pixel(x,y);
            int paletteIndex = 0;
            if(color.red()>0){
                paletteIndex+=4;
            }
            if(color.green()>0){
                paletteIndex+=2;
            }
            if(color.blue()>0){
                paletteIndex+=1;
            }
            sourceMatrix[x].append(paletteIndex);
        }
    }
}

BackgroundAnimation::BackgroundAnimation(const BackgroundAnimation& other){
    sourceMatrix=other.sourceMatrix;
    height=other.height;
    width=other.width;
    palette=other.palette;
    sign=other.sign;
    countingAngle=other.countingAngle;
    isPaletteShifting=other.isPaletteShifting;
    isOffsetInverted=other.isOffsetInverted;
    isSinWave=other.isSinWave;
    isTranslating=other.isTranslating;
}

BackgroundAnimation& BackgroundAnimation::operator=(BackgroundAnimation other){
    std::swap(sourceMatrix, other.sourceMatrix);
    std::swap(height, other.height);
    std::swap(width, other.width);
    std::swap(palette, other.palette);
    std::swap(sign, other.sign);
    std::swap(countingAngle, other.countingAngle);
    std::swap(isPaletteShifting, other.isPaletteShifting);
    std::swap(isOffsetInverted, other.isOffsetInverted);
    std::swap(isSinWave, other.isSinWave);
    std::swap(isTranslating, other.isTranslating);
    return*this;
}

QImage BackgroundAnimation::getNextFrame(){
    //palette shift
    if(isPaletteShifting){
        shiftPalette();
    }
    //translation
    if(isTranslating){
        translateUpLeft();
    }
    double angle = countingAngle;
    int offset =  0;
    QImage retImage(width, height, QImage::Format_RGB32);
    for(int y = 0; y<height; y++){
        //sin wav offset
        if(isSinWave){
            offset = int(std::sin(angle)*10);
        }
        for(int x=0; x<width; x++){
            //calculate shift, multiply by - and + interchangably to create virtual scanlines
            int offsetX = (width+x+(offset*sign))%width;
            //draw a pixel
            //if the index is in the palette
            int paletteIndex = sourceMatrix[offsetX][y];
            if(palette.size()>paletteIndex){
                retImage.setPixelColor(x,y,palette[paletteIndex]);
            }
            else{
                //default color
                retImage.setPixelColor(x,y,QColor(0,0,0));
            }
        }
        //update changing calculation values
        //line offset inversion
        if(isOffsetInverted){
            sign*=-1;
        }
        angle+=.1*width/200;
    }
    countingAngle+=.4;
    if(countingAngle>6.2){
        countingAngle=0;
    }
    return retImage;
}

void BackgroundAnimation::shiftPalette(){
    QVector<QColor> newPalette;
    for(int i=1; i<int(palette.size()); i++){
        newPalette.append(palette[i]);
    }
    newPalette.append(palette[0]);
    palette=newPalette;
}

void BackgroundAnimation::translateUpLeft(){
    //shift up
    for(int x=1; x<width; x++){
        sourceMatrix[x].append(sourceMatrix[x][0]);
        sourceMatrix[x].removeFirst();
    }
    //shift corner
    sourceMatrix[0].append(sourceMatrix[0][0]);
    sourceMatrix[0].removeFirst();

    //shift left
    sourceMatrix.append(sourceMatrix[0]);
    sourceMatrix.removeFirst();
}
