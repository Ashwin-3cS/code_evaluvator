#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Function to compare two files
int compareFiles(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");
    if (!f1 || !f2) {
        perror("Error opening files for comparison");
        return 0;
    }
    char c1, c2;
    while ((c1 = fgetc(f1)) != EOF && (c2 = fgetc(f2)) != EOF) {
        if (c1 != c2) {
            fclose(f1);
            fclose(f2);
            return 0; // Files are different
        }
    }
    fclose(f1);
    fclose(f2);
    return 1; // Files are identical
}

// Function to display compilation errors
void displayCompilationErrors(const char *errorFile) {
    FILE *file = fopen(errorFile, "r");
    if (!file) {
        perror("Error opening compile_errors.txt");
        return;
    }
    char line[256];
    printf("Compilation Errors:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }
    fclose(file);
}

// Factorial computation function (to be implemented by students)
int factorial(int n) {
    if (n == 0 || n == 1) return 1;
    return n * factorial(n - 1);
}

int main() {
    char *studentCode = "student_code.c"; // Input student code file
    char *executable = "student_code.out"; // Output executable
    char *testInput = "test_input.txt";    // Test case input file
    char *expectedOutput = "expected_output.txt"; // Expected output file
    char *actualOutput = "actual_output.txt"; // Actual output file generated
    char *errorFile = "compile_errors.txt"; // File to store compilation errors

    // Step 1: Compile the student code
    printf("Compiling student code...\n");
    char compileCommand[256];
    // sprintf(compileCommand, "gcc %s -o %s >> %s", studentCode, executable, errorFile);
    sprintf(compileCommand, "gcc -o %s %s 2> %s", executable, studentCode, errorFile);
    int compileStatus = system(compileCommand);

    if (compileStatus != 0) {
        printf("Compilation failed.\n");
        displayCompilationErrors(errorFile);
        return 1;
    }
    printf("Compilation successful!\n");

    // Step 2: Execute the compiled program with test inputs
    printf("Executing test cases...\n");

    FILE *testCases = fopen("test_input.txt", "r");
    if (!testCases) {
        perror("Error opening test_input.txt");
        return 1;
    }

    char input[256];
    char expectedOutputLine[256];
    int testCaseNumber = 1;
    int allPassed = 1;

    while (fscanf(testCases, "%s %s", input, expectedOutputLine) != EOF) {
        printf("\n--- Test Case %d ---\n", testCaseNumber);

        // Write input to test_input.txt
        FILE *inputFile = fopen(testInput, "w");
        fprintf(inputFile, "%s\n", input);
        fclose(inputFile);

        // Write expected output to expected_output.txt
        FILE *outputFile = fopen(expectedOutput, "w");
        fprintf(outputFile, "%s\n", expectedOutputLine);
        fclose(outputFile);

        char executeCommand[256];
        // sprintf(executeCommand, "%s < %s > %s", executable, testInput, actualOutput);
// In code_evaluvator.c, change this line:
        sprintf(executeCommand, "./%s < %s > %s", executable, testInput, actualOutput);
        // Measure execution time
        clock_t start = clock();
        int executeStatus = system(executeCommand);
        clock_t end = clock();

        if (executeStatus != 0) {
            printf("Execution failed with status %d.\n", executeStatus);
            allPassed = 0;
            continue;
        }

        // Compare actual output with expected output
        if (compareFiles(expectedOutput, actualOutput)) {
            printf("Test case passed! Output is correct.\n");
        } else {
            printf("Test case failed. Output is incorrect.\n");
            allPassed = 0;
        }

        // Evaluate execution time
        double executionTime = ((double)(end - start)) / CLOCKS_PER_SEC;
        printf("Execution Time: %.5f seconds\n", executionTime);

        testCaseNumber++;
    }

    fclose(testCases);

    // Step 4: RibbonCI Summary
    printf("\n--- RibbonCI Summary ---\n");
    if (allPassed) {
        printf("All test cases passed! \n");
    } else {
        printf("Some test cases failed. \n");
    }

    // Clean up
    printf("Cleaning up...\n");
    remove(executable);
    remove(testInput);
    remove(expectedOutput);
    remove(actualOutput);

    return 0;
}