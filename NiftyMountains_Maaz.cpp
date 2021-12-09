//Maaz Mazharul 	ICS4U   	Ms. Cullum  	24th September
//This program finds the lowest number of steps a person has to take to cross the Nifty Mountains
//Since the algorithm starts from the center and splits in two directions (left and right),
//the program goes through more pathways than a normal Greedy Walk algorithm would,  which in-turn means there is higher chance of finding a lower number

//listing up the libraries
#include <iostream>
#include <vector>
#include <fstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>

#define COLUMNS 844
#define ROWS 480

//initializing functions
int initializeAllegro(int width, int height, const char title[]);
int findMin(int matrixVal, int minVal);
int findMax(int matrixVal, int maxVal);
int mapDataDrawer(std::ifstream &fname, int map1);
void mapDrawer(int matrixVal, int minVal, int colFactor, int j, int i);
void drawLowestElevPath(int i, int iCopy, int rightRow, int leftRow, ALLEGRO_COLOR RED);
bool escapeKeypress();
int findColorFactor(int maxValue, int minValue);

//allegro stuff
ALLEGRO_DISPLAY *display;
ALLEGRO_TIMER *timer;
ALLEGRO_FONT *arial;
ALLEGRO_FONT *garamond;
ALLEGRO_EVENT_QUEUE *event_queue;

using namespace std;

