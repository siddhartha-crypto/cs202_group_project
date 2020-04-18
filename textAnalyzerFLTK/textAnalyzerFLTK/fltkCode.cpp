//
//  fltkCode.cpp
//  textAnalyzerFLTK
//
//  Created by Sarah Carter on 3/3/20.
//  Copyright © 2020 Sarah Carter. All rights reserved.
//

#include <FL/Fl_Button.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Output.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Text_Display.H>
#include <FL/Fl_Text_Buffer.H>
#include <fstream>
#include <iostream>
#include <sstream>

#include "fltkCode.hpp"
#include "meta.hpp"

//Fl_Native_File_Chooser* fileFind = nullptr;
Fl_Box* progTitle = nullptr;
Fl_Box* instructions = nullptr;
Fl_Box* description = nullptr;
Fl_Box* resultDisp = nullptr;
Fl_Input* fileChoice = nullptr;
Fl_Input* fileTitle = nullptr;
Fl_Button* browser = nullptr;
Fl_Button* analyze = nullptr;
Fl_Button* display = nullptr;
Fl_Button* close = nullptr;
Fl_Button* quit = nullptr;
Fl_Output* report = nullptr;
Fl_Text_Display* reportDisp = nullptr;
Fl_Text_Buffer* buff = nullptr;

std::string userFile;
std::string processedFile;
std::string output;

void browserClicked(Fl_Widget*, void* data){
    Fl_Native_File_Chooser fileFind;
    fileFind.title("Select File to Analyze");
    fileFind.type(Fl_Native_File_Chooser::BROWSE_FILE);
    switch( fileFind.show() ){
        case -1: fprintf(stderr, "ERROR: %s\n", fileFind.errmsg()); break;    // ERROR
        case  1: fprintf(stderr, "*** CANCEL\n"); break;        // CANCEL
        default:                                 // PICKED FILE
            fileChoice->value(fileFind.filename());
          break;
    }
    std::string choice = fileChoice->value();
    std::istringstream is(choice);
    is >> userFile;
}

std::string fileNameNoExt(const std::string& file){
    auto pd = file.find_last_of(".");
    auto slsh = file.find_last_of("/");
    return file.substr(slsh, pd);
}

void textAnalysis_CB(Fl_Widget*, void* data){
    std::vector<string> categoryNames;
    std::vector<string> categoryFiles;
    /********************************************************************
     * Capture Category Names and Files
    ********************************************************************/
    captureCategories(categoryNames, categoryFiles);

    /********************************************************************
     * Create Baseline Analysis Files
    ********************************************************************/
    createAnalysisFiles(categoryNames, categoryFiles);

    /********************************************************************
     * Perform MeTA Analysis on Baseline Analysis Files
    ********************************************************************/
    performAnalysisOnBaselineFiles(categoryNames, categoryFiles);
}

void textDisplay_cb(Fl_Widget*, void* data){
    Fl_Window* win = (Fl_Window*)data;
    win->show();
    std::string line;
    std::ifstream fin(userFile);
    fileTitle->value(fileNameNoExt(userFile).c_str());
    while(std::getline(fin, line)){
        if(!fin){
            if(fin.eof())
                std::cout << "End of file\n";
            else{
                std::cout << "Error opening file\n";
                break;
            }
        }
        output += (line + "\n");
    }
    buff = new Fl_Text_Buffer();
    reportDisp -> buffer(buff);
    buff->text(output.c_str());
}

//Close window when "Exit" button is clicked
void OnExitClicked_cb(Fl_Widget* w, void* data){
    if(!data) return;
    Fl_Window* win = (Fl_Window*)data;
    win->hide();
}

Fl_Window* PopupWindow(){
    Fl_Window* pWin = new Fl_Window(800, 600,"");
    pWin -> begin();
    
    resultDisp = new Fl_Box(250,20,300,40, "Analysis Results");
    resultDisp->box(FL_UP_BOX);
    resultDisp->labelsize(20);
    
    fileTitle = new Fl_Input(250, 80, 300,40);
    
    reportDisp = new Fl_Text_Display(25, 160, 750, 300);
    close = new Fl_Button(350, 500, 100, 20, "Close");
    
    close->callback(OnExitClicked_cb, (void*) pWin);

    pWin -> end();
    return pWin;
}

Fl_Window* CreateWindow(){
    Fl_Window* win = new Fl_Window(800, 300, "");
    win->begin();
    
    progTitle = new Fl_Box(275,10,250, 50, "Text Analyzer");
    progTitle->box(FL_UP_BOX);
    progTitle->labelsize(24);
    
    description = new Fl_Box(100,70,600,50, "This program will take a text that you select"
                                           " and analyze the content for frequency \nof "
                             "specific words to categorize the text for you.");
    description->labelsize(14);
    
    instructions = new Fl_Box(175, 155, 500, 40, "Click the \"Browse\" button to search "
                              "for the file you wish to analyze.");
    
    fileChoice = new Fl_Input(175,200,500,45);
    browser = new Fl_Button(50, 210, 100, 20, "Browse");
    analyze = new Fl_Button(250, 250, 100, 20, "Analyze");
    display = new Fl_Button(375, 250, 100, 20, "Display Results");
    quit = new Fl_Button(350, 275, 100, 20, "Exit");
    
    quit -> callback(OnExitClicked_cb, (void*) win);

    browser->callback(browserClicked);
    analyze->callback(textAnalysis_CB);
    display->callback(textDisplay_cb, (void*)PopupWindow());

    

    win->end();
    return win;
}
