////////////////////////////////////////////////////////////////////////////////////////////
/*
  Algorithms and Data Structures
  Recursive Algorithms Assigment
  Contributors:
  Muhammed Suwaneh
  Department of Computer Engineering 
  Eskisehir Osmangazi University, October 2020
 */
////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <string.h>
#include <windows.h>

// can be changed 
#define MAX_SIZE 50 

class Blobs {
private:

	std::ifstream dataFile;
	std::string filename;
	char** blobs;
	int* pixels;
	int* blob_count;
	int rows, cols;
	int num_of_blobs;
	double* comX;
	double* comY;
public:
	Blobs();
	~Blobs();
	void readFile();
	void display_Matrix();
	void find_blobs();
	int count_pixels(int, int);
	void display_results();
	void center_of_mass(int, double&, double&);
};

// start main
int main() {

	// control console size
	HWND console = GetConsoleWindow();
	RECT ConsoleRect;
	GetWindowRect(console, &ConsoleRect);
	MoveWindow(console, ConsoleRect.left, ConsoleRect.top, 1000, 650, 1);

	// set precision for floating numbers
	std::cout.setf(std::ios::fixed);
	std::cout.setf(std::ios::showpoint);
	std::cout.precision(2);

	Blobs* blob = new Blobs();

	// obtain data from file
	blob->readFile();

	// output the obtained data in matrix form
	blob->display_Matrix();

	// find blobs and compute pixels of each
	blob->find_blobs();

	// display pixels, comRow, comCol
	blob->display_results();

	// release memory allocated for blob object
	delete blob;
	std::cout << std::endl << std::endl;
	return 0;
}

// end main

////////////////////////////////////////////////////////////////////////////////////////////

// initializes blob, pixels, buffer arrays and filename

////////////////////////////////////////////////////////////////////////////////////////////

Blobs::Blobs() {
   
	// allocate memory to file Matrix
	blobs = new char*[MAX_SIZE];
	blob_count = new int[MAX_SIZE];
	pixels = new int[MAX_SIZE];
	comX = new double[MAX_SIZE];
	comY = new double[MAX_SIZE];

	for (int i = 0; i < MAX_SIZE; i++) {

		blobs[i] = new char[MAX_SIZE];
	}

	// set default filename 

	filename = "blobs1.txt";

	for (int i = 0; i < MAX_SIZE; i++) {

		for (int j = 0; j < MAX_SIZE; j++) {

			blobs[i][j] = ' ';
		}
	}
}


// end Blob constructor

////////////////////////////////////////////////////////////////////////////////////////////

// reads blob data from a .txt file

////////////////////////////////////////////////////////////////////////////////////////////

void Blobs::readFile() {

	// get filename from user
	std::cout << "Please enter filename: ";
	std::cin >> filename;

	// attempt file ios::in operation
	dataFile.open(filename, std::ios::in);

	std::cout << std::endl;

	if (!dataFile) {

		std::cout << "File failed to open" << std::endl;
		exit(-1);
	}

	else {

		std::cout << "File opened successfully" << std::endl;

		int i = 0, j, counter = 0;
		std::string buffer;

		// obtain coordinates 
		dataFile >> rows >> cols;

		// move cursor to next line 
		std::getline(dataFile, buffer);

		// read pixels
		 while (!dataFile.eof()) {

			std::getline(dataFile, buffer);

			// string for implementing noskipws function
			std::istringstream iss(buffer);

			iss.seekg(0);
	
			for (j = 0; j < cols; j++) {

				// avoid skipping spaces
				iss >> std::noskipws >> blobs[i][j];
			} 
			i++;
		}
	}

	// close file 
	dataFile.close();
}

// end readFile

////////////////////////////////////////////////////////////////////////////////////////////

// dislays the obtained blob matrix

////////////////////////////////////////////////////////////////////////////////////////////


void Blobs::display_Matrix() {

	// display Matrix
	std::cout << std::endl << "BLOBS:" << std::endl;

	// create matrix frame
	std::cout << std::left << std::setw(2) << " ";
	int count = 0;

	// output num frame
	for (int i = 0; i < cols; i++) {

		std::cout << count;
		count++;

		if (count > 9) {

			count = 0;
		}
	}

	// output character frame
	std::cout << std::endl <<" +";

	for (int i = 0; i < cols; i++) {

		 std::cout << "-";
	}

	std::cout << "+" << std::endl;

	for (int i = 0; i < rows; i++) {

		std::cout << i << "|";

		for (int j = 0; j < cols; j++) {

			std::cout << blobs[i][j];
		}
		std::cout << "|" << i;
		std::cout << std::endl;
	}

	std::cout << " +";

	for (int i = 0; i < cols; i++) {

		std::cout << "-";
	}

	std::cout << "+" << std::endl;
	std::cout << std::left << std::setw(2) << " ";

	count = 0;

	for (int i = 0; i < cols; i++) {

		std::cout << count;
		count++;

		if (count > 9) {

			count = 0;
		}
	}

	std::cout << std::endl << std::endl;
}

