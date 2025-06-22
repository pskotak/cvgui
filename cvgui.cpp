#include "cvgui.h"

// Union TLB - pristup k longu po bytech/
typedef union {
    uint32_t L; ///< unsigned long
    unsigned char B[4]; ///< jednotlive byty LSB first
} TLB;

// 256 color palette
const unsigned long ColorPalette[] = {
    0x000000,0x800000,0x008000,0x808000,0x000080,0x800080,0x008080,0xc0c0c0, // 0
    0x808080,0xff0000,0x00ff00,0xffff00,0x0000ff,0xff00ff,0x00ffff,0xffffff, // 8
    0x000000,0x00005f,0x000087,0x0000af,0x0000d7,0x0000ff,0x005f00,0x005f5f, // 16
    0x005f87,0x005faf,0x005fd7,0x005fff,0x008700,0x00875f,0x008787,0x0087af, // 24
    0x0087d7,0x0087ff,0x00af00,0x00af5f,0x00af87,0x00afaf,0x00afd7,0x00afff, // 32
    0x00d700,0x00d75f,0x00d787,0x00d7af,0x00d7d7,0x00d7ff,0x00ff00,0x00ff5f, // 40
    0x00ff87,0x00ffaf,0x00ffd7,0x00ffff,0x5f0000,0x5f005f,0x5f0087,0x5f00af, // 48
    0x5f00d7,0x5f00ff,0x5f5f00,0x5f5f5f,0x5f5f87,0x5f5faf,0x5f5fd7,0x5f5fff, // 56
    0x5f8700,0x5f875f,0x5f8787,0x5f87af,0x5f87d7,0x5f87ff,0x5faf00,0x5faf5f, // 64
    0x5faf87,0x5fafaf,0x5fafd7,0x5fafff,0x5fd700,0x5fd75f,0x5fd787,0x5fd7af, // 72
    0x5fd7d7,0x5fd7ff,0x5fff00,0x5fff5f,0x5fff87,0x5fffaf,0x5fffd7,0x5fffff, // 80
    0x870000,0x87005f,0x870087,0x8700af,0x8700d7,0x8700ff,0x875f00,0x875f5f, // 88
    0x875f87,0x875faf,0x875fd7,0x875fff,0x878700,0x87875f,0x878787,0x8787af, // 96
    0x8787d7,0x8787ff,0x87af00,0x87af5f,0x87af87,0x87afaf,0x87afd7,0x87afff, // 104
    0x87d700,0x87d75f,0x87d787,0x87d7af,0x87d7d7,0x87d7ff,0x87ff00,0x87ff5f, // 112
    0x87ff87,0x87ffaf,0x87ffd7,0x87ffff,0xaf0000,0xaf005f,0xaf0087,0xaf00af, // 120
    0xaf00d7,0xaf00ff,0xaf5f00,0xaf5f5f,0xaf5f87,0xaf5faf,0xaf5fd7,0xaf5fff, // 128
    0xaf8700,0xaf875f,0xaf8787,0xaf87af,0xaf87d7,0xaf87ff,0xafaf00,0xafaf5f, // 136
    0xafaf87,0xafafaf,0xafafd7,0xafafff,0xafd700,0xafd75f,0xafd787,0xafd7af, // 144
    0xafd7d7,0xafd7ff,0xafff00,0xafff5f,0xafff87,0xafffaf,0xafffd7,0xafffff, // 152
    0xd70000,0xd7005f,0xd70087,0xd700af,0xd700d7,0xd700ff,0xd75f00,0xd75f5f, // 160
    0xd75f87,0xd75faf,0xd75fd7,0xd75fff,0xd78700,0xd7875f,0xd78787,0xd787af, // 168
    0xd787d7,0xd787ff,0xd7af00,0xd7af5f,0xd7af87,0xd7afaf,0xd7afd7,0xd7afff, // 176
    0xd7d700,0xd7d75f,0xd7d787,0xd7d7af,0xd7d7d7,0xd7d7ff,0xd7ff00,0xd7ff5f, // 184
    0xd7ff87,0xd7ffaf,0xd7ffd7,0xd7ffff,0xff0000,0xff005f,0xff0087,0xff00af, // 192
    0xff00d7,0xff00ff,0xff5f00,0xff5f5f,0xff5f87,0xff5faf,0xff5fd7,0xff5fff, // 200
    0xff8700,0xff875f,0xff8787,0xff87af,0xff87d7,0xff87ff,0xffaf00,0xffaf5f, // 208
    0xffaf87,0xffafaf,0xffafd7,0xffafff,0xffd700,0xffd75f,0xffd787,0xffd7af, // 216
    0xffd7d7,0xffd7ff,0xffff00,0xffff5f,0xffff87,0xffffaf,0xffffd7,0xffffff, // 224
    0x080808,0x121212,0x1c1c1c,0x262626,0x303030,0x3a3a3a,0x444444,0x4e4e4e, // 232
    0x585858,0x606060,0x666666,0x767676,0x808080,0x8a8a8a,0x949494,0x9e9e9e, // 240
    0xa8a8a8,0xb2b2b2,0xbcbcbc,0xc6c6c6,0xd0d0d0,0xdadada,0xe4e4e4,0xeeeeee  // 248
};