int main(int argc, char *argv[]){

   //initialize video mode to 844*480
	initializeAllegro(COLUMNS, ROWS, "Sample apmatrix");

	//initializes randomizer
	srand(time(NULL));

    vector < vector<int> > matrix(ROWS);
    for (int i = 0; i < matrix.size(); i++) {
        matrix[i] = vector<int>(COLUMNS);
    }
	int minValue = 100000000;
	int maxValue = 0;
	ALLEGRO_COLOR GREEN = al_map_rgb(0, 255, 0);    // Green
	ALLEGRO_COLOR RED  =  al_map_rgb(255, 0, 0);    // Red

	//failsafe for if the text file is not loaded
	ifstream fin("NiftyMountains.dat");


	//stores the data into an AP Matrix
	for(int i = 0; i < ROWS; i++){
    	for(int j = 0; j < COLUMNS; j++){
        	matrix[i][j] = mapDataDrawer(fin, matrix[i][j]);
    	}
	}

	//finds the minimum and maximum elevation to find the color factor
	for(int i = 0; i < ROWS; i++){
    	for(int j = 0; j < COLUMNS; j++){
        	minValue = findMin(matrix[i][j], minValue);
        	maxValue = findMax(matrix[i][j], maxValue);
    	}
	}

	//finds the color factor
	int colorFactor = findColorFactor(maxValue, minValue);

	//draws out the map on the allegro display
	for(int i = 0; i < ROWS; i++){
    	for(int j = 0; j < COLUMNS; j++){
        	mapDrawer(matrix[i][j], minValue, colorFactor, j, i);
    	}
	}
	al_flip_display();

	//variables to calculate the shortest elevation change
	int currentDist = 1000000;
	int totalDist = 10000000;
	//variables that store the upwards and downwards value as the line moves
	int rowPointRight;
	int rowPointLeft;
	//variables for drawing the lowest elevation in red
	int iCopy;
	vector<int> rowPointRightCopyTemp(COLUMNS/2);
	vector<int> rowPointLeftCopyTemp (COLUMNS/2);
	vector<int> rowPointRightCopy(COLUMNS/2);
	vector<int> rowPointLeftCopy (COLUMNS/2);
	int totalLeft;
	int totalRight;
	int q = 0;

	//Greedy Walk Algortihm that starts in the center
	for (int i = 0; i < ROWS - 1; i++){
      //algorithm for calculating the lowest elevation change and storing it
    	if(totalDist > currentDist){
        	totalDist = currentDist;
        	printf("The Shortest Total Distance is: %d\n", totalDist);
        	iCopy = i - 1;
        	q+=1;
        	for (int p = 0; p < (COLUMNS/2 - 1); p++){
            	rowPointLeftCopy[p] = rowPointLeftCopyTemp[p];
            	rowPointRightCopy[p] = rowPointRightCopyTemp[p];
        	}
    	}
    	//resetting stuff when the row number changes
    	currentDist = 0;
    	rowPointRight = 0;
    	rowPointLeft = 0;
    	totalLeft = 0;
    	totalRight = 0;
    	al_draw_filled_rectangle(COLUMNS/2, i, COLUMNS/2 - 1, i + 1, GREEN);
    	for (int j = 0; j < (COLUMNS/2 - 1); j++){
            //draws the green pixels
        	al_draw_filled_rectangle(COLUMNS/2 + j, i + rowPointRight, COLUMNS/2 + j + 1, i + rowPointRight + 1, GREEN);
        	al_draw_filled_rectangle(COLUMNS/2 - j - 1, i + rowPointLeft, COLUMNS/2 - j - 1 - 1, i + rowPointLeft + 1, GREEN);

        	//algorithm starts off by storing the middle section as the lowest
        	int shortDistRight = abs(matrix[i + rowPointRight][COLUMNS/2 + j] - matrix[i + rowPointRight][COLUMNS/2 + j + 1]);
        	int shortDistLeft = abs(matrix[i + rowPointLeft][COLUMNS/2 - j - 1] - matrix[i + rowPointLeft][COLUMNS/2 - j - 1 - 1]);

        	//goes in this if statement if there're 3 numbers to compare from instead of two
        	if((i + rowPointRight > 0) && ( i + rowPointRight < 479) ){
                //if top < middle
            	if(shortDistRight > abs(matrix[i + rowPointRight][COLUMNS/2 + j] - matrix[i - 1 + rowPointRight][COLUMNS/2 + j + 1])){
                    shortDistRight = abs(matrix[i + rowPointRight][COLUMNS/2 + j] - matrix[i - 1 + rowPointRight][COLUMNS/2 + j + 1]);
                	rowPointRight -= 1;
                	if(i + rowPointRight < 479){
                        //if bot < top
                    	if(shortDistRight > abs(matrix[i + rowPointRight + 1][COLUMNS/2 + j] - matrix[i + 1 + rowPointRight + 1][COLUMNS/2 + j + 1])){
                        	shortDistRight = abs(matrix[i + rowPointRight + 1][COLUMNS/2 + j] - matrix[i + 1 + rowPointRight + 1][COLUMNS/2 + j + 1]);
                        	rowPointRight += 2;
                    	}
                    	//if bot = top, it randomizes
                    	else if(shortDistRight == abs(matrix[i + rowPointRight + 1][COLUMNS/2 + j] - matrix[i + 1 + rowPointRight + 1][COLUMNS/2 + j + 1])){
                        	if(rand()%2 == 0){
                            	shortDistRight = abs(matrix[i + rowPointRight + 1][COLUMNS/2 + j] - matrix[i + 1 + rowPointRight + 1][COLUMNS/2 + j + 1]);
                            	rowPointRight += 2;
                        	}
                        	else{
                            	shortDistRight = abs(matrix[i + rowPointRight + 1][COLUMNS/2 + j] - matrix[i + 1 + rowPointRight - 1][COLUMNS/2 + j + 1]);
                        	}
                    	}
                	}
            	}
            	//if top > middle AND bot < middle
            	else if(shortDistRight > abs(matrix[i + rowPointRight][COLUMNS/2 + j] - matrix[i + 1 + rowPointRight][COLUMNS/2 + j + 1])){
                	shortDistRight = abs(matrix[i + rowPointRight][COLUMNS/2 + j] - matrix[i + 1 + rowPointRight][COLUMNS/2 + j + 1]);
                	rowPointRight += 1;
            	}
        	}
        	//if there're only middle and bot to compare from
        	else if(i + rowPointRight == 0){
                //if bot < middle
            	if(shortDistRight > abs(matrix[i + rowPointRight][COLUMNS/2 + j] - matrix[i + 1 + rowPointRight][COLUMNS/2 + j + 1])){
                	shortDistRight = abs(matrix[i + rowPointRight][COLUMNS/2 + j] - matrix[i + 1 + rowPointRight][COLUMNS/2 + j + 1]);
                	rowPointRight += 1;
            	}
        	}
        	//if there're only middle and top to compare from
        	else if(i + rowPointRight == 479){
                //if top < middle
            	if(shortDistRight > abs(matrix[i + rowPointRight][COLUMNS/2 + j] - matrix[i - 1 + rowPointRight][COLUMNS/2 + j + 1])){
                	shortDistRight = abs(matrix[i + rowPointRight][COLUMNS/2 + j] - matrix[i - 1 + rowPointRight][COLUMNS/2 + j + 1]);
                	rowPointRight -= 1;
            	}
        	}
        	//making sure it picks one of the above
        	else{
            	return -1;
        	}
        	//THIS SECTION OF CODE IS BASICALLY THE ABOVE PORTION, BUT INSTEAD OF GOING RIGHT, THE ALGORITH GOES LEFT
        	if((i + rowPointLeft > 0) && (i + rowPointLeft < 479)){
            	if(shortDistLeft > abs(matrix[i + rowPointLeft][COLUMNS/2 - j - 1] - matrix[i - 1 + rowPointLeft][COLUMNS/2 - j - 1 - 1])){
                	shortDistLeft = abs(matrix[i + rowPointLeft][COLUMNS/2 - j - 1] - matrix[i - 1 + rowPointLeft][COLUMNS/2 - j - 1 - 1]);
                	rowPointLeft -= 1;
                	if(i + rowPointLeft < 479){
                    	if(shortDistLeft > abs(matrix[i + rowPointLeft +1][COLUMNS/2 - j - 1] - matrix[i + 1 + rowPointLeft + 1][COLUMNS/2 - j - 1 - 1])){
                        	shortDistLeft = abs(matrix[i + rowPointLeft + 1][COLUMNS/2 - j - 1] - matrix[i + 1 + rowPointLeft + 1][COLUMNS/2 - j - 1 - 1]);
                        	rowPointLeft += 2;
                    	}
                    	else if(shortDistLeft == abs(matrix[i + rowPointLeft +1][COLUMNS/2 - j - 1] - matrix[i + 1 + rowPointLeft + 1][COLUMNS/2 - j - 1 - 1])){
                        	if(rand()%2 == 0){
                            	shortDistLeft =abs(matrix[i + rowPointLeft +1][COLUMNS/2 - j - 1] - matrix[i + 1 + rowPointLeft + 1][COLUMNS/2 - j - 1 - 1]);
                            	rowPointLeft += 2;
                        	}
                        	else{
                            	shortDistLeft = abs(matrix[i + rowPointLeft +1][COLUMNS/2 - j - 1] - matrix[i + 1 + rowPointLeft - 1][COLUMNS/2 - j - 1 - 1]);
                        	}
                    	}
                	}
            	}
            	else if(shortDistLeft > abs(matrix[i + rowPointLeft][COLUMNS/2 - j - 1] - matrix[i + 1 + rowPointLeft][COLUMNS/2 - j - 1 - 1])){
                	shortDistLeft = abs(matrix[i + rowPointLeft][COLUMNS/2 - j - 1] - matrix[i + 1 + rowPointLeft][COLUMNS/2 - j - 1 - 1]);
                	rowPointLeft += 1;
            	}
        	}
        	else if(i + rowPointLeft == 0){
            	if(shortDistLeft > abs(matrix[i + rowPointLeft][COLUMNS/2 - j - 1] - matrix[i + 1 + rowPointLeft][COLUMNS/2 - j - 1 - 1])){
                	shortDistLeft = abs(matrix[i + rowPointLeft][COLUMNS/2 - j - 1] - matrix[i + 1 + rowPointLeft][COLUMNS/2 - j - 1 - 1]);
                	rowPointLeft += 1;
            	}
        	}
        	else if(i + rowPointLeft == 479){
            	if(shortDistLeft > abs(matrix[i + rowPointLeft][COLUMNS/2 - j - 1] - matrix[i - 1 + rowPointLeft][COLUMNS/2 - j - 1 - 1])){
                	shortDistLeft = abs(matrix[i + rowPointLeft][COLUMNS/2 - j - 1] - matrix[i - 1 + rowPointLeft][COLUMNS/2 - j - 1 - 1]);
                	rowPointLeft -= 1;
            	}
        	}
        	else{
            	return 1;
        	}
        	//calculates the total elevation change after each move
        	totalLeft += shortDistLeft;
        	totalRight += shortDistRight;
        	currentDist = totalRight + totalLeft;
        	//stores the value of pointer change for the drawing of the red line
        	rowPointRightCopyTemp[j] = rowPointRight;
        	rowPointLeftCopyTemp[j] = rowPointLeft;
    	}
    	al_flip_display();
    	currentDist += (abs(matrix[i][COLUMNS/2 - 1] - matrix[i][COLUMNS/2]));
	}
	printf("The least amount of distance required to cross the mountains is on row %d, with the distance travelled of: %d", q, totalDist);

	//draws the red line to indicate the location of the shortest distance
	al_draw_filled_rectangle(COLUMNS/2 - 3, iCopy , COLUMNS/2 + 3, iCopy + 1, RED);
	for(int i = 0; i < COLUMNS/2; i++){
    	drawLowestElevPath(i, iCopy, rowPointRightCopy[i], rowPointLeftCopy[i], RED);
	}
	//closes the allegro display when Esc key is pressed
	while(!escapeKeypress()){
        al_rest(1);
	}
	return 0;
}

