#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <iostream>
#include <string>
#include <random>
#include <stdio.h>
#include <assert.h>
#include <cmath>

class WordList {
const char* list[12];

public:
    WordList(){
        const char* list2 [12]{"CIGAR", "CRANE", "BRAIN", "DWARF", "MODEL", "QUIET", "GRADE", "SLATE", "REACT", "DEATH", "FEIGN", "BENCH"};
        for (int i=0; i<12; i++) list[i] = list2[i];
    }

    const char* choose_word(){
        srand(time(0));
        int number = rand();
        return list[number%12];
    }
};

class Tracker { 
private:
    int x = 0, y = 0; // Keeps track of x, y indices of tiles in play
    // x is horizontal, y is vertical
public:
    void next_letter() {
        if (x!=4) x++;
        else{
            y++;
            x=0;
        }
    }
    void prev_letter() {
        if (x!=0) x--;
        else if (y!=0){
            y--;
            x=4;
        }
    }
    int get_x() {return x;}
    int get_y() {return y;}
};
class Tile;

class Wordle{
const char * answer;
Tracker tracker;
int attempts;
Tile * tiles[5][10];
int size;
bool finish = false;

    friend Tile;

public: 
    Wordle (int size, int attempts);
    void set_answer(const char* answer) {
        this->answer = answer;
    }
    ~Wordle();
};

class Tile : public Fl_Widget {

char* letter;    // letter inside the tile
int result;     // if letter is wrong:1 , correct:1, wrong position: 0
int size;       // size of the tile
Wordle *wordle;      // back-pointer
int x, y, w, h;
int code=3;
public: 
    Tile():Fl_Widget(0, 0 ,0 ,0){};
    Tile (Wordle *wordle, int size, int x, int y, int w, int h) : Fl_Widget(x, y, w, h) {
        this->size = size;
        this->wordle = wordle;
        this->x = x; this->y = y ; this->w = w; this->h = h;
        this->letter = new char;
    }
    
    void check(){   // to be used when at the beginning of the next line
        for (int j = 0; j<5; j++){
            
            for (int i=0; i<5; i++){
                if (wordle->answer[i] == (wordle->tiles[j][(wordle->tracker.get_y())-1]->letter)[0] && (j==i)){
                    wordle->tiles[j][(wordle->tracker.get_y())-1]->set_color(100);
                    wordle->tiles[j][(wordle->tracker.get_y())-1]->set_code(1);
                }
                else if (wordle->answer[i] == (wordle->tiles[j][(wordle->tracker.get_y())-1]->letter)[0] && (j!=i)){
                    wordle->tiles[j][(wordle->tracker.get_y())-1]->set_color(94);
                    wordle->tiles[j][(wordle->tracker.get_y())-1]->set_code(2);
                }
            }
            if (wordle->tiles[j][(wordle->tracker.get_y())-1]->code == 3) wordle->tiles[j][(wordle->tracker.get_y())-1]->set_color(48);
        }
        
        
    }
    
    void draw() {
        fl_color((int) 7);
        fl_rectf(x, y, w, h);
        fl_color(FL_BLACK);
        fl_rect(x, y, w, h);
    }

    void set_letter(const char* letter) {
        *(this->letter) = letter[0];
        fl_color(0);
        fl_font(FL_HELVETICA, size/2);
        fl_draw(letter, x, y, w, h, FL_ALIGN_CENTER);
    }

    void remove_letter() {
        *(this->letter) = '\0';
        fl_color((int) 7);
        fl_rectf(x, y, w, h);
        fl_color(FL_BLACK);
        fl_rect(x, y, w, h);
    }
    
    void finish(){
        double height = 5;
        for (int frame = 0; frame < 100; frame++){
            Fl::wait(0.003);
            fl_color((int) 100);
            fl_rectf(x, y - height*sin(8*atan(1)*frame/200), w, h);
            fl_color((int) 207);
            fl_rectf(x, y+ h - height*sin(8*atan(1)*frame/200), w, height*sin(8*atan(1)*frame/200));
            fl_rectf(x, y - height, w, height - height*sin(8*atan(1)*frame/200));
            fl_color(7);
            fl_font(FL_HELVETICA, size/2);
            fl_draw(letter, x,  y - height*sin(8*atan(1)*frame/200) , w, h, FL_ALIGN_CENTER);
        }    
        fl_color((int) 100);
        fl_rectf(x, y, w, h);
        fl_color(7);
        fl_font(FL_HELVETICA, size/2);
        fl_draw(letter, x, y, w, h, FL_ALIGN_CENTER);
        fl_color(FL_WHITE);
        fl_rect(x, y, w, h);  
    }
    