uint8_t JetMapR[256];
uint8_t JetMapG[256];
uint8_t JetMapB[256];

unsigned char VRAM[DispW][DispH];
unsigned char JetRAM[DispW][DispH];
bool DispJet = false;

unsigned short MouseX;
unsigned short MouseY;
bool NewMouse;
bool NewMouseMove;
bool NewMouseUp;

static void GUI_Callback(int event, int x, int y, int flags, void* userdata) {
    if (event == cv::EVENT_LBUTTONDOWN) {
        NewMouse = true;
        MouseX = x;
        MouseY = y;
    }
    else if (event == cv::EVENT_LBUTTONUP) {
        NewMouseUp = true;
    }
    else if (event == cv::EVENT_MOUSEMOVE) {
        NewMouseMove = true;
        MouseX = x;
        MouseY = y;
    }
}

cvgui::cvgui() {
    // Calculate JetMap
    float ratio = 0.0;    
    for (int i=0;i<256;i++) {
        int normalized = int(ratio * 256 * 6);
        int x = normalized % 256;
        int red = 0, grn = 0, blu = 0;
        switch(normalized / 256)
        {
            case 0: red = 255;      grn = x;        blu = 0;       break;//red
            case 1: red = 255 - x;  grn = 255;      blu = 0;       break;//yellow
            case 2: red = 0;        grn = 255;      blu = x;       break;//green
            case 3: red = 0;        grn = 255 - x;  blu = 255;     break;//cyan
            case 4: red = x;        grn = 0;        blu = 255;     break;//blue
            case 5: red = 255;      grn = 0;        blu = 255 - x; break;//magenta
        }
        JetMapR[255-i] = red;
        JetMapG[255-i] = grn;
        JetMapB[255-i] = blu;
        ratio = ratio + (1.0/255.0);
    }
    
    screen = cv::Mat(DispH,DispW,CV_8UC3);
    //screen.setTo(cv::Scalar(255,255,255));
    screen.setTo(cv::Scalar(0,0,0));
    QuitProgram = false;
    MouseX = MouseY = 0;
    NewMouse = false;
    NewMouseMove = false;
    NewMouseUp = false;

    NewKey = false;
    Key = 0;

    cv::namedWindow(gui_window,cv::WINDOW_AUTOSIZE);
    //cv::moveWindow(gui_window,0,25);
    cv::setMouseCallback(gui_window, GUI_Callback, NULL);
    FullViewport();
    Cls(clBlack);
}

void cvgui::Update() {
    char inKey = cv::waitKey(1); //10
    if (inKey >= 0) {
        switch (inKey) {
            case 27: // Esc
                QuitProgram = true;
                break;

            default:
                Key = inKey;
                NewKey = 1;
                break;
        }
    }

}

bool cvgui::inPort(unsigned short X, unsigned short Y) {
    if ((X >= VpX1) && (X < VpX2) && (Y >= VpY1) && (Y < VpY2))
        return true;
    else
        return false;
}

