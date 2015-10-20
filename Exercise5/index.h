#ifndef INDEX_H
#define INDEX_H

#define GRAY(IMAGE, X, Y) \
    ((uchar*)((IMAGE)->imageData  + (IMAGE)->widthStep*(Y)))[X]
#define RED(IMAGE, X, Y) \
  ((uchar*)((IMAGE)->imageData + (IMAGE)->widthStep*(Y)))[(X)*3+2]
#define GREEN(IMAGE, X, Y) \
  ((uchar*)((IMAGE)->imageData + (IMAGE)->widthStep*(Y)))[(X)*3+1]
#define BLUE(IMAGE, X, Y) \
  ((uchar*)((IMAGE)->imageData + (IMAGE)->widthStep*(Y)))[(X)*3]

#define GRAY_T(TYPE, IMAGE, X, Y) \
    ((TYPE*)((IMAGE)->imageData  + (IMAGE)->widthStep*(Y)))[X]
#define RED_T(TYPE, IMAGE, X, Y) \
  ((TYPE*)((IMAGE)->imageData + (IMAGE)->widthStep*(Y)))[(X)*3+2]
#define GREEN_T(TYPE, IMAGE, X, Y) \
  ((TYPE*)((IMAGE)->imageData + (IMAGE)->widthStep*(Y)))[(X)*3+1]
#define BLUE_T(TYPE, IMAGE, X, Y) \
  ((TYPE*)((IMAGE)->imageData + (IMAGE)->widthStep*(Y)))[(X)*3]

/* VIRKER IKKE
foreach_gray_pixel(IMAGE, VAL) \
    for (int Y=0; Y < (IMAGE)->height; Y++) \
        for (int X=0; X < (IMAGE)->width; X++) \
            (VAL) = ((uchar*)((IMAGE)->imageData  + (IMAGE)->widthStep*(Y)))[X]
*/
#endif