    void set_color(int color){
        for (int i=0; i<100; i++){
            Fl::wait(0.005);
            fl_color((int) 207);
            fl_rectf(x, y, w, (h - h*cos(8*atan(1)*i/100))/2);
            fl_rectf(x, y + h - (h - h*cos(8*atan(1)*i/100))/2, w, (h - h*cos(8*atan(1)*i/100))/2);
            if (i < 50) fl_color((int) 7);
            else fl_color((int) color);
            fl_rectf(x, y + (h - h*cos(8*atan(1)*i/100))/2, w, h*cos(8*atan(1)*i/100));
        }
        fl_color((int) color);
        fl_rectf(x, y, w, h);
        fl_color(7);
        fl_font(FL_HELVETICA, size/2);
        fl_draw(letter, x, y, w, h, FL_ALIGN_CENTER);
        fl_color(FL_WHITE);
        fl_rect(x, y, w, h);  
    }

    void set_code(int code){
        this->code = code;
    }

    const char* get_letter(){
        return letter;
    }

    virtual int handle(int e){
        if (e == FL_KEYDOWN && !(wordle->finish)){
            const char* c = Fl::event_text();
            int d = c[0];
            if (d>=65 && d<=90){
                if (wordle->tracker.get_x()== 4 && wordle->tracker.get_y() == wordle->attempts-1){
                    
                    wordle->tiles[wordle->tracker.get_x()][wordle->tracker.get_y()]->set_letter(c);
                    wordle->tracker.next_letter();
                    wordle->tiles[wordle->tracker.get_x()][wordle->tracker.get_y()]->check();
                    wordle->finish = true;
                    bool result = true;
                    for (int i = 0; i<5; i++){
                        result = result && (wordle->tiles[i][wordle->tracker.get_y()-1]->code == 1);
                    }
                    if (result == true){
                        for (int i=0; i<5; i++){
                            wordle->tiles[i][wordle->tracker.get_y()-1]->finish();
                        }
                    }
                    
                }
                
                else if (wordle->tracker.get_x() == 4){
                    
                    wordle->tiles[wordle->tracker.get_x()][wordle->tracker.get_y()]->set_letter(c);
                    wordle->tracker.next_letter();
                    wordle->tiles[wordle->tracker.get_x()][wordle->tracker.get_y()]->check();
                    bool result = true;
                    for (int i = 0; i<5; i++){
                        result = result && (wordle->tiles[i][wordle->tracker.get_y()-1]->code == 1);
                    }
                    if (result == true){
                        wordle->finish = true;
                        for (int i=0; i<5; i++){
                            wordle->tiles[i][wordle->tracker.get_y()-1]->finish();
                        }
                        
                    }
                    
                }
                else if (wordle->tracker.get_x() != 4) {
                    
                    wordle->tiles[wordle->tracker.get_x()][wordle->tracker.get_y()]->set_letter(c);
                    wordle->tracker.next_letter();
                }
            }
            if (d == 8){
                if (wordle->tracker.get_x() > 0){
                    wordle->tracker.prev_letter();
                    wordle->tiles[wordle->tracker.get_x()][wordle->tracker.get_y()]->remove_letter();
                }
            }
        }
        return 1;
    }

    ~Tile(){
        delete[] letter;
    }
};


Wordle::Wordle (int size,int attempts) {
    this->size = size; // size is the vertical size
    this->attempts = attempts;
    assert(attempts>=5 && attempts<=10);
    //Constructing tiles
    for (int i=0 ; i<5*attempts; i++){
        tiles[i%5][i/5] = new Tile(this, size/attempts, size*(i%5)/attempts + 840/2 - (size*5/attempts)/2, size*(i/5)/attempts + size/attempts , size/attempts-5, size/attempts-5);
    }
    tiles[0][attempts] = new Tile(this, 0, 0, 0, 0, 0);  // IMPORTANT STEP
}


Wordle::~Wordle() {
    delete[] tiles;
}


int main () {
    Fl_Window* play = new Fl_Window(840, 840, "WORDLE : CS154");
    play->color((int) 207);
    Wordle *game = new Wordle(600, 6);   // SIZE, ATTEMPTS
    WordList words;
    game->set_answer(words.choose_word());
    play->show();
    Fl::run();
    
}