void cvgui::SetViewport(unsigned short X1, unsigned short Y1, unsigned short X2, unsigned short Y2) {
    VpX1 = X1; VpY1 = Y1; VpX2 = X2; VpY2 = Y2;
}

void cvgui::ClearViewport(unsigned char Color) {
    Bar(VpX1,VpY1,VpX2,VpY2,Color);
}

void cvgui::FullViewport() {
    VpX1 = 0; VpY1 = 0; VpX2 = DispW; VpY2 = DispH;
}

void cvgui::Show() {
    NewMouse = false; // Pokud nikdo neobslouzil kliknuti, nulovat pro dalsi kolo
    cv::imshow(gui_window,screen);
//     if (!full_rgb_image.empty()) {
//         cv::Mat qRGB;
//         cvtColor(full_rgb_image,qRGB,CV_YUV2BGR_YUY2);
//         cv::imshow("full RGB",qRGB);
//     }
}

#define HelpStepY 22
void cvgui::Draw() {
    FullViewport();
    //HLine(0,DispW-1,ScrTop-1,clBlack);
    Dump();
}

void cvgui::Dump() {
    TLB L;
    cv::Vec3b Color;
    unsigned char B;

    for(unsigned short X=0;X<DispW;X++) {
        for(unsigned short Y=0;Y<DispH;Y++) {
            if (DispJet) {
                B = JetRAM[X][Y];
                if (B == 255) {
                    B = VRAM[X][Y];            
                    L.L = ColorPalette[B];
                    Color = cv::Vec3b(L.B[0],L.B[1],L.B[2]);
                }
                else {
                    Color = cv::Vec3b(JetMapB[B],JetMapG[B],JetMapR[B]);
                }
            }
            else {
                B = VRAM[X][Y];            
                L.L = ColorPalette[B];
                Color = cv::Vec3b(L.B[0],L.B[1],L.B[2]);
            }            
            screen.at<cv::Vec3b>(Y,X) = Color;
        }
    }
    DispJet = false;
}

long cvgui::LAbs(long L) {
    if (L >= 0)
        return L;
    else
        return (~L + 1);
}

void cvgui::Cls(unsigned char Color) {
    memset(VRAM,Color,sizeof(VRAM));
}

void cvgui::ClsJet() {
    memset(JetRAM,255,sizeof(JetRAM));
}

void cvgui::Pixel(unsigned short X, unsigned short Y, unsigned char Color) {
    if (inPort(X,Y))
        VRAM[X][Y] = Color;
}

void cvgui::ThickPixel(unsigned short X,unsigned short Y,unsigned char Thickness,unsigned char Color) {
    if (inPort(X,Y)) {
        if (Thickness == 1)
            Pixel(X,Y,Color);
        else if (Thickness == 2) {
            Bar(X-1,Y-1,X+1,Y+1,Color);
        }
        else if (Thickness == 3) {
            Bar(X-3,Y-3,X+3,Y+3,Color);
        }
        else if (Thickness >= 3) {
            Bar(X-4,Y-4,X+4,Y+4,Color);
        }
    }
}

void cvgui::VLine(unsigned short Y1,unsigned short Y2,unsigned short X, unsigned char Color) {
    unsigned short YY1,YY2,I;

    if (Y1 == Y2) {
        Pixel(X,Y1,Color);
    }
    else {
        if (Y1 < Y2) {
            YY1 = Y1; YY2 = Y2;
        }
        else {
            YY1 = Y2; YY2 = Y1;
        }
        for(I=YY1;I<YY2+1;I++) {
            Pixel(X,I,Color);
            //VRAM[X][I] = Color;
        }
    }
}

void cvgui::HLine(unsigned short X1,unsigned short X2,unsigned short Y, unsigned char Color) {
    unsigned short I,XX1,XX2;

    if (X1 == X2) {
        Pixel(X1,Y,Color);
    }
    else {
        if (X1 < X2) {
            XX1 = X1; XX2 = X2;
        }
        else {
            XX1 = X2; XX2 = X1;
        }
        for (I=XX1;I<XX2+1;I++) {
            Pixel(I,Y,Color);
            //VRAM[I][Y] = Color;
        }
    }
}