// end display_Matrix

////////////////////////////////////////////////////////////////////////////////////////////

// obtains the blobs and pixel counts

////////////////////////////////////////////////////////////////////////////////////////////

void Blobs::find_blobs() {

	int counter = 0, sum_of_pixels;
	double comx, comy;

	// find blobs and count pixels
	for (int i = 0; i < rows; i++) {

		for (int j = 0; j < cols; j++) {

			sum_of_pixels = count_pixels(i, j);

			if (sum_of_pixels > 0) {

				center_of_mass(sum_of_pixels, comx, comy);
				pixels[counter] = sum_of_pixels;
				comX[counter] = comx;
				comY[counter] = comy;
				counter++;	 
			}
		}	
	}

	num_of_blobs = counter;
}

// end find_blobs()


////////////////////////////////////////////////////////////////////////////////////////////

// counts number of pixels in each blob

////////////////////////////////////////////////////////////////////////////////////////////

int Blobs::count_pixels(int r, int c) {

	int sum = NULL, count;

	if (r < 0 || r >= rows || c < 0 || c >= cols) {
		return 0;
	}
	else if (blobs[r][c] != 'x') {
		return 0;
	}
	else {
		// mark visited
		blobs[r][c] = '#';
		sum = 1 +  count_pixels(r, c + 1) +
			count_pixels(r, c - 1) +
			count_pixels(r - 1, c) +
			count_pixels(r + 1, c);
		   return sum;
	}
}

// end count_pixels

////////////////////////////////////////////////////////////////////////////////////////////

// computes the center of mass of each blob

////////////////////////////////////////////////////////////////////////////////////////////

void Blobs::center_of_mass(int sum, double& comx, double& comy) {

	double countx = 0, county = 0;
	double sumx = 0, sumy = 0;

	// compute total masses
	for (int i = 0; i < rows; i++) {

		for (int j = 0; j < cols; j++) {

			if (blobs[i][j] == '#') {

				countx++;
				blobs[i][j] = '0';
			}
		}

		sumx += countx * i;
		countx = 0;
	}

	for (int i = 0; i < cols; i++) {

		for (int j = 0; j < rows; j++) {

			if (blobs[j][i] == '0') {

				county++;
				blobs[j][i] = ' ';
			}
		}

		sumy += county * i;
		county = 0;
	}

	// computer center of masses of xcor and ycor
	comx = sumx / sum;
	comy = sumy / sum;
}

// end center_of_mass

////////////////////////////////////////////////////////////////////////////////////////////

// displays results of all computed values

////////////////////////////////////////////////////////////////////////////////////////////
 

void Blobs::display_results() {

	std::string blob_number = "BLOB";
	std::string num_of_pixels = "NoOfPixels";
	std::string comRow = "comRow ";
	std::string comCol = "comCol";

	for (int i = 0; i < num_of_blobs; i++) {

		blob_count[i] = i + 1;
	}

	std::cout << std::endl << "RESULTS:" << std::endl;

	std::cout << "+------------+------------+------------+------------+" << std::endl;
	std::cout << "| " << std::left << std::setw(10) << blob_number << " | ";
	std::cout << std::left << std::setw(10) << num_of_pixels << " | ";
	std::cout << std::left << std::setw(10) << comRow << " | ";
	std::cout << std::left << std::setw(10) << comCol << " |";
	std::cout << std::endl << "+------------+------------+------------+------------+" << std::endl;

	for (int i = 0; i < num_of_blobs; i++) {

		std::cout << "| " << std::left << std::setw(10) << blob_count[i] << " |";
		std::cout << " " << std::left << std::setw(10) << pixels[i] << " |";
		std::cout << " " << std::left << std::setw(10) << comX[i] << " |";
		std::cout << " " << std::left << std::setw(10) << comY[i] << " |";
		std::cout << std::endl;
	}
	std::cout << "+------------+------------+------------+------------+" << std::endl;

}
// end display results

////////////////////////////////////////////////////////////////////////////////////////////

// releases allocated memory

////////////////////////////////////////////////////////////////////////////////////////////

Blobs :: ~Blobs() {

	// release allocated memory
    delete[] pixels;
	delete[] blob_count;
	delete[] comX;
	delete[] comY;
	delete[] blobs;
}

// end Destructor
