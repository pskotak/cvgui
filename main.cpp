#include <iostream>
#include "cvgui.h"

int main(int argc, char **argv) {
    char Key;
    
    std::cout << "CvGui example" << std::endl;

	cvgui gui;
    
    while (!gui.QuitProgram) {
        gui.Update();
        
        if (gui.NewKey) {
            gui.NewKey = false;
            Key = gui.Key;
        }
        gui.Draw();
        gui.Show();
    }
    return 0;
}