void cvgui::NegHLine(unsigned short X1,unsigned short X2,unsigned short Y) {
    unsigned short I,XX1,XX2;
    unsigned char B;

    if (X1 == X2) {
        if (inPort(X1,Y)) {
            B = VRAM[X1][Y];
            VRAM[X1][Y] = ~B;
        }
    }
    else {
        if (X1 < X2) {
            XX1 = X1; XX2 = X2;
        }
        else {
            XX1 = X2; XX2 = X1;
        }
        for (I=XX1;I<XX2+1;I++) {
            if (inPort(I,Y)) {
                B = VRAM[I][Y];
                VRAM[I][Y] = ~B;
            }
        }
    }
}

void cvgui::Frame(unsigned short X1, unsigned short Y1, unsigned short X2, unsigned short Y2, unsigned char Color) {
    HLine(X1,X2,Y1,Color);
    HLine(X1,X2,Y2,Color);
    VLine(Y1,Y2,X1,Color);
    VLine(Y1,Y2,X2,Color);
}

void cvgui::Bar(unsigned short X1, unsigned short Y1, unsigned short X2, unsigned short Y2, unsigned char Color) {
    unsigned short I;

    for (I=Y1;I<(Y2+1);I++) {
        HLine(X1,X2,I,Color);
    }
}

void cvgui::NegBar(unsigned short X1, unsigned short Y1, unsigned short X2, unsigned short Y2) {
    unsigned short I;

    for (I=Y1;I<(Y2+1);I++) {
        NegHLine(X1,X2,I);
    }
}

void cvgui::Line(short X1, short Y1, short X2, short Y2, unsigned char Color) {
    if (inPort(X1,Y1) && inPort(X2,Y2)) {
        long dx =  LAbs(X2-X1), sx = X1<X2 ? 1 : -1;
        long dy = -LAbs(Y2-Y1), sy = Y1<Y2 ? 1 : -1;
        long err = dx+dy, e2; // error value e_xy

        for(;;) {  // loop
            Pixel(X1,Y1,Color);
            if (X1==X2 && Y1==Y2) break;
            e2 = 2*err;
            if (e2 >= dy) { err += dy; X1 += sx; } // e_xy+e_x > 0
            if (e2 <= dx) { err += dx; Y1 += sy; } // e_xy+e_y < 0
        }
    }
}

void cvgui::ThickLine(short X1, short Y1, short X2, short Y2, unsigned char Thickness, unsigned char Color) {
    if (inPort(X1,Y1) && inPort(X2,Y2)) {
        long dx =  LAbs(X2-X1), sx = X1<X2 ? 1 : -1;
        long dy = -LAbs(Y2-Y1), sy = Y1<Y2 ? 1 : -1;
        long err = dx+dy, e2; // error value e_xy

        for(;;) {  // loop
            ThickPixel(X1,Y1,Thickness,Color);
            if (X1==X2 && Y1==Y2) break;
            e2 = 2*err;
            if (e2 >= dy) { err += dy; X1 += sx; } // e_xy+e_x > 0
            if (e2 <= dx) { err += dx; Y1 += sy; } // e_xy+e_y < 0
        }
    }
}

void cvgui::Circle(short xm, short ym, short r, unsigned char Color) {
    long x = -r, y = 0, err = 2-2*r; // II. Quadrant

    do {
        Pixel(xm-x,ym+y,Color); //   I. Quadrant
        Pixel(xm-y,ym-x,Color); //  II. Quadrant
        Pixel(xm+x,ym-y,Color); // III. Quadrant
        Pixel(xm+y,ym+x,Color); //  IV. Quadrant
        r = err;
        if (r <= y) err += ++y*2+1;           // e_xy+e_y < 0
        if (r > x || err > y) err += ++x*2+1; // e_xy+e_x > 0 or no 2nd y-step
    } while (x < 0);
}

void cvgui::ThickCircle(short xm, short ym, short r, unsigned char Thickness, unsigned char Color) {
    long x = -r, y = 0, err = 2-2*r; // II. Quadrant

    do {
        ThickPixel(xm-x,ym+y,Thickness,Color); //   I. Quadrant
        ThickPixel(xm-y,ym-x,Thickness,Color); //  II. Quadrant
        ThickPixel(xm+x,ym-y,Thickness,Color); // III. Quadrant
        ThickPixel(xm+y,ym+x,Thickness,Color); //  IV. Quadrant
        r = err;
        if (r <= y) err += ++y*2+1;           // e_xy+e_y < 0
        if (r > x || err > y) err += ++x*2+1; // e_xy+e_x > 0 or no 2nd y-step
    } while (x < 0);
}