int initializeAllegro(int width, int height, const char title[]) {

	const float FPS = 16;   		 // set frame rate
	ALLEGRO_COLOR background = al_map_rgb(255, 255, 255);   	// make background white.

	// Initialize Allegro
    al_init();

    // initialize display
    display = al_create_display(width, height);
    if (!display) {
   	 al_show_native_message_box(display, "Error", "Error", "Failed to initialize display!",
                             	nullptr, ALLEGRO_MESSAGEBOX_ERROR);
  		 return -1;
    }
    al_set_window_title(display, title);

  	 // Initialize keyboard routines
    if (!al_install_keyboard()) {
    	al_show_native_message_box(display, "Error", "Error", "failed to initialize the keyboard!",
                             	nullptr, ALLEGRO_MESSAGEBOX_ERROR);
		 return -1;
  	 }

    // need to add image processor
	 if (!al_init_image_addon()) {
   	 al_show_native_message_box(display, "Error", "Error", "Failed to initialize image addon!",
                                nullptr, ALLEGRO_MESSAGEBOX_ERROR);
   	 return -1;
    }
    // 2. setup timer
    timer = al_create_timer(1.0 / FPS);
  	 if (!timer) {
  		 al_show_native_message_box(display, "Error", "Error", "Failed to create timer!",
                             	nullptr, ALLEGRO_MESSAGEBOX_ERROR);
    	return -1;
	}
	// Add fonts


   al_init_font_addon(); // initialize the font addon
   al_init_ttf_addon();// initialize the ttf (True Type Font) addon

   arial = al_load_ttf_font("C:/Windows/Fonts/arial.ttf", 16, 0);
   if (!arial){
  	al_show_native_message_box(display, "Error", "Error", "Could not load arial.ttf",
                                	nullptr, ALLEGRO_MESSAGEBOX_ERROR);
  	return -1;
   }
   garamond = al_load_ttf_font("C:/Windows/Fonts/gara.ttf", 16, 0);
   if (!garamond){
  	al_show_native_message_box(display, "Error", "Error", "Could not load garamond.ttf",
                                	nullptr, ALLEGRO_MESSAGEBOX_ERROR);
  	return -1;
   }

	// Initialize primative add on
	 if (!al_init_primitives_addon()) {
   	 al_show_native_message_box(display, "Error", "Error", "Failed to initialize primatives addon!",
                             	nullptr, ALLEGRO_MESSAGEBOX_ERROR);
   	 return -1;
    }

    // set up event queue
    event_queue = al_create_event_queue();
    if (!event_queue) {
   	 al_show_native_message_box(display, "Error", "Error", "Failed to create event_queue!",
                             	nullptr, ALLEGRO_MESSAGEBOX_ERROR);
   	 al_destroy_display(display);
 		 return -1;
    }

    al_register_event_source(event_queue, al_get_display_event_source(display));
	 al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));    	 // 3. register timer events

    al_clear_to_color(background);

    al_flip_display();
    //al_start_timer(timer);
