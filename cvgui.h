#ifndef CVGUI_H
#define CVGUI_H

//#include <opencv4/opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>
#include "fonty.h"

#define ProgVersion "CvGui Test V1.0.0"

#define PrnBufLen 255

#define DispW 1018
#define DispH 540

#define ScrTop 40 // Oblast obrazovky pod ribbonem
#define ScrMiddle (DispW/3)
#define ScrRight (2*(DispW/3)-20)
#define DispRight (DispW-88)

#define VoltageFont f_Tahoma_16_Bold

#define RibbonFont f_MS_Sans_8_Bold
#define RibbonYSpace 12
#define RibbonVal 30
#define RibbonR1 100
#define RibbonR1Val (RibbonR1+30)
#define RibbonR2 215
#define RibbonR2Val (RibbonR2+30)

#define RibbonMidVal (ScrMiddle+65)
#define RibbonRightVal (ScrRight+40)

// System colors
#define clBlack 0
#define clMaroon 1
#define clGreen 2
#define clOlive 3
#define clNavy 4
#define clPurple 5
#define clTeal 6
#define clSilver 7
#define clGrey 8
#define clRed 9
#define clLime 10
#define clYellow 11
#define clBlue 12
#define clMagenta 13 // clFuchsia
#define clAqua 14
#define clWhite 15

#define clButton 253
#define clOrange 214

#define bmpNormal 0 ///< Normalni zobrazeni bitmapy
#define bmpNegative 1 ///< Negativni zobrazeni bitmapy

#define bmpSolid 0 ///< Vypnln nulove pixely bitmapy bilou
#define bmpTransparent 1 ///< Ponech na miste nulovych pixelu bitmapy puvodni obsah

#define CaptionLen 32
typedef struct {
    unsigned short X;
    unsigned short Y;
    unsigned short W;
    unsigned short H;
    unsigned char ButtonBkgColor;
    unsigned char ButtonFrameColor;
    unsigned char ButtonCaptionColor;
    unsigned char ButtonCaptionFont;
    char Caption[CaptionLen];
    unsigned char Pressed;
} TButton;

typedef struct {
    unsigned short X;
    unsigned short Y;
    unsigned short W;
    unsigned short H;
    unsigned char SliderBkgColor;
    unsigned char SliderFrameColor;
    int MaxVal;
    int Value;
    unsigned char Activated;
    unsigned char Changed;
} TSlider;

typedef struct {
    unsigned short X;
    unsigned short Y;
    unsigned short W;
    unsigned short H;
    unsigned char BkgColor;
    unsigned char FrameColor;
    unsigned char BarColor;
    int MaxVal;
    int Value;    
} TIntBarGraph;

typedef struct {
    unsigned short X;
    unsigned short Y;
    unsigned short W;
    unsigned short H;
    unsigned char BkgColor;
    unsigned char FrameColor;
    unsigned char BarColor;
    unsigned char ValColor;
    float MaxVal;
    float Value;    
} TFloatBarGraph;

class cvgui {
    const char* gui_window = ProgVersion; // "FVE control panel";
    unsigned short VpX1;
    unsigned short VpY1;
    unsigned short VpX2;
    unsigned short VpY2;
    
    void Dump();
    long LAbs(long L);
    //void GUI_Callback(int event, int x, int y, int flags, void* userdata);
    
public:
	bool QuitProgram = false;
    cv::Mat screen;
    bool NewKey;
    char Key;
    char PrnBuf[PrnBufLen];
    cvgui();
    void Update();
    void Draw();
    void Show();
    void Cls(unsigned char Color);
    void ClsJet();
    bool inPort(unsigned short X, unsigned short Y);
    void SetViewport(unsigned short X1, unsigned short Y1, unsigned short X2, unsigned short Y2);
    void ClearViewport(unsigned char Color);
    void FullViewport();
    void DrawButton(TButton* Button);
    void DrawButtonLeft(TButton* Button);
    void HandleButton(TButton* Button);    
    void DrawSlider(TSlider* Slider);
    void HandleSlider(TSlider* Slider);
    void DrawIntBarGraph(TIntBarGraph* BarGraph);
    void DrawFloatBarGraph(TFloatBarGraph* BarGraph);
    void DrawFloatValBarGraph(TFloatBarGraph* BarGraph, unsigned char Fnt, char *Unit);
    void Pixel(unsigned short X,unsigned short Y,unsigned char Color);
    void ThickPixel(unsigned short X,unsigned short Y,unsigned char Thickness,unsigned char Color);
    void VLine(unsigned short Y1,unsigned short Y2,unsigned short X, unsigned char Color);
    void HLine(unsigned short X1,unsigned short X2,unsigned short Y, unsigned char Color);
    void NegHLine(unsigned short X1,unsigned short X2,unsigned short Y);
    void Frame(unsigned short X1, unsigned short Y1, unsigned short X2, unsigned short Y2, unsigned char Color);
    void Bar(unsigned short X1, unsigned short Y1, unsigned short X2, unsigned short Y2, unsigned char Color);
    void NegBar(unsigned short X1, unsigned short Y1, unsigned short X2, unsigned short Y2);
    void Line(short X1, short Y1, short X2, short Y2, unsigned char Color);
    void ThickLine(short X1, short Y1, short X2, short Y2, unsigned char Thickness, unsigned char Color);
    void Circle(short xm, short ym, short r, unsigned char Color);
    void ThickCircle(short xm, short ym, short r, unsigned char Thickness, unsigned char Color);
    void CircleFilled(short xm, short ym, short r, unsigned char Color);
    void Ellipse(short x0, short y0, short x1, short y1, unsigned char Color);
    void Arc(unsigned short X, unsigned short Y, unsigned short R, float SAngle, float EAngle, unsigned char Thickness, unsigned char Color);
    void BMP(unsigned short X, unsigned short Y, unsigned char *Bm, unsigned char Neg, unsigned char Color, unsigned char Transparent);
    void Write(unsigned short X, unsigned short Y, char* Line, unsigned char Fnt, unsigned char Neg, unsigned char Color);
    void JetPixel(unsigned short X, unsigned short Y, unsigned char Color);
};    

#endif