void cvgui::CircleFilled(short xm, short ym, short r, unsigned char Color) {
    long x = -r, y = 0, err = 2-2*r; // II. Quadrant

    do {
        // Vypln
        HLine(xm+x,xm-x,ym+y,Color);
        HLine(xm+x,xm-x,ym-y,Color);
        r = err;
        if (r <= y) err += ++y*2+1;           // e_xy+e_y < 0
        if (r > x || err > y) err += ++x*2+1; // e_xy+e_x > 0 or no 2nd y-step
    } while (x < 0);
}

void cvgui::Ellipse(short x0, short y0, short x1, short y1, unsigned char Color) {
    long a = LAbs(x1-x0), b = LAbs(y1-y0), b1 = b&1; // values of diameter
    long dx = 4*(1-a)*b*b, dy = 4*(b1+1)*a*a; // error increment
    long err = dx+dy+b1*a*a, e2; // error of 1.step

    if (x0 > x1) { x0 = x1; x1 += a; } // if called with swapped points
    if (y0 > y1) y0 = y1; // .. exchange them
    y0 += (b+1)/2; y1 = y0-b1;   // starting pixel
    a *= 8*a; b1 = 8*b*b;

    do {
        Pixel(x1,y0,Color); //   I. Quadrant
        Pixel(x0,y0,Color); //  II. Quadrant
        Pixel(x0,y1,Color); // III. Quadrant
        Pixel(x1,y1,Color); //  IV. Quadrant
        e2 = 2*err;
        if (e2 <= dy) { y0++; y1--; err += dy += a; }  // y step
        if (e2 >= dx || 2*err > dy) { x0++; x1--; err += dx += b1; } // x step
    } while (x0 <= x1);

    while (y0-y1 < b) {  // too early stop of flat ellipses a=1
        Pixel(x0-1,y0,Color); // -> finish tip of ellipse
        Pixel(x1+1,y0++,Color);
        Pixel(x0-1,y1,Color);
        Pixel(x1+1,y1--,Color);
    }
}

void cvgui::Arc(unsigned short X, unsigned short Y, unsigned short R, float SAngle, float EAngle, unsigned char Thickness, unsigned char Color) {
    float AngleInc = 2*M_PI / 1000;
    float Theta;
    float Fx,Fy;

    SAngle -= M_PI/2.0; EAngle -= M_PI/2.0;
    for(Theta = SAngle; Theta < EAngle; Theta += AngleInc) {
        Fx = X;
        Fy = Y;
        Fx += R * cos(Theta);
        Fy += R * sin(Theta);
        //   if (Theta == SAngle)
        //    Pixel(floor(Fx+0.5),floor(Fy+0.5),clRed);
        //   else
        Pixel(floor(Fx+0.5),floor(Fy+0.5),Color);
        if (Thickness == 2) {
            Pixel(floor(Fx+0.5)+1,floor(Fy+0.5),Color);
            Pixel(floor(Fx+0.5)-1,floor(Fy+0.5),Color);
            Pixel(floor(Fx+0.5),floor(Fy+0.5)+1,Color);
            Pixel(floor(Fx+0.5),floor(Fy+0.5)-1,Color);
        }
        if (Thickness == 3) {
            Pixel(floor(Fx+0.5)+1,floor(Fy+0.5)+1,Color);
            Pixel(floor(Fx+0.5)-1,floor(Fy+0.5)+1,Color);
            Pixel(floor(Fx+0.5)+1,floor(Fy+0.5)-1,Color);
            Pixel(floor(Fx+0.5)-1,floor(Fy+0.5)-1,Color);
            Pixel(floor(Fx+0.5)+2,floor(Fy+0.5),Color);
            Pixel(floor(Fx+0.5)-2,floor(Fy+0.5),Color);
            Pixel(floor(Fx+0.5),floor(Fy+0.5)+2,Color);
            Pixel(floor(Fx+0.5),floor(Fy+0.5)-2,Color);
        }
    }
}

