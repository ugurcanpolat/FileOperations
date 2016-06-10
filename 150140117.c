     /* ***************** */
    /*     HOMEWORK-4    */
   /*  	Ugurcan Polat   */
  /*	  150140117    */
 /*	polatu@itu.edu.tr */
/* ***************** */

#include <stdio.h> // FILE, fopen(), fclose(), fprintf(), 
				  // feof(), fgets(), sscanf(), printf()
#include <stdlib.h> // malloc(), realloc(), free(), atoi(), atof()
#include <math.h> // fabs(), pow()

// Default values for N, M and threshold to use if user doesn't enter
// these values. MAX_PEOPLE for N and M; DEFAULT_TRESH for threshold.
#define MAX_PEOPLE    1729 
#define DEFAULT_THRESH  20

// Struct stores ID, height, weight and BMI values 
struct person{ 
	int personId; 
	double height; 
	double weight; 
	double BMI;
};

// Prototypes
void read_from_input_file (FILE* input_file, struct person *personPtr);
void bmi_calculator (struct person *personPtr, double *bmi_diffs,
											   double threshold);
void sort_bmi (struct person *personPtr, double *bmi_diffs, int num_ppl);

int main(int argc, char **argv) {
	
	char* input_file_name  = "height_weight.txt";
	char* output_file_name = "output.txt";
	
	FILE* input_file;
	FILE* output_file;
	
	// Define and initialize N, M, threshold values by default values
	int counter, num_farthest = MAX_PEOPLE, num_ppl = MAX_PEOPLE;
	double threshold = DEFAULT_THRESH;
	double *bmi_diffs;
	
	// Define and allocate memory for struct pointer.
	// Initially allocate memory by MAX_PEOPLE. 
	struct person *personPtr;
	personPtr=(struct person*)malloc(MAX_PEOPLE * sizeof(struct person));
	
	// If arguments are passed while executing, use the arguments.
	if (argc > 1){
		// Use atoi() and atof() to convert passed strings 
		// to int and float. Arguments: N, M and threshold values. 
		num_ppl = atoi(*(argv+1));
		printf("\nNum of people(N)   : %d\n", num_ppl);
		
		if (argc > 2) {
			num_farthest = atoi(*(argv+2));
			printf("Num of farthest(M) : %d \n", num_farthest);
		}
		// If M value is not passed by user print default value
		else 
			printf("Num of farthest(M) : %d (Default)\n", num_farthest);
			
		if (argc > 3) {
			threshold = atof(*(argv+3));
			printf("Threshold value    : %g\n\n", threshold);
		}
		// If threshold value is not passed by user print default value
		else 
			printf("Threshold value    : %g (Default)\n\n", threshold);
			
		// Re-allocate struct pointer to use just enough memory 
		personPtr = (struct person*)realloc(personPtr, 
											num_ppl * sizeof(struct person)
										    );							
	} // end if
	
	// If there are no passed arguments, print default values.
	else {
		printf("\nUsing default values... \n");
		printf("Num of people(N)   : %d\n", num_ppl);
		printf("Num of farthest(M) : %d\n", num_farthest);
		printf("Threshold value    : %g\n\n", threshold);
	}
	
	printf("Opening input file  : \"%s\" ...\n", input_file_name);
	printf("Opening output file : \"%s\" ...\n", output_file_name);
	
	// Check if input_file and output_file can be opened. 
	// Otherwise, give error. 
	if(!(input_file = fopen(input_file_name, "r"))) {
		printf("\nCannot open %s\n", input_file_name);
		return -1;
	} // end if
	
	if(!(output_file = fopen(output_file_name, "w"))) {
		printf("\nCannot open %s\n", output_file_name);
		return -1;
	} // end if
	
	printf("Successfully opened!\n\n");
	
	// Allocate memory for bmi_diffs that will store absolute diffs 
	bmi_diffs = malloc(num_ppl * sizeof(double));
	
	printf("Reading data from input file and calculating BMI values...\n");
	// Initialize counter by 0										
	counter = 0; 
	// Read the first line of input_file to jump over table header 
	read_from_input_file (input_file, personPtr);
	// Read values from input_file until it reaches to end of file 
	// or num_ppl 
	while(!feof(input_file) && counter < num_ppl){
		// Read height and weight values from input file and
		// Assign these values to the relevant variables in the struct
		read_from_input_file (input_file, personPtr+counter);
		// Calculate the BMI value	
		bmi_calculator (personPtr+counter, bmi_diffs+counter, threshold);							
		// Increment counter by one 
		counter++;
	} // end while
	
	// Call sorting function to sort people by the absolute differences
	// between BMI value and threshold.
	sort_bmi(personPtr, bmi_diffs, num_ppl);
	
	printf("BMI values are successfully calculated and sorted!\n");
	
	// Print table header to the first line of the output file. 
	fprintf(output_file, "%9s %11s %11s %7s %19s\n", "Person_Id", 
													  "Height(cm)", 
													  "Weight(kg)", 
													  "BMI",
													  "|BMI-Threshold|");
		
	// Print the data of the people farthest from 'threshold' into 
	// output file 'output.txt' 		
	printf("\nWriting to output file...\n");							      
	for(counter = 0; counter < num_farthest; counter++){
		fprintf(output_file, "%7d %9g %11g %12.2f %13.2f \n", 
										(personPtr+counter)->personId, 
										(personPtr+counter)->height,
										(personPtr+counter)->weight,   
										(personPtr+counter)->BMI,
										*(bmi_diffs+counter) 
										);
	}
	
	// If output is written as expected, print success.
	if (counter == num_farthest)
		printf("Successfully written!\n");
	
	printf("\nClosing files...\n");
	// Close input and output files 
	fclose(input_file);
	fclose(output_file);
	
	// Free the memory used by struct pointer and array
	free(personPtr);
	free(bmi_diffs);
	
	printf("\nGoodbye!\n\n");
	return 0;
} // end main()