//    std::cout << "Init return zero " << std::endl;
    return 0;
}

int findMin(int matrixVal, int minVal){
	if(matrixVal < minVal){
    	minVal = matrixVal;
	}
	return minVal;
}
int findMax(int matrixVal, int maxVal){
	if(matrixVal > maxVal){
    	maxVal = matrixVal;
	}
	return maxVal;
}

int mapDataDrawer(std::ifstream &fname, int map1){
    if (!fname) {
    	cout << "NiftyMountains.dat not found" << endl;
    	return 404;
	}

	fname >> map1;
	return map1;
}

void mapDrawer(int matrixVal, int minVal, int colFactor, int j, int i){
	int color;
	color = ((matrixVal - minVal) / colFactor);
	ALLEGRO_COLOR drawColor = al_map_rgb(color, color, color);
	al_draw_filled_rectangle(j, i, j+1, i+1, drawColor);
}

void drawLowestElevPath(int i, int iCopy, int rightRow, int leftRow, ALLEGRO_COLOR RED){
    	al_draw_filled_rectangle(COLUMNS/2 + i + 1, iCopy + rightRow, COLUMNS/2 + i + 1 + 1, iCopy + rightRow + 1, RED);
    	al_draw_filled_rectangle(COLUMNS/2 - i - 1 - 1, iCopy + leftRow, COLUMNS/2 - i - 1 - 1 - 1, iCopy + leftRow + 1, RED);
    	al_flip_display();

}

bool escapeKeypress() {

	ALLEGRO_KEYBOARD_STATE key_state;
	ALLEGRO_EVENT ev;

    al_wait_for_event(event_queue, &ev);
    if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        	return true;
    }
	al_get_keyboard_state(&key_state);
	return al_key_down(&key_state, ALLEGRO_KEY_ESCAPE);
}

int findColorFactor(int maxValue, int minValue){
    return ((maxValue - minValue) / 256) + 1;
}

void greedyPathAlgo(){

}