void cvgui::BMP(unsigned short X, unsigned short Y, unsigned char *Bm, unsigned char Neg, unsigned char Color, unsigned char Transparent) {
    unsigned char* M; // Ukazatel do bitmapy
    unsigned char W,H,Wi,B;
    unsigned char ScanLine[DispW+100];
    unsigned short I,J,K,Len;

    W = *Bm; // Sirka bitmapy
    H = *(Bm+1); // Vyska bitmapy
    M = Bm+2; // Ukaz na treti byte bitmapy - data
    if (W % 8 == 0)
        Len = (W / 8);
    else
        Len = (W / 8)+1;

    //Frame(X,Y,X+W,Y+H,Color); // Test

    for (K=Y;K<Y+H;K++) {
        J = 0; Wi = 1;
        if (Transparent) {
            for (I=0;I<Len;I++) {
                B = *(M+I);
                if (inPort(X+J,K)) {
                    if (B & 0x80) ScanLine[J] = Color; else ScanLine[J] = VRAM[X+J][K]; J++; Wi++; if (Wi > W) break;
                    if (B & 0x40) ScanLine[J] = Color; else ScanLine[J] = VRAM[X+J][K]; J++; Wi++; if (Wi > W) break;
                    if (B & 0x20) ScanLine[J] = Color; else ScanLine[J] = VRAM[X+J][K]; J++; Wi++; if (Wi > W) break;
                    if (B & 0x10) ScanLine[J] = Color; else ScanLine[J] = VRAM[X+J][K]; J++; Wi++; if (Wi > W) break;
                    if (B & 0x08) ScanLine[J] = Color; else ScanLine[J] = VRAM[X+J][K]; J++; Wi++; if (Wi > W) break;
                    if (B & 0x04) ScanLine[J] = Color; else ScanLine[J] = VRAM[X+J][K]; J++; Wi++; if (Wi > W) break;
                    if (B & 0x02) ScanLine[J] = Color; else ScanLine[J] = VRAM[X+J][K]; J++; Wi++; if (Wi > W) break;
                    if (B & 0x01) ScanLine[J] = Color; else ScanLine[J] = VRAM[X+J][K]; J++; Wi++; if (Wi > W) break;
                }
            }
        }
        else {
            // Copy Bitmap to scanline (non transparent)
            for (I=0;I<Len;I++) {
                B = *(M+I);
                if (B & 0x80) ScanLine[J] = Color; else ScanLine[J] = clWhite; J++; Wi++; if (Wi > W) break;
                if (B & 0x40) ScanLine[J] = Color; else ScanLine[J] = clWhite; J++; Wi++; if (Wi > W) break;
                if (B & 0x20) ScanLine[J] = Color; else ScanLine[J] = clWhite; J++; Wi++; if (Wi > W) break;
                if (B & 0x10) ScanLine[J] = Color; else ScanLine[J] = clWhite; J++; Wi++; if (Wi > W) break;
                if (B & 0x08) ScanLine[J] = Color; else ScanLine[J] = clWhite; J++; Wi++; if (Wi > W) break;
                if (B & 0x04) ScanLine[J] = Color; else ScanLine[J] = clWhite; J++; Wi++; if (Wi > W) break;
                if (B & 0x02) ScanLine[J] = Color; else ScanLine[J] = clWhite; J++; Wi++; if (Wi > W) break;
                if (B & 0x01) ScanLine[J] = Color; else ScanLine[J] = clWhite; J++; Wi++; if (Wi > W) break;
            }
        }
        //J = 0;
        for (I=0;I<Len;I++) {
            for (J=0;J<W;J++) {
                if (inPort(X+J,K))
                    VRAM[X+J][K] = ScanLine[J];
            }
        }
        M += Len;
    }
}