void read_from_input_file (FILE* input_file, struct person *personPtr) {
	char buffer[50];
	
	// Read a line from input_file and store it into buffer
	fgets(buffer, sizeof(buffer), input_file);
	
	// Assign the stored height and weight values to 
	// related variables in structure.
	sscanf(buffer, "%d %lf %lf", &personPtr->personId,
								 &personPtr->height, 
								 &personPtr->weight ); 
} // end read_from_input_file()

void bmi_calculator (struct person *personPtr, double *bmi_diffs, 
											   double threshold) {											
	// Convert the height value from (cm) to (m)
	double meter_height = personPtr->height / 100;

	// Calculate BMI value by the equation [weight/((height)^2)]
	personPtr->BMI = personPtr->weight / pow(meter_height, 2);	
	// Calculate the gap between threshold-BMI and get absolute of it
	*bmi_diffs = fabs(threshold - personPtr->BMI);	
} // end bmi_calculator()

void sort_bmi (struct person *personPtr, double *bmi_diffs, int num_ppl){
	int current, biggest, check, tempId, tempHeight, tempWeight;
	double tempBMI, tempDiff;
	
	for (current = 0; current < num_ppl; current++) {
		// Assume biggest value is the current value 
	    biggest = current;

	    for (check = current + 1; check < num_ppl; check++){
			// If a member greater than biggest BMI-diff value, 
			// change biggest counter.
	        if (*(bmi_diffs+check) > *(bmi_diffs+biggest))
				biggest = check;
	    } // end for
	    
	    // Swap current BMI-diff value with bigger one
	    tempDiff = *(bmi_diffs+current);
	    *(bmi_diffs+current) = *(bmi_diffs+biggest);
	    *(bmi_diffs+biggest) = tempDiff;
		
		// Swap current BMI regarding to BMI-diff sorting
	    tempBMI = (personPtr+current)->BMI;
	    (personPtr+current)->BMI  = (personPtr+biggest)->BMI;
	    (personPtr+biggest)->BMI = tempBMI;
	    
	    // Swap ID number regarding to BMI-diff sorting
	    tempId = (personPtr+current)->personId;
	    (personPtr+current)->personId  = (personPtr+biggest)->personId;
	    (personPtr+biggest)->personId = tempId;
	    
	    // Swap height regarding to BMI-diff sorting
		tempHeight = (personPtr+current)->height;
	    (personPtr+current)->height  = (personPtr+biggest)->height;
	    (personPtr+biggest)->height = tempHeight;
	    
	    // Swap weight regarding to BMI-diff sorting
	    tempWeight = (personPtr+current)->weight;
	    (personPtr+current)->weight  = (personPtr+biggest)->weight;
	    (personPtr+biggest)->weight = tempWeight;
	     
	} // end for
} // end selection_sort()
