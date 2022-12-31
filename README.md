This is a fairly large program with four classes working together : 
1. WordList ( Stored the words and has functions for random selection )
2. Tracker ( Keeps track of which Tile is taking input )
3. Tile ( Individual Tiles of the grid )
4. Wordle ( Main grid containing the tiles ) : The most supreme class (highest level of abstraction)

First the instructions of the program :
1. Unzip the file and compile using fltk-config --compile wordle.cpp
2. Run the code using ./wordle

![5x5:wordle](https://github.com/Ihsoj-Mahos/CS154-wordle/blob/main/5-wordle.png)

Features : 
1. Based on the popular wordle game which took the world by storm recently
2. Green : Correct letter as well as position, Yellow : Correct letter but wrong position, Gray : Wrong letter
3. Fool-proof : Ignores all keystrokes where the input isn't a capital alphabet (shift+lowercase works!) - backspace is
allowed if the user isn't tryig to change the previous word whose result has already come .
4. Generalisable : The constructor has both size of grid and the number of attempts, thus can be used to 
play multiple games in the same window that too with different number of attempts (demonstrated in the images attached)
5. The Game ends when all tiles are green at which point the player has won, or when the player runs out of moves

Subtleties : 
1. The words can have repeated letters but in cases of conflict green is given priority (better to avoid these things in 
the word list )
2. In thory any number of attempts can be given but for the sake of realism and the grid looking decent, the attempts have
been restricted between 5 to 10. ( If the standard is not followed, the code will give abort, dump the core due to assertion
statement inside the constructor )

Specifics of Classes : 
The Code uses four classes, working complementarily with each other 

1. class WordList -- used in main

Has data member which contains a list of strings (array of characters)

i. choose_word() : Randomly chooses a word out of the list

2. class Tracker -- used in Tile class

Has data members x,y containing the array indices of the current Tile

i. next_letter() : Moves the indices correspondingly to the next Tile (edge cases have been taken care of)
ii. get_x() : returns the x value of the tile
iii. get_y() : returns the y value of the tile
iv. prev_letter() : reverses effect of next letter
3. class Tile -- Lenghiest abstraction in the program, used in Wordle

Has data members : 
i. char* letter -- letter inside the tile
ii. int result -- if letter is wrong:1 , correct:1, wrong position: 0
iii. int size -- size of the tile
iv. Wordle *wordle -- back-pointer
v. int x, y, w, h -- x,y indices, width, height
vi. int code -- 1=correct position as well as letter, 2 = correct letter, 3 = neither

Has member functions : 

i. Tile():Fl_Widget(0, 0 ,0 ,0){} -- Default constructor of this class
ii. Tile (Wordle *wordle, int size, int x, int y, int w, int h) : Fl_Widget(x, y, w, h) -- Constructor used in
Wordle for constructing tiles using pointers ( allocating memory on the heap )
iii. void check() -- to be used by a tile on the row next to the one being checked, gives appropriate colors to 
the tiles, and gives the appropriate codes to the tiles (again, edge cases are taken care of)
iv. void draw() -- responsible for drawing the widget upon initialization
v. void set_letter(const char* letter) -- sets the letter the first time key is pressed using the tile object
vi. void remove_letter() -- used with backspace in handle, removes the letter if the user has made a mistake in
entering the choice of letter .
vii. void set_color(int color) -- changes the color of the tile corresponding to instructions via void check() 
and changes the letters to white AND animates the motion of reveal (using trigonometry)
viii. void set_code(int code) -- sets the code of the tile
ix. const char* get_letter() -- returns the letter corresponding to the tile
x. virtual int handle(int e) -- The most important function of this class . Takes keyboard inputs and regulates 
the entry of letters into tiles and the change of the corresponding colors when check() is called .
xi. ~Tile() -- Destructor of this class. Frees up the memory allocated on the heap
xii. void finish() : responsible for finishing animation. (hopping of tiles with joy)

4. class Wordle : The master of all classes 

Has data members;
i. const char * answer -- contains the answer of the wordle
ii. Tracker tracker -- tracks the current tile
iii. int attempts -- contains the number of attempts by the user
iv. Tile * tiles[5][10] -- array of tiles (2D)
v. int size -- vertical size of the grid
vi. bool finish -- checks if game is finished, once finished handle() of Tile class ensures no further keystrokes
are registered

Note : It is extremely important for the (attempt+1)th row's first element to be constructed on the heap otherwise 
it leads to segmentation faults (line 171) as the pointer may be in the read-only region of memory

Note : It is important for the declaration of Wordle class to be above definition of Tile and the declaration of Tile 
to be above declaration of Wordle and Wordle uses Tile array and Tile uses a back-pointer to Wordle

Has member functions : 

i. Wordle (int size, int attempts) -- constructor of the class : responsible for generalisability of code .
ii. void set_answer(const char* answer) -- sets the answer to the parameter of the function .
iii. ~Wordle() -- Destructor of the function, frees the memory allocated on the heap .

Learnings + Thought Process: 

1. The pointers and allocation on the heap using the "new" keyword was extremely important for this code (most time consuming
part )
2. I thought of the game as having various modules and made a class/abstraction for each module of the program 
3. Then , I put the abstractions into place, almost like a jigsaw puzzle (with a lot of troubleshooting on the way)
4. Finally I did a lot of fine tuning, making the game as close to the real version as possible, adding generalisability and 
flexibility making it modular and easy to use (just 2 parameters!)