void cvgui::Write(unsigned short X, unsigned short Y, char* Line, unsigned char Fnt, unsigned char Neg, unsigned char Color) {
    unsigned short CharW, CharH, BytLen;
    unsigned short PosX, XCorr, YCorr;
    unsigned char C;
    unsigned char CBuf[2048]; // Pomocny buffer bitmapy jednoho znaku
    short TstY;

    XCorr = 0;
    YCorr = 0;
//     if (Fnt == f_Small_7) // f_Small_7 je vygenerovany blbe -> ma o 2 pixely vetsi vysku
//         YCorr = 2;

    // Musel jsem zavest korekci rozestupu znaku, protoze nektere fonty maji o 1 uzsi bitmapu (Tahoma)
//     switch (Fnt) {
//         case f_Tahoma_10:
//         case f_Tahoma_10_Bold:
//         case f_Tahoma_16:
//         case f_MS_Sans_14:
//         case f_MS_Sans_14_Bold:
//         case f_Fixed_5_7:
//         case f_Fixed_3_5:
//             XCorr = 1;
//             break;
//         default:
//             XCorr = 0;
//             break;
//     }
    PosX = X;
    C = *Line;
    while (C != 0) {
        if (C > 0x1F) { // Vynechat ridici znaky (0..0x1F)
            C = C-FIRST_CHAR;
            CharW = FontTable[Fnt][C].Width;
            CharH = FontTable[Fnt][C].Height;
            BytLen = ((CharW / 8) + 1) * CharH;
            CBuf[0] = CharW; // Prvni byte je sirka znaku v pixelech
            CBuf[1] = CharH; // Druhy byte je vyska znaku v pixelech
            memcpy(CBuf+2,FontTable[Fnt][C].Ptr,BytLen); // a od tretiho byte nasleduje bitova mapa zarovnana vlevo
            TstY = Y-YCorr;
            if (TstY < 0) Y = 0; else Y = Y-YCorr;
            BMP(PosX,Y,(unsigned char *) &CBuf,Neg,Color,bmpTransparent);
            //BMP(PosX,Y-YCorr,(unsigned char *) &CBuf,Neg,Color,bmpSolid);
            PosX = PosX + CharW + XCorr;
        }
        Line++;
        C = *Line;
    }
}

void cvgui::DrawButton(TButton* Button) {
    unsigned short W,H;

    Bar(Button->X,Button->Y,Button->X+Button->W,Button->Y+Button->H,Button->ButtonBkgColor);
    Frame(Button->X,Button->Y,Button->X+Button->W,Button->Y+Button->H,Button->ButtonFrameColor);
    CalcPixelSize((unsigned char*) &Button->Caption,Button->ButtonCaptionFont,&W,&H);
    W = W/2; H = H/2;
    W = (Button->W/2)-W; H = (Button->H/2)-H;
    Write(Button->X+W,Button->Y+H-3,(char *)Button->Caption,Button->ButtonCaptionFont,0,Button->ButtonCaptionColor);
}

void cvgui::HandleButton(TButton* Button) {
    if (NewMouse) {
        if (((MouseX > Button->X) && (MouseX < (Button->X+Button->W))) && ((MouseY > Button->Y) && (MouseY < (Button->Y+Button->H)))) {
            NewMouse = false;
            Button->Pressed = 1;
        }
    }
}

void cvgui::DrawButtonLeft(TButton* Button) {
    unsigned short W,H;

    Bar(Button->X,Button->Y,Button->X+Button->W,Button->Y+Button->H,Button->ButtonBkgColor);
    Frame(Button->X,Button->Y,Button->X+Button->W,Button->Y+Button->H,Button->ButtonFrameColor);
    CalcPixelSize((unsigned char*) &Button->Caption,Button->ButtonCaptionFont,&W,&H);
    //W = W/2; 
    H = H/2;
    //W = (Button->W/2)-W; 
    H = (Button->H/2)-H;
    Write(Button->X+2,Button->Y+H-3,(char *)Button->Caption,Button->ButtonCaptionFont,0,Button->ButtonCaptionColor);
}

void cvgui::DrawSlider(TSlider* Slider) {
    unsigned short VX;
    
    Bar(Slider->X,Slider->Y,Slider->X+Slider->W,Slider->Y+Slider->H,Slider->SliderBkgColor);
    Frame(Slider->X,Slider->Y,Slider->X+Slider->W,Slider->Y+Slider->H,Slider->SliderFrameColor);
    float R = Slider->W;
    R = R / Slider->MaxVal;
    VX = floor((R*Slider->Value)+0.5);
    Bar(Slider->X+VX,Slider->Y,Slider->X+VX+2,Slider->Y+Slider->H,clBlack);
}


void cvgui::HandleSlider(TSlider* Slider) {
    if (NewMouse) {
        if (((MouseX > Slider->X) && (MouseX < (Slider->X+Slider->W))) && ((MouseY > Slider->Y) && (MouseY < (Slider->Y+Slider->H)))) {
            float R = Slider->W;
            R = R / Slider->MaxVal;
            R = (MouseX-Slider->X) / R;
            Slider->Value = floor(R+0.5);
            Slider->Activated = 1;
            Slider->Changed = 1;
        }
    }
    if (NewMouseUp) {
        Slider->Activated = 0;
    }
    if (NewMouseMove && Slider->Activated) {
        if (((MouseX > Slider->X) && (MouseX < (Slider->X+Slider->W))) && ((MouseY > Slider->Y) && (MouseY < (Slider->Y+Slider->H)))) {
            float R = Slider->W;
            R = R / Slider->MaxVal;
            R = (MouseX-Slider->X) / R;
            Slider->Value = floor(R+0.5);
            Slider->Changed = 1;
        }
    }
}

void cvgui::DrawIntBarGraph(TIntBarGraph* BarGraph) {
    unsigned short VX;
    
    Bar(BarGraph->X,BarGraph->Y,BarGraph->X+BarGraph->W,BarGraph->Y+BarGraph->H,BarGraph->BkgColor);
    Frame(BarGraph->X,BarGraph->Y,BarGraph->X+BarGraph->W,BarGraph->Y+BarGraph->H,BarGraph->FrameColor);
    float R = BarGraph->W;
    R = R / BarGraph->MaxVal;
    VX = floor((R*BarGraph->Value)+0.5);
    Bar(BarGraph->X+1,BarGraph->Y+1,BarGraph->X+VX+2,BarGraph->Y+BarGraph->H-1,BarGraph->BarColor);
}

void cvgui::DrawFloatBarGraph(TFloatBarGraph* BarGraph) {
    unsigned short VX;
    
    Bar(BarGraph->X,BarGraph->Y,BarGraph->X+BarGraph->W,BarGraph->Y+BarGraph->H,BarGraph->BkgColor);
    Frame(BarGraph->X,BarGraph->Y,BarGraph->X+BarGraph->W,BarGraph->Y+BarGraph->H,BarGraph->FrameColor);
    float R = BarGraph->W;
    R = R / BarGraph->MaxVal;
    VX = floor((R*BarGraph->Value)+0.5);
    Bar(BarGraph->X+1,BarGraph->Y+1,BarGraph->X+VX+2,BarGraph->Y+BarGraph->H-1,BarGraph->BarColor);
}

void cvgui::DrawFloatValBarGraph(TFloatBarGraph* BarGraph, unsigned char Fnt, char *Unit) {
    unsigned short VX;
    
    Bar(BarGraph->X,BarGraph->Y,BarGraph->X+BarGraph->W,BarGraph->Y+BarGraph->H,BarGraph->BkgColor);
    Frame(BarGraph->X,BarGraph->Y,BarGraph->X+BarGraph->W,BarGraph->Y+BarGraph->H,BarGraph->FrameColor);
    float R = BarGraph->W;
    R = R / BarGraph->MaxVal;
    VX = floor((R*BarGraph->Value)+0.5);
    Bar(BarGraph->X+1,BarGraph->Y+1,BarGraph->X+VX+2,BarGraph->Y+BarGraph->H-1,BarGraph->BarColor);
    sprintf(PrnBuf,"%.1f %s",BarGraph->Value,Unit);
    Write(BarGraph->X+VX+2+10,(BarGraph->H/2)+BarGraph->Y-12,PrnBuf,Fnt,0,BarGraph->ValColor);
}


void cvgui::JetPixel(unsigned short X, unsigned short Y, unsigned char Color) {
    JetRAM[X][Y] = Color;
    DispJet = true;
}
